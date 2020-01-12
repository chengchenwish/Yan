#include "lexer.h"
namespace Yan
{
    lexer::lexer(std::string&filename):
        fileName(filename),
        lineNum(0)
    {
    }

   bool  lexer::openFile()
   {
      infile.open(fileName.c_str(),std::ios_base::in);
      if(!infile.is_open())
      {
          std::cout<<"open file fail"<<std::endl;
    
         return false; 
      }
      return true;
   }
   bool lexer::next(char& c)
   {
        if(infile.get(c))
        {
            if(c == '\n')
            {
                lineNum++;
            }
            return true;
        }
        return false;
   }
   bool  lexer::skip(char& c)
   {
        
        if(!next(c))
        {
            return false;
        }
        while (c == '\t'||c == ' '|| c=='\n' || c == '\r' || c == '\f')
        {
            std::cout<<c<<" ";
            if(!next(c))
            {
                return false;
            }
        
        }
        return true;
   }
   bool lexer::scan(token* t)
   {
       char c;
       if (skip(c))
       {
           switch(c)
           {
            case '+':
                t->token = T_ADD;
                break;
            case '-':
                t->token = T_MINUS;
                break;
            case '*':
                t->token = T_STAR;
                break;
            case '/':
                t->token = T_SLASH;
                break;
            default:
                int num = 0;
                while( isdigit(c) )
                {  
                    int tempnum= static_cast<int>(c)-'0';
                    num = num*10+tempnum;
                    if (isdigit(peek()))
                    {   
                        skip(c);

                    }
                    else
                    {
                        break;
                    }
                }
                int temp;
                temp = peek();
                if(temp == ' '||temp == '+' || temp == '-' ||temp == '/'|| temp == '*'|| temp == '\n'|| temp == '\t'||temp=='\r'||temp == '\f'|| temp == EOF)
                {
                    t->token = T_INTLIT;
                    t->value = num;

                }
                else
                {
                    std::cout<<"ERROR: unknown token LINE:"<<lineNum<<" "<<temp<<std::endl;
                    return false;

                }
           }

        return true;
       }
       else
       {
            t->token = T_EOF;
            return true;
       }
   
       
   
   }

   bool lexer::isdigit(char c)
   {
     if( c>='0' && c<='9')
     {
         return true;
     }
     else
     {
         return false;
     }
   }
   int lexer::peek()
   {
       return infile.peek();

   }
   lexer::~lexer()
   {
       if(infile.is_open())
       {
           infile.close();
       }
   }
std::string tokenToString(tokenType t)
{
    switch (t)
    {
        case T_ADD:
            return "+";
        case T_STAR:
            return "*";
        case T_MINUS:
            return "-";
        case T_SLASH:
            return "/";
        case T_INTLIT :
            return "number";
        case T_EOF:
            return "eof";
        default:
            return "unknow";
    }
};
}
