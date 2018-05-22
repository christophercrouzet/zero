/* @include "partials/license.h" */

#ifndef ZERO_ALLOCATOR_H
#define ZERO_ALLOCATOR_H

#define ZR_ALLOCATOR_MAJOR_VERSION 0
#define ZR_ALLOCATOR_MINOR_VERSION 1
#define ZR_ALLOCATOR_PATCH_VERSION 1

/* @include "partials/environment.h" */
/* @include "partials/platform.h" */
/* @include "partials/types.h" */
/* @include "partials/unused.h" */

#if defined(ZR_ALLOCATOR_SPECIFY_INTERNAL_LINKAGE)                             \
    || defined(ZR_SPECIFY_INTERNAL_LINKAGE)
#define ZRP_ALLOCATOR_LINKAGE static
#else
#define ZRP_ALLOCATOR_LINKAGE extern
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

ZRP_ALLOCATOR_LINKAGE void *
zrAllocate(ZrSize size);

ZRP_ALLOCATOR_LINKAGE void *
zrReallocate(void *pOriginal, ZrSize size);

ZRP_ALLOCATOR_LINKAGE void
zrFree(void *pMemory);

ZRP_ALLOCATOR_LINKAGE void *
zrAllocateAligned(ZrSize size, ZrSize alignment);

ZRP_ALLOCATOR_LINKAGE void *
zrReallocateAligned(void *pOriginal, ZrSize size, ZrSize alignment);

ZRP_ALLOCATOR_LINKAGE void
zrFreeAligned(void *pMemory);

#ifdef __cplusplus
}
#endif /* __cplusplus */

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

/* @include "partials/logger.h" */
/* @include "partials/logging.h" */

#if defined(ZR_ALLOCATOR_ENABLE_DEBUGGING) || defined(ZR_ENABLE_DEBUGGING)
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

static int
zrpAllocatorIsPowerOfTwo(ZrSize x)
{
    /* Decrement and compare approach. */
    return (x != 0) && !(x & (x - 1));
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
zrReallocate(void *pOriginalBuffer, ZrSize size)
{
    if (pOriginalBuffer == NULL) {
        return zrAllocate(size);
    }

    if (size == 0) {
        ZRP_LOG_INFO("reallocation called with a size of 0\n");
        zrFree(pOriginalBuffer);
        return NULL;
    }

    return ZR_REALLOC(pOriginalBuffer, (size_t)size);
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void
zrFree(void *pBuffer)
{
    ZR_FREE(pBuffer);
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void *
zrAllocateAligned(ZrSize size, ZrSize alignment)
{
    void *pBuffer;
    void *pBlock;
    struct ZrpAllocatorAlignedHeader *pHeader;

    if (size == 0) {
        ZRP_LOG_INFO("allocation called with a size of 0\n");
        return NULL;
    }

    if (!zrpAllocatorIsPowerOfTwo(alignment)) {
        ZRP_LOG_ERROR("invalid argument ‘alignment’ (not a power of two)\n");
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
    pHeader->size = size;
#if ZRP_ALLOCATOR_DEBUGGING
    pHeader->alignment = (size_t)alignment;
#endif /* ZRP_ALLOCATOR_DEBUGGING */

    return pBuffer;
}

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void *
zrReallocateAligned(void *pOriginalBuffer, ZrSize size, ZrSize alignment)
{
    void *pBuffer;
    struct ZrpAllocatorAlignedHeader originalHeader;
    void *pOriginalBlock;
    void *pBlock;
    struct ZrpAllocatorAlignedHeader *pHeader;

    if (pOriginalBuffer == NULL) {
        return zrAllocateAligned(size, alignment);
    }

    if (size == 0) {
        ZRP_LOG_INFO("reallocation called with a size of 0\n");
        zrFreeAligned(pOriginalBuffer);
        return NULL;
    }

    if (!zrpAllocatorIsPowerOfTwo(alignment)) {
        ZRP_LOG_ERROR("invalid argument ‘alignment’ (not a power of two)\n");
        return NULL;
    }

    if (alignment < zrpAllocatorMinAlignment) {
        alignment = (ZrSize)zrpAllocatorMinAlignment;
    }

    originalHeader = ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pOriginalBuffer);
#if ZRP_ALLOCATOR_DEBUGGING
    ZR_ASSERT(alignment == originalHeader.alignment);
#endif /* ZRP_ALLOCATOR_DEBUGGING */

    pOriginalBlock = ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(pOriginalBuffer,
                                                     originalHeader.offset);
    pBlock = ZR_REALLOC(
        pOriginalBlock,
        (size_t)ZRP_ALLOCATOR_GET_ALIGNED_BLOCK_SIZE(size, alignment));
    if (pBlock == NULL) {
        ZRP_LOG_ERROR("failed to allocate the block\n");
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

ZRP_MAYBE_UNUSED ZRP_ALLOCATOR_LINKAGE void
zrFreeAligned(void *pBuffer)
{
    struct ZrpAllocatorAlignedHeader *pHeader;

    if (pBuffer == NULL) {
        return;
    }

    pHeader = &ZRP_ALLOCATOR_GET_ALIGNED_HEADER(pBuffer);
    ZR_FREE(ZRP_ALLOCATOR_GET_ALIGNED_BLOCK(pBuffer, pHeader->offset));
}

#endif /* ZRP_ALLOCATOR_IMPLEMENTATION_DEFINED */
#endif /* ZR_DEFINE_IMPLEMENTATION */
