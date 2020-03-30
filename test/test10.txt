#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

/*
NEXT GOALS
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
This is really starting to come together this part of the project. I think the next step I should spend helping the program to be able to handle comments. Now I believe
this will be another large challenge because unlike strings you can have comments that have no clear end really if you dont have the WHOLE line of input. So I think I will
need to change the way i go through the file by using "getline" instead of just going word by word. After that Ill need to be able to detect when we are exiting or entering
a comment block such as this one with the slash start OR the comment thats just two slashes.

Bugs to fix:
		- Large amounts of excess whitespace are being added. Though tbh with the goal of this program being to make it unreadable i dont necessarily see this as bad.
		Its more a feature than a bug ;)
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

//Determines if input is currently interacting with a string. if isString%2==0, it is not a string
int isString=0;
//Determins if input is in a comment block or not. if isComment%2==0, it is not a comment
int isComment=0;

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

//Checks if a character is an opperator which are: =, <, >, +, -, /, *, ^, !, %, :, |, &
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
		c == '|' ||
		c == '&')
	{
		return true;
	}
	return false;
}

//Searches the string to see if the word is a comment
bool isLineComment(std::string s)
{
	for(int i=0;i<s.length();i++)
	{
		if((s[i] =='/' && s[i+1] == '/'))
		{
			return true;
		}
	}
	return false;
}

//Searches the string to see if the word contains /* or */
bool isBlockComment(std::string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(	(s[i] =='/' && s[i+1] == '*') ||
			(s[i] =='*' && s[i+1] == '/'))
		{
			return true;
		}
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
	std::string result = " ";
	//std::cerr<<std::endl<<"SPACEIFY CALLED WITH WORD = " << symbolWord << std::endl;

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
					//std::cerr<<std::endl <<"ENTERING THE STRING AT RESULT = " << result << " WITH CHARACTER ENCOUNTERED AT POSITION " << i << " isString = " << isString << " WORD = " << symbolWord <<std::endl;
					result+=symbolWord[i];
				}
				//Exiting the string
				else
				{
					//std::cerr<<std::endl <<"EXITING THE STRING AT RESULT = " << result << " WITH CHARACTER ENCOUNTERED AT POSITION " << i << " isString = " << isString << " WORD = " << symbolWord <<std::endl;					
					result+=symbolWord[i];
				}
				isString++;
			}
			//Section is not a string but IS an operand so it lumps the two symbols together and moves on
			else if((isOperand(symbolWord[i]) && isOperand(symbolWord[i+1])) && isString%2 == 0)
			{
				//std::cerr<<std::endl<<"OPERAND FOUND AT " << i << " IT WAS: " << symbolWord[i] << symbolWord[i+1] << std::endl;
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
	//std::cerr<<"SPACEIFY RETURNED " << result <<std::endl;
	result+=" ";
	return result;
}

//Extracts basic comments from a string s that contain "//"
std::string extractLineComment(std::string line)
{
	std::string result;
	std::string remainderString;


	for(int i=0;i<line.length();i++)
	{
		if(line[i] == '/' && line[i+1] == '/')
		{
			while(i<line.length())
			{
				result+=line[i];
				i++;
			}
		}
		else
		{
			remainderString+=line[i];
		}
	}
	//Readds the part of the string that wasnt a comment.
	result.insert(0,spaceifySymbolWord(remainderString));

	return result;
}

//Extracts complex comments where a line could contain multiple /* */ comments
std::string extractBlockComment(std::string line)
{
	std::string result;
	std::string buffer;

	for(int i=0;i<line.length();i++)
	{
		//String block has encountered a comment beginning and up until now the buffer contains non commented strings. The iscomment%2 is used here to ensure we dont have a line like /* foo/* bar*/
		if(	(line[i] == '/' && line[i+1] == '*') && isComment%2 == 0)
		{
			//Add current word to result after separating out the symbolwords
			result+= spaceifySymbolWord(buffer);
			//reset the buffer to prepare to accept commented strings
			buffer ="";

			//add the comment beginning of /* to result
			result+=line[i];
			i++;
			result+=line[i];
			i++;

			//Increment our iscomment to notify the main program that we are now handling a comment string.
			isComment++;
		}
		//String block has encountered a comment end. The buffer now contains unmodified strings from the comment
		else if( (line[i] == '*' && line[i+1] == '/') && isComment%2 == 1)
		{
			//add buffer to result
			result+= buffer;

			//add the comment tail of */
			result+=line[i];
			i++;
			result+=line[i];
			i++;

			//ensure theres a gap between this and the next string.
			result+=" ";

			//reset buffer
			buffer="";

			//increment isComment to signal we are no longer in a comment block
			isComment++;
		}
		//The line contains outside of a block comment, a comment line. This also would be the end of whatever this function needs to do so we break after
		else if( (line[i] == '/' && line[i+1] == '/') && isComment%2 == 0)
		{
			result+=extractLineComment(line.substr(i,line.length()-i));
			break;
		}
		//Adds current char to buffer, buffer will be modified each time isBlockComment returns true.
		buffer+=line[i];
	}
	if(isComment%2==1)
	{
		result+=buffer;
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

		std::string line;
		
		//Prints each word of the file on a new line
		while(std::getline(inputFile,line))
		{
			std::string word;
			std::istringstream currentLine(line);

			//Check if the word contains a block comment OR if it is currently still in one. Also handles when there is both a line comment and block comment on the same line
			if(isBlockComment(line) || isComment%2 == 1)
			{
				std::cout<<extractBlockComment(line)<<std::endl;
			}
			//Check if the REST of the line is a // comment
			else if(isLineComment(line))
			{
				std::cout<<extractLineComment(line)<<std::endl;
			}
			//Line does not contain a comment so must be examined
			else
			{
				while(currentLine >> word)
				{
					//Preprocessor directive thingy, essentially not something I want to mess with. Think #include <iostream>
					if(word[0] == '#')
					{
						std::cout<<word<<" ";
						while(currentLine>>word)
						{
							std::cout<<word<<" ";
						}
						std::cout<<std::endl;
						break;
					}
					//for some resemblance of "formatting" when a ; is encountered it will put it on a new line
					else if(newLineNeeded(word))
					{
						std::cout<<spaceifySymbolWord(word)<<std::endl;
					}
					//Handles breaking up symbol words and passing through strings without changing the interior.
					else if(isSymbolWord(word))
					{
						std::cout<<spaceifySymbolWord(word);		
					}
					//Word is not a symbol word, it can remain unchanged.
					else
					{
						std::cout<<word<<" ";
					}
				}
			}
		}

		inputFile.close();
	}
	
	return 0;
	
}