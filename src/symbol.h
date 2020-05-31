#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <array>
#include<map>
#include "token.h"
#include "AST.h"

namespace Yan
{

//class Idetifier;

struct symbol
{
    symbol(std::string&s)
    {
        this->name = s;
    }
    symbol()=default;

    std::string name;//name of symbol
};
 
 enum class Scope
 {
     GLOBAL,
     FUNC,
     BLOCK
 };

class symbolTable
{
public:
    symbolTable();
    ~symbolTable();   
    //new 
    void addSymoble(const std::string& name,  Identifier* indenti);
   bool  getIdentiInCurrentScope(const std::string& name,  Identifier** indenti);
    bool  getIdentiInAllScope(const std::string& name,  Identifier** indenti);
   void setParent(symbolTable* parent){ parent_ = parent;}
   void setScope(const Scope s){ scope_ = s;}
//    bool existInCurrentScope(const std::string& name)
//    {
//        for(auto kv)
//    }
   symbolTable* getParentScop(){ return parent_;}
   int caculateOffset(const std::string& name)
   {
       int off = 0;
       for (const auto&kv:list_)
       {    off += kv.second->type_->getsize();

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
       for(const auto& kv: list_)
       {
           size += kv.second->type_->getsize();
       }
       return size;
   }
   void printSymbol()
   {
       for(const auto& kv: list_)
       {
           std::cout<<kv.first<<": "<<kv.second->type_->getsize()<<std::endl;
       }
   }

private:
    Scope scope_;
    std::vector<std::pair<std::string,Identifier*>>list_;
    symbolTable* parent_;


    //disable copy and assign
    symbolTable  ( const symbolTable&) = delete;
    symbolTable& operator = (const symbolTable&) = delete;                     

};
}
#endif