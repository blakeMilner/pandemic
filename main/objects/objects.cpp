#include "objects.hpp"
#include <iostream>

pObject::pObject(Pair<int> coor, Map_symbol sym):
coor(coor),
SYMBOL(sym)
{

}

pObject::~pObject(){}

Pair<int> pObject::get_coor(){
	return(coor);
}

void pObject::set_coor(Pair<int> newcoor){
	coor = newcoor;
}

Map_symbol pObject::get_symbol(){
	return(SYMBOL);
}

void pObject::change_symbol(Map_symbol newsym){
	SYMBOL = newsym;
}
