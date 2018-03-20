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

#ifndef ZERO_TIMER_H
#define ZERO_TIMER_H

#define ZR_TIMER_MAJOR_VERSION 0
#define ZR_TIMER_MINOR_VERSION 1
#define ZR_TIMER_PATCH_VERSION 0

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
#ifdef ZR_IMPLEMENTATION
typedef char zrp_invalid_environment_value
    [ZR_ENVIRONMENT == 32 || ZR_ENVIRONMENT == 64 ? 1 : -1];
#endif
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
#ifdef ZR_IMPLEMENTATION
typedef char zrp_invalid_int8_type[sizeof(ZrInt8) == 1 ? 1 : -1];
typedef char zrp_invalid_uint8_type[sizeof(ZrUint8) == 1 ? 1 : -1];
typedef char zrp_invalid_int16_type[sizeof(ZrInt16) == 2 ? 1 : -1];
typedef char zrp_invalid_uint16_type[sizeof(ZrUint16) == 2 ? 1 : -1];
typedef char zrp_invalid_int32_type[sizeof(ZrInt32) == 4 ? 1 : -1];
typedef char zrp_invalid_uint32_type[sizeof(ZrUint32) == 4 ? 1 : -1];
typedef char zrp_invalid_int64_type[sizeof(ZrInt64) == 8 ? 1 : -1];
typedef char zrp_invalid_uint64_type[sizeof(ZrUint64) == 8 ? 1 : -1];
#endif
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
#ifdef ZR_IMPLEMENTATION
typedef char
    zrp_invalid_size_type[sizeof(ZrSize) == sizeof sizeof(void *) ? 1 : -1];
#endif
#endif /* ZRP_BASIC_TYPES_DEFINED */

#ifndef ZRP_UNUSED_DEFINED
#define ZRP_UNUSED_DEFINED
#ifdef __GNUC__
#define ZRP_MAYBE_UNUSED __attribute__((unused))
#else
#define ZRP_MAYBE_UNUSED
#endif
#endif /* ZRP_UNUSED_DEFINED */

#ifndef ZRP_RESULT_DEFINED
#define ZRP_RESULT_DEFINED
typedef enum ZrResult { ZR_SUCCESS = 0, ZR_ERROR = -1 } ZrResult;
#endif /* ZRP_RESULT_DEFINED */

#ifndef ZRP_TIMER_SCOPE
#if defined(ZR_TIMER_STATIC) || defined(ZR_STATIC)
#define ZRP_TIMER_SCOPE static
#else
#define ZRP_TIMER_SCOPE extern
#endif
#endif /* ZRP_TIMER_SCOPE */

#define ZR_TIMER_TICKS_PER_SECOND 1000000000ull

typedef struct ZrCpuTimes {
    ZrUint64 user;
    ZrUint64 system;
} ZrCpuTimes;

#ifdef __cplusplus
extern "C" {
#endif

ZRP_TIMER_SCOPE ZrResult
zrGetRealTime(ZrUint64 *pTime) ZRP_MAYBE_UNUSED;

ZRP_TIMER_SCOPE ZrResult
zrGetCpuTimes(ZrCpuTimes *pTimes) ZRP_MAYBE_UNUSED;

#ifdef __cplusplus
}
#endif

#endif /* ZERO_TIMER_H */

#ifdef ZR_IMPLEMENTATION
#ifndef ZRP_TIMER_IMPLEMENTATION
#define ZRP_TIMER_IMPLEMENTATION

#ifndef ZR_TIMER_ASSERT
#ifdef ZR_ASSERT
#define ZR_TIMER_ASSERT ZR_ASSERT
#else
#include <assert.h>
#define ZR_TIMER_ASSERT assert
#endif /* ZR_ASSERT */
#endif /* ZR_TIMER_ASSERT */

#if defined(ZRP_PLATFORM_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(ZRP_PLATFORM_UNIX)
#include <sys/resource.h>
#include <sys/time.h>
#include <time.h>
#if defined(ZRP_PLATFORM_DARWIN)
#include <mach/mach_time.h>
#elif _POSIX_C_SOURCE >= 199309L
#define ZRP_TIMER_USE_CLOCK_GETTIME
#if defined(CLOCK_MONOTONIC_RAW)
#define ZRP_TIMER_CLOCK_ID CLOCK_MONOTONIC_RAW
#elif defined(CLOCK_MONOTONIC)
#define ZRP_TIMER_CLOCK_ID CLOCK_MONOTONIC
#else
#define ZRP_TIMER_CLOCK_ID CLOCK_REALTIME
#endif
#endif
#else
typedef char zrp_timer_unsupported_platform[-1];
#endif

/*
    In some cases, tick values are casted to 64-bit floating-points in order
    to prevent (unlikely) integer overflows during some arithmetic operations.
    This effectively reducse the precision of the ticks having a value greater
    than 2^53, which corresponds to approximatively 104 days.
*/

ZRP_TIMER_SCOPE ZrResult
zrGetRealTime(ZrUint64 *pTime)
{
    ZR_TIMER_ASSERT(pTime != NULL);

#if defined(ZRP_PLATFORM_WINDOWS)
    {
        static double timeToNano;
        LARGE_INTEGER time;

        if (timeToNano == 0.0) {
            LARGE_INTEGER frequency;

            if (!QueryPerformanceFrequency(&frequency)) {
                return ZR_ERROR;
            }

            timeToNano = (double)ZR_TIMER_TICKS_PER_SECOND / frequency.QuadPart;
        }

        if (!QueryPerformanceCounter(&time)) {
            return ZR_ERROR;
        }

        *pTime = time.QuadPart * timeToNano;
        return ZR_SUCCESS;
    }
#elif defined(ZRP_PLATFORM_DARWIN)
    /*
       Since Darwin 5.2, `clock_gettime()` can return high resolution times
       with the `CLOCK_UPTIME_RAW` clock but it internally only calls
       `mach_absolute_time()` with the overhead of converting the result into
       the `timespec` format.
    */
    {
        static double timeToNano;

        if (timeToNano == 0.0) {
            mach_timebase_info_data_t info;

            if (mach_timebase_info(&info) != KERN_SUCCESS) {
                return ZR_ERROR;
            }

            timeToNano = (double)info.numer / info.denom;
        }

        *pTime = mach_absolute_time() * timeToNano;
        return ZR_SUCCESS;
    }
#elif defined(ZRP_PLATFORM_UNIX)
#if defined(ZRP_TIMER_USE_CLOCK_GETTIME)
    {
        struct timespec time;

        if (clock_gettime(ZRP_TIMER_CLOCK_ID, &time) == 0) {
            *pTime = (ZrUint64)time.tv_sec * 1000000000ull + time.tv_nsec;
            return ZR_SUCCESS;
        }
    }
#else
    {
        struct timeval time;

        if (gettimeofday(&time, NULL) == 0) {
            *pTime = (ZrUint64)time.tv_sec * 1000000000ull
                     + (ZrUint64)time.tv_usec * 1000ull;
            return ZR_SUCCESS;
        }
    }
#endif
#endif

    return ZR_ERROR;
}

ZRP_TIMER_SCOPE ZrResult
zrGetCpuTimes(ZrCpuTimes *pTimes)
{
    ZR_TIMER_ASSERT(pTimes != NULL);

#if defined(ZRP_PLATFORM_WINDOWS)
    {
        FILETIME creationTime;
        FILETIME exitTime;
        FILETIME kernelTime;
        FILETIME userTime;
        ULARGE_INTEGER time;

        if (!GetProcessTimes(GetCurrentProcess(),
                             &creationTime,
                             &exitTime,
                             &kernelTime,
                             &userTime)) {
            return ZR_ERROR;
        }

        time.LowPart = userTime.dwLowDateTime;
        time.HighPart = userTime.dwHighDateTime;
        *pTimes->user = time.QuadPart * 100ull;

        time.LowPart = kernelTime.dwLowDateTime;
        time.HighPart = kernelTime.dwHighDateTime;
        *pTimes->system = time.QuadPart * 100ull;
    }
#elif defined(ZRP_PLATFORM_UNIX)
    {
        struct rusage usage;

        if (getrusage(RUSAGE_SELF, &usage)) {
            return ZR_ERROR;
        }

        pTimes->user = (ZrUint64)usage.ru_utime.tv_sec * 1000000000ull
                       + (ZrUint64)usage.ru_utime.tv_usec * 1000ull;
        pTimes->system = (ZrUint64)usage.ru_stime.tv_sec * 1000000000ull
                         + (ZrUint64)usage.ru_stime.tv_usec * 1000ull;
        return ZR_SUCCESS;
    }
#endif

    return ZR_ERROR;
}

#endif /* ZRP_TIMER_IMPLEMENTATION */
#endif /* ZR_IMPLEMENTATION */
