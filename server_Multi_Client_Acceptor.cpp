#include "server_Multi_Client_Acceptor.h"
#include "common_Server.h"
#include "common_thread.h"
#include <vector>
#include <iostream>
using std::vector;


Multi_Client_Acceptor::Multi_Client_Acceptor(char* port, Index* index_f) {
	this->index_file = index_f;
	this->port = port;
	vector<Thread*> threads;
	this->threads = threads;
}

void Multi_Client_Acceptor::run() {
	Socket main_socket;
	main_socket.bind_and_listen(this->port);
	
	while (true) {
		Socket asoc = main_socket.accept_();
		this->threads.push_back(new Server(asoc, this->index_file));
		this->threads[this->threads.size() - 1]->start();
		
		for (size_t i = 0; i < this->threads.size(); i++) {
			if (((Server*)this->threads[i])->has_ended()) {
				this->threads[i]->join();
				delete this->threads[i];
			}
		}
	}
	main_socket.shutdown_rw();
}

Multi_Client_Acceptor::~Multi_Client_Acceptor() {
	for (size_t i = 0; i < this->threads.size(); i++) {
		this->threads[i]->join();
		delete this->threads[i];
	}
}
