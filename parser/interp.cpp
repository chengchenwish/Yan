#include "interp.h"
namespace Yan{
int  interpExpAST(ASTnode* root)
{

    if(root == nullptr)
    {
        return 0;
    }
    
    int leftValue, rightValue;
    leftValue = interpExpAST(root->left);
    rightValue = interpExpAST(root->right);
    
    switch(root->op)
    {   
        case ASTop::A_ADD:
    
            return (leftValue+rightValue);
        case ASTop::A_DIVIDE:
            return (leftValue/rightValue);
        case ASTop::A_MULTIPLY:
            return(leftValue*rightValue);
        case ASTop::A_SUBTRACT:
            return(leftValue-rightValue);
        case ASTop::A_INTLIT:
            return root->intValue;
        default:
            std::cout<<" ERROR op"<<root->op<<std::endl;
            exit(1);
    }
}
}
