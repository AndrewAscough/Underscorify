/*

ALRIGHT HERE WE GO AGAIN. So the goal for this part of the project is to go through any file and store each unique word into a hashmap. 
	For this hashmap to be completed I would like to be able to:
		- Add a new string
		- Remove a string
		- Lookup a string
		- Handle Collisions for entries that may have the same key
*/
#include "hash.h"
#include <fstream>

int main(int argc, char* argv[])
{
	

	for(int i=1;i<argc;i++)
	{
		//Init hashtable
		hashTable h1;
		std::cout<<"printing initialised hash table..." <<std::endl;
		h1.print();

		//Init input file
		std::ifstream inputFile;
		inputFile.open(argv[i]);
		std::string word;
		//Word by word add it to the hashtable.
		while(inputFile >> word)
		{
			h1.addEntry(word);
		}
		//display final hashtable result
		std::cout<<"printing file's hash table..." <<std::endl;
		h1.print();
		h1.printDist();
	}


	return 0;
}