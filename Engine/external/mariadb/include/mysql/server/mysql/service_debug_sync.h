#ifndef MYSQL_SERVICE_DEBUG_SYNC_INCLUDED




#ifndef MYSQL_ABI_CHECK
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MYSQL_DYNAMIC_PLUGIN
extern void (*debug_sync_service)(MYSQL_THD, const char *, size_t);
#else
#define debug_sync_service debug_sync_C_callback_ptr
extern void (*debug_sync_C_callback_ptr)(MYSQL_THD, const char *, size_t);
#endif

#ifdef ENABLED_DEBUG_SYNC
#define DEBUG_SYNC(thd, name)                           \
  do {                                                  \
    if (debug_sync_service)                             \
      debug_sync_service(thd, STRING_WITH_LEN(name));   \
  } while(0)

#define DEBUG_SYNC_C_IF_THD(thd, name)                   \
  do {                                                   \
    if (debug_sync_service && thd)                       \
      debug_sync_service((MYSQL_THD) thd, STRING_WITH_LEN(name));   \
  } while(0)
#else
#define DEBUG_SYNC(thd,name)                        do { } while(0)
#define DEBUG_SYNC_C_IF_THD(thd, _sync_point_name_) do { } while(0)
#endif 


#define DEBUG_SYNC_C(name) DEBUG_SYNC(NULL, name)

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_DEBUG_SYNC_INCLUDED
#endif
