#include "common_Client.h"
#include "common_file.h"
#include <iostream>
#include <stdlib.h>
#include <string>
using std::string;

Client::Client(Socket* sock) : socket(sock) {}

void Client::push(char* filename, char* hash) {
	unsigned char function = '1';
	this->socket->send_(&function, sizeof(unsigned char));
	
	unsigned int len_filename = string((const char*)filename).size();
	this->socket->send_((unsigned char*)&len_filename, sizeof(unsigned int));
	this->socket->send_((unsigned char*)&filename, len_filename);

	unsigned int len_hash = string((const char*)hash).size();
	this->socket->send_((unsigned char*)&len_hash, sizeof(unsigned int));
	this->socket->send_((unsigned char*)&hash, len_hash);
	
	// recibo el byte que me indica si es valida la operacion
	unsigned char is_valid;
	this->socket->receive_(&is_valid, sizeof(unsigned char));
	
	if (atoi((const char*)&is_valid) == 1) {
		// envio el archivo.
		File file(filename, std::ios::in);
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
		this->socket->send_((unsigned char*)&len_file, sizeof(unsigned int));
		
		char* file_content = new char(len_file);
		file.read(file_content, len_file);
		this->socket->send_((unsigned char*)file_content, len_file);
		
		delete file_content;
	}
}


void Client::tag(int argc, char* argv []) {
	// envio el comando codificado con el valor 2
	unsigned char function = '2';
	this->socket->send_(&function, 1);
	
}


void Client::pull(char* tag) {
	// envio el comando codificado con el valor 3
	unsigned char function = '3';
	this->socket->send_(&function, 1);
	
}


Client::~Client() {
	this->socket->shutdown_rw();
}
