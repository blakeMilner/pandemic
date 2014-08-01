#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"
#include "../mapping/map.hpp"
#include "../enums/object_enum.hpp"

using namespace std;

class Human : public Character{
public:
	// TODO: remove automatic pair setting for all objects - it's unsafe
	Human(Pair<int> = Pair<int>(-1,-1), Map_symbol = HUMAN, Stats = Char_stats::DEFAULT_HUMAN);

	virtual void exec();
private:
	virtual void init_stats(Stats);
};



class Infected: public Human{
public:
	Infected(Pair<int> = Pair<int>(-1,-1), Map_symbol = INFECTED, Stats = Char_stats::DEFAULT_INFECTED);
	Infected(Human*);

	virtual void exec();
	bool infected_time_over();
private:
	virtual void init_stats(Stats);

	int infected_time;
};


#endif
