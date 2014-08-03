/*
 * clock.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: blake
 */

#ifndef CLOCK_HPP_
#define CLOCK_HPP_


#include <ctime>

class Clock {
// IN SECONDS!

public:
	Clock();
	virtual ~Clock();

	double last_run_epoch();
	double total_runtime();

	void reset_clock();
	void abort();

	void tick();
	double tock();

	void pause();
	void unpause();
private:
	clock_t beg;
	clock_t paused_interval;
	double last_epoch;
	double time_running;

};


#endif /* CLOCK_HPP_ */
