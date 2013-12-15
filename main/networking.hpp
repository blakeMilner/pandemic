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
#define DEFAULT_PORTNO 65535
#define DEFAULT_SLAVE_IP "localhost"

using namespace std;

class Networking{
public:
	static bool is_connected;
	static int sockfd, clisockfd, portno, num_read;
	static socklen_t clilen;
	static char buffer[MAX_BUFFER_LENGTH];
	static struct sockaddr_in serv_addr, cli_addr;
	static struct hostent *server;
	static string slave_ip;

	static void init_slave() throw();
	static void slave_listen() throw();

	static void init_master() throw();
	static void master_send() throw();
};

#endif
