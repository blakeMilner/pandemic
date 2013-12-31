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

Footprint::Footprint(queue<Pair>& ver, Pair dims):
dimensions(dims)
{
	alloc_footprint(dims);

	Pair direction, pos;
	Pair it;

	ver.push(ver.front());
	Pair prev_vert = ver.front();
	ver.pop();

	while(!ver.empty()){
		it = ver.front();
		ver.pop();

		footprint[it.x][it.y] = 1;

		direction.x = ((prev_vert.x < it.x) - (prev_vert.x > it.x));
		direction.y = ((prev_vert.y < it.y) - (prev_vert.y > it.y));

		pos = it;
		while((pos.x != prev_vert.x) || (pos.y != prev_vert.y)){
			pos.x -= direction.x;
			pos.y -= direction.y;

			footprint[pos.x][pos.y] = 1;
		}

		prev_vert.x = it.x;
		prev_vert.y = it.y;
	}

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

void Footprint::alloc_footprint(Pair dims){
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
