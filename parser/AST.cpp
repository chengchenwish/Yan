#include "AST.h"

exprAST::exprAST(parser::parser& s):scan(s)
{
    
}
exprAST::~exprAST()
{
 
}
ASTnode* exprAST::primary(parser::token& t)
{
    ASTnode* node =nullptr;
    if (t.token == parser::T_INTLIT)
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
ASTnode* exprAST::binExpr(parser::token& t, int ptp)
{
    ASTnode* left = nullptr, *right = nullptr;
    int tokenType;
    if(t.token == parser::T_EOF)
    {
        std::cout<<"end of the file";
        return nullptr;
    }

    left = primary(t);
    scan.scan(&currentToken);
    tokenType = currentToken.token;
    if(tokenType == parser::T_EOF)
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
        if (tokenType == parser::T_EOF)
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
    case parser::T_ADD:
        op = A_ADD;
        break;
    case parser::T_STAR:
        op = A_MULTIPLY;
        break;
    case  parser::T_MINUS:
        op = A_SUBTRACT;
        break;
    case parser::T_SLASH:
        op = A_DIVIDE;
        break;
    case parser::T_INTLIT:
        op = A_INTLIT;
        break;

}

return op;
    
}
/*void exprAST::AddToTree(parser::token& t,)
{
    int op;
    switch (t.token)
    {
        case parser::T_ADD:
            op = A_ADD;
            break;
        case parser::T_STAR:
            op = A_MULTIPLY;
            break;
        case  parser::T_MINUS:
            op = A_SUBTRACT;
            break;
        case parser::T_SLASH:
            op = A_DIVIDE;
            break;
        case parser::T_INTLIT:
            op = A_INTLIT;
            break;

    }
    if(root == std::nullptr)
    {
        root = new ASTnode();
        //std::memset(root,0,sizeof(root));
        root->op = op;
        root->left = std::nullptr;
        root->right = std::nullptr;

    }

}*/
