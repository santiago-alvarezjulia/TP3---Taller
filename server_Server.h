#ifndef SERVER_H
#define SERVER_H

#include "common_socket.h"
#include "common_thread.h"
#include "server_Index.h"

class Server : public Thread {
	Socket socket;
	Index index_file;
	bool is_alive;
	void pull();
	void tag();
	void push();
	void save_new_file(unsigned char* filename, unsigned char* content);
	
	public:
		explicit Server(Socket& sock, Index& index_f);
		virtual void run() override;
		bool has_ended();
		~Server();
};

#endif // SERVER_H
