#include "AST.h"
namespace Yan
{

// void BinaryOp::accept(Ivisitor* v)
// {
//     v->visit(this);
// }

BinaryOp::BinaryOp(int op, Expr*left, Expr* right)
{
    this->op = op;
    this->left = left;
    this->right = right;
}
}