#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <cstdlib>
#include <ctime>
#include "../mapping/map.hpp"
#include "../settings/map_settings.hpp"

using namespace std;

class Supervisor{
public:
	Supervisor();
	~Supervisor();
	void create_map();

	bool is_running();
	void iterate();
	void iterate(int);
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
