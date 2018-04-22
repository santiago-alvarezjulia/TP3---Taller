#include "common_Index.h"
#include "common_socket.h"
#define OK 0
#define CANT_MIN_PARAM 3
#define POS_PORT 1
#define POS_INDEX 2	

int main(int argc, char* argv []) {
	// hilo para esperar q por stdin
	// primero leo el indice
	Index index_file(argv[POS_INDEX]);
	Socket socket_server;
	socket_server.bind_and_listen(argv[POS_PORT]);
	Socket socket_server_asoc;
	socket_server.accept_(socket_server_asoc); 
	socket_server.shutdown_rw();
	index_file.overwrite();
	return OK;
}
