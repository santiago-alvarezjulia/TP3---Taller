#ifndef CLIENT_H
#define CLIENT_H

#include "common_socket.h"

class Client {
	Socket socket;
	void save_new_file(unsigned char* filename, unsigned char* content);
	
	public:
		explicit Client(Socket& sock);
		void push(char* filename, char* hash);
		void tag(int argc, char* argv []);
		void pull(char* tag);
		~Client();
};

#endif // CLIENT_H
