#include "common_Server.h"
#include "common_Index.h"
#include "common_file.h"
#include <iostream>
#define VALID '1'
#define INVALID '0'
#define PUSH '1'
#define TAG '2'
#define PULL '3'
using std::string;

Server::Server(Socket& sock, Index* index_f) : socket(std::move(sock)) {
	this->index_file = index_f;
	this->is_alive = true;
}


void Server::run() {
	unsigned char function;
	this->socket.receive_(&function, sizeof(unsigned char));

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
	unsigned int len_filename[1];
	this->socket.receive_((unsigned char*)len_filename, sizeof(unsigned int));
		
	unsigned char* filename = new unsigned char(*len_filename);
	this->socket.receive_(filename, *len_filename);
	
	unsigned int len_hash [1];
	this->socket.receive_((unsigned char*)len_hash, sizeof(unsigned int));
	
	unsigned char* hash = new unsigned char(*len_hash);
	this->socket.receive_(hash, *len_hash);
	
	for (unsigned int i = 0; i < *len_hash; i++) {
		std::cout << (int)hash[i] << std::endl;
	}	
	if (this->index_file->contains_file_and_hash(
	string(reinterpret_cast<const char*>(filename)), string(reinterpret_cast<const char*>(hash)))) {
		unsigned char invalid = INVALID;
		this->socket.send_(&invalid, sizeof(unsigned char));
		delete hash;
		return;
	} else {
		unsigned char valid = VALID;
		this->socket.send_(&valid, sizeof(unsigned char));
	}
	unsigned int len_file[1];
	this->socket.receive_((unsigned char*)len_file, sizeof(unsigned int));

	unsigned char* file = new unsigned char(*len_file);
	this->socket.receive_(file, *len_file);

	this->save_new_file(hash, file); 

	delete file;
	delete filename;
	delete hash;
}


void Server::save_new_file(unsigned char* filename, unsigned char* content) {
	File file((char*)filename, std::ios::out);
	file.write((char*)content);
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
