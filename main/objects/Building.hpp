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
	Building(Pair<int>, Pair<int>, queue<Pair<int> >&);
private:

};

#endif

