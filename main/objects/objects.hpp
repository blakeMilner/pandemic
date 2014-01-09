#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "../mapping/map_attributes.hpp"
#include "../containers/containers.hpp"
#include "../enums/object_enum.hpp"
#include "../settings/character_stats.hpp"
#include "attributes.hpp"
#include <queue>
#include <math.h>

using namespace std;

class pObject{
public:
	pObject(Pair<int> = Pair<int>(-1,-1), Map_symbol = OBJECT);
	Pair<int> get_coor();
	void set_coor(Pair<int>);
	Map_symbol get_symbol();
protected:
	Pair<int> coor;
	Map_symbol SYMBOL;
};

#endif