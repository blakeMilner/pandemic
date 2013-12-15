#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>
#include <string>

using namespace std;

class User_data{
public:
	static bool gui_wanted;
	static bool listen_mode;
	static bool connect_to_slave;

	User_data(){}
};

/* command line options for getopt() */
const static struct option options[] =
{
		{ "nogui",		no_argument,		NULL,					'n' },
		{ "listen",		no_argument,		NULL,					'l' },
		{ "connect",	optional_argument,	NULL,					'c' },
		{ "port", 		required_argument,	NULL,					'p' },
		{ 0,			0,					0,						0	}
};

const static char* options_string = "nlcp:";

#endif
