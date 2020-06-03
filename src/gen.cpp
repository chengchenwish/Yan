#include "gen.h"
namespace Yan
{

    const std::vector<std::string> gen::argReg1 = {"%dil", "%sil", "%dl", "%cl", "%r8b", "%r9b"};
    const std::vector<std::string> gen::argReg2 = {"%di", "%si", "%dx", "%cx", "%r8w", "%r9w"};
    const std::vector<std::string> gen::argReg4 = {"%edi", "%esi", "%edx", "%ecx", "%r8d", "%r9d"};
    const std::vector<std::string> gen::argReg8 = {"%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"};

    int gen::labelseq = 1;
    gen::gen(const std::string &fileName) : outfileName(fileName)
    {
        outfstream.open(fileName.c_str(), std::ios::out);
        if (!outfstream.is_open())
        {
            ExitWithError("Open output file:%s fail", fileName.c_str());
        }
    }
    gen::~gen()
    {
        if (outfstream.is_open())
        {
            outfstream.close();
            Info("Successfully generated :%s ", outfileName.c_str());
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

    void gen::emit(std::string inst)
    {
        outfstream << "\t" << inst << "\n";
    }
    void gen::emit(std::stringstream &inst)
    {
        outfstream << "\t" << inst.str() << std::endl;
    }
    std::string gen::genLabe()
    {
        std::stringstream l;
        l << ".L" << labelseq++;
        return l.str();
    }

    //store value from register to statck
    void gen::storeLValue(Type *ty)
    {
        emit("popq %rax");
        emit("popq %rbx");
        std::stringstream fm;

        if (ty->getsize() == 2)
        {
            emit("mov %eax, (%rbx)");
        }
        else if (ty->getsize() == 4)
        {
            emit("movl %eax, (%rbx)");
        }
        else if (ty->getsize() == 8)
        {
            emit("movq %rax, (%rbx)");
        }
    }
    //load value from stack to register

    void gen::loadLValue(Identifier *node)
    {

        std::stringstream fm;

        if (node->type_->getsize() == 2)
        {
            fm << "mov  -" << node->offset_ << "(%rbp) , "
               << "%eax";
        }
        else if (node->type_->getsize() == 4)
        {
            fm << "movl  -" << node->offset_ << "(%rbp) , "
               << "%eax";
        }
        else if (node->type_->getsize() == 8)
        {
            fm << "movq  -" << node->offset_ << "(%rbp) , "
               << "%rax";
        }

        emit(fm.str());
        emit("pushq %rax");
    }

    void gen::visit(BinaryOp *node)
    {

        //i++;
        Info("binary");
        Info("fff:%d", node->op);
        if (node->op == OpType::OP_ASSIGN)
        {
            // node->left->accept(this);
            genLvalue(static_cast<Identifier *>(node->left));
            node->right->accept(this);
            storeLValue(node->type_);

            //emit("pushq %rax");
        }
        else if (node->op == OpType::OP_ADD)
        {
            node->left->accept(this);
            node->right->accept(this);
            genAdd();
        }
        else if (node->op == OpType::OP_SUBTRACT)
        {
            node->left->accept(this);
            node->right->accept(this);
            genSub();
        }
        else if (node->op == OpType::OP_MULTIPLY)
        {
            node->left->accept(this);
            node->right->accept(this);
            genMul();
        }
        else if (node->op == OpType::OP_DIVIDE)
        {
            node->left->accept(this);
            node->right->accept(this);
            genDiv();
        }
        else if (node->op == OpType::OP_MOD)
        {
        }
        else if (node->op == OpType::OP_GT)
        {
            node->left->accept(this);
            node->right->accept(this);
            //Todo Check node type if long/long
            genCmp("setg");
        }
        else if (node->op == OpType::OP_LT)
        {
            node->left->accept(this);
            node->right->accept(this);
            genCmp("setle");
        }
        else if (node->op == OpType::OP_EQ)
        {
            node->left->accept(this);
            node->right->accept(this);
            genCmp("sete");
        }
        else if (node->op == OpType::OP_NE)
        {
            node->left->accept(this);
            node->right->accept(this);
            genCmp("setne");
        }
        else if (node->op == OpType::OP_GE)
        {
            node->left->accept(this);
            node->right->accept(this);
            genCmp("setge");
        }
        else if (node->op == OpType::OP_LE)
        {
            node->left->accept(this);
            node->right->accept(this);
            genCmp("setle");
            ;
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
        emit("subq %rdx , %rax");
        emit("pushq %rax");
    }
    void gen::genMul()
    {
        emit("popq %rdx");
        emit("popq %rax");

        emit("imulq %rdx , %rax");
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

    void gen::genCmp(const std::string &how, bool longType)
    {
        emit("popq %rdx");
        emit("popq %rax");
        if (longType)
        {
            emit("cmp %rdx, %rax");
        }
        emit("cmp %edx, %eax");
        std::string inst = how + " %al";
        emit(inst);
        emit("movzbq %al, %rax");
        emit("pushq %rax");
    }

    void gen::visit(IntegerLiteral *node)
    {
        emit("movq $" + std::to_string(node->value_) + ", %rax");
        Info(std::to_string(node->value_).c_str());
        emit("pushq %rax");
    }
    void gen::visit(FunctionDef *node)
    {
        Info("FunctionDef");
        functionName_ = node->identi_->name_;
        emit(".global " + node->identi_->name_);
        emit(node->identi_->name_ + ":");
        emit("pushq %rbp");
        emit("movq %rsp, %rbp");
        std::stringstream fm;
        fm << "subq $" << node->getStackSize() << ", %rsp";
        emit(fm.str());
        //   emit("subq $24, %rsp");
        int index = 0;
        for (auto &arg : static_cast<FuncType *>(node->identi_->type_)->getParam())
        {
            loardArgs(arg, index++);
        }
        //    emit("movq %rdi , -4(%rbp)");
        //    emit("movq %rsi, -8(%rbp)");

        //node->identi_->accept(this);
        node->body_->accept(this);
        emit("movq $0, %rax");
        emit("leave");
        emit("ret");
    }
    void gen::loardArgs(Identifier *node, int index)
    {
        Info(__func__);
        auto size = node->type_->getsize();
        auto offset = node->offset_;
        std::stringstream fm;
        switch (size)
        {
        case 1:
            fm << "movb " << argReg1[index] << ", -" << offset << "(%rbp)";
            break;
        case 2:
            fm << "mov " << argReg2[index] << ", -" << offset << "(%rbp)";
            break;
        case 4:
            fm << "movl " << argReg4[index] << ", -" << offset << "(%rbp)";
            break;
        case 8:
            fm << "movq " << argReg8[index] << ", -" << offset << "(%rbp)";
            break;
        default:
            ExitWithError("Unsupported type");
            break;
        }
        emit(fm.str());
    }
    void gen::visit(Declaration *node)
    {

        Info("decalration");
        if (node->obj_->isLocal_)
        {
            return;
        }

        auto &name = node->obj_->name_;

        auto align = (node->obj_->type_)->getalign();
        auto size = node->obj_->type_->getsize();
        std::stringstream out;
        emit(".data");
        emit(".global " + name);

        out << ".lcomm " << name << "," << size;
        emit(out.str());
    }
    void gen::visit(Identifier *node)
    {
        Info("identifier");
        //if(node)
        Info(node->name_.c_str());
        std::stringstream fm;
        if (node->isLocal_)
        {
            loadLValue(node);
        }
        else
        {

            fm << "movq  " << node->name_ << "(%rip), %rax";
            emit(fm.str());
            emit("pushq %rax");
        }

        //  Info(static_cast<FuncType*>(node->type_)->
    }
    void gen::genLvalue(Identifier *node)
    {
        std::stringstream fm;
        if (node->isLocal_)
        {
            Info("name:%s offset :%d", node->name_.c_str(), node->offset_);
            fm << "leaq  -" << node->offset_ << "(%rbp) , "
               << "%rax";
            emit(fm);
            emit("pushq %rax");
        }
        else
        {

            fm << "leaq " << node->name_ << "(%rip), %rax";
            emit(fm.str());
            emit("pushq %rax");
        }
    }
    void gen::visit(IfStmt *node)
    {
        node->cond_->accept(this);
        emit("popq %rax");
        emit("cmp $0, %rax");
        // static int seq = 0;
        auto elsLabel = genLabe();
        auto elsEndLabel = genLabe();
        emit("jz " + elsLabel);
        node->then_->accept(this);
        emit("jmp " + elsEndLabel);
        emit(elsLabel + " :");
        if (node->else_)
            node->else_->accept(this);

        emit(elsEndLabel + ":");
    }
    void gen::visit(LoopStmt *node)
    {

        auto startLabel = genLabe();
        auto endLabel = genLabe();
        breakLabels_.push(endLabel);
        continueLabels_.push(startLabel);
        if (node->postcheck_)
        {
            emit("jmp " + startLabel);
        }
        else
        {
            checkCondition(node->cond_, startLabel, endLabel);
        }
        emit(startLabel + ":");
        node->then_->accept(this);
        checkCondition(node->cond_, startLabel, endLabel);
        emit(endLabel + ":");
        breakLabels_.pop();
        continueLabels_.pop();
    }
    void gen::visit(BreakContinueStmt *node)
    {
        if (node->kind_ == BreakContinueStmt::kBreak)
        {
            if (breakLabels_.empty())
            {
                ExitWithError("break doesn't in a loop or switch statment");
            }
            emit("jmp " + breakLabels_.top());
        }
        else
        {
            if (continueLabels_.empty())
            {
                ExitWithError("contine doesn't in a loop or switch statment");
            }
            emit("jmp " + continueLabels_.top());
        }
    }
    void gen::checkCondition(Expr *node, std::string trueLabel, std::string falsedLabel)
    {
        node->accept(this);
        emit("popq %rax");
        emit("cmp $0, %rax");
        emit("jz " + falsedLabel);
        emit("jmp " + trueLabel);
    }

    void gen::visit(GotoStmt *node)
    {
        std::stringstream s;
        s << "jmp .L." << functionName_ << "." << node->label_;
        emit(s);
    }

    void gen::visit(LabelStmt *node)
    {
        std::stringstream s;
        s << ".L." << functionName_ << "." << node->label_ << ":";
        emit(s);
    }
    void gen::visit(FunctionCall *node)
    {
        static int seq = 0;
        Info("functionCall");

        auto &args = node->argList_;
        for (int i = 0; i < args.size(); i++)
        {
            args[i]->accept(this);
            emit("popq %rax");
            emit("movq %rax, " + argReg8[i]);
        }
        // align RSP to a 16 byte boundary before
        // calling a function because it is an ABI requirement.
        emit("movq %rsp, %rax");
        emit("andq $15, %rax");
        auto labe = genLabe();
        auto labe1 = genLabe();
        emit("jnz " + labe);
        emit("mov $0,%rax");
        emit("call " + node->designator_->name_);
        emit("jmp " + labe1);
        emit(labe + ":");
        emit("subq $8,%rsp");
        emit("movq $0,%rax");
        emit("   call " + node->designator_->name_);
        emit(" addq $8,%rsp");
        emit(labe1 + ":");
    }
    void gen::visit(CompousedStmt *node)
    {
        for (auto &stmt : node->stmtlist_)
        {
            stmt->accept(this);
        }
        Info("compoused");
    }
    void gen::visit(Program *node)
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
        emit("movq -8(%rbp), %rax");
        emit("movq %rax, %rsi");
        emit("leaq .LC0(%rip), %rdi");
        emit("movq $0,%rax");
        emit("call printf@PLT");
        emit("nop");
        emit("leave");
        emit("ret");

        for (auto &decl : node->decls_)
        {
            decl->accept(this);
            // visit(decl);
        }
    }
} // namespace Yan