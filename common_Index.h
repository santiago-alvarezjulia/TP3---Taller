#ifndef INDEX_H
#define INDEX_H

#include <map>
#include <string>
#include <vector>

class Index {
    std::map<std::string, std::vector<std::string>> hashes_by_file;
    std::map<std::string, std::vector<std::string>> hashes_by_tag;
    std::string index_namefile;
    void initialize_index(std::string index_namefile);
    
    public:
		explicit Index(std::string index_namefile);
		void add_tag(std::string tag, std::vector<std::string> hashes);
		void add_file(std::string namefile, std::vector<std::string> hashes);
		bool contains_file_and_hash(std::string filename, std::string hash);
		bool contains_tag(std::string tag);
		void overwrite();
		~Index();
};

#endif // INDEX_H
