#include "parser.h"
namespace Yan{
const parser::precedenceMap parser::opPrecedence({
    std::pair<tokenType,int>(tokenType::T_EOF, 0),
 
    std::pair<tokenType,int>(tokenType::T_EQ, 70),
    std::pair<tokenType,int>(tokenType::T_NE, 70),

    std::pair<tokenType,int>(tokenType::T_GT, 80),
    std::pair<tokenType,int>(tokenType::T_LT, 80),
    std::pair<tokenType,int>(tokenType::T_LE, 80),
    std::pair<tokenType,int>(tokenType::T_GE, 80),

    std::pair<tokenType,int>(tokenType::T_ADD, 100),
    std::pair<tokenType,int>(tokenType::T_MINUS, 100),
    std::pair<tokenType,int>(tokenType::T_STAR, 110),
    std::pair<tokenType,int>(tokenType::T_SLASH, 110)


});// = parser::initPrecedenceMap();


parser::parser(lexer& s,symbolTable& t):scan(s),symb(t)
{
    tmpToken.hasvule = false;
}
parser::~parser()
{
 
}
Expr* parser::primary()
{
    Expr* node =nullptr;
    Info(__func__);

        if(match(tokenType::T_INTLIT))
        {
            Info(std::to_string(tmpToken.tempToken.value).c_str());
            node =  ConstantValue::create(tmpToken.tempToken.value);
        }
        else if(match(tokenType::T_IDENT))
        {    Identifier* identi;
             auto exist =symb.getIdentiInCurrentScope(tmpToken.tempToken.text,&identi);
             if(!exist)
            {
                ExitWithError("undefined variable :%s",tmpToken.tempToken.text.c_str());
             }
          // return Identifier::create(identi->name_,nullptr);
            return identi;
        }
        else
        {
            ExitWithError("The Primary token must be a number or identifier");
        }
                   

    return node;
    Info("endfff");
}
Expr* parser::binExpr( int ptp)
{
    Expr* left = nullptr, *right = nullptr;
    tokenType tokenType;
    Info("token:%s",tmpToken.tempToken.tostring().c_str());
    if(match(tokenType::T_SEMI))
    {
        return left;
    }

    left = primary();
    
    if(test(tokenType::T_SEMI)|| test(tokenType::T_RPAREN))
    {
        return left;
    }
    tokenType = tmpToken.tempToken.type;
    
    while(getOpPrecedence(tokenType)>ptp)
    {
        auto t = consume();        
        int current_ptp = getOpPrecedence(tokenType);
        right = binExpr(current_ptp);
        left = new BinaryOp(tokenType2ASTop(tokenType),left,right);
        tokenType = t.type; 
        if(test(tokenType::T_SEMI))
        {
            return left;
        }
    }
    return left;    
       
}
//if the front token doesn't match the specific token exit with error
void parser::expect(tokenType t, const std::string& what)
{
    if(!match(t))
    {
         ExitWithError(scan.getLocation(),"matchTed %s,but current token is %s ",what.c_str(),tmpToken.tempToken.tostring().c_str());
    }
   
}
//Test the front token, if matched consume it,if not put back
bool parser::match(tokenType t)
{
    if(!tmpToken.hasvule)
    {
        
        scan.scan(&tmpToken.tempToken);
        Info(tmpToken.tempToken.tostring().c_str());
        tmpToken.hasvule = true;
    }
    if(tmpToken.tempToken.type == t)
    {
        //consume the token
          tmpToken.hasvule = false;
          return true;     
    }
    else
    {
        return false;
    } 

}
bool  parser::test(tokenType t)
{
    if(match(t))
    {
        tmpToken.hasvule = true;
        return true;
    }
    else
    {
        tmpToken.hasvule = true;
        return false;
    }
}
token  parser::consume()
{
     if(tmpToken.hasvule)
     {
         tmpToken.hasvule = false;
         return tmpToken.tempToken;
     }
     else
     {
         scan.scan(&tmpToken.tempToken);
         tmpToken.hasvule = true;
         return tmpToken.tempToken;
     }
     
}
// void parser::printStatement()
// {

//     Info(__func__);
//     auto tree = binExpr();
//    // expect(tokenType::T_SEMI, ";");

//     Info("jjjj");
//     auto reg = codeGen.genBinaryOp(tree);
//     Info("5550:%d",reg);
//     codeGen.printint(reg);
//     Info("5556");
//     codeGen.freeAllReg();
//     Info(__func__);
               
//         //nextToken()
//     expect(tokenType::T_SEMI,"match semi");

// }
PrintStmt* parser::parserPrintStmt()
{
    auto expr = binExpr();
    expect(tokenType::T_SEMI,";");
    return PrintStmt::create(expr);
}
// void parser::varDeclaration()
// {
//     Info(__func__);
//     expect(tokenType::T_IDENT,"identi");
//     Info("44445");
//     symb.addGlob(tmpToken.tempToken.text);
//     Info("middle");
//     codeGen.genGlobalSymbol(tmpToken.tempToken.text);
//     expect(tokenType::T_SEMI,";");
//     Info("end declar");
// }
BinaryOp* parser::parserAssignExpr(token var)

{
    //int id = symb.findGlob(tmpToken.tempToken.text);
    Identifier* identi;
    auto exist =symb.getIdentiInCurrentScope(var.text,&identi);
    if(!exist)
    {
        ExitWithError("undefined variable :%s",var.text.c_str());
    }
     Expr *left, *right;
     BinaryOp* tree;
     

     left = identi;
     expect(tokenType::T_ASSIGN,"=");
     right = binExpr();
    // expect(tokenType::T_SEMI, ";");
     tree = new BinaryOp(BinaryOp::A_ASSIGN, left, right);
     //codeGen.genAST(tree);
    // codeGen.genBinaryOp(tree);
    // codeGen.freeAllReg();
     expect(tokenType::T_SEMI,";");
     Info(__func__);
     return tree;
     

}

// void parser::ifStatement()
// {
//     expect(tokenType::T_IF,"if");

//     expect(tokenType::T_LPAREN,"(");

//     auto tree = binExpr();

//     expect(tokenType::T_RPAREN,")");

//     expect(tokenType::T_RBRACE,"{");
//     statements();
//     expect(tokenType::T_RBRACE,"}");

// }

 CompousedStmt* parser::parserCompoundStmt()
 {
      Info(__func__);
     expect(tokenType::T_LBRACE,"{");
     auto compoused = CompousedStmt::create();
     while(!match(tokenType::T_RBRACE))
     {
     if(match(tokenType::T_EOF))
        {
            ExitWithError(__func__);
        }
        if (match(tokenType::T_PRINT))
        {
           compoused->addStmt(parserPrintStmt());
        }
        else if(match(tokenType::T_INT))
        {
            auto ty = IntType::create();
            if(test(tokenType::T_IDENT))
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
        else if(match(tokenType::T_IF))
        {
            //ifStatement();
        }
        else if(match(tokenType::T_LBRACE))
        {

        }
        else if(match(tokenType::T_IDENT))
        {
             auto varToken = tmpToken.tempToken;
             if(match(tokenType::T_LPAREN))
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
     
           ExitWithError("unkown token :%s",tmpToken.tempToken.tostring().c_str());   
        } 
     }
     return compoused;

 }
  FunctionCall* parser::parserFuncCall(token var)
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
      auto expr = binExpr();
      Info("add args");
      funcCall->addArg(expr);
      expect(tokenType::T_RPAREN,")");
      expect(tokenType::T_SEMI,";");
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
    expect(tokenType::T_SEMI,";");
    return Declaration::create(identi);
}
 Program* parser::parserProgram()
 {

     auto program = Program::create();
     
     while(!match(tokenType::T_EOF))
     {
        storageClass sclass;     
        auto type = baseType(&sclass);
        Identifier* identi =nullptr;
        if(test(tokenType::T_IDENT))
        {
            auto t = consume();
            identi = Identifier::create(t.text,type);
        }
        else
        {
            ExitWithError(__func__);
        }
        if(match(tokenType::T_LPAREN))
        {
            identi->type_ = FuncType::create(type);
            if(match(tokenType::T_INT))
            {
                while(!match(tokenType::T_RPAREN))
                {
                    auto t = consume();
                    auto param = Identifier::create(t.text, IntType::create());
                    static_cast<FuncType*>(identi->type_)->addParam(param);
                    if(!test(tokenType::T_RPAREN))
                    {
                        match(tokenType::T_COMMA);
                    }
                }
              
            }
            else
            {
                expect(tokenType::T_RPAREN,")");
               // ExitWithError("function paser expect int");
            }
            
           // expect(tokenType::T_RPAREN,")");
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
     return (test(tokenType::T_INT) ||test(tokenType::T_CHAR));
 }
 Type*parser::baseType(storageClass* sclass)
 {
     if(!isTypeName())
     {
         ExitWithError("match type token ");
     }
     
     if(match(tokenType::T_INT))
     {
         return IntType::create();
     }
     else if(match(tokenType::T_CHAR))
     {
         return CharType::create();
     }
     else
     {
         ExitWithError("unkown type name ");
     }
     

 }
int parser::getOpPrecedence(tokenType  optype) const
{
    
    return opPrecedence.at(optype);
}
int parser::tokenType2ASTop(tokenType type)
{ 
    Info(__func__);
    int op;

 switch (type)
{
    case tokenType::T_ADD:
        op = BinaryOp::A_ADD;
        break;
    case tokenType::T_STAR:
        op =  BinaryOp::A_MULTIPLY;
        break;
    case tokenType::T_MINUS:
        op = BinaryOp::A_SUBTRACT;
        break;
    case tokenType::T_SLASH:
        op =  BinaryOp::A_DIVIDE;
        break;
    // case tokenType::T_INTLIT:
    //     op =  BinaryOp::A_INTLIT;
    //     break;
    // case tokenType::T_IDENT:
    //     op =  BinaryOp::A_IDENTI;
    //     break;
    case tokenType::T_EQ:
        op =  BinaryOp::A_EQ;
        break;
    case tokenType::T_NE:
        op =  BinaryOp::A_NE;
        break;
    case tokenType::T_LT:
        op =  BinaryOp::A_LT;
        break;
    case tokenType::T_GT:
        op =  BinaryOp::A_GT;
        break;
    case tokenType::T_LE:
        op =  BinaryOp::A_LE;
        break;
    case tokenType::T_GE:
        op =  BinaryOp::A_GE;
        break;

    
}

return op;
    
}

}
