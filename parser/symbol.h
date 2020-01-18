#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <array>
#include "token.h"
namespace Yan
{


struct symbol
{
    symbol(std::string&s)
    {
        this->name = s;
    }
    symbol()=default;

    std::string name;//name of symbol
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

private:
    static constexpr int MAX_TABLE_SIZE = 1024;
    std::array<symbol,MAX_TABLE_SIZE> table;
    int glob;
    //disable copy and assign
   // symbolTable  ( const symbolTable&) = delete;
   // symbolTable& operator = (const symbolTable&) = delete;

};
}
#endif