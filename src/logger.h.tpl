/* @include "partials/license.h" */

#ifndef ZERO_LOGGER_H
#define ZERO_LOGGER_H

#include <stdarg.h>

#define ZR_LOGGER_MAJOR_VERSION 0
#define ZR_LOGGER_MINOR_VERSION 1
#define ZR_LOGGER_PATCH_VERSION 1

/* @include "partials/platform.h" */

#if defined(ZR_LOGGER_SPECIFY_INTERNAL_LINKAGE)                                \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_LOGGER_LINKAGE static
#elif defined(__cplusplus)
#define ZRP_LOGGER_LINKAGE extern "C"
#else
#define ZRP_LOGGER_LINKAGE extern
#endif

/* @include "partials/loglevel.h" */

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

/* @include "partials/unused.h" */
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
