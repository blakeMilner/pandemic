#ifndef RNG_HPP
#define RNH_HPP

#include <time.h>
#include <stdlib.h>
#include "../helpers/containers.hpp"

#define FLOAT_PRECISION 1000

namespace RNG{
	Pair<int> random_pair(int, int, int, int);
	Pair<int> random_pair(Pair<int>, Pair<int>);
	int random_num(int, int);
	bool yes_or_no(float);
};

#endif
