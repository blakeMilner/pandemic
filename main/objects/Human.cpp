#include "Human.hpp"

Human::Human(Pair<int> newcoor, Map_symbol sym, Stats stats) :
Character(newcoor, sym, stats)
{
	Human::init_stats(stats);
}

void Human::exec(){
	Pair<int> pos;

	Character::exec();

	if(Character::find_nearest(pos, ZOMBIE)){
		Character::go_away_from(pos);
	}
	else
		Character::random_walk();

}

void Human::init_stats(Stats stats){

}


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
