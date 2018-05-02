#include "common_Index.h"
#include "common_thread.h"
#include "server_Multi_Client_Acceptor.h"
#include "server_End_App.h"
#include <iostream>
#define OK 0
#define CANT_MIN_PARAM 3
#define POS_PORT 1
#define POS_INDEX 2	

int main(int argc, char* argv []) {
	// primero leo el indice
	Index index_file(argv[POS_INDEX]);
	
	Socket main_socket;
	main_socket.bind_and_listen(argv[POS_PORT]);
	
	Multi_Client_Acceptor* thread_acceptor = new Multi_Client_Acceptor
	(index_file, main_socket);
	thread_acceptor->start();
	
	End_App* thread_end_app = new End_App();
	thread_end_app->start();
	
	while (true) {
		if (thread_end_app->has_ended()) {
			thread_end_app->join();
			delete thread_end_app;
			break;
		}
	}
	thread_acceptor->stop();
	thread_acceptor->join();
	delete thread_acceptor;
	
	index_file.overwrite();

	return OK;
}
