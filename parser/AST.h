#ifndef _AST_H_
#define _AST_H_
#include"parser.h"

enum ASTop{
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT

};
struct ASTnode
{
    ASTnode(int op, int intvalue )
    {
        this->op = op;
        this->intValue = intvalue;
        this->left = nullptr;
        this->right = nullptr;
    }
    int op;
    ASTnode*left;
    ASTnode*right;
    int intValue;
};
class exprAST 
{
    public:
        exprAST(parser::parser& s);
        ~exprAST();
        ASTnode*primary(parser::token& t);
        ASTnode*binExpr(parser::token& t);
        int tokenType2ASTop(int tokenType);
        //void buildTree(parser::token& t);
    private:
//        ASTnode* root;
        //parser::token currentToken;
        parser::parser& scan;
};
#endif
