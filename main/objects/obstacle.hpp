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
	Obstacle(Pair<int>, Pair<int>, Map_symbol = OBSTACLE);
	virtual ~Obstacle();
	Map_symbol get_footprint(int, int);
	Pair<int> get_dimensions();
private:
	Footprint footprint;
};

#endif
