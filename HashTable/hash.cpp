#include "hash.h"

hashTable::hashTable()
{
	inOrder = 0;

	for(int i=0;i<tableSize;i++)
	{
		HashTable[i] = new item;
		HashTable[i]->word ="";
		HashTable[i]->order = -1;
		HashTable[i]->nextItem = NULL;
	}
}

//Prints hashtable
void hashTable::print()
{
	for(int i=0;i<tableSize;i++)
	{
		std::cout<<"#"<<i<<" "<<HashTable[i]->word << " order: " << HashTable[i]->order;
		
		item *ptr=HashTable[i];

		while(ptr->nextItem != NULL)
		{
			ptr = ptr->nextItem;
			std::cout<<"|"<<ptr->word << " order: " << ptr->order;
		}
		std::cout<<std::endl;
	}
}

//prints the rough distribution of words throughout the buckets
void hashTable::printDist()
{
	std::cout<<"number of uniques = " << inOrder << std::endl;

	for(int i=0;i<tableSize;i++)
	{
		int c=1;
		float percentContained;

		std::cout<<"#"<<i<<" ";
		item *ptr=HashTable[i];
		while(ptr->nextItem != NULL)
		{
			ptr = ptr->nextItem;
			c++;
		}
		percentContained = (c/inOrder) * 100;
		std::cout << c << " which is " << (float)((float)c/(float)inOrder)*100 <<"%" << std::endl;
	}
}


bool hashTable::searchItem(std::string s)
{
	int index = hashCode(s);
	if(HashTable[index]->word == s)
	{
		std::cout<<"didnt add word: " << s <<std::endl;
		return true;
	}

	item *ptr = HashTable[index];

	while(ptr->nextItem != NULL)
	{
		ptr = ptr->nextItem;
		if(ptr->word == s)
		{
			return true;
		}
	}

	return false;
}

//For now assume there are no duplicate entries
void hashTable::addEntry(std::string s)
{
	//If string is already in hash table we dont need to readd it
	if(searchItem(s))
	{
		return;
	}
	std::cout<<"added word: " << s << std::endl;
	int index = hashCode(s);
	inOrder++;

	if(HashTable[index]->word == "")
	{
		HashTable[index]->word = s;
		HashTable[index]->order = inOrder;
	}
	else
	{
		item *ptr = HashTable[index];
		//generate new entry
		item *entry;
		entry = new item;
		entry->word = s;
		entry->order = inOrder;
		entry->nextItem = NULL;

		//takes me to the place where nextItem would be empty, meaning i can throw this new one in there
		while(ptr->nextItem != NULL)
		{
			ptr = ptr->nextItem;
		}
		ptr->nextItem = entry;
	}
}

//Returns index value of string's hash using a polynomial rolling hash function. 
int hashTable::hashCode(std::string key)
{
	const int p = 83;
	long long hashVal = 0;
	long long power = 1;
	//Using a hashfunction like this one https://cp-algorithms.com/string/string-hashing.html
	for(int i=0;i<key.length();i++)
	{
		hashVal = (hashVal + ((int)key[i])* power) % tableSize;
		power = (power*p)%tableSize;
	}
	int index = hashVal;

	return index;
}