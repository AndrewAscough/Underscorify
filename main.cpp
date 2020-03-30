#include "HashTable/hash.h"
#include "Parser/Parser.h"

int main(int argc, char *argv[])
{
	Hashify::hashTable h1;
	Parsify::Parser p1(argc, argv);
	p1.Parse();

	return 0;
}