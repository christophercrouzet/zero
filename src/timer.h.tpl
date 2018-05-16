/* @include "partials/license.h" */

#ifndef ZERO_TIMER_H
#define ZERO_TIMER_H

#define ZR_TIMER_MAJOR_VERSION 0
#define ZR_TIMER_MINOR_VERSION 1
#define ZR_TIMER_PATCH_VERSION 0

/* @include "partials/environment.h" */
/* @include "partials/platform.h" */
/* @include "partials/types.h" */
/* @include "partials/unused.h" */

/* @include "partials/status.h" */

#ifndef ZRP_TIMER_LINKAGE
#if defined(ZR_TIMER_SPECIFY_INTERNAL_LINKAGE)                                 \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_TIMER_LINKAGE static
#else
#define ZRP_TIMER_LINKAGE extern
#endif
#endif /* ZRP_TIMER_LINKAGE */

#define ZR_TIMER_TICKS_PER_SECOND 1000000000ull

typedef struct ZrCpuTimes {
    ZrUint64 user;
    ZrUint64 system;
} ZrCpuTimes;

#ifdef __cplusplus
extern "C" {
#endif

ZRP_TIMER_LINKAGE ZrStatus
zrGetRealTime(ZrUint64 *pTime) ZRP_MAYBE_UNUSED;

ZRP_TIMER_LINKAGE ZrStatus
zrGetCpuTimes(ZrCpuTimes *pTimes) ZRP_MAYBE_UNUSED;

#ifdef __cplusplus
}
#endif

#endif /* ZERO_TIMER_H */

#ifdef ZR_DEFINE_IMPLEMENTATION
#ifndef ZRP_TIMER_IMPLEMENTATION_DEFINED
#define ZRP_TIMER_IMPLEMENTATION_DEFINED

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

ZRP_TIMER_LINKAGE ZrStatus
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
            *pTime = (ZrUint64)time.tv_sec * 1000000000ull
                     + (ZrUint64)time.tv_nsec;
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

ZRP_TIMER_LINKAGE ZrStatus
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
