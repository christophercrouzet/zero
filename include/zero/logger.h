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

#ifndef ZRP_UNUSED_DEFINED
#define ZRP_UNUSED_DEFINED
#ifdef __GNUC__
#define ZRP_MAYBE_UNUSED __attribute__((unused))
#else
#define ZRP_MAYBE_UNUSED
#endif
#endif /* ZRP_UNUSED_DEFINED */

#if defined(ZR_LOGGER_SPECIFY_INTERNAL_LINKAGE)                                \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_LOGGER_LINKAGE static
#else
#define ZRP_LOGGER_LINKAGE extern
#endif

enum ZrLogLevel {
    ZR_LOG_LEVEL_ERROR = 0,
    ZR_LOG_LEVEL_WARNING = 1,
    ZR_LOG_LEVEL_INFO = 2,
    ZR_LOG_LEVEL_TRACE = 3,
    ZR_LOG_LEVEL_DEBUG = 4
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

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

#ifdef __cplusplus
}
#endif /* __cplusplus */

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

#ifndef ZRP_LOGGER_DEFINED
#define ZRP_LOGGER_DEFINED

#if !defined(ZR_DISABLE_LOG_STYLING) && defined(ZRP_PLATFORM_UNIX)             \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define ZRP_LOGGER_STYLING 1
#else
#define ZRP_LOGGER_STYLING 0
#endif

enum ZrpLogLevel {
    ZRP_LOG_LEVEL_ERROR = 0,
    ZRP_LOG_LEVEL_WARNING = 1,
    ZRP_LOG_LEVEL_INFO = 2,
    ZRP_LOG_LEVEL_TRACE = 3,
    ZRP_LOG_LEVEL_DEBUG = 4
};

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

static const char *
zrpLoggerGetLogLevelName(enum ZrpLogLevel level)
{
    switch (level) {
        case ZRP_LOG_LEVEL_ERROR:
            return "error";
        case ZRP_LOG_LEVEL_WARNING:
            return "warning";
        case ZRP_LOG_LEVEL_INFO:
            return "info";
        case ZRP_LOG_LEVEL_TRACE:
            return "trace";
        case ZRP_LOG_LEVEL_DEBUG:
            return "debug";
        default:
            ZR_ASSERT(0);
            return "invalid";
    }
}

#if ZRP_LOGGER_STYLING
static enum ZrpLoggerStyle
zrpLoggerGetLogLevelStyle(enum ZrpLogLevel level)
{
    switch (level) {
        case ZRP_LOG_LEVEL_ERROR:
            return ZRP_LOGGER_STYLE_BRIGHT_RED;
        case ZRP_LOG_LEVEL_WARNING:
            return ZRP_LOGGER_STYLE_BRIGHT_YELLOW;
        case ZRP_LOG_LEVEL_INFO:
            return ZRP_LOGGER_STYLE_BRIGHT_GREEN;
        case ZRP_LOG_LEVEL_TRACE:
            return ZRP_LOGGER_STYLE_BRIGHT_CYAN;
        case ZRP_LOG_LEVEL_DEBUG:
            return ZRP_LOGGER_STYLE_BRIGHT_MAGENTA;
        default:
            ZR_ASSERT(0);
            return ZRP_LOGGER_STYLE_RESET;
    };
}

static const char *
zrpLoggerGetStyleAnsiCode(enum ZrpLoggerStyle style)
{
    switch (style) {
        case ZRP_LOGGER_STYLE_RESET:
            return "\x1b[0m";
        case ZRP_LOGGER_STYLE_BLACK:
            return "\x1b[30m";
        case ZRP_LOGGER_STYLE_RED:
            return "\x1b[31m";
        case ZRP_LOGGER_STYLE_GREEN:
            return "\x1b[32m";
        case ZRP_LOGGER_STYLE_YELLOW:
            return "\x1b[33m";
        case ZRP_LOGGER_STYLE_BLUE:
            return "\x1b[34m";
        case ZRP_LOGGER_STYLE_MAGENTA:
            return "\x1b[35m";
        case ZRP_LOGGER_STYLE_CYAN:
            return "\x1b[36m";
        case ZRP_LOGGER_STYLE_BRIGHT_BLACK:
            return "\x1b[1;30m";
        case ZRP_LOGGER_STYLE_BRIGHT_RED:
            return "\x1b[1;31m";
        case ZRP_LOGGER_STYLE_BRIGHT_GREEN:
            return "\x1b[1;32m";
        case ZRP_LOGGER_STYLE_BRIGHT_YELLOW:
            return "\x1b[1;33m";
        case ZRP_LOGGER_STYLE_BRIGHT_BLUE:
            return "\x1b[1;34m";
        case ZRP_LOGGER_STYLE_BRIGHT_MAGENTA:
            return "\x1b[1;35m";
        case ZRP_LOGGER_STYLE_BRIGHT_CYAN:
            return "\x1b[1;36m";
        default:
            ZR_ASSERT(0);
            return "";
    }
}
#endif /* ZRP_LOGGER_STYLING */

ZRP_MAYBE_UNUSED static void
zrpLogVaList(enum ZrpLogLevel level,
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

    pLevelName = zrpLoggerGetLogLevelName(level);

#if ZRP_LOGGER_STYLING
    if (isatty(fileno(stderr))) {
        enum ZrpLoggerStyle levelStyle;

        levelStyle = zrpLoggerGetLogLevelStyle(level);
        pLevelStyleStart = zrpLoggerGetStyleAnsiCode(levelStyle);
        pLevelStyleEnd = zrpLoggerGetStyleAnsiCode(ZRP_LOGGER_STYLE_RESET);
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
zrpLog(enum ZrpLogLevel level,
       const char *pFile,
       int line,
       const char *pFormat,
       ...)
{
    va_list args;

    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    va_start(args, pFormat);
    zrpLogVaList(level, pFile, line, pFormat, args);
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

    if (pFile == NULL) {
        return;
    }

    if (pFormat == NULL) {
        return;
    }

    ZR_ASSERT(ZR_LOG_LEVEL_ERROR == ZRP_LOG_LEVEL_ERROR);
    ZR_ASSERT(ZR_LOG_LEVEL_WARNING == ZRP_LOG_LEVEL_WARNING);
    ZR_ASSERT(ZR_LOG_LEVEL_INFO == ZRP_LOG_LEVEL_INFO);
    ZR_ASSERT(ZR_LOG_LEVEL_TRACE == ZRP_LOG_LEVEL_TRACE);
    ZR_ASSERT(ZR_LOG_LEVEL_DEBUG == ZRP_LOG_LEVEL_DEBUG);

    va_start(args, pFormat);
    zrpLogVaList((enum ZrpLogLevel)level, pFile, line, pFormat, args);
    va_end(args);
}

ZRP_MAYBE_UNUSED ZRP_LOGGER_LINKAGE void
zrLogVaList(enum ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args)
{
    if (pFile == NULL) {
        return;
    }

    if (pFormat == NULL) {
        return;
    }

    ZR_ASSERT(ZR_LOG_LEVEL_ERROR == ZRP_LOG_LEVEL_ERROR);
    ZR_ASSERT(ZR_LOG_LEVEL_WARNING == ZRP_LOG_LEVEL_WARNING);
    ZR_ASSERT(ZR_LOG_LEVEL_INFO == ZRP_LOG_LEVEL_INFO);
    ZR_ASSERT(ZR_LOG_LEVEL_TRACE == ZRP_LOG_LEVEL_TRACE);
    ZR_ASSERT(ZR_LOG_LEVEL_DEBUG == ZRP_LOG_LEVEL_DEBUG);

    zrpLogVaList((enum ZrpLogLevel)level, pFile, line, pFormat, args);
}

#endif /* ZRP_LOGGER_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
