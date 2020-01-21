#ifndef _AST_H_
#define _AST_H_
#include<memory>
#include"lexer.h"
namespace Yan{
class Ivisitor;
struct  Node
{
    Node(){};
    virtual ~Node(){};
};

struct  BinaryOp: public Node
{
    enum ASTop
    {   A_ADD,
        A_SUBTRACT,
        A_MULTIPLY,
        A_DIVIDE,
        A_INTLIT,
        A_LVIDENT,
        A_ASSIGN,
        A_IDENTI,
        A_EQ,
        A_NE,
        A_GT,
        A_LT,
        A_GE,
        A_LE
    };
    
    virtual void accept(Ivisitor*v);
    BinaryOp(int op, BinaryOp*left, BinaryOp* right, int value);
    

    int op;
    BinaryOp*  left;
    BinaryOp*  right;
    int intValue;
};
enum ASTop
    {   A_ADD,
        A_SUBTRACT,
        A_MULTIPLY,
        A_DIVIDE,
        A_INTLIT,
        A_LVIDENT,
        A_ASSIGN,
        A_IDENTI,
        A_EQ,
        A_NE,
        A_GT,
        A_LT,
        A_GE,
        A_LE
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

class Ivisitor 
{
public:
   virtual void visit(BinaryOp* node) = 0;
};


};
#endif
