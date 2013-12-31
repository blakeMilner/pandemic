#include "Zombie.hpp"

Zombie::Zombie(Pair coor):
Character(coor, ZOMBIE)
{
	Zombie::init_stats();
}

void Zombie::exec(){
	Pair pos;

	Character::exec();

	if(Character::find_nearest(pos, HUMAN)){
		if(is_adjacent(pos)){
			Zombie::bite(pos);
		}
		else
			Character::go_towards(pos);
	}
	else
		Character::random_walk();
}

void Zombie::bite(Pair pos){
	MapServer::bite_player(pos + coor - stats.VIS_RAD);
}

void Zombie::init_stats(){

}
