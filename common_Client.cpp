#include "common_Client.h"
#include "common_file.h"

Client::Client(Socket& sock) : socket(sock) {}

void Client::pull() {

}

void Client::tag() {

}

void Client::push() {

}

Client::~Client() {
	this->socket.shutdown_rw();
}
