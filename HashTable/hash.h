#include <iostream>
#include <string>

#ifndef HASH_H
#define HASH_H

class hashTable {

public:
	hashTable();
	int hashCode(std::string key);
	void addEntry(std::string s);
	bool searchItem(std::string s);
	void print();

private:
	static const int tableSize=53;
	int inOrder;
	struct item{
		std::string word;
		int order;
		item *nextItem;
	};

	item* HashTable[tableSize];
};

#endif //HASH_H