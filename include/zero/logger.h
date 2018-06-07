/*
   The MIT License (MIT)

   Copyright (c) 2018 Christopher Crouzet

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#ifndef ZERO_LOGGER_H
#define ZERO_LOGGER_H

#include <stdarg.h>

#define ZR_LOGGER_MAJOR_VERSION 0
#define ZR_LOGGER_MINOR_VERSION 1
#define ZR_LOGGER_PATCH_VERSION 1

#ifndef ZRP_PLATFORM_DEFINED
#define ZRP_PLATFORM_DEFINED
#if defined(_WIN32)
#define ZRP_PLATFORM_WINDOWS
#elif defined(__unix__) || defined(__APPLE__)
#define ZRP_PLATFORM_UNIX
#if defined(__APPLE__)
#define ZRP_PLATFORM_DARWIN
#if TARGET_OS_IPHONE == 1
#define ZRP_PLATFORM_IOS
#elif TARGET_OS_MAC == 1
#define ZRP_PLATFORM_MACOS
#endif
#elif defined(__linux__)
#define ZRP_PLATFORM_LINUX
#endif
#endif
#endif /* ZRP_PLATFORM_DEFINED */

#if defined(ZR_LOGGER_SPECIFY_INTERNAL_LINKAGE)                                \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_LOGGER_LINKAGE static
#elif defined(__cplusplus)
#define ZRP_LOGGER_LINKAGE extern "C"
#else
#define ZRP_LOGGER_LINKAGE extern
#endif

#ifndef ZRP_LOGLEVEL_DEFINED
#define ZRP_LOGLEVEL_DEFINED

enum ZrLogLevel {
    ZR_LOG_LEVEL_ERROR = 0,
    ZR_LOG_LEVEL_WARNING = 1,
    ZR_LOG_LEVEL_INFO = 2,
    ZR_LOG_LEVEL_TRACE = 3,
    ZR_LOG_LEVEL_DEBUG = 4
};

#endif /* ZRP_LOGLEVEL_DEFINED */

ZRP_LOGGER_LINKAGE void
zrLog(enum ZrLogLevel level,
      const char *pFile,
      int line,
      const char *pFormat,
      ...);

ZRP_LOGGER_LINKAGE void
zrLogVaList(enum ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args);

#endif /* ZERO_LOGGER_H */

#ifdef ZR_DEFINE_IMPLEMENTATION
#ifndef ZRP_LOGGER_IMPLEMENTATION_DEFINED
#define ZRP_LOGGER_IMPLEMENTATION_DEFINED

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#ifndef ZR_ASSERT
#include <assert.h>
#define ZR_ASSERT assert
#endif /* ZR_ASSERT */

#ifndef ZRP_UNUSED_DEFINED
#define ZRP_UNUSED_DEFINED
#ifdef __GNUC__
#define ZRP_MAYBE_UNUSED __attribute__((unused))
#else
#define ZRP_MAYBE_UNUSED
#endif
#endif /* ZRP_UNUSED_DEFINED */

#ifndef ZRP_LOGGER_DEFINED
#define ZRP_LOGGER_DEFINED

#if !defined(ZR_DISABLE_LOG_STYLING) && defined(ZRP_PLATFORM_UNIX)             \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define ZRP_LOGGER_STYLING 1
#else
#define ZRP_LOGGER_STYLING 0
#endif

#if ZRP_LOGGER_STYLING
enum ZrpLoggerStyle {
    ZRP_LOGGER_STYLE_RESET = 0,
    ZRP_LOGGER_STYLE_BLACK = 1,
    ZRP_LOGGER_STYLE_RED = 2,
    ZRP_LOGGER_STYLE_GREEN = 3,
    ZRP_LOGGER_STYLE_YELLOW = 4,
    ZRP_LOGGER_STYLE_BLUE = 5,
    ZRP_LOGGER_STYLE_MAGENTA = 6,
    ZRP_LOGGER_STYLE_CYAN = 7,
    ZRP_LOGGER_STYLE_BRIGHT_BLACK = 8,
    ZRP_LOGGER_STYLE_BRIGHT_RED = 9,
    ZRP_LOGGER_STYLE_BRIGHT_GREEN = 10,
    ZRP_LOGGER_STYLE_BRIGHT_YELLOW = 11,
    ZRP_LOGGER_STYLE_BRIGHT_BLUE = 12,
    ZRP_LOGGER_STYLE_BRIGHT_MAGENTA = 13,
    ZRP_LOGGER_STYLE_BRIGHT_CYAN = 14
};
#endif /* ZRP_LOGGER_STYLING */

static void
zrpLoggerGetLogLevelName(const char **ppName, enum ZrLogLevel level)
{
    ZR_ASSERT(ppName != NULL);

    switch (level) {
        case ZR_LOG_LEVEL_ERROR:
            *ppName = "error";
            return;
        case ZR_LOG_LEVEL_WARNING:
            *ppName = "warning";
            return;
        case ZR_LOG_LEVEL_INFO:
            *ppName = "info";
            return;
        case ZR_LOG_LEVEL_TRACE:
            *ppName = "trace";
            return;
        case ZR_LOG_LEVEL_DEBUG:
            *ppName = "debug";
            return;
        default:
            ZR_ASSERT(0);
    }
}

#if ZRP_LOGGER_STYLING
static void
zrpLoggerGetLogLevelStyle(enum ZrpLoggerStyle *pStyle, enum ZrLogLevel level)
{
    ZR_ASSERT(pStyle != NULL);

    switch (level) {
        case ZR_LOG_LEVEL_ERROR:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_RED;
            return;
        case ZR_LOG_LEVEL_WARNING:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_YELLOW;
            return;
        case ZR_LOG_LEVEL_INFO:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_GREEN;
            return;
        case ZR_LOG_LEVEL_TRACE:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_CYAN;
            return;
        case ZR_LOG_LEVEL_DEBUG:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_MAGENTA;
            return;
        default:
            ZR_ASSERT(0);
    };
}

static void
zrpLoggerGetStyleAnsiCode(const char **ppCode, enum ZrpLoggerStyle style)
{
    ZR_ASSERT(ppCode != NULL);

    switch (style) {
        case ZRP_LOGGER_STYLE_RESET:
            *ppCode = "\x1b[0m";
            return;
        case ZRP_LOGGER_STYLE_BLACK:
            *ppCode = "\x1b[30m";
            return;
        case ZRP_LOGGER_STYLE_RED:
            *ppCode = "\x1b[31m";
            return;
        case ZRP_LOGGER_STYLE_GREEN:
            *ppCode = "\x1b[32m";
            return;
        case ZRP_LOGGER_STYLE_YELLOW:
            *ppCode = "\x1b[33m";
            return;
        case ZRP_LOGGER_STYLE_BLUE:
            *ppCode = "\x1b[34m";
            return;
        case ZRP_LOGGER_STYLE_MAGENTA:
            *ppCode = "\x1b[35m";
            return;
        case ZRP_LOGGER_STYLE_CYAN:
            *ppCode = "\x1b[36m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_BLACK:
            *ppCode = "\x1b[1;30m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_RED:
            *ppCode = "\x1b[1;31m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_GREEN:
            *ppCode = "\x1b[1;32m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_YELLOW:
            *ppCode = "\x1b[1;33m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_BLUE:
            *ppCode = "\x1b[1;34m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_MAGENTA:
            *ppCode = "\x1b[1;35m";
            return;
        case ZRP_LOGGER_STYLE_BRIGHT_CYAN:
            *ppCode = "\x1b[1;36m";
            return;
        default:
            ZR_ASSERT(0);
    }
}
#endif /* ZRP_LOGGER_STYLING */

ZRP_MAYBE_UNUSED static void
zrpLoggerLogVaList(enum ZrLogLevel level,
                   const char *pFile,
                   int line,
                   const char *pFormat,
                   va_list args)
{
    const char *pLevelName;
    const char *pLevelStyleStart;
    const char *pLevelStyleEnd;

    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    zrpLoggerGetLogLevelName(&pLevelName, level);

#if ZRP_LOGGER_STYLING
    if (isatty(fileno(stderr))) {
        enum ZrpLoggerStyle levelStyle;

        zrpLoggerGetLogLevelStyle(&levelStyle, level);
        zrpLoggerGetStyleAnsiCode(&pLevelStyleStart, levelStyle);
        zrpLoggerGetStyleAnsiCode(&pLevelStyleEnd, ZRP_LOGGER_STYLE_RESET);
    } else {
        pLevelStyleStart = pLevelStyleEnd = "";
    }
#else
    pLevelStyleStart = pLevelStyleEnd = "";
#endif /* ZRP_LOGGER_STYLING */

    fprintf(stderr,
            "%s:%d: %s%s%s: ",
            pFile,
            line,
            pLevelStyleStart,
            pLevelName,
            pLevelStyleEnd);
    vfprintf(stderr, pFormat, args);
}

ZRP_MAYBE_UNUSED static void
zrpLoggerLog(enum ZrLogLevel level,
             const char *pFile,
             int line,
             const char *pFormat,
             ...)
{
    va_list args;

    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    va_start(args, pFormat);
    zrpLoggerLogVaList(level, pFile, line, pFormat, args);
    va_end(args);
}

#endif /* ZRP_LOGGER_DEFINED */

ZRP_MAYBE_UNUSED ZRP_LOGGER_LINKAGE void
zrLog(enum ZrLogLevel level,
      const char *pFile,
      int line,
      const char *pFormat,
      ...)
{
    va_list args;

    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    va_start(args, pFormat);
    zrpLoggerLogVaList(level, pFile, line, pFormat, args);
    va_end(args);
}

ZRP_MAYBE_UNUSED ZRP_LOGGER_LINKAGE void
zrLogVaList(enum ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args)
{
    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    zrpLoggerLogVaList(level, pFile, line, pFormat, args);
}

#endif /* ZRP_LOGGER_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
