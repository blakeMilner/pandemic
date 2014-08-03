/*
 * Infected.hpp
 *
 *  Created on: Aug 3, 2014
 *      Author: blake
 */

#ifndef INFECTED_HPP_
#define INFECTED_HPP_


#include "Human.hpp"
#include "../enums/object_enum.hpp"

using namespace std;


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

#endif /* INFECTED_HPP_ */
