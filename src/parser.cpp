#include "parser.h"
namespace Yan
{

    parser::parser(lexer &s) : scan(s)
    {
        currentScop_ = new Scope;
        currentScop_->setScope(ScopeKind::GLOBAL);
    }
    parser::~parser()
    {
    }
    void parser::enterScope(ScopeKind kind)
    {
        auto sc = new Scope;
        sc->setParent(this->currentScop_);
        sc->setScope(kind);
        currentScop_ = sc;
    }
    void parser::leaveScope()
    {
        currentScop_ = currentScop_->getParentScop();
        Info(__func__);
    }
    Expr *parser::primary()
    {
        Expr *node = nullptr;

        if (is(TokenType::T_INTLIT))
        {
            auto t = consume();

            Info(std::to_string(t.getValue()).c_str());
            node = IntegerLiteral::create(t.getValue());
            node->type_ = int_type;
        }
        else if(is(TokenType::T_STRLIT))
        {
            auto t = consume();
            auto len = t.getText().length()+1;
            node = StringLiteral::create(t.getText().data(), len);
            auto ty=  ArrayType::create(char_type,len);
            node->type_ = ty;
            
        }
        else if (is(TokenType::T_IDENT))
        {
            Identifier *identi;
            auto t = consume();
            Info(t.tostring().c_str());
            auto exist = currentScop_->getIdentiInAllScope(t.getText(), &identi);
            if (!exist)
            {
                ExitWithError("undefined variable :%s", t.getText().c_str());
            }
            if (match(TokenType::T_LPAREN))
            {
                return parseFuncCall(identi);
            }
            return identi;
        }
        else if (match(TokenType::T_LPAREN))
        {
            Expr *expr = assign();
            expect(TokenType::T_RPAREN, ")");
            return expr;
        }
        else
        {
            ExitWithError("The Primary token must be a number or identifier");
        }

        return node;
    }
    //if the front token doesn't match the specific token exit with error
    void parser::expect(TokenType t, const std::string &what)
    {
        if (!is(t))
        {
            ExitWithError(scan.getLocation(), "matchTed %s,but current token is %s ", what.c_str(), scan.getToken().tostring().c_str());
        }
        else
        {
            consume();
        }
    }
    //Test the front token, if matched consume it,if not put back
    bool parser::match(TokenType t)
    {
        auto token = scan.getToken();
        if (token.type == t)
        {
            //current_token_ = token;
            return true;
        }
        else
        {
            //  current_token_ = token;
            scan.putBack(token);
            return false;
        }
    }
    bool parser::is(TokenType t) const
    {
        auto token = scan.getToken();
        if (token.type == t)
        {
            scan.putBack(token);
            return true;
        }
        else
        {
            scan.putBack(token);
            return false;
        }
    }
    Token parser::consume()
    {
        return scan.getToken();
    }

    // expr = assign(","assign")*
    Expr *parser::expr()
    {
        Expr *left = assign();
        while (match(TokenType::T_COMMA))
        {
            auto right = assign();
            left = BinaryOp::create(OpType::OP_COMMA, left, right);
        }
        return left;
    }
    // assign    = conditional (assign-op assign)?
    // assign-op = "=" | "+=" | "-=" | "*=" | "/=" | "<<=" | ">>="
    //           | "&=" | "|=" | "^="
    Expr *parser::assign()
    {
        auto node = conditional();
        if (match(TokenType::T_ASSIGN)) //=
        {
            return BinaryOp::create(OpType::OP_ASSIGN, node, assign(), node->type_);
        }
        if (match(TokenType::T_ASSLASH)) // /=
        {
            return BinaryOp::create(OpType::OP_ASSMOD, node, assign(), node->type_);
        }
        if (match(TokenType::T_ASSTAR)) //*=
        {
            return BinaryOp::create(OpType::OP_ASSMUL, node, assign(), node->type_);
        }
        if (match(TokenType::T_ASPLUS)) // +=
        {
            return BinaryOp::create(OpType::OP_ASSPLUS, node, assign(), node->type_);
        }
        if (match(TokenType::T_ASMINUS)) // -=
        {
            return BinaryOp::create(OpType::OP_ASSSUB, node, assign(), node->type_);
        }

        if (match(TokenType::T_ASMOD)) // %=
        {
            return BinaryOp::create(OpType::OP_ASSMOD, node, assign(), node->type_);
        }

        //  if (match(TokenType::T_ASSIGN))
        // {
        //     return BinaryOp::create(OpType::OP_ASSIGN,node,assign());
        // }
        //TODO <<= >>= &= ^= |=
        return node;
    }
    // conditional = logor ("?" expr ":" conditional)?
    Expr *parser::conditional()
    {
        Expr *node = logicOr();
        if (match(TokenType::T_QUSTION))
        {
            auto true_expr = expr();
            expect(TokenType::T_COLON, ":");
            auto false_expr = conditional();
            return ConditionExpr::create(node, true_expr, false_expr);
        }
        return node;
    }
    // logor = logand ("||" logand)*
    Expr *parser::logicOr()
    {
        Expr *node = logicAnd();
        while (match(TokenType::T_LOGOR))
        {
            node = BinaryOp::create(OpType::OP_LOGICOR, node, logicAnd());
        }
        return node;
    }
    // logand = bitor ("&&" bitor)*
    Expr *parser::logicAnd()
    {
        Expr *node = bitOr();
        while (match(TokenType::T_LOGAND))
        {
            node = BinaryOp::create(OpType::OP_LOGICAND, node, bitOr());
        }
        return node;
    }

    // bitor = bitand ("|" bitand)*
    Expr *parser::bitOr()
    {
        Expr *node = bitXor();
        while (match(TokenType::T_BITOR))
        {
            node = BinaryOp::create(OpType::OP_BITOR, node, bitXor());
        }
        return node;
    }
    Expr *parser::bitXor()
    {
        Expr *node = bitAnd();
        while (match(TokenType::T_BITXOR))
        {
            node = BinaryOp::create(OpType::OP_BITXOR, node, bitAnd());
        }
        return node;
    }
    Expr *parser::bitAnd()
    {
        Expr *node = equality();
        while (match(TokenType::T_AMPER))
        {
            node = BinaryOp::create(OpType::OP_BITAND, node, equality());
        }
        return node;
    }
    // equality = relational ("==" relational | "!=" relational)*
    Expr *parser::equality()
    {
        Expr *node = relational();
        while (1)
        {
            if (match(TokenType::T_EQ))
            {
                node = BinaryOp::create(OpType::OP_EQ, node, relational());
            }
            else if (match(TokenType::T_NE))
            {
                node = BinaryOp::create(OpType::OP_NE, node, relational());
            }
            else
                return node;
        }
        return node;
    }
    // relational = shift ("<" shift | "<=" shift | ">" shift | ">=" shift)*
    Expr *parser::relational()
    {
        Expr *node = shift();
        while (1)
        {
            if (match(TokenType::T_LT))
            {
                node = BinaryOp::create(OpType::OP_LT, node, shift());
            }
            else if (match(TokenType::T_LE))
            {
                node = BinaryOp::create(OpType::OP_LE, node, shift());
            }
            else if (match(TokenType::T_GT))
            {
                node = BinaryOp::create(OpType::OP_GT, node, shift());
            }
            else if (match(TokenType::T_GE))
            {
                node = BinaryOp::create(OpType::OP_GE, node, shift());
            }
            else
                return node;
        }
        return node;
    }
    // shift = sum ("<<" sum | ">>" sum)*
    Expr *parser::shift()
    {
        Expr *node = sum();
        while (1)
        {
            if (match(TokenType::T_LSHIFT))
            {
                node = BinaryOp::create(OpType::OP_LSHIFT, node, sum());
            }
            else if (match(TokenType::T_RSHIFT))
            {
                node = BinaryOp::create(OpType::OP_RSHIFT, node, sum());
            }
            else
                return node;
        }
        return node;
    }
    //sum -> mul (('+' mul)|('-' mul))*
    Expr *parser::sum()
    {
        Expr *node = mul();
        for (;;)
        {
            if (match(TokenType::T_ADD))
            {
                node = BinaryOp::create(OpType::OP_ADD, node, mul());
            }
            else if (match(TokenType::T_MINUS))
            {
                node = BinaryOp::create(OpType::OP_SUBTRACT, node, mul());
            }
            else
            {
                return node;
            }
        }
    }

    Expr *parser::mul()
    {
        Expr *node = cast();
        for (;;)
        {
            if (match(TokenType::T_STAR))
            {
                node = BinaryOp::create(OpType::OP_MULTIPLY, node, cast());
            }
            else if (match(TokenType::T_SLASH))
            {
                node = BinaryOp::create(OpType::OP_DIVIDE, node, cast());
            }
            else if (match(TokenType::T_PERCENT))
            {
                node = BinaryOp::create(OpType::OP_MOD, node, cast());
            }
            else
            {
                return node;
            }
        }
    }
    //cast = (type)cast|unary
    Expr *parser::cast()
    {
        if (is(TokenType::T_RPAREN))
        {
            auto t = consume(); //consume '('
            if (isTypeName())
            {

                auto ty = baseType(nullptr);

                expect(TokenType::T_LPAREN, ")");
                if (!is(TokenType::T_LBRACE))
                {
                    consume();
                    return UnaryOp::create(OpType::OP_CAST, cast(), ty);
                }
            }
            scan.putBack(t); //puback token'('
        }
        return unary();
    }
    Expr *parser::unary()
    {
        if (match(TokenType::T_ADD))
        {
            return cast();
        }
        if (match(TokenType::T_MINUS))
        {
            return BinaryOp::create(OpType::OP_SUBTRACT, IntegerLiteral::create(0), cast());
        }
        if(match(TokenType::T_AMPER))
        {
            Info("ffffffffffffffffffffffff");
            return UnaryOp::create(OpType::OP_ADDR,cast());
        }
        if(match(TokenType::T_STAR))
        {
            Info("uuuuuuuuuuuuuuuuuuuuu");
             auto operand = cast();
             Type* t;
             if(operand->type_->isKindOf(Type::T_PTR))
             {
                 t = operand->type_->castToPtr()->getBaseType();
             }
             else 
             {
                 ExitWithError(operand->type_->tostring().c_str());
             }

            return UnaryOp::create(OpType::OP_DEREF,operand,t);
            //return UnaryOp::create(OpType::OP_DEREF, cast());
        }
        return postfix();
    }

    Expr *parser::postfix()
    {
        return primary();
    }

    IfStmt *parser::parseIfStmt()
    {
        expect(TokenType::T_LPAREN, "(");
        auto condition = expr();
        expect(TokenType::T_RPAREN, ")");
        Stmt *then = nullptr;
        Stmt *els = nullptr;
        if (!is(TokenType::T_LBRACE))
        {
            then = parseSingleStmt();
        }
        else
        {
            selfScope self(*this, ScopeKind::BLOCK);
            then = parseCompoundStmt();
            static_cast<CompousedStmt *>(then)->scope_ = currentScop_;
        }
        if (is(TokenType::T_ELSE))
        {
            consume();

            if (!is(TokenType::T_LBRACE))
            {
                els = parseSingleStmt();
            }
            else
            {
                selfScope self(*this, ScopeKind::BLOCK);
                els = parseCompoundStmt();
                static_cast<CompousedStmt *>(els)->scope_ = currentScop_;
            }
        }

        return IfStmt::create(condition, then, els);
    }
    LoopStmt *parser::parseWhileStmt()
    {
        expect(TokenType::T_LPAREN, "(");
        auto cond = expr();
        expect(TokenType::T_RPAREN, ")");
        Stmt *then = nullptr;
        if (!is(TokenType::T_LBRACE))
        {
            then = parseSingleStmt();
        }
        else
        {
            selfScope self(*this, ScopeKind::BLOCK);
            then = parseCompoundStmt();
            static_cast<CompousedStmt *>(then)->scope_ = currentScop_;
        }

        return LoopStmt::create(cond, then);
    }
    LoopStmt *parser::parseForStmt()
    {
        //this function should only be called by singleStmt
        expect(TokenType::T_SEMI, ";");
        auto cond = expr();
        expect(TokenType::T_SEMI, ";");
        auto inc = expr();
        expect(TokenType::T_RPAREN, ")");
        Stmt *body = nullptr;
        if (!is(TokenType::T_LBRACE))
        {
            body = parseSingleStmt();
            auto stmt = CompousedStmt::create();
            stmt->addStmt(body);
            stmt->addStmt(inc);
            stmt->scope_ = currentScop_;
        }
        else
        {
            body = parseCompoundStmt();
            static_cast<CompousedStmt *>(body)->addStmt(inc);
            static_cast<CompousedStmt *>(body)->scope_ = currentScop_;
        }

        return LoopStmt::create(cond, body);
    }
    LoopStmt *parser::parseDoWhileStmt()
    {
        Stmt *then = nullptr;
        {
            selfScope self(*this, ScopeKind::BLOCK);
            then = parseCompoundStmt();
            static_cast<CompousedStmt *>(then)->scope_ = currentScop_;
        }
        expect(TokenType::T_WHILE, "while");
        expect(TokenType::T_LPAREN, "(");
        auto cond = expr();
        expect(TokenType::T_RPAREN, ")");
        expect(TokenType::T_SEMI, ";");
        return LoopStmt::create(cond, then, true);
    }
    BreakContinueStmt *parser::parseContinueBreakStmt(BreakContinueStmt::kind type)
    {
        expect(TokenType::T_SEMI, ";");
        return BreakContinueStmt::create(type);
    }
    ReturnStmt *parser::parseReturnStmt()
    {
        Expr *exp = nullptr;
        if (!is(TokenType::T_SEMI))
        {
            exp = expr();
        }
        expect(TokenType::T_SEMI, ";");
        return ReturnStmt::create(exp);
    }
    Stmt *parser::parseSingleStmt()
    {
        if (match(TokenType::T_IF))
        {
            return parseIfStmt();
        }
        if (match(TokenType::T_WHILE))
        {
            return parseWhileStmt();
        }
        if (match(TokenType::T_DO))
        {
            return parseDoWhileStmt();
        }
        if (match(TokenType::T_BREAK))
        {
            return parseContinueBreakStmt(BreakContinueStmt::kBreak);
        }
        if (match(TokenType::T_CONTINUE))
        {
            return parseContinueBreakStmt(BreakContinueStmt::kcontinue);
        }
        if (match(TokenType::T_RETURN))
        {
            return parseReturnStmt();
        }
        if (match(TokenType::T_GOTO))
        {
            auto token = consume();
            if (token.type != TokenType::T_IDENT)
            {
                ExitWithError("expect identifier");
            }
            expect(TokenType::T_SEMI, ";");
            return GotoStmt::create(token.getText());
        }
        if (match(TokenType::T_FOR))
        {
            CompousedStmt *stmt = nullptr;
            //split for into two part
            //before: for (int i = 0; i<10;i++)
            //after:  int i = 0;
            //            ;i<10;i++)

            expect(TokenType::T_LPAREN, "(");
            //TODO variabale
            //auto cond = expr();
            selfScope self(*this, ScopeKind::BLOCK);
            if (!is(TokenType::T_SEMI))
            {
                stmt = CompousedStmt::create();
                auto init = expr();
                stmt->addStmt(init);
            }
            auto forstmt = parseForStmt();
            if (stmt)
            {
                stmt->addStmt(forstmt);
                return stmt;
            }
            return forstmt;
        }
        if (is(TokenType::T_IDENT))
        {
            Token varToken = consume();
            Info("vartoken:%s", varToken.tostring().c_str());

            if (match(TokenType::T_COLON))
            {

                return LabelStmt::create(varToken.getText());
            }
            else
            {
                scan.putBack(varToken);
                auto assign = expr();
                expect(TokenType::T_SEMI, ";");
                return assign;
            }
        }
        else
        {
            auto exp = expr();
            expect(TokenType::T_SEMI, ";");
            return exp;
        }
        
        ExitWithError("Token: %s unknow statment", peek().tostring().c_str());
    }
    CompousedStmt *parser::parseCompoundStmt()
    {
        Info(__func__);
        expect(TokenType::T_LBRACE, "{");
        auto compoused = CompousedStmt::create();
        while (!match(TokenType::T_RBRACE))
        {
            if (match(TokenType::T_EOF))
            {
                ExitWithError(__func__);
            }

            if (match(TokenType::T_SEMI))
            {
                //consume empty stmt
                continue;
            }
            else if (isTypeName())
            {
                auto declaration = parseDeclaration(true);
                if (declaration)
                {
                    compoused->addStmt(declaration);
                }
                else
                {
                    continue;
                }
            }

            else if (is(TokenType::T_LBRACE))
            {
                selfScope self(*this, ScopeKind::BLOCK);
                auto block = parseCompoundStmt();
                block->scope_ = currentScop_;
                compoused->addStmt(block);
            }

            else
            {

                compoused->addStmt(parseSingleStmt());
            }
        }
        return compoused;
    }
    FunctionCall *parser::parseFuncCall(Identifier *identi)
    {

        if (!identi->type_->isKindOf(Type::T_FUNC))
        {
            ExitWithError("Expect function type");
        }
        auto funcCall = FunctionCall::create(identi);

        auto argsNum = identi->type_->castToFunc()->getParam().size();
        Info("num    %d", argsNum);
        for (int i = 0; i < argsNum; i++)
        {
            Expr *exp = assign();
            Info("add args");
            funcCall->addArg(exp);
            if (i < argsNum - 1)
            {
                Info("i=%d:", i);
                expect(TokenType::T_COMMA, ",");
            }
        }
        expect(TokenType::T_RPAREN, ")");
        // expect(TokenType::T_SEMI, ";");
        return funcCall;
    }
    FunctionDef *parser::parseFuncDef()
    {
        storageClass sclass;
        auto type = baseType(&sclass);
        auto pair = declarator(type);
        auto name = pair.second;
        auto new_ty = pair.first;
        auto funcIden = Identifier::create(name, new_ty, false);
        currentScop_->addSymoble(name, funcIden);

        selfScope self(*this, ScopeKind::FUNC);
        for (auto param : new_ty->castToFunc()->getParam())
        {
            currentScop_->addSymoble(param->name_, param);
            param->offset_ = currentScop_->caculateOffset(param->name_);
        }
        auto func = FunctionDef::create(funcIden);
        auto body = parseCompoundStmt();
        body->scope_ = currentScop_;
        func->setBody(body);

        Info(__func__);
        return func;
    }
    Declaration *parser::parseDeclaration(bool isloacl)
    {
        storageClass sclass = storageClass::UNKNOW;
        auto type = baseType(&sclass);
        auto pair = declarator(type);

        auto identi = Identifier::create(pair.second, pair.first, isloacl);
        currentScop_->addSymoble(pair.second, identi);
        if (sclass == storageClass::TYPE_DEF)
        {
            identi->class_ = storageClass::TYPE_DEF;
            expect(TokenType::T_SEMI, ";");
            return nullptr;
        }
        identi->setoffset(currentScop_->caculateOffset(pair.second));
        expect(TokenType::T_SEMI, ";");
        return Declaration::create(identi);
    }
    void parser::defineBuildinFunc(std::string name, Type *reType, std::vector<Type *> paramType)
    {
        auto functype = FuncType::create(reType);
        for (const auto &paramT : paramType)
        {
            functype->addParam(Identifier::create("", paramT, true));
        }
        auto identi = Identifier::create(name, functype, false);
        currentScop_->addSymoble(name, identi);
    }
    Program *parser::parseProgram()
    {
        auto program = Program::create();
        defineBuildinFunc("print", void_type, {int_type});

        Scope *funcscop = nullptr;
        while (!match(TokenType::T_EOF))
        {
            if (isFuncdef())
            {
                program->add(parseFuncDef());
            }
            else
            {
                auto decl = parseDeclaration(false);
                if (decl)
                {
                    program->add(decl);
                }
            }
        }

        return program;
    }

    bool parser::isFuncdef()
    {
        std::stack<Token> cache;
        bool isfuncdef = false;
        while (!is(TokenType::T_SEMI) && !is(TokenType::T_LBRACE))
        {
            if (is(TokenType::T_EOF))
            {
                ExitWithError("unexpected Eof");
            }

            cache.push(consume());
        }
        if (is(TokenType::T_SEMI))
        {

            isfuncdef = false;
        }
        //end with pattern .* ) {.*
        else if (is(TokenType::T_LBRACE) && cache.top().type == TokenType::T_RPAREN)
        {
            isfuncdef = true;
        }
        while (!cache.empty())
        {
            putBack(cache.top());
            cache.pop();
        }
        Info("isfuncdef:%s", std::to_string(isfuncdef).c_str());
        return isfuncdef;
    }

    bool parser::isTypeName()
    {
        //current only support int/char
        return isOneOf(TokenType::T_INT,
                       TokenType::T_CHAR,
                       TokenType::T_BOOL,
                       TokenType::T_ENUM,
                       TokenType::T_SHORT,
                       TokenType::T_SIGNED,
                       TokenType::T_UNSIGNED,
                       TokenType::T_STATIC,
                       TokenType::T_STRUCT,
                       TokenType::T_VOID,
                       TokenType::T_TYPDEF,
                       TokenType::T_EXTERN,
                       TokenType::T_LONG) ||
               (is(TokenType::T_IDENT) && findtypedef(peek().getText()));
    }
    bool parser::findtypedef(const std::string &name)
    {
        Identifier *identi = nullptr;
        currentScop_->getIdentiInAllScope(name, &identi);
        if (identi && identi->class_ == storageClass::TYPE_DEF)
        {
            return true;
        }
        return false;
    }

    // basetype = builtin-type | struct-decl | typedef-name | enum-specifier
    //
    // builtin-type = "void" | "_Bool" | "char" | "short" | "int"
    //              | "long" | "long" "long"
    Type *parser::baseType(storageClass *sclass)
    {
        if (!isTypeName())
        {
            ExitWithError("Type name expected");
        }
        enum
        {
            kkindnone = 0,
            kvoid = 1,
            kbool,
            kchar,
            kint
            // kfloat,
            //kdouble
        } kind = kkindnone;
        enum
        {
            ksizenone = 0,
            kshort = 1,
            klong,
            kllong
        } size = ksizenone;

        enum
        {
            ksignnone = 0,
            ksigned = 1,
            kunsigned
        } sign = ksignnone;

        Type *ty = nullptr;
        storageClass storage_class = storageClass::UNKNOW;
        while (isTypeName())
        {
            if (isOneOf(TokenType::T_STATIC, TokenType::T_EXTERN, TokenType::T_TYPDEF))
            {
                //handle storage class
                if (storage_class != storageClass::UNKNOW)
                {
                    //storage alreadly have a value;
                    ExitWithError("Type mismatch");
                }
                if (match(TokenType::T_STATIC))
                {
                    storage_class = storageClass::STATIC;
                }
                else if (match((TokenType::T_EXTERN)))
                {
                    storage_class = storageClass::EXTERN;
                }
                else if (match(TokenType::T_TYPDEF))
                {
                    storage_class = storageClass::TYPE_DEF;
                }
            }
            //handle user defined type
            else if (is(TokenType::T_STRUCT))
            {
                ty = parseStructDecl();
            }
            else if (is(TokenType::T_ENUM))
            {
                ty = parseEnumSpecifier();
            }
            else if (is(TokenType::T_UNION))
            {
                ty = parseUnionSpecifier();
            }
            //Handle build-in type

            else if (isOneOf(TokenType::T_SHORT, TokenType::T_LONG))
            {
                //handle size;
                if (size != ksizenone && size != klong)
                {
                    ExitWithError("Type mismatch");
                }
                if (match(TokenType::T_SHORT))
                {
                    size = kshort;
                }
                else if (match(TokenType::T_LONG))
                {
                    size = size ? kllong : klong;
                }
            }
            //build-in type
            else if (isOneOf(TokenType::T_INT, TokenType::T_CHAR, TokenType::T_VOID, TokenType::T_BOOL))
            {
                if (kind != kkindnone)
                {
                    ExitWithError("Type mismatch");
                }
                if (match(TokenType::T_INT))
                {
                    kind = kint;
                }
                else if (match(TokenType::T_CHAR))
                {
                    kind = kvoid;
                }
                else if (match(TokenType::T_VOID))
                {
                    kind = kvoid;
                }
                else if (match(TokenType::T_BOOL))
                {
                    kind = kbool;
                }
            }

            //sign

            else if (isOneOf(TokenType::T_UNSIGNED, TokenType::T_SIGNED))
            {
                if (sign != ksignnone)
                {
                    ExitWithError("mismatch type");
                }
                if (match(TokenType::T_UNSIGNED))
                {
                    sign = kunsigned;
                }
                else if (match(TokenType::T_SIGNED))
                {
                    sign = ksigned;
                }
            }
            //handle user defined type;
            else if (peek().type == TokenType::T_IDENT)
            {
                Identifier *identi = nullptr;
                if (currentScop_->getIdentiInAllScope(peek().getText(), &identi))
                {
                    consume();
                    return identi->type_;
                }
            }
        }
        if (sclass)
        {
            *sclass = storage_class;
        }

        switch (kind)
        {
        case kvoid:
            ty = void_type;
            break;
        case kchar:
            ty = char_type;
            break;
        case kbool:
            ty = bool_type;
            break;
        default:
            break;
        }
        if (ty == nullptr)
        {
            switch (size)
            {
            case kshort:
                ty = short_type;
                break;
            case klong:
                ty = long_type;
                break;
            case kllong:
                ty = long_type;
                break; //todo
            default:
                ty = int_type;
                break;
            }
        }
        return ty;
    }
    Type *parser::modifyBase(Type *type, Type *base, Type *new_base)
    {
        if (type == base)
        {
            return new_base;
        }
        auto ty = base->castToDeried();
        ty->setBase(modifyBase(type, ty->getBaseType(), new_base));
        return ty;
    }
    // declarator = "*"* ("(" declarator ")" | ident) type-suffix
    //int *p();
    Declarator parser::declarator(Type *type)
    {
        Type *ty = type;
        Info(peek().tostring().c_str());
        while (match(TokenType::T_STAR))
        {
            ty = PtrType::create(ty);
        }

        if (match(TokenType::T_LPAREN))
        {

            auto pair = declarator(ty);
            Type *base = pair.first;
            expect(TokenType::T_RPAREN, ")");

            auto new_base = type_suffix(ty);
            auto return_type = modifyBase(ty, base, new_base);

            return std::make_pair(return_type, pair.second);
        }
        else if (is(TokenType::T_IDENT))
        {
            auto name = consume().getText();
            ty = type_suffix(ty);
            return std::make_pair(ty, name);
        }
        else
        {
            ty = type_suffix(ty);
            //abstract declarator int*();
            return std::make_pair(ty, "");
        }
    }
    Type *parser::type_suffix(Type *type)
    {
        if (is(TokenType::T_LBRACKET))
        {
            return declarator_array(type);
        }
        if (is(TokenType::T_LPAREN))
        {
            return declarator_func(type);
        }
        return type;
    }

    Type *parser::declarator_array(Type *type)
    {
        expect(TokenType::T_LBRACKET, "[");
        bool incomplete = true;
        int len = -1;
        if (!is(TokenType::T_RBRACKET))
        {
            len = constExpr<int>();
            expect(TokenType::T_RBRACKET, "]");
            incomplete = false;
        }

        if (is(TokenType::T_LBRACKET))
        {
            type = declarator_array(type);
            if (type->isIncomplete())
            {
                ExitWithError("Array type incompleye");
            }
        }
        assert(len > 0);
        Type *array = ArrayType::create(type, len);
        array->setIncomplete(incomplete);

        return array;
    }

    Type *parser::declarator_func(Type *type)
    {
        if (type->isKindOf(Type::T_ARRAY) ||
            type->isKindOf(Type::T_FUNC))
        {
            ExitWithError("function return type can't be:%s", type->tostring().c_str());
        }
        auto functionType = FuncType::create(type);
        expect(TokenType::T_LPAREN, "(");
        while (1)
        {
            if (isOneOf(TokenType::T_EOF, TokenType::T_RPAREN))
            {
                break;
            }
            if (match(TokenType::T_ELLIPSIS))
            {
                functionType->setVarargsFlag(true);
                break;
            }
            auto pair = parse_func_param();
            auto param = Identifier::create(pair.second, pair.first, true);
            functionType->addParam(param);
            if (isOneOf(TokenType::T_EOF, TokenType::T_RPAREN))
            {
                break;
            }

            expect(TokenType::T_COMMA, ",");
        }
        expect(TokenType::T_RPAREN, ")");

        return functionType;
    }

    Declarator parser::parse_func_param()
    {
        Type *ty;
        if (isTypeName())
        {
            ty = baseType(nullptr);
        }
        else
        {
            ExitWithError("Expect type");
        }

        auto pair = declarator(ty);
        ty = pair.first;
        if (ty->isKindOf(Type::T_ARRAY))
        {
            ty = PtrType::create(ty->castToArray()->getBaseType());
        }
        else if (ty->isKindOf(Type::T_FUNC))
        {
            ty = PtrType::create(ty);
        }
        return std::make_pair(ty, pair.second);
    }

    Type *parser::parseStructDecl()
    {
        //TODO
        return nullptr;
    }
    Type *parser::parseEnumSpecifier()
    {
        return nullptr;
    }
    Type *parser::parseUnionSpecifier()
    {
        return nullptr;
    }

} // namespace Yan
