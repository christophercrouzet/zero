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

#ifndef ZERO_ALLOCATOR_H
#define ZERO_ALLOCATOR_H

#define ZR_ALLOCATOR_MAJOR_VERSION 0
#define ZR_ALLOCATOR_MINOR_VERSION 1
#define ZR_ALLOCATOR_PATCH_VERSION 1

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

#if defined(ZR_ALLOCATOR_SPECIFY_INTERNAL_LINKAGE)                             \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_ALLOCATOR_LINKAGE static
#elif defined(__cplusplus)
#define ZRP_ALLOCATOR_LINKAGE extern "C"
#else
#define ZRP_ALLOCATOR_LINKAGE extern
#endif

ZRP_ALLOCATOR_LINKAGE void *
zrAllocate(ZrSize size);

ZRP_ALLOCATOR_LINKAGE void *
zrReallocate(void *pOriginal, ZrSize size);

ZRP_ALLOCATOR_LINKAGE void
zrFree(const void *pMemory);

ZRP_ALLOCATOR_LINKAGE void *
zrAllocateAligned(ZrSize size, ZrSize alignment);

ZRP_ALLOCATOR_LINKAGE void *
zrReallocateAligned(void *pOriginal, ZrSize size, ZrSize alignment);

ZRP_ALLOCATOR_LINKAGE void
zrFreeAligned(const void *pMemory);

#endif /* ZERO_ALLOCATOR_H */

#ifdef ZR_DEFINE_IMPLEMENTATION
#ifndef ZRP_ALLOCATOR_IMPLEMENTATION_DEFINED
#define ZRP_ALLOCATOR_IMPLEMENTATION_DEFINED

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifndef ZR_ASSERT
#include <assert.h>
#define ZR_ASSERT assert
#endif /* ZR_ASSERT */

#ifndef ZR_MALLOC
#include <stdlib.h>
#define ZR_MALLOC malloc
#endif /* ZR_MALLOC */

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

static void
zrpLoggerGetLogLevelName(const char **ppName, enum ZrpLogLevel level)
{
    ZR_ASSERT(ppName != NULL);

    switch (level) {
        case ZRP_LOG_LEVEL_ERROR:
            *ppName = "error";
            return;
        case ZRP_LOG_LEVEL_WARNING:
            *ppName = "warning";
            return;
        case ZRP_LOG_LEVEL_INFO:
            *ppName = "info";
            return;
        case ZRP_LOG_LEVEL_TRACE:
            *ppName = "trace";
            return;
        case ZRP_LOG_LEVEL_DEBUG:
            *ppName = "debug";
            return;
        default:
            ZR_ASSERT(0);
    }
}

#if ZRP_LOGGER_STYLING
static void
zrpLoggerGetLogLevelStyle(enum ZrpLoggerStyle *pStyle, enum ZrpLogLevel level)
{
    ZR_ASSERT(pStyle != NULL);

    switch (level) {
        case ZRP_LOG_LEVEL_ERROR:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_RED;
            return;
        case ZRP_LOG_LEVEL_WARNING:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_YELLOW;
            return;
        case ZRP_LOG_LEVEL_INFO:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_GREEN;
            return;
        case ZRP_LOG_LEVEL_TRACE:
            *pStyle = ZRP_LOGGER_STYLE_BRIGHT_CYAN;
            return;
        case ZRP_LOG_LEVEL_DEBUG:
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

#if defined(ZR_ALLOCATOR_ENABLE_DEBUGGING) || defined(ZR_ENABLE_DEBUGGING)
#define ZRP_ALLOCATOR_DEBUGGING 1
#else
#define ZRP_ALLOCATOR_DEBUGGING 0
#endif

#define ZRP_ALLOCATOR_CAST_CONST(type, x) (type)(uintptr_t)(x)

#ifdef __cplusplus
template<typename T>
struct ZrpAllocatorAlignmentOfHelper {
    char first;
    T second;
};
#define ZRP_ALLOCATOR_GET_ALIGNMENT_OF(type)                                   \
    offsetof(ZrpAllocatorAlignmentOfHelper<type>, second)
#else
#define ZRP_ALLOCATOR_GET_ALIGNMENT_OF(type)                                   \
    offsetof(                                                                  \
        struct {                                                               \
            char first;                                                        \
            type second;                                                       \
        },                                                                     \
        second)
#endif /* __cplusplus */

#define ZRP_ALLOCATOR_IS_POWER_OF_TWO(x)                                       \
    ((x) == 1 || (x) == 2 || (x) == 4 || (x) == 8 || (x) == 16 || (x) == 32    \
     || (x) == 64 || (x) == 128 || (x) == 256 || (x) == 512 || (x) == 1024     \
     || (x) == 2048 || (x) == 4096 || (x) == 8192 || (x) == 16384              \
     || (x) == 32768 || (x) == 65536 || (x) == 131072 || (x) == 262144         \
     || (x) == 524288 || (x) == 1048576 || (x) == 2097152 || (x) == 4194304    \
     || (x) == 8388608 || (x) == 16777216 || (x) == 33554432                   \
     || (x) == 67108864 || (x) == 134217728 || (x) == 268435456                \
     || (x) == 1073741824 || (x) == 2147483648 || (x) == 4294967296)

/*
   The aligned allocator works by allocating a block of larger size than
   requested to hold some necessary bookkeeping and some padding space to
   secure the desired alignment.

     block           user pointer
      /                  /
     +---------+--------+------+---------+
     | padding | header | size | padding |
     +---------+--------+------+---------+
      \                  \
       0                offset

   The sum of the front and back paddings equals `alignment - 1`, and the
   pointer returned to the user is located at a distance of `offset` from
   the beginning of the block.
*/

#define ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(pBuffer, offset)                       \
    (void *)((unsigned char *)(pBuffer) - (offset))
#define ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer)                              \
    ((struct ZrpAllocatorAlignedHeader *)(pBuffer))[-1]
#define ZRP_ALLOCATOR_GET_ALIGNED_BUFFER(pBlock, alignment)                    \
    (void *)((uintptr_t)((unsigned char *)(pBlock) + (alignment)-1             \
                         + sizeof(struct ZrpAllocatorAlignedHeader))           \
             & ~(uintptr_t)((alignment)-1))

#define ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment)                  \
    (size) + (alignment)-1 + sizeof(struct ZrpAllocatorAlignedHeader)

struct ZrpAllocatorAlignedHeader {
    ptrdiff_t offset;
    size_t size;
#if ZRP_ALLOCATOR_DEBUGGING
    size_t alignment;
#endif /* ZRP_ALLOCATOR_DEBUGGING */
};

typedef char zrp_allocator_invalid_aligned_header_alignment
    [ZRP_ALLOCATOR_IS_POWER_OF_TWO(
         ZRP_ALLOCATOR_GET_ALIGNMENT_OF(struct ZrpAllocatorAlignedHeader))
         ? 1
         : -1];
typedef char zrp_allocator_invalid_void_pointer_alignment
    [ZRP_ALLOCATOR_IS_POWER_OF_TWO(sizeof(void *)) ? 1 : -1];

/*
   Any power of two alignment requested for the user pointer that is greater or
   equal to this minimum value is guaranteed to be a multiple of
   `sizeof(void *)`, thus conforming to the requirement of `posix_memalign()`,
   and is also guaranteed to provide correct alignment for the block header.
*/
static const size_t zrpAllocatorMinAlignment
    = ZRP_ALLOCATOR_GET_ALIGNMENT_OF(struct ZrpAllocatorAlignedHeader)
              > sizeof(void *)
          ? ZRP_ALLOCATOR_GET_ALIGNMENT_OF(struct ZrpAllocatorAlignedHeader)
          : sizeof(void *);

ZRP_MAYBE_UNUSED static void
zrpAllocatorIsPowerOfTwo(int *pResult, size_t x)
{
    ZR_ASSERT(pResult != NULL);

    /* Decrement and compare approach. */
    *pResult = (x != 0) && !(x & (x - 1));
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void *
zrAllocate(ZrSize size)
{
    if (size == 0) {
        ZRP_LOG_INFO("allocation called with a size of 0\n");
        return NULL;
    }

    return ZR_MALLOC((size_t)size);
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void *
zrReallocate(void *pOriginal, ZrSize size)
{
    if (pOriginal == NULL) {
        return zrAllocate(size);
    }

    if (size == 0) {
        ZRP_LOG_INFO("reallocation called with a size of 0\n");
        zrFree(pOriginal);
        return NULL;
    }

    return ZR_REALLOC(pOriginal, (size_t)size);
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void
zrFree(const void *pMemory)
{
    ZR_FREE(ZRP_ALLOCATOR_CAST_CONST(void *, pMemory));
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void *
zrAllocateAligned(ZrSize size, ZrSize alignment)
{
    int valid;
    void *pBuffer;
    void *pBlock;
    struct ZrpAllocatorAlignedHeader *pHeader;

    zrpAllocatorIsPowerOfTwo(&valid, (size_t)alignment);
    ZR_ASSERT(valid);

    if (size == 0) {
        ZRP_LOG_INFO("allocation called with a size of 0\n");
        return NULL;
    }

    if (alignment < zrpAllocatorMinAlignment) {
        alignment = (ZrSize)zrpAllocatorMinAlignment;
    }

    pBlock = ZR_MALLOC(
        (size_t)ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment));
    if (pBlock == NULL) {
        ZRP_LOG_ERROR("failed to allocate the block\n");
        return NULL;
    }

    pBuffer = ZRP_ALLOCATOR_GET_ALIGNED_BUFFER(pBlock, alignment);

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer);
    pHeader->offset = (unsigned char *)pBuffer - (unsigned char *)pBlock;
    pHeader->size = (size_t)size;
#if ZRP_ALLOCATOR_DEBUGGING
    pHeader->alignment = (size_t)alignment;
#endif /* ZRP_ALLOCATOR_DEBUGGING */

    return pBuffer;
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void *
zrReallocateAligned(void *pOriginal, ZrSize size, ZrSize alignment)
{
    int valid;
    void *pBuffer;
    struct ZrpAllocatorAlignedHeader originalHeader;
    void *pOriginalBlock;
    void *pBlock;
    struct ZrpAllocatorAlignedHeader *pHeader;

    zrpAllocatorIsPowerOfTwo(&valid, (size_t)alignment);
    ZR_ASSERT(valid);

    if (pOriginal == NULL) {
        return zrAllocateAligned(size, alignment);
    }

    if (size == 0) {
        ZRP_LOG_INFO("reallocation called with a size of 0\n");
        zrFreeAligned(pOriginal);
        return NULL;
    }

    if (alignment < zrpAllocatorMinAlignment) {
        alignment = (ZrSize)zrpAllocatorMinAlignment;
    }

    originalHeader = ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pOriginal);
#if ZRP_ALLOCATOR_DEBUGGING
    ZR_ASSERT(alignment == originalHeader.alignment);
#endif /* ZRP_ALLOCATOR_DEBUGGING */

    pOriginalBlock
        = ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(pOriginal, originalHeader.offset);
    pBlock = ZR_REALLOC(
        pOriginalBlock,
        (size_t)ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment));
    if (pBlock == NULL) {
        ZRP_LOG_ERROR("failed to allocate the block\n");
        return NULL;
    }

    if (pBlock == pOriginalBlock) {
        /* `realloc()` expanded the block in place. */
        ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pOriginal).size = (size_t)size;
        return pOriginal;
    }

    pBuffer = ZRP_ALLOCATOR_GET_ALIGNED_BUFFER(pBlock, alignment);

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer);
    pHeader->offset = (unsigned char *)pBuffer - (unsigned char *)pBlock;
    pHeader->size = (size_t)size;

    if (pHeader->offset == originalHeader.offset) {
        /*
           `realloc()` allocated a new block that is still correctly
           aligned.
        */
        return pBuffer;
    }

    memmove(pBuffer,
            (void *)((unsigned char *)pBlock + originalHeader.offset),
            originalHeader.size);

    return pBuffer;
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void
zrFreeAligned(const void *pMemory)
{
    struct ZrpAllocatorAlignedHeader *pHeader;

    if (pMemory == NULL) {
        return;
    }

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(
        ZRP_ALLOCATOR_CAST_CONST(void *, pMemory));
    ZR_FREE(ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(
        ZRP_ALLOCATOR_CAST_CONST(void *, pMemory), pHeader->offset));
}

#endif /* ZRP_ALLOCATOR_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
