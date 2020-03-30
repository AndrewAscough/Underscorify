#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <string>

namespace Hashify {
	
	struct item {
		std::string word;
		int order;
		item *nextItem;
	};

	class hashTable {
		public:
			hashTable();
			//Adds an entry to the hashtable.
			void addEntry(std::string s);
			//Prints hash table and each entry at each location
			void print();
			//Prints number of entries at each point of the table and the %of the total
			void printDist();
			//Pops an item off the hashtable beginning from hashtable[0]-->end tail and finishes popping each tail item until it moves on to hashtable[1]
			item popEntry();

		private:
			//Returns hashcode of string using polynomial rolling hash function
			int hashCode(std::string key);
			//Searches if an item is in the hashtable already
			bool searchItem(std::string s);
			//Deletes a tail item off the hashtable.
			void deleteTailEntry(std::string s);			

			static const int tableSize=53;
			int inOrder;
			item* HashTable[tableSize];

	};
};

#endif //HASH_H