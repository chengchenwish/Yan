#ifndef _TOKEN_H_
#define _TOKEN_H_
#include<string>
namespace Yan{

//max str len for one token

constexpr int MAX_STR_LEN = 100;
/*
#define TOKEN_MAP(xx) \
        xx(T_EOF,'eof')\
        xx(T_ASSGIN, '=')\
        xx(T_ASPLUS,'+=')\
*/
/*
std::string getstring(Token t)
{

    #define c(Type,Text)\
        case Type: return #Text;
    switch(t)
    {
        TOKEN_MAP(c)
    }
    #undef c

}
*/

enum class TokenType 
{ 
    //The token is a meaningful string
    T_EOF,
    //op
    T_ASSIGN, //=
    T_ASPLUS,// +=
    T_ASMINUS,//-=
    T_ASSTAR,//*=
    T_ASSLASH,// /=
    T_ASMOD,// %=
    T_QUSTION,//?
    T_ADD    ,//+
    T_MINUS ,//-
    T_STAR ,//*
    T_SLASH ,// /
    T_MOD,// %
    T_EQ, //==
    T_GT, //>
    T_LT, //<
    T_LE, //<=
    T_GE, //>=
    T_NE, // !=
    T_OR,// |
   // T_
    T_LOGAND,// &&
    T_LOGOR,// ||

//type keyword
    T_VOID,// void
    T_CHAR,// char
    T_INT, // int
    T_LONG, // long

    T_INTLIT,
    T_PRINT,
    
    T_IDENT,
//KEY WORD
    T_IF,
    T_ELSE,

    T_LBRACE,// {
    T_RBRACE,// }
    T_LPAREN, // (
    T_RPAREN,// )
    T_LBRACKET, //[
    T_RBRACKET,//]
    T_COMMA, // ,
    T_SEMI //;
};
struct location
{
    std::string fileName;
    int line;
    int colum;
    location(const std::string& file, int line, int colum)
    {
        this->fileName = file;
        this->line = line;
        this->colum = colum;
    }
    
};
 struct Token 
{   
    int value;//store 
    std::string text;//store identi name;
    TokenType type;
    //location loc;
    std::string  tostring();
};
}

#endif
