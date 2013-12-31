#ifndef FACTORY_HPP
#define FACTORY_HPP

using namespace std;

#include "../objects/Human.hpp"
#include "../objects/Building.hpp"
#include "../settings/map_settings.hpp"
#include <list>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

#define INVALID_COOR 'I'
#define VALID_COOR 'V'
#define MAX_TRIES 100

namespace Factory{
	Pair find_empty_path(int, int, int, int);
	bool placement_is_clear(Pair, Pair);

};

// to sort by area
struct Dimension_cmp {
    bool operator()(Building a, Building b) const {
    	Pair adim = a.get_dimensions();
    	Pair bdim = b.get_dimensions();
        return (adim.x*adim.y < bdim.x*bdim.y);
    }
};


#endif
