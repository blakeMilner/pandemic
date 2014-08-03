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

		// determine if we're overlapping a block
		for(int x = corner.x; x < corner.x + dimen.x; x++){
			for(int y = corner.y; y < corner.y + dimen.y; y++){
				if(footprint[x][y]){
					hit_block = true;
				}
			}
		}

		/* KEY HERE:
		 *  0 - empty block
		 *  1 - block that we placed
		 *  2 - indoor block we must remove later
		 */

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

	/* select and carve out door, keep trying and updating door chance if not found
	 *
	 */

	// we want to always place at least one door
	int doors_to_skip = 0;

	for(vector<Pair<int> >::iterator it = borders.begin(); it != borders.end(); it++){
		door_found = false;

		// door is on straight edge
		if(footprint[(*it).x - 1][(*it).y] == 1 		and
			footprint[(*it).x + 1][(*it).y] == 1 		and
			footprint[(*it).x - 1][(*it).y - 1] == 0 	and
			footprint[(*it).x + 1][(*it).y + 1] == 0 	and
			footprint[(*it).x][(*it).y - 1] == 0 		and
			footprint[(*it).x][(*it).y + 1] == 0){

				door_found = true;
		}
		// door is on other straight edge
		else if(!footprint[(*it).x][(*it).y - 1] and
			!footprint[(*it).x][(*it).y + 1] and
			footprint[(*it).x - 1][(*it).y - 1] == 0 and
			footprint[(*it).x + 1][(*it).y + 1] == 0 and
			footprint[(*it).x - 1][(*it).y] == 0 and
			footprint[(*it).x + 1][(*it).y] == 0){
				door_found = true;
		}

		// carve door and exit if found
		if( door_found and (0 == doors_to_skip--) ){
			footprint[(*it).x][(*it).y] = 0;
			doors_to_skip = RNG::random_num(5, dims.x / 2);
		}
	}

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
