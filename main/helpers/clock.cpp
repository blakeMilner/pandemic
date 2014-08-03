/*
 * clock.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: blake
 */

#include "clock.hpp"

Clock::Clock():
	last_epoch(0),
	time_running(0),
	beg(0),
	paused_interval(0)
{}

Clock::~Clock(){}

double Clock::last_run_epoch(){ return(last_epoch); }

double Clock::total_runtime(){ return(time_running); }

void Clock::tick(){	beg = clock(); }
double Clock::tock(){
	time_running += last_epoch = double(clock() - beg)/CLOCKS_PER_SEC;
	beg = 0;

	return last_epoch;
}

void Clock::reset_clock(){ time_running = 0, last_epoch = 0; }

void Clock::pause(){
	paused_interval = (clock() - beg);
	beg = 0;

}

void Clock::unpause(){
	beg = (clock() - paused_interval);
}



