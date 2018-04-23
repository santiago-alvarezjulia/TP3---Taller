#include "common_Index.h"
#include "common_socket.h"
#include "common_Server.h"
#include "common_thread.h"
#define OK 0
#define CANT_MIN_PARAM 3
#define POS_PORT 1
#define POS_INDEX 2	

int main(int argc, char* argv []) {
	// hilo para esperar q por stdin
	// primero leo el indice
	Index index_file(argv[POS_INDEX]);
	
	// ver handout server con multicliente
	Socket socket_server;
	socket_server.bind_and_listen(argv[POS_PORT]);
	vector<Thread*> threads;
	
	while (true) { // deberia terminar con q
		Socket socket_server_asoc;
		socket_server.accept_(socket_server_asoc); 
	
		threads.push_back(new Server(socket_server_asoc));
		threads[threads.size()]->start();
		
		for (size_t i = 0; i < threads.size(); i++) {
			if (((Server*)threads[i])->has_ended()) {
				threads[i]->join();
				delete threads[i];
			}
		}
	}
	
	for (size_t i = 0; i < threads.size(); i++) {
		threads[i]->join();
		delete threads[i];
	}
	socket_server.shutdown_rw();
	//socket_server_asoc.shutdown_rw(); a ver donde lo hago
	index_file.overwrite();
	return OK;
}
