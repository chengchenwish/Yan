#include "AST.h"
namespace Yan
{

void BinaryOp::accept(Ivisitor* v)
{
    v->visit(this);
}

BinaryOp::BinaryOp(int op, BinaryOp*left, BinaryOp* right, int value)
{
    this->op = op;
    this->intValue = value;
    this->left = left;
    this->right = right;
}
}