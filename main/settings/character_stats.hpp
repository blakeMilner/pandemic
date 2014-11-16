#ifndef STATS_HPP
#define STATS_HPP

#include "../enums/object_enum.hpp"
#include "../helpers/rng.hpp"

class Stats{
public:
	int VIS_RAD;
	int TIME_TO_CONVERSION;
	int SPEED;
	int SPEED_EQUIVALENT_ITER;

	int num_iter_since_last_move;
};

namespace Char_stats{
	static const int MAX_VIS_RAD = 20;

	static Stats DEFAULT_CHARACTER(Map_symbol char_symbol){
		Stats stats;

		stats.VIS_RAD = 15;

		switch(char_symbol){
		case HUMAN:

			return stats;
		case ZOMBIE:

			return stats;
		case INFECTED:
			stats.TIME_TO_CONVERSION = 60;

			return stats;
		case CHARACTER:

			return stats;
		}

	}


	static Stats RANDOM_CHARACTER(Map_symbol char_symbol){
		Stats stats;

		stats.SPEED = RNG::random_num(1, 3); // higher number is better
		stats.SPEED_EQUIVALENT_ITER = 3 - stats.SPEED; // translate this into move/iter
		stats.VIS_RAD = 15;

		switch(char_symbol){
		case HUMAN:

			return stats;
		case ZOMBIE:

			return stats;
		case INFECTED:
			stats.TIME_TO_CONVERSION = RNG::random_num(30, 120);

			return stats;
		case CHARACTER:

			return stats;
		}

	}
};

#endif
