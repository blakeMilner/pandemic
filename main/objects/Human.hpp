#ifndef HUMAN_HPP
#define HUMAN_HPP

#include "character.hpp"
#include "../enums/object_enum.hpp"

using namespace std;

class Human : public Character{
public:
	Human(Pair<int> = Pair<int>(-1,-1));
	void exec();
private:
	virtual void init_stats();
};


#endif
