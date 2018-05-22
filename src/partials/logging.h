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
