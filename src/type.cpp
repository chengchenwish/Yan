#include "type.h"
namespace Yan
{
  
Type::Type(int size, int align, TypeKind kind, bool incomplete):size_(size),
align_(align), kind_(kind),isIncomplete_(incomplete)
{

}

Type::TypeKind Type::getType()
{
    return kind_;
}
int Type::getalign()const
{
    return align_;
}
int Type:: getsize()const 
{
    return size_;
}
std::string Type::tostring()
{
    switch(kind_)
    {
        case T_VOID: return "void";

        case T_BOOL: return "bool";
        case T_CHAR: return "char";
        case T_SHORT:return "short";
        case T_INT: return "int";
        case T_LONG: return "long";

        case T_ENUM: return "enum";
        case T_PTR: return "ptr";
        case T_ARRAY:return "array";
        case T_STRUCT:return "struct";
        case T_FUNC: return "function";
    }
}


PtrType* PtrType::create(Type* base)
{ 
    return new PtrType(base);
}

PtrType::PtrType(Type* base):Type(8,8,T_PTR),baseType_(base)
{

}

ArrayType* ArrayType::create(Type*base, int len)
{ 
    return new ArrayType(base,len);
}

 ArrayType::ArrayType(Type*base, int len):Type(base->getsize()*len,base->getalign(),T_ARRAY),
 baseType_(base),len_(len)
 {

 }

void FuncType::addParam(Identifier* param)
{
    paramList_.push_back(param);
}
std::vector<Identifier*>& FuncType::getParam()
{
    return paramList_;
}
FuncType* FuncType::create(Type* returnType)
{ 
    return new FuncType(returnType); 
}

FuncType::FuncType(Type* returnType):Type(4,4,T_FUNC), returnType_(returnType)
{

}

StructType* StructType::create()
{
     return new StructType();
}
void StructType::addMember(const StructType::Member& memb)
{
    members_.push_back(memb);
}
StructType::StructType():Type(0,14,T_STRUCT,true)
{

}
}