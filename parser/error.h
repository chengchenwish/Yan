#ifndef _ERROR_H_
#define _ERROR_H_
#include "token.h"
namespace Yan
{

void Error(const char* format, ...);

void Info(const char*format, ...);

void Error(const location& loc, const char* format, ...);

};
#endif