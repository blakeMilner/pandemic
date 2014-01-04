#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

#include <queue>
#include "../containers/containers.hpp"

using namespace std;

class Footprint{
public:
	Footprint();
	Footprint(queue<Pair<int> >&, Pair<int>);
	Footprint(const Footprint&);
	virtual ~Footprint();
	void alloc_footprint(Pair<int> dims);
	void copy_footprint(int**);

	Pair<int> dimensions;
	bool is_alloced;
	int** footprint;
};

#endif
