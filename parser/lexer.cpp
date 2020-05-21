#include "lexer.h"
#include<string.h>
namespace Yan
{
    lexer::lexer(std::string&filename):
        loc(filename,1,0)
    {
        infile.open(filename.c_str(),std::ios_base::in);
      if(!infile.is_open())
      {
          ExitWithError("Fail to open file:%s",filename.c_str());
      }

    }

   bool lexer::next(char& c)
   {
        if(infile.get(c))
        {
            loc.colum++;
            if(c == '\n')
            {
                loc.line++;
                loc.colum = 0;
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
            if(!next(c))
            {
                return false;
            }
        
        }
        return true;
   }
   bool lexer::scan(Token* t)
   {
       char c;
       if (skip(c))
       {
           switch(c)
           {
            case '+':
                t->type = TokenType::T_ADD;
                break;
            case '-':
                t->type = TokenType::T_MINUS;
                break;
            case '*':
                t->type = TokenType::T_STAR;
                break;
            case '/':
                t->type = TokenType::T_SLASH;
                break;
            case '{':
                t->type = TokenType::T_LBRACE;
                break;
            case '}':
                t->type =  TokenType::T_RBRACE;
                break;
            case '(':
                t->type = TokenType::T_LPAREN;
                break;
            case ')':
                t->type =  TokenType::T_RPAREN;
                break;
            case ';':
                t->type = TokenType::T_SEMI;
                break;
            case ',':
                t->type = TokenType::T_COMMA;
                break;
            case '|':
                t->type = TokenType::T_OR;
                break;
            case '=':
                if(peek() == '=')
                {
                    consume();
                    t->type = TokenType::T_EQ;
                }
                else
                {
                   t->type = TokenType::T_ASSIGN;
                }
                break;
                
                
            case '>':
        
                if(peek() =='=')
                {   consume();
                    t->type = TokenType::T_GE;
                }
                else
                {
                    t->type = TokenType::T_GT;
                }
                
               break;
             case '<':
                
                if(peek() =='=')
                {   consume();
                    t->type = TokenType::T_LE;
                }
                else
                {
                    t->type = TokenType::T_LT;
                }
                
               break;
            case '!':
                if(peek() == '=')
                {
                    consume();
                    t->type = TokenType::T_NE;
                }
                else
                {
                    exit(1);
                }
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
            t->type = TokenType::T_EOF;
            return true;
       }
   
       
   
   }
   bool lexer::scanInt(char c, Token* t)
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
        if(temp==')'||temp == ';'||temp == ' '||isOperator(temp)|| temp == '\n'|| temp == '\t'||temp=='\r'||temp == '\f'|| temp == EOF)
        {
            t->type = TokenType::T_INTLIT;
            t->value = num;

        }
        else
        {
          //  t->sourceLocation.colum = 122;
            //t->sourceLocation.fileName ="p";
            ExitWithError(loc,"error toekn");
            //std::cout<<"ERROR: unknown Token LINE:"<<lineNum<<" "<<temp<<std::endl;
           // return false;

        }
        return true;
    }

   void lexer::scanIdenti(char c, Token*t)
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
    // if(strcmp(identi,"print")==0)
    // {
    //     t->type = TokenType::T_PRINT;
    // }
     if(strcmp(identi,"int") == 0)
    {
        t->type = TokenType::T_INT;
    }
    else if(strcmp(identi,"if") == 0)
    {
        t->type = TokenType::T_IF;
    }
    else if(strcmp(identi,"else") == 0)
    {
        t->type = TokenType::T_ELSE;
    }
    else
    {
        t->type = TokenType::T_IDENT;
        t->text = identi;
       // ExitWithError(loc,"unkown toekn:%s")
        //std::cout<<"unkown identi :"<<identi<<std::endl;
        //exit(1);
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
       if((c>='a' && c<='z')||(c>='A' && c<= 'Z'))
       {
           return true;
       }
       else
       {
           return false;
       }
   }
   bool lexer:: isOperator(char c)
   {
       if(c=='+'||c == '-'||c=='*'||c=='/'||c=='<'||c=='>'||c == '!'||c == '=')
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
   void lexer::consume()
   {
       char c;
       next(c);
       if(c == '\n')
       {
           loc.line++;
           loc.colum = 0;
       }
       else
       {
           loc.colum++;
       }
       
       
   }

   void lexer::putBack(const Token& t)
   {
       tokenCache_.push(t);

   }
   Token lexer::getToken()
   {
       if(tokenCache_.empty())
       {
           Token t;
           scan(&t);
           return t;
       }
       else
       {
           auto t = tokenCache_.front();
           tokenCache_.pop();
           return t;
       }
   }
   lexer::~lexer()
   {
       if(infile.is_open())
       {
           infile.close();
       }
   }

};

