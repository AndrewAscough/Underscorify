#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
int isString   =   0 ;  
bool  isSymbolWord ( std :: string  s )    {  bool  symbol = false   ;  
 for ( int  i = 0 ; i < s . length (  )  
  ; i ++  )  
  {   if (    ( s [ i ]  >= 48   &&   s [ i ]  <= 57   )    ||    ( s [ i ]  >= 65   &&   s [ i ]    <= 90 )    ||    ( s [ i ]  >= 97   &&   s [ i ]  <= 122 )  )    {   continue ;  
  }  else   {   symbol = true ;  
  }    }   if ( symbol )    {  return  true ;  
  }  return  false ;  
  }  bool  isSymbol ( char  c )    {   if (    ( c >= 48   &&   c <= 57   )    ||    ( c >= 65   &&   c <= 90 )    ||    ( c >= 97   &&   c <= 122 )  )    {  return  false ;  
  }  else   {  return  true ;  
  }  return  false ;  
  }  bool  isOperand ( char  c )    {   if (  c   ==   '='   ||  c   ==   '<'   ||  c   ==   '>'   ||  c   ==   '+'   ||  c   ==   '-'   ||  c   ==   '/'   ||  c   ==   '*'   ||  c   ==   '^'   ||  c   ==   '!'   ||  c   ==   '%'   ||  c   ==   ':'   ||  c   ==   '|'   ||  c   ==   '&' )    {  return  true ;  
  }  return  false ;  
  }  bool  newLineNeeded ( std :: string  s )    {   for ( int  i = 0 ; i < s . length (  )  ; i ++  )  
  {   if ( s [ i ]    ==   ';' )  
  {  return  true ;  
  }    }  return  false ;  
  }   std :: string  spaceifySymbolWord ( std :: string  symbolWord )    {   std :: string result   =   "  " ;  
 for ( int  i = 0 ; i < symbolWord . length (  )  ; i ++  )  
  {   if ( isSymbol ( symbolWord [ i ]  )  )    {   if ( symbolWord [ i ]    ==  34   ||   symbolWord [ i ]    ==   39 )    {   if ( isString % 2 == 0 )    {   result += symbolWord [ i ]  ;  
  }  else   {   result += symbolWord [ i ]  ;  
  }   isString ++  ;  
  }  else  if (  ( isOperand ( symbolWord [ i ]  )    &&   isOperand ( symbolWord [ i + 1 ]  )  )    &&   isString % 2   ==   0 )    {   result += "  " ;  
 result += symbolWord [ i ]  ;  
 result += symbolWord [ i + 1 ]  ;  
 result += "  " ;  
 i ++  ;  
  }  else  if ( isString % 2   ==   1 )    {   result += symbolWord [ i ]  ;  
  }  else   {   result += "  " ;  
 result += symbolWord [ i ]  ;  
 result += "  " ;  
  }    }  else   {   result += symbolWord [ i ]  ;  
  }    }  result   +=   "  " ;  
return  result ;  
  }  int  main ( int  argc ,  char   * argv [  ]  )    {   if ( argc   <   2 )    {   std :: cout << "Error, use is  ./Underscorify  [filename]" << std :: endl ;  
return  1 ;  
  }   for ( int  i = 1 ; i < argc ; i ++  )  
  {   std :: ifstream  inputFile ;  
 inputFile . open ( argv [ i ]  )  ;  
 std :: string  word ;  
 while ( inputFile   >>   word )    {   if ( word [ 0 ]    ==   '#' )    {   std :: cout << word << "  " ;  
 inputFile >> word ;  
 std :: cout << word << std :: endl ;  
  }  else  if ( newLineNeeded ( word )  )    {   std :: cout << spaceifySymbolWord ( word )  << std :: endl ;  
  }  else  if ( isSymbolWord ( word )  )    {   std :: cout << spaceifySymbolWord ( word )  ;  
  }  else   {   std :: cout << word << "  " ;  
  }    }   inputFile . close (  )  ;  
  }  return  0 ;  
  }  