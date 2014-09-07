/*
 * MapStats.cpp
 *
 *  Created on: Aug 3, 2014
 *      Author: blake
 */
#include "MapStats.hpp"

MapStats::MapStats()
{
	// TODO: need an enum class to do this stuff (like iterate over)... also implement NAV in this!
	num_iter = 0;
	num_bites = 0;
	num_characters[HUMAN] = 0;
	num_characters[INFECTED] = 0;
	num_characters[ZOMBIE] = 0;
}

MapStats::~MapStats() {
}



