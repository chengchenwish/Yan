#include "gen.h"
namespace Yan
{
    char const *gen::registerList[gen::regNum] = {"%r8" ,"%r9","%r10","%r11"};
    std::array<int,gen::regNum> gen::freeRegMark ={1,1,1,1};
    gen::gen(const std::string& fileName):outfileName(fileName)
    {
        outfstream.open(fileName.c_str(), std::ios::out);
        if(!outfstream.is_open())
        {
            std::cout<<"Error open output file failed"<<std::endl;
            exit(1);
        }

    }
    gen::~gen()
    {
        if ( outfstream.is_open())
        {
            outfstream.close();

        }
    }

    int gen::genAST(ASTnode* root)
    {

   
    int leftReg, rightReg;
    if(root->left) leftReg = genAST(root->left);
    if(root->right) rightReg = genAST(root->right);
    
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
       std::cout<<"error no freed reg"<<std::endl;
       exit(1);
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

 
	
}