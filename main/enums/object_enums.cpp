#include "object_enum.hpp"

const Nav_symbol NAV::POSS_DIR[NUM_POSS_DIR	] = {
	NORTH,
	NORTHE,
	EAST,
	SOUTHE,
	SOUTH,
	SOUTHW,
	WEST,
	NORTHW };

int sgn(float x){ return (x > 0) - (x < 0); }

bool NAV::block_is(Map_symbol s, Map_symbol type_sym){
	// change this to be a list full of occluded types, search list
	switch(type_sym){
	case OCCLUDED:
		if(s == OCCLUDED or s == OBSTACLE or s == OBJECT or s == BUILDING or s == INVALID)
			return true;
		else
			return false;
	case PASSABLE:
		if(s == EMPTY or s == BUILDING_DOOR or s == BUILDING_INDOOR	)
			return true;
		else
			return false;
	}
}

Nav_symbol NAV::get_poss_dir(int i){
	if( i >= 0 && i < NUM_POSS_DIR)	return(POSS_DIR[i]);
	else							return(NONE);
}

Nav_symbol NAV::get_left(Nav_symbol n){
	if(n > NORTH and n <= NORTHW) 	return (NAV::POSS_DIR[n - 1]);
	else if (n == NORTH)			return (NORTHW);
	else							return (NONE);
}

Nav_symbol NAV::get_right(Nav_symbol n){
	if(n >= NORTH and n < NORTHW) 	return (NAV::POSS_DIR[n + 1]);
	else if (n == NORTHW)			return (NORTH);
	else							return (NONE);
}

bool NAV::apply_direction(Pair<int>& index, Nav_symbol dir){
	switch(dir){
	case(NORTH): 	index.y -= 1; break;
	case(NORTHE): 	index.x += 1, index.y -= 1; break;
	case(EAST): 	index.x += 1; break;
	case(SOUTHE): 	index.x += 1, index.y += 1; break;
	case(SOUTH): 	index.y += 1; break;
	case(SOUTHW):	index.x -= 1, index.y += 1; break;
	case(WEST): 	index.x -= 1 ;break;
	case(NORTHW): 	index.x -= 1, index.y -= 1; break;
	default: 		return false;
	}

	return true;
}

Pair<int> NAV::get_direction(Nav_symbol dir){
	switch(dir){
	case(NORTH): 	return Pair<int>(0,-1); break;
	case(NORTHE): 	return Pair<int>(1,-1); break;
	case(EAST): 	return Pair<int>(1,0); break;
	case(SOUTHE): 	return Pair<int>(1,1); break;
	case(SOUTH): 	return Pair<int>(0,1); break;
	case(SOUTHW):	return Pair<int>(-1,1); break;
	case(WEST): 	return Pair<int>(-1,0); break;
	case(NORTHW): 	return Pair<int>(-1,-1); break;
	}

	return Pair<int>(0,0);
}

Nav_symbol NAV::get_direction(Pair<int> dir){
	if(dir.x == 0 and dir.y == -1) 	return NORTH;
	if(dir.x == 1 and dir.y == -1) return NORTHE;
	if(dir.x == 1 and dir.y == 0) return EAST;
	if(dir.x == 1 and dir.y == 1) return SOUTHE;
	if(dir.x == 0 and dir.y == 1) return SOUTH;
	if(dir.x == -1 and dir.y == 1) return SOUTHW;
	if(dir.x == -1 and dir.y == 0) return WEST;
	if(dir.x == -1 and dir.y == -1) return NORTHW;

	return NONE;
}

