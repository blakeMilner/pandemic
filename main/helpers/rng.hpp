#ifndef RNG_HPP
#define RNH_HPP

#include <time.h>
#include <stdlib.h>
#include "../containers/containers.hpp"

#define FLOAT_PRECISION 1000

namespace RNG{
	Pair random_pair(int, int, int, int);
	Pair random_pair(Pair, Pair);
	int random_num(int, int);
	bool yes_or_no(float);
};

#endif
