#ifndef _ERROR_H_
#define _ERROR_H_
#include "token.h"
namespace Yan
{

void Error(const char* format, ...);

void Info(const char*format, ...);

void Error(const token* tok, const char* format, ...);

};
#endif