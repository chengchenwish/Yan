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
        parser(lexer& s);
        ~parser();
        
        Program* parserProgram();
        FunctionDef* parserFuncDef(Identifier* identi);
        Declaration* parserDeclaration(Identifier* identi);
        CompousedStmt*parserCompoundStmt();
        PrintStmt* parserPrintStmt();
        FunctionCall* parserFuncCall(Token var);
       
        //EXPR
        Expr* expr();
        Expr* assign();
        Expr* conditional();
        Expr* logicOr();
        Expr* logicAnd();
        Expr* bitOr();
        Expr* bitXor();
        Expr* bitAnd();
        Expr* equality();
        Expr* relational();
        Expr* shift();
        Expr* sum();
        Expr* mul();
        Expr* cast();
        Expr* unary();
        Expr* postfix();
        Expr* primary();
        
        // Expr* term();
        // Expr* group();        

       
    private:
        //Begin a block scope;
        symbolTable* enterScope(Scope kind);
        void leaveScope();

        bool isTypeName();
         bool findtypedef(const std::string& name );
        Type*baseType(storageClass* sclass);
        Type*Declarator(Type*type);

        //Token ralated function
        bool match(TokenType t);
        bool test(TokenType t);
        Token consume();
        void putBack(const Token& t){ scan.putBack(t);}
        Token peek(){ return scan.peektoken();}
        void expect(TokenType t, const std::string& what);

    private:
        lexer& scan;
        
        symbolTable* currentScop_;     

        //disable copy and assign
        parser(const parser&) = delete;
        parser& operator = (const parser&) = delete;
};

}
#endif