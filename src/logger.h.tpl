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

/* @include "partials/logger.h" */

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

    ZR_ASSERT((int)ZR_LOG_LEVEL_ERROR == (int)ZRP_LOG_LEVEL_ERROR);
    ZR_ASSERT((int)ZR_LOG_LEVEL_WARNING == (int)ZRP_LOG_LEVEL_WARNING);
    ZR_ASSERT((int)ZR_LOG_LEVEL_INFO == (int)ZRP_LOG_LEVEL_INFO);
    ZR_ASSERT((int)ZR_LOG_LEVEL_TRACE == (int)ZRP_LOG_LEVEL_TRACE);
    ZR_ASSERT((int)ZR_LOG_LEVEL_DEBUG == (int)ZRP_LOG_LEVEL_DEBUG);

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
    ZR_ASSERT(pFile != NULL);
    ZR_ASSERT(pFormat != NULL);

    ZR_ASSERT((int)ZR_LOG_LEVEL_ERROR == (int)ZRP_LOG_LEVEL_ERROR);
    ZR_ASSERT((int)ZR_LOG_LEVEL_WARNING == (int)ZRP_LOG_LEVEL_WARNING);
    ZR_ASSERT((int)ZR_LOG_LEVEL_INFO == (int)ZRP_LOG_LEVEL_INFO);
    ZR_ASSERT((int)ZR_LOG_LEVEL_TRACE == (int)ZRP_LOG_LEVEL_TRACE);
    ZR_ASSERT((int)ZR_LOG_LEVEL_DEBUG == (int)ZRP_LOG_LEVEL_DEBUG);

    zrpLogVaList((enum ZrpLogLevel)level, pFile, line, pFormat, args);
}

#endif /* ZRP_LOGGER_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
