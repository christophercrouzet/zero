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
