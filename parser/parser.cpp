#include "parser.h"
namespace Yan{
#define TOKEN_2_OP(xx) \
    xx(TokenType::T_EOF,OpType::UNKOWN, 0,) \
    xx(TokenType::T_EQ,OpType::OP_EQ, 70)


parser::parser(lexer& s,symbolTable& t):scan(s),symb(t)
{
}
parser::~parser()
{
 
}
Expr* parser::primary()
{
    Expr* node =nullptr;
    Info(__func__);

        if(test(TokenType::T_INTLIT))
        {
            auto t = consume();
            Info(std::to_string(t.value).c_str());
            node =  ConstantValue::create(t.value);
        }
        else if(test(TokenType::T_IDENT))
        {    Identifier* identi;
             auto t =consume();
             auto exist =symb.getIdentiInCurrentScope(t.text,&identi);
             if(!exist)
            {
                ExitWithError("undefined variable :%s",t.text.c_str());
             }
          // return Identifier::create(identi->name_,nullptr);
            return identi;
        }
        else
        {
            ExitWithError("The Primary token must be a number or identifier");
        }
                   

    return node;
}
//if the front token doesn't match the specific token exit with error
void parser::expect(TokenType t, const std::string& what)
{
    if(!test(t))
    {
         ExitWithError(scan.getLocation(),"matchTed %s,but current token is %s ",what.c_str(),scan.getToken().tostring().c_str());
    }
    else
    {
        consume();
    }
   
}
//Test the front token, if matched consume it,if not put back
bool parser::match(TokenType t)
{
    auto token = scan.getToken();
    if(token.type == t)
    {
        //current_token_ = token;
        return true;
    }
    else
    {
      //  current_token_ = token;
        scan.putBack(token);
        return false;
    }

}
bool  parser::test(TokenType t)
{
     auto token = scan.getToken();
    if(token.type == t)
    {
        scan.putBack(token); 
        return true;
    }
    else
    {
        scan.putBack(token);
        return false;
    }

}
Token  parser::consume()
{
    return scan.getToken();
     
}


PrintStmt* parser::parserPrintStmt()
{
    auto expr = sum();
    expect(TokenType::T_SEMI,";");
    return PrintStmt::create(expr);
}


BinaryOp* parser::parserAssignExpr(Token var)
{
    //int id = symb.findGlob(cacheToken.token_.text);
    Identifier* identi;
    auto exist =symb.getIdentiInCurrentScope(var.text,&identi);
    if(!exist)
    {
        ExitWithError("undefined variable :%s",var.text.c_str());
    }
     Expr *left, *right;
     BinaryOp* tree;
     

     left = identi;
     expect(TokenType::T_ASSIGN,"=");
     right = sum();
     tree = new BinaryOp(OpType::OP_ASSIGN, left, right);
     expect(TokenType::T_SEMI,";");
     Info(__func__);
     return tree;
     

}
//sum -> factor (('+' factor)|('-' factor))*
Expr* parser::sum()
{
    Expr* node = factor();
    for(;;)
    {
         if(match(TokenType::T_ADD))
         {
             node = BinaryOp::create(OpType::OP_ADD, node, factor());
         }
         else if(match(TokenType::T_MINUS))
         {
             node = BinaryOp::create(OpType::OP_SUBTRACT, node, factor());
         }
         else
         {
             return node;
         }
         
    }
 
}
//sum -> primary (('+' primary)|('-' primary))*
Expr* parser::factor()
{
    Expr* node = primary();
    for(;;)
    {
         if(match(TokenType::T_STAR))
         {
             node = BinaryOp::create(OpType::OP_MULTIPLY, node, primary());
         }
         else if(match(TokenType::T_SLASH))
         {
             node = BinaryOp::create(OpType::OP_DIVIDE, node, primary());
         }
         else
         {
             return node;
         }
         
    }


}
//cast = (type)cast|unary
 Expr*parser::cast()
 {
     if(test(TokenType::T_RPAREN))
     {
         auto t = consume();//consume '('
        if(isTypeName())
        {
         
          auto ty = baseType(nullptr);
         
             expect(TokenType::T_LPAREN,")");
            if(!test(TokenType::T_LBRACE))
            {
                 consume();
                return UnaryOp::create(OpType::OP_CAST,cast(),ty);
            }
        }
        scan.putBack(t);//puback token'('
     }
     return unary();
     
 }
 Expr* parser::unary()
 {
     if(match(TokenType::T_ADD))
     {
         return cast();
     }
     if(match(TokenType::T_MINUS))
     {
         return BinaryOp::create(OpType::OP_SUBTRACT,ConstantValue::create(0),cast());
     }
     if(match(TokenType::T_STAR))
     {

     }
     return postfix();
 }

 Expr* parser::postfix()
 {
     return primary();
 }

// Expr* parser::term()
// {

// }
// Expr* parser::group()
// {

// }
 CompousedStmt* parser::parserCompoundStmt()
 {
      Info(__func__);
     expect(TokenType::T_LBRACE,"{");
     auto compoused = CompousedStmt::create();
     while(!match(TokenType::T_RBRACE))
     {
     if(match(TokenType::T_EOF))
        {
            ExitWithError(__func__);
        }
        if (match(TokenType::T_PRINT))
        {
           compoused->addStmt(parserPrintStmt());
        }
        else if(match(TokenType::T_INT))
        {
            auto ty = IntType::create();
            if(test(TokenType::T_IDENT))
            {
                auto t = consume();

                auto identi = Identifier::create(t.text,ty);
                symb.addSymoble(identi->name_,identi);
                 compoused->addStmt(parserDeclaration(identi));
            }
            else
            {
                ExitWithError("wrong declaration");
            }
            
        }
        else if(match(TokenType::T_IF))
        {
            //ifStatement();
        }
        else if(match(TokenType::T_LBRACE))
        {

        }
        else if(test(TokenType::T_IDENT))
        {
             auto varToken = consume();
             if(match(TokenType::T_LPAREN))
             {
                 compoused->addStmt(parserFuncCall(varToken));
             }
             else
             {
                 compoused->addStmt(parserAssignExpr(varToken)); 
             }  
        }
        else
        {
     
           ExitWithError("unkown token :%s",scan.getToken().tostring().c_str());   
        } 
     }
     return compoused;

 }
  FunctionCall* parser::parserFuncCall(Token var)
  {
       Identifier* identi;
    auto exist =symb.getIdentiInCurrentScope(var.text,&identi);
    if(!exist)
    {
        if(var.text == "print")
        {
            identi = Identifier::create(var.text,FuncType::create(VoidType::create()));
        }
        else

            ExitWithError("undefined variable :%s",var.text.c_str());
    }
    else if(identi->type_->getType()!=Type::T_FUNC)
    {
        ExitWithError("Expect function type");
    }
      auto funcCall = FunctionCall::create(identi);

      auto expr = sum();
      Info("add args");
      funcCall->addArg(expr);
      expect(TokenType::T_RPAREN,")");
      expect(TokenType::T_SEMI,";");
      return funcCall;
 
  }
 FunctionDef* parser::parserFuncDef(Identifier* identi)
 {
     auto func = FunctionDef::create(identi);
     auto body = parserCompoundStmt();
     func->setBody(body);
     Info(__func__);
     return func;
     
 }
Declaration* parser::parserDeclaration(Identifier* identi)
{
    expect(TokenType::T_SEMI,";");
    return Declaration::create(identi);
}
 Program* parser::parserProgram()
 {

     auto program = Program::create();
     
     while(!match(TokenType::T_EOF))
     {
        storageClass sclass;     
        auto type = baseType(&sclass);
        Identifier* identi =nullptr;
        if(test(TokenType::T_IDENT))
        {
            auto t = consume();
            identi = Identifier::create(t.text,type);
        }
        else
        {
            ExitWithError(__func__);
        }
        if(match(TokenType::T_LPAREN))
        {
            identi->type_ = FuncType::create(type);
            if(match(TokenType::T_INT))
            {
                while(!match(TokenType::T_RPAREN))
                {
                    auto t = consume();
                    auto param = Identifier::create(t.text, IntType::create());
                    static_cast<FuncType*>(identi->type_)->addParam(param);
                    if(!test(TokenType::T_RPAREN))
                    {
                        match(TokenType::T_COMMA);
                    }
                }
              
            }
            else
            {
                expect(TokenType::T_RPAREN,")");
               // ExitWithError("function paser expect int");
            }
            
           // expect(TokenType::T_RPAREN,")");
        }
        
        symb.addSymoble(identi->name_,identi);

         if(identi->type_->getType() == Type::T_FUNC)
         {
            program->add(parserFuncDef(identi));
         }
         else
         {
             program->add(parserDeclaration(identi));
         }

     }
     Info(__func__);
     return program;
 }

 bool parser::isTypeName()
 {
     //current only support int/char
     return (test(TokenType::T_INT) ||test(TokenType::T_CHAR));
 }
 Type*parser::baseType(storageClass* sclass)
 {
     if(!isTypeName())
     {
         ExitWithError("match type token ");
     }
     
     if(match(TokenType::T_INT))
     {
         return IntType::create();
     }
     else if(match(TokenType::T_CHAR))
     {
         return CharType::create();
     }
     else
     {
         ExitWithError("unkown type name ");
     }
     

 }
 

OpType parser::TokenType2ASTop(TokenType type)
{ 
    Info(__func__);
    OpType op;

 switch (type)
{
    case TokenType::T_ADD:
        op = OpType::OP_ADD;
        break;
    case TokenType::T_STAR:
        op =  OpType::OP_MULTIPLY;
        break;
    case TokenType::T_MINUS:
        Info("ooooo-----");
        op = OpType::OP_SUBTRACT;
        break;
    case TokenType::T_SLASH:
        op =  OpType::OP_DIVIDE;
        break;
    case TokenType::T_EQ:
        op =  OpType::OP_EQ;
        break;
    case TokenType::T_NE:
        op =  OpType::OP_NE;
        break;
    case TokenType::T_LT:
        op =  OpType::OP_LT;
        break;
    case TokenType::T_GT:
        op =  OpType::OP_GT;
        break;
    case TokenType::T_LE:
        op =  OpType::OP_LE;
        break;
    case TokenType::T_GE:
        op =  OpType::OP_GE;
        break;
    default:
        op= OpType::OP_UNKOWN;

    
}

return op;
    
}

}
