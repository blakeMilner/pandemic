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

using namespace std;

#define MAX_TRIES 100

class Map{
public:
	friend class MapServer;

	Map();
	~Map();
	void iterate();
	void print_map();
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

	Map_symbol get_symbol(int, int);
	Map_symbol get_symbol(Pair);
	int get_ID(int, int);
	int get_ID(Pair);
	void add_character(Character*);
	void delete_character(Character*, int);
	void add_obstacle(Obstacle*);
	Pair find_region(Pair);
	bool regional_bounds_check(Pair);
	bool regional_bounds_check(int, int);
	bool overall_bounds_check(Pair);
	bool overall_bounds_check(int, int);

	// TERRAIN GENERATION
	void regionalize();
	void populate();
	void make_buildings();
	void make_characters();
	Pair find_empty_path(int, int, int, int);
	bool placement_is_clear(Pair, Pair);
	void remove_corners(Pair&, queue<Pair>&);
	void trace_outline(Pair&, queue<Pair>&);
	void turn_left(queue<Pair>&, Pair&, Pair&);
	void turn_right(queue<Pair>&, Pair&, Pair&);

	// functions meant for MapServer
	void move_character(Character*, Pair&);
	void infect_player(int ID);
};

class MapServer{
public:
	friend class Map;
	static Map_symbol get_symbol(int, int);
	static int get_ID(int, int);
	static void copy_field(Map_symbol**, Pair, Pair);
	static void move_character(Character*, Nav_symbol);
	static void bite_player(Pair);
private:
	static Map* currmap;
};

// to sort by area
struct Dimension_cmp {
    bool operator()(Building* a, Building* b) const {
    	Pair adim = a->get_dimensions();
    	Pair bdim = b->get_dimensions();
        return (adim.x*adim.y < bdim.x*bdim.y);
    }
};


#endif
