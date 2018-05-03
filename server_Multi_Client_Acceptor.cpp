#include "server_Multi_Client_Acceptor.h"
#include "server_Server.h"
#include "common_thread.h"
#include <vector>
#include <iostream>
using std::vector;


Multi_Client_Acceptor::Multi_Client_Acceptor(Index& index_f, Socket& sock) 
: index_file(std::move(index_f)),main_socket(std::move(sock)) {
	this->is_alive = true;
	vector<Thread*> threads;
	this->threads = threads;
}

void Multi_Client_Acceptor::run() {
	while (is_alive) {
		Socket asoc = this->main_socket.accept_();
		if (!is_alive) {
			break;
		}
		this->threads.push_back(new Server(asoc, this->index_file));
		this->threads[this->threads.size() - 1]->start();
		
		for (size_t i = 0; i < this->threads.size(); i++) {
			if (((Server*)this->threads[i])->has_ended()) {
				this->threads[i]->join();
				delete this->threads[i];
			}
		}
	}
}

void Multi_Client_Acceptor::stop() {
	this->main_socket.shutdown_rw();
	this->is_alive = false;
}

Multi_Client_Acceptor::~Multi_Client_Acceptor() {
	for (size_t i = 0; i < this->threads.size(); i++) {
		this->threads[i]->join();
		delete this->threads[i];
	}
}
