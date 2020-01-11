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
        node = new ASTnode(tokenType2ASTop(t.token),t.value);
    }
    else
    {
        std::cout<<"ERROR:unexpected token"<<std::endl;
        exit(1);
    }
    return node;
}
ASTnode* exprAST::binExpr(parser::token& t)
{
    ASTnode* left = nullptr, *right = nullptr, *root= nullptr;
    int nodeType;
    if(t.token == parser::T_EOF)
    {
        std::cout<<"end of the file";
        return nullptr;
    }
    left = primary(t);
    parser::token nextToken;
    scan.scan(&nextToken);
    if(nextToken.token == parser::T_EOF)
    {
        return left;
    }
    nodeType = tokenType2ASTop(nextToken.token);
     
    scan.scan(&nextToken);
    right = binExpr(nextToken);
    root = new ASTnode(nodeType,0);
    root->left = left;
    root->right = right;
    return root;
    
       
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
