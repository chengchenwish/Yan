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
   void setParent(symbolTable* parent){ parent_ = parent;}
   void setScope(const Scope s){ scope_ = s;}

private:
     Scope scope_;
    std::map<std::string, Identifier*> map_;
    symbolTable* parent_;


    //disable copy and assign
    symbolTable  ( const symbolTable&) = delete;
    symbolTable& operator = (const symbolTable&) = delete;                     

};
}
#endif