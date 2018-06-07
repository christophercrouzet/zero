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
zrpLogVaList(enum ZrLogLevel level,
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
zrpLog(enum ZrLogLevel level,
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
