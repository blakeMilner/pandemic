#ifndef OBJECT_ENUM
#define OBJECT_ENUM

#include "../containers/containers.hpp"

enum Map_symbol {
	INVALID = 'I',
	HUMAN = 'H',
	ZOMBIE = 'Z',
	CHARACTER = 'C',
	BUILDING = 'O',
	OBJECT = 'o',
	OBSTACLE = '*',
	OCCLUDED = '#',
	EMPTY = ' '
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

// navigation helpers
class NAV{
public:
	static Nav_symbol get_poss_dir(int);
	static const int NUM_POSS_DIR = 8;

	static bool apply_direction(Pair&, Nav_symbol);
	static Pair get_direction(Nav_symbol);
	static Nav_symbol get_direction(Pair);
	static Nav_symbol get_left(Nav_symbol);
	static Nav_symbol get_right(Nav_symbol);
	static bool is_occluded(Map_symbol);
private:
	static const Nav_symbol POSS_DIR[NUM_POSS_DIR];
};

#endif
