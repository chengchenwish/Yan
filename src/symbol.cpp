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
          auto ele = std::find_if(list_.begin(),list_.end(),[& name](Symbol& s){
           return s.first == name;
       });
      
       if(ele != list_.end())
       {
            *indenti = ele->second;
             return true;
       }
       else 
       {
           return false;
       }
       
        

    }
     bool symbolTable::existInCurrentScope(const std::string& name)
   {
       auto ele = std::find_if(list_.begin(),list_.end(),[& name](Symbol& s){
           return s.first == name;
       });
       return (ele != list_.end());
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


extern std::string  scopeToString(Scope s)
{
    switch(s)
    {
        case Scope::BLOCK: return "block";
        case Scope::FUNC: return "function";
        case Scope::GLOBAL: return "global";
    }
    return "";
}


}