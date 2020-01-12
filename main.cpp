#include "parser/lexer.h"
#include "parser/AST.h"
#include "parser/interp.h"
//namespace Yan{

int main()
{
    std::cout<<" input file name:";
    std::string file;
    std::cin>> file;
    Yan::lexer p(file);
    Yan::token t;
    if(p.openFile())
    {
        while(p.scan(&t))
        {
            std::cout<<Yan::tokenToString(t.token)<<" "<<t.value<<std::endl;
            if(t.token == Yan::T_EOF)
            {
                break;
            }
        }

    }
    else
    {
        std::cout<<"open file fail"<<std::endl;

    }
    Yan::lexer pp(file);
    if(pp.openFile())
    {
        Yan::ASTnode* node;

        Yan::token ttt;
        pp.scan(&ttt);
        Yan::exprAST ast(pp);
        node = ast.binExpr(ttt);
        std::cout<<"node type:"<<node->op<<std::endl;
    
        int result = Yan::interpExpAST(node);
        std::cout<<"result is:"<<result<<std::endl;
    }
    return 0;
}

