#include "AST.h"
#include<assert.h>
namespace Yan
{
template<typename T>
class evaluator: public Ivisitor
{
public:
   virtual void visit(BinaryOp* node)override;
   virtual void visit(Identifier* node)override{ assert(0);}
   virtual void visit(ConstantValue* node)override ;
   virtual void visit(FunctionDef* node)override{ assert(0);}
   virtual void visit(Declaration* node)override{ assert(0);}
   virtual void visit(Program* node)override{ assert(0);}
   virtual void visit(IfStmt* node)override{ assert(0);}
   virtual void visit(PrintStmt* node)override{ assert(0);}
   virtual void visit(CompousedStmt* node)override{ assert(0);}
   virtual void visit(FunctionCall* node)override{ assert(0);}
   virtual void visit(JumpStmt* node)override { assert(0);}
   virtual void visit(ReturnStmt* node)override{ assert(0);}
   virtual void visit(UnaryOp*node)override;
   virtual void visit(ConditionExpr* node)override;
   T eval(Expr* node)
   {
       node->accept(this);
       return result;
   }

   T getResult(){ return result;}

private:
    T result;

};

}
#include "evaluator.cpp"