#ifndef _AST_H_
#define _AST_H_
#include"lexer.h"
namespace Yan{

enum ASTop{
    A_ADD,
    A_SUBTRACT,
    A_MULTIPLY,
    A_DIVIDE,
    A_INTLIT

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
// + - * / intlitr EOF
static constexpr int op_priority[] = {0, 0, 1, 1, -1, -1};

class exprAST 
{
    public:
        exprAST(lexer& s);
        ~exprAST();
        ASTnode*primary(token& t);
        ASTnode*binExpr(token& t, int ptp = -1);
        int tokenType2ASTop(int tokenType);
        int getOpPriority(int optype);
        //void buildTree(lexer::token& t);
    private:
//        ASTnode* root;
        //lexer::token currentToken;
        lexer& scan;
        token currentToken;
};
};
#endif
