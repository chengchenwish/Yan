#include "symbol.h"
#include "error.h"

namespace Yan
{
   
    symbolTable::symbolTable()
    {

    }
    symbolTable::~symbolTable()
    {

    }
    void symbolTable:: addSymoble(const std::string& name, Identifier* indenti)
    {
         map_.insert({name, indenti});

    }
    bool symbolTable::getIdentiInCurrentScope(const std::string& name,  Identifier** indenti)

    {
        auto it = map_.find(name);
        if(it != map_.end())
        {
            *indenti = it->second;
            return true;
        }
        else
        {
            return false;
        }
        

    }
     bool symbolTable::getIdentiInAllScope(const std::string& name,  Identifier** indenti)
     {
         if(getIdentiInCurrentScope(name, indenti))
         {
             return true;
         }
        if(parent_)
        {
           return  parent_->getIdentiInAllScope(name,indenti);
        }
        return false;
     }



}