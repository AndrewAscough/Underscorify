#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
//Determines if input is currently interacting with a string or not
int isString   =   0 ;  
//checks if the string s contains a special character (defined by anything that isnt an underscore OR alphanumeric) AND an alphanumeric character
bool  isSymbolWord ( std :: string  s )    {  bool  symbol = false   ;  
 for ( int  i = 0 ; i < s . length (  )  ; i ++  )  
  {   if (    ( s [ i ]  >= 48   &&   s [ i ]  <= 57   )    ||    ( s [ i ]  >= 65   &&   s [ i ]    <= 90 )    ||    ( s [ i ]  >= 97   &&   s [ i ]  <= 122 )  )    {   continue ;  
  }  else   {   symbol = true ;  
  }    }   if ( symbol )    {  return  true ;  
  }  return  false ;  
  }  //if the character is a symbol the function returns true.
bool  isSymbol ( char  c )    {   if (    ( c >= 48   &&   c <= 57   )    ||    ( c >= 65   &&   c <= 90 )    ||    ( c >= 97   &&   c <= 122 )  )    {  return  false ;  
  }  else   {  return  true ;  
  }  return  false ;  
  }  //Checks if a character is an opperator which are: =, <, >, +, -, /, *, ^, !, %, :, |, &
bool  isOperand ( char  c )    {   if (  c   ==   '='   ||  c   ==   '<'   ||  c   ==   '>'   ||  c   ==   '+'   ||  c   ==   '-'   ||  c   ==   '/'   ||  c   ==   '*'   ||  c   ==   '^'   ||  c   ==   '!'   ||  c   ==   '%'   ||  c   ==   ':'   ||  c   ==   '|'   ||  c   ==   '&' )    {  return  true ;  
  }  return  false ;  
  }  //searches the string if it has a ';' character it'll want to add a newline after it.
bool  newLineNeeded ( std :: string  s )    {   for ( int  i = 0 ; i < s . length (  )  ; i ++  )  
  {   if ( s [ i ]    ==   ';' )  
  {  return  true ;  
  }    }  return  false ;  
  }  //Takes in a symbolword string and returns a nonsymbolword by separating the symbols with spaces. Also handles string opening and closings without modifying the string by checking isString.
 std :: string  spaceifySymbolWord ( std :: string  symbolWord )    {   std :: string result   =   "  " ;  
 for ( int  i = 0 ; i < symbolWord . length (  )  ; i ++  )  
  {  //So the encountered part of the string is in fact a symbol.
 if ( isSymbol ( symbolWord [ i ]  )  )    {  //Checks to see if the symbolword is entering/exiting a string. This is to handle a string like: "hellothere&&"; or '%' currently within one
 if ( symbolWord [ i ]    ==  34   ||   symbolWord [ i ]    ==   39 )    {  //Entering the string
 if ( isString % 2 == 0 )    {   result += symbolWord [ i ]  ;  
  }  //Exiting the string
else   {   result += symbolWord [ i ]  ;  
  }   isString ++  ;  
  }  //Section is not a string but IS an operand so it lumps the two symbols together and moves on
else  if (  ( isOperand ( symbolWord [ i ]  )    &&   isOperand ( symbolWord [ i + 1 ]  )  )    &&   isString % 2   ==   0 )    {   result += "  " ;  
 result += symbolWord [ i ]  ;  
 result += symbolWord [ i + 1 ]  ;  
 result += "  " ;  
 i ++  ;  
  }  //Section is not an operand, but it is in a string so it does nothing.
else  if ( isString % 2   ==   1 )    {   result += symbolWord [ i ]  ;  
  }  //Section is not an operand, nor is it in a string so it splits the symbol away.
else   {   result += "  " ;  
 result += symbolWord [ i ]  ;  
 result += "  " ;  
  }    }  //Encountered part of string is just a normal alphanumeric char so do nothing.
else   {   result += symbolWord [ i ]  ;  
  }    }   result += "  " ;  
return  result ;  
  }  int  main ( int  argc ,  char   * argv [  ]  )  //this will break it also there should be 8 spaces between this        this.
  {  //Check if there is no filename being given
 if ( argc   <   2 )    {   std :: cout << "Error, use is  ./Underscorify  [filename]" << std :: endl ;  
return  1 ;  
  }  //Opens each file
 for ( int  i = 1 ; i < argc ; i ++  )  
  {   std :: ifstream  inputFile ;  
 inputFile . open ( argv [ i ]  )  ;  
 std :: string  line ;  
//Prints each word of the file on a new line
 while ( std :: getline ( inputFile , line )  )    {   std :: string  word ;  
 std :: istringstream  currentLine ( line )  ;  
 while ( currentLine   >>   word )    {  //Preprocessor directive thingy, essentially not something I want to mess with. Think #include <iostream>
 if ( word [ 0 ]    ==   '#' )    {   std :: cout << word << "  " ;  
 currentLine >> word ;  
 std :: cout << word << std :: endl ;  
  }  //Check if the REST of the line is a // comment
else  if ( word [ 0 ]    ==   '/'   &&   word [ 1 ]    ==   '/' )    {   break ;  
  }  //for some resemblance of "formatting"
else  if ( newLineNeeded ( word )  )    {   std :: cout << spaceifySymbolWord ( word )  << std :: endl ;  
  }  //Handles breaking up symbol words and passing through strings without changing the interior.
else  if ( isSymbolWord ( word )  )    {   std :: cout << spaceifySymbolWord ( word )  ;  
  }  //Word is not a symbol word, it can remain unchanged.
else   {   std :: cout << word << "  " ;  
  }    }    }   inputFile . close (  )  ;  
  }  return  0 ;  
  }  