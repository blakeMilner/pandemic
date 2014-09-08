#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <getopt.h>
#include <string>
#include <exception>
#include <ctime>

#include "settings/user_data.hpp"
#include "networking/networking.hpp"
#include "supervisor/supervisor.hpp"

#ifdef GUI
#include <QtGui/QApplication>
#include "gui/mainwindow.h"
#endif


using namespace std;

namespace UD = User_data;
namespace NET = Networking;

Supervisor* supervisor = NULL;

void run_from_cmd(){
//    try{
//        // wait until remote computer connects to proceed
//        if(UD::listen_mode){
//            NET::init_slave();
//            NET::slave_listen();
//            NET::slave_listen();
//
//            NET::is_connected = true;
//        }
//        // wait until we connect with remote computer
//        else if(UD::connect_to_slave){
//            NET::init_master();
//            NET::master_send();
//            NET::master_send();
//
//            NET::is_connected = true;
//        }
//    }catch(exception& e){
//        cerr << "Network connection failed." << endl;
//        NET::is_connected = false;
//    }


    if(UD::user_iterate and UD::print_to_cmd){
        while(supervisor->is_running()){
            cout << endl << "Press Enter for next iteration." << endl;
            cin.ignore();
            supervisor->print_map();
            supervisor->iterate();
        }
    }
    else{
        supervisor->iterate(UD::num_iter);
    }
}

void run_gui(int argc, char** argv){
    QApplication a(argc, argv);
    MainWindow* w = new MainWindow(supervisor);

	w->start();
	a.exec();

	delete w;
}

void setup(){
    srand(time(NULL));

    supervisor = new Supervisor();
	supervisor->create_map();

    if(UD::print_to_cmd) supervisor->print_map();
		cout << "Time to generate: " << supervisor->clk.last_run_epoch() << " sec" << endl;

	supervisor->clk.reset_clock();
}

int main(int argc, char **argv){
	char ch;

	while ((ch = getopt_long(argc, argv, UD::options_string, UD::options, NULL)) != EOF){
		try{
			switch (ch) {
			 case 'n':
				 UD::gui_wanted = false;
				 break;
			 case 'l':
				 UD::gui_wanted = false;
				 UD::listen_mode = true;
				 break;
			 case 'c':
				 UD::connect_to_slave = true;
				 if(optarg)
					 NET::slave_ip = optarg;
				 break;
			 case 'p':
				 NET::portno = (unsigned short) atoi(optarg);
				 break;
			 case 'm':
				 UD::print_to_cmd = true;
				 break;
			 case 'f':
				 UD::user_iterate = false;
				 if(optarg)
					 UD::num_iter = atoi(optarg);
				 break;
			}
		}
		catch(exception e){
			cerr << "Whoops, one of your inputs caused pandemic to fail." << endl;
		};
	}

    setup();


#ifdef GUI
	if(UD::gui_wanted)
		run_gui(argc, argv);
	else
		run_from_cmd();
#else
	run_from_cmd();
#endif



    delete supervisor;
    MapServer::Instance()->delete_instance();

	return EXIT_SUCCESS;
}
