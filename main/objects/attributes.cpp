#include "attributes.hpp"
#include <iostream>
#include <stdlib.h>

Footprint::Footprint():
is_alloced(false)
{
}

Footprint::Footprint(const Footprint& f):
dimensions(f.dimensions)
{
	alloc_footprint(f.dimensions);

	for(int x = 0; x < dimensions.x; x++){
		for(int y = 0; y < dimensions.y; y++){
			footprint[x][y] = f.footprint[x][y];
		}
	}
}


Footprint::~Footprint()
{
	if(is_alloced){
		for(int i = 0; i < dimensions.x; i++){
			delete [] footprint[i];
		}
		delete [] footprint;
	}
}

void Footprint::copy_footprint(Map_symbol** foot){
	for(int x = 0; x < dimensions.x; x++){
		for(int y = 0; y < dimensions.y; y++){
			foot[x][y] = footprint[x][y];
		}
	}
}

void Footprint::alloc_footprint(Pair<int> dims){
	footprint = new Map_symbol*[dims.x];
	for (int i = 0; i < dims.x; i++){
		footprint[i] = new Map_symbol[dims.y]();
		for(int j = 0; j < dims.y; j++){
			footprint[i][j] = EMPTY;
		}
	}
	is_alloced = true;
}

//void Footprint::turn_left(Pair<int>& pos, Pair<int>& change){
//	if(change.x == 1) change.set(0,-1);
//	else if(change.x == -1) change.set(0,1);
//	else if(change.y == 1) change.set(1,0);
//	else if(change.y == -1) change.set(-1,0);
//}
//
//void Footprint::turn_right(Pair<int>& pos, Pair<int>& change){
//	if(change.x == 1) change.set(0,1);
//	else if(change.x == -1) change.set(0,-1);
//	else if(change.y == 1) change.set(-1,0);
//	else if(change.y == -1) change.set(1,0);
//}

bool Footprint::block_is_inside(int x, int y){
	int xs, xe, ys, ye;

	// remove blocks outside boundary from consideration
	if(x - 1 >= 0)
		xs = x - 1;
	else
		xs = x;

	if(y - 1 >= 0)
		ys = y - 1;
	else
		ys = y;

	if(x + 1 < dimensions.x)
		xe = x + 1;
	else
		xe = x;

	if(y + 1 < dimensions.y)
		ye = y + 1;
	else
		ye = y;

	// if radius of block contains empty space then it must be an edge block
	for(int xx = xs; xx <= xe; xx++){
		for(int yy = ys; yy <= ye; yy++){
			if(footprint[xx][yy] == EMPTY){
				return false;
			}
		}
	}

	return true;
}

Footprint::Footprint(Pair<int> dims):
dimensions(dims)
{
	alloc_footprint(dims);

	Pair<int> corner, dimen;
	bool hit_block = false; // whether we will be overlapping existing block
	bool door_found = false;
	float door_chance = Map_settings::door_probability;
	int num_poss_doors = 0;
	int max_x, max_y; // max length in direction
	vector<Pair<int> > borders;

//TODO: make sure we overlap by more than one square

	for(int i = 0; i < Map_settings::blocks_per_building; i++){
		// make random coordinate
		corner = RNG::random_pair(1, dims.x - Map_settings::min_building_len,
								1,	dims.y - Map_settings::min_building_len);

		// clip max dimensions to not go over boundary
		if(dims.x - 1 - corner.x < Map_settings::max_building_len){
			max_x = dims.x - 1 - corner.x;
		}
		else{
			max_x = Map_settings::max_building_len;
		}
		if(dims.y - 1 - corner.y < Map_settings::max_building_len){
			max_y = dims.y - 1 - corner.y;
		}
		else{
			max_y = Map_settings::max_building_len;
		}

		// make random dimension
		dimen = RNG::random_pair(3, max_x, 3, max_y); // must be greater than 1

		// determine if we're overlapping a block (not on our first time, though)
		if(i != 0){
			for(int x = corner.x; x < corner.x + dimen.x; x++){
				for(int y = corner.y; y < corner.y + dimen.y; y++){
					if( footprint[x][y] == BUILDING_INDOOR ){
						hit_block = true;
						break;
					}
				}
			}
		}

		// only place block if we're overlapping another block or this is our first
		if(i == 0 or hit_block){
			for(int x = corner.x; x < corner.x + dimen.x; x++){
				for(int y = corner.y; y < corner.y + dimen.y; y++){
					footprint[x][y] = BUILDING_INDOOR;
				}
			}

			hit_block = false;
		}
	}

	// mark indoor blocks with a 2 so we can go through and remove later
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			if( footprint[x][y] == BUILDING_INDOOR ){
				// check if this is an indoor block (has at least 1 adjacent 0)
				if( ! block_is_inside(x, y) ){
				// add it to vector of possible locations for doors
					footprint[x][y] = BUILDING_WALL;
					borders.push_back(Pair<int>(x,y));
				}
			}
		}
	}

	/* select and carve out door, keep trying and updating door chance if not found
	 *
	 */

	// we want to always place at least one door
	int doors_to_skip = 0;

	for(vector<Pair<int> >::iterator it = borders.begin(); it != borders.end(); it++){
		door_found = false;


		// door is not on corner
		if(
				footprint[(*it).x - 1][(*it).y + 1] != BUILDING_WALL
			and footprint[(*it).x + 1][(*it).y + 1] != BUILDING_WALL
			and footprint[(*it).x - 1][(*it).y - 1] != BUILDING_WALL
			and footprint[(*it).x + 1][(*it).y - 1] != BUILDING_WALL
			)
		{

			// door is on straight edge
			if(
				(	 footprint[(*it).x][(*it).y - 1] 	== BUILDING_WALL
				and footprint[(*it).x][(*it).y + 1] 	== BUILDING_WALL)

				or

				( 	footprint[(*it).x - 1][(*it).y] 	== BUILDING_WALL
				and footprint[(*it).x + 1][(*it).y] 	== BUILDING_WALL)
				)
			{
				door_found = true;
			}

		}

		// carve door and exit if found
		if( door_found and (0 == doors_to_skip--) ){
			footprint[(*it).x][(*it).y] = BUILDING_DOOR;
			doors_to_skip = RNG::random_num(5, dims.x / 2);
		}
	}

}

void Footprint::print(){
	for (int y = 0; y < dimensions.y; y++){
	for (int x = 0; x < dimensions.x; x++){
			cout << (char) footprint[x][y];
	}
	cout << endl;
	}
	cout << endl;
}



// old code for tracing vertices (may still be useful)

//	float TURN_CHANCE = (dims.x < dims.y) ? dims.x : dims.y;
//	TURN_CHANCE = 0.5 / TURN_CHANCE;
//
//	list<Nav_symbol> turn_order;
//	turn_order.push_front(LEFT), turn_order.push_front(RIGHT);
//	turn_order.push_front(RIGHT), turn_order.push_front(LEFT);
//	list<Nav_symbol>::iterator turn = turn_order.begin();
//
//	Pair<int> limit(dims - 1);
//
//	Pair<int> start = Pair<int>(0,0);
//	Pair<int> change(0,1), position(0,0);
//
//	int num_turns = 0;
//int make_turn = false;
//	footprint[0][0] = 1;
//int junk;
//	while(true){
//		while(cin.get() != '\n'){}
//		position += change;
//
//		if(position.x < start.x and (num_turns == 3)){ // if we've arrived back at origin and used our turns
//			break;
//		}
//		else if(position.outside(start, limit)){ // we've hit the border - always turn left
//			position -= change; // undo step
//			turn_left(position, change);
//			turn = turn_order.begin(); // reset turns
//			num_turns++;
//			while(position.within(start + 1, limit - 1)){ // go to next border
//				position += change;
//			}
//		}
//		else if(footprint[position.x][position.y] == 1){ // if we've hit an old wall we made
//			position -= change; // backstep
//			make_turn = true;
//		}
//		else if(RNG::yes_or_no(TURN_CHANCE)){ // choose to turn by chance if we haven't hit border
//			make_turn = true;
//		}
//
//		if(make_turn){
//			if(*turn == LEFT){
//				turn_left(position, change);
//			}else{
//				turn_right(position, change);
//			}
//
//			if(turn == turn_order.end()){ // advance us if done turning
//				while(position.within(start + 1, limit - 1)){
//					position += change;
//				}
//				turn_left(position, change);
//				turn = turn_order.begin();
//				num_turns++;
//			}
//			else{
//				turn++;
//			}
//
//			make_turn = false;
//		}
//
//		footprint[position.x][position.y] = 1; // put down wall
//
//		print();
//	}

//	Pair<int> direction, pos;
//	Pair<int> it;
//
//	ver.push(ver.front());
//	Pair<int> prev_vert = ver.front();
//	ver.pop();
//
//	while(!ver.empty()){
//		it = ver.front();
//		ver.pop();
//
//		footprint[it.x][it.y] = 1;
//
//		direction.x = ((prev_vert.x < it.x) - (prev_vert.x > it.x));
//		direction.y = ((prev_vert.y < it.y) - (prev_vert.y > it.y));
//
//		pos = it;
//		while((pos.x != prev_vert.x) || (pos.y != prev_vert.y)){
//			pos.x -= direction.x;
//			pos.y -= direction.y;
//
//			footprint[pos.x][pos.y] = 1;
//		}
//
//		prev_vert.x = it.x;
//		prev_vert.y = it.y;
//	}

//	for(int x = 0; x < dimensions.x; x++){
//		for(int y = 0; y < dimensions.y; y++){
//			cout << footprint[x][y] << " ";
//		}
//		cout << endl;
//	}


//	cout << endl;
//	bool startfound = false;
//	bool spacefound = false;
//
//	for(int i = 0; i < dims.x; i++){
//		for(int j = 0; j < dims.y; j++){
//			if(footprint[i][j] == 1){
//				if(!startfound)
//					startfound = true;
//				else if(spacefound && startfound)
//					startfound = false;
//					spacefound = false;
//			}
//			else if(startfound){
//				footprint[i][j] = 1;
//				spacefound = true;
//			}
//		}
//	}
