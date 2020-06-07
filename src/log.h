#ifndef _LOG_H_
#define _LOG_H_
#include <ostream>
#include <sstream>
#include <iostream>
#include "token.h"
namespace Yan
{
    enum Loglevel
    {
        L_DEBUG,
        L_NOTICE
    };
    class logger
    {
    public:
        logger(std::ostream &os, const char *func, int line, Loglevel level);
        std::stringstream &stream() { return impl.stream; }

    private:
        struct Impl
        {
            Impl(std::ostream &stream, const char *func, int line, Loglevel level);
            ~Impl() { os << stream.str() << std::endl; }
            std::ostream &os;
            //    std::string func;
            //    int line;
            //    Loglevel l;
            std::stringstream stream;
        };
        Impl impl;
    };

    extern Loglevel log_level;
#define DEBUG_LOG             \
    if (log_level <= L_DEBUG) \
        logger(std::cout, __func__, __LINE__, L_DEBUG).stream()
#define DEBUG(format, ...)    \
    if (log_level <= L_DEBUG) \
    Info(format, ##__VA_ARGS__)

#define NOTICE(format, ...)    \
    if (log_level <= L_NOTICE) \
    Info(format, ##__VA_ARGS__)

    void ExitWithError(const char *format, ...);

    void Info(const char *format, ...);

    void ExitWithError(const location &loc, const char *format, ...);

}; // namespace Yan
#endif