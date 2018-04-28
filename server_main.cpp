#include "common_Index.h"
#include "common_thread.h"
#include "server_Multi_Client_Acceptor.h"
#include "server_End_App.h"
#define OK 0
#define CANT_MIN_PARAM 3
#define POS_PORT 1
#define POS_INDEX 2	

int main(int argc, char* argv []) {
	// primero leo el indice
	Index index_file(argv[POS_INDEX]);
	
	Thread* thread_acceptor = new Multi_Client_Acceptor(argv[POS_PORT], 
	&index_file);
	thread_acceptor->start();
	
	Thread* thread_end_app = new End_App();
	thread_end_app->start();
	
	// falta ver que cuando termine el thread end_app haga join del thread_acceptor
	if (((End_App*)thread_end_app)->has_ended()) {
		thread_acceptor->join();
		delete thread_acceptor;
	}
	thread_end_app->join();
	delete thread_end_app;
	
	index_file.overwrite();
	return OK;
}
