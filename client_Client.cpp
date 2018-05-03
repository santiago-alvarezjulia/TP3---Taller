#include "client_Client.h"
#include "common_file.h"
#include <iostream>
#include <stdlib.h>
#include <string>
using std::string;

/*
 *	FALTA VERIFICAR LOS SEND Y RECEIVE (CUANDO SE QUIERE CERRAR EL PROGRAMA)
 * 	TAMBIEN CAMBIAR EL CODIGO PARA VOLVER ATRAS SI CORTA EN LA MITAD DEL COMANDO
 * 
 */

Client::Client(Socket& sock) : socket(std::move(sock)) {}

void Client::push(char* filename, char* hash) {
	// primero abro y verifico que existe el archivo
	File file(filename, std::ios::in | std::ios::binary);
	if (file.fail_open()) {
		std::cerr << "Error: archivo inexistente." << std::endl;
		return;
	}
		
	unsigned char function = '1';
	this->socket.send_(&function, sizeof(unsigned char));
	
	unsigned int len_filename = string((const char*)filename).size();
	this->socket.send_((unsigned char*)&len_filename, sizeof(unsigned int));
	this->socket.send_((unsigned char*)&filename, len_filename);

	unsigned int len_hash = string((const char*)hash).size();
	this->socket.send_((unsigned char*)&len_hash, sizeof(unsigned int));
	this->socket.send_((unsigned char*)&hash, len_hash);

	// recibo el byte que me indica si es valida la operacion
	unsigned char is_valid;
	this->socket.receive_(&is_valid, sizeof(unsigned char));
	
	if (atoi((const char*)&is_valid) == 1) {
		// envio el archivo.
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
		
		delete file_content;
	}
}


void Client::tag(int argc, char* argv []) {
	// envio el comando codificado con el valor 2
	unsigned char function = '2';
	this->socket.send_(&function, 1);
	
	unsigned int cant_hashes = argc - 5;
	this->socket.send_((unsigned char*)&cant_hashes, sizeof(unsigned int));
	
	unsigned int len_tag = string((const char*)argv[4]).size();
	this->socket.send_((unsigned char*)&len_tag, sizeof(unsigned int));
	this->socket.send_((unsigned char*)&argv[4], len_tag);

	for (int i = 5; i < argc; i++) {
		unsigned int len_hash = string((const char*)argv[i]).size();
		this->socket.send_((unsigned char*)&len_hash, sizeof(unsigned int));
		this->socket.send_((unsigned char*)&argv[i], len_hash);
	}
	unsigned char is_valid;
	this->socket.receive_(&is_valid, sizeof(unsigned char));

	if (atoi((const char*)&is_valid) == 0) {
		std::cerr << "Error: tag/hash incorrecto." << std::endl;
	}
}


void Client::pull(char* tag) {
	// envio el comando codificado con el valor 3
	unsigned char function = '3';
	this->socket.send_(&function, 1);
	
	unsigned int len_tag = string((const char*)tag).size();
	this->socket.send_((unsigned char*)&len_tag, sizeof(unsigned int));
	this->socket.send_((unsigned char*)&tag, len_tag);
	
	// recibo el byte que me indica si es valida la operacion
	unsigned char is_valid;
	this->socket.receive_(&is_valid, sizeof(unsigned char));
	
	if (atoi((const char*)&is_valid) == 1) {
		// recibo la cantidad de archivos taggeados
		unsigned int cant_hashes [1];
		this->socket.receive_((unsigned char*)cant_hashes, sizeof(unsigned int));
		
		// recibo los archivos taggeados.
		for (unsigned int i = 0; i < *cant_hashes; i++) {
			// recibo el nombre de hash
			unsigned int len_name[1];
			this->socket.receive_((unsigned char*)len_name, sizeof(unsigned int));
			
			unsigned char* filename = new unsigned char(*len_name);
			this->socket.receive_(filename, *len_name);
			
			unsigned int len_file[1];
			this->socket.receive_((unsigned char*)len_file, sizeof(unsigned int));
			
			unsigned char* file = new unsigned char(*len_file);
			this->socket.receive_(file, *len_file);
			
			this->save_new_file(filename, file);
			
			delete filename;
			delete file;
		}
	} else {
		std::cerr << "Error: tag/hash incorrecto." << std::endl;
	}
}


void Client::save_new_file(unsigned char* filename, unsigned char* content) {
	File file((char*)filename, std::ios::out);
	file << string(reinterpret_cast<const char*>(content));
}


Client::~Client() {
	this->socket.shutdown_rw();
}
