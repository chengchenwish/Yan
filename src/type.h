#ifndef TYPE_H_
#define TYPE_H_
#include <vector>
#include <string>
namespace Yan
{

    struct Identifier;

    class PtrType;
    class ArrayType;
    class StructType;
    class FuncType;
    class DerivedType;

#define BUILD_IN_TYPE(xx, size, align, kind)     \
    class xx : public Type                       \
    {                                            \
    public:                                      \
        static xx *create() { return new xx(); } \
                                                 \
    private:                                     \
        xx() : Type(size, align, kind) {}        \
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
        TypeKind getKind();
        int getalign() const;
        int getsize() const;
        void setIncomplete(bool incomplete) { isIncomplete_ = incomplete; }
        std::string tostring();
        bool isKindOf(TypeKind kind) const { return kind_ == kind; }
        bool isIncomplete() { return isIncomplete_; }

        //cast
        virtual DerivedType *castToDeried() { return nullptr; }
        virtual PtrType *castToPtr() { return nullptr; }
        virtual ArrayType *castToArray() { return nullptr; }
        virtual FuncType *castToFunc() { return nullptr; }
        virtual StructType *castToStruct() { return nullptr; }

    private:
        int size_; //sizeo()value

        int align_;
        TypeKind kind_;
        bool isIncomplete_;
        bool isunsigned;
        storageClass class_;
    };

    class DerivedType : public Type
    {
    public:
        Type *getBaseType() { return baseType_; }
        void setBase(Type *ty) { baseType_ = ty; }
        DerivedType(int size, int align, TypeKind kind, Type *base) : Type(size, align, kind), baseType_(base) {}

        virtual DerivedType *castToDeried() { return this; }

    private:
        Type *baseType_;
    };

    class PtrType : public DerivedType
    {
    public:
        static PtrType *create(Type *base);
        virtual PtrType *castToPtr() { return this; }

    private:
        PtrType(Type *base);
    };

    class ArrayType : public DerivedType
    {
    public:
        static ArrayType *create(Type *base, int len);
        virtual ArrayType *castToArray() { return this; }

    private:
        ArrayType(Type *base, int len);
        int len_;
    };
    class FuncType : public DerivedType
    {
    public:
        void addParam(Identifier *param);
        std::vector<Identifier *> &getParam();
        static FuncType *create(Type *returnType);
        void setVarargsFlag(bool has){varargs = has;}
        bool hasVarargs()const{return varargs;}

        virtual FuncType *castToFunc() { return this; }

    private:
        FuncType(Type *returnType);
        std::vector<Identifier *> paramList_;
        bool varargs;
    };
    class StructType : public Type
    {
    public:
        struct Member
        {
            Type *ty_;
            std::string name_;
            int offset;
        };
        static StructType *create();
        void addMember(const Member &memb);
        virtual StructType *castToStruct() { return this; }

    private:
        StructType();
        std::vector<Member> members_;
    };

    BUILD_IN_TYPE(IntType, 4, 4, T_INT)
    BUILD_IN_TYPE(VoidType, 1, 1, T_VOID)
    BUILD_IN_TYPE(BoolType, 1, 1, T_BOOL)
    BUILD_IN_TYPE(CharType, 1, 1, T_CHAR)
    BUILD_IN_TYPE(ShortType, 2, 2, T_SHORT)
    BUILD_IN_TYPE(LongType, 8, 8, T_LONG)
#undef BUILD_IN_TYPE
    extern IntType *int_type;
    extern VoidType *void_type;
    extern BoolType *bool_type;
    extern CharType *char_type;
    extern ShortType *short_type;
    extern LongType *long_type;

} // namespace Yan
#endif