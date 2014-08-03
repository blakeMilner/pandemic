#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

#include <queue>
#include <list>
#include <vector>
#include "../enums/object_enum.hpp"
#include "../containers/containers.hpp"
#include "../helpers/rng.hpp"
#include "../settings/map_settings.hpp"

using namespace std;

class Footprint{
public:
	Footprint();
	Footprint(Pair<int>);
	Footprint(const Footprint&);
	virtual ~Footprint();

	// TODO: perhaps make these private later??
	Map_symbol** footprint;
	Pair<int> dimensions;

	void alloc_footprint(Pair<int> dims);
	void copy_footprint(Map_symbol**);
	void print();

	bool block_is_inside(int x, int y);
private:
	void turn_left(Pair<int>&, Pair<int>&);
	void turn_right(Pair<int>&, Pair<int>&);

	bool is_alloced;
};

#endif
