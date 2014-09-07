#ifndef MAP_SETTINGS_HPP
#define MAP_SETTINGS_HPP

#include "../helpers/containers.hpp"

// TODO: put these in XML file to quickly change without recompiling
class Map_settings{
// TODO: GO THROUGH AND DEFINE EXTREME VALUES AS THEY RELATE TO EACH OTHER
	//TODO: make some of these private/const
	// lowering map_len below 600 produces blank screen???
public:

static Pair<int> map_len; // put in condition to check that division is even
static int region_len;

// CHARACTER GENERATION SETTINGS
static int min_human_density;
static int max_human_density;
static int min_zombie_density;
static int max_zombie_density;

// BUILDING GENERATION SETTINGS
static const int MAX_LENGTH_DIFF;
static int min_building_density;
static int max_building_density;

static int min_footprint_len; // possible put these as pairs???
static int max_footprint_len;
static int min_building_len;
static int max_building_len;

static int blocks_per_building;
static int max_build_placement_tries;
static float door_probability;

static bool settings_check();
static void correct_settings();

	//FOR CMD SMALL SCALE TESTING:
//	static Pair<int> map_lenPair<int>(10, 10); // put in condition to check that division is even
//	static int region_len10;
//
//	// check random num generater because these produce 0 sometimes?
//	static int min_human_density3;
//	static int max_human_density3;
//	static int min_zombie_density1;
//	static int max_zombie_density1;
//
//	static int min_building_density0;
//	static int max_building_density0;
//	static int min_footprint_len10;
//	static int max_footprint_len30; // need to make this function of region len
//	static int min_building_len3;
//	static int max_building_len25;
//	static int blocks_per_building10;
//	static float door_chance0.05;
};


#endif
