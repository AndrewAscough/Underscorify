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
		std::cout<<"#"<<i<<" "<<HashTable[i]->word << " " << HashTable[i]->order;
		
		item *ptr=HashTable[i];

		while(ptr->nextItem != NULL)
		{
			ptr = ptr->nextItem;
			std::cout<<"|"<<ptr->word << " " << ptr->order;
		}
		std::cout<<std::endl;
	}
}

bool searchItem(std::string s)
{
	return false;
}

//For now assume there are no duplicate entries
void hashTable::addEntry(std::string s)
{
	//If string is already in hash table we dont need to readd it
	/*
	if(searchTable(s))
	{
		return;
	}*/

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

//Im gonna make a better hashcode function later..
int hashTable::hashCode(std::string key)
{
	int hash =0;
	int index = 0;

	for(int i=0;i<key.length();i++)
	{
		hash += key[i];
	}

	index = hash % tableSize;

	return index;
}