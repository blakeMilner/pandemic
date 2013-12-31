#ifndef BUILDING_HPP
#define BUILDING_HPP

using namespace std;

#include "objects.hpp"
#include "obstacle.hpp"
#include "../containers/containers.hpp"

class Building : public Obstacle {
public:
	Building();
	Building(const Building&);
	Building(Pair, Pair, queue<Pair>&);
private:

};

#endif


