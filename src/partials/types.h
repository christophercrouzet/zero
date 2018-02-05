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
#endif
#ifdef ZR_IMPLEMENTATION
typedef char zr_invalid_int8_type[sizeof(ZrInt8) == 1 ? 1 : -1];
typedef char zr_invalid_uint8_type[sizeof(ZrUint8) == 1 ? 1 : -1];
typedef char zr_invalid_int16_type[sizeof(ZrInt16) == 2 ? 1 : -1];
typedef char zr_invalid_uint16_type[sizeof(ZrUint16) == 2 ? 1 : -1];
typedef char zr_invalid_int32_type[sizeof(ZrInt32) == 4 ? 1 : -1];
typedef char zr_invalid_uint32_type[sizeof(ZrUint32) == 4 ? 1 : -1];
typedef char zr_invalid_int64_type[sizeof(ZrInt64) == 8 ? 1 : -1];
typedef char zr_invalid_uint64_type[sizeof(ZrUint64) == 8 ? 1 : -1];
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
    zr_invalid_size_type[sizeof(ZrSize) == sizeof sizeof(void *) ? 1 : -1];
#endif
#endif /* ZRP_BASIC_TYPES_DEFINED */
