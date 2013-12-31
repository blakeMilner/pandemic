#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>
#include <string>

namespace User_data{
	static bool gui_wanted = true;
	static bool listen_mode = false;
	static bool connect_to_slave = false;
	static bool print_to_cmd = false;
	static bool user_iterate = true;
	static int num_iter = 100;

/* command line options for getopt() */
const static struct option options[] =
{
		{ "nogui",		no_argument,		NULL,					'n' },
		{ "listen",		no_argument,		NULL,					'l' },
		{ "connect",	optional_argument,	NULL,					'c' },
		{ "port", 		required_argument,	NULL,					'p' },
		{ "printmap",	no_argument,		NULL,					'm' },
		{ "frames",		required_argument,	NULL,					'f' },
		{ 0,			0,					0,						0	}
};

const static char* options_string = "nlcp:mf:";

};

#endif
