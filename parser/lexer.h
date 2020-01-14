#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream> 
#include <iostream>
#include <string>
namespace Yan{

enum tokenType 
{
    T_ADD,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_INTLIT,
    T_EOF
};
std::string tokenToString(tokenType t);
struct token 
{
    int value;
    tokenType token;  
};
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
private:
  std::string fileName;
  int lineNum;
  std::ifstream infile;
};
}

#endif
