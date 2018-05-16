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
#endif
#endif /* ZR_ENVIRONMENT */

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
#ifdef ZR_DEFINE_IMPLEMENTATION
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

#ifndef ZRP_ALLOCATOR_LINKAGE
#if defined(ZR_ALLOCATOR_SPECIFY_INTERNAL_LINKAGE)                             \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_ALLOCATOR_LINKAGE static
#else
#define ZRP_ALLOCATOR_LINKAGE extern
#endif
#endif /* ZRP_ALLOCATOR_LINKAGE */

#ifdef __cplusplus
extern "C" {
#endif

ZRP_ALLOCATOR_LINKAGE void *
zrAllocate(ZrSize size) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_LINKAGE void *
zrReallocate(void *pOriginal, ZrSize size) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_LINKAGE void
zrFree(void *pMemory) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_LINKAGE void *
zrAllocateAligned(ZrSize size, ZrSize alignment) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_LINKAGE void *
zrReallocateAligned(void *pOriginal,
                    ZrSize size,
                    ZrSize alignment) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_LINKAGE void
zrFreeAligned(void *pMemory) ZRP_MAYBE_UNUSED;

#ifdef __cplusplus
}
#endif

#endif /* ZERO_ALLOCATOR_H */

#ifdef ZR_DEFINE_IMPLEMENTATION
#ifndef ZRP_ALLOCATOR_IMPLEMENTATION_DEFINED
#define ZRP_ALLOCATOR_IMPLEMENTATION_DEFINED

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef ZR_ALLOCATOR_ASSERT
#ifdef ZR_ASSERT
#define ZR_ALLOCATOR_ASSERT ZR_ASSERT
#else
#include <assert.h>
#define ZR_ALLOCATOR_ASSERT assert
#endif /* ZR_ASSERT */
#endif /* ZR_ALLOCATOR_ASSERT */

#ifndef ZR_ALLOCATOR_MALLOC
#ifdef ZR_MALLOC
#define ZR_ALLOCATOR_MALLOC ZR_MALLOC
#else
#include <stdlib.h>
#define ZR_ALLOCATOR_MALLOC malloc
#endif /* ZR_MALLOC */
#endif /* ZR_ALLOCATOR_MALLOC */

#ifndef ZR_ALLOCATOR_REALLOC
#ifdef ZR_REALLOC
#define ZR_ALLOCATOR_REALLOC ZR_REALLOC
#else
#include <stdlib.h>
#define ZR_ALLOCATOR_REALLOC realloc
#endif /* ZR_REALLOC */
#endif /* ZR_ALLOCATOR_REALLOC */

#ifndef ZR_ALLOCATOR_FREE
#ifdef ZR_FREE
#define ZR_ALLOCATOR_FREE ZR_FREE
#else
#include <stdlib.h>
#define ZR_ALLOCATOR_FREE free
#endif /* ZR_FREE */
#endif /* ZR_ALLOCATOR_FREE */

#ifdef ZR_ALLOCATOR_ENABLE_DEBUGGING
#define ZRP_ALLOCATOR_DEBUGGING 1
#else
#define ZRP_ALLOCATOR_DEBUGGING 0
#endif

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
    ((ZrpAllocatorAlignedHeader *)(pBuffer))[-1]
#define ZRP_ALLOCATOR_GET_ALIGNED_BUFFER(pBlock, alignment)                    \
    (void *)((uintptr_t)((unsigned char *)(pBlock) + (alignment)-1             \
                         + sizeof(ZrpAllocatorAlignedHeader))                  \
             & ~(uintptr_t)((alignment)-1))

#define ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment)                  \
    (size) + (alignment)-1 + sizeof(ZrpAllocatorAlignedHeader)

typedef struct ZrpAllocatorAlignedHeader {
    ptrdiff_t offset;
    size_t size;
#if ZRP_ALLOCATOR_DEBUGGING
    size_t alignment;
#endif /* ZRP_ALLOCATOR_DEBUGGING */
} ZrpAllocatorAlignedHeader;

typedef char zrp_allocator_invalid_aligned_header_alignment
    [ZRP_ALLOCATOR_IS_POWER_OF_TWO(
         ZRP_ALLOCATOR_GET_ALIGNMENT_OF(ZrpAllocatorAlignedHeader))
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
    = ZRP_ALLOCATOR_GET_ALIGNMENT_OF(ZrpAllocatorAlignedHeader) > sizeof(void *)
          ? ZRP_ALLOCATOR_GET_ALIGNMENT_OF(ZrpAllocatorAlignedHeader)
          : sizeof(void *);

static int
zrpAllocatorIsPowerOfTwo(ZrSize x)
{
    /* Decrement and compare approach. */
    return (x != 0) && !(x & (x - 1));
}

ZRP_ALLOCATOR_LINKAGE void *
zrAllocate(ZrSize size)
{
    if (size == 0) {
        return NULL;
    }

    return ZR_ALLOCATOR_MALLOC((size_t)size);
}

ZRP_ALLOCATOR_LINKAGE void *
zrReallocate(void *pOriginalBuffer, ZrSize size)
{
    if (pOriginalBuffer == NULL) {
        return zrAllocate(size);
    }

    if (size == 0) {
        zrFree(pOriginalBuffer);
        return NULL;
    }

    return ZR_ALLOCATOR_REALLOC(pOriginalBuffer, (size_t)size);
}

ZRP_ALLOCATOR_LINKAGE void
zrFree(void *pBuffer)
{
    ZR_ALLOCATOR_FREE(pBuffer);
}

ZRP_ALLOCATOR_LINKAGE void *
zrAllocateAligned(ZrSize size, ZrSize alignment)
{
    void *pBuffer;
    void *pBlock;
    ZrpAllocatorAlignedHeader *pHeader;

    if (size == 0 || !zrpAllocatorIsPowerOfTwo(alignment)) {
        return NULL;
    }

    if (alignment < zrpAllocatorMinAlignment) {
        alignment = (ZrSize)zrpAllocatorMinAlignment;
    }

    pBlock = ZR_ALLOCATOR_MALLOC(
        (size_t)ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment));
    if (pBlock == NULL) {
        return NULL;
    }

    pBuffer = ZRP_ALLOCATOR_GET_ALIGNED_BUFFER(pBlock, alignment);

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer);
    pHeader->offset = (unsigned char *)pBuffer - (unsigned char *)pBlock;
    pHeader->size = size;
#if ZRP_ALLOCATOR_DEBUGGING
    pHeader->alignment = (size_t)alignment;
#endif /* ZRP_ALLOCATOR_DEBUGGING */

    return pBuffer;
}

ZRP_ALLOCATOR_LINKAGE void *
zrReallocateAligned(void *pOriginalBuffer, ZrSize size, ZrSize alignment)
{
    void *pBuffer;
    ZrpAllocatorAlignedHeader originalHeader;
    void *pOriginalBlock;
    void *pBlock;
    ZrpAllocatorAlignedHeader *pHeader;

    if (pOriginalBuffer == NULL) {
        return zrAllocateAligned(size, alignment);
    }

    if (size == 0 || !zrpAllocatorIsPowerOfTwo(alignment)) {
        zrFreeAligned(pOriginalBuffer);
        return NULL;
    }

    if (alignment < zrpAllocatorMinAlignment) {
        alignment = (ZrSize)zrpAllocatorMinAlignment;
    }

    originalHeader = ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pOriginalBuffer);
#if ZRP_ALLOCATOR_DEBUGGING
    ZR_ALLOCATOR_ASSERT(alignment == originalHeader.alignment);
#endif /* ZRP_ALLOCATOR_DEBUGGING */

    pOriginalBlock = ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(pOriginalBuffer,
                                                     originalHeader.offset);
    pBlock = ZR_ALLOCATOR_REALLOC(
        pOriginalBlock,
        (size_t)ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment));
    if (pBlock == NULL) {
        return NULL;
    }

    if (pBlock == pOriginalBlock) {
        /* `realloc()` expanded the block in place. */
        ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pOriginalBuffer).size = size;
        return pOriginalBuffer;
    }

    pBuffer = ZRP_ALLOCATOR_GET_ALIGNED_BUFFER(pBlock, alignment);

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer);
    pHeader->offset = (unsigned char *)pBuffer - (unsigned char *)pBlock;
    pHeader->size = size;

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

ZRP_ALLOCATOR_LINKAGE void
zrFreeAligned(void *pBuffer)
{
    ZrpAllocatorAlignedHeader *pHeader;

    if (pBuffer == NULL) {
        return;
    }

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer);
    ZR_ALLOCATOR_FREE(
        ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(pBuffer, pHeader->offset));
}

#endif /* ZRP_ALLOCATOR_IMPLEMENTATION */
#endif /* ZR_IMPLEMENTATION */
