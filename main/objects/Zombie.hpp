#ifndef ZOMBIE_HPP
#define ZOMBIE_HPP

#include "character.hpp"
#include "../mapping/map.hpp"
#include "../enums/object_enum.hpp"

using namespace std;

class Zombie : public Character{
public:
	Zombie(Pair = Pair(-1,-1));
	void exec();
	void bite(Pair);
private:
	virtual void init_stats();
};


#endif
