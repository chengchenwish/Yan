#ifndef _GEN_H_
#define _GEN_H_
#include "AST.h"
#include "symbol.h"
#include<array>
namespace Yan{

class gen:public Ivisitor
{
public:
    explicit gen(symbolTable& sym,const std::string& fileName = "a.s");
    ~gen();
    int genAST(ASTnode* root,int reg = -1);
    void generateCode(ASTnode* root);
    void freeAllReg();
    int allocReg();
    void freeReg(int reg);
    virtual void visit(BinaryOp* node);
    int genBinaryOp(BinaryOp* root,int reg=-1);
    void genPreamble();
    void genpostamble();
    int loadValue(int value);
    //operator
    int genAdd(int r1, int r2);
    int genSub(int r1, int r2);
    int genMul(int r1, int r2);
    int genDiv(int r1, int r2);
    int  genGE(int r1, int r2);
    int  genEQ(int r1, int r2);
    int  genNE(int r1, int r2);
    int  genGT(int r1, int r2);
    int  genLT(int r1, int r2);
    int  genLE(int r1, int r2);
    int  genCmp(int r1, int r2, const std::string& how);

    void printint(int r);
    void genGlobalSymbol(std::string& s);
    int loadGlobal(std::string& text);
    int storeGlobal(int reg,std::string& text);
private:
    static constexpr std::size_t  regNum = 4;
    
    static char const *registerList[regNum];
    static char const *breglist[regNum];
    static std::array<int,regNum> freeRegMark;
    
    symbolTable& symb;
    std::string outfileName; 
    std::fstream outfstream;

    //disable copy and assgin
    gen(const gen&) = delete;
    gen& operator = (const gen&) = delete;     

    
};
}
#endif
