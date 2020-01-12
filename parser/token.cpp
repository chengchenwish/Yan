#include "token.h"
std::string okenToString(tokenType t)
{
    switch (t)
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
        default:
            return "unknow";
    }
}
