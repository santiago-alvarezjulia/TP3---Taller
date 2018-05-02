#define _POSIX_C_SOURCE 200112L
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include "common_socket.h"
#define OK 0
#define ERROR -1
#define MAX_CLIENTS_WAITING 10


Socket::Socket() {
	int sock = socket(AF_INET, SOCK_STREAM, 0);  
	if (sock < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		// excepcion
	}
	this->socket_fd = sock;
}


Socket::Socket(int fd) {
	this->socket_fd = fd;
}

Socket::Socket(Socket&& other) {
	this->socket_fd = other.socket_fd;
	other.socket_fd = -1;
}


int Socket::connect_(const char* hostname, const char* port) {
	struct sockaddr_in client;  

	client.sin_family = AF_INET;
	client.sin_port = htons((uint16_t)atoi(port)); 
	client.sin_addr.s_addr = inet_addr(hostname);  
	
	if (connect(this->socket_fd, (struct sockaddr *)&client, sizeof(client)) < 0) {
		fprintf(stderr, "Error: %s\n", strerror(errno));
		return ERROR;
	}
    return OK;
}


int Socket::send_(unsigned char* chunk, int sizeof_chunk) {
	int bytes_enviados = 0;
	int s;
	bool is_valid_socket = true;
	bool is_open_socket = true;
	
	while(bytes_enviados < sizeof_chunk && is_valid_socket && is_open_socket) {
		s = send(this->socket_fd, &chunk[bytes_enviados], 
		sizeof_chunk - bytes_enviados, MSG_NOSIGNAL);
		if (s < 0) {
			is_valid_socket = false;
		} else if (s == 0) {
			is_open_socket = false;
		} else {
			bytes_enviados += s;
		}
	}	
	if (is_valid_socket) {
		return OK;
	}
	return ERROR;
}

	
int Socket::bind_and_listen(const char* port) {
	int val = 1;
	setsockopt(this->socket_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
	
	struct addrinfo hints;
	struct addrinfo *result;
	int s = 0;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	s = getaddrinfo(NULL, port, &hints, &result);
	
	s = bind(this->socket_fd, result->ai_addr, result->ai_addrlen);
	if (s == -1) {
		fprintf(stderr,"Error : %s\n", strerror(errno));
		return ERROR;
	}
	freeaddrinfo(result);
	
	s = listen(this->socket_fd, MAX_CLIENTS_WAITING);
	if (s == -1) {
		fprintf(stderr,"Error : %s\n", strerror(errno));
		return ERROR;
	}
	return OK;
}

Socket Socket::accept_(){
	int s = accept(this->socket_fd, NULL, NULL);
	if (s < 0) {
		// excepcion
	}
	return std::move(Socket(s));
}


int Socket::receive_(unsigned char* chunk, int sizeof_chunk) {
	int bytes_recibidos = 0;
	int s;
	bool is_open_socket = true;
	bool is_valid_socket = true;
	
	while(bytes_recibidos < sizeof_chunk && is_valid_socket && is_open_socket) {
		s = recv(this->socket_fd, &chunk[bytes_recibidos], 
		sizeof_chunk - bytes_recibidos, 0);
		if (s < 0) {
			is_valid_socket = false;
		} else if (s == 0) {
			is_open_socket = false;
		} else {	
			bytes_recibidos += s;
		}
	}	
		
	if (!is_open_socket && is_valid_socket) {
		return bytes_recibidos;
	}
	return ERROR;
}


void Socket::shutdown_rw() {
	shutdown(this->socket_fd, SHUT_RDWR);
}


Socket::~Socket() {
	if (this->socket_fd != -1) {
		close(this->socket_fd);
	}
}
