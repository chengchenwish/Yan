#ifndef _AST_H_
#define _AST_H_
#include<memory>
#include<list>
#include<vector>
#include"lexer.h"
#include "type.h"

namespace Yan{

struct Expr;
struct BinaryOp;
struct Identifier;
struct ConstantValue;
struct FunctionDef;
struct Declaration;
struct Program;

struct IfStmt;
struct PrintStmt;
struct CompousedStmt;
struct FunctionCall;
struct JumpStmt;
struct ReturnStmt;
struct UnaryOp;
struct ConditionExpr;


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
   virtual void visit(FunctionCall* node) = 0;
   virtual void visit(JumpStmt* node)=0;
   virtual void visit(ReturnStmt* node) = 0;
   virtual void visit(UnaryOp*node)=0;
   virtual void visit(ConditionExpr* node) = 0;

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
    CompousedStmt();
    virtual ~CompousedStmt();

    static CompousedStmt* create();
    void addStmt(Stmt* stmt);
    virtual void accept(Ivisitor*v) override;  

    StmtList stmtlist_;
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
    virtual void accept(Ivisitor* v)override{ v->visit(this);}
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
    Expr(Type* ty): type_(ty){}
    Expr(): type_(nullptr){}
    virtual ~Expr(){};
    Type* type_;
};
enum class OpType
{ 
    //BinaryOP
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_ASSIGN,
    OP_ASSPLUS,//+=
    OP_ASSSUB, // -=
    OP_ASSMUL, //*=
    OP_ASSDIV,// /=
    OP_ASSMOD, // %=
    OP_SHLASSIGN,// <<=
    OP_SHRASSIGN,
    OP_ANDASSIGN,
    OP_ORASSIGN,
    OP_XORASSIGN,
    OP_LSHIFT,//<<
    OP_RSHIFT,//>>

    OP_COMMA,// ,

    OP_EQ,
    OP_NE,
    OP_GT,
    OP_LT,
    OP_GE,
    OP_LE,

    OP_LOGICAND,
    OP_LOGICOR,
    OP_BITOR,
    OP_BITXOR,
    OP_BITAND,

    //UnaryOP
    OP_CAST,

    OP_UNKOWN 
};
struct  BinaryOp: public Expr
{

    //svirtual void accept(Ivisitor*v);
    BinaryOp(OpType op, Expr*left, Expr* right, Type* ty);
    virtual ~BinaryOp();  
    virtual void accept(Ivisitor*v) override;
    static BinaryOp* create(OpType op, Expr*left, Expr* right, Type* ty = nullptr);

    OpType   op;
    Expr*  left;
    Expr*  right;

};
/*
// unary = ("+" | "-" | "*" | "&" | "!" | "~")? cast

//       | ("++" | "--") unary

//       | postfix
*/
struct UnaryOp: public Expr
{
    UnaryOp(OpType op, Expr*operand,Type* ty):Expr(ty),op_(op),operand_(operand){}
    static UnaryOp*create(OpType op, Expr*operand,Type*ty){ return new UnaryOp(op,operand,ty);}
    virtual void accept(Ivisitor*v){v->visit(this);}
    OpType op_;
    Expr* operand_;

};
struct ConditionExpr: public Expr
{  
    ConditionExpr(Expr* cond, Expr* then, Expr* els )
      : cond_(cond), trueExpr_(then),  falseExpr_(els) {}
    virtual void accept(Ivisitor*v) override{ v->visit(this);}
    static ConditionExpr* create(Expr* cond, Expr* then, Expr* els);
    Expr* cond_;
    Expr* trueExpr_;
    Expr* falseExpr_;

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
struct FunctionCall:public Stmt
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
    
    std::string name_;
    bool isLocal_;
    //only for local varibale
    int offset_ = 0;
    virtual void accept(Ivisitor*v) override{v->visit(this);}
    void setoffset(int offset){ offset_ = offset;}
    Identifier(const std::string& name, Type* type, bool islocal):Expr(type),name_(name), isLocal_(islocal){}
    static Identifier* create(const std::string& name, Type* type, bool islocal){ return new Identifier(name,type,islocal);}
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
