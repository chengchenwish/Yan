#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <array>
#include<algorithm>
#include<iomanip>
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
   using Symbol =  std::pair<std::string,Identifier*>;
    symbolTable();
    ~symbolTable();   
    //new 
    void addSymoble(const std::string& name,  Identifier* indenti);
   bool  getIdentiInCurrentScope(const std::string& name,  Identifier** indenti);
    bool  getIdentiInAllScope(const std::string& name,  Identifier** indenti);
   void setParent(symbolTable* parent){ parent_ = parent;}
   void setScope(const Scope s){ scope_ = s;}
   bool existInCurrentScope(const std::string& name);
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
   void dumpSymbol( std::ostream& os);
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