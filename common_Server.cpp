#include "common_Server.h"
#include "common_Index.h"
#include "common_file.h"
#include <iostream>
#include <vector>
#include <string>
#define VALID '1'
#define INVALID '0'
#define PUSH '1'
#define TAG '2'
#define PULL '3'
using std::string;
using std::vector;

/*
 *	FALTA VERIFICAR LOS SEND Y RECEIVE (CUANDO SE QUIERE CERRAR EL PROGRAMA)
 * 	TAMBIEN CAMBIAR EL CODIGO PARA VOLVER ATRAS SI CORTA EN LA MITAD DEL COMANDO
 * 
 */
 
Server::Server(Socket& sock, Index& index_f) : socket(std::move(sock)), index_file(std::move(index_f)) {
	this->is_alive = true;
}


void Server::run() {
	unsigned char function;
	this->socket.receive_(&function, sizeof(unsigned char));
	//if (i < 0) {
	//	std::cout << "AAA" << std::endl;
	//	return;
	//}
	
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
		
	if (this->index_file.contains_file_and_hash(
	string(reinterpret_cast<const char*>(filename)), 
	string(reinterpret_cast<const char*>(hash)))) {
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
	file << string(reinterpret_cast<const char*>(content));
}


void Server::tag() {
	// primero verificar que existan todos los hashes en map->files
	// Si existen todos, seguir, sino salir
	bool is_valid = true;
	
	unsigned int cant_hashes[1];
	this->socket.receive_((unsigned char*)cant_hashes, sizeof(unsigned int));
	
	unsigned int len_tag[1];
	this->socket.receive_((unsigned char*)len_tag, sizeof(unsigned int));
	
	unsigned char* tag = new unsigned char(*len_tag);
	this->socket.receive_(tag, *len_tag);
	
	if (this->index_file.contains_tag(string(reinterpret_cast<const char*>
		(tag)))) {
		is_valid = false;
	}
	
	vector<unsigned char*> hash_vector;
	for (unsigned int i = 0; i < *cant_hashes; i++) {
		unsigned int len_hash[1];
		this->socket.receive_((unsigned char*)len_hash, sizeof(unsigned int));
	
		unsigned char* hash = new unsigned char(*len_hash);
		this->socket.receive_(tag, *len_hash);
		
		hash_vector.push_back(hash);
		
		if (is_valid) {
			if (!this->index_file.contains_hash_stored(
			string(reinterpret_cast<const char*>(hash)))) {
				is_valid = false;
			}
		}
	}
	
	if (is_valid) {
		for (unsigned int i = 0; i < hash_vector.size(); i++) {
			this->index_file.add_tag(string(reinterpret_cast<const char*>
			(tag)), string(reinterpret_cast<const char*>(hash_vector[i])));
		}
		unsigned char valid = '1';
		this->socket.send_(&valid, 1);
	} else {
		unsigned char invalid = '0';
		this->socket.send_(&invalid, 1);
	}
	
	for (unsigned int i = 0; i < hash_vector.size(); i++) {	
		delete hash_vector[i]; // mirar si se elimina o no del index
	}
	delete tag;
}


void Server::pull() {
	unsigned int len_tag [1];
	this->socket.receive_((unsigned char*)len_tag, sizeof(unsigned int));
	
	unsigned char* tag = new unsigned char(*len_tag);
	this->socket.receive_(tag, *len_tag);
	
	if (this->index_file.contains_tag(
	string(reinterpret_cast<const char*>(tag)))) {
		unsigned char invalid = INVALID;
		this->socket.send_(&invalid, sizeof(unsigned char));
		delete tag;
		return;
	} else {
		unsigned char valid = VALID;
		this->socket.send_(&valid, sizeof(unsigned char));
	}
	
	std::vector<string> name_hashes = this->index_file.get_hashes_by_tag(
	(char*)tag);
	// envio la cantidad de archivos taggeados
	unsigned int len_name_hashes = name_hashes.size();
	this->socket.send_((unsigned char*)&len_name_hashes, sizeof(unsigned int));
	
	for (unsigned int i = 0; i < name_hashes.size(); i++) {
		// envio el nombre del archivo
		unsigned int len_name = name_hashes[i].size();
		this->socket.send_((unsigned char*)&len_name, sizeof(unsigned int));
	
		unsigned char* name_hash = new unsigned char(len_name);
		this->socket.receive_(name_hash, len_name);
		
		File file(name_hashes[i].c_str(), std::ios::in);
		if (file.fail_open()) {
			std::cerr << "Error: archivo inexistente." << std::endl;
			return;
		}
		
		std::ios_base::seekdir end = std::ios::end;
		std::ios_base::seekdir begin = std::ios::beg;
		std::ios_base::seekdir current = std::ios::cur;
		size_t pos_actual = file.tell_g(); 
		file.seek_g(0, end);
		size_t pos_final_archivo = file.tell_g();
		file.seek_g(0, begin);
		file.seek_g(pos_actual, current);
		
		unsigned int len_file = pos_final_archivo;
		this->socket.send_((unsigned char*)&len_file, sizeof(unsigned int));
		
		char* file_content = new char(len_file);
		file.read(file_content, len_file);
		this->socket.send_((unsigned char*)file_content, len_file);
		
		delete name_hash;
		delete file_content;
	}
}


bool Server::has_ended() {
	return this->is_alive == false;
}


Server::~Server() {
	this->socket.shutdown_rw();
}
