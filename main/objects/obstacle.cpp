#include "obstacle.hpp"
#include <iostream>

Obstacle::Obstacle(Map_symbol sym):
pObject(pObject(Pair(-1,-1), sym)),
footprint(Footprint())
{}

Obstacle::Obstacle(const Obstacle& o):
pObject(o),
footprint(o.footprint){
}

Obstacle::Obstacle(Pair coor, Pair dimens, queue<Pair>& vertices, Map_symbol sym):
pObject(coor, sym),
footprint(Footprint(vertices, dimens))
{
}

Obstacle::~Obstacle(){}

int Obstacle::get_footprint(int x, int y){
	if(x >= 0 && y >= 0 && x < footprint.dimensions.x && y < footprint.dimensions.y){
		return(footprint.footprint[x][y]);
	}
	else{
		return(INVALID);
	}
}

Pair Obstacle::get_dimensions(){
	return(footprint.dimensions);
}
