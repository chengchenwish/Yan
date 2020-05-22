#include "token.h"
#include <map>
namespace Yan
{

std::string Token::tostring()
{


    #define xx(T,Text)\
        case TokenType::T: return Text;
    switch(type)
    {
        RESERVED(xx)
        KEYWORD(xx)
        case TokenType::T_EOF: return "EOF";
        case TokenType::T_IDENT: return "identi";
        case TokenType::T_INTLIT: return "intlit";
        default:
        return "unkown";
    }
    #undef xx

}



}