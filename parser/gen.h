#ifndef _GEN_H_
#define _GEN_H_
#include "AST.h"
#include<array>
namespace Yan{

class gen 
{
public:
    explicit gen(const std::string& fileName = "a.s");
    ~gen();
    int genAST(ASTnode* root);
    void generateCode(ASTnode* root);
    void freeAllReg();
    int allocReg();
    void freeReg(int reg);

    void genPreamble();
    void genpostamble();
    int loadValue(int value);
    int genAdd(int r1, int r2);
    int genSub(int r1, int r2);
    int genMul(int r1, int r2);
    int genDiv(int r1, int r2);
    void printint(int r);
private:
    static constexpr std::size_t  regNum = 4;
    
    static char const *registerList[regNum];
    static std::array<int,regNum> freeRegMark;
    
    
    std::string outfileName; 
    std::fstream outfstream;

    //disable copy and assgin
    gen(const gen&) = delete;
    gen& operator = (const gen&) = delete;     

    
};
}
#endif
