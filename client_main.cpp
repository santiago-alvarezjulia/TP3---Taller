#include "common_socket.h"
#include "client_Client.h"
#include <string>
#include <iostream>
#define OK 0
#define ERROR 1
#define POS_HOSTNAME 1
#define POS_PORT 2
#define POS_FUNCTION 3
using std::string;

int main(int argc, char* argv []) {
	string functions = string("pull") + string("tag") + string("push");
	size_t found = functions.find(argv[POS_FUNCTION]);
	if (found == string::npos) {
		std::cout << "Error: argumentos invalidos." << std::endl;
		return ERROR;
	}
	Socket socket_client;
	socket_client.connect_(argv[POS_HOSTNAME], argv[POS_PORT]);

	Client client(socket_client);
	if (found == 0) {
		if (argc == 5) {
			client.pull(argv[4]);
		} else {
			std::cout << "Error: argumentos invalidos." << std::endl;
			return ERROR;
		}
	} else if (found == 4) {
		if (argc >= 6) {
			client.tag(argc, argv);
		} else {
			std::cout << "Error: argumentos invalidos." << std::endl;
			return ERROR;
		}
	} else {
		if (argc == 6) {
			client.push(argv[4], argv[5]);
		} else {
			std::cout << "Error: argumentos invalidos." << std::endl;
			return ERROR;
		}
	}
	return OK;
}
