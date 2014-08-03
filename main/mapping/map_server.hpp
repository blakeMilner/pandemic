/*
 * map_server.hpp
 *
 *  Created on: Aug 3, 2014
 *      Author: blake
 */

#ifndef MAP_SERVER_HPP_
#define MAP_SERVER_HPP_


#include <vector>

#include "../objects/character.hpp"
#include "../objects/Human.hpp"
#include "../objects/Zombie.hpp"
#include "map.hpp"


class Infected; // to resolve circular inclusion

// singleton class
class MapServer{
public:
	friend class Map;

	static MapServer* Instance(); // returns singleton pointer

	Map_symbol get_symbol(int, int);
	int get_ID(int, int);


	void empty_field(Map_symbol**, Pair<int>);
	void copy_field(Map_symbol**, Pair<int>, Pair<int>);

	void move_character(Character*, Nav_symbol);
	void bite_player(Pair<int>);
	void convert_infected(Infected*);
private:
	MapServer();
	~MapServer();

	static MapServer* p_Instance; // singleton pointer

	static Map* currmap;
	vector<Pair<int> >** BLOCKS_OCCLUDED; // LUT FOR OCCLUSIONS
};

#endif /* MAP_SERVER_HPP_ */
