#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

//Bash file was made and now its time for the next goal of taking in an *actual* :O C++ file. I figure a large problem Ill run into is when you have code that has a line like
//"while(this==3){"
//Which would be a problem because rn my program will bump into that and lump the whole thing as one word, which I dont want.
//I figure I have a few options and one is to put spaces inbetween each symbol but that may not work too because that line would then be
//while ( this = = 3 ) {
//which isnt going to work because i need to treat the == as a word of itself.
//I think for now for the sake of ""simplicity"" Ill allow myself to go through each word in a for loop and check if it contains ANY symbols and to just count how many symbolwords there are.
//So a test where my example occurs and that being the only one the program would output the whole file like normal here and that there were 1 (or more) symbolword(s)

//checks if the string s contains a special character defined by anything that isnt an underscore OR alphanumeric
bool isSymbolWord(std::string s)
{
	bool symbol=false;
	bool alphanumeric=false;

	for(int i=0;i<s.length();i++)
	{
		if( (s[i]>=48 && s[i] <=57) ||
			(s[i]>=65 && s[i] <=90) ||
			(s[i]>=97 && s[i]<=122))
			{
				symbol=true;
			}
		else
		{
			alphanumeric=true;
		}
	}
	if(symbol && alphanumeric)
	{
		std::cout<<"word: " << s << std::endl;
		return true;
	}
	return false;
}

int main(int argc, char *argv[])
{
	//Check if there is no filename being given
	if(argc < 2)
	{
		std::cout<<"Error, use is ./Underscorify [filename]"<<std::endl;
		return 1;
	}

	//Opens each file and outputs the words on a new line
	for(int i=1;i<argc;i++)
	{
		std::ifstream inputFile;
		inputFile.open(argv[i]);

		std::string word;

		int symbolWords=0;

		while(inputFile >> word)
		{
			std::cout<<word<<std::endl;
			if(isSymbolWord(word))
			{
				symbolWords++;
			}
		}
		std::cout<<"number of symbolwords was: " << symbolWords << std::endl;
		inputFile.close();
	}
	
	return 0;
	
}