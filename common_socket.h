#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

#include <stddef.h>
#include <stdbool.h>
#include <netdb.h> 


class Socket {
	int socket_fd;
	
	public:
		Socket();
		
		int connect_(const char* hostname, const char* service_name);
		int send_(unsigned char* chunk, int sizeof_chunk);
		int receive_(unsigned char* chunk, int sizeof_chunk);
		int bind_and_listen(const char* port);
		int accept_(Socket& new_socket);
		void shutdown_rw();
		
		~Socket();
};

#endif // COMMON_SOCKET_H
