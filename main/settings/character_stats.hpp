#ifndef STATS_HPP
#define STATS_HPP

class Stats{
public:
	int VIS_RAD;
	int TIME_TO_CONVERSION;
};

namespace Char_stats{
	static int MAX_VIS_RAD = 20;


	static Stats default_character(){
		Stats stats;

		stats.VIS_RAD = 6;

		return stats;
	}
	static Stats default_zombie(){
		Stats stats;

		stats.VIS_RAD = 15;

		return stats;
	}
	static Stats default_human(){
		Stats stats;

		stats.VIS_RAD = 15;

		return stats;
	}
	static Stats default_infected(){
		Stats stats;

		stats.VIS_RAD = 15;
		stats.TIME_TO_CONVERSION = 6;

		return stats;
	}

	static Stats DEFAULT_CHARACTER = default_character();
	static Stats DEFAULT_ZOMBIE = default_zombie();
	static Stats DEFAULT_HUMAN = default_human();
	static Stats DEFAULT_INFECTED = default_infected();
};

#endif
