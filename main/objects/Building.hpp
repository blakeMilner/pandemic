#ifndef BUILDING_HPP
#define BUILDING_HPP

using namespace std;

#include "objects.hpp"
#include "obstacle.hpp"
#include "../helpers/containers.hpp"

class Building : public Obstacle {
public:
	Building();
	Building(const Building&);
	Building(Pair<int>, Pair<int>);
private:

};

#endif


