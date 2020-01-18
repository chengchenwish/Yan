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
        ASTnode*primary(token& t);
        ASTnode*binExpr(token& t, int ptp = 0);
        void statements();
        void assignmentStatement();
        void varDeclaration();
        void printStatement();
        void match(tokenType t, const std::string& what);

        int tokenType2ASTop(tokenType type);
        int getOpPrecedence(tokenType optype) const;
    private:
        lexer& scan;
        token currentToken;
        gen& codeGen;
        symbolTable& symb;
        
        static const precedenceMap opPrecedence;        

        //disable copy and assign
        parser(const parser&) = delete;
        parser& operator = (const parser&) = delete;
};

}
#endif