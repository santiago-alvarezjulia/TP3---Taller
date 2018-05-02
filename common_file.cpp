#include "common_file.h"
#include <iostream>
#include <string>
using std::string;

File::File(const char* name, std::ios_base::openmode flags) {
	this->file.open(name, flags);
	if (!this->file) {
		// excepcion
	}
	this->filename = name;
}


File::File(File&& other) {
	this->file = std::move(other.file);
	this->filename = other.filename;
	
	other.filename = "";
}

File& File::operator=(File&& other) {
	if (this == &other) {
		return *this;
	}
	
	this->file = std::move(other.file);
	this->filename = other.filename;
	
	other.filename = "";
	return *this;
}

void File::operator<<(string str) {
	this->file << str << std::endl;
}


void File::set_name(string name) {
	this->filename = name;
}


string File::get_name() {
	return this->filename;
}


bool File::fail_open() {
	return this->file.fail() == 1;
}


bool File::eof() {
	return this->file.eof();
}


void File::get_line(string& line) {
	getline(this->file, line);
}


void File::read(char* byte_leido, size_t n) {
	this->file.read(byte_leido, n);
}


void File::write(string buffer) {
	this->file.write(buffer.c_str(), buffer.size());
}


size_t File::tell_g() {
	return this->file.tellg();
} 


void File::seek_g(size_t a, std::ios_base::seekdir dir) {
	this->file.seekg(a, dir);
}


File::~File() {
	this->file.close();
}
