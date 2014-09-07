/*
 * MapStats.hpp
 *
 *  Created on: Aug 3, 2014
 *      Author: blake
 */

#ifndef MAPSTATS_HPP_
#define MAPSTATS_HPP_


#include "../enums/object_enum.hpp"

#include <map>

class MapStats{
public:
	int num_bites;
	std::map<Map_symbol, int> num_characters;

	int num_iter;

	MapStats();
	~MapStats();
};



#endif /* MAPSTATS_HPP_ */
