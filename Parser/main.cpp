#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

//Curently this prompts the person on running the program to input a file name, then prints each word onto a new line
//Next goal is to pass in the file name thru the running of the program. Ie ./test test1.txt
int main()
{
	std::string fileName;
	std::cin>>fileName;
	
	std::ifstream inputFile;
	inputFile.open(fileName.c_str());
	

	while(inputFile >> word)
	{
		std::cout<<word<<std::endl;
	}
	
	inputFile.close();

	return 0;
}