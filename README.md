# Underscorify
The most annoying way to submit any c++ code.

PARSER:
	The goal of this part of the project is to take in any c++ file and separate each symbol and word from each other whilst also destroying any sembalance of formatting
	that may have previously been there. This is so that the file will become easier to underscorify later on in the project. It will not modify comments though. As an 
	example of what it does is it will take a line, say int main()//test comment. and change that to: int main ( ) //test comment
	Also will not modify preprocessing directives.