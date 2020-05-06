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


parser::parser(lexer& s,gen&g,symbolTable& t):scan(s),codeGen(g),symb(t)
{
    tmpToken.hasvule = false;
}
parser::~parser()
{
 
}
BinaryOp* parser::primary()
{
    BinaryOp* node =nullptr;
    Info(__func__);

        if(match(tokenType::T_INTLIT))
        {
            Info(std::to_string(tmpToken.tempToken.value).c_str());
            node = new BinaryOp(tokenType2ASTop(tmpToken.tempToken.type),nullptr, nullptr,tmpToken.tempToken.value);
        }
        else if(match(tokenType::T_IDENT))
        {   auto id = symb.findGlob(tmpToken.tempToken.text);
            if(id == -1)
            {
                ExitWithError("undefined variable");
            }
            node = new BinaryOp(tokenType2ASTop(tmpToken.tempToken.type),nullptr, nullptr,id);
        }
        else
        {
            ExitWithError("The Primary token must be a number or identifier");
        }
                   

    return node;
    Info("endfff");
}
BinaryOp* parser::binExpr( int ptp)
{
    BinaryOp* left = nullptr, *right = nullptr;
    tokenType tokenType;
    Info("token:%s",tmpToken.tempToken.tostring().c_str());
    if(match(tokenType::T_SEMI))
    {
        return left;
    }

    left = primary();
    
    if(test(tokenType::T_SEMI))
    {
        return left;
    }
    tokenType = tmpToken.tempToken.type;
    
    while(getOpPrecedence(tokenType)>ptp)
    {
        auto t = consume();        
        int current_ptp = getOpPrecedence(tokenType);
        right = binExpr(current_ptp);
        left = new BinaryOp(tokenType2ASTop(tokenType),left,right,0);
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
void parser::printStatement()
{

    Info(__func__);
    auto tree = binExpr();
   // expect(tokenType::T_SEMI, ";");

    Info("jjjj");
    auto reg = codeGen.genBinaryOp(tree);
    Info("5550:%d",reg);
    codeGen.printint(reg);
    Info("5556");
    codeGen.freeAllReg();
    Info(__func__);
               
        //nextToken()
    expect(tokenType::T_SEMI,"match semi");

}
void parser::varDeclaration()
{
    Info(__func__);
    expect(tokenType::T_IDENT,"identi");
    Info("44445");
    symb.addGlob(tmpToken.tempToken.text);
    Info("middle");
    codeGen.genGlobalSymbol(tmpToken.tempToken.text);
    expect(tokenType::T_SEMI,";");
    Info("end declar");
}
void parser::assignmentStatement()
{
    int id = symb.findGlob(tmpToken.tempToken.text);
    if(id == -1)
    {
        ExitWithError("undefined variable :%s",tmpToken.tempToken.text.c_str());
    }
     BinaryOp *left, *right, *tree;
     

     right = new  BinaryOp(BinaryOp::A_LVIDENT,nullptr,nullptr,id);
     expect(tokenType::T_ASSIGN,"=");
     left = binExpr();
    // expect(tokenType::T_SEMI, ";");
     tree = new BinaryOp(BinaryOp::A_ASSIGN, left, right, 0);
     //codeGen.genAST(tree);
     codeGen.genBinaryOp(tree);
     codeGen.freeAllReg();
     expect(tokenType::T_SEMI,";");
     Info(__func__);
     

}

void parser::ifStatement()
{
    expect(tokenType::T_IF,"if");

    expect(tokenType::T_LPAREN,"(");

    auto tree = binExpr();

    expect(tokenType::T_RPAREN,")");

    expect(tokenType::T_RBRACE,"{");
    statements();
    expect(tokenType::T_RBRACE,"}");

}
void parser::statements()
{
    while(1){
        if(match(tokenType::T_EOF))
        {
            return;
        }
        if (match(tokenType::T_PRINT))
        {
            printStatement();
        }
        else if(match(tokenType::T_INT))
        {
             varDeclaration();
        }
        else if(match(tokenType::T_IF))
        {
            ifStatement();
        }
        else if(match(tokenType::T_LBRACE))
        {

        }
        else if(match(tokenType::T_IDENT))
        {
             assignmentStatement();   
        }
        else
        {
     
           ExitWithError("unkown token :%s",tmpToken.tempToken.tostring().c_str());   
        } 
    }        
    
}
 CompousedStmt* parser::parserCompoundStmt()
 {
     expect(tokenType::T_LBRACE,"{");
     auto compoused = CompousedStmt::create();
     while(match(tokenType::T_RBRACE))
     {
     if(match(tokenType::T_EOF))
        {
            ExitWithError(__func__);
        }
        if (match(tokenType::T_PRINT))
        {
            printStatement();
        }
        else if(match(tokenType::T_INT))
        {
             varDeclaration();
        }
        else if(match(tokenType::T_IF))
        {
            ifStatement();
        }
        else if(match(tokenType::T_LBRACE))
        {

        }
        else if(match(tokenType::T_IDENT))
        {
             assignmentStatement();   
        }
        else
        {
     
           ExitWithError("unkown token :%s",tmpToken.tempToken.tostring().c_str());   
        } 
     }
     return compoused;

 }
 FunctionDef* parser::parserFuncDef(Identifier* identi)
 {
     auto func = FunctionDef::create(identi);
     auto body = parserCompoundStmt();
     func->setBody(body);
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
            identi->type_ = (Type*)FuncType::create(type);
            expect(tokenType::T_RPAREN,")");
        }
        
        symb.addSymoble(identi->name_,*identi);

         if(identi->type_->getType() == Type::T_FUNC)
         {
            program->add(parserFuncDef(identi));
         }
         else
         {
             program->add(parserDeclaration(identi));
         }

     }
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
    case tokenType::T_INTLIT:
        op =  BinaryOp::A_INTLIT;
        break;
    case tokenType::T_IDENT:
        op =  BinaryOp::A_IDENTI;
        break;
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
