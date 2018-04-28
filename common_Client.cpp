#include "common_Client.h"
#include "common_file.h"
#include <iostream>
#include <stdlib.h>
#include <string>
using std::string;

Client::Client(Socket sock) : socket(sock) {}

void Client::push(char* filename, char* hash) {
	//unsigned char function = '1';
	unsigned int len_filename = string((const char*)filename).size();
	unsigned int len_hash = string((const char*)hash).size();
	
	unsigned char buffer_send;
	int len_buffer_send = 1 + len_filename + len_hash + 8; // 8 = 2 ints
	int b = this->socket.send_(&buffer_send, len_buffer_send);
	std::cout << "b:" << b << std::endl;
	// recibo el byte que me indica si es valida la operacion
	unsigned char is_valid;
	this->socket.receive_(&is_valid, sizeof(unsigned char));
	if (atoi((const char*)&is_valid) == 1) {
		// envio el archivo.
		File file(filename, std::ios::in);
		if (file.fail_open()) {
			std::cerr << "Error: archivo inexistente." << std::endl;
			return; // esta medio raro
		}
		
		std::ios_base::seekdir end = std::ios::end;
		std::ios_base::seekdir begin = std::ios::beg;
		file.seek_g(0, end);
		size_t pos_final_archivo = file.tell_g();
		file.seek_g(0, begin);
		
		// aca no se si tengo que sacar un \0 o otra cosa, porlas lo hago
		unsigned int len_file = pos_final_archivo - 1;
		this->socket.send_((unsigned char*)&len_file, sizeof(int));
		
		char* file_content = new char(len_file);
		file.read(file_content, len_file);
		this->socket.send_((unsigned char*)file_content, len_file);
		delete file_content;
	}
}


void Client::tag(int argc, char* argv []) {
	// envio el comando codificado con el valor 2
	unsigned char function = '2';
	this->socket.send_(&function, 1);
}


void Client::pull(char* tag) {
	// envio el comando codificado con el valor 3
	unsigned char function = '3';
	this->socket.send_(&function, 1);
}


Client::~Client() {
	this->socket.shutdown_rw();
}
