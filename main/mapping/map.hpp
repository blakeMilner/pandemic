#ifndef MAP_HPP
#define MAP_HPP

#include <algorithm>
#include <list>
#include <map>
#include <vector>
#include <stack>
#include <iostream>
#include <string>

#include "map_attributes.hpp"
#include "mRegion.hpp"
#include "MapStats.hpp"
#include "map_server.hpp"
#include "../objects/objects.hpp"
#include "../objects/obstacle.hpp"
#include "../objects/character.hpp"
#include "../objects/Building.hpp"
#include "../settings/map_settings.hpp"
#include "../helpers/rng.hpp"
#include "../enums/object_enum.hpp"
#include "../../gui/gui_settings.hpp"

using namespace std;

// definitions for the index layers of the blockmap
#define TOP_LAYER 0
#define BOTTOM_LAYER 1

class Map{
public:
	// MapServer accesses private methods directly to interact with code implemented
	// in character classes
	friend class MapServer;

	Map();
	virtual ~Map();
	void iterate();
	void print_map();
	bool check_game();
	void copy_ROI(Map_symbol**, Pair<int>, Pair<int>);
	MapStats get_map_stats();


private:
	int xmap_len, ymap_len;
	mRegion*** regions;
	int num_x_regions, num_y_regions;


	ObjectBlock**** blockmap;
	list<ObjectBlock*> all_object_blocks;

	map<int, pObject*> IDhash;
	list<Character*> characters;
	int NEXT_EMPTY_ID;
	pObject empty_object;
	// EMPTY_BLOCK is our singleton block that convers all empty spaces (most of the map) to save memory
	ObjectBlock EMPTY_BLOCK;

	ObjectBlock* new_object_block(int, Map_symbol);

	MapStats map_stats;

	Map_symbol get_symbol(int, int);
	Map_symbol get_symbol(Pair<int>);
	int get_ID(int, int);
	int get_ID(Pair<int>);


	void add_character(Map_symbol type, Pair<int> coor);
	Character* new_character_instance(Map_symbol type, Pair<int> coor);
	void delete_character(pObject* c, int ID);
	void add_obstacle(Obstacle*);
	void add_block(ObjectBlock* block, Pair<int> coor);
	void remove_block(Pair<int> coor);

	Pair<int> find_region(Pair<int>);
	bool regional_bounds_check(Pair<int>);
	bool regional_bounds_check(int, int);
	bool overall_bounds_check(Pair<int>);
	bool overall_bounds_check(int, int);

	// TERRAIN GENERATION
	void regionalize();
	void populate();
	void make_buildings();
	void make_characters();
	bool placement_is_clear(Pair<int>, Pair<int>);

	// functions meant for MapServer
	void move_character(Character*, Pair<int>&);
	void infect_player(int ID);
	void convert_infected(Character*);
};


#endif
