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
		Index(Index&& other);
		void add_tag(std::string tag, std::string hash);
		void add_file(std::string namefile, std::string hash);
		std::string get_namefile_by_hash(std::string hash);
		bool contains_file_and_hash(std::string filename, std::string hash);
		bool contains_tag(std::string tag);
		bool contains_hash_stored(std::string hash);
		std::vector<std::string> get_hashes_by_tag(char* tag);
		void overwrite();
		~Index();
};

#endif // INDEX_H
