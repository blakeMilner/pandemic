#include "mRegion.hpp"


// PROBLEM: WHAT IF A CHARACTER GETS ADDED TO A NEW REGION AND GETS EXECUTED AGAIN?
// ANSWER: JUST MAKE A SEPARATE LIST THAT CONTAINS ALL ACTIVE CHARACTERS AND ITERATE THORUGH IT:
// 		IF A NEW CHARACTER NEEDS TO BE ADDED AND EXECUTED, PUT IT AT THE END;

mRegion::mRegion(int regionlen, int x, int y) :
coor(Pair(x, y)),
len(regionlen)
{
}

mRegion::~mRegion(){
	while(!characters.empty()) delete characters.front(), characters.pop_front();
	while(!obstacles.empty()) delete obstacles.front(), obstacles.pop_front();
}

void mRegion::insert_character(Character* c){
	characters.push_front(c);
}

void mRegion::insert_obstacle(Obstacle* o){
	obstacles.push_front(o);
}

void mRegion::remove_character(Character* c){
	characters.remove(c);
}

void mRegion::remove_obstacle(Obstacle* o){
	obstacles.remove(o);
}

Pair mRegion::get_coor(){
	return(coor);
}

int mRegion::get_length(){
	return(len);
}






