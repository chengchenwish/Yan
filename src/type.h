#ifndef TYPE_H_
#define TYPE_H_
#include <vector>
#include<string>
namespace Yan
{

struct Identifier;

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
    Type(int size, int align, TypeKind kind, bool incomplete = false):size_(size),align_(align), 
    kind_(kind),isIncomplete_(incomplete){}

    TypeKind getType(){return kind_;}
    int getalign()const{return align_;}
    int getsize()const {return size_;}
private:
     int size_; //sizeo()value

     int align_ ;
     TypeKind kind_ ;
     bool isIncomplete_;
};
#define BASIC(xx,size,align,kind)\
class xx :public Type{ \
public:\
    static xx* create(){ return new xx();}\
private:\
    xx():Type(size,align,kind){}\
\
};
BASIC(IntType,4,4,T_INT)
BASIC(VoidType,1,1,T_VOID)
BASIC(BoolType,1,1,T_BOOL)
BASIC(CharType,1,1,T_CHAR)
BASIC(ShortType,2,2,T_SHORT)
BASIC(LongType,8,8,T_LONG)
#undef BASIC        


class PtrType:public Type
{
public:
    static PtrType* create(Type* base){ return new PtrType(base);}
private:
PtrType(Type* base):Type(8,8,T_PTR),baseType_(base){}

Type* baseType_;
};

class ArrayType:public Type
{
public:
    static ArrayType* create(Type*base, int len){ return new ArrayType(base,len);}
private:
    ArrayType(Type*base, int len):Type(base->getsize()*len,base->getalign(),T_ARRAY),baseType_(base),len_(len){}
    Type* baseType_;
    int len_;

};
class FuncType: public Type
{
public:
    void addParam(Identifier* param){paramList_.push_back(param);}
    std::vector<Identifier*>&getParam(){return paramList_;}
    static FuncType* create(Type* returnType){ return new FuncType(returnType); }
private:
    FuncType(Type* returnType):Type(4,4,T_FUNC), returnType_(returnType){}
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
static StructType* create(){return new StructType();}
void addMember(const Member& memb){members_.push_back(memb);}
private:
    StructType():Type(0,14,T_STRUCT,true){}
    std::vector<Member> members_;
};

enum class storageClass 
{
    EXTERN,
    STATIC
};

}
#endif