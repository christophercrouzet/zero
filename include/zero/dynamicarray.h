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

#ifndef ZERO_DYNAMICARRAY_H
#define ZERO_DYNAMICARRAY_H

#define ZR_DYNAMICARRAY_MAJOR_VERSION 0
#define ZR_DYNAMICARRAY_MINOR_VERSION 1
#define ZR_DYNAMICARRAY_PATCH_VERSION 1

#ifndef ZRP_ARCH_DEFINED
#define ZRP_ARCH_DEFINED
#if defined(__x86_64__) || defined(_M_X64)
#define ZRP_ARCH_X86_64
#elif defined(__i386) || defined(_M_IX86)
#define ZRP_ARCH_X86_32
#elif defined(__itanium__) || defined(_M_IA64)
#define ZRP_ARCH_ITANIUM_64
#elif defined(__powerpc64__) || defined(__ppc64__)
#define ZRP_ARCH_POWERPC_64
#elif defined(__powerpc__) || defined(__ppc__)
#define ZRP_ARCH_POWERPC_32
#elif defined(__aarch64__)
#define ZRP_ARCH_ARM_64
#elif defined(__arm__)
#define ZRP_ARCH_ARM_32
#endif
#endif /* ZRP_ARCH_DEFINED */

/*
   The environment macro represents whether the code is to be generated for a
   32-bit or 64-bit target platform. Some CPUs, such as the x86-64 processors,
   allow running code in 32-bit mode if compiled using the -m32 or -mx32
   compiler switches, in which case `ZR_ENVIRONMENT` is set to 32.
*/
#ifndef ZR_ENVIRONMENT
#if (!defined(ZRP_ARCH_X86_64) || defined(__ILP32__))                          \
    && !defined(ZRP_ARCH_ITANIUM_64) && !defined(ZRP_ARCH_POWERPC_64)          \
    && !defined(ZRP_ARCH_ARM_64)
#define ZR_ENVIRONMENT 32
#else
#define ZR_ENVIRONMENT 64
#endif
#ifdef ZR_DEFINE_IMPLEMENTATION
typedef char zrp_invalid_environment_value
    [ZR_ENVIRONMENT == 32 || ZR_ENVIRONMENT == 64 ? 1 : -1];
#endif /* ZR_DEFINE_IMPLEMENTATION */
#endif /* ZR_ENVIRONMENT */

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

#ifndef ZRP_FIXED_TYPES_DEFINED
#define ZRP_FIXED_TYPES_DEFINED
#ifdef ZR_USE_STD_FIXED_TYPES
#include <stdint.h>
typedef int8_t ZrInt8;
typedef uint8_t ZrUint8;
typedef int16_t ZrInt16;
typedef uint16_t ZrUint16;
typedef int32_t ZrInt32;
typedef uint32_t ZrUint32;
typedef int64_t ZrInt64;
typedef uint64_t ZrUint64;
#else
/*
   The focus here is on the common data models, that is ILP32 (most recent
   32-bit systems), LP64 (Unix-like systems), and LLP64 (Windows). All of these
   models have the `char` type set to 8 bits, `short` to 16 bits, `int` to
   32 bits, and `long long` to 64 bits.
*/
#ifdef ZR_INT8
typedef ZR_INT8 ZrInt8;
#else
typedef char ZrInt8;
#endif
#ifdef ZR_UINT8
typedef ZR_UINT8 ZrUint8;
#else
typedef unsigned char ZrUint8;
#endif
#ifdef ZR_INT16
typedef ZR_INT16 ZrInt16;
#else
typedef short ZrInt16;
#endif
#ifdef ZR_UINT16
typedef ZR_UINT16 ZrUint16;
#else
typedef unsigned short ZrUint16;
#endif
#ifdef ZR_INT32
typedef ZR_INT32 ZrInt32;
#else
typedef int ZrInt32;
#endif
#ifdef ZR_UINT32
typedef ZR_UINT32 ZrUint32;
#else
typedef unsigned int ZrUint32;
#endif
#ifdef ZR_INT64
typedef ZR_INT64 ZrInt64;
#else
typedef long long ZrInt64;
#endif
#ifdef ZR_UINT64
typedef ZR_UINT64 ZrUint64;
#else
typedef unsigned long long ZrUint64;
#endif
#endif /* ZR_USE_STD_FIXED_TYPES */
#ifdef ZR_DEFINE_IMPLEMENTATION
typedef char zrp_invalid_int8_type[sizeof(ZrInt8) == 1 ? 1 : -1];
typedef char zrp_invalid_uint8_type[sizeof(ZrUint8) == 1 ? 1 : -1];
typedef char zrp_invalid_int16_type[sizeof(ZrInt16) == 2 ? 1 : -1];
typedef char zrp_invalid_uint16_type[sizeof(ZrUint16) == 2 ? 1 : -1];
typedef char zrp_invalid_int32_type[sizeof(ZrInt32) == 4 ? 1 : -1];
typedef char zrp_invalid_uint32_type[sizeof(ZrUint32) == 4 ? 1 : -1];
typedef char zrp_invalid_int64_type[sizeof(ZrInt64) == 8 ? 1 : -1];
typedef char zrp_invalid_uint64_type[sizeof(ZrUint64) == 8 ? 1 : -1];
#endif /* ZR_DEFINE_IMPLEMENTATION */
#endif /* ZRP_FIXED_TYPES_DEFINED */

#ifndef ZRP_BASIC_TYPES_DEFINED
#define ZRP_BASIC_TYPES_DEFINED
#ifdef ZR_USE_STD_BASIC_TYPES
#include <stddef.h>
typedef size_t ZrSize;
#else
/*
   The C standard provides no guarantees about the size of the type `size_t`,
   and some exotic platforms will in fact provide original values, but this
   should cover most of the use cases.
*/
#ifdef ZR_SIZE_TYPE
typedef ZR_SIZE_TYPE ZrSize;
#elif ZR_ENVIRONMENT == 32
typedef ZrUint32 ZrSize;
#else
typedef ZrUint64 ZrSize;
#endif
#endif
#ifdef ZR_DEFINE_IMPLEMENTATION
typedef char
    zrp_invalid_size_type[sizeof(ZrSize) == sizeof sizeof(void *) ? 1 : -1];
#endif /* ZR_DEFINE_IMPLEMENTATION */
#endif /* ZRP_BASIC_TYPES_DEFINED */

#ifndef ZRP_STATUS_DEFINED
#define ZRP_STATUS_DEFINED
enum ZrStatus {
    ZR_SUCCESS = 0,
    ZR_ERROR = -1,
    ZR_ERROR_ALLOCATION = -2,
    ZR_ERROR_MAX_SIZE_EXCEEDED = -3
};
#endif /* ZRP_STATUS_DEFINED */

#if defined(ZR_DYNAMICARRAY_SPECIFY_INTERNAL_LINKAGE)                          \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_DYNAMICARRAY_LINKAGE static
#elif defined(__cplusplus)
#define ZRP_DYNAMICARRAY_LINKAGE extern "C"
#else
#define ZRP_DYNAMICARRAY_LINKAGE extern
#endif

#define ZRP_DYNAMIC_ARRAY_DECLARE_CREATE_FUNCTION(name, type)                  \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrCreate##name(type **ppArray,      \
                                                          ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_DESTROY_FUNCTION(name, type)                 \
    ZRP_DYNAMICARRAY_LINKAGE void zrDestroy##name(type *pArray)

#define ZRP_DYNAMIC_ARRAY_DECLARE_GET_SIZE_FUNCTION(name, type)                \
    ZRP_DYNAMICARRAY_LINKAGE void zrGet##name##Size(const type *pArray,        \
                                                    ZrSize *pSize)

#define ZRP_DYNAMIC_ARRAY_DECLARE_GET_CAPACITY_FUNCTION(name, type)            \
    ZRP_DYNAMICARRAY_LINKAGE void zrGet##name##Capacity(const type *pArray,    \
                                                        ZrSize *pCapacity)

#define ZRP_DYNAMIC_ARRAY_DECLARE_GET_MAX_CAPACITY_FUNCTION(name, type)        \
    ZRP_DYNAMICARRAY_LINKAGE void zrGet##name##MaxCapacity(const type *pArray, \
                                                           ZrSize *pCapacity)

#define ZRP_DYNAMIC_ARRAY_DECLARE_RESIZE_FUNCTION(name, type)                  \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrResize##name(type **ppArray,      \
                                                          ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_RESERVE_FUNCTION(name, type)                 \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrReserve##name(type **ppArray,     \
                                                           ZrSize capacity)

#define ZRP_DYNAMIC_ARRAY_DECLARE_EXTEND_FUNCTION(name, type)                  \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrExtend##name(                     \
        type **ppSlice, type **ppArray, ZrSize position, ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_EXTEND_FRONT_FUNCTION(name, type)            \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrExtend##name##Front(              \
        type **ppSlice, type **ppArray, ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_EXTEND_BACK_FUNCTION(name, type)             \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrExtend##name##Back(               \
        type **ppSlice, type **ppArray, ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_INSERT_FUNCTION(name, type)                  \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrInsert##name(                     \
        type **ppArray, ZrSize position, ZrSize size, const type *pValues)

#define ZRP_DYNAMIC_ARRAY_DECLARE_INSERT_FRONT_FUNCTION(name, type)            \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrInsert##name##Front(              \
        type **ppArray, ZrSize size, const type *pValues)

#define ZRP_DYNAMIC_ARRAY_DECLARE_INSERT_BACK_FUNCTION(name, type)             \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrInsert##name##Back(               \
        type **ppArray, ZrSize size, const type *pValues)

#define ZRP_DYNAMIC_ARRAY_DECLARE_PUSH_FUNCTION(name, type)                    \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrPush##name(                       \
        type **ppArray, ZrSize position, type value)

#define ZRP_DYNAMIC_ARRAY_DECLARE_PUSH_FRONT_FUNCTION(name, type)              \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrPush##name##Front(type **ppArray, \
                                                               type value)

#define ZRP_DYNAMIC_ARRAY_DECLARE_PUSH_BACK_FUNCTION(name, type)               \
    ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrPush##name##Back(type **ppArray,  \
                                                              type value)

#define ZRP_DYNAMIC_ARRAY_DECLARE_TRIM_FUNCTION(name, type)                    \
    ZRP_DYNAMICARRAY_LINKAGE void zrTrim##name(                                \
        type *pArray, ZrSize position, ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_TRIM_FRONT_FUNCTION(name, type)              \
    ZRP_DYNAMICARRAY_LINKAGE void zrTrim##name##Front(type *pArray, ZrSize size)

#define ZRP_DYNAMIC_ARRAY_DECLARE_TRIM_BACK_FUNCTION(name, type)               \
    ZRP_DYNAMICARRAY_LINKAGE void zrTrim##name##Back(type *pArray, ZrSize size)

#define ZR_MAKE_DYNAMIC_ARRAY(name, type)                                      \
    ZRP_DYNAMIC_ARRAY_DECLARE_MAX_CAPACITY(name, type)                         \
    ZRP_DYNAMIC_ARRAY_DECLARE_CREATE_FUNCTION(name, type)                      \
    ZRP_DYNAMIC_ARRAY_DECLARE_DESTROY_FUNCTION(name, type)                     \
    ZRP_DYNAMIC_ARRAY_DECLARE_GET_SIZE_FUNCTION(name, type)                    \
    ZRP_DYNAMIC_ARRAY_DECLARE_GET_CAPACITY_FUNCTION(name, type)                \
    ZRP_DYNAMIC_ARRAY_DECLARE_GET_MAX_CAPACITY_FUNCTION(name, type)            \
    ZRP_DYNAMIC_ARRAY_DECLARE_RESIZE_FUNCTION(name, type)                      \
    ZRP_DYNAMIC_ARRAY_DECLARE_RESERVE_FUNCTION(name, type)                     \
    ZRP_DYNAMIC_ARRAY_DECLARE_EXTEND_FUNCTION(name, type)                      \
    ZRP_DYNAMIC_ARRAY_DECLARE_EXTEND_FRONT_FUNCTION(name, type)                \
    ZRP_DYNAMIC_ARRAY_DECLARE_EXTEND_BACK_FUNCTION(name, type)                 \
    ZRP_DYNAMIC_ARRAY_DECLARE_INSERT_FUNCTION(name, type)                      \
    ZRP_DYNAMIC_ARRAY_DECLARE_INSERT_FRONT_FUNCTION(name, type)                \
    ZRP_DYNAMIC_ARRAY_DECLARE_INSERT_BACK_FUNCTION(name, type)                 \
    ZRP_DYNAMIC_ARRAY_DECLARE_PUSH_FUNCTION(name, type)                        \
    ZRP_DYNAMIC_ARRAY_DECLARE_PUSH_FRONT_FUNCTION(name, type)                  \
    ZRP_DYNAMIC_ARRAY_DECLARE_PUSH_BACK_FUNCTION(name, type)                   \
    ZRP_DYNAMIC_ARRAY_DECLARE_TRIM_FUNCTION(name, type)                        \
    ZRP_DYNAMIC_ARRAY_DECLARE_TRIM_FRONT_FUNCTION(name, type)                  \
    ZRP_DYNAMIC_ARRAY_DECLARE_TRIM_BACK_FUNCTION(name, type)

#endif /* ZERO_DYNAMICARRAY_H */

#ifdef ZR_DEFINE_IMPLEMENTATION
#ifndef ZRP_DYNAMICARRAY_IMPLEMENTATION_DEFINED
#define ZRP_DYNAMICARRAY_IMPLEMENTATION_DEFINED

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#ifndef ZR_ASSERT
#include <assert.h>
#define ZR_ASSERT assert
#endif /* ZR_ASSERT */

#ifndef ZR_REALLOC
#include <stdlib.h>
#define ZR_REALLOC realloc
#endif /* ZR_REALLOC */

#ifndef ZR_FREE
#include <stdlib.h>
#define ZR_FREE free
#endif /* ZR_FREE */

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

#ifndef ZRP_LOGGING_DEFINED
#define ZRP_LOGGING_DEFINED

#if defined(ZR_SET_LOGGING_LEVEL_DEBUG)
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_DEBUG
#elif defined(ZR_SET_LOGGING_LEVEL_TRACE)
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_TRACE
#elif defined(ZR_SET_LOGGING_LEVEL_INFO)
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_INFO
#elif defined(ZR_SET_LOGGING_LEVEL_WARNING)
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_WARNING
#elif defined(ZR_SET_LOGGING_LEVEL_ERROR)
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_ERROR
#elif defined(ZR_ENABLE_DEBUGGING)                                             \
    || (!defined(ZR_DISABLE_DEBUGGING)                                         \
        && (defined(DEBUG) || !defined(NDEBUG)))
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_DEBUG
#else
#define ZRP_LOGGING_LEVEL ZRP_LOG_LEVEL_WARNING
#endif

#ifdef ZR_DISABLE_LOGGING
#define ZRP_LOGGING 0
#else
#define ZRP_LOGGING 1
#endif /* ZR_DISABLE_LOGGING */

#ifndef ZR_LOG
#define ZR_LOG(level, ...)                                                     \
    do {                                                                       \
        if (ZRP_LOGGING && level <= ZRP_LOGGING_LEVEL) {                       \
            zrpLog(level, __FILE__, __LINE__, __VA_ARGS__);                    \
        }                                                                      \
    } while (0)
#endif /* ZR_LOG */

#define ZRP_LOG_DEBUG(...) ZR_LOG(ZRP_LOG_LEVEL_DEBUG, __VA_ARGS__)

#define ZRP_LOG_TRACE(...) ZR_LOG(ZRP_LOG_LEVEL_TRACE, __VA_ARGS__)

#define ZRP_LOG_INFO(...) ZR_LOG(ZRP_LOG_LEVEL_INFO, __VA_ARGS__)

#define ZRP_LOG_WARNING(...) ZR_LOG(ZRP_LOG_LEVEL_WARNING, __VA_ARGS__)

#define ZRP_LOG_ERROR(...) ZR_LOG(ZRP_LOG_LEVEL_ERROR, __VA_ARGS__)

#endif /* ZRP_LOGGING_DEFINED */

#define ZRP_DYNAMIC_ARRAY_GET_BLOCK(pBuffer)                                   \
    ((void *)&((struct ZrpDynamicArrayHeader *)pBuffer)[-1])
#define ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)                                   \
    ((struct ZrpDynamicArrayHeader *)(pBlock))
#define ZRP_DYNAMIC_ARRAY_GET_BUFFER(pBlock)                                   \
    ((void *)&((struct ZrpDynamicArrayHeader *)(pBlock))[1])
#define ZRP_DYNAMIC_ARRAY_GET_CONST_BLOCK(pBuffer)                             \
    ((const void *)&((const struct ZrpDynamicArrayHeader *)pBuffer)[-1])
#define ZRP_DYNAMIC_ARRAY_GET_CONST_HEADER(pBlock)                             \
    ((const struct ZrpDynamicArrayHeader *)(pBlock))

#define ZRP_DYNAMIC_ARRAY_DEFINE_MAX_CAPACITY(name, type)                      \
    static const size_t zrpMax##name##Capacity                                 \
        = (((size_t)-1 - sizeof(struct ZrpDynamicArrayHeader))                 \
           / sizeof(type));

#define ZRP_DYNAMIC_ARRAY_DEFINE_CREATE_FUNCTION(name, type)                   \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrCreate##name(    \
        type **ppArray, ZrSize size)                                           \
    {                                                                          \
        enum ZrStatus status;                                                  \
        void *pBlock;                                                          \
                                                                               \
        ZR_ASSERT(ppArray != NULL);                                            \
                                                                               \
        pBlock = NULL;                                                         \
                                                                               \
        status = zrpDynamicArrayEnsureHasEnoughCapacity(                       \
            &pBlock, 0, (size_t)size, zrpMax##name##Capacity, sizeof(type));   \
        if (status != ZR_SUCCESS) {                                            \
            ZRP_LOG_ERROR("failed to reserve a large enough capacity for the " \
                          "type ‘" #type "’ (requested capacity: %zu)\n",      \
                          (size_t)size);                                       \
            return status;                                                     \
        }                                                                      \
                                                                               \
        ZR_ASSERT(pBlock != NULL);                                             \
                                                                               \
        ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size = (size_t)size;             \
        ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->capacity = (size_t)size;         \
        *ppArray = (type *)ZRP_DYNAMIC_ARRAY_GET_BUFFER(pBlock);               \
        return ZR_SUCCESS;                                                     \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_DESTROY_FUNCTION(name, type)                  \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrDestroy##name(            \
        type *pArray)                                                          \
    {                                                                          \
        if (pArray == NULL) {                                                  \
            return;                                                            \
        }                                                                      \
                                                                               \
        ZR_FREE(ZRP_DYNAMIC_ARRAY_GET_BLOCK(pArray));                          \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_GET_SIZE_FUNCTION(name, type)                 \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrGet##name##Size(          \
        const type *pArray, ZrSize *pSize)                                     \
    {                                                                          \
        ZR_ASSERT(pArray != NULL);                                             \
                                                                               \
        *pSize = (ZrSize)ZRP_DYNAMIC_ARRAY_GET_CONST_HEADER(                   \
                     ZRP_DYNAMIC_ARRAY_GET_CONST_BLOCK(pArray))                \
                     ->size;                                                   \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_GET_CAPACITY_FUNCTION(name, type)             \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrGet##name##Capacity(      \
        const type *pArray, ZrSize *pCapacity)                                 \
    {                                                                          \
        ZR_ASSERT(pArray != NULL);                                             \
                                                                               \
        *pCapacity = (ZrSize)ZRP_DYNAMIC_ARRAY_GET_CONST_HEADER(               \
                         ZRP_DYNAMIC_ARRAY_GET_CONST_BLOCK(pArray))            \
                         ->capacity;                                           \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_GET_MAX_CAPACITY_FUNCTION(name, type)         \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrGet##name##MaxCapacity(   \
        ZrSize *pMaxCapacity)                                                  \
    {                                                                          \
        *pMaxCapacity = zrpMax##name##Capacity;                                \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_RESIZE_FUNCTION(name, type)                   \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrResize##name(    \
        type **ppArray, ZrSize size)                                           \
    {                                                                          \
        enum ZrStatus status;                                                  \
        void *pBlock;                                                          \
                                                                               \
        ZR_ASSERT(ppArray != NULL);                                            \
        ZR_ASSERT(*ppArray != NULL);                                           \
                                                                               \
        pBlock = ZRP_DYNAMIC_ARRAY_GET_BLOCK(*ppArray);                        \
                                                                               \
        status = zrpDynamicArrayEnsureHasEnoughCapacity(                       \
            &pBlock,                                                           \
            ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->capacity,                    \
            (size_t)size,                                                      \
            zrpMax##name##Capacity,                                            \
            sizeof(type));                                                     \
        if (status != ZR_SUCCESS) {                                            \
            ZRP_LOG_ERROR("failed to reserve a large enough capacity for the " \
                          "type ‘" #type "’ (requested capacity: %zu)\n",      \
                          (size_t)size);                                       \
            return status;                                                     \
        }                                                                      \
                                                                               \
        ZR_ASSERT(pBlock != NULL);                                             \
                                                                               \
        ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size = (size_t)size;             \
        *ppArray = (type *)ZRP_DYNAMIC_ARRAY_GET_BUFFER(pBlock);               \
        return ZR_SUCCESS;                                                     \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_RESERVE_FUNCTION(name, type)                  \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrReserve##name(   \
        type **ppArray, ZrSize capacity)                                       \
    {                                                                          \
        enum ZrStatus status;                                                  \
        void *pBlock;                                                          \
                                                                               \
        ZR_ASSERT(ppArray != NULL);                                            \
        ZR_ASSERT(*ppArray != NULL);                                           \
                                                                               \
        pBlock = ZRP_DYNAMIC_ARRAY_GET_BLOCK(*ppArray);                        \
                                                                               \
        status = zrpDynamicArrayEnsureHasEnoughCapacity(                       \
            &pBlock,                                                           \
            ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->capacity,                    \
            (size_t)capacity,                                                  \
            zrpMax##name##Capacity,                                            \
            sizeof(type));                                                     \
        if (status != ZR_SUCCESS) {                                            \
            ZRP_LOG_ERROR("failed to reserve a large enough capacity for the " \
                          "type ‘" #type "’ (requested capacity: %zu)\n",      \
                          (size_t)capacity);                                   \
            return status;                                                     \
        }                                                                      \
                                                                               \
        ZR_ASSERT(pBlock != NULL);                                             \
                                                                               \
        *ppArray = (type *)ZRP_DYNAMIC_ARRAY_GET_BUFFER(pBlock);               \
        return ZR_SUCCESS;                                                     \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_EXTEND_FUNCTION(name, type)                   \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrExtend##name(    \
        type **ppSlice, type **ppArray, ZrSize position, ZrSize size)          \
    {                                                                          \
        enum ZrStatus status;                                                  \
        void *pBlock;                                                          \
                                                                               \
        ZR_ASSERT(ppArray != NULL);                                            \
        ZR_ASSERT(*ppArray != NULL);                                           \
                                                                               \
        pBlock = ZRP_DYNAMIC_ARRAY_GET_BLOCK(*ppArray);                        \
                                                                               \
        if (position > ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size) {           \
            position = (ZrSize)ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size;     \
        }                                                                      \
                                                                               \
        status = zrpDynamicArrayEnsureHasEnoughCapacity(                       \
            &pBlock,                                                           \
            ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->capacity,                    \
            ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size + (size_t)size,         \
            zrpMax##name##Capacity,                                            \
            sizeof(type));                                                     \
        if (status != ZR_SUCCESS) {                                            \
            ZRP_LOG_ERROR(                                                     \
                "failed to reserve a large enough capacity for the "           \
                "type ‘" #type "’ (requested capacity: %zu)\n",                \
                ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size + (size_t)size);    \
            return status;                                                     \
        }                                                                      \
                                                                               \
        ZR_ASSERT(pBlock != NULL);                                             \
                                                                               \
        *ppArray = (type *)ZRP_DYNAMIC_ARRAY_GET_BUFFER(pBlock);               \
                                                                               \
        memmove(&(*ppArray)[(size_t)position + (size_t)size],                  \
                &(*ppArray)[(size_t)position],                                 \
                sizeof(type)                                                   \
                    * (ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size              \
                       - (size_t)position));                                   \
                                                                               \
        if (ppSlice != NULL) {                                                 \
            *ppSlice = &(*ppArray)[position];                                  \
        }                                                                      \
                                                                               \
        ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size += (size_t)size;            \
        return ZR_SUCCESS;                                                     \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_EXTEND_FRONT_FUNCTION(name, type)             \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus                    \
        zrExtend##name##Front(type **ppSlice, type **ppArray, ZrSize size)     \
    {                                                                          \
        return zrExtend##name(ppSlice, ppArray, 0, size);                      \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_EXTEND_BACK_FUNCTION(name, type)              \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus                    \
        zrExtend##name##Back(type **ppSlice, type **ppArray, ZrSize size)      \
    {                                                                          \
        return zrExtend##name(ppSlice,                                         \
                              ppArray,                                         \
                              (ZrSize)ZRP_DYNAMIC_ARRAY_GET_HEADER(            \
                                  ZRP_DYNAMIC_ARRAY_GET_BLOCK(*ppArray))       \
                                  ->size,                                      \
                              size);                                           \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_INSERT_FUNCTION(name, type)                   \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrInsert##name(    \
        type **ppArray, ZrSize position, ZrSize size, const type *pValues)     \
    {                                                                          \
        enum ZrStatus status;                                                  \
        type *pSlice;                                                          \
                                                                               \
        ZR_ASSERT(ppArray != NULL);                                            \
        ZR_ASSERT(*ppArray != NULL);                                           \
        ZR_ASSERT(pValues != NULL);                                            \
                                                                               \
        status = zrExtend##name(&pSlice, ppArray, position, size);             \
        if (status != ZR_SUCCESS) {                                            \
            ZRP_LOG_ERROR("failed to extend the array\n");                     \
            return status;                                                     \
        }                                                                      \
                                                                               \
        memcpy(pSlice, pValues, sizeof(type) * (size_t)size);                  \
        return ZR_SUCCESS;                                                     \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_INSERT_FRONT_FUNCTION(name, type)             \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus                    \
        zrInsert##name##Front(                                                 \
            type **ppArray, ZrSize size, const type *pValues)                  \
    {                                                                          \
        return zrInsert##name(ppArray, 0, size, pValues);                      \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_INSERT_BACK_FUNCTION(name, type)              \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus                    \
        zrInsert##name##Back(type **ppArray, ZrSize size, const type *pValues) \
    {                                                                          \
        return zrInsert##name(ppArray, (ZrSize)-1, size, pValues);             \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_PUSH_FUNCTION(name, type)                     \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus zrPush##name(      \
        type **ppArray, ZrSize position, type value)                           \
    {                                                                          \
        enum ZrStatus status;                                                  \
        type *pSlice;                                                          \
                                                                               \
        ZR_ASSERT(ppArray != NULL);                                            \
        ZR_ASSERT(*ppArray != NULL);                                           \
                                                                               \
        status = zrExtend##name(&pSlice, ppArray, position, 1);                \
        if (status != ZR_SUCCESS) {                                            \
            ZRP_LOG_ERROR("failed to extend the array\n");                     \
            return status;                                                     \
        }                                                                      \
                                                                               \
        *pSlice = value;                                                       \
        return ZR_SUCCESS;                                                     \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_PUSH_FRONT_FUNCTION(name, type)               \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus                    \
        zrPush##name##Front(type **ppArray, type value)                        \
    {                                                                          \
        return zrPush##name(ppArray, 0, value);                                \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_PUSH_BACK_FUNCTION(name, type)                \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE enum ZrStatus                    \
        zrPush##name##Back(type **ppArray, type value)                         \
    {                                                                          \
        return zrPush##name(ppArray, (ZrSize)-1, value);                       \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_TRIM_FUNCTION(name, type)                     \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrTrim##name(               \
        type *pArray, ZrSize position, ZrSize size)                            \
    {                                                                          \
        void *pBlock;                                                          \
                                                                               \
        ZR_ASSERT(pArray != NULL);                                             \
                                                                               \
        pBlock = ZRP_DYNAMIC_ARRAY_GET_BLOCK(pArray);                          \
                                                                               \
        if (position >= ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size) {          \
            return;                                                            \
        }                                                                      \
                                                                               \
        if (size > ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size - position) {    \
            size = ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size - position;      \
        }                                                                      \
                                                                               \
        memmove(&pArray[position],                                             \
                &pArray[position + size],                                      \
                sizeof(type) * (size_t)size);                                  \
        ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->size -= (size_t)size;            \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_TRIM_FRONT_FUNCTION(name, type)               \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrTrim##name##Front(        \
        type *pArray, ZrSize size)                                             \
    {                                                                          \
        zrTrim##name(pArray, 0, size);                                         \
    }

#define ZRP_DYNAMIC_ARRAY_DEFINE_TRIM_BACK_FUNCTION(name, type)                \
    ZRP_MAYBE_UNUSED ZRP_DYNAMICARRAY_LINKAGE void zrTrim##name##Back(         \
        type *pArray, ZrSize size)                                             \
    {                                                                          \
        zrTrim##name(pArray, (ZrSize)-1, size);                                \
    }

#undef ZR_MAKE_DYNAMIC_ARRAY
#define ZR_MAKE_DYNAMIC_ARRAY(name, type)                                      \
    ZRP_DYNAMIC_ARRAY_DEFINE_MAX_CAPACITY(name, type)                          \
    ZRP_DYNAMIC_ARRAY_DEFINE_CREATE_FUNCTION(name, type)                       \
    ZRP_DYNAMIC_ARRAY_DEFINE_DESTROY_FUNCTION(name, type)                      \
    ZRP_DYNAMIC_ARRAY_DEFINE_GET_SIZE_FUNCTION(name, type)                     \
    ZRP_DYNAMIC_ARRAY_DEFINE_GET_CAPACITY_FUNCTION(name, type)                 \
    ZRP_DYNAMIC_ARRAY_DEFINE_GET_MAX_CAPACITY_FUNCTION(name, type)             \
    ZRP_DYNAMIC_ARRAY_DEFINE_RESIZE_FUNCTION(name, type)                       \
    ZRP_DYNAMIC_ARRAY_DEFINE_RESERVE_FUNCTION(name, type)                      \
    ZRP_DYNAMIC_ARRAY_DEFINE_EXTEND_FUNCTION(name, type)                       \
    ZRP_DYNAMIC_ARRAY_DEFINE_EXTEND_FRONT_FUNCTION(name, type)                 \
    ZRP_DYNAMIC_ARRAY_DEFINE_EXTEND_BACK_FUNCTION(name, type)                  \
    ZRP_DYNAMIC_ARRAY_DEFINE_INSERT_FUNCTION(name, type)                       \
    ZRP_DYNAMIC_ARRAY_DEFINE_INSERT_FRONT_FUNCTION(name, type)                 \
    ZRP_DYNAMIC_ARRAY_DEFINE_INSERT_BACK_FUNCTION(name, type)                  \
    ZRP_DYNAMIC_ARRAY_DEFINE_PUSH_FUNCTION(name, type)                         \
    ZRP_DYNAMIC_ARRAY_DEFINE_PUSH_FRONT_FUNCTION(name, type)                   \
    ZRP_DYNAMIC_ARRAY_DEFINE_PUSH_BACK_FUNCTION(name, type)                    \
    ZRP_DYNAMIC_ARRAY_DEFINE_TRIM_FUNCTION(name, type)                         \
    ZRP_DYNAMIC_ARRAY_DEFINE_TRIM_FRONT_FUNCTION(name, type)                   \
    ZRP_DYNAMIC_ARRAY_DEFINE_TRIM_BACK_FUNCTION(name, type)

struct ZrpDynamicArrayHeader {
    size_t size;
    size_t capacity;
};

ZRP_MAYBE_UNUSED static void
zrpDynamicArrayGetNewCapacity(size_t *pNewCapacity,
                              size_t current,
                              size_t requested,
                              size_t max)
{
    *pNewCapacity = current + current / 2 + 1;
    if (*pNewCapacity < current) {
        *pNewCapacity = max;
        return;
    }

    if (*pNewCapacity < requested) {
        *pNewCapacity = requested;
    }
}

ZRP_MAYBE_UNUSED static enum ZrStatus
zrpDynamicArrayEnsureHasEnoughCapacity(void **ppBlock,
                                       size_t currentCapacity,
                                       size_t requestedCapacity,
                                       size_t maxCapacity,
                                       size_t elementSize)
{
    void *pBlock;
    size_t newCapacity;

    ZR_ASSERT(ppBlock != NULL);
    ZR_ASSERT(elementSize > 0);

    if (requestedCapacity > maxCapacity) {
        ZRP_LOG_TRACE("the requested capacity is too large\n");
        return ZR_ERROR_MAX_SIZE_EXCEEDED;
    }

    if (*ppBlock != NULL && currentCapacity >= requestedCapacity) {
        return ZR_SUCCESS;
    }

    zrpDynamicArrayGetNewCapacity(
        &newCapacity, currentCapacity, requestedCapacity, maxCapacity);
    ZR_ASSERT(newCapacity >= requestedCapacity);
    ZR_ASSERT(newCapacity <= maxCapacity);

    pBlock = ZR_REALLOC(
        *ppBlock,
        sizeof(struct ZrpDynamicArrayHeader) + elementSize * newCapacity);
    if (pBlock == NULL) {
        ZRP_LOG_TRACE("failed to reallocate the block\n");
        return ZR_ERROR_ALLOCATION;
    }

    ZRP_DYNAMIC_ARRAY_GET_HEADER(pBlock)->capacity = newCapacity;
    *ppBlock = pBlock;
    return ZR_SUCCESS;
}

#endif /* ZRP_DYNAMICARRAY_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
