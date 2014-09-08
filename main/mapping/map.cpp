#include "map.hpp"

#include "../objects/Human.hpp"
#include "../objects/Zombie.hpp"
#include "../objects/Infected.hpp"

Map::Map(){
	NEXT_EMPTY_ID = 0;

	empty_object = pObject(Pair<int>(-1,-1));
	EMPTY_BLOCK = ObjectBlock(EMPTY_TYPE_ID, EMPTY);
	IDhash[EMPTY_TYPE_ID] = &empty_object;
	NEXT_EMPTY_ID++;

	xmap_len = Map_settings::map_len.x;
	ymap_len = Map_settings::map_len.y;

	blockmap = new ObjectBlock***[xmap_len];
	for(int i = 0; i < xmap_len; i++){

		blockmap[i] = new ObjectBlock**[ymap_len];
		for(int j = 0; j < ymap_len; j++){

			blockmap[i][j] = new ObjectBlock*[2];
			for(int k = 0; k < 2; k++){
				blockmap[i][j][k] = &EMPTY_BLOCK;

			}
		}
	}

	regionalize();
	populate();
}

Map::~Map(){
	// delete regions
	for(int i = 0; i < num_x_regions; i++){
		for(int j = 0; j < num_y_regions; j++){
			delete regions[i][j];
		}
		delete [] regions[i];
	}
	delete [] regions;

	// delete map
	for(int i = 0; i < xmap_len; i++){
		for(int j = 0; j < ymap_len; j++){
			delete [] blockmap[i][j];
		}
		delete [] blockmap[i];
	}
	delete [] blockmap;

	// delete object_blocks we made for the map
	for(auto it = all_object_blocks.begin(); it != all_object_blocks.end(); it++){
		delete *it;
	}
	all_object_blocks.clear();

	// delete all alloc'd characters
	for(auto it = characters.begin(); it != characters.end(); it++){
		delete *it;
	}
	characters.clear();


	IDhash.clear();

	NEXT_EMPTY_ID = 0;
}

// return current stats, e.g. for GUI
MapStats Map::get_map_stats(){	return map_stats;  }

// game is running only if there are more than 0 humans left
bool Map::check_game(){ return(  map_stats.num_characters[HUMAN] ); }

// play out an entire turn in the map
void Map::iterate(){
	// loop through characters and execute turn actions
	for(list<Character*>::iterator it = characters.begin(); it != characters.end(); it++){
		(*it)->exec();

		// convert infected
		if((*it)->get_symbol() == INFECTED and ((Infected*) *it)->infected_time_over()){
			Pair<int> coor = (*it)->get_coor();
			int ID = get_ID(coor);

			delete_character(*(it++), ID); // have to ++ here because we're iterating over the list we're altering
			add_character(ZOMBIE, coor);
		}
	}

	map_stats.num_iter++;
}

Map_symbol Map::get_symbol(Pair<int> coor){ return get_symbol(coor.x, coor.y); }
Map_symbol Map::get_symbol(int xcoor, int ycoor){
	if(overall_bounds_check(xcoor, ycoor)){
		return(blockmap[xcoor][ycoor][TOP_LAYER]->symbol);
	}

	return(INVALID);
}

int Map::get_ID(Pair<int> coor){ return (blockmap[coor.x][coor.y][TOP_LAYER]->ID); }
int Map::get_ID(int xcoor, int ycoor){ return (blockmap[xcoor][ycoor][TOP_LAYER]->ID); }


void Map::add_block(ObjectBlock* block, Pair<int> new_loc){
	blockmap[new_loc.x][new_loc.y][BOTTOM_LAYER] = blockmap[new_loc.x][new_loc.y][TOP_LAYER];
	blockmap[new_loc.x][new_loc.y][TOP_LAYER] = block;
}

void Map::remove_block(Pair<int> coor){
	if(blockmap[coor.x][coor.y][BOTTOM_LAYER] != &EMPTY_BLOCK){
		blockmap[coor.x][coor.y][TOP_LAYER] = blockmap[coor.x][coor.y][BOTTOM_LAYER];
		blockmap[coor.x][coor.y][BOTTOM_LAYER] = &EMPTY_BLOCK;
	}
	else{
		blockmap[coor.x][coor.y][TOP_LAYER] = &EMPTY_BLOCK;
	}
}

Character* Map::new_character_instance(Map_symbol sym, Pair<int> coor){
		switch(sym){
		case HUMAN:
			return new Human(coor);
			break;
		case ZOMBIE:
			return new Zombie(coor);
			break;

		return(NULL);
		}
}

// TODO: map_symbols to char symbols
void Map::add_character(Map_symbol type, Pair<int> coor){
	// perhaps put stats in char factory?
	// TODO: change mapstats to its own charfactory class
	Character* c = new_character_instance(type, coor);

	Pair<int> reg_coor = find_region(coor);

	if(overall_bounds_check(coor) && regional_bounds_check(reg_coor)){
		add_block(new_object_block(NEXT_EMPTY_ID, c->get_symbol()), coor);
		regions[reg_coor.x][reg_coor.y]->insert_character(c);
	}
	// else make a debug printout and print to this...

	IDhash[NEXT_EMPTY_ID++] = c;
	characters.push_front(c);
	map_stats.num_characters[type]++;
}

void Map::delete_character(pObject* c, int ID){
	Character* dead_char = static_cast<Character*>(c);
	Pair<int> coor = c->get_coor();
	Pair<int> reg_coor = find_region(coor);

	// this remove takes O(n), can we make this better?
	characters.remove(dead_char);

	regions[reg_coor.x][reg_coor.y]->remove_character(dead_char);
	IDhash.erase(ID);

	remove_block(coor);

	map_stats.num_characters[dead_char->get_symbol()]--;

	delete c;
}

void Map::add_obstacle(Obstacle *o){
	Pair<int> coor = o->get_coor();
	Pair<int> dimens = o->get_dimensions();
	Pair<int> reg_coor = find_region(coor);
	Map_symbol next_sym = EMPTY;

	for(int x = 0; x < dimens.x; x++){
	for(int y = 0; y < dimens.y; y++){
		if(overall_bounds_check(coor + Pair<int>(x,y))){
			next_sym = o->get_footprint(x, y);
			if(next_sym != EMPTY){
				blockmap[coor.x + x][coor.y + y][TOP_LAYER] = new_object_block(NEXT_EMPTY_ID, next_sym);
			}
		}
	}}

	IDhash[NEXT_EMPTY_ID++] = o;

	if(regional_bounds_check(reg_coor)){
		regions[reg_coor.x][reg_coor.y]->insert_obstacle(o);
	}
}

ObjectBlock* Map::new_object_block(int ID, Map_symbol sym){
	ObjectBlock* newobj = new ObjectBlock(NEXT_EMPTY_ID, sym);
	all_object_blocks.push_front(newobj);
	return(newobj);
}

// TODO: change this to inline
Pair<int> Map::find_region(Pair<int> coor){
	return Pair<int>(coor.x / Map_settings::region_len, coor.y / Map_settings::region_len);
}

bool Map::overall_bounds_check(Pair<int> p){ return overall_bounds_check(p.x, p.y); }
bool Map::overall_bounds_check(int x, int y){
	if(x < 0 || y < 0 || x >= xmap_len || y >= ymap_len){
		return false;
	}

	return true;
}

bool Map::regional_bounds_check(Pair<int> p){ return regional_bounds_check(p.x, p.y); }
bool Map::regional_bounds_check(int x, int y){
	if(x < 0 || y < 0 || x >= num_x_regions || y >= num_y_regions){
		return false;
	}

	return true;
}

void Map::print_map(){
	cout << '#';
	for(int x = 0; x < Map_settings::map_len.y; x++){
		cout << '-';
	}
	cout << '#' << endl;

	for(int y = 0; y < Map_settings::map_len.x; y++){
		cout << '|';
		for(int x = 0; x < Map_settings::map_len.y; x++){
			cout << (char) blockmap[x][y][TOP_LAYER]->symbol;
		}
		cout << '|' << endl;
	}

	cout << '#';
	for(int x = 0; x < Map_settings::map_len.y; x++){
		cout << '-';
	}
	cout << '#' << endl;
}


void Map::regionalize(){
	int region_len = Map_settings::region_len;

	num_x_regions = (xmap_len / region_len);
	num_y_regions = (ymap_len / region_len);

	regions = new mRegion**[num_x_regions];
	for(int i = 0; i < num_x_regions; i++){
		regions[i] = new mRegion*[num_y_regions];

		for(int j = 0; j < num_y_regions; j++){
			regions[i][j] = new mRegion(region_len, region_len*i, region_len*j);
		}
	}

}

void Map::populate(){
	make_buildings();
	make_characters();
}


void Map::make_buildings(){
// list of possible buildings we can place
list<Pair<int> > poss_build;
list<Pair<int> >::iterator itc;

// vertices of buildings and basic attributes
queue<Pair<int> > vertices;
Pair<int> ran_coor, build_dims, reg_coor;
int num_buildings_region;

int min_len = Map_settings::min_footprint_len;
int max_len = Map_settings::max_footprint_len;
int reg_len = Map_settings::region_len;

int tryc; // number of times we've tried to place building

for(int x = 0; x < num_x_regions; x++){
for(int y = 0; y < num_y_regions; y++){
	num_buildings_region = RNG::random_num(Map_settings::min_building_density, Map_settings::max_building_density);
	reg_coor = Pair<int>(x,y) * reg_len;

	poss_build.clear();
	for(int i = 0; i < num_buildings_region; i++){
		poss_build.push_front(RNG::random_pair(min_len, max_len, min_len, max_len));
	}
	poss_build.sort(Rect_area_cmp<int>()); // sort by size, try to place largest first

	// try to fit bulding in by guessing random coordinates to place it
	for(itc = poss_build.begin(); itc != poss_build.end(); itc++){
		for(tryc = 0; tryc < Map_settings::max_build_placement_tries; tryc++){
			ran_coor = RNG::random_pair(reg_coor, reg_coor + reg_len);

			if(placement_is_clear(*itc, ran_coor)){
				add_obstacle(new Building(ran_coor, *itc));
				break;
			}
		}
	}

}}

}

void Map::make_characters(){
	Pair<int> ran_coor, reg_coor;
	int num_hum, num_zom;

	int reg_len = Map_settings::region_len;

	for(int x = 0; x < num_x_regions; x++){
	for(int y = 0; y < num_y_regions; y++){
		num_hum = RNG::random_num(Map_settings::min_human_density, Map_settings::max_human_density);
		num_zom = RNG::random_num(Map_settings::min_zombie_density, Map_settings::max_zombie_density);
		reg_coor = Pair<int>(x,y) * Pair<int>(Map_settings::region_len, Map_settings::region_len);

		// get random coordinate for human and try and fit him in
		for(int i = 0; i < num_hum + num_zom; i++){
			ran_coor = RNG::random_pair(reg_coor, reg_coor + Map_settings::region_len);

			if(get_symbol(ran_coor) != EMPTY){ // chosen coordinate isn't empty
				for(int x = -3; x < 3; x++){
				for(int y = -3; y < 3; y++){ // find nearby locations that may be empty
					if(get_symbol(ran_coor + Pair<int>(x,y)) == EMPTY)
					if(i < num_zom){
						add_character(ZOMBIE, ran_coor + Pair<int>(x,y)); // change this to input only character name
					}else{
						add_character(HUMAN, ran_coor + Pair<int>(x,y));
					}
					x = y = 3; // end both for loops
					}
				}
			}
			else if(i < num_zom){
				add_character(ZOMBIE, ran_coor);
			}else{
				add_character(HUMAN, ran_coor);
			}
		}
	}}

}

bool Map::placement_is_clear(Pair<int> dims, Pair<int> coor){
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			if(get_symbol(coor.x + x, coor.y + y) != EMPTY)
				return false;
		}
	}

	return true;
}

void Map::copy_ROI(Map_symbol** buf, Pair<int> start_coor, Pair<int> length){
	Pair<int> end_coor = start_coor + length;

	if(overall_bounds_check(start_coor) and overall_bounds_check(end_coor)){
		for(int x = start_coor.x; x < end_coor.x; x++){
		for(int y = start_coor.y; y < end_coor.y; y++){
			buf[x][y] = blockmap[x][y][TOP_LAYER]->get_symbol();
		}}
	}
	else{
		cout << "ERROR: Overall bounds check failed." << endl;
	}
}

//
// FUNCTIONS THAT INTERFACE WITH MAPSERVER WRAPPER
//

void Map::move_character(Character* this_ptr, Pair<int>& new_loc){
	Pair<int> curr = this_ptr->get_coor();
	Pair<int> curr_reg = find_region(curr);
	Pair<int> new_reg = find_region(new_loc);

	if( overall_bounds_check(new_loc) && regional_bounds_check(new_reg)
	and NAV::block_is(blockmap[new_loc.x][new_loc.y][TOP_LAYER]->symbol, PASSABLE) ){
		this_ptr->set_coor(new_loc);

		if(curr_reg != new_reg){ // if character has crossed regions, change region
			regions[curr_reg.x][curr_reg.y]->remove_character(this_ptr);
			regions[new_reg.x][new_reg.y]->insert_character(this_ptr);
		}

		add_block(blockmap[curr.x][curr.y][TOP_LAYER], new_loc);
		remove_block(curr);

	}
}

void Map::infect_player(int ID){
	map<int, pObject*>::iterator it = IDhash.find(ID);
	pObject* inf_obj = it->second;
	Infected* new_obj = NULL;

	// only execute if ID was found and if it was a human
	if(it != IDhash.end() and inf_obj->get_symbol() == HUMAN){
		Pair<int> coor = inf_obj->get_coor();
		Pair<int> reg_coor = find_region(coor);

		new_obj = new Infected((Human*) inf_obj);

		// this takes O(n), can we make this better?
		(*find(characters.begin(), characters.end(), inf_obj)) = new_obj ;

		blockmap[coor.x][coor.y][TOP_LAYER]->change_symbol(INFECTED);
		IDhash[ blockmap[coor.x][coor.y][TOP_LAYER]->ID ] = new_obj;
		regions[reg_coor.x][reg_coor.y]->switch_character((Character*) inf_obj, (Character*) new_obj);

		delete inf_obj;

		map_stats.num_bites++;
		map_stats.num_characters[INFECTED]++;
		map_stats.num_characters[HUMAN]--;
	}
}


void Map::convert_infected(Character* infected){
	// need type checking somewhere along the line for this...
	Pair<int> coor = infected->get_coor();
	int ID = get_ID(coor);

	delete_character(infected, ID);
	add_character(ZOMBIE, coor);
}

