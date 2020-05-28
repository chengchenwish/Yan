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
         list_.push_back({name, indenti});

    }
    bool symbolTable::getIdentiInCurrentScope(const std::string& name,  Identifier** indenti)

    {
        for(const auto& v: list_)
        {
            if(v.first == name)
            {
                *indenti = v.second;
                return true;
            }
        }
        return false;
       
        

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