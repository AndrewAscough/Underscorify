#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

/*
NEXT GOALS
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Okay next goal will be a little more challenging. Im debating right now if its better to "preprocess" the file, ie go through it and generate a new file with spaces
inbetween each word to ensure there's no symbol words remaining and then by using that file for the underscorifying
ORRRRR trying to "fix" (by add spaces to symbol words) the file as I go.
I like to follow the K.I.S.S. principle of keep it simple and i can see trying to do it as i go (at least at first ill probably come back to it) to be too complex for me.

So next commit I want my program to generate a new file, with each word separated by a space all on one line (because lets be real we're trying to make this unreadable anyway
so formatting can take a back seat until later iterations of this program.) and for there to be no remaining symbol words.
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

//checks if the string s contains a special character (defined by anything that isnt an underscore OR alphanumeric) AND an alphanumeric character
bool isSymbolWord(std::string s)
{
	bool symbol=false ;
	bool alphanumeric=false;

	for(int i=0;i<s.length();i++)
	{
		if( (s[i]>=48 && s[i]<=57 ) ||
			(s[i]>=65 && s[i] <=90) ||
			(s[i]>=97 && s[i]<=122))
			{
				alphanumeric=true;
			}
		else
		{
			symbol=true;
		}
	}
	if(symbol && alphanumeric)
	{
		return true;
	}
	return false;
}

//if the character is a symbol the function returns true.
bool isSymbol(char c)
{

	if( (c>=48 && c<=57 ) ||
		(c>=65 && c<=90) ||
		(c>=97 && c<=122))
		{
			return false;
		}
		else
		{
			return true;
		}
	
	return false;
}

//Takes in a symbolword string and returns a nonsymbolword
std::string spaceifySymbolWord(std::string symbolWord)
{
	std::string result;

	//i want the input to go from something like 0; to 0 ;
	//or something like int i=0; to go to int i = 0 ;
	for(int i=0;i<symbolWord.length();i++)
	{
		//So the encountered part of the string is in fact a symbol.
		if(isSymbol(symbolWord[i]))
		{
			result+=" ";
			result+=symbolWord[i];
			result+=" ";
		}
		else
		{
			result+=symbolWord[i];
		}
	}
	return result;
}

int main(int argc, char *argv[])
{
	//Check if there is no filename being given
	if(argc < 2)
	{
		std::cout<<"Error, use is ./Underscorify [filename]"<<std::endl;
		return 1;
	}

	//Opens each file
	for(int i=1;i<argc;i++)
	{
		std::ifstream inputFile;
		inputFile.open(argv[i]);

		std::string word;

		//Prints each word of the file on a new line
		while(inputFile >> word)
		{
			//If the encountered word is a symbol word, we need to break it down
			if(isSymbolWord(word))
			{
				std::cout<<spaceifySymbolWord(word)<<std::endl;
			}
			//Word is not a symbol word, it can remain unchanged.
			else
			{
				std::cout<<word<<std::endl;
			}
		}
		inputFile.close();
	}
	
	return 0;
	
}