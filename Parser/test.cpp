#include "Parser.h"

int main(int argc, char* argv[])
{
	Parsify::Parser p1(argc, argv);
	p1.Parse();

	return 0;
}