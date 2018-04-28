#include "server_End_App.h"
#include <iostream>
#define END_APP 'q'
using std::cin;

End_App::End_App() {
	this->is_alive = true;
}

void End_App::run() {
	while (true) {
		char input = cin.get();
		if (input == END_APP) {
			this->is_alive = false;
			break;
		}
	}
}

bool End_App::has_ended() {
	return this->is_alive == false;
}

End_App::~End_App() {}
