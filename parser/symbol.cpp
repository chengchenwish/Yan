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

}