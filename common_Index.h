#ifndef INDEX_H
#define INDEX_H

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

class Index {
    map<string, vector<string>> map_files;
    map<string, vector<string>> map_tags;
    string index_namefile;
    void initialize_index(string index_namefile);
    
    public:
		Index(string index_namefile);
		
		void add_tag(string tag, vector<string> hashes);
		void add_file(string namefile, vector<string> hashes);
		void overwrite();
		
		~Index();
};

#endif // INDEX_H
