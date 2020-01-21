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
    
}
parser::~parser()
{
 
}
BinaryOp* parser::primary()
{
    BinaryOp* node =nullptr;
    Info(__func__);
    Info("hhhh%s",currentToken.tostring().c_str());
    switch( currentToken.type)
    {
        case tokenType::T_INTLIT:
            node = new BinaryOp(tokenType2ASTop(currentToken.type),nullptr, nullptr,currentToken.value);
            break;
        case tokenType::T_IDENT:
            auto id = symb.findGlob(currentToken.text);
            if(id == -1)
            {
                ExitWithError("undefined variable");
            }
            node = new BinaryOp(tokenType2ASTop(currentToken.type),nullptr, nullptr,id);
            break; 
       // default:
         //   ExitWithError("expect intlitr or identi");
    }
    nextToken();

    return node;
}
BinaryOp* parser::binExpr( int ptp)
{
    BinaryOp* left = nullptr, *right = nullptr;
    tokenType tokenType;
    if(currentToken.type == tokenType::T_SEMI)
    {
        std::cout<<"end of expr";
        return nullptr;
    }
    Info("token:%s",currentToken.tostring().c_str());
    left = primary();
    tokenType = currentToken.type;
    if(tokenType == tokenType::T_SEMI)
    {
        return left;
    }
    
    while(getOpPrecedence(tokenType)>ptp)
    {
        int current_ptp = getOpPrecedence(tokenType);
        std::cout<<"current_ptp"<<current_ptp<<std::endl;
        nextToken();
        right = binExpr(current_ptp);
        left = new BinaryOp(tokenType2ASTop(tokenType),left,right,0);
        tokenType = currentToken.type; 
        if (tokenType == tokenType::T_SEMI)
        {
            return left;
        }
    }
    return left;
    
       
}
void parser::match(tokenType t, const std::string& what)
{
    if(currentToken.type == t)
    {

    }
    else
    {
        ExitWithError(scan.getLocation(),"expected %s,but current token is %s ",what.c_str(),currentToken.tostring().c_str());
   
    }
}
void parser::printStatement()
{

    nextToken();
    Info(__func__);
    auto tree = binExpr();
    Info("jjjj");
    auto reg = codeGen.genBinaryOp(tree);
    Info("5550:%d",reg);
    codeGen.printint(reg);
    Info("5556");
    codeGen.freeAllReg();
    Info(__func__);
               
        //nextToken()
    match(tokenType::T_SEMI,"expect semi");

}
void parser::varDeclaration()
{
    Info(__func__);
    match(tokenType::T_INT,"int");
    nextToken();
    match(tokenType::T_IDENT,"identi");
    Info("44445");
    symb.addGlob(currentToken.text);
    Info("middle");
    codeGen.genGlobalSymbol(currentToken.text);
    nextToken();
    match(tokenType::T_SEMI,";");
    Info("end declar");
}
void parser::assignmentStatement()
{
    int id = symb.findGlob(currentToken.text);
    if(id == -1)
    {
        ExitWithError("undefined variable :%s",currentToken.text.c_str());
    }
     BinaryOp *left, *right, *tree;
     

     right = new  BinaryOp(BinaryOp::A_LVIDENT,nullptr,nullptr,id);
     nextToken();
     match(tokenType::T_ASSIGN,"=");
     nextToken();
     left = binExpr();
     tree = new BinaryOp(A_ASSIGN, left, right, 0);
     //codeGen.genAST(tree);
     codeGen.genBinaryOp(tree);
     codeGen.freeAllReg();
     match(tokenType::T_SEMI,";");
     Info(__func__);
     

}
void parser::statements()
{

    while (1)
    {
        nextToken();
        if(currentToken.type == tokenType::T_EOF)
        {
            return;
        }
        switch (currentToken.type)
        {
        case tokenType::T_PRINT:
            printStatement();
            break;
        case tokenType::T_INT:
            varDeclaration();
            break;
        case tokenType::T_IDENT:
            assignmentStatement();
            break;
        default:
            ExitWithError("unkown token :%s",currentToken.tostring().c_str());
            break;
        }
      

        
    }
    
}
int parser::getOpPrecedence(tokenType  optype) const
{
    
    return opPrecedence.at(optype);
}
int parser::tokenType2ASTop(tokenType type)
{ 
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

void parser::nextToken()
{
    scan.scan(&currentToken);
}
}
