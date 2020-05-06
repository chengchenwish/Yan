#ifndef _PARSER_H_
#define _PARSER_H_
#include<map>
#include "lexer.h"
#include "AST.h"
#include "gen.h"
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
        parser(lexer& s,gen& g,symbolTable& t);
        ~parser();
        BinaryOp*primary();
        BinaryOp*binExpr(int ptp = 0);
        Program* parserProgram();
        FunctionDef* parserFuncDef(Identifier* identi);
        Declaration* parserDeclaration(Identifier* identi);
        CompousedStmt*parserCompoundStmt();
        void statements();
        void assignmentStatement();
        void varDeclaration();
        void printStatement();
        void ifStatement();
        void compoundStatement();
        

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
            bool hasvule;
            token tempToken;
        } tmpToken;
        lexer& scan;
       // token currentToken;
        gen& codeGen;
        symbolTable& symb;
        
        static const precedenceMap opPrecedence;        

        //disable copy and assign
        parser(const parser&) = delete;
        parser& operator = (const parser&) = delete;
};

}
#endif