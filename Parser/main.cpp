#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

//So now currently this program takes in multiple files to be underscorified in the command line
//and outputs using stdout
//I think next before I go further I would like to create a bash file to assist in testing this.
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

		while(inputFile >> word)
		{
			std::cout<<word<<std::endl;
		}
		
		inputFile.close();
	}
	
	return 0;
	
}