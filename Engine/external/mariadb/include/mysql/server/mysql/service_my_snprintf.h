#ifndef MYSQL_SERVICE_MY_SNPRINTF_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

#ifndef MYSQL_ABI_CHECK
#include <stdarg.h>
#include <stdlib.h>
#endif

extern struct my_snprintf_service_st {
  size_t (*my_snprintf_type)(char*, size_t, const char*, ...);
  size_t (*my_vsnprintf_type)(char *, size_t, const char*, va_list);
} *my_snprintf_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define my_vsnprintf my_snprintf_service->my_vsnprintf_type
#define my_snprintf my_snprintf_service->my_snprintf_type

#else

size_t my_snprintf(char* to, size_t n, const char* fmt, ...);
size_t my_vsnprintf(char *to, size_t n, const char* fmt, va_list ap);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_MY_SNPRINTF_INCLUDED
#endif

