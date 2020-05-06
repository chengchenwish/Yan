#include "symbol.h"
#include "error.h"

namespace Yan
{
    int symbolTable::findGlob(const std::string& s)
    {
        for(int i=0;i<glob;++i)
        {
            if(table[i].name == s)
            {
                return i;
            }
        }
        return -1;
    }
    void symbolTable::addGlob(std::string& s)
    {
        Info(s.c_str());
        table[glob++] = symbol(s);

    }
    symbolTable::symbolTable()
    {
        glob = 0;
    }
    symbolTable::~symbolTable()
    {

    }
    void symbolTable:: addSymoble(const std::string& name, const Identifier& indenti)
    {
         map_.insert({name,indenti});
    }
    bool symbolTable::getIdentiInCurrentScope(const std::string& name, const Identifier* indenti)const

    {
        auto it = map_.find(name);
        if(it != map_.end())
        {
            indenti = &it->second;
            return true;
        }
        else
        {
            return false;
        }
        

    }

}