#include "common_Client.h"
#include <stdlib.h>

Client::Client(Socket& sock) : socket(sock) {}

void Client::push(char* filename, char* hash) {
	unsigned char* function = {(unsigned char*)'1'};
	this->socket.send_(function, 1);
	
	/*
	unsigned char* len_filename;
	this->socket.send_(len_filename, 4);
	unsigned char* filename;
	this->socket.send_(filename, atoi((const char*)len_filename));
	unsigned char* hash;
	this->socket.send_(hash);
	*/
	unsigned char is_valid[1];
	this->socket.receive_(is_valid, 1);
	if (atoi((const char*)is_valid) == 1) {
		// envio el archivo
		/*
		unsigned char* len_file;
		this->socket.send_(len_file, 4);
		unsigned char* file;
		this->socket.send_(file, atoi((const char*)len_file));
		*/
	}
}


void Client::tag() {
	unsigned char* function = {(unsigned char*)'2'};
	this->socket.send_(function, 1);
}


void Client::pull(char* tag) {
	unsigned char* function = {(unsigned char*)'3'};
	this->socket.send_(function, 1);
}


Client::~Client() {
	this->socket.shutdown_rw();
}
