#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream> 
#include <iostream>
#include <string>
#include <queue>
#include "token.h"
#include "error.h"
namespace Yan{


class lexer 
{
public:
   explicit lexer(std::string& filename);
   ~lexer();  
   bool scan(Token* t);
   void putBack(const Token& t);
   Token getToken();

   int peek();
   location& getLocation(){ return loc;}
private:
   bool next(char& c);
   bool skip( char& c);
   void consume();
   bool isdigit(char c);
   bool isalpha(char c);
   bool isOperator(char c);
   bool scanInt(char c, Token*t);
   void scanIdenti(char c, Token*t);
private:
   std::queue<Token> tokenCache_;
   location loc;
  //std::string fileName;
  //int lineNum;
  std::ifstream infile;
};
}

#endif
