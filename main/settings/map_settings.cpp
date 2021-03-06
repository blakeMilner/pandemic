/*
 * map_settings.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: blake
 */


/*
 * map_settings.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: blake
 */

#include "map_settings.hpp"

Pair<int> Map_settings::map_len = Pair<int>(300, 300); // put in condition to check that division is even
const Pair<int> Map_settings::MIN_WORLD_DIMS = Pair<int>(50,50);
const Pair<int> Map_settings::MAX_WORLD_DIMS = Pair<int>(5000,5000); // TODO: need to put in check that this isn't over actual dims

const int Map_settings::MIN_REGION_LEN = 50;
int Map_settings::region_len = 100;
const int Map_settings::REGION_LEN_INCREMENT = 10;
const int Map_settings::MAX_REGION_LEN = 5000;
Pair<int> Map_settings::next_game_world_size = Map_settings::map_len;
int Map_settings::next_game_region_len = Map_settings::region_len;

// CHARACTER GENERATION SETTINGS
int Map_settings::min_human_density = 6;
int Map_settings::max_human_density = 12;
int Map_settings::min_zombie_density = 4;
int Map_settings::max_zombie_density = 8;

// BUILDING GENERATION SETTINGS
const int Map_settings::MAX_LENGTH_DIFF = 3;
int Map_settings::min_building_density = 10;
int Map_settings::max_building_density = 35;

int Map_settings::min_footprint_len = 14; // possible put these as pairs???
int Map_settings::max_footprint_len = 32;
int Map_settings::min_building_len = 10;
int Map_settings::max_building_len = 28;

int Map_settings::blocks_per_building = 12;
int Map_settings::max_build_placement_tries = 100;
float Map_settings::door_probability = 0.002;

bool Map_settings::settings_check(){
	int footprint_span = max_footprint_len - min_footprint_len;
	int building_span = max_building_len - min_building_len;

	if( building_span - footprint_span > MAX_LENGTH_DIFF){
		return false;
	}

	// if map is not evenly divisble by regions
	if( ! (Map_settings::map_len % Map_settings::region_len).elements_equal() ){
		return false;
	}

	return true;
}

void Map_settings::correct_settings(){
	// correct for if building won't fit inside footprint
	int footprint_span = max_footprint_len - min_footprint_len;
	int building_span = max_building_len - min_building_len;

	while (building_span - footprint_span > MAX_LENGTH_DIFF){
		max_footprint_len++;

		footprint_span = max_footprint_len - min_footprint_len;
	}

	// keep increasing the region length until it tiles the map
	while( ! (Map_settings::map_len % Map_settings::region_len).elements_equal() ){
		Map_settings::region_len++;
	}
}

