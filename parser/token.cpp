#include "token.h"
#include <map>
namespace Yan
{

std::string token::tostring()
{
     switch (type)
    {
        case tokenType::T_ADD:
            return "+";
        case tokenType::T_STAR:
            return "*";
        case tokenType::T_MINUS:
            return "-";
        case tokenType::T_SLASH:
            return "/";
        case tokenType::T_INTLIT :
            return "number";
        case tokenType::T_EOF:
            return "eof";
        case tokenType::T_PRINT:
            return "print";
        case tokenType::T_SEMI:
            return ";";
        case tokenType::T_INT:
            return "int";
        case tokenType::T_EQ:
            return "=";
        case tokenType::T_IDENT:
            return "identi";
        default:
            return "unknow value";
    }

}

}