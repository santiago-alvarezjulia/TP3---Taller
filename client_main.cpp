#include "common_socket.h"
#include "common_Client.h"
#include <string>
#include <iostream>
#define OK 0
#define ERROR 1
#define POS_HOSTNAME 1
#define POS_PORT 2
#define POS_FUNCTION 3

int main(int argc, char* argv []) {
	std::string functions = std::string("pull") + std::string("tag") + std::string("push");
	std::size_t found = functions.find(argv[POS_FUNCTION]);
	if (found == std::string::npos) {
		std::cout << "Error: argumentos invalidos." << std::endl;
		return ERROR;
	}
	Socket socket_client;
	socket_client.connect_(argv[POS_HOSTNAME], argv[POS_PORT]);
	
	Client client(socket_client);
	if (found == 0) {
		client.pull();
	} else if (found == 4) {
		client.tag();
	} else {
		client.push();
	}
	
	return OK;
}
