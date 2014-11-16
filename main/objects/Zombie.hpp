#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "character.hpp"
#include "../mapping/map.hpp"
#include "../enums/object_enum.hpp"

using namespace std;

class Zombie : public Character{
public:
	Zombie(Pair<int> = Pair<int>(-1,-1), Map_symbol = ZOMBIE, Stats = Char_stats::RANDOM_CHARACTER(ZOMBIE));
	virtual void exec();
	void bite(Pair<int>);
private:
	virtual void init_stats(Stats);
};


#endif
