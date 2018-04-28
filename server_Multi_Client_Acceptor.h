#ifndef MULTI_CLIENT_ACCEPTOR_H
#define MULTI_CLIENT_ACCEPTOR_H

#include "common_thread.h"
#include "common_socket.h"
#include "common_Index.h"
#include <vector>

class Multi_Client_Acceptor : public Thread {
	Index* index_file;
	char* port;
	std::vector<Thread*> threads;
	
	public:
		explicit Multi_Client_Acceptor(char* port, Index* index_f);
		virtual void run() override;
		~Multi_Client_Acceptor();
};

#endif // MULTI_CLIENT_ACCEPTOR_H
