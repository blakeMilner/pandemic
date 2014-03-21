#include "Zombie.hpp"

Zombie::Zombie(Pair<int> coor):
Character(coor, ZOMBIE)
{
	Zombie::init_stats();
}

void Zombie::exec(){
	Pair<int> pos;

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

void Zombie::bite(Pair<int> pos){
	MapServer::Instance()->bite_player(pos + coor);
}

void Zombie::init_stats(){

}
