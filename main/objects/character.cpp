#include "character.hpp"

 // put this here instead of in character.hpp to resolve circular inclusion issue
 // pulls in map server
#include "../mapping/map.hpp"

Character::Character(Pair<int> coor, Map_symbol sym, Stats stats):
pObject(coor, sym)
{
	Character::init_stats(stats);

	vision_len = 2*(stats.VIS_RAD) + 1;
	// idea: have managed memory such that we have vision fields ready to
	// go, regardless of threading
	vision_field = new Map_symbol*[vision_len];
	for(int x = 0; x < vision_len; x++){
		vision_field[x] = new Map_symbol[vision_len];
		for(int y = 0; y < vision_len; y++){
			vision_field[x][y] = EMPTY;
		}
	}
}

Character::~Character(){
	for(int x = 0; x < vision_len; x++){
		delete [] vision_field[x];
	}
	delete [] vision_field;
}

void Character::init_stats(Stats stats){
	this->stats.VIS_RAD = stats.VIS_RAD;
	this->stats.num_iter_since_last_move = 0;
}

bool Character::ready_to_move(){
	// return false to tell derived class that we can't move
	if( this->stats.num_iter_since_last_move >= this->stats.SPEED_EQUIVALENT_ITER ){
		this->stats.num_iter_since_last_move = 0;
		return true;
	}

	return false;
}

void Character::exec(){
	int vision_rad = stats.VIS_RAD;
	int vision_len = 2*vision_rad + 1;

	this->stats.num_iter_since_last_move ++ ;

	MapServer::Instance()->copy_field(vision_field, coor - vision_rad, Pair<int>(stats.VIS_RAD));
//	this->print_vision();
}

///// TODO: THIS SHOULD ALL PROBABLY GO IN SEPARATE NAVIGATION/FINDING FILE LATER...

Map_symbol Character::get_directional_block(Nav_symbol dir){
	Pair<int> vis_index(stats.VIS_RAD);

	if(!NAV::apply_direction(vis_index, dir))
		return(INVALID);
	else
		return(vision_field[vis_index.x][vis_index.y]);
}

void Character::move_direction(Nav_symbol dir){
	MapServer::Instance()->move_character(this, dir);
}

void Character::random_walk(){
	vector<Nav_symbol> poss_dir;

	for(int i = 0; i < NAV::NUM_POSS_DIR; i++){
		Nav_symbol dir = NAV::get_poss_dir(i);

		if(NAV::block_is(get_directional_block(dir), PASSABLE))
			poss_dir.push_back(dir);
	}

	if(poss_dir.size() > 0){
		move_direction( poss_dir.at(RNG::random_num(0, poss_dir.size() - 1)) );
	}
}

void Character::go_away_from(Pair<int> pos){
	go_towards(-pos);
}

void Character::go_towards(Pair<int> pos){
	Pair<float> unit = pos.unit_v();
	Pair<int> dir(0,0);

	// translate position to move to as binary direction change
	if(abs(unit.x) > UNIT_BOUND) dir.x = sgn(unit.x);
	if(abs(unit.y) > UNIT_BOUND) dir.y = sgn(unit.y);

	// move to space if empty, otherwise search for empty spaces in surrounding area
	Nav_symbol left_direc, right_direc, direc = NAV::get_direction(dir);

	if( NAV::block_is(get_directional_block(direc), PASSABLE) )
		MapServer::Instance()->move_character(this, direc);
	else{
		left_direc = right_direc = direc;
		for(int i = 0; i < 2; i++){
			left_direc = NAV::get_left(left_direc);
			right_direc = NAV::get_right(right_direc);

			if( NAV::block_is(get_directional_block(left_direc), PASSABLE) ) {
				MapServer::Instance()->move_character(this, left_direc);
				return;
			}
			else if( NAV::block_is(get_directional_block(right_direc), PASSABLE) ){
				MapServer::Instance()->move_character(this, right_direc);
				return;
			}
		}
	}
}

bool Character::find_nearest(Pair<int>& pos, Map_symbol sym){
	float dist = vision_len;
	float min_dist = vision_len;
	bool found = false;

	for(int x = -stats.VIS_RAD; x < stats.VIS_RAD; x++){
		for(int y = -stats.VIS_RAD; y < stats.VIS_RAD; y++){
			if(vision_field[x + stats.VIS_RAD][y + stats.VIS_RAD] == sym){

				if((dist = sqrt(pow(x, 2) + pow(y, 2))) < min_dist){
					found = true;
					min_dist = dist;
					pos = Pair<int>(x,y);
				}
			}
	}}

	return(found);
}

bool Character::is_adjacent(Pair<int>& pos){
	return abs(pos.x) <= 1 and abs(pos.y) <= 1;
}

void Character::print_vision(){
	for(int x = 0; x < vision_len; x++) 	cout << "=";
	cout << endl;

	for(int x = 0; x < vision_len; x++){
	for(int y = 0; y < vision_len; y++){
		cout << (char) vision_field[y][x];
	}
	cout << "|" << endl;
	}

	for(int x = 0; x < vision_len; x++) 	cout << "=";
	cout << endl;
}
