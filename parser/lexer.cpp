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

   int lexer::next()
   {
      int c = infile.get();

       loc.colum++;
       if(c == '\n')
      {
         loc.line++;
         loc.colum = 0;
      }
      return c;

   }
   //skip the the charecter that we didn't care about when hanging token's beginnig
   int lexer::skip()
   {
        
        int c = next();
        while (c == '\t'||c == ' '|| c=='\n' || c == '\r' || c == '\f')
        {
           c = next();             
        
        }
        return c;
   }
   void lexer::scan(Token* t)
   {
       int c = skip();
     
           switch(c)
           {
            case EOF:
            Info("eof");
                t->type = TokenType::T_EOF;
                break;
            case '+':
                t->type = TokenType::T_ADD;
                break;
            case '-':
                t->type = TokenType::T_MINUS;
                break;
            case '*':
                t->type = TokenType::T_STAR;
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
            case '/':
                if(peek() == '*')
                {
                    consume('*');
                    int pre;
                   while(1)
                   {
                       pre = next();
                       if(pre == EOF)
                       {
                           ExitWithError("Expect end of file");
                       }
                       if(peek() == '/' && pre =='*')
                       { 
                           consume('/');
                            t->type = TokenType::T_COMMENTS;
                           break;
                       }
                   }
                }
                else
                {
                     t->type = TokenType::T_SLASH;
                }
                break;
            case '|':
                t->type = TokenType::T_OR;
                break;
            case '=':
                if(peek() == '=')
                {
                    consume('=');
                    t->type = TokenType::T_EQ;
                }
                else
                {
                   t->type = TokenType::T_ASSIGN;
                }
                break;
                
                
            case '>':
        
                if(peek() =='=')
                {   consume('=');
                    t->type = TokenType::T_GE;
                }
                else
                {
                    t->type = TokenType::T_GT;
                }
                
               break;
             case '<':
                
                if(peek() =='=')
                {   consume('=');
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
                   
                    consume('=');
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
                    scanInt(c, t);
                }
                else if(isalpha(c))
                {
                    scanIdenti(c,t);
                }
                else
                {
                    ExitWithError(loc,"unexpect charector:%c",c);
                }
           }
         

   
       
   
   }
   void lexer::scanInt(char c, Token* t)
   {
        int num = 0;
        while( isdigit(c) )
        {  
            int tempnum= c - '0';
            num = num*10+tempnum;
            if (isdigit(peek()))
            {   
               c = skip();

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
            t->text = num;

        }
        else
        {
          //  t->sourceLocation.colum = 122;
            //t->sourceLocation.fileName ="p";
            ExitWithError(loc,"error toekn");
            //std::cout<<"ERROR: unknown Token LINE:"<<lineNum<<" "<<temp<<std::endl;
           // return false;

        }
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
                ch = next();
                
                identi[len++]=ch;
            }
           
            else
            {
                break;
            }
           
       }
       identi[len++]='\0';
       if(len>MAX_STR_LEN)
       {
           ExitWithError(loc,"Identifier too long");
       }
    //    char end = peek();
    //    if(end !=' '|| end !='('|| end ! = ')' || end != ';' || end != ',')
    // if(strcmp(identi,"print")==0)
    // {
    //     t->type = TokenType::T_PRINT;
    // }
   
     if(keyword(identi,t))
    {
        return;
    }   
  
    t->type = TokenType::T_IDENT;
    t->text = identi;
       // ExitWithError(loc,"unkown toekn:%s")
        //std::cout<<"unkown identi :"<<identi<<std::endl;
        //exit(1);
  
         
      
   }
   bool lexer::keyword(char* s,Token*t)
   {
       #define xx(ty,str) \
       if(strcmp(s,str)==0)\
       {\
           t->type = TokenType::ty;\
           return true;\
       }
       KEYWORD(xx)
       #undef xx
       return false;
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
   //consume the match char
   void lexer::consume(char c)
   {
       if(peek() == c)
       {
           next();
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
   Token lexer::peektoken()
   {
        if(tokenCache_.empty())
       {
           Token t;
           scan(&t);
           tokenCache_.push(t);
           return t;
       }
       else
       {
          return  tokenCache_.front();
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

