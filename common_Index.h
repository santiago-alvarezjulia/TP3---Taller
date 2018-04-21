#ifndef INDEX_H
#define INDEX_H

#include <unordered_map>
#include <string>
#include <vector>
using std::unordered_map;
using std::string;
using std::vector;

class Index {
    unordered_map<string, vector<string>> map_files;
    unordered_map<string, vector<string>> map_tags;
    
    public:
		Index();
		
		void add_tag(string tag, vector<string> hashes);
		void add_file(string namefile, vector<string> hashes);
		
		~Index();
};

#endif // INDEX_H
