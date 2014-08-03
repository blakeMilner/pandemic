#include "supervisor.hpp"

// TODO: include config header in each source file that enumerates all namespaces
namespace UD = User_data;

Supervisor::Supervisor() :
map(NULL),
game_is_running(true),
clk(Clock())
{
}

Supervisor::~Supervisor(){
	delete map;
}

void Supervisor::create_map(){
	if(!Map_settings::settings_check()){
		Map_settings::correct_settings();
	}

	clk.tick();
	map = new Map();
	clk.tock();

	if(UD::print_to_cmd)
		cout << "Total time to generate: " << clk.last_run_epoch() << " sec" << endl;

	// CRITICAL: connect mapserver to our new map so out characters can access it.
	MapServer::Instance()->set_map(map);
}

void Supervisor::print_map(){
	map->print_map();
}

bool Supervisor::is_running(){
	return(game_is_running);
}

void Supervisor::iterate(int frames){
	clk.tick();

	for(int i = 0; i < frames; i++){
		if ((game_is_running = map->check_game())){
			map->iterate();
		}
		else{
			// send "game is over" to command window and reset gui state
			break;
		}
	}

	clk.tock();
}

void Supervisor::copy_ROI(Map_symbol** buf, Pair<int> x, Pair<int> y){
	map->copy_ROI(buf, x, y);
}

// TODO: new settings input as optional - can't change overall map size later so how do we deal with that?
// TODO: make staclk.tic function print_to_console that contains if(UD//) and passes a specific comment code - put in helpers
void Supervisor::reset_game(){
	if(UD::print_to_cmd)
		cout << "Game reset. Total runtime: " << clk.last_run_epoch() << " sec" << endl;

	clk.reset_clock();
	game_is_running = 0;

	delete map;
	create_map();
}
