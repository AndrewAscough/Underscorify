#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

/*
NEXT GOALS
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
IT ACTUALLY WORKS (mostly.. see bug list). For next goals i think for a little bit im done with modifying this file. Id like to organise this directory more so that the
tests are in a separate file. Id also like my bash script to compile all the cpp files after this parser is run on them to see if they still work as intended. Furthering
that, id like to parse this parser file, compile the output and see if it still works the exact same. Again it has to work THE EXACT same, so no strings can be modified
or anything.

Furthering that when I actually come back to modifying this I'd like to make it generate an output file where it takes in the file name and adds "out" before the .cpp or
whatever. Then I'd actually like to change this to a header and class file.

Next stage after that is storing variable names and FINALLY UNDERSCORIFYING THEM YES.

Bugs to fix:
		- Large amounts of excess whitespace are being added. Though tbh with the goal of this program being to make it unreadable i dont necessarily see this as bad.
		Its more a feature than a bug ;)
		- Currently this won't be able to handle something like a bit shift operator ie <<= or >>= as its 3 operands in a row and i only check 2.
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
		//These number ranges are all alphanumeric characters AND including underscores ;)
		if( (s[i]>=48 && s[i]<=57 ) ||
			(s[i]>=65 && s[i] <=90) ||
			(s[i]>=97 && s[i]<=122) ||
			 s[i]==95)
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

	if( (c>=48 && c<=57) ||
		(c>=65 && c<=90) ||
		(c>=97 && c<=122)||
		 c==95)
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
	std::string result;
	//std::cerr<<std::endl<<"SPACEIFY CALLED WITH WORD = " << symbolWord << std::endl;

	for(int i=0;i<symbolWord.length();i++)
	{
		//So the encountered part of the string is in fact a symbol.
		if(isSymbol(symbolWord[i]))
		{
			//Checks to see if the symbolword is entering/exiting a string. This is to handle a string like: "hellothere&&"; or '%' currently within one
			if(symbolWord[i] == 34 || symbolWord[i] == 39)
			{
				result+=symbolWord[i];
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
	//result+=" ";
	//std::cerr<<"SPACEIFY RETURNED " << result <<" with length = " << result.length()<<std::endl;
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
		if(line[i] == 34 || line[i] == 39)
		{
			buffer+=line[i];
			i++;
			result+=spaceifySymbolWord(buffer);
			buffer = "";
		}
		//Word block has encountered a comment beginning and up until now the buffer contains non commented words. The iscomment%2 is used here to ensure we dont have a line like /* foo/* bar*/
		else if(	(line[i] == '/' && line[i+1] == '*') && isComment%2 == 0 && isString%2 == 0)
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

			//Increment our iscomment to notify the main program that we are now handling a comment
			isComment++;
		}
		//Word block has encountered a comment end. The buffer now contains unmodified strings from the comment
		else if( (line[i] == '*' && line[i+1] == '/') && isComment%2 == 1 && isString%2 == 0)
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
		
		//if line is currently not within a string, add the char to buffer, else it will go to the result.
		if(isString%2 ==0)
		{
			buffer+=line[i];
		}
		else
		{
			result+=line[i];
		}
	}
	if(isComment%2 == 1)
	{
		result+=buffer;
	}
	else
	{
		result+=spaceifySymbolWord(buffer);
	}
	return result;
}

//adds "out" before the . in a normal file (if there is a dot). If there isnt it will add it to the end of the filename
std::string nameOutputFile(std::string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i] == '.')
		{
			s.insert(i,"out");
			return s;
		}
	}
	s+="out";
	return s;
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
		
		//Generate output file
		std::string inputFileName(argv[i]);
		std::ofstream outputFile(nameOutputFile(inputFileName).c_str());

		std::string line;
		
		//Prints each word of the file on a new line
		while(std::getline(inputFile,line))
		{
			std::string word;
			std::istringstream currentLine(line);

			//Preprocessor directive thingy, essentially not something I want to mess with. Think #include <iostream> AND this isnt found in a comment
			if(line[0] == '#' && isComment%2 ==0)
			{
				outputFile<<line<<std::endl;
			}
			//Check if the word contains a block comment OR if it is currently still in one and that it isnt in astring. Also handles when there is both a line comment and block comment on the same line
			else if((isBlockComment(line) || isComment%2 == 1) && isString%2 == 0)
			{
				outputFile<<extractBlockComment(line)<<std::endl;
			}
			//Check if the REST of the line is a // comment
			else if(isLineComment(line))
			{
				outputFile<<extractLineComment(line)<<std::endl;
			}
			//Line does not contain a comment so must be examined
			else
			{
				while(currentLine >> word)
				{
					//for some resemblance of "formatting" when a ; is encountered it will put it on a new line
					if(newLineNeeded(word))
					{
						outputFile<<spaceifySymbolWord(word)<<std::endl;
					}
					//Handles breaking up symbol words and passing through strings without changing the interior.
					else if(isSymbolWord(word))
					{
						outputFile<<spaceifySymbolWord(word)<<" ";		
					}
					//Word is not a symbol word, it can remain unchanged.
					else
					{
						outputFile<<word<<" ";
					}
				}
			}
		}

		inputFile.close();
		outputFile<<std::endl;
		outputFile.close();
	}
	
	return 0;
	
}