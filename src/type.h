#ifndef TYPE_H_
#define TYPE_H_
#include <vector>
#include<string>
namespace Yan
{

struct Identifier;

#define BASIC(xx,size,align,kind)            \
class xx :public Type{                       \
public:                                      \
    static xx* create(){ return new xx();}   \
private:                                     \
    xx():Type(size,align,kind){}             \
                                             \
};
enum class storageClass 
{
    TYPE_DEF,
    EXTERN,
    STATIC,
    AUTO,
    REGISTER,

    UNKNOW = 255
};

class Type
{
public:

    enum TypeKind
    {
        T_VOID,
        T_BOOL,
        T_CHAR,
        T_SHORT,
        T_INT,
        T_LONG,
        T_ENUM,
        T_PTR,
        T_ARRAY,
        T_STRUCT,
        T_FUNC,
    };
    Type(int size, int align, TypeKind kind, bool incomplete = false);
    Type() = default;
    TypeKind getType();
    int getalign()const;
    int getsize()const ;
    std::string tostring();
private:
     int size_; //sizeo()value

     int align_ ;
     TypeKind kind_ ;
     bool isIncomplete_;
     bool isunsigned;
     storageClass class_;
};
// void clone(Type*src, type*dest)
// {
//     int size = 0;
//     switch(source->getType())
//     {
//         case Type::T_VOID:
//         case Type::T_BOOL:
//         case Type::T_CHAR:
//         case Type::T_SHORT:
//         case Type::T_INT:
//         case Type::T_LONG:
//             size = sizeof(Type);break;
//         case Type::T_PTR:
//             size = sizeof(PtrType);break;
//         case Type::T_FUNC:
//             size = sizeof(FuncType);break;
//         case Type::T_ARRAY:
//             size = sizeof(ArrayType);break;
//         case Type::T_STRUCT:
//             size = sizeof(StructType);break;



//     }
//     memcpy(dest,src,size);
// }
class DerivedType :public Type
{
public:
Type* getBaseType(){ return baseType_;}
void setBase(Type* ty){baseType_ = ty;}
DerivedType(int size, int align, TypeKind kind, Type*base): Type(size, align, kind),baseType_(base){}

private:
    Type* baseType_;
};


class PtrType:public DerivedType
{
public:
    static PtrType* create(Type* base);
    
private:
    PtrType(Type* base);
};

class ArrayType:public DerivedType
{
public:
    static ArrayType* create(Type*base, int len);
private:
    ArrayType(Type*base, int len);
    int len_;

};
class FuncType: public DerivedType
{
public:
    void addParam(Identifier* param);
    std::vector<Identifier*>&getParam();
    static FuncType* create(Type* returnType);
private:
    FuncType(Type* returnType);
    std::vector<Identifier*>paramList_;
};
class StructType:public Type 
{
public:
    struct Member
    {
        Type* ty_;
        std::string name_;
        int offset;
    };
static StructType* create();
void addMember(const Member& memb);
private:
    StructType();
    std::vector<Member> members_;
};




BASIC(IntType,4,4,T_INT)
BASIC(VoidType,1,1,T_VOID)
BASIC(BoolType,1,1,T_BOOL)
BASIC(CharType,1,1,T_CHAR)
BASIC(ShortType,2,2,T_SHORT)
BASIC(LongType,8,8,T_LONG)
#undef BASIC        


}
#endif