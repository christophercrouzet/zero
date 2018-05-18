/* @include "partials/license.h" */

#ifndef ZERO_LOGGER_H
#define ZERO_LOGGER_H

#include <stdarg.h>

#define ZR_LOGGER_MAJOR_VERSION 0
#define ZR_LOGGER_MINOR_VERSION 1
#define ZR_LOGGER_PATCH_VERSION 1

/* @include "partials/platform.h" */
/* @include "partials/unused.h" */

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
      ...) ZRP_MAYBE_UNUSED;

ZRP_LOGGER_LINKAGE void
zrLogVaList(enum ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args) ZRP_MAYBE_UNUSED;

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

#ifndef ZR_LOGGER_ASSERT
#ifdef ZR_ASSERT
#define ZR_LOGGER_ASSERT ZR_ASSERT
#else
#include <assert.h>
#define ZR_LOGGER_ASSERT assert
#endif /* ZR_ASSERT */
#endif /* ZR_LOGGER_ASSERT */

#if !defined(ZR_DISABLE_LOG_COLORING) && defined(ZRP_PLATFORM_UNIX)            \
    && defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define ZRP_LOGGER_COLORING 1
#else
#define ZRP_LOGGER_COLORING 0
#endif

#if ZRP_LOGGER_COLORING
enum ZrpLoggerAnsiColor {
    ZRP_LOGGER_ANSI_COLOR_RESET = 0,
    ZRP_LOGGER_ANSI_COLOR_BLACK = 1,
    ZRP_LOGGER_ANSI_COLOR_RED = 2,
    ZRP_LOGGER_ANSI_COLOR_GREEN = 3,
    ZRP_LOGGER_ANSI_COLOR_YELLOW = 4,
    ZRP_LOGGER_ANSI_COLOR_BLUE = 5,
    ZRP_LOGGER_ANSI_COLOR_MAGENTA = 6,
    ZRP_LOGGER_ANSI_COLOR_CYAN = 7,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_BLACK = 8,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_RED = 9,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_GREEN = 10,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_YELLOW = 11,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_BLUE = 12,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_MAGENTA = 13,
    ZRP_LOGGER_ANSI_COLOR_BRIGHT_CYAN = 14,
    ZRP_LOGGER_ANSI_COLOR_ENUM_LAST = ZRP_LOGGER_ANSI_COLOR_BRIGHT_CYAN,
    ZRP_LOGGER_ANSI_COLOR_ENUM_COUNT = ZRP_LOGGER_ANSI_COLOR_ENUM_LAST + 1
};
#endif /* ZRP_LOGGER_COLORING */

static const char *
zrpLoggerGetLogLevelString(enum ZrLogLevel level)
{
    switch (level) {
        case ZR_LOG_LEVEL_ERROR:
            return "error";
        case ZR_LOG_LEVEL_WARNING:
            return "warning";
        case ZR_LOG_LEVEL_INFO:
            return "info";
        case ZR_LOG_LEVEL_TRACE:
            return "trace";
        case ZR_LOG_LEVEL_DEBUG:
            return "debug";
        default:
            return "invalid";
    }
}

#if ZRP_LOGGER_COLORING
static enum ZrpLoggerAnsiColor
zrpLoggerGetLogLevelColor(enum ZrLogLevel level)
{
    switch (level) {
        case ZR_LOG_LEVEL_ERROR:
            return ZRP_LOGGER_ANSI_COLOR_BRIGHT_RED;
        case ZR_LOG_LEVEL_WARNING:
            return ZRP_LOGGER_ANSI_COLOR_BRIGHT_YELLOW;
        case ZR_LOG_LEVEL_INFO:
            return ZRP_LOGGER_ANSI_COLOR_BRIGHT_CYAN;
        case ZR_LOG_LEVEL_TRACE:
            return ZRP_LOGGER_ANSI_COLOR_BRIGHT_GREEN;
        case ZR_LOG_LEVEL_DEBUG:
            return ZRP_LOGGER_ANSI_COLOR_BRIGHT_MAGENTA;
        default:
            ZR_LOGGER_ASSERT(0);
            return ZRP_LOGGER_ANSI_COLOR_RESET;
    };
}

static const char *
zrpLoggerGetAnsiColorString(enum ZrpLoggerAnsiColor color)
{
    switch (color) {
        case ZRP_LOGGER_ANSI_COLOR_RESET:
            return "\x1b[0m";
        case ZRP_LOGGER_ANSI_COLOR_BLACK:
            return "\x1b[30m";
        case ZRP_LOGGER_ANSI_COLOR_RED:
            return "\x1b[31m";
        case ZRP_LOGGER_ANSI_COLOR_GREEN:
            return "\x1b[32m";
        case ZRP_LOGGER_ANSI_COLOR_YELLOW:
            return "\x1b[33m";
        case ZRP_LOGGER_ANSI_COLOR_BLUE:
            return "\x1b[34m";
        case ZRP_LOGGER_ANSI_COLOR_MAGENTA:
            return "\x1b[35m";
        case ZRP_LOGGER_ANSI_COLOR_CYAN:
            return "\x1b[36m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_BLACK:
            return "\x1b[1;30m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_RED:
            return "\x1b[1;31m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_GREEN:
            return "\x1b[1;32m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_YELLOW:
            return "\x1b[1;33m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_BLUE:
            return "\x1b[1;34m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_MAGENTA:
            return "\x1b[1;35m";
        case ZRP_LOGGER_ANSI_COLOR_BRIGHT_CYAN:
            return "\x1b[1;36m";
        default:
            ZR_LOGGER_ASSERT(0);
            return "";
    }
}
#endif /* ZRP_LOGGER_COLORING */

ZRP_LOGGER_LINKAGE void
zrLog(enum ZrLogLevel level,
      const char *pFile,
      int line,
      const char *pFormat,
      ...)
{
    va_list args;

    ZR_LOGGER_ASSERT(pFile != NULL);
    ZR_LOGGER_ASSERT(pFormat != NULL);

    va_start(args, pFormat);
    zrLogVaList(level, pFile, line, pFormat, args);
    va_end(args);
}

ZRP_LOGGER_LINKAGE void
zrLogVaList(enum ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args)
{
    const char *pLevelName;
    const char *pLevelColorStart;
    const char *pLevelColorEnd;

    ZR_LOGGER_ASSERT(pFile != NULL);
    ZR_LOGGER_ASSERT(pFormat != NULL);

    pLevelName = zrpLoggerGetLogLevelString(level);

#if ZRP_LOGGER_COLORING
    if (isatty(fileno(stderr))) {
        enum ZrpLoggerAnsiColor levelColor;

        levelColor = zrpLoggerGetLogLevelColor(level);
        pLevelColorStart = zrpLoggerGetAnsiColorString(levelColor);
        pLevelColorEnd
            = zrpLoggerGetAnsiColorString(ZRP_LOGGER_ANSI_COLOR_RESET);
    } else {
        pLevelColorStart = pLevelColorEnd = "";
    }
#else
    pLevelColorStart = pLevelColorEnd = "";
#endif /* ZRP_LOGGER_COLORING */

    fprintf(stderr,
            "%s:%d: %s%s%s: ",
            pFile,
            line,
            pLevelColorStart,
            pLevelName,
            pLevelColorEnd);
    vfprintf(stderr, pFormat, args);
}

#endif /* ZRP_LOGGER_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
