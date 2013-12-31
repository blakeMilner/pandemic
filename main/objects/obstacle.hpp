#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#include <queue>
#include "../objects/objects.hpp"
#include "../containers/containers.hpp"
#include "../enums/object_enum.hpp"
#include "../mapping/map_attributes.hpp"

using namespace std;

class Obstacle : public pObject{
public:
	Obstacle(Map_symbol = OBSTACLE);
	Obstacle(const Obstacle&);
	Obstacle(Pair, Pair, queue<Pair>&, Map_symbol = OBSTACLE);
	virtual ~Obstacle();
	int get_footprint(int, int);
	Pair get_dimensions();
private:
	Footprint footprint;
};

#endif
