#ifndef COMMON_FILE_H
#define COMMON_FILE_H

#include <fstream>
#include <string>
using std::string;

class File {
	std::fstream file;
	string filename;
	
	public:
		File(const char* name, std::ios_base::openmode flags);
		File(File&& other);
		File& operator=(File&& file);
		bool fail_open();
		bool eof();
		void set_name(string name);
		string get_name();
		void get_line(string& line); // cambiar por lo que devuelve getline
		void read(char* buffer, size_t n);
		void write(string buffer);
		size_t tell_g();
		void seek_g(size_t a, std::ios_base::seekdir dir);
		~File();
};

#endif //COMMON_FILE_H
