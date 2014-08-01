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

Pair<int> Map_settings::map_len = Pair<int>(120, 120); // put in condition to check that division is even
int Map_settings::region_len = 60;

// CHARACTER GENERATION SETTINGS
int Map_settings::min_human_density = 6;
int Map_settings::max_human_density = 12;
int Map_settings::min_zombie_density = 4;
int Map_settings::max_zombie_density = 8;

// BUILDING GENERATION SETTINGS
const int Map_settings::MAX_LENGTH_DIFF = 3;
int Map_settings::min_building_density = 2;
int Map_settings::max_building_density = 4;

int Map_settings::min_footprint_len = 9; // possible put these as pairs???
int Map_settings::max_footprint_len = 12;
int Map_settings::min_building_len = 7;
int Map_settings::max_building_len = 10;

int Map_settings::blocks_per_building = 3;
int Map_settings::max_build_placement_tries = 100;
float Map_settings::door_probability = 0.05;

bool Map_settings::settings_check(){
	if( max_footprint_len - min_footprint_len < MAX_LENGTH_DIFF
			or max_building_len - min_building_len < MAX_LENGTH_DIFF){
					return false;
	}

	return true;
}


