#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include "token.h"

struct symbol
{

    std::string name;//name of symbol
};

class symbolTable
{
private:
    static constexpr int MAX_TABLE_SIZE = 1024;
};

#endif