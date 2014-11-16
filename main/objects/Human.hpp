#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"
#include "../mapping/map.hpp"
#include "../enums/object_enum.hpp"

using namespace std;

class Human : public Character{
public:
	// TODO: remove automatic pair setting for all objects - it's unsafe
	Human(Pair<int> = Pair<int>(-1,-1), Map_symbol = HUMAN, Stats = Char_stats::RANDOM_CHARACTER(HUMAN));

	virtual void exec();
private:
	virtual void init_stats(Stats);
};


#endif
