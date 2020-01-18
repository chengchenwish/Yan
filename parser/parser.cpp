#include "parser.h"
namespace Yan{
const parser::precedenceMap parser::opPrecedence({
    std::pair<tokenType,int>(tokenType::T_EOF, 0),
    std::pair<tokenType,int>(tokenType::T_ADD, 100),
    std::pair<tokenType,int>(tokenType::T_MINUS, 100),
    std::pair<tokenType,int>(tokenType::T_STAR, 110),
    std::pair<tokenType,int>(tokenType::T_SLASH, 110)


});// = parser::initPrecedenceMap();

parser::parser(lexer& s,  gen& g):scan(s),codeGen(g)
{
    
}
parser::~parser()
{
 
}
ASTnode* parser::primary(token& t)
{
    ASTnode* node =nullptr;
    if (t.type == tokenType::T_INTLIT)
    {   Info("function:%s,token:%s",__func__,t.tostring().c_str());
        node = new ASTnode(tokenType2ASTop(t.type),nullptr, nullptr,t.value);
    }
    else
    {
        std::cout<<"ERROR:unexpected token"<<std::endl;
        exit(1);
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
void parser::statements()
{

    while (1)
    {
        scan.scan(&currentToken);
        if(currentToken.type == tokenType::T_EOF)
        {
            return;
        }
       // std::cout<<tokenToString(currentToken.token)<<"stmt"<<std::endl;;
        match(tokenType::T_PRINT,"expect print");
        if (currentToken.type!= tokenType::T_PRINT)
        {
            exit(1);
        }
        scan.scan(&currentToken);
        auto tree = binExpr(currentToken);
        auto reg = codeGen.genAST(tree);
        codeGen.printint(reg);
        codeGen.freeAllReg();
               
        //scan.scan(&currentToken);
        match(tokenType::T_SEMI,"expect semi");

        
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

}

return op;
    
}
}
