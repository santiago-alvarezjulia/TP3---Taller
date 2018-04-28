#ifndef INDEX_H
#define INDEX_H

#include <map>
#include <string>
#include <vector>
using std::map;
using std::string;
using std::vector;

class Index {
    map<string, vector<string>> hashes_by_file;
    map<string, vector<string>> hashes_by_tag;
    string index_namefile;
    void initialize_index(string index_namefile);
    
    public:
		explicit Index(string index_namefile);
		void add_tag(string tag, vector<string> hashes);
		void add_file(string namefile, vector<string> hashes);
		bool contains_file_and_hash(string filename, string hash);
		bool contains_tag(string tag);
		void overwrite();
		~Index();
};

#endif // INDEX_H
