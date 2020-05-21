#include "gen.h"
namespace Yan
{

    const std::vector<std::string> gen::argReg1 ={"dil", "sil", "dl", "cl", "r8b", "r9b"};
    const std::vector<std::string> gen::argReg2 ={"di", "si", "dx", "cx", "r8w", "r9w"};
    const std::vector<std::string> gen::argReg4 ={"edi", "esi", "edx", "ecx", "r8d", "r9d"};
    const std::vector<std::string> gen::argReg8 ={"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
   
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
            Info("Successfully generated :%s ",outfileName.c_str());

        }
    }
    /*
   void gen::emit(char* mt, ...)
   {
     // Replace "#" with "%%" so that vfprintf prints out "#" as "%".
    char buf[256];
    int i = 0;
    for (char *p = mt; *p; p++) {
       // assert(i < sizeof(buf) - 3);
        if (*p == '#') {
            buf[i++] = '%';
            buf[i++] = '%';
        } else {
            buf[i++] = *p;
        }
    }
    buf[i] = '\0';
    va_list args;
    va_start(args, mt);
    char outputstr[100];
    int col = sprintf(outputstr, buf, args);
    va_end(args);
    outfstream <<"\t"<< std::string(outputstr)<<std::endl;

   }*/
   void gen::emit(std::string inst, std::string dest, std::string source)
    {
        outfstream<<"\t"<<inst<<"\t"<<dest<<", "<<source<<"\n";
    }

    void gen::emit(std::string inst)
    {
        outfstream<<"\t"<<inst<<"\n";
    }

    //void gen::genLvalue(node* node);
    
    void gen::visit(BinaryOp* node)
    {
        
        //i++;
       Info("binary");
       Info("fff:%d",node->op);
       if(node->op == OpType::OP_ASSIGN)
       {
          // node->left->accept(this);
          genLvalue(static_cast<Identifier*>(node->left));
           node->right->accept(this);
                  
           emit("popq %rax");
           emit("popq %rbx");
           emit("movq %rax, (%rbx)");
           emit("pushq %rax");

       }
       else if(node->op ==OpType::OP_ADD)
       {
           node->left->accept(this);
           node->right->accept(this);
           genAdd();
       }
       else if(node->op == OpType::OP_SUBTRACT)
       {
           node->left->accept(this);
           node->right->accept(this);
           genSub();
       }
       else if(node->op == OpType::OP_MULTIPLY)
       {
           node->left->accept(this);
           node->right->accept(this);
           genMul();
       }
       else if(node->op == OpType::OP_DIVIDE)
       {
            node->left->accept(this);
            node->right->accept(this);
            genDiv();    
       }
 
    }
    

    void gen::genAdd()
    {
        emit("popq %rax");
        emit("popq %rbx");
        emit("addq %rbx, %rax");
        emit("pushq %rax");
    }
    void gen::genSub()
    {
        
        emit("popq %rdx");
        emit("popq %rax");
        emit("subq","%rdx","%rax");
        emit("pushq %rax");
    }
    void gen::genMul()
    {
        emit("popq %rdx");
        emit("popq %rax");

        emit("imulq","%rdx","%rax");
        emit("pushq %rax");
    }
    void gen::genDiv()
    {
        emit("popq %rdi");
        emit("popq %rax");
        emit("cqto");
        emit("idivq %rdi");
        emit("push %rax");
    }
   

   void gen::genGE()
    {
        genCmp("setge");
    }
     void gen::genEQ()
    {
        genCmp("sete");
    }
     void gen::genNE()
    {
        genCmp("setne");
    }
     void gen::genGT()
    {
        genCmp("setg");
    }
     void gen::genLT()
    {
        genCmp("setl");
    }
     void gen::genLE()
    {
        genCmp("setle");
    }
    void  gen::genCmp(const std::string& how)
    {
        emit("cmp", "rax", "rdx");
        std::string inst = how+" al";
        emit(inst);
        emit("movezb","rax","al");

    }

void gen::genProgram(Program* node)
{  
    //emit(".intel_syntax noprefix\n");
    emit(".LC0:");
    emit("\t.string \"%d\\n\"");
    emit(".text");
    emit(".global print");
    emit("print:");
    emit("pushq %rbp");
    emit("movq %rsp, %rbp");
    emit("subq $16, %rsp");
    emit("movq %rdi, -8(%rbp)");
    emit("movq -8(%rbp), %rax") ;
    emit("movq %rax, %rsi");
    emit("leaq .LC0(%rip), %rdi");
    emit("movq $0,%rax");
    emit("call printf@PLT");
    emit("nop");
    emit("leave");
    emit("ret");         

    for(auto& decl: node->decls_)
    {
        decl->accept(this);
       // visit(decl);
    }
}
                
   void gen::visit(ConstantValue* node)
   {
       emit("movq $" +std::to_string(node->ivalue_)+", %rax");
       Info(std::to_string(node->ivalue_).c_str());
       emit("pushq %rax");
   }
   void gen::visit(FunctionDef* node)
   {
           Info("FunctionDef");
           emit(".global "+node->identi_->name_);
           emit(node->identi_->name_+":");
           emit("pushq %rbp");
           emit("movq %rsp, %rbp");
           emit("subq $16, %rsp");

           //node->identi_->accept(this);
           node->body_->accept(this);
           emit("movq $0, %rax");
           emit("leave");
           emit("ret");

   }
   void gen::visit(Declaration* node)
   {

       Info("decalration");
      auto& name = node->obj_->name_;

      auto align = (node->obj_->type_)->getalign();
      auto size = node->obj_->type_->getsize();
      std::stringstream out;
      emit(".data");
      emit(".global "+name);

      out<<".lcomm "<<name<<","<<size;
      emit(out.str());
       
   }
  void  gen:: visit(Program* node)
  {

  }
 void  gen::visit(Identifier* node)
 {
     Info("identifier");
     //if(node)
     Info(node->name_.c_str());
     std::stringstream fm;
     fm<<"movq "<<node->name_<<"(%rip), %rax";
     emit(fm.str());
     emit("pushq %rax"); 
   //  Info(static_cast<FuncType*>(node->type_)->
 }
 void gen::genLvalue(Identifier*node)
 {
     std::stringstream fm;
     fm<<"leaq "<<node->name_<<"(%rip), %rax";
     emit(fm.str());
     emit("pushq %rax"); 
 }
  void  gen::visit(IfStmt* node)
  {

  }
  void  gen::visit(PrintStmt* node)
  {

  }
  void gen::visit(FunctionCall* node)
{
    Info("functionCall");
    auto arg = node->argList_.front();
    arg->accept(this);
    emit("popq %rax");
    emit("movq %rax, %rdi");
    emit("call "+node->designator_->name_);
}
void  gen::visit(CompousedStmt* node)
{
    for(auto& stmt: node->stmtlist_)
    {
        stmt->accept(this);
    }
    Info("compoused");
}

}