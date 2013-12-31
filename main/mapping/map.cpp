#include "map.hpp"

namespace MS = Map_settings;

Map::Map(){
	MapServer::currmap = this;

	newID = 0;

	empty_object = pObject(Pair(-1,-1));
	EMPTY_BLOCK = ObjectBlock(EMPTY_TYPE_ID, EMPTY);
	IDhash[EMPTY_TYPE_ID] = &empty_object;
	newID = EMPTY_TYPE_ID + 1;

	xmap_len = MS::map_len[0];
	ymap_len = MS::map_len[1];

	blockmap = new ObjectBlock**[xmap_len];
	for(int i = 0; i < xmap_len; i++){
		blockmap[i] = new ObjectBlock*[ymap_len];
		for(int j = 0; j < ymap_len; j++){
			blockmap[i][j] = &EMPTY_BLOCK;
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
		delete [] blockmap[i];
	}
	delete [] blockmap;

	// deleting all objects in hash map and characters
	// is unnecessary, deleting regions does it

	// delete object_blocks we made for the map
	while(!all_object_blocks.empty()) {
		delete all_object_blocks.front(), all_object_blocks.pop_front();
	}
}

void Map::iterate(){
	for(list<Character*>::iterator it = characters.begin(); it != characters.end(); it++)
		(*it)->exec();
}

Map_symbol Map::get_symbol(Pair coor){ return get_symbol(coor.x, coor.y); }
Map_symbol Map::get_symbol(int xcoor, int ycoor){
	if(overall_bounds_check(xcoor, ycoor)){
		return(blockmap[xcoor][ycoor]->symbol);
	}

	return(INVALID);
}

int Map::get_ID(Pair coor){ return (blockmap[coor.x][coor.y]->ID); }
int Map::get_ID(int xcoor, int ycoor){ return (blockmap[xcoor][ycoor]->ID); }

void Map::add_character(Character *c){
	Pair coor = c->get_coor();
	Pair reg_coor = find_region(coor);

	if(overall_bounds_check(coor) && regional_bounds_check(reg_coor)){
		blockmap[coor.x][coor.y] = new_object_block(newID, c->get_symbol());
		regions[reg_coor.x][reg_coor.y]->insert_character(c);
	}

	IDhash[newID++] = c;
	characters.push_front(c);
}

void Map::delete_character(Character* c, int ID){
	Pair coor = c->get_coor();
	Pair reg_coor = find_region(coor);

	regions[reg_coor.x][reg_coor.y]->remove_character(c);
	// should this stuff be in its own function?
	IDhash.erase(ID);

	all_object_blocks.remove(blockmap[coor.x][coor.y]);
	delete blockmap[coor.x][coor.y];
	blockmap[coor.x][coor.y] = &EMPTY_BLOCK;

	delete c;
}

void Map::add_obstacle(Obstacle *o){
	Pair coor = o->get_coor();
	Pair dimens = o->get_dimensions();
	Pair reg_coor = find_region(coor);
	// newblock is a wall that refers to the parent obstacle, same newblock
	// symbol at different places in the map
	ObjectBlock* newblock = new_object_block(newID, o->get_symbol());

	for(int x = 0; x < dimens.x; x++){
	for(int y = 0; y < dimens.y; y++){
		if(o->get_footprint(x,y) && overall_bounds_check(coor + Pair(x,y))){
			blockmap[coor.x + x][coor.y + y] = newblock;
		}
	}
	}


	IDhash[newID++] = o;

	if(regional_bounds_check(reg_coor)){
		regions[reg_coor.x][reg_coor.y]->insert_obstacle(o);
	}
}

ObjectBlock* Map::new_object_block(int ID, Map_symbol sym){
	ObjectBlock* newobj = new ObjectBlock(newID, sym);
	all_object_blocks.push_front(newobj);
	return(newobj);
}

Pair Map::find_region(Pair coor){
	return Pair(coor.x / MS::region_len, coor.y / MS::region_len);
}

bool Map::overall_bounds_check(Pair p){ return overall_bounds_check(p.x, p.y); }
bool Map::overall_bounds_check(int x, int y){
	if(x < 0 || y < 0 || x >= xmap_len || y >= ymap_len){
		return false;
	}

	return true;
}

bool Map::regional_bounds_check(Pair p){ return regional_bounds_check(p.x, p.y); }
bool Map::regional_bounds_check(int x, int y){
	if(x < 0 || y < 0 || x >= num_x_regions || y >= num_y_regions){
		return false;
	}

	return true;
}

void Map::print_map(){
	cout << '#';
	for(int x = 0; x < MS::map_len[0]; x++){
		cout << '-';
	}
	cout << '#' << endl;

	for(int y = 0; y < MS::map_len[1]; y++){
		cout << '|';
		for(int x = 0; x < MS::map_len[0]; x++){
			cout << (char) blockmap[x][y]->symbol;
		}
		cout << '|' << endl;
	}

	cout << '#';
	for(int x = 0; x < MS::map_len[0]; x++){
		cout << '-';
	}
	cout << '#' << endl;
}


void Map::regionalize(){
	int region_len = MS::region_len;

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
list<Building*> poss_build;
list<Building*>::iterator itc;

queue<Pair> vertices;
Pair ran_dims, ran_coor, build_dims, reg_coor;
int num_buildings_region;

int min_len = MS::min_building_len;
int max_len = MS::max_building_len;
int reg_len = MS::region_len;

int tryc;

for(int x = 0; x < num_x_regions; x++){
for(int y = 0; y < num_y_regions; y++){
	num_buildings_region = RNG::random_num(MS::min_building_density, MS::max_building_density);
	reg_coor = Pair(x,y) * Pair(MS::region_len, MS::region_len);

	// make dimensions and shapes of new buildings
	poss_build.clear();
	for(int i = 0; i < num_buildings_region; i++){
		// temporary
		ran_dims = RNG::random_pair(min_len, max_len, min_len, max_len);

		vertices.push(Pair(0,0));
		vertices.push(Pair(ran_dims.x-1,0));
		vertices.push(Pair(ran_dims.x-1,ran_dims.y-1));
		vertices.push(Pair(0,ran_dims.y-1));
		//trace_outline(ran_dims, vertices);

		//door_edge = RNG::random_pair(0,2, 0,2);

		poss_build.push_front(new Building(reg_coor, ran_dims, vertices));

		vertices = queue<Pair>(); // empty queue
	}

	poss_build.sort(Dimension_cmp()); // sort by area

	// try to fit bulding in by guessing random coordinates to place it
	for(itc = poss_build.begin(); itc != poss_build.end(); itc++){
		build_dims = (*itc)->get_dimensions();
		for(tryc = 0; tryc < MAX_TRIES; tryc++){
			ran_coor = RNG::random_pair(reg_coor, reg_coor + reg_len);
			if(placement_is_clear(build_dims, ran_coor)){
				(*itc)->set_coor(ran_coor);
				add_obstacle(*itc);
				break;
			}
		}
		if(tryc == MAX_TRIES){ delete *itc; }// delete if no place found
	}
}}

}

void Map::make_characters(){
	Pair ran_coor, reg_coor;
	int num_hum, num_zom;

	int reg_len = MS::region_len;

	for(int x = 0; x < num_x_regions; x++){
	for(int y = 0; y < num_y_regions; y++){
		num_hum = RNG::random_num(MS::min_human_density, MS::max_human_density);
		num_zom = RNG::random_num(MS::min_zombie_density, MS::max_zombie_density);
		reg_coor = Pair(x,y) * Pair(MS::region_len, MS::region_len);

		// get random coordinate for human and try and fit him in
		for(int i = 0; i < num_hum + num_zom; i++){
			ran_coor = RNG::random_pair(reg_coor, reg_coor + MS::region_len);

			if(get_symbol(ran_coor) != EMPTY){ // chosen coordinate isn't empty
				for(int x = -3; x < 3; x++){
				for(int y = -3; y < 3; y++){ // find nearby locations that may be empty
					if(get_symbol(ran_coor + Pair(x,y)) == EMPTY)
					if(i < num_zom){
						add_character(new Zombie(ran_coor + Pair(x,y)));
					}else{
						add_character(new Human(ran_coor + Pair(x,y)));
					}
					x = y = 3;
					}
				}
			}
			else if(i < num_zom){
				add_character(new Zombie(ran_coor));
			}else{
				add_character(new Human(ran_coor));
			}
		}
	}}

}

void Map::remove_corners(Pair& ran_dims, queue<Pair>& vertices){

//	vertices.push()
//	for(int i = 0; i < 4; i++){
//	if(RNG::yes_or_no(0.25)){
//		for(list<Pair>::iterator it = sqrs.begin(); it != sqrs.end(); it++){
//			if(it->x ) {}
//		}
//	}
//	}
}

void Map::trace_outline(Pair& ran_dims, queue<Pair>& vertices){
	float turn_chance = (ran_dims.x < ran_dims.y) ? ran_dims.x : ran_dims.y;
	turn_chance = 3 / turn_chance;

	list<Nav_symbol> turns;
	turns.push_front(LEFT), turns.push_front(RIGHT);
	turns.push_front(RIGHT), turns.push_front(LEFT);
	list<Nav_symbol>::iterator turn = turns.begin();

	Pair limit(ran_dims.x - 1, ran_dims.y - 1);

	Pair null_pair = Pair(0,0);
	vertices.push(null_pair);
	Pair change(0,1), position(0,0);

	int num_turns = 0;
cout << limit << endl;

	while(true){
		cout << position << endl << change << endl << endl;
		position += change;

		if((position.x < null_pair.x) && (num_turns == 3)){ // if we've arrived back at origin
			break;
		}
		else if((position.y > limit.y) || (position.x > limit.x) || (position.x < null_pair.x) || (position.y < null_pair.y)){ // we've hit the border - always turn left
			cout << "LIMIT" << endl;
			position -= change;
			turn_left(vertices, position, change);
			turn = turns.begin();
			num_turns++;
			while((position.y < limit.y) && (position.x < limit.x) && (position.x > null_pair.x) && (position.y > null_pair.y)){
				cout << "pos" << position << "   lim" << limit << endl;
//					cout << position << "  " << change << endl;
				position += change;
			}
		}
		else if(RNG::yes_or_no(turn_chance)){ // choose to turn by chance
			if(*turn == LEFT){
				cout << "LEFT" << endl;
				turn_left(vertices, position, change);
			}else{
				cout << "RIGHT" << endl;
				turn_right(vertices, position, change);
			}

			if(turn == turns.end()){ // advance us if done turning
				cout << "CONTINUING" << endl;
				while((position.y < limit.y) && (position.x < limit.x) && (position.x > null_pair.x) && (position.y > null_pair.y)){
					cout << "pos" << position << "   lim" << limit << endl;
//					cout << position << "  " << change << endl;
					position += change;
				}
				turn_left(vertices, position, change);
				turn = turns.begin();
				num_turns++;
			}
			else{
				turn++;
			}
		}

	}
}

void Map::turn_left(queue<Pair>& vertices, Pair& pos, Pair& change){
	vertices.push(pos);

	if(change.x == 1) change.set(0,-1);
	else if(change.x == -1) change.set(0,1);
	else if(change.y == 1) change.set(1,0);
	else if(change.y == -1) change.set(-1,0);
}

void Map::turn_right(queue<Pair>& vertices, Pair& pos, Pair& change){
	vertices.push(pos);

	if(change.x == 1) change.set(0,1);
	else if(change.x == -1) change.set(0,-1);
	else if(change.y == 1) change.set(-1,0);
	else if(change.y == -1) change.set(1,0);
}

Pair Map::find_empty_path(int x_search, int y_search, int xincr, int yincr){
	Pair empty = Pair(0,0);
//	int i = 0;
//
//	while(poss_coors[MS::region_len + x_search + i*xincr][MS::region_len + y_search + i*yincr] != INVALID_COOR){
//		empty.x += xincr;
//		empty.y += yincr;
//
//		empty++;
//		i++;
//	}
//
	return(empty);

}

bool Map::placement_is_clear(Pair dims, Pair coor){
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			if(get_symbol(coor.x + x, coor.y + y) != EMPTY)
				return false;
		}
	}

	return true;
}


//
// FUNCTIONS FOR MAPSERVER WRAPPER
//

void Map::move_character(Character* this_ptr, Pair& new_loc){
	Pair curr = this_ptr->get_coor();
	Pair curr_reg = find_region(curr);
	Pair new_reg = find_region(new_loc);

	if( overall_bounds_check(new_loc) && regional_bounds_check(new_reg)){
	if(blockmap[new_loc.x][new_loc.y]->symbol == EMPTY){
		this_ptr->set_coor(new_loc);

		if(curr_reg != new_reg){ // if character has crossed regions, change region
			regions[curr_reg.x][curr_reg.y]->remove_character(this_ptr);
			regions[new_reg.x][new_reg.y]->insert_character(this_ptr);
		}

		blockmap[new_loc.x][new_loc.y] = blockmap[curr.x][curr.y];
		blockmap[curr.x][curr.y] = &EMPTY_BLOCK;
	}
	}
}

void Map::infect_player(int ID){
	map<int, pObject*>::iterator it = IDhash.find(ID);
	pObject* inf_obj = it->second;

	// only execute if ID was found and if it was a human
	if(it != IDhash.end() and inf_obj->get_symbol() == HUMAN){
		Pair coor = inf_obj->get_coor();

		delete_character(static_cast<Character*>(inf_obj), ID);
		add_character(new Zombie(coor));
	}
}
