#include "map_attributes.hpp"

ObjectBlock::ObjectBlock(int newID, Map_symbol new_symbol):
ID(newID),
symbol(new_symbol)
{}

ObjectBlock::~ObjectBlock(){}

void ObjectBlock::change_symbol(Map_symbol newsym){	symbol = newsym; }

Map_symbol ObjectBlock::get_symbol(){ return symbol; }

int ObjectBlock::get_ID(){ return ID; }
