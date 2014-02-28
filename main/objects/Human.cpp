#include "Human.hpp"

Human::Human(Pair<int> newcoor) :
Character(newcoor, HUMAN)
{
	Human::init_stats();
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

void Human::init_stats(){

}
