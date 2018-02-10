/* @include "partials/license.h" */

#ifndef ZERO_LOGGER_H
#define ZERO_LOGGER_H

#include <stdarg.h>

#define ZR_LOGGER_MAJOR_VERSION 0
#define ZR_LOGGER_MINOR_VERSION 1
#define ZR_LOGGER_PATCH_VERSION 0

/* @include "partials/platform.h" */
/* @include "partials/unused.h" */

#ifndef ZRP_LOGGER_SCOPE
#ifdef ZR_LOGGER_STATIC
#define ZRP_LOGGER_SCOPE static
#else
#define ZRP_LOGGER_SCOPE extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ZrLogLevel {
    ZR_LOG_LEVEL_DEBUG = 0,
    ZR_LOG_LEVEL_INFO = 1,
    ZR_LOG_LEVEL_WARNING = 2,
    ZR_LOG_LEVEL_ERROR = 3
} ZrLogLevel;

ZRP_LOGGER_SCOPE void
zrLog(ZrLogLevel level, const char *pFile, int line, const char *pFormat, ...)
    ZRP_MAYBE_UNUSED;

ZRP_LOGGER_SCOPE void
zrLogVaList(ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args) ZRP_MAYBE_UNUSED;

#ifdef __cplusplus
}
#endif

#endif /* ZERO_LOGGER_H */

#ifdef ZR_IMPLEMENTATION
#ifndef ZRP_LOGGER_IMPLEMENTATION
#define ZRP_LOGGER_IMPLEMENTATION

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#ifndef ZR_ASSERT
#include <assert.h>
#define ZR_ASSERT assert
#endif /* ZR_ASSERT */

#if !defined(ZR_LOGGER_NO_COLORING) && defined(ZRP_PLATFORM_UNIX)              \
    && _POSIX_C_SOURCE >= 1
#include <unistd.h>
#define ZRP_LOGGER_COLORING
#endif

#ifdef ZRP_LOGGER_COLORING
typedef enum ZrpAnsiColor {
    ZRP_ANSI_COLOR_RESET = 0,
    ZRP_ANSI_COLOR_BLACK = 1,
    ZRP_ANSI_COLOR_RED = 2,
    ZRP_ANSI_COLOR_GREEN = 3,
    ZRP_ANSI_COLOR_YELLOW = 4,
    ZRP_ANSI_COLOR_BLUE = 5,
    ZRP_ANSI_COLOR_MAGENTA = 6,
    ZRP_ANSI_COLOR_CYAN = 7,
    ZRP_ANSI_COLOR_BRIGHT_BLACK = 8,
    ZRP_ANSI_COLOR_BRIGHT_RED = 9,
    ZRP_ANSI_COLOR_BRIGHT_GREEN = 10,
    ZRP_ANSI_COLOR_BRIGHT_YELLOW = 11,
    ZRP_ANSI_COLOR_BRIGHT_BLUE = 12,
    ZRP_ANSI_COLOR_BRIGHT_MAGENTA = 13,
    ZRP_ANSI_COLOR_BRIGHT_CYAN = 14,
    ZRP_ANSI_COLOR_ENUM_LAST = ZRP_ANSI_COLOR_BRIGHT_CYAN,
    ZRP_ANSI_COLOR_ENUM_COUNT = ZRP_ANSI_COLOR_ENUM_LAST + 1
} ZrpAnsiColor;
#endif /* ZRP_LOGGER_COLORING */

static const char *
zrpGetLogLevelString(ZrLogLevel level)
{
    switch (level) {
        case ZR_LOG_LEVEL_DEBUG:
            return "debug";
        case ZR_LOG_LEVEL_INFO:
            return "info";
        case ZR_LOG_LEVEL_WARNING:
            return "warning";
        case ZR_LOG_LEVEL_ERROR:
            return "error";
        default:
            return "invalid";
    }
}

#ifdef ZRP_LOGGER_COLORING
static ZrpAnsiColor
zrpGetLogLevelColor(ZrLogLevel level)
{
    switch (level) {
        case ZR_LOG_LEVEL_DEBUG:
            return ZRP_ANSI_COLOR_BRIGHT_MAGENTA;
        case ZR_LOG_LEVEL_INFO:
            return ZRP_ANSI_COLOR_BRIGHT_CYAN;
        case ZR_LOG_LEVEL_WARNING:
            return ZRP_ANSI_COLOR_BRIGHT_YELLOW;
        case ZR_LOG_LEVEL_ERROR:
            return ZRP_ANSI_COLOR_BRIGHT_RED;
        default:
            ZR_ASSERT(0);
            return ZRP_ANSI_COLOR_RESET;
    };
}

static const char *
zrpGetAnsiColorString(ZrpAnsiColor color)
{
    switch (color) {
        case ZRP_ANSI_COLOR_RESET:
            return "\x1b[0m";
        case ZRP_ANSI_COLOR_BLACK:
            return "\x1b[30m";
        case ZRP_ANSI_COLOR_RED:
            return "\x1b[31m";
        case ZRP_ANSI_COLOR_GREEN:
            return "\x1b[32m";
        case ZRP_ANSI_COLOR_YELLOW:
            return "\x1b[33m";
        case ZRP_ANSI_COLOR_BLUE:
            return "\x1b[34m";
        case ZRP_ANSI_COLOR_MAGENTA:
            return "\x1b[35m";
        case ZRP_ANSI_COLOR_CYAN:
            return "\x1b[36m";
        case ZRP_ANSI_COLOR_BRIGHT_BLACK:
            return "\x1b[1;30m";
        case ZRP_ANSI_COLOR_BRIGHT_RED:
            return "\x1b[1;31m";
        case ZRP_ANSI_COLOR_BRIGHT_GREEN:
            return "\x1b[1;32m";
        case ZRP_ANSI_COLOR_BRIGHT_YELLOW:
            return "\x1b[1;33m";
        case ZRP_ANSI_COLOR_BRIGHT_BLUE:
            return "\x1b[1;34m";
        case ZRP_ANSI_COLOR_BRIGHT_MAGENTA:
            return "\x1b[1;35m";
        case ZRP_ANSI_COLOR_BRIGHT_CYAN:
            return "\x1b[1;36m";
        default:
            ZR_ASSERT(0);
            return "";
    }
}
#endif /* ZRP_LOGGER_COLORING */

ZRP_LOGGER_SCOPE void
zrLog(ZrLogLevel level, const char *pFile, int line, const char *pFormat, ...)
{
    va_list args;

    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    va_start(args, pFormat);
    zrLogVaList(level, pFile, line, pFormat, args);
    va_end(args);
}

ZRP_LOGGER_SCOPE void
zrLogVaList(ZrLogLevel level,
            const char *pFile,
            int line,
            const char *pFormat,
            va_list args)
{
    const char *pLevelName;
    const char *pLevelColorStart;
    const char *pLevelColorEnd;

    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    pLevelName = zrpGetLogLevelString(level);

#ifdef ZRP_LOGGER_COLORING
    if (isatty(fileno(stderr))) {
        ZrpAnsiColor levelColor;

        levelColor = zrpGetLogLevelColor(level);
        pLevelColorStart = zrpGetAnsiColorString(levelColor);
        pLevelColorEnd = zrpGetAnsiColorString(ZRP_ANSI_COLOR_RESET);
    } else {
        pLevelColorStart = pLevelColorEnd = "";
    }
#else
    pLevelColorStart = pLevelColorEnd = "";
#endif

    fprintf(stderr,
            "%s:%d: %s%s%s: ",
            pFile,
            line,
            pLevelColorStart,
            pLevelName,
            pLevelColorEnd);
    vfprintf(stderr, pFormat, args);
}

#endif /* ZRP_LOGGER_IMPLEMENTATION */
#endif /* ZR_IMPLEMENTATION */
