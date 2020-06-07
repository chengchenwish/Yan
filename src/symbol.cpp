#include "symbol.h"
#include "error.h"

namespace Yan
{

    Scope::Scope()
    {
    }
    Scope::~Scope()
    {
    }
    void Scope::addSymoble(const std::string &name, Identifier *indenti)
    {
        symbols_.push_back({name, indenti});
    }
    bool Scope::getIdentiInCurrentScope(const std::string &name, Identifier **indenti)

    {
        auto ele = std::find_if(symbols_.begin(), symbols_.end(), [&name](Symbol &s) {
            return s.first == name;
        });

        if (ele != symbols_.end())
        {
            *indenti = ele->second;
            return true;
        }
        else
        {
            return false;
        }
    }
    bool Scope::existInCurrentScope(const std::string &name)
    {
        auto ele = std::find_if(symbols_.begin(), symbols_.end(), [&name](Symbol &s) {
            return s.first == name;
        });
        return (ele != symbols_.end());
    }
    bool Scope::getIdentiInAllScope(const std::string &name, Identifier **indenti)
    {
        if (getIdentiInCurrentScope(name, indenti))
        {
            return true;
        }
        if (parent_)
        {
            return parent_->getIdentiInAllScope(name, indenti);
        }
        return false;
    }

    void Scope::dumpSymbol(std::ostream &os)
    {
        os << " Current scope:" << scopeToString(kind_) << std::endl;
        os << std::endl;
        os << std::endl;

        os << "name" << std::setw(20) << "Type" << std::setw(20) << "size" << std::setw(20) << "scope" << std::endl;

        for (const auto &kv : symbols_)
        {

            os << kv.first << std::setw(20) << kv.second->type_->tostring() << std::setw(20) << kv.second->type_->getsize();
            os << std::setw(20) << scopeToString(kind_) << std::endl;
        }
        //    os<<std::setiosflags(std::ios::left)<<std::endl;
    }
    std::string scopeToString(ScopeKind s)
    {
        switch (s)
        {
        case ScopeKind::BLOCK:
            return "block";
        case ScopeKind::FUNC:
            return "function";
        case ScopeKind::GLOBAL:
            return "global";
        }
        return "";
    }

} // namespace Yan