#include "lexer.h"
#include<string.h>
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
                t->type = tokenType::T_ADD;
                break;
            case '-':
                t->type = tokenType::T_MINUS;
                break;
            case '*':
                t->type = tokenType::T_STAR;
                break;
            case '/':
                t->type = tokenType::T_SLASH;
                break;
            case ';':
                t->type = tokenType::T_SEMI;
                break;
            default:
                if(isdigit(c))
                {
                    if(scanInt(c, t)==false)
                    {
                        exit(1);
                    } 
                }
                else if(isalpha(c))
                {
                    scanIdenti(c,t);
                }
                else
                {
                    std::cout<<"unkown char:"<<c<<std::endl;
                    exit(1);
                }
           }
         

        return true;
       }
       else
       {
            t->type = tokenType::T_EOF;
            return true;
       }
   
       
   
   }
   bool lexer::scanInt(char c, token* t)
   {
        int num = 0;
        while( isdigit(c) )
        {  
            int tempnum= c - '0';
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
        if(temp == ';'||temp == ' '||temp == '+' || temp == '-' ||temp == '/'|| temp == '*'|| temp == '\n'|| temp == '\t'||temp=='\r'||temp == '\f'|| temp == EOF)
        {
            t->type = tokenType::T_INTLIT;
            t->value = num;

        }
        else
        {
            std::cout<<"ERROR: unknown token LINE:"<<lineNum<<" "<<temp<<std::endl;
            return false;

        }
        return true;
    }

   void lexer::scanIdenti(char c, token*t)
   {
       char identi[MAX_STR_LEN];
       int len = 0;
       identi[len++] = c;
       char ch;
       while(1)
       {
            if(isalpha(peek())||isdigit(peek())|| peek() == '_')
            {
                next(ch);
                identi[len++]=ch;
            }
           
            else
            {
                break;
            }
           
       }
       identi[len++]='\0';
    if(strcmp(identi,"print")==0)
    {
        t->type = tokenType::T_PRINT;
    }
    else
    {
        std::cout<<"unkown identi :"<<identi<<std::endl;
        exit(1);
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
   bool lexer::isalpha(char c)
   {
       if((c>='a' && c<='z')||c>='A' && c<= 'Z')
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

};

