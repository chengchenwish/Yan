#ifndef _TOKEN_H_
#define _TOKEN_H_
#include<string>
enum class tokenType 
{ 
    //The token is a meaningful string
    T_ADD    = 0,
    T_MINUS  = 1,
    T_STAR   = 2,
    T_SLASH  = 3,
    T_INTLIT = 4,
    T_EOF    =5
};
std::string tokenToString(tokenType t);
struct token 
{
    int value;
    tokenType token;  
};

#endif
