#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
/*
Back to a good ol' blank slate here. So I can now assume my input will ALWAYS be a c++ file with no symbol words but may still have some comments and strings around

For a rough outline of how i plan on handling this part of the project I'll need to do a few things to successfully convert it all to underscores.

The first goal i think overall should be to take in an input file that's been spaceified but has no comments. My desired output will then be that same file (minus the formatting lol)
except with every variable name changed to some kind of underscore. Now i think this is gonna be so basic I won't actually be able to handle having an input like int main() because
I dont want to bother with #define yet.
	Sub Goals:
		1. Detect when a variable is declared. ie int i, char c, std::string s, etc
		2. Store the variable name in some kind of hashmap
			a) Decide on a good way to key the value
			b) Once variable is stored, decide on how many _'s to replace it with. So if i had int i=0; and char c=0; i'd want i = _ and c = __
		3. Go back through the parsed file and replace each variable name with corresponding underscore amounts

Next goal I think is to take in any parsed cpp file without strings and comments and change everything to _'s as this shouldnt be too far a step above the last part.
*/

//For Goal 1 subgoal 1 to detect if something is about to be a variable name it will be preceded by: int, char, bool, float, void, and surprisingly wchar_t too. Never used that last one before..
//Okay big problem.. What happens when i include BASICALLY ANY LIBRARY THAT HAS ITS OWN TYPE DEFINITIONS. A good friend of mine suggested I look at how the compiler does it and i guess thats
//what ill need to do 
bool isDataType(std::string s)
{
	if(	s == "int" 		||
		s == "char" 	||
		s == "bool" 	||
		s == "float" 	||
		s == "void" 	||
		s == "wchar_t")
	{
		return true;
	}
	return false;
}

int main(int argc, char* argv[])
{
	std::ifstream inputFile;
	inputFile.open(argv[1]);

	std::string line;

	while(std::getline(inputFile,line))
	{
		std::string word;
		std::istringstream currentLine(line);

		while(currentLine >> word)
		{
			if(isDataType(word))
			{
				std::cout<<"data type found as: " << word << " with variable name = ";
				currentLine >> word;
				std::cout<<word<<std::endl;
			}
		}
	}
}