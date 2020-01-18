#include "gen.h"
namespace Yan
{
    char const *gen::registerList[gen::regNum] = {"%r8" ,"%r9","%r10","%r11"};
    //1 means freed, o means allocated
    std::array<int,gen::regNum> gen::freeRegMark ={1,1,1,1};
    gen::gen(symbolTable& sym, const std::string& fileName):outfileName(fileName),symb(sym)
    {
        outfstream.open(fileName.c_str(), std::ios::out);
        if(!outfstream.is_open())
        {
            ExitWithError("Open output file:%s fail", fileName.c_str());
         
        }

    }
    gen::~gen()
    {
        if ( outfstream.is_open())
        {
            outfstream.close();

        }
    }

    int gen::genAST(ASTnode* root,int reg)
    {

   
    int leftReg, rightReg;
    if(root->left) leftReg = genAST(root->left,-1);
    if(root->right) rightReg = genAST(root->right,leftReg);
    
    switch(root->op)
    {   
        case ASTop::A_ADD:
    
            return genAdd(leftReg, rightReg);
        case ASTop::A_DIVIDE:
            return genDiv(leftReg, rightReg);
        case ASTop::A_MULTIPLY:
            return genMul(leftReg, rightReg);
        case ASTop::A_SUBTRACT:
            return genSub(leftReg, rightReg);
        case ASTop::A_INTLIT:
            return loadValue(root->intValue);
        case ASTop::A_IDENTI:
            return (loadGlobal(symb.getSymbol(root->intValue).name));
        case A_LVIDENT:
            return (storeGlobal(reg, symb.getSymbol(root->intValue).name));
        case A_ASSIGN:
    // The work has already been done, return the result
        return (rightReg);
        default:
            
            std::cout<<" ERROR op"<<root->op<<std::endl;
            exit(1);
    }
    }
    void gen::generateCode(ASTnode* root)
    {
        int reg;
        genPreamble();
        reg = genAST(root);
        printint(reg);
        genpostamble();

    }
    void gen::freeAllReg()
    {
        for(int i=0;i<regNum; ++i)
        {
            freeRegMark[i]= 1;
        }
    }
    int gen::allocReg()
    {
       for (int i = 0; i<regNum; i++)
       {
           if (freeRegMark[i])
           {
               freeRegMark[i] = 0;
               return i;
           }
       }
       outfstream.flush();
       outfstream.close();
       ExitWithError("%s:No freed reg",__func__);
    }
    void gen::freeReg(int reg)
    {
        freeRegMark[reg] = 1;
    }
    void gen::genPreamble()
    {

        freeAllReg();
        outfstream<<"\t.text\n"
                   ".LC0:\n"
                    "\t.string\t\"%d\\n\"\n"
	                "printint:\n"
	                "\tpushq\t%rbp\n"
	                "\tmovq\t%rsp, %rbp\n"
	                "\tsubq\t$16, %rsp\n"
	                "\tmovl\t%edi, -4(%rbp)\n"
	                "\tmovl\t-4(%rbp), %eax\n"
	                "\tmovl\t%eax, %esi\n"
	                "\tleaq	.LC0(%rip), %rdi\n"
	                "\tmovl	$0, %eax\n"
	                "\tcall	printf@PLT\n"
	                "\tnop\n"
	                "\tleave\n"
	                "\tret\n"
	                "\n"
	                "\t.globl\tmain\n"
	                "\t.type\tmain, @function\n"
	                "main:\n"
	                "\tpushq\t%rbp\n"
	                "\tmovq	%rsp, %rbp\n";
    }

    void gen::genpostamble()
    {
        outfstream << "\tmovl $0, %eax\n"
	                 "\tpopq %rbp\n"
	                 "\tret\n";
    }

    int gen::loadValue(int value)
    {
        int reg;
        reg = allocReg();
        outfstream<<"\tmovq\t$"<<value<<", "<<registerList[reg]<<"\n";
        return reg;
    }
    int gen::genAdd(int r1, int r2)
    {
        outfstream<<"\taddq\t"<<registerList[r1]<<", "<<registerList[r2]<<"\n";
        freeReg(r1);
        return r2;
    }
    int gen::genSub(int r1, int r2)
    {
        outfstream<<"\tsubq\t"<<registerList[r2]<<", "<<registerList[r1]<<"\n";
        freeReg(r2);
        return r1;
    }
    int gen::genMul(int r1, int r2)
    {
        outfstream<<"\timulq\t"<<registerList[r1]<<", "<<registerList[r2]<<"\n";
        freeReg(r1);
        return r2;
    }
    int gen::genDiv(int r1, int r2)
    {
        outfstream<<"\tmovq\t"<<registerList[r1]<<","<<"%rax\n";
        outfstream<<"\tcqo\n";
        outfstream<<"\tidivq\t"<<registerList[r2]<<"\n";
        outfstream<<"\tmovq\t%rax, "<<registerList[r1]<<"\n";
        freeReg(r2);
        return r1;

    }
    void  gen::printint(int r)
    {   
        outfstream<<"\tmovq\t"<<registerList[r]<<","<<"%rdi\n";
        outfstream<<"\tcall\tprintint\n";

        freeReg(r);

    }
    void gen::genGlobalSymbol(std::string& s)
    {
        outfstream<<"\t.comm\t"<<s<<",8,8\n";
    }

    int gen::loadGlobal(std::string& text)
    {
        int r = allocReg();
        outfstream<<"\tmovq\t"<<text<<"(%rip),"<<registerList[r]<<"\n";
        return r;
    }
    int gen::storeGlobal(int reg, std::string& text)
    {
        outfstream<<"\tmovq\t"<<registerList[reg]<<","<<text<<"(%rip)\n";
       return reg;
    }
	
}