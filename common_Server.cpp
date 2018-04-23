#include "common_Server.h"

Server::Server(Socket& sock) : socket(sock) {
	this->is_alive = true;
}


void Server::run() {
	while (this->is_alive) {
		unsigned char chunk_function [1];
		this->socket.receive_(chunk_function, 1);
		int function = atoi((const char*)chunk_function);
		if (function == 1) {
			this->push();
		} else if (function == 2) {
			this->tag();
		} else if (function == 3) {
			this->pull();
		}
	}
}

void Server::push() {

}


void Server::tag() {

}


void Server::pull() {

}


bool Server::has_ended() {
	return this->is_alive == false;
}


Server::~Server() {
	this->socket.shutdown_rw();
}
