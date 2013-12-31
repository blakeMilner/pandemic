#ifndef ATTRIBUTES_HPP
#define ATTRIBUTES_HPP

#include <queue>
#include "../containers/containers.hpp"

using namespace std;

class Footprint{
public:
	Footprint();
	Footprint(queue<Pair>&, Pair);
	Footprint(const Footprint&);
	virtual ~Footprint();
	void alloc_footprint(Pair dims);
	void copy_footprint(int**);

	Pair dimensions;
	bool is_alloced;
	int** footprint;
};

#endif
