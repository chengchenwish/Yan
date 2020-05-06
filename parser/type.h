#ifndef TYPE_H_
#define TYPE_H_
#include <vector>
#include<string>
namespace Yan
{



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

class VoidType: public Type
{
public:
    static VoidType* create(){ return new VoidType();}
private:
    VoidType():Type(1,1,T_VOID){}
};

class BoolType: public Type
{
 public:
    static BoolType* create(){ return new BoolType();}
private:
    BoolType():Type(1,1,T_BOOL){}
};


class CharType: public Type
{
public:
    static CharType* create(){ return new CharType();}
private:
    CharType():Type(1,1,T_CHAR){}
};
class ShortType: public Type
{
public:
    static ShortType* create(){ return new ShortType();}
private:
    ShortType():Type(2,2,T_SHORT){}
};


class IntType: public Type
{
public:
    static IntType* create(){ return new IntType();}
private:
    IntType():Type(4,4,T_INT){}
};


class LongType: public Type
{
public:
    static LongType* create(){ return new LongType();}
private:
    LongType():Type(8,8,T_LONG){}
};
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
class FuncType: Type
{
public:
    static FuncType* create(Type* returnType){ return new FuncType(returnType); }
private:
    FuncType(Type* returnType):Type(4,4,T_FUNC), returnType_(returnType){}
    Type* returnType_;
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
static StructType* create(){new StructType();}
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