#ifndef DATA_OPTIMIZER_H
#define DATA_OPTIMIZER_H

#include "FolderCreator.h"

// can't think of a better name, but this class will manage 
// exporting meshes so that meshes can be reused instead of exporting the same mesh multiple times
class DataOptimizer {
private:
	std::map<void *, int> indexes; // maps pointers to indexes
	int most_recently_added_index = 0;

	std::string base_directory;

public:

	DataOptimizer(std::string base_directory_in) {
		base_directory = base_directory_in;
	}

	DataOptimizer() {
		base_directory = ".";
	}

	
	bool checkExists(void * pointer) {
		return indexes.find(pointer) != indexes.end();
	}

	int getIndex(void * pointer) {
		return indexes[pointer];
	}

	// returns index for this data set and creates a directory to store this data
	int addData(void * pointer) {
		most_recently_added_index++;
		indexes[pointer] = most_recently_added_index;
		createFolder(base_directory + std::string("/") + std::to_string(most_recently_added_index));
		return most_recently_added_index;
	}

	std::string getDataDirectory(int index) {
		return base_directory + std::string("/") + std::to_string(index);
	}
};

#endif