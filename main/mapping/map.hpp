#ifndef MAP_HPP
#define MAP_HPP

#include <algorithm>
#include <list>
#include <map>
#include <iostream>

#include "map_attributes.hpp"
#include "mRegion.hpp"
#include "../objects/objects.hpp"
#include "../objects/obstacle.hpp"
#include "../objects/character.hpp"
#include "../objects/Building.hpp"
#include "../objects/Human.hpp"
#include "../objects/Zombie.hpp"
#include "../settings/map_settings.hpp"
#include "../helpers/rng.hpp"
#include "../enums/object_enum.hpp"
#include "../../gui/gui_settings.hpp"

using namespace std;

#define MAX_TRIES 100


class MapStats{
public:
	int num_humans;
	int num_zombies;
	int num_bites;

	MapStats():
		num_humans(0), num_zombies(0),
		num_bites(0) {}
};


class Map{
public:
	friend class MapServer;

	Map();
	~Map();
	void iterate();
	void print_map();
	bool check_game();
	void copy_ROI(Map_symbol**, Pair<int>, Pair<int>);


private:
	int xmap_len, ymap_len;
	ObjectBlock*** blockmap;
	map<int, pObject*> IDhash;
	list<Character*> characters;
	int newID;

	pObject empty_object;
	ObjectBlock EMPTY_BLOCK;
	ObjectBlock* new_object_block(int, Map_symbol);
	list<ObjectBlock*> all_object_blocks;

	mRegion*** regions;
	int num_x_regions, num_y_regions;

	MapStats map_stats;

	Map_symbol get_symbol(int, int);
	Map_symbol get_symbol(Pair<int>);
	int get_ID(int, int);
	int get_ID(Pair<int>);
	void add_zombie(Pair<int>);
	void add_human(Pair<int>);
	void add_character(Character*);
	void delete_human(pObject*, int);
	void delete_character(Character*, int);
	void add_obstacle(Obstacle*);
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
	Pair<int> find_empty_path(int, int, int, int);
	bool placement_is_clear(Pair<int>, Pair<int>);
	void remove_corners(Pair<int>&, queue<Pair<int> >&);
	void trace_outline(Pair<int>&, queue<Pair<int> >&);
	void turn_left(queue<Pair<int> >&, Pair<int>&, Pair<int>&);
	void turn_right(queue<Pair<int> >&, Pair<int>&, Pair<int>&);

	// functions meant for MapServer
	void move_character(Character*, Pair<int>&);
	void infect_player(int ID);
};

class MapServer{
public:
	friend class Map;
	static Map_symbol get_symbol(int, int);
	static int get_ID(int, int);
	static void copy_field(Map_symbol**, Pair<int>, Pair<int>);
	static void move_character(Character*, Nav_symbol);
	static void bite_player(Pair<int>);
private:
	static Map* currmap;
};

// to sort by area
struct Dimension_cmp {
    bool operator()(Building* a, Building* b) const {
    	Pair<int> adim = a->get_dimensions();
    	Pair<int> bdim = b->get_dimensions();
        return (adim.x*adim.y < bdim.x*bdim.y);
    }
};

#endif
