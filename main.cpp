#include "HashTable/hash.h"
#include "Parser/Parser.h"
#include <boost/filesystem.hpp>

int isComment = 0;

//Generates a string n underscores long
std::string generateUnderscoreWord(int n)
{
	std::string word ="";

	for(int i=0;i<=n;i++)
	{
		word+="_";
	}
	return word;
}

//Takes in a string s and adds a 'U' before the . or just a U to the end
std::string nameOutputFile(std::string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i] == '.')
		{
			s.insert(i,"U");
			return s;
		}
	}
	s+="U";
	return s;
}

//takes in a string with a file path like tests/mydirectory/testfile.cpp and returns testfile.cpp
std::string removePath(std::string s)
{
	std::string result ="";
	std::string character ="";
	for(int i=s.length()-1;i>=0;i--)
	{
		if(s[i] == '/')
		{
			break;
		}
		character = s[i];
		result.insert(0,character);
	}
	return result;
}

//Takes in a string s and adds a 'U' before the . or just a U to the end
std::string nameOutputHeaderFile(std::string s)
{
	std::string result;

	for(int i=0;i<s.length();i++)
	{
		if(s[i] == '.')
		{
			break;
		}
		result+=s[i];
	}
	result+="U.h";
	return result;
}

bool containsString(std::string line)
{
	for(int i=0;i<line.length();i++)
	{
		if(line[i] == 34)
		{
			return true;
		}
	}
	return false;
}

//Extracts the string from the line passed in then returns the first contained string. removeString() should always be called after this function to recieve the remainder string
std::string extractString(std::string line)
{
	int isString = 0;
	std::string contained ="";

	for(int i=0;i<line.length();i++)
	{
		if(line[i] == 34)
		{
			contained += line[i];
			i++;
			while(line[i]!=34)
			{
				if(line[i] == '\\')
				{
					contained += line[i];
					i++;
				}
				contained+=line[i];
				i++;
			}
			contained += line[i];
			i++;
			return contained;
		}
	}
	return "I HSOULDNT HAVE REACHED THIS POINT OF CODE";
}

//Removes a string from a line.
std::string removeString(std::string line)
{
	std::string result ="";
	bool isRemoved = false;

	for(int i=0;i<line.length();i++)
	{
		if(line[i] == 34 && !isRemoved)
		{
			i++;
			while(line[i]!=34)
			{
				if(line[i] == '\\')
				{
					i++;
				}
				i++;
			}
			i++;
			isRemoved = true;
		}
		else
		{
			result+=line[i];
		}
	}
	return result;
}

//Takes in the line where the string was found, as well as the order for the corresponding string so it can replace it with underscores
std::string replaceString(std::string line, int order)
{
	std::string result ="";
	bool isRemoved = false;


	for(int i=0;i<line.length();i++)
	{
		if(line[i] == 34 && !isRemoved)
		{
			result+=generateUnderscoreWord(order);
			i++;
			while(line[i]!=34)
			{
				if(line[i] == '\\')
				{
					i++;
				}
				i++;
			}
			i++;
			result+=line[i];

			isRemoved = true;

		}
		else
		{
			result += line[i];
		}
	}
	return result;
}

//Removes all the comments from a given line
std::string removeComments(std::string line)
{
	std::string result ="";
	int isString =0;

	for(int i=0;i<line.length();i++)
	{
		//line is entering a string
		if(line[i] == 34 && line[i-1] != 92 && isComment%2 == 0)
		{
			result += line[i];
			isString++;
		}
		//line is entering to a comment that isnt in a string
		else if(((line[i] =='/' && line[i+1] == '*') || (line[i] =='*' && line[i+1] == '/')) && isString%2 == 0)
		{
			isComment++;
			i++;
		}
		//line is entering a line comment meaning the rest of the line is done and we can just exit here
		else if(line[i] =='/' && line[i+1] == '/' && isString%2 ==0)
		{
			return result;
		}
		//Line is not in a comment so whatever is encountered can be added to result.
		else if(isComment%2 == 0)
		{
			result+=line[i];
		}
	}
	return result;
}

//Returns the path to the file without the filename.
std::string extractFilePath(std::string s)
{
	for(int i=s.length()-1;i>=0;i--)
	{
		if(s[i] == '/')
		{
			return s;
		}
		s.erase(s.begin()+i);
	}
	return "";
}

std::string underscoresIncludePath(std::string fileName)
{
	int steps =0;
	std::string filePath ="#include \"";
	for(int i=1;i<fileName.length();i++)
	{
		if(fileName[i] == '/')
		{
			filePath+="../";
		}
	}
	filePath+="Underscores.h\"";
	return filePath;
}

//Generates the directory structure for the output to be the same as the files input.
void generateOutputDirectories(std::vector<std::string> fileNames)
{
	boost::filesystem::create_directory("Underscorified");
	for(int i=0;i<fileNames.size();i++)
	{
		std::string filePath = "Underscorified/";
		filePath += extractFilePath(fileNames.at(i));
		boost::filesystem::create_directories(filePath);
	}
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cerr<<"ERROR, USE IS ./Underscorify [filenames]"<<std::endl;
		return 1;
	}

	std::vector<std::string> fileNames;
	for(int i=1;i<argc;i++)
	{
		std::string s(argv[i]);
		fileNames.push_back(s);
	}

	generateOutputDirectories(fileNames);
	//Output folder

	Parsify::Parser p1(argc, argv);
	p1.Parse();
	Hashify::hashTable h1;

	//Opens each of the parsed files
	for(int i=1;i<argc;i++)
	{
		//Open parsed input file.
		std::ifstream inputFile;
		std::string inputFileName(argv[i]);
		inputFile.open(Parsify::nameOutputFile(inputFileName).c_str());
		
		//Generate output file for code
		std::ofstream outputFile;
		std::string outFileName = "Underscorified/";
		outFileName+=nameOutputFile(inputFileName);

		outputFile.open(outFileName.c_str());
		outputFile<<underscoresIncludePath(inputFileName)<<std::endl;
		
		std::string line;

		//Generates the hashtable by adding each word to the hashtable. Lumps strings together and stores as one entry
		while(getline(inputFile,line))
		{
			//First, remove all the comments from line so we can ignore adding them to the hashtable as they do not need to be changed. 
			line = removeComments(line);
			//Preprocessor directive thingy, essentially not something I want to mess with. Think #include <iostream>
			if(line[0] == '#' && isComment%2 ==0)
			{
				line="";
			}
			//If the line contains a string (or multiple) we extract them and put those extracted strings into the hashtable.
			else if(containsString(line))
			{
				//repeat until there are no strings left in line
				while(containsString(line))
				{
					//extract the string from the line, add it to hashtable.
					h1.addEntry(extractString(line));
					//remove the string
					line=removeString(line);
				}
			}

			//Lastly adds remaining words to table
			std::istringstream currentLine(line);
			std::string word;
			while(currentLine >> word)
			{
					h1.addEntry(word);
			}
		}

		//reset ifstream var
		inputFile.clear();
		inputFile.seekg(0,std::ios::beg);

		//Generates the file with underscores as all of the variable names
		while(getline(inputFile,line))
		{
			//Replaces the string (if present) in the line with the underscorified version
			if(containsString(line) && line[0] != '#')
			{
				while(containsString(line))
				{
					line = replaceString(line,h1.getOrder(extractString(line)));
				}
			}

			std::istringstream currentLine(line);
			std::string word;

			//To detect when the program enters a line comment and needs to NOT underscorify the words
			bool lineComment = false;

			while(currentLine >> word)
			{
				//To enter a block comment we need the start of the word = /* and to not already be in a block comment and to not already be in a line comment
				if((word[0] == '/' && word[1] =='*') && isComment%2 == 0 && !lineComment)
				{
					isComment++;
				}
				else if(Parsify::isLineComment(word) && isComment%2 == 0)
				{
					lineComment = true;
				}

				//Word exists in the hashtable
				if(h1.getOrder(word)!=-1 && !lineComment && isComment%2 == 0)
				{
					outputFile << generateUnderscoreWord(h1.getOrder(word)) << " ";
				}
				//Word doesnt exist meaning in table in current form meaning it must be a comment or a string thats already been replaced.
				else
				{
					outputFile << word << " ";
				}

				//if the word also contains an ending character for the comment we need to increment iscomment
				if((word[word.length()-2] == '*' && word[word.length()-1] == '/') && !lineComment)
				{
					isComment++;
				}
			}
			outputFile<<std::endl;	
		}
		outputFile.close();
		inputFile.close();		
	}

	//Generates output file for #defines
	std::ofstream outputHeaderFile;
	outputHeaderFile.open("Underscorified/Underscores.h");

	while(true)
	{
		Hashify::item i = h1.popEntry();
		//Hashtable has no more words to pop
		if(i.word=="")
		{
			break;
		}
		outputHeaderFile << "#define " << generateUnderscoreWord(i.order) << " " << i.word << std::endl;
	}

	return 0;
}