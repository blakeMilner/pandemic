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
	void alloc_footprint(Pair<int> dims);
	void copy_footprint(int**);
	void print();

	bool has_adjacent_zero(int x, int y);
	void turn_left(Pair<int>&, Pair<int>&);
	void turn_right(Pair<int>&, Pair<int>&);

	Pair<int> dimensions;
	bool is_alloced;
	int** footprint;
};

#endif
