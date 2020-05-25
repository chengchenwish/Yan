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
    TypeKind getType();
    int getalign()const;
    int getsize()const ;
private:
     int size_; //sizeo()value

     int align_ ;
     TypeKind kind_ ;
     bool isIncomplete_;
};

class PtrType:public Type
{
public:
    static PtrType* create(Type* base);
private:
    PtrType(Type* base);

    Type* baseType_;
};

class ArrayType:public Type
{
public:
    static ArrayType* create(Type*base, int len);
private:
    ArrayType(Type*base, int len);
    Type* baseType_;
    int len_;

};
class FuncType: public Type
{
public:
    void addParam(Identifier* param);
    std::vector<Identifier*>&getParam();
    static FuncType* create(Type* returnType);
private:
    FuncType(Type* returnType);
    Type* returnType_;
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

enum class storageClass 
{
    EXTERN,
    STATIC
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