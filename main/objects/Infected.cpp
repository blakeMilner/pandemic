/*
 * Infected.cpp
 *
 *  Created on: Aug 3, 2014
 *      Author: blake
 */

#include "Infected.hpp"

Infected::Infected(Human* human):
Human(human->get_coor(), INFECTED, human->stats),
infected_time(0)
{
	Infected::init_stats(Char_stats::DEFAULT_INFECTED);
}

void Infected::exec(){
	Human::exec();

	infected_time++;
}

void Infected::init_stats(Stats stats){
	this->stats.TIME_TO_CONVERSION = stats.TIME_TO_CONVERSION;
}

bool Infected::infected_time_over(){
	return(infected_time >= stats.TIME_TO_CONVERSION);
}


