#ifndef COMMON_SERVER_H
#define COMMON_SERVER_H

#include "common_socket.h"
#include "common_thread.h"

class Server : public Thread {
	Socket socket;
	bool is_alive;
	void pull();
	void tag();
	void push();
	
	public:
		Server(Socket& sock);
		virtual void run() override;
		bool has_ended();
		~Server();
};

#endif // COMMON_SERVER_H
