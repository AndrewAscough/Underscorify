/*

ALRIGHT HERE WE GO AGAIN. So the goal for this part of the project is to go through any file and store each unique word into a hashmap. 
	For this hashmap to be completed I would like to be able to:
		- Add a new string
		- Remove a string
		- Lookup a string
		- Handle Collisions for entries that may have the same key
*/
#include "hash.h"


int main()
{
	hashTable h1;
	std::cout<<"printing hash table..." <<std::endl;
	h1.print();

	h1.addEntry("test");
	h1.addEntry("test");
	std::cout<<"printing hash table..." <<std::endl;
	h1.print();

	return 0;
}