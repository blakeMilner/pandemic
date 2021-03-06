#ifndef OBJECT_ENUM
#define OBJECT_ENUM

#include "../helpers/containers.hpp"

enum Map_symbol {
	HUMAN = 'H',
	INFECTED = 'N',
	ZOMBIE = 'Z',
	CHARACTER = 'C',

	BUILDING = 'O',
	BUILDING_DOOR = '$',
	BUILDING_WALL = '|',
	BUILDING_INDOOR = '^',

	OBJECT = 'o',
	OBSTACLE = '*',

	PASSABLE = 'P',
	OCCLUDED = '#',
	EMPTY = ' ',
	INVALID = 'I'
};

enum Nav_symbol {
	LEFT = -1,
	RIGHT = 1,

	NORTH = 1,
	NORTHE = 2,
	EAST = 3,
	SOUTHE = 4,
	SOUTH = 5,
	SOUTHW = 6,
	WEST = 7,
	NORTHW = 8,

	NONE = 0
};


int sgn(float);

// navigation helpers
class NAV{
public:
	static bool block_is(Map_symbol, Map_symbol);
	static Nav_symbol get_poss_dir(int);
	static const int NUM_POSS_DIR = 8;

	static bool apply_direction(Pair<int>&, Nav_symbol);
	static Pair<int> get_direction(Nav_symbol);
	static Nav_symbol get_direction(Pair<int>);
	static Nav_symbol get_left(Nav_symbol);
	static Nav_symbol get_right(Nav_symbol);
private:
	static const Nav_symbol POSS_DIR[NUM_POSS_DIR];
};

#endif
