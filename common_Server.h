#ifndef COMMON_SERVER_H
#define COMMON_SERVER_H

#include "common_socket.h"
#include "common_thread.h"
#include "common_Index.h"

class Server : public Thread {
	Socket& socket;
	Index* index_file;
	bool is_alive;
	void pull();
	void tag();
	void push();
	void save_new_file(char* filename, char* content);
	
	public:
		explicit Server(Socket& sock, Index* index_f);
		virtual void run() override;
		bool has_ended();
		~Server();
};

#endif // COMMON_SERVER_H
