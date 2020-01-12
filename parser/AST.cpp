#include "AST.h"
namespace Yan{

exprAST::exprAST(lexer& s):scan(s)
{
    
}
exprAST::~exprAST()
{
 
}
ASTnode* exprAST::primary(token& t)
{
    ASTnode* node =nullptr;
    if (t.token == T_INTLIT)
    {   std::cout<<t.value<<"value "<<std::endl;
        node = new ASTnode(tokenType2ASTop(t.token),nullptr, nullptr,t.value);
    }
    else
    {
        std::cout<<"ERROR:unexpected token"<<std::endl;
        exit(1);
    }
    return node;
}
ASTnode* exprAST::binExpr(token& t, int ptp)
{
    ASTnode* left = nullptr, *right = nullptr;
    int tokenType;
    if(t.token == T_EOF)
    {
        std::cout<<"end of the file";
        return nullptr;
    }

    left = primary(t);
    scan.scan(&currentToken);
    tokenType = currentToken.token;
    if(tokenType ==T_EOF)
    {
        return left;
    }
    
    while(getOpPriority(tokenType)>ptp)
    {
        int current_ptp = getOpPriority(tokenType);
        std::cout<<"current_ptp"<<current_ptp<<std::endl;
        scan.scan(&currentToken);
        right = binExpr(currentToken,current_ptp);
        left = new ASTnode(tokenType2ASTop(tokenType),left,right,0);
        tokenType = currentToken.token; 
        if (tokenType == T_EOF)
        {
            return left;
        }
    }
    return left;
    
       
}

int exprAST::getOpPriority(int  optype)
{
    if(op_priority[optype] == -1)
    {
        std::cout<<"ERROR:Invalid operation"<<std::endl;
        exit(1);
    }
    return op_priority[optype];
}
int exprAST::tokenType2ASTop(int tokenType)
{ 
    int op;

 switch (tokenType)
{
    case T_ADD:
        op = A_ADD;
        break;
    case T_STAR:
        op = A_MULTIPLY;
        break;
    case T_MINUS:
        op = A_SUBTRACT;
        break;
    case T_SLASH:
        op = A_DIVIDE;
        break;
    case T_INTLIT:
        op = A_INTLIT;
        break;

}

return op;
    
}
}
