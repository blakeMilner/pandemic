#ifndef CHARACTERS_HPP
#define CHARACTERS_HPP

#include "objects.hpp"
#include <cmath>

// Abstract class
class Character : public pObject{
public:
	Character(Pair<int>, Map_symbol = CHARACTER, Stats = Char_stats::DEFAULT_CHARACTER);
	virtual ~Character();
	virtual void exec();

	//TODO: change to getter/setters, make this private
	Stats stats;

protected:
	// TODO: vision field array shouldn't be create for each instance
	Map_symbol** vision_field;
	// TODO: put vision_len in stats class
	int vision_len;

	virtual void init_stats(Stats) = 0;

	Map_symbol get_directional_block(Nav_symbol);
	void move_direction(Nav_symbol);

	void go_towards_state(Pair<int> pos, Map_symbol state);
	void go_away_from(Pair<int> pos, Map_symbol state);
	int find_nearest(Pair<int>&, Map_symbol);
	void random_walk();
	bool is_adjacent(Pair<int>&);
	void print_vision();

};


#endif
