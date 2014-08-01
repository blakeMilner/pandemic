#ifndef MAP_SETTINGS_HPP
#define MAP_SETTINGS_HPP

#include "../containers/containers.hpp"

// TODO: put these in XML file to quickly change without recompiling
class Map_settings{
// TODO: GO THROUGH AND DEFINE EXTREME VALUES AS THEY RELATE TO EACH OTHER
	//TODO: make some of these private/const
	// lowering map_len below 600 produces blank screen???
public:

static Pair<int> map_len = Pair<int>(120, 120); // put in condition to check that division is even
static int region_len = 60;

// CHARACTER GENERATION SETTINGS
static int min_human_density = 6;
static int max_human_density = 12;
static int min_zombie_density = 4;
static int max_zombie_density = 8;

// BUILDING GENERATION SETTINGS
static const int MAX_LENGTH_DIFF = 3;
static int min_building_density = 2;
static int max_building_density = 4;

static int min_footprint_len = 9; // possible put these as pairs???
static int max_footprint_len = 12;
static int min_building_len = 7;
static int max_building_len = 10;

static int blocks_per_building = 3;
static int max_build_placement_tries = 100;
static float door_probability = 0.05;

static bool settings_check();

	//FOR CMD SMALL SCALE TESTING:
//	static Pair<int> map_len = Pair<int>(10, 10); // put in condition to check that division is even
//	static int region_len = 10;
//
//	// check random num generater because these produce 0 sometimes?
//	static int min_human_density = 3;
//	static int max_human_density = 3;
//	static int min_zombie_density = 1;
//	static int max_zombie_density = 1;
//
//	static int min_building_density = 0;
//	static int max_building_density = 0;
//	static int min_footprint_len = 10;
//	static int max_footprint_len = 30; // need to make this function of region len
//	static int min_building_len = 3;
//	static int max_building_len = 25;
//	static int blocks_per_building = 10;
//	static float door_chance = 0.05;
};


#endif
