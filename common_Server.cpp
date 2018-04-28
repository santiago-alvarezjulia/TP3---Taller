#include "common_Server.h"
#include "common_Index.h"
#include "common_file.h"
#include <iostream>
#define VALID '1'
#define INVALID '0'
#define PUSH '1'
#define TAG '2'
#define PULL '3'

Server::Server(Socket& sock, Index* index_f) : socket(sock) {
	this->index_file = index_f;
	this->is_alive = true;
}


void Server::run() {
	unsigned char function;
	int a = this->socket.receive_(&function, sizeof(unsigned char));
	std::cout << "a: " << a << std::endl;
	if (function == PUSH) {
		this->push();
	} else if (function == TAG) {
		this->tag();
	} else if (function == PULL) {
		this->pull();
	}
	this->is_alive = false;
}


void Server::push() {
	// recibo el hash y su longitud
	int len_hash = 4;
	char* hash = new char(len_hash);
	this->socket.receive_((unsigned char*)hash, len_hash);
	std::cout << "CACA" << std::endl;/*
	if (this->index_file->contains_file_and_hash(string(filename), string(hash))) {
		unsigned char invalid = INVALID;
		this->socket.send_(&invalid, sizeof(unsigned char));
		delete hash;
		return;
	} else {
		unsigned char valid = VALID;
		this->socket.send_(&valid, sizeof(unsigned char));
	}
	*/
	int len_file;
	this->socket.receive_((unsigned char*)&len_file, sizeof(int));
	
	char* file = new char(len_file);
	this->socket.receive_((unsigned char*)file, len_file);
	
	this->save_new_file(hash, file);
	delete file;
	std::cout << "CACA" << std::endl;
}


void Server::save_new_file(char* filename, char* content) {
	File file(filename, std::ios::out);
	file.write(content);
}


void Server::tag() {
	// primero verificar que existan todos los hashes en map->files
	// Si existen todos, seguir, sino salir
}


void Server::pull() {}


bool Server::has_ended() {
	return this->is_alive == false;
}


Server::~Server() {
	this->socket.shutdown_rw();
}
