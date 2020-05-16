#include "parser/lexer.h"
#include "parser/AST.h"
#include "parser/interp.h"
#include "parser/gen.h"
#include "parser/parser.h"
//namespace Yan{

int main(int argc,char*argv[])
{
    if(argc != 2)
    {

    }
    std::cout<<" input file name:";
    std::string file;
    std::cin>> file;
    Yan::lexer p(file);
    Yan::token t;
    //if(p.openFile())
    {
        while(p.scan(&t))
        {
            std::cout<<t.tostring()<<" "<<t.value<<std::endl;
            if(t.type == Yan::tokenType::T_EOF)
            {
                break;
            }
        }

    }
    
    
    Yan::lexer pp(file);
    //if(pp.openFile())
    {
       // Yan::ASTnode* node;

        //Yan::token ttt;
       // pp.scan(&ttt);
        Yan::symbolTable symb;
        Yan::gen gen(symb);
        Yan::parser pars(pp,symb);
        auto pp= pars.parserProgram();
        gen.genProgram(pp);
        
       // gen.genPreamble();
       // Yan::Info("statements");
       // ast.statements();
        //gen.genpostamble();
       // node = ast.binExpr(ttt);
       // std::cout<<"node type:"<<node->op<<std::endl;
    
        //int result = Yan::interpExpAST(node);
        //std::cout<<"result is:"<<result<<std::endl;
        //Yan::gen gen;
       // gen.generateCode(node);
        
    }
    return 0;
}

