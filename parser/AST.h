#ifndef _AST_H_
#define _AST_H_
#include<memory>
#include<list>
#include<vector>
#include"lexer.h"
#include "type.h"
namespace Yan{
class Ivisitor;
class Expr;
struct  Node
{
    Node(){};
    //virtual void accept(Ivisitor*v) = 0;
    virtual ~Node(){};
};

struct Stmt : public Node
{
    Stmt(){};
    virtual ~Stmt(){};
};
using StmtList = std::list<Stmt*>;
struct CompousedStmt:public Stmt
{
    CompousedStmt(){};
    static CompousedStmt* create(){new CompousedStmt();}
    void addStmt(Stmt* stmt){stmtlist_.push_back(stmt);}
  
    virtual ~CompousedStmt(){};
    StmtList stmtlist_;
};
struct  DeclStmt: public Stmt
{
  
};
struct  AssginStmt: public Stmt
{
    AssginStmt(int id, Expr* expr):id_(id),expr_(expr){};
    int id_;//symbol slot
    Expr* expr_;
};
struct  PrintStmt: public Stmt
{
    PrintStmt(Expr*expr):expr_(expr){}
    Expr* expr_;
    
};
struct IfStmt: public Stmt
{  
    IfStmt(Expr* cond, Stmt* then, Stmt* els = nullptr)
      : cond_(cond), then_(then), else_(els) {}

    virtual void visit(Ivisitor* v);
    Expr* cond_;
    Stmt* then_;
    Stmt* else_;

};

struct Expr : public Node
{
    Expr(){};
    virtual ~Expr(){};
};
struct  BinaryOp: public Expr
{
    enum OpType
    {   A_ADD,
        A_SUBTRACT,
        A_MULTIPLY,
        A_DIVIDE,
        A_INTLIT,
        A_LVIDENT,
        A_ASSIGN,
        A_IDENTI,
        A_EQ,
        A_NE,
        A_GT,
        A_LT,
        A_GE,
        A_LE
    };
    
    virtual void accept(Ivisitor*v);
    BinaryOp(int op, BinaryOp*left, BinaryOp* right, int value);
    virtual ~BinaryOp(){};   

    int    op;
    BinaryOp*  left;
    BinaryOp*  right;
    int intValue;
};
struct Identifier :Node
{
    Type* type_;
    std::string name_;
    Identifier(const std::string& name, Type* type):type_(type),name_(name){}
    static Identifier* create(const std::string& name, Type* type){ new Identifier(name,type);}
};
using ExtDecl = Node;
struct FunctionDef:ExtDecl
{
    FunctionDef(Identifier* identi): identi_(identi){}
    virtual ~FunctionDef()override{}
    static FunctionDef* create(Identifier* identi){ return new FunctionDef(identi);}
    void setBody(CompousedStmt* body){body_ = body;}
    Identifier* identi_;
    //LabelStmt* returnLebl_;
    CompousedStmt* body_;
};
struct Initializer
{
    Type* type_;
    Expr* expr_;
};
using InitList = std::list<Initializer>;
struct Declaration:ExtDecl
{
    Declaration(Identifier* identi): obj_(identi){}
    static Declaration* create(Identifier* identi){ return new Declaration(identi);}
    Identifier* obj_;
    //a {1,2,4}
    InitList inits_;

};

using ExtDeclList = std::vector<ExtDecl*>;
struct Program : Node
{
    Program(){}
    virtual ~Program(){}
    //void accept(Ivisitor* v)override{}
    static Program* create(){ return new Program();}
    void add(ExtDecl* decl){ decls_.push_back(decl);}

    ExtDeclList decls_;

};

class Ivisitor 
{
public:
   virtual void visit(BinaryOp* node) = 0;
};


};
#endif
