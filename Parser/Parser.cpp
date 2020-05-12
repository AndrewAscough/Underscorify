#include "Parser.h"

Parsify::Parser::Parser(int argc, char* argv[])
{
	isString = 0;
	isComment = 0;

	for(int i=1;i<argc;i++)
	{
		std::string s(argv[i]);
		files.push_back(s);
	}
}

void Parsify::Parser::Parse()
{
	//Opens each file
	for(int i=0;i<files.size();i++)
	{
		std::ifstream inputFile;
		inputFile.open((files.at(i)).c_str());
		
		//Generate output file
		std::string inputFileName = files.at(i);
		std::ofstream outputFile(nameOutputFile(inputFileName).c_str());

		std::string line;
		
		//Takes in lines of file and splits any symbolWords while leaving comments and strings unchanged.
		while(std::getline(inputFile,line))
		{
			std::string word;
			std::istringstream currentLine(line);

			//Preprocessor directive thingy, essentially not something I want to mess with. Think #include <iostream>
			if(line[0] == '#' && isComment%2 ==0)
			{
				outputFile<<line<<std::endl;
			}
			//Check if the word contains a block comment OR if it is currently still in one and that it isnt in a string. Also handles when there is both a line comment and block comment on the same line
			else if((isBlockComment(line) || isComment%2 == 1) && isString%2 == 0)
			{
				outputFile<<extractBlockComment(line)<<std::endl;
			}
			//Splits and spaceifys whatever comes before the //comment
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
}

//Takes in a symbolword string and returns a nonsymbolword by separating the symbols with spaces. Also handles string opening and closings without modifying the string by checking isString.
std::string Parsify::Parser::spaceifySymbolWord(std::string symbolWord)
{
	std::string result;

	for(int i=0;i<symbolWord.length();i++)
	{
		//Encountered part of the string is a symbol.
		if(isSymbol(symbolWord[i]))
		{
			//If a \ is encountered make sure to add that and the next character to the result before moving on.
			if(symbolWord[i] == '\\')
			{
				result+=symbolWord[i];
				i++;
				result+=symbolWord[i];
			}
			//Checks to see if the symbolword is entering/exiting a string. This is to handle a string like: "hellothere&&"; or '%'
			else if(symbolWord[i] == 34 || symbolWord[i] == 39)
			{
				result+=symbolWord[i];
				isString++;
			}
			//Section is not a string but IS an operand so it lumps the two symbols together and moves on
			else if((isOperand(symbolWord[i]) && isOperand(symbolWord[i+1])) && isString%2 == 0)
			{
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
	return result;
}

//Extracts basic comments from a string s that contain "//"
std::string Parsify::Parser::extractLineComment(std::string line)
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
std::string Parsify::Parser::extractBlockComment(std::string line)
{
	std::string result;
	std::string buffer;

	for(int i=0;i<line.length();i++)
	{
		//String is encountered before the comment block
		if((line[i] == 34 || line[i] == 39) && isComment%2 ==0)
		{
			buffer+=line[i];
			i++;
			result+=spaceifySymbolWord(buffer);
			buffer = "";
		}
		//Word block has encountered a comment beginning and up until now the buffer contains non commented words. The iscomment%2 is used here to ensure we dont have a line like /* foo/* bar*/
		else if((line[i] == '/' && line[i+1] == '*') && isComment%2 == 0 && isString%2 == 0)
		{
			//Add current word to result after separating out the symbolwords
			result+= spaceifySymbolWord(buffer);
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
			buffer="";

			//add the comment tail of */
			result+=line[i];
			i++;
			result+=line[i];
			i++;

			//ensure theres a gap between this and the next string.
			result+=" ";

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
	//If we're in a comment buffer will remain unmodified and added to result, else we spaceify it and add it to the result
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

//checks if the string s contains a special character (defined by anything that isnt an underscore OR alphanumeric)
bool Parsify::isSymbolWord(std::string s)
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
bool Parsify::isSymbol(char c)
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
bool Parsify::isOperand(char c)
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
bool Parsify::isLineComment(std::string s)
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
bool Parsify::isBlockComment(std::string s)
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
bool Parsify::newLineNeeded(std::string s)
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

//adds "P" before the . in a normal file (if there is a dot). If there isnt it will add it to the end of the filename
std::string Parsify::nameOutputFile(std::string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i] == '.')
		{
			s.insert(i,"P");
			return s;
		}
	}
	s+="P";
	return s;
}
