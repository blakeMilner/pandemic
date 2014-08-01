#ifndef MAP_ATTRIBUTES_HPP
#define MAP_ATTRIBUTES_HPP

#include <cstdlib>
#include "../enums/object_enum.hpp"

#define INVALID_ID -100
#define EMPTY_TYPE_ID 1

class Map; // to resolve circular dependency

class ObjectBlock{
public:
	friend class Map;

	ObjectBlock(int = INVALID_ID, Map_symbol = INVALID);
	virtual ~ObjectBlock();

	Map_symbol get_symbol();
	void change_symbol(Map_symbol);
	int get_ID();
private:
	int ID;
	Map_symbol symbol;
};


#endif
