#ifndef _PARSER_H_
#define _PARSER_H_
#include<map>
#include "lexer.h"
#include "AST.h"
#include "error.h"
#include "symbol.h"
#include "evaluator.h"
namespace Yan {

// + - * / intlitr EOF
 using Declarator = std::pair<Type*,std::string>;

class parser 
{
   
    public:
        parser(lexer& s);
        ~parser();
        //Begin a block scope;
        void enterScope(Scope kind);
        void leaveScope();
        symbolTable* getSymbolTable(){ return currentScop_;}
        
        Program* parserProgram();
        FunctionDef* parserFuncDef(Identifier* identi);
        
        CompousedStmt*parserCompoundStmt();
        FunctionCall* parserFuncCall(Token var);
        IfStmt* parserIfStmt();
        Stmt* parserSingleStmt();
       
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
        
        //declaration
        Declaration* parserDeclaration(Identifier* identi);
        Type*baseType(storageClass* sclass);
        Declarator declarator(Type*type);
        Type* declarator_array(Type* type);
        Type* declarator_func(Type* type);
        Type* type_suffix(Type* type);
        Type* modifyBase(Type* type, Type* base,Type*new_base);

        Declarator parser_func_param();


        template<typename T>
        T constExpr()
        {
            evaluator<T> eval;
            return eval.eval(assign());
        }  

       
    private:


        bool isTypeName();
         bool findtypedef(const std::string& name );


        //Token ralated function
        bool match(TokenType t);
        bool test(TokenType t) const;   

        
        bool isOneOf(TokenType K1,TokenType K2)const
        {
            return test(K1)||test(K2);
        }
        template <typename... Ts>
        bool isOneOf(TokenType K1, TokenType K2, Ts... Ks) const {
        return test(K1) || isOneOf(K2, Ks...);
        }
        Token consume();
        void putBack(const Token& t){ scan.putBack(t);}
        Token peek(){ return scan.peektoken();}
        void expect(TokenType t, const std::string& what);

    private:
        lexer& scan;
        //Token currenttoken_;//used for error output
        
        symbolTable* currentScop_;     

        //disable copy and assign
        parser(const parser&) = delete;
        parser& operator = (const parser&) = delete;
};
//RAII scope
class selfScope
{
public:
    selfScope( parser& p, Scope kind): self(p)
    {        
        self.enterScope(kind);
    }
    ~selfScope()
    {
        self.leaveScope();
    }
private:

     parser& self;


};


}
#endif