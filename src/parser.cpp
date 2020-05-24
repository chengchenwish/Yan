#include "parser.h"
namespace Yan{
// #define TOKEN_2_OP(xx) \
//     xx(TokenType::T_EOF,OpType::UNKOWN, 0,) \
//     xx(TokenType::T_EQ,OpType::OP_EQ, 70)


parser::parser(lexer& s):scan(s)
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
            
            Info(std::to_string(t.getValue()).c_str());
            node =  ConstantValue::create(t.getValue());
        }
        else if(test(TokenType::T_IDENT))
        {    Identifier* identi;
             auto t =consume();
             auto exist =currentScop_->getIdentiInAllScope(t.getText(),&identi);
             if(!exist)
            {
                ExitWithError("undefined variable :%s",t.getText().c_str());
             }
          // return Identifier::create(identi->name_,nullptr);
            return identi;
        }
        else if(match(TokenType::T_LPAREN))
        {
            auto expr = sum();
            expect(TokenType::T_RPAREN,")");
            return expr;

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
    //int id = currentScop_->findGlob(cacheToken.token_.getText());
    Identifier* identi;
    auto exist =currentScop_->getIdentiInAllScope(var.getText(),&identi);
    if(!exist)
    {
        ExitWithError("undefined variable :%s",var.getText().c_str());
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
// expr = assign(","assign")*
Expr* parser::expr()
{
    Expr* left = assign();
    while(match(TokenType::T_COMMA))
    {
        auto right = assign();
        left = BinaryOp::create(OpType::OP_COMMA,left,right);

    }
    return left;

}
// assign    = conditional (assign-op assign)?
// assign-op = "=" | "+=" | "-=" | "*=" | "/=" | "<<=" | ">>="
//           | "&=" | "|=" | "^="
Expr* parser::assign()
{
    auto node = conditional();
    if (match(TokenType::T_ASSIGN))//=
    {
        return BinaryOp::create(OpType::OP_ASSIGN,node,assign());
    }
    if (match(TokenType::T_ASSLASH))// /=
    {
        return BinaryOp::create(OpType::OP_ASSMOD,node,assign());
    }
     if (match(TokenType::T_ASSTAR))//*=
    {
        return BinaryOp::create(OpType::OP_ASSMUL,node,assign());
    }
     if (match(TokenType::T_ASPLUS))// +=
    {
        return BinaryOp::create(OpType::OP_ASSPLUS,node,assign());
    }
     if (match(TokenType::T_ASMINUS)) // -=
    {
        return BinaryOp::create(OpType::OP_ASSSUB,node,assign());
    }

     if (match(TokenType::T_ASMOD)) // %=
    {
        return BinaryOp::create(OpType::OP_ASSMOD,node,assign());
    }

     if (match(TokenType::T_ASSIGN))
    {
        return BinaryOp::create(OpType::OP_ASSIGN,node,assign());
    }
    //TODO <<= >>= &= ^= |=
    return node;
    
}
Expr* parser::conditional()
{
    return sum();
}

//sum -> mul (('+' mul)|('-' mul))*
Expr* parser::sum()
{
    Expr* node = mul();
    for(;;)
    {
         if(match(TokenType::T_ADD))
         {
             node = BinaryOp::create(OpType::OP_ADD, node, mul());
         }
         else if(match(TokenType::T_MINUS))
         {
             node = BinaryOp::create(OpType::OP_SUBTRACT, node, mul());
         }
         else
         {
             return node;
         }
         
    }
 
}
//sum -> primary (('+' primary)|('-' primary))*
Expr* parser::mul()
{
    Expr* node = cast();
    for(;;)
    {
         if(match(TokenType::T_STAR))
         {
             node = BinaryOp::create(OpType::OP_MULTIPLY, node, cast());
         }
         else if(match(TokenType::T_SLASH))
         {
             node = BinaryOp::create(OpType::OP_DIVIDE, node, cast());
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

                auto identi = Identifier::create(t.getText(),ty, true);
                currentScop_->addSymoble(identi->name_,identi);
                 compoused->addStmt(parserDeclaration(identi));
                 identi->setoffset(currentScop_->caculateOffset(t.getText()));
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
    auto exist =currentScop_->getIdentiInAllScope(var.getText(),&identi);
    if(!exist)
    {
        if(var.getText() == "print")
        {
            identi = Identifier::create(var.getText(),FuncType::create(VoidType::create()),false);
        }
        else

            ExitWithError("undefined variable :%s",var.getText().c_str());
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
    currentScop_ = new symbolTable();
    currentScop_->setScope(Scope::GLOBAL);
     symbolTable*funcScop = nullptr;
     
     while(!match(TokenType::T_EOF))
     {
        if(match(TokenType::T_COMMENTS))
        {
            Info("comments");
             continue;
        }
        storageClass sclass;     
        auto type = baseType(&sclass);
        Identifier* identi =nullptr;
        if(test(TokenType::T_IDENT))
        {
            auto t = consume();
            identi = Identifier::create(t.getText(),type,false);
        }
        else
        {
            ExitWithError(__func__);
        }
        if(match(TokenType::T_LPAREN))
        {
            identi->type_ = FuncType::create(type);
            funcScop = new symbolTable();
            funcScop->setScope(Scope::FUNC);
            if(match(TokenType::T_INT))
            {
                while(!match(TokenType::T_RPAREN))
                {
                    auto t = consume();
                    auto text = t.getText();
                    auto param = Identifier::create(text, IntType::create(),true);
                    funcScop->addSymoble(text,param);
                    funcScop->caculateOffset(text);
                    param->offset_ = funcScop->caculateOffset(text);
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
        
        currentScop_->addSymoble(identi->name_,identi);

         if(identi->type_->getType() == Type::T_FUNC)
         {
            funcScop->setParent(currentScop_);
            currentScop_ = funcScop;
            program->add(parserFuncDef(identi));
            currentScop_= currentScop_-> getParentScop();
         }
         else
         {
             program->add(parserDeclaration(identi));
         }

     }
     Info("hello");
     Info(__func__);
     return program;
 }

 bool parser::isTypeName()
 {
     //current only support int/char
     return (test(TokenType::T_INT) ||test(TokenType::T_CHAR)|| test(TokenType::T_BOOL)
             || test(TokenType::T_ENUM) || test(TokenType::T_SHORT)|| test(TokenType::T_SIGNED)||
             test(TokenType::T_STATIC)|| test(TokenType::T_STRUCT)|| test(TokenType::T_VOID)||
             test(TokenType::T_TYPDEF) || test(TokenType::T_EXTERN) || test(TokenType::T_LONG) 
             ||(test(TokenType::T_IDENT) && findtypedef(peek().getText())));
 }
 bool parser::findtypedef(const std::string& name )
 {
     //todo
     return false;
 }

 // basetype = builtin-type | struct-decl | typedef-name | enum-specifier
//
// builtin-type = "void" | "_Bool" | "char" | "short" | "int"
//              | "long" | "long" "long"
 Type*parser::baseType(storageClass* sclass)
 {
     if(!isTypeName())
     {
         ExitWithError("Type name expected");
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
