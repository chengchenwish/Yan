#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream> 
#include <iostream>
#include <string>
#include "token.h"
#include "error.h"
namespace Yan{


class lexer 
{
public:
   explicit lexer(std::string& filename);
   ~lexer();  
   bool scan(token* t);
   int peek();
   bool openFile();  
   int getLineNum(){ return lineNum;}
private:
   bool next(char& c);
   bool skip( char& c);
   bool isdigit(char c);
   bool isalpha(char c);
   bool scanInt(char c, token*t);
   void scanIdenti(char c, token*t);
private:
  std::string fileName;
  int lineNum;
  std::ifstream infile;
};
}

#endif
