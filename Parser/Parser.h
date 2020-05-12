#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

namespace Parsify {
	class Parser {
		public:
			Parser(int argc, char *argv[]);
			//main driving function, uses the string passed in from when it was initialised and parses the file by generating a new file with a P after the name
			void Parse();
			//Just removes all the parsed version of the files it made and dumped around the place.
			void cleanup();
		private:
			//Stores list of filenames to be parsed
			std::vector<std::string> files;
			//Determins if input is in a comment block or not. if isComment%2==0, it is not a comment
			int isComment;
			//Determines if input is currently interacting with a string. if isString%2==0, it is not a string
			int isString;	
			//Takes in a symbolword string and returns a nonsymbolword by separating the symbols with spaces. Also handles string opening and closings without modifying the string by checking isString.
			std::string spaceifySymbolWord(std::string symbolWord);
			//Extracts basic comments from a string s that contain "//"
			std::string extractLineComment(std::string line);
			//Extracts complex comments where a line could contain multiple /* */ comments
			std::string extractBlockComment(std::string line);
	};

	//checks if the string s contains a special character (defined by anything that isnt an underscore OR alphanumeric)
	bool isSymbolWord(std::string s);
	//if the character is a symbol the function returns true.
	bool isSymbol(char c);
	//Checks if a character is an opperator which are: =, <, >, +, -, /, *, ^, !, %, :, |, &
	bool isOperand(char c);
	//Searches the string to see if the word is a comment
	bool isLineComment(std::string s);
	//Searches the string to see if the word contains /* or */
	bool isBlockComment(std::string s);
	//searches the string if it has a ';' character it'll want to add a newline after it.
	bool newLineNeeded(std::string s);
	//adds "P" before the . in a normal file (if there is a dot). If there isnt it will add it to the end of the filename
	std::string nameOutputFile(std::string s);
};

#endif //PARSER_H
