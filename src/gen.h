#ifndef _GEN_H_
#define _GEN_H_
#include "AST.h"
#include "symbol.h"
#include<array>
#include<sstream>
namespace Yan{

class gen:public Ivisitor
{
public:
    explicit gen(const std::string& fileName = "a.s");
    ~gen();
    //impliment Ivistor
    virtual void visit(BinaryOp* node)override;
    virtual void visit(Identifier* node) override;
    virtual void visit(ConstantValue* node)override;
    virtual void visit(FunctionDef* node)override;
    virtual void visit(Declaration* node)override;
    virtual void visit(Program* node)override{};
   virtual void visit(IfStmt* node)override;
   virtual void visit(PrintStmt* node)override;
   virtual void visit(CompousedStmt* node)override;
   virtual void visit(FunctionCall* node)override;
    virtual void visit(JumpStmt* node)override {}
   virtual void visit(ReturnStmt* node)override {}
   virtual void visit(UnaryOp* node)override{}
   virtual void visit(ConditionExpr* node)override{};

    void genProgram(Program* node);
    void genLvalue(Identifier*node);
   void loardArgs(Identifier* node, int index);

private:
    void storeLValue(Type *ty);
    void loadLValue(Identifier *node);
    //operator
    void genAdd();
    void genSub();
    void genMul();
    void genDiv();
    void  genGE();
    void genEQ();
    void  genNE();
    void  genGT();
    void  genLT();
    void  genLE();
    void  genCmp(const std::string& how);

private:
    void emit(std::string inst, std::string dest, std::string source);
    void emit(std::string inst);

    //1 byte
    static const std::vector<std::string> argReg1;
    //2 byte
    static const std::vector<std::string> argReg2;
    //4 byte
    static const std::vector<std::string> argReg4;
    //8 byte
    static const std::vector<std::string> argReg8;
    
    std::string outfileName; 
    std::fstream outfstream;

    //disable copy and assgin
    gen(const gen&) = delete;
    gen& operator = (const gen&) = delete;     

    
};
}
#endif
