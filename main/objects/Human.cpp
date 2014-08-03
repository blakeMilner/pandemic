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

