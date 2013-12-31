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
	pObject(Pair = Pair(-1,-1), Map_symbol = OBJECT);
	Pair get_coor();
	void set_coor(Pair);
	Map_symbol get_symbol();
protected:
	Pair coor;
	Map_symbol SYMBOL;
};

#endif
