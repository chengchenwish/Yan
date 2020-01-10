#include "parser.h"

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
        }
        if(p.peek() == EOF)
        {
            std::cout<<"end of the file"<<std::endl;
        }

    }
    else
    {
        std::cout<<"open file fail"<<std::endl;

    }
    return 0;
}
