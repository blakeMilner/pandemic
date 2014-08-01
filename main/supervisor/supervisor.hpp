#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <cstdlib>
#include <ctime>
#include "../settings/user_data.hpp"
#include "../mapping/map.hpp"
#include "../settings/map_settings.hpp"

using namespace std;

class Supervisor{
public:
	Supervisor();
	~Supervisor();
	void create_map();
	void reset_game();

	bool is_running();
	void iterate(int = 1);
	double last_run_epoch();
	double total_runtime();
	void reset_clock();
	void tick();
	void tock();

	void print_map();
	void copy_ROI(Map_symbol**, Pair<int>, Pair<int>);
private:
	clock_t beg;
	double last_epoch;
	double time_running;
	Map* map;
	bool game_is_running;
};

#endif
