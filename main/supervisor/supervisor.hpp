#ifndef SUPERVISOR_HPP
#define SUPERVISOR_HPP

#include <cstdlib>
#include "../helpers/clock.hpp"
#include "../settings/user_data.hpp"
#include "../mapping/map.hpp"
#include "../settings/map_settings.hpp"

using namespace std;

class Supervisor{
public:
	Supervisor();
	~Supervisor();

	Clock clk;

	void create_map();
	void reset_game();

	bool is_running();
	void iterate(int = 1);

	void print_map();
	void copy_ROI(Map_symbol**, Pair<int>, Pair<int>);
private:
	Map* map;

	bool game_is_running, frame_is_running;
};

#endif
