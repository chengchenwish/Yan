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
    int findGlob(const std::string& s);
    void addGlob(std::string& s);
    int getGlobIdex() const{ return glob;} 
    symbol& getSymbol(int index){ return table.at(index);}
    //new 
    void addSymoble(const std::string& name, const Identifier& indenti);
   bool  getIdentiInCurrentScope(const std::string& name, const Identifier* indenti)const;
   void setParent(symbolTable* parent){ parent_ = parent;}
   void setScope(const Scope s){ scope_ = s;}

private:
    static constexpr int MAX_TABLE_SIZE = 1024;
    std::array<symbol,MAX_TABLE_SIZE> table;
    int glob;
    Scope scope_;
    std::map<std::string, Identifier> map_;
    symbolTable* parent_;


    //disable copy and assign
   // symbolTable  ( const symbolTable&) = delete;
   // symbolTable& operator = (const symbolTable&) = delete;

};
}
#endif