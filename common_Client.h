#ifndef CLIENT_H
#define CLIENT_H

#include "common_socket.h"

class Client {
	Socket socket;
	
	public:
		Client(Socket& sock);
		
		void push(char* filename, char* hash);
		void tag();
		void pull(char* tag);
		
		~Client();
};

#endif // CLIENT_H
