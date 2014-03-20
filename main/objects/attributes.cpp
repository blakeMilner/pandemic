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

bool Footprint::has_adjacent_zero(int x, int y){
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
			if(!footprint[xx][yy]){
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
	float door_chance = Map_settings::door_chance;
	int num_poss_doors = 0;
	int max_x, max_y; // max length in direction
	vector<Pair<int> > borders;

//TODO: make sure we overlap by more than one square


	for(int i = 0; i < Map_settings::blocks_per_building; i++){
		// make random coordinate
		corner = RNG::random_pair(1, dims.x - Map_settings::min_building_len - 1,
								1,	dims.y - Map_settings::min_building_len - 1);

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

		// determine if we're overlapping a block
		for(int x = corner.x; x < corner.x + dimen.x; x++){
			for(int y = corner.y; y < corner.y + dimen.y; y++){
				if(footprint[x][y]){
					hit_block = true;
				}
			}
		}

		// only place block if we're overlapping another block or this is our first
		if(i == 0 or hit_block){
			for(int x = corner.x; x < corner.x + dimen.x; x++){
				for(int y = corner.y; y < corner.y + dimen.y; y++){
					footprint[x][y] = 1;
				}
			}

			hit_block = false;
		}
	}

	// mark indoor blocks with a 2 so we can go through and remove later
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			if(footprint[x][y]){
				// check if this is an indoor block (has at least 1 adjacent 0)
				if(has_adjacent_zero(x, y)){
					footprint[x][y] = 2;
				}
				// otherwise add it to vector of possible locations for doors
				else{
					borders.push_back(Pair<int>(x,y));
				}
			}
		}
	}

	// remove indoor blocks
	for(int x = 0; x < dims.x; x++){
		for(int y = 0; y < dims.y; y++){
			if(footprint[x][y] == 2){
				footprint[x][y] = 0;
			}
		}
	}

//	// select and carve out door, keep trying and updating door chance if not found
//	while(!door_found){
//		for(vector<Pair<int> >::iterator it = borders.begin(); it != borders.end(); it++){
//			// check if this is our door by chance and if we're not on the footprint border
//			if(RNG::yes_or_no(door_chance) and
//					(*it).x >= 0 and (*it).x < dims.x and (*it).y >= 0 and (*it).y < dims.y){
//
//				door_found = false;
//
//				// door is on straight edge
//				if(!footprint[(*it).x - 1][(*it).y] and
//					!footprint[(*it).x + 1][(*it).y] and
//					footprint[(*it).x - 1][(*it).y - 1] == 0 and
//					footprint[(*it).x + 1][(*it).y + 1] == 0 and
//					footprint[(*it).x][(*it).y - 1] == 0 and
//					footprint[(*it).x][(*it).y + 1] == 0){
//						door_found = true;
//				}
//				// door is on other straight edge
//				else if(!footprint[(*it).x][(*it).y - 1] and
//					!footprint[(*it).x][(*it).y + 1] and
//					footprint[(*it).x - 1][(*it).y - 1] == 0 and
//					footprint[(*it).x + 1][(*it).y + 1] == 0 and
//					footprint[(*it).x - 1][(*it).y] == 0 and
//					footprint[(*it).x + 1][(*it).y] == 0){
//						door_found = true;
//				}
//			}
//
//			// carve door and exit if found
//			if(door_found){
//				footprint[(*it).x][(*it).y] = 0;
//				break;
//			}
//		}
//
//		door_chance += 0.05;
//	}


//	// generate random rectangle to push in, start/end coordinates and push length
//	int top_start, bot_start, right_start, left_start;
//	int top_end, bot_end, right_end, left_end;
//	int top_length, bot_length, right_length, left_length;
//
//	left_start = RNG::random_num(0, dims.y - 3);
//	left_end = RNG::random_num(left_start + 2, dims.y - 1);
//	left_length = RNG::random_num(0, 3);
//
//	// loop through entire left border
//	for(int y = 0; y < dims.y; y++){
//		// start pushing rectangle if we're within its length
//		if(y >= left_start and y <= left_end){
//			footprint[left_length][y] = 1;
//
//			// push rectangle in (towards right)
//			for(int x = 0; x < left_length; x++){
//				// make perpendicular edges
//				if(left_start != 0){
//					footprint[x][left_start] = 1;
//				}
//				if(left_end != dims.y - 1){
//					footprint[x][left_end] = 1;
//				}
//			}
//
//		}
//		// otherwise just make normal border
//		else{
//			footprint[0][y] = 1;
//		}
//	}
//
//	// generate random rectangle to push in
//	right_start = RNG::random_num(0, dims.y - 3);
//	right_end = RNG::random_num(right_start + 2, dims.y - 1);
//	right_length = RNG::random_num(0, 3);
//
//	// only push in if it won't overlap the other
//	if((left_length < (dims.x - 2 - right_length)) or ((right_start > left_end - 1) and (left_start > right_end - 1))){
//
//	// loop through entire right border
//	for(int y = 0; y < dims.y; y++){
//		// start pushing rectangle if we're within its length
//		if(y >= right_start and y <= right_end){
//			footprint[dims.x - right_length - 1][y] = 1;
//
//			// push rectangle in (towards right)
//			for(int x = dims.x - 1; x > dims.x - right_length - 1; x--){
//				// make perpendicular edges
//				if(right_start != 0){
//					footprint[x][right_start] = 1;
//				}
//				if(right_end != dims.y - 1){
//					footprint[x][right_end] = 1;
//				}
//			}
//
//		}
//		// otherwise just make normal border
//		else{
//			footprint[dims.x - 1][y] = 1;
//		}
//	}
//
//	}
//	else{
//
//	}




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

}

void Footprint::copy_footprint(int** foot){
	for(int x = 0; x < dimensions.x; x++){
		for(int y = 0; y < dimensions.y; y++){
			foot[x][y] = footprint[x][y];
		}
	}
}

void Footprint::alloc_footprint(Pair<int> dims){
	footprint = new int*[dims.x];
	for (int i = 0; i < dims.x; i++){
		footprint[i] = new int[dims.y]();
	}
	is_alloced = true;
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

void Footprint::print(){
	for (int y = 0; y < dimensions.y; y++){
	for (int x = 0; x < dimensions.x; x++){
			cout << footprint[x][y];
	}
	cout << endl;
	}
	cout << endl;
}
