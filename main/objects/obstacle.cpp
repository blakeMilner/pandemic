#include "obstacle.hpp"
#include <iostream>

Obstacle::Obstacle(Map_symbol sym):
pObject(pObject(Pair<int>(-1,-1), sym)),
footprint(Footprint())
{}

Obstacle::Obstacle(const Obstacle& o):
pObject(o),
footprint(o.footprint){
}

Obstacle::Obstacle(Pair<int> coor, Pair<int> dimens, Map_symbol sym):
pObject(coor, sym),
footprint(Footprint(dimens))
{
}

Obstacle::~Obstacle(){}

Map_symbol Obstacle::get_footprint(int x, int y){
	if(x >= 0 && y >= 0 && x < footprint.dimensions.x && y < footprint.dimensions.y){
		return(footprint.footprint[x][y]);
	}
	else{
		return(INVALID);
	}
}

Pair<int> Obstacle::get_dimensions(){
	return(footprint.dimensions);
}
