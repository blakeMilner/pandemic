#include "objects.hpp"
#include <iostream>

pObject::pObject(Pair coor, Map_symbol sym):
coor(coor),
SYMBOL(sym)
{

}

Pair pObject::get_coor(){
	return(coor);
}

void pObject::set_coor(Pair newcoor){
	coor = newcoor;
}

Map_symbol pObject::get_symbol(){
	return(SYMBOL);
}
