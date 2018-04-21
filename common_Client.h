#ifndef CLIENT_H
#define CLIENT_H

#include "common_socket.h"

class Client {
	Socket socket;
	
	public:
		Client(Socket& sock);
		
		void pull();
		void tag();
		void push();
		
		~Client();
};

#endif // CLIENT_H
