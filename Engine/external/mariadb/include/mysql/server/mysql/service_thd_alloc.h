#ifndef MYSQL_SERVICE_THD_ALLOC_INCLUDED




#ifndef MYSQL_ABI_CHECK
#include <stdlib.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct st_mysql_lex_string
{
  char *str;
  size_t length;
};
typedef struct st_mysql_lex_string MYSQL_LEX_STRING;

struct st_mysql_const_lex_string
{
  const char *str;
  size_t length;
};
typedef struct st_mysql_const_lex_string MYSQL_CONST_LEX_STRING;

extern struct thd_alloc_service_st {
  void *(*thd_alloc_func)(MYSQL_THD, size_t);
  void *(*thd_calloc_func)(MYSQL_THD, size_t);
  char *(*thd_strdup_func)(MYSQL_THD, const char *);
  char *(*thd_strmake_func)(MYSQL_THD, const char *, size_t);
  void *(*thd_memdup_func)(MYSQL_THD, const void*, size_t);
  MYSQL_CONST_LEX_STRING *(*thd_make_lex_string_func)(MYSQL_THD,
                                        MYSQL_CONST_LEX_STRING *,
                                        const char *, size_t, int);
} *thd_alloc_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define thd_alloc(thd,size) (thd_alloc_service->thd_alloc_func((thd), (size)))

#define thd_calloc(thd,size) (thd_alloc_service->thd_calloc_func((thd), (size)))

#define thd_strdup(thd,str) (thd_alloc_service->thd_strdup_func((thd), (str)))

#define thd_strmake(thd,str,size) \
  (thd_alloc_service->thd_strmake_func((thd), (str), (size)))

#define thd_memdup(thd,str,size) \
  (thd_alloc_service->thd_memdup_func((thd), (str), (size)))

#define thd_make_lex_string(thd, lex_str, str, size, allocate_lex_string) \
  (thd_alloc_service->thd_make_lex_string_func((thd), (lex_str), (str), \
                                               (size), (allocate_lex_string)))

#else


void *thd_alloc(MYSQL_THD thd, size_t size);

void *thd_calloc(MYSQL_THD thd, size_t size);

char *thd_strdup(MYSQL_THD thd, const char *str);

char *thd_strmake(MYSQL_THD thd, const char *str, size_t size);

void *thd_memdup(MYSQL_THD thd, const void* str, size_t size);


MYSQL_CONST_LEX_STRING
*thd_make_lex_string(MYSQL_THD thd, MYSQL_CONST_LEX_STRING *lex_str,
                     const char *str, size_t size,
                     int allocate_lex_string);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_THD_ALLOC_INCLUDED
#endif

