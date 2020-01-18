#ifndef _AST_H_
#define _AST_H_
#include"lexer.h"
namespace Yan{

enum ASTop{
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT,
    A_LVIDENT,
    A_ASSIGN,
    A_IDENTI

};
struct ASTnode
{
    ASTnode(int op, ASTnode* left, ASTnode* right,int intvalue)
    {
        this->op = op;
        this->intValue = intvalue;
        this->left = left;
        this->right = right;
    }
    int op;
    ASTnode*left;
    ASTnode*right;
    int intValue;
};


};
#endif
