#ifndef _AST_H_
#define _AST_H_
#include<memory>
#include<list>
#include<vector>
#include"lexer.h"
#include "type.h"

namespace Yan{

class Expr;
class BinaryOp;
class Identifier;
class ConstantValue;
class FunctionDef;
class Declaration;
class Program;

class IfStmt;
class PrintStmt;
class CompousedStmt;
class AssginStmt;
class FunctionCall;
class JumpStmt;
class ReturnStmt;


class Ivisitor 
{
public:
   virtual void visit(BinaryOp* node) = 0;
   virtual void visit(Identifier* node) = 0;
   virtual void visit(ConstantValue* node) = 0;
   virtual void visit(FunctionDef* node) = 0;
   virtual void visit(Declaration* node) = 0;
   virtual void visit(Program* node) = 0;
   virtual void visit(IfStmt* node) = 0;
   virtual void visit(PrintStmt* node) = 0;
   virtual void visit(CompousedStmt* node) = 0;
   virtual void visit(AssginStmt* node) = 0;
   virtual void visit(FunctionCall* node) = 0;
   virtual void visit(JumpStmt* node)=0;
   virtual void visit(ReturnStmt* node) = 0;

};

struct  Node 
{
    Node(){};
    virtual void accept(Ivisitor*v) = 0;
    virtual ~Node(){};
};
//
struct Stmt : public Node
{
    Stmt(){};
    virtual ~Stmt(){};
};

using StmtList = std::list<Stmt*>;
struct CompousedStmt:public Stmt
{
    CompousedStmt(){};
    static CompousedStmt* create(){return new CompousedStmt();}
    void addStmt(Stmt* stmt){stmtlist_.push_back(stmt);}
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
  
    virtual ~CompousedStmt(){};
    StmtList stmtlist_;
};

struct  AssginStmt: public Stmt
{
    AssginStmt(int id, Expr* expr):id_(id),expr_(expr){};
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
    int id_;//symbol slot
    Expr* expr_;
};
struct  PrintStmt: public Stmt
{
    PrintStmt(Expr*expr):expr_(expr){}
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
    static PrintStmt* create(Expr*expr){ return new PrintStmt(expr);}
    Expr* expr_;
    
};
struct IfStmt: public Stmt
{  
    IfStmt(Expr* cond, Stmt* then, Stmt* els = nullptr)
      : cond_(cond), then_(then), else_(els) {}
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
    Expr* cond_;
    Stmt* then_;
    Stmt* else_;

};
struct ReturnStmt:public Stmt
{
    ReturnStmt(Expr*expr):expr_(expr){}
    static ReturnStmt* create(Expr* expr){ return new ReturnStmt(expr);}
    virtual void accept(Ivisitor* v){ v->visit(this);}
    Expr* expr_;
};
struct JumpStmt:public Stmt
{
    JumpStmt(std::string label):label_(label){}
    static JumpStmt*create(std::string label){ return new JumpStmt(label);}
     virtual void accept(Ivisitor* v){ v->visit(this);}

    std::string label_;
};

struct Expr : public Stmt
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
      //  A_INTLIT,
        A_LVIDENT,
        A_ASSIGN,
       /// A_IDENTI,
        A_EQ,
        A_NE,
        A_GT,
        A_LT,
        A_GE,
        A_LE
    };
    
    //svirtual void accept(Ivisitor*v);
    BinaryOp(int op, Expr*left, Expr* right);
    virtual ~BinaryOp(){};  
    virtual void accept(Ivisitor*v) override{ v->visit(this);} 

    int    op;
    Expr*  left;
    Expr*  right;
   // int intValue;
};

struct ConstantValue: public Expr 
{
    ConstantValue(int value):ivalue_(value){}
    static  ConstantValue*create(int value){ return new ConstantValue(value);}
    union 
    {    
        int ivalue_;
        std::string* svalue_;
    };
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
};
struct FunctionCall:public Expr
{
    FunctionCall(Identifier* designator):designator_(designator){}
    static FunctionCall* create(Identifier* designator){ return new FunctionCall(designator);}
    void addArg(Expr* arg){ argList_.push_back(arg);}
    virtual void accept(Ivisitor*v)override{v->visit(this);}
    Identifier* designator_;
    std::vector<Expr*> argList_;

};
//represent a varaible 
struct Identifier :public Expr
{
    Type* type_;
    std::string name_;
    virtual void accept(Ivisitor*v) override{v->visit(this);}
    Identifier(const std::string& name, Type* type):type_(type),name_(name){}
    static Identifier* create(const std::string& name, Type* type){ return new Identifier(name,type);}
};
//using ExtDecl = Node;
struct FunctionDef:Node
{
    FunctionDef(Identifier* identi): identi_(identi){}
    virtual ~FunctionDef()override{}
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
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
//using InitList = std::list<Initializer>;
struct Declaration: public Stmt
{
    Declaration(Identifier* identi): obj_(identi){}
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
    static Declaration* create(Identifier* identi){ return new Declaration(identi);}
    Identifier* obj_;
    //a {1,2,4}
   // InitList inits_;

};

using ExtDeclList = std::vector<Node*>;
struct Program : Node
{
    Program(){}
    virtual ~Program(){}
    void accept(Ivisitor* v)override{ v->visit(this);}
    static Program* create(){ return new Program();}
    void add(Node* decl){ decls_.push_back(decl);}

    ExtDeclList decls_;

};





};
#endif
