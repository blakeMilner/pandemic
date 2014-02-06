#ifndef MAP_SETTINGS_HPP
#define MAP_SETTINGS_HPP

namespace Map_settings{
	static int map_len[2] = {90, 90};
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
