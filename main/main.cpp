#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>
#include <string>
#include <exception>

#include "user_data.hpp"
#include "networking.hpp"

using namespace std;

#ifdef GUI
#include <QtGui/QApplication>
#include "gui/mainwindow.h"
#endif

static User_data user_data;
static Networking networking;

int main(int argc, char **argv){
	char ch;

	while ((ch = getopt_long(argc, argv, options_string, options, NULL)) != EOF){
		try{
			switch (ch) {
			 case 'n':
				 user_data.gui_wanted = false;
				 break;
			 case 'l':
				 user_data.gui_wanted = false;
				 user_data.listen_mode = true;
				 break;
			 case 'c':
				 user_data.connect_to_slave = true;
				 if(optarg)
					 networking.slave_ip = optarg;
				 break;
			 case 'p':
				 networking.portno = (unsigned short) atoi(optarg);
				 break;
			}
		}
		catch(exception e){
			cerr << "Whoops, something you input caused pandemic to fail" << endl;
		};
	}


	#ifdef GUI
	if(user_data.gui_wanted){
		QApplication a(argc, argv);
		MainWindow w;
		w.show();
		a.exec();
	}
	else{
		start_engine();
	}
	#else
	// this function call is within MainWindow otherwise
	start_engine();
	#endif

	return EXIT_SUCCESS;

}

// CONTROL GOES BACK TO MAINWINDOW AFTER THIS FUNCTION ENDS
void start_engine(){
	try{
		// wait until remote computer connects to proceed
		if(user_data.listen_mode){
			networking.init_slave();
			networking.slave_listen();
			networking.slave_listen();

			networking.is_connected = true;
		}
		// wait until we connect with remote computer
		else if(user_data.connect_to_slave){
			networking.init_master();
			networking.master_send();
			networking.master_send();

			networking.is_connected = true;
		}
	}catch(exception e){
		cerr << "Network connection failed." << endl;
		networking.is_connected = false;
	}

	if(networking.is_connected){
		cout << "DOING STUFF" << endl;
	}

}
