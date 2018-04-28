#include "common_Index.h"
#include "common_file.h"
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <string>
using std::vector;
using std::map;
using std::pair;
using std::string;
using std::ios;


Index::Index(string index_name) : index_namefile(index_name) {
	map<string, vector<string>> map_f;
	map<string, vector<string>> map_t;
	this->hashes_by_file = map_f;
	this->hashes_by_tag = map_t;
	this->initialize_index(index_name);
}


void Index::initialize_index(string index_name) {
	std::ios_base::openmode flags = ios::in;
	File index(index_name.c_str(), flags);
	while (!index.eof()) {
		string line;
		index.get_line(line);
		// para el caso el que el eof se encuentra en una linea "vacia"
		if (line.empty()) {
			break;
		}
		
		// type indica el tipo de entrada en el índice. true=tag false=file
		bool type = true; 
		if (line[0] == 'f') {
			type = false;
		}
		
		string name;
		size_t pos_in_line;
		for (size_t i = 1; i < line.size(); i++) {
			if (line[i] ==	' ' && name.size() > 0) {
				pos_in_line = i;
				break;
			} else if (line[i] != ' ') {
				name.push_back(line[i]);
			}
		}
		
		for (size_t i = pos_in_line + 1; i < line.size(); i++) {
			if (line[i] !=	' ') {
				pos_in_line = i;
				break;
			}
		}
		
		for (size_t i = pos_in_line + 1; i < line.size(); i++) {
			if (line[i] ==	' ') {
				pos_in_line = i;
				break;
			}
		}
		
		for (size_t i = pos_in_line + 1; i < line.size(); i++) {
			if (line[i] !=	' ') {
				pos_in_line = i;
				break;
			}
		}
		
		vector<string> hashes;
		// falta agregar los hashes al vector
		if (type) {
			this->hashes_by_tag.insert(pair<string, vector<string>> (name, hashes));
		} else {
			this->hashes_by_file.insert(pair<string, vector<string>> (name, hashes));
		}
	}
}

// puede ser que falte verificar algo en los add o simplemente hacer 2
// bool is_at_index()
// cambiar los parametros de vectro por string
void Index::add_tag(string tag, vector<string> hashes) {
	this->hashes_by_tag.insert(pair<string, vector<string>> (tag, hashes));
}


void Index::add_file(string namefile, vector<string> hashes) {
	this->hashes_by_file.insert(pair<string, vector<string>> (namefile, hashes));
}


bool Index::contains_file_and_hash(string filename, string hash) {
	map<string, vector<string>>::iterator it = this->hashes_by_file.find(filename);
	if (it != this->hashes_by_file.end()) {
		if (std::find(it->second.begin(), it->second.end(), hash) 
		!= it->second.end()) {
			return true;
		}
	}
	return false;
}


bool Index::contains_tag(string tag) {
	map<string, vector<string>>::iterator it = this->hashes_by_tag.find(tag);
	return it != this->hashes_by_file.end();
}


void Index::overwrite() {
	std::ios_base::openmode flags = ios::out | ios::trunc;
	File index(this->index_namefile.c_str(), flags);
	
	for (map<string, vector<string>>::iterator it = this->hashes_by_file.begin(); 
	it != this->hashes_by_file.end(); ++it) {
		string line;
		line += string("t ");
		line += it->first;
		line += string(" hash ");

		// ordeno vector de hashes por orden alfabetico y lo sumo a la linea
		std::sort(it->second.begin(), it->second.end());
		for (size_t i = 0; i < it->second.size(); i++) {
			line += it->second[i];
			if (i != it->second.size() - 1) {
				line += string(" ");
			}
		}
		line += string(";\n");
		index.write(line);
	}
	
	for (map<string, vector<string>>::iterator it = this->hashes_by_tag.begin(); 
	it != this->hashes_by_tag.end(); ++it) {
		string line;
		line += string("f ");
		line += it->first;
		line += string(" hash ");

		// ordeno vector de hashes por orden alfabetico y lo sumo a la linea
		std::sort(it->second.begin(), it->second.end());
		for (size_t i = 0; i < it->second.size(); i++) {
			line += it->second[i];
			if (i != it->second.size() - 1) {
				line += string(" ");
			}
		}
		line += ";\n";
		index.write(line);
	}
}


Index::~Index() {}
