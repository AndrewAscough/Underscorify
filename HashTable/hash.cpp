#include "hash.h"

Hashify::hashTable::hashTable()
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
void Hashify::hashTable::print()
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
void Hashify::hashTable::printDist()
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

//Pops an item off the hashtable beginning from hashtable[0]-->end tail and finishes popping each tail item until it moves on to hashtable[1]
Hashify::item Hashify::hashTable::popEntry()
{
	//Generates blank return val
	item returnVal;
	returnVal.word="";
	returnVal.order=-1;
	returnVal.nextItem = NULL;

	for(int i=0;i<tableSize;i++)
	{
		//valid entry has been found
		if(HashTable[i]->word != "")
		{
			//Entry has no tail
			if(HashTable[i]->nextItem == NULL)
			{
				returnVal.word = HashTable[i]->word;
				returnVal.order = HashTable[i]->order;
			}
			//Skips to end of tail
			else
			{
				item *ptr = HashTable[i];
				while(ptr->nextItem != NULL)
				{
					ptr = ptr->nextItem;
				}
				returnVal.word = ptr->word;
				returnVal.order = ptr->order;
			}
			//Pops entry off
			deleteTailEntry(returnVal.word);
			return returnVal;
		}
	}

	return returnVal;
}

bool Hashify::hashTable::searchItem(std::string s)
{
	int index = hashCode(s);
	if(HashTable[index]->word == s)
	{
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

//Adds a unique entry to the hashtable
void Hashify::hashTable::addEntry(std::string s)
{
	//If string is already in hash table we dont need to readd it
	if(searchItem(s))
	{
		return;
	}
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

//Deletes entry at the end of the tail or resets it to default if theres no tail left.
void Hashify::hashTable::deleteTailEntry(std::string s)
{
	//If the item is not in the hashtable, do nothing
	if(!searchItem(s))
	{
		return;
	}

	int index = hashCode(s);
	item *ptr = HashTable[index];
	//Tail entry is first entry
	if(ptr->word == s)
	{
		ptr->word ="";
		ptr->order = -1;
		ptr->nextItem = NULL;
		return;
	}
	//Tail entry is somewhere further in the list
	while(ptr->nextItem->nextItem != NULL)
	{
		ptr = ptr->nextItem;
	}
	item *deletionTarget = ptr->nextItem;
	ptr->nextItem = NULL;
	delete deletionTarget;
	return;
}

//Returns index value of string's hash using a polynomial rolling hash function. 
int Hashify::hashTable::hashCode(std::string key)
{
	const int p = 83;
	long long hashVal = 0;
	long long power = 1;
	//Using a hashfunction like this one https://cp-algorithms.com/string/string-hashing.html
	for(int i=0;i<key.length();i++)
	{
		hashVal = (hashVal + ((int)key[i])* power) % tableSize;
		power = (power*p) % tableSize;
	}
	int index = hashVal;

	return index;
}