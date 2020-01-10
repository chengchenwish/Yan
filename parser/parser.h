#ifndef _PARSER_H_
#define _PARSER_H_
#include <fstream> 
#include <iostream>
#include <string>
namespace parser {

enum tokenType 
{
    T_ADD,
    T_MINUS,
    T_STAR,
    T_SLASH,
    T_NUMBER
};
std::string tokenToString(tokenType t);
struct token 
{
    int value;
    tokenType token;  
};

class parser 
{
public:
   explicit parser(std::string& filename);
   ~parser();
   bool next(char& c);
   bool skip( char& c);
   bool scan(token* t);
   int peek();
   bool openFile();
   bool isdigit(char c);
   int getLineNum(){ return lineNum;}
private:
  std::string fileName;
  int lineNum;
  std::ifstream infile;
};
}

#endif
