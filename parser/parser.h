#ifndef _PARSER_H_
#define _PARSER_H_
#include<map>
#include "lexer.h"
#include "AST.h"
#include "error.h"
#include "symbol.h"
namespace Yan {

// + - * / intlitr EOF

class parser 
{
    public:
        using precedenceMap = std::map<TokenType,int>;
       // static precedenceMap initPrecedenceMap();
    public:
        parser(lexer& s,symbolTable& t);
        ~parser();
        Expr*primary();
        Program* parserProgram();
        FunctionDef* parserFuncDef(Identifier* identi);
        Declaration* parserDeclaration(Identifier* identi);
        CompousedStmt*parserCompoundStmt();
        PrintStmt* parserPrintStmt();
        BinaryOp* parserAssignExpr(Token var);
        FunctionCall* parserFuncCall(Token var);
       
        //EXPR
        Expr* sum();
        Expr* factor();
        Expr* cast();
        Expr* unary();
        Expr* postfix();
        // Expr* term();
        // Expr* group();


        


        OpType TokenType2ASTop(TokenType type);
       
    private:
        bool isTypeName();
        Type*baseType(storageClass* sclass);
        Type*Declarator(Type*type);
        
        //Token ralated function
        bool match(TokenType t);
        bool test(TokenType t);
        Token consume();
        void expect(TokenType t, const std::string& what);

    private:
        lexer& scan;
        
        symbolTable& symb;     

        //disable copy and assign
        parser(const parser&) = delete;
        parser& operator = (const parser&) = delete;
};

}
#endif