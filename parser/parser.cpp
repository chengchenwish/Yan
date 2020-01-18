#include "parser.h"
namespace Yan{
const parser::precedenceMap parser::opPrecedence({
    std::pair<tokenType,int>(tokenType::T_EOF, 0),
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
ASTnode* parser::primary(token& t)
{
    ASTnode* node =nullptr;
    Info(__func__);
    switch( t.type)
    {
        case tokenType::T_INTLIT:
            node = new ASTnode(tokenType2ASTop(t.type),nullptr, nullptr,t.value);
            break;
        case tokenType::T_IDENT:
            auto id = symb.findGlob(t.text);
            if(id == -1)
            {
                ExitWithError("undefined variable");
            }
            node = new ASTnode(tokenType2ASTop(t.type),nullptr, nullptr,id);
            break; 
       // default:
         //   ExitWithError("expect intlitr or identi");
    }
    return node;
}
ASTnode* parser::binExpr(token& t, int ptp)
{
    ASTnode* left = nullptr, *right = nullptr;
    tokenType tokenType;
    if(t.type == tokenType::T_SEMI)
    {
        std::cout<<"end of expr";
        return nullptr;
    }
    Info("token:%s",t.tostring().c_str());
    left = primary(t);
    scan.scan(&currentToken);
    tokenType = currentToken.type;
    if(tokenType == tokenType::T_SEMI)
    {
        return left;
    }
    
    while(getOpPrecedence(tokenType)>ptp)
    {
        int current_ptp = getOpPrecedence(tokenType);
        std::cout<<"current_ptp"<<current_ptp<<std::endl;
        scan.scan(&currentToken);
        right = binExpr(currentToken,current_ptp);
        left = new ASTnode(tokenType2ASTop(tokenType),left,right,0);
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
        std::cout<<what<<std::endl;
        exit(1);
    }
}
void parser::printStatement()
{

    scan.scan(&currentToken);
    Info(__func__);
    auto tree = binExpr(currentToken);
    auto reg = codeGen.genAST(tree);
    codeGen.printint(reg);
    codeGen.freeAllReg();
    Info(__func__);
               
        //scan.scan(&currentToken);
    match(tokenType::T_SEMI,"expect semi");

        

}
void parser::varDeclaration()
{
    Info(__func__);
    match(tokenType::T_INT,"int");
    scan.scan(&currentToken);
    match(tokenType::T_IDENT,"identi");
    Info("44445");
    symb.addGlob(currentToken.text);
    Info("middle");
    codeGen.genGlobalSymbol(currentToken.text);
    scan.scan(&currentToken);
    match(tokenType::T_SEMI,";");
    Info("end declar");
}
void parser::assignmentStatement()
{
    int id = symb.findGlob(currentToken.text);
    if(id == -1)
    {
        ExitWithError("undefined variable :%s",currentToken.text);
    }
     ASTnode *left, *right, *tree;
     

     right = new ASTnode(A_LVIDENT,nullptr,nullptr,id);
     scan.scan(&currentToken);
     match(tokenType::T_ASSIGN,"=");
     scan.scan(&currentToken);
     left = binExpr(currentToken);
     tree = new ASTnode(A_ASSIGN, left, right, 0);
     codeGen.genAST(tree);
     codeGen.freeAllReg();
     match(tokenType::T_SEMI,";");
     Info(__func__);
     

}
void parser::statements()
{

    while (1)
    {
        scan.scan(&currentToken);
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
        op = A_ADD;
        break;
    case tokenType::T_STAR:
        op = A_MULTIPLY;
        break;
    case tokenType::T_MINUS:
        op = A_SUBTRACT;
        break;
    case tokenType::T_SLASH:
        op = A_DIVIDE;
        break;
    case tokenType::T_INTLIT:
        op = A_INTLIT;
        break;
    case tokenType::T_IDENT:
        op = A_IDENTI;
        break;
}

return op;
    
}
}
