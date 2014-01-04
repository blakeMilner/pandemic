#ifndef CHARACTERS_HPP
#define CHARACTERS_HPP

#include "objects.hpp"
#include <cmath>

class Character : public pObject{
public:
	Character(Pair<int>, Map_symbol = CHARACTER);
	virtual ~Character();
	virtual void exec();
protected:
	Map_symbol** vision_field;
	int vision_len;

	virtual void init_stats() = 0;

	Map_symbol get_direction(Nav_symbol);
	void move_direction(Nav_symbol);

	void go_towards(Pair<int>);
	void go_away_from(Pair<int>);
	bool find_nearest(Pair<int>&, Map_symbol);
	bool block_is(Nav_symbol, Map_symbol);
	void random_walk();
	bool is_adjacent(Pair<int>&);
	void print_vision();

	Stats stats;
};


#endif
