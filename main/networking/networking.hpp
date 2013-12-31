#ifndef NETWORKING_HPP
#define NETWORKING_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define MAX_BUFFER_LENGTH 256

using namespace std;

namespace Networking{
	static bool is_connected = false;

	static int sockfd = -1;
	static int clisockfd = -1;
	static int portno = 65535;
	static int num_read = 0;

	static socklen_t clilen = 0;

	static char buffer[MAX_BUFFER_LENGTH] = {0};

	static struct sockaddr_in serv_addr, cli_addr;
	static struct hostent *server;

	static string slave_ip = string("localhost");

	void init_slave() throw();
	void slave_listen() throw();

	void init_master() throw();
	void master_send() throw();
};

#endif
