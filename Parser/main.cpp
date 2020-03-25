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

//Determines if input is currently interacting with a string or not
int isString = 0;

//checks if the string s contains a special character (defined by anything that isnt an underscore OR alphanumeric) AND an alphanumeric character
bool isSymbolWord(std::string s)
{
	bool symbol=false ;

	for(int i=0;i<s.length();i++)
	{
		if( (s[i]>=48 && s[i]<=57 ) ||
			(s[i]>=65 && s[i] <=90) ||
			(s[i]>=97 && s[i]<=122))
		{
			continue;
		}
		else
		{
			symbol=true;
		}
	}
	if(symbol)
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

//Checks if a character is an opperator which are: =, <, >, +, -, /, *, ^, !, %, :, |
bool isOperand(char c)
{
	if(	c == '=' ||
		c == '<' ||
		c == '>' ||
		c == '+' ||
		c == '-' ||
		c == '/' ||
		c == '*' ||
		c == '^' ||
		c == '!' ||
		c == '%' ||
		c == ':' ||
		c == '|')
	{
		return true;
	}
	return false;
}

//searches the string if it has a ';' character it'll want to add a newline after it.
bool newLineNeeded(std::string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i] == ';')
		{
			return true;
		}
	}
	return false;
}

//Takes in a symbolword string and returns a nonsymbolword by separating the symbols with spaces. Also handles string opening and closings without modifying the string by checking isString.
std::string spaceifySymbolWord(std::string symbolWord)
{
	std::string result;
	std::cerr<<std::endl<<"SPACEIFY CALLED WITH WORD = " << symbolWord << std::endl;

	for(int i=0;i<symbolWord.length();i++)
	{
		//So the encountered part of the string is in fact a symbol.
		if(isSymbol(symbolWord[i]))
		{
			//Checks to see if the symbolword is entering/exiting a string. This is to handle a string like: "hellothere&&"; or '%' currently within one
			if(symbolWord[i] == 34 || symbolWord[i] == 39)
			{
				//Entering the string
				if(isString%2==0)
				{
					std::cerr<<std::endl <<"ENTERING THE STRING AT RESULT = " << result << " WITH CHARACTER ENCOUNTERED AT POSITION " << i << " isString = " << isString << " WORD = " << symbolWord <<std::endl;
					//result+=" ";
					result+=symbolWord[i];
				}
				//Exiting the string
				else
				{
					std::cerr<<std::endl <<"EXITING THE STRING AT RESULT = " << result << " WITH CHARACTER ENCOUNTERED AT POSITION " << i << " isString = " << isString << " WORD = " << symbolWord <<std::endl;					
					result+=symbolWord[i];
					//result+=" ";
				}
				isString++;
			}
			//Section is not a string but IS an operand so it lumps the two symbols together and moves on
			else if((isOperand(symbolWord[i]) && isOperand(symbolWord[i+1])) && isString%2 == 0)
			{
				std::cerr<<std::endl<<"OPERAND FOUND AT " << i << " IT WAS: " << symbolWord[i] << symbolWord[i+1] << std::endl;
				result+=" ";
				result+=symbolWord[i];
				result+=symbolWord[i+1];
				result+=" ";
				i++;
			}
			//Section is not an operand, but it is in a string so it does nothing.
			else if(isString%2 == 1)
			{
				result+=symbolWord[i];
			}
			//Section is not an operand, nor is it in a string so it splits the symbol away.
			else
			{
				result+=" ";
				result+=symbolWord[i];
				result+=" ";
			}
		}
		//Encountered part of string is just a normal alphanumeric char so do nothing.
		else
		{
			result+=symbolWord[i];
		}
	}
	std::cerr<<"SPACEIFY RETURNED " << result <<std::endl;
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
		std::cerr<<"(int)'\"' = " << (int)'\"' << std::endl;
		std::ifstream inputFile;
		inputFile.open(argv[i]);

		std::string word;

		//Prints each word of the file on a new line
		while(inputFile >> word)
		{
			//Preprocessor directive thingy, essentially not something I want to mess with. Think #include <iostream>
			if(word[0] == '#')
			{
				std::cout<<word<<" ";
				inputFile>>word;
				std::cout<<word<<std::endl;
			}
			//for some resemblance of "formatting"
			else if(newLineNeeded(word))
			{
				std::cout<<spaceifySymbolWord(word)<<std::endl;
			}
			//Handles breaking up symbol words and passing through strings without changing the interior.
			else if(isSymbolWord(word))
			{
				std::cout<<spaceifySymbolWord(word)<<" ";		
			}
			//Word is not a symbol word, it can remain unchanged.
			else
			{
				std::cout<<word<<" ";
			}
		}
		inputFile.close();
	}
	
	return 0;
	
}