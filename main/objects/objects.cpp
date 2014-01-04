#include "objects.hpp"
#include <iostream>

pObject::pObject(Pair<int> coor, Map_symbol sym):
coor(coor),
SYMBOL(sym)
{

}

Pair<int> pObject::get_coor(){
	return(coor);
}

void pObject::set_coor(Pair<int> newcoor){
	coor = newcoor;
}

Map_symbol pObject::get_symbol(){
	return(SYMBOL);
}
