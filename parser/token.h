#ifndef _TOKEN_H_
#define _TOKEN_H_
#include<string>
namespace Yan{

//max str len for one token
constexpr int MAX_STR_LEN = 100;
enum class tokenType 
{ 
    //The token is a meaningful string
    T_EOF,
    T_ASSIGN, //=
    T_ADD    ,//+
    T_MINUS ,//-
    T_STAR ,//*
    T_SLASH ,// /
    T_EQ, //==
    T_GT, //>
    T_LT, //<
    T_LQ, //<=
    T_GE, //>=
//type keyword
    T_VOID,// void
    T_CHAR,// char
    T_INT, // int
    T_LONG, // long

    T_INTLIT,
    T_PRINT,
    T_SEMI,
    T_IDENT,

    T_LBRACE,// {
    T_RBRACE,// }
    T_LPAREN, // (
    T_RPAREN,// )
    T_LBRACKET,
    T_RBRACKET
};

 struct token 
{   
    int value;
    tokenType type;
    std::string  tostring();
};
}

#endif
