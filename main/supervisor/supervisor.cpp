#include "supervisor.hpp"

// TODO: include config header in each source file that enumerates all namespaces

Supervisor::Supervisor() :
map(NULL),
game_is_running(true),
time_running(0), last_epoch(0)
{
}

Supervisor::~Supervisor(){
	delete map;
}

void Supervisor::create_map(){
	tick();
	map = new Map();
	tock();
}

void Supervisor::print_map(){
	map->print_map();
}

bool Supervisor::is_running(){
	return(game_is_running);
}

void Supervisor::iterate(int frames){
	tick();

	for(int i = 0; i < frames; i++){
		if ((game_is_running = map->check_game())){
			Map_settings::settings_check();
			map->iterate();
		}
		else
			break;
	}

	tock();
}

double Supervisor::last_run_epoch(){ return(last_epoch); }
double Supervisor::total_runtime(){ return(time_running); }

void Supervisor::reset_clock(){ time_running = 0, last_epoch = 0; }
void Supervisor::tick(){	beg = clock(); }
void Supervisor::tock(){
	last_epoch = double(clock() - beg)/CLOCKS_PER_SEC;
	time_running += beg;
}

void Supervisor::copy_ROI(Map_symbol** buf, Pair<int> x, Pair<int> y){
	map->copy_ROI(buf, x, y);
}
