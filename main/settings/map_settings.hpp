#ifndef MAP_SETTINGS_HPP
#define MAP_SETTINGS_HPP

namespace Map_settings{
	static Pair<int> map_len = Pair<int>(600, 600); // put in condition to check that division is even
	static int region_len = 30;

	static int min_human_density = 4;
	static int max_human_density = 7;
	static int min_zombie_density = 1;
	static int max_zombie_density = 2;

	static int min_building_density = 2;
	static int max_building_density = 4;
	static int min_building_len = 5;
	static int max_building_len = 20; // need to make this function of region len
};


#endif
