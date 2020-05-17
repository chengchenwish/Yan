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
        using precedenceMap = std::map<tokenType,int>;
       // static precedenceMap initPrecedenceMap();
    public:
        parser(lexer& s,symbolTable& t);
        ~parser();
        Expr*primary();
        Expr*binExpr(int ptp = 0);
        Program* parserProgram();
        FunctionDef* parserFuncDef(Identifier* identi);
        Declaration* parserDeclaration(Identifier* identi);
        CompousedStmt*parserCompoundStmt();
        PrintStmt* parserPrintStmt();
        BinaryOp* parserAssignExpr(token var);
        FunctionCall* parserFuncCall(token var);

        


        int tokenType2ASTop(tokenType type);
        int getOpPrecedence(tokenType optype) const;
       
    private:
        bool isTypeName();
        Type*baseType(storageClass* sclass);
        Type*Declarator(Type*type);
        //token ralated function
        bool match(tokenType t);
        bool test(tokenType t);
        token consume();
        void expect(tokenType t, const std::string& what);

    private:
        struct {
            bool hasvalue_;
            token token_;
        } cacheToken;
        lexer& scan;
        
        symbolTable& symb;
        
        static const precedenceMap opPrecedence;        

        //disable copy and assign
        parser(const parser&) = delete;
        parser& operator = (const parser&) = delete;
};

}
#endif