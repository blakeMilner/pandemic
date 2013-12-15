#include "networking.hpp"

int Networking::sockfd = -1;
int Networking::clisockfd = -1;
int Networking::portno = DEFAULT_PORTNO;
int Networking::num_read = 0;
bool Networking::is_connected = false;
socklen_t Networking::clilen = 0;
sockaddr_in Networking::cli_addr;
sockaddr_in Networking::serv_addr;
hostent* Networking::server;
char Networking::buffer[] = {0};
string Networking::slave_ip = string(DEFAULT_SLAVE_IP);


void Networking::init_slave() throw(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		cerr << "Error opening server socket. " << strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Server socket opened." << endl; }

	memset((char *) &(serv_addr), 0, sizeof(serv_addr));
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
		cerr << "Error binding socket: " << strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Socket bound." << endl; }

	listen(sockfd, 1);

	clilen = sizeof(cli_addr);
	clisockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
	if(clisockfd < 0){
		cerr << "Error accepting client connection " << strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Client connection accepted." << endl; }
}

void Networking::init_master() throw(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		cerr << "Error opening client socket. " << strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Client socket opened." << endl; }

	server = gethostbyname((char*) slave_ip.c_str());
	if(server == NULL){
		cerr << "Could not find a server with address: " << strerror(errno) << slave_ip << endl;
		throw exception();
	}
	else{ cerr << "Host address verified." << endl; }

	memset((char *) &(serv_addr), 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	memcpy((char *) server->h_addr, (char*) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if(connect(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0){
		cerr << "Error connecting to host: "  << slave_ip << "\t: " <<  strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Now connected to host: " << slave_ip << endl; }
}

void Networking::slave_listen() throw(){
	num_read = read(clisockfd, buffer, MAX_BUFFER_LENGTH - 1);
	if(num_read < 0){
		cerr << "Error reading client transmission " << strerror(errno) << endl;
		throw exception();
	}
	else{
		cerr << "Client transmission received." << endl;
		cerr << "Here is the message: " << buffer << endl;
	}

	num_read = write(clisockfd, "I got your message\0", strlen("I got your message\0"));
	if(num_read < 0){
		cerr << "Failed to send confirmation to client. " << strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Confirmation message successfully sent." << endl; }
}

void Networking::master_send() throw(){
	num_read = write(sockfd, "Here's the message\0", strlen("Here's the message\0"));
	if(num_read < 0){
		cerr << "Error sending transmission. " << strerror(errno) << endl;
		throw exception();
	}
	else{ cerr << "Transmission sent successfully." << endl; }

	num_read = read(sockfd, buffer, MAX_BUFFER_LENGTH-1);
	if(num_read < 0){
		cerr << "Error receving response from host. " << strerror(errno) << endl;
		throw exception();
	}
	else{
		cerr << "Response received from host: " << buffer << endl;
	}
}
