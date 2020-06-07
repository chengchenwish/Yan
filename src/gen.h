#ifndef _GEN_H_
#define _GEN_H_
#include "AST.h"
#include "symbol.h"
#include <array>
#include <sstream>
#include <assert.h>
#include<map>
namespace Yan
{

    class LvalueAddrGenerator;
    class gen : public Ivisitor
    {
        using LabelStack = std::stack<std::string>;
        using StringLitLableMap = std::map<std::string, std::string>;

    public:
        explicit gen(Scope* sc, const std::string &fileName = "a.s" );
        ~gen();
        std::string genLabe();
        void visitExpr(Expr* node){node->accept(this);}
        //impliment Ivistor
        virtual void visit(BinaryOp *node) override;
        virtual void visit(Identifier *node) override;
        virtual void visit(IntegerLiteral *node) override;
        virtual void visit(FunctionDef *node) override;
        virtual void visit(Declaration *node) override;
        virtual void visit(Program *node) override;
        virtual void visit(IfStmt *node) override;
        virtual void visit(LoopStmt *node) override;
        virtual void visit(CompousedStmt *node) override;
        virtual void visit(FunctionCall *node) override;
        virtual void visit(GotoStmt *node) override;
        virtual void visit(ReturnStmt *node) override;
        virtual void visit(UnaryOp *node) override;
        virtual void visit(ConditionExpr *node) override {}
        virtual void visit(StringLiteral *node) override;
        virtual void visit(BreakContinueStmt *node) override;
        virtual void visit(LabelStmt *node) override;

        void genAddr(Identifier *node);
        void genLvalue(Expr* node);
        void loardArgs(Identifier *node, int index);
        void checkCondition(Expr *node, std::string trueLabel, std::string falsedLabel);

    private:
        void storeLValue(Type *ty);
        void loadLValue(Identifier *node);
        void load(Type* ty);
        //operator
        void genAdd();
        void genSub();
        void genMul();
        void genDiv();
        void genCmp(const std::string &how, bool longType = false);

        void genStringLitLables();

    private:
        void emit(std::string inst);
        void emit(std::stringstream &inst);

        //1 byte
        static const std::vector<std::string> argReg1;
        //2 byte
        static const std::vector<std::string> argReg2;
        //4 byte
        static const std::vector<std::string> argReg4;
        //8 byte
        static const std::vector<std::string> argReg8;

        //used to avoid generating duplicated label
        static int labelseq;
        LabelStack breakLabels_;
        LabelStack continueLabels_;
        std::string functionName_;
        //
        StringLitLableMap strlitMap_;
        Scope* globalScope;

        std::string outfileName;
        std::fstream outfstream;

        LvalueAddrGenerator *addrGnerator_;

        //disable copy and assgin
        gen(const gen &) = delete;
        gen &operator=(const gen &) = delete;
    };

    class LvalueAddrGenerator : public Ivisitor
    {
    public:
        LvalueAddrGenerator(gen *instance) : generator_(instance) {}

    private:
        virtual void visit(BinaryOp *node) override { assert(0); }
        virtual void visit(Identifier *node) override { generator_->genAddr(node); }
        virtual void visit(IntegerLiteral *node) override { assert(0); }
        virtual void visit(FunctionDef *node) override { assert(0); }
        virtual void visit(Declaration *node) override { assert(0); }
        virtual void visit(Program *node) override { assert(0); }
        virtual void visit(IfStmt *node) override { assert(0); }
        virtual void visit(CompousedStmt *node) override { assert(0); }
        virtual void visit(FunctionCall *node) override { assert(0); }
        virtual void visit(GotoStmt *node) override { assert(0); }
        virtual void visit(ReturnStmt *node) override { assert(0); }
        virtual void visit(StringLiteral *node) override { assert(0); }
        virtual void visit(UnaryOp *node) override {assert(node->op_ == OpType::OP_DEREF); generator_->visitExpr(node->operand_); }
        virtual void visit(ConditionExpr *node) override { assert(0); }
        virtual void visit(LoopStmt *node) override { assert(0); }
        virtual void visit(BreakContinueStmt *node) override { assert(0); }
        virtual void visit(LabelStmt *node) override { assert(0); }
private:
        gen *generator_;
    };
} // namespace Yan
#endif
