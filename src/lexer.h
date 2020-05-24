#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream> 
#include <iostream>
#include <string>
#include <queue>
#include "error.h"
namespace Yan{


class lexer 
{
public:
   explicit lexer(std::string& filename);
   ~lexer();  
   void scan(Token* t);
   void putBack(const Token& t);
   Token getToken();
   Token peektoken();

   
   location& getLocation(){ return loc;}
private:
   int next();
   int peek();
   void skipcomment();
   void consume(char c);
   bool isdigit(char c);
   bool isalpha(char c);
   bool ispunct(char c);
   bool keyword(char* s, Token*t);

   void scanpunct(char c,Token*t);
   void scanInt(char c, Token*t);
   void scanIdenti(char c, Token*t);
   int  scanchar();
   std::string scanstr();
private:
   std::queue<Token> tokenCache_;
   location loc;
  //std::string fileName;
  //int lineNum;
  std::ifstream infile;
};
}

#endif
