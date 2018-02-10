#ifndef ZRP_UNUSED_DEFINED
#define ZRP_UNUSED_DEFINED
#ifdef __GNUC__
#define ZRP_MAYBE_UNUSED __attribute__((unused))
#else
#define ZRP_MAYBE_UNUSED
#endif
#endif /* ZRP_UNUSED_DEFINED */
