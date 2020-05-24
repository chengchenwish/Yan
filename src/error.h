#ifndef _ERROR_H_
#define _ERROR_H_
#include "token.h"
namespace Yan
{

void ExitWithError(const char* format, ...);

void Info(const char*format, ...);

void ExitWithError(const location& loc, const char* format, ...);

};
#endif