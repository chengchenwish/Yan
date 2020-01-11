#include "parser/parser.h"
#include "parser/AST.h"
#include "parser/interp.h"
int main()
{
    std::cout<<" input file name:";
    std::string file;
    std::cin>> file;
    parser::parser p(file);
    parser::token t;
    if(p.openFile())
    {
        while(p.scan(&t))
        {
            std::cout<<parser::tokenToString(t.token)<<" "<<t.value<<std::endl;
            if(t.token == parser::T_EOF)
            {
                break;
            }
        }

    }
    else
    {
        std::cout<<"open file fail"<<std::endl;

    }
    parser::parser pp(file);
    if(pp.openFile())
    {
        ASTnode* node;

        parser::token ttt;
        pp.scan(&ttt);
        exprAST ast(pp);
        node = ast.binExpr(ttt);
        std::cout<<"node type:"<<node->op<<std::endl;
    
        int result = interpExpAST(node);
        std::cout<<"result is:"<<result<<std::endl;
    }
    return 0;
}
