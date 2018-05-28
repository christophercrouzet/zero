/* @include "partials/license.h" */

#ifndef ZERO_DYNAMICARRAY_H
#define ZERO_DYNAMICARRAY_H

#define ZR_DYNAMICARRAY_MAJOR_VERSION 0
#define ZR_DYNAMICARRAY_MINOR_VERSION 1
#define ZR_DYNAMICARRAY_PATCH_VERSION 1

/* @include "partials/environment.h" */
/* @include "partials/platform.h" */
/* @include "partials/types.h" */

/* @include "partials/status.h" */

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

/* @include "partials/unused.h" */
/* @include "partials/logger.h" */
/* @include "partials/logging.h" */

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
