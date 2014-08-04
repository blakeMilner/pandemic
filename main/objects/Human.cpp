#include "Human.hpp"

Human::Human(Pair<int> newcoor, Map_symbol sym, Stats stats) :
Character(newcoor, sym, stats)
{
	Human::init_stats(stats);
}

void Human::exec(){
	Pair<int> pos;
	int distance;


	Character::exec();

	if(Character::find_nearest(pos, ZOMBIE)){
		Character::go_away_from(pos, ZOMBIE);
	}
	else if(distance = Character::find_nearest(pos, BUILDING_DOOR)){
		if(distance > 1){
			Character::go_towards_state(pos, BUILDING_DOOR);
		}
		else{
			Character::go_towards_state(pos, BUILDING_DOOR);
		}
	}
	else
		Character::random_walk();

}

void Human::init_stats(Stats stats){

}

