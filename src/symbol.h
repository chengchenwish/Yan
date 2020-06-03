#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <array>
#include <algorithm>
#include <iomanip>
#include "token.h"
#include "AST.h"
#include "type.h"

namespace Yan
{

    //class Idetifier;

    enum class Scope
    {
        GLOBAL,
        FUNC,
        BLOCK
    };
    extern std::string scopeToString(Scope s);
    class symbolTable
    {
    public:
        using Symbol = std::pair<std::string, Identifier *>;
        using SymbolList = std::vector<Symbol>;
        symbolTable();
        ~symbolTable();
        //new
        void addSymoble(const std::string &name, Identifier *indenti);
        bool getIdentiInCurrentScope(const std::string &name, Identifier **indenti);
        bool getIdentiInAllScope(const std::string &name, Identifier **indenti);
        void setParent(symbolTable *parent) { parent_ = parent; }
        void setScope(const Scope s) { scope_ = s; }
        Scope getScope() { return scope_; }
        SymbolList &getlist() { return list_; }
        bool existInCurrentScope(const std::string &name);
        symbolTable *getParentScop() { return parent_; }
        SymbolList::iterator begin() { return list_.begin(); }
        SymbolList::iterator end() { return list_.end(); }

        int caculateOffset(const std::string &name)
        {
            int off = caculateParentScopeOffSet(parent_);
            for (const auto &kv : list_)
            {
                off += kv.second->type_->getsize();

                if (kv.first == name)
                {
                    break;
                }
            }
            return off;
        }
        int getTyepSize()
        {
            int size = 0;
            for (const auto &kv : list_)
            {
                size += kv.second->type_->getsize();
            }
            return size;
        }
        void dumpSymbol(std::ostream &os);

    private:
        int caculateParentScopeOffSet(symbolTable *sc)
        {
            int off = 0;
            if (sc && (sc->getScope() == Scope::BLOCK || (sc->getScope() == Scope::FUNC)))
            {
                Info("iiiiii");
                off += caculateParentScopeOffSet(sc->getParentScop());
                for (const auto &kv : sc->getlist())
                {
                    off += kv.second->type_->getsize();
                }
            }
            else
            {
                return 0;
            }
            return off;
        }
        Scope scope_;
        SymbolList list_;
        symbolTable *parent_;

        //disable copy and assign
        symbolTable(const symbolTable &) = delete;
        symbolTable &operator=(const symbolTable &) = delete;
    };
} // namespace Yan
#endif