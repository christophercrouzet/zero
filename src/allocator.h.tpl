/* @include "partials/license.h" */

#ifndef ZERO_ALLOCATOR_H
#define ZERO_ALLOCATOR_H

#define ZR_ALLOCATOR_MAJOR_VERSION 0
#define ZR_ALLOCATOR_MINOR_VERSION 1
#define ZR_ALLOCATOR_PATCH_VERSION 0

/* @include "partials/environment.h" */
/* @include "partials/types.h" */
/* @include "partials/unused.h" */

#ifndef ZRP_ALLOCATOR_SCOPE
#if defined(ZR_ALLOCATOR_STATIC) || defined(ZR_STATIC)
#define ZRP_ALLOCATOR_SCOPE static
#else
#define ZRP_ALLOCATOR_SCOPE extern
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

ZRP_ALLOCATOR_SCOPE void *
zrAllocate(ZrSize size) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_SCOPE void *
zrReallocate(void *pOriginal, ZrSize size) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_SCOPE void
zrFree(void *pMemory) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_SCOPE void *
zrAllocateAligned(ZrSize size, ZrSize alignment) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_SCOPE void *
zrReallocateAligned(void *pOriginal,
                    ZrSize size,
                    ZrSize alignment) ZRP_MAYBE_UNUSED;

ZRP_ALLOCATOR_SCOPE void
zrFreeAligned(void *pMemory) ZRP_MAYBE_UNUSED;

#ifdef __cplusplus
}
#endif

#endif /* ZERO_ALLOCATOR_H */

#ifdef ZR_IMPLEMENTATION
#ifndef ZRP_ALLOCATOR_IMPLEMENTATION
#define ZRP_ALLOCATOR_IMPLEMENTATION

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

#ifndef ZR_ALLOCATOR_DEBUGGING
#define ZR_ALLOCATOR_DEBUGGING 0
#endif

#ifdef __cplusplus
template<typename T>
struct ZrpAlignmentOfHelper {
    char first;
    T second;
};
#define ZRP_ALIGNMENT_OF(type) offsetof(ZrpAlignmentOfHelper<type>, second)
#else
#define ZRP_ALIGNMENT_OF(type)                                                 \
    offsetof(                                                                  \
        struct {                                                               \
            char first;                                                        \
            type second;                                                       \
        },                                                                     \
        second)
#endif /* __cplusplus */

#define ZRP_IS_POWER_OF_TWO(x)                                                 \
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

typedef struct ZrpAlignedBlockHeader {
    ptrdiff_t offset;
    size_t size;
#if ZR_ALLOCATOR_DEBUGGING
    size_t alignment;
#endif /* ZR_ALLOCATOR_DEBUGGING */
} ZrpAlignedBlockHeader;

typedef char zr_invalid_block_header_alignment
    [ZRP_IS_POWER_OF_TWO(ZRP_ALIGNMENT_OF(ZrpAlignedBlockHeader)) ? 1 : -1];
typedef char
    zr_invalid_void_pointer_alignment[ZRP_IS_POWER_OF_TWO(sizeof(void *)) ? 1
                                                                          : -1];

/*
   Any power of two alignment requested for the user pointer that is greater or
   equal to this minimum value is guaranteed to be a multiple of
   `sizeof(void *)`, thus conforming to the requirement of `posix_memalign()`,
   and is also guaranteed to provide correct alignment for the block header.
*/
static const size_t zrpMinAlignment
    = ZRP_ALIGNMENT_OF(ZrpAlignedBlockHeader) > sizeof(void *)
          ? ZRP_ALIGNMENT_OF(ZrpAlignedBlockHeader)
          : sizeof(void *);

static int
zrpIsPowerOfTwo(ZrSize x)
{
    /* Decrement and compare approach. */
    return (x != 0) && !(x & (x - 1));
}

ZRP_ALLOCATOR_SCOPE void *
zrAllocate(ZrSize size)
{
    if (size == 0) {
        return NULL;
    }

    return ZR_ALLOCATOR_MALLOC((size_t)size);
}

ZRP_ALLOCATOR_SCOPE void *
zrReallocate(void *pOriginal, ZrSize size)
{
    if (pOriginal == NULL) {
        return zrAllocate(size);
    }

    if (size == 0) {
        zrFree(pOriginal);
        return NULL;
    }

    return ZR_ALLOCATOR_REALLOC(pOriginal, (size_t)size);
}

ZRP_ALLOCATOR_SCOPE void
zrFree(void *pMemory)
{
    ZR_ALLOCATOR_FREE(pMemory);
}

ZRP_ALLOCATOR_SCOPE void *
zrAllocateAligned(ZrSize size, ZrSize alignment)
{
    void *pOut;
    void *pBlock;
    ZrpAlignedBlockHeader *pHeader;

    if (size == 0 || !zrpIsPowerOfTwo(alignment)) {
        return NULL;
    }

    if (alignment < zrpMinAlignment) {
        alignment = (ZrSize)zrpMinAlignment;
    }

    pBlock = ZR_ALLOCATOR_MALLOC(
        (size_t)(size + alignment - 1 + sizeof(ZrpAlignedBlockHeader)));
    if (pBlock == NULL) {
        return NULL;
    }

    pOut = (void *)((uintptr_t)((unsigned char *)pBlock + alignment - 1
                                + sizeof(ZrpAlignedBlockHeader))
                    & ~(uintptr_t)(alignment - 1));

    pHeader = &((ZrpAlignedBlockHeader *)pOut)[-1];
    pHeader->offset = (unsigned char *)pOut - (unsigned char *)pBlock;
    pHeader->size = size;
#if ZR_ALLOCATOR_DEBUGGING
    pHeader->alignment = (size_t)alignment;
#endif /* ZR_ALLOCATOR_DEBUGGING */

    return pOut;
}

ZRP_ALLOCATOR_SCOPE void *
zrReallocateAligned(void *pOriginal, ZrSize size, ZrSize alignment)
{
    ZrpAlignedBlockHeader originalHeader;
    void *pOriginalBlock;
    void *pBlock;

    if (pOriginal == NULL) {
        return zrAllocateAligned(size, alignment);
    }

    if (size == 0 || !zrpIsPowerOfTwo(alignment)) {
        zrFreeAligned(pOriginal);
        return NULL;
    }

    if (alignment < zrpMinAlignment) {
        alignment = (ZrSize)zrpMinAlignment;
    }

    originalHeader = ((ZrpAlignedBlockHeader *)pOriginal)[-1];
#if ZR_ALLOCATOR_DEBUGGING
    ZR_ALLOCATOR_ASSERT(alignment == originalHeader.alignment);
#endif /* ZR_ALLOCATOR_DEBUGGING */

    pOriginalBlock
        = (void *)((unsigned char *)pOriginal - originalHeader.offset);
    pBlock = ZR_ALLOCATOR_REALLOC(
        pOriginalBlock,
        (size_t)(size + alignment - 1 + sizeof(ZrpAlignedBlockHeader)));
    if (pBlock == NULL) {
        return NULL;
    }

    if (pBlock == pOriginalBlock) {
        /* `realloc()` expanded the block in place. */
        ((ZrpAlignedBlockHeader *)pOriginal)[-1].size = size;
        return pOriginal;
    }

    {
        void *pOut;
        ZrpAlignedBlockHeader *pHeader;

        pOut = (void *)((uintptr_t)((unsigned char *)pBlock + alignment - 1
                                    + sizeof(ZrpAlignedBlockHeader))
                        & ~(uintptr_t)(alignment - 1));

        pHeader = &((ZrpAlignedBlockHeader *)pOut)[-1];
        pHeader->offset = (unsigned char *)pOut - (unsigned char *)pBlock;
        pHeader->size = size;

        if (pHeader->offset == originalHeader.offset) {
            /*
               `realloc()` allocated a new block that is still correctly
               aligned.
            */
            return pOut;
        }

        memmove(pOut,
                (void *)((unsigned char *)pBlock + originalHeader.offset),
                originalHeader.size);

        return pOut;
    }
}

ZRP_ALLOCATOR_SCOPE void
zrFreeAligned(void *pMemory)
{
    ZrpAlignedBlockHeader *pHeader;

    if (pMemory == NULL) {
        return;
    }

    pHeader = &((ZrpAlignedBlockHeader *)pMemory)[-1];
    ZR_ALLOCATOR_FREE((void *)((unsigned char *)pMemory - pHeader->offset));
}

#endif /* ZRP_ALLOCATOR_IMPLEMENTATION */
#endif /* ZR_IMPLEMENTATION */
