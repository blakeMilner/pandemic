#include "Human.hpp"

Human::Human(Pair newcoor) :
Character(newcoor, HUMAN)
{
	Human::init_stats();
}

void Human::exec(){
	Character::exec();

	Character::random_walk();
}

void Human::init_stats(){

}
