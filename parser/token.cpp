#include "token.h"
#include <map>
namespace Yan
{

std::string Token::tostring()
{
     switch (type)
    {
        case TokenType::T_ADD:
            return "+";
        case TokenType::T_STAR:
            return "*";
        case TokenType::T_MINUS:
            return "-";
        case TokenType::T_SLASH:
            return "/";
        case TokenType::T_INTLIT :
            return "number";
        case TokenType::T_EOF:
            return "eof";
        case TokenType::T_PRINT:
            return "print";
        case TokenType::T_SEMI:
            return ";";
        case TokenType::T_INT:
            return "int";
        case TokenType::T_EQ:
            return "==";
        case TokenType::T_IDENT:
            return "identi";
        case TokenType::T_ASSIGN:
            return "=";
        case TokenType::T_LT:
            return "<";
         case TokenType::T_GT:
            return ">";
        case TokenType::T_LE:
            return "<=";
        case TokenType::T_GE:
            return ">=";
        case TokenType::T_NE:
            return "!=";
        case TokenType::T_IF: return "if";
        case TokenType::T_ELSE: return "else";
        case TokenType::T_LBRACE:return "{";
        case TokenType::T_RBRACE: return "}";
        case TokenType::T_LPAREN:return "(";
        case TokenType::T_RPAREN: return ")";

        default:
            return "unknow value";
    }

}

}