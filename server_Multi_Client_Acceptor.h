#ifndef MULTI_CLIENT_ACCEPTOR_H
#define MULTI_CLIENT_ACCEPTOR_H

#include "common_thread.h"
#include "common_socket.h"
#include "common_Index.h"
#include <vector>

class Multi_Client_Acceptor : public Thread {
	Index* index_file;
	Socket main_socket;
	char* port;
	std::vector<Thread*> threads;
	bool is_alive;
	
	public:
		explicit Multi_Client_Acceptor(Index* index_f, Socket& sock);
		virtual void run() override;
		void stop();
		~Multi_Client_Acceptor();
};

#endif // MULTI_CLIENT_ACCEPTOR_H
