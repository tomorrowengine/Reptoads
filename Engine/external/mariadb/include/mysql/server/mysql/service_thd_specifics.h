#ifndef MYSQL_SERVICE_THD_SPECIFICS_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

typedef int MYSQL_THD_KEY_T;

extern struct thd_specifics_service_st {
  int (*thd_key_create_func)(MYSQL_THD_KEY_T *key);
  void (*thd_key_delete_func)(MYSQL_THD_KEY_T *key);
  void *(*thd_getspecific_func)(MYSQL_THD thd, MYSQL_THD_KEY_T key);
  int  (*thd_setspecific_func)(MYSQL_THD thd, MYSQL_THD_KEY_T key, void *value);
} *thd_specifics_service;

#define thd_key_create_from_var(K, V) do { *(K)= MYSQL_SYSVAR_NAME(V).offset; } while(0)

#ifdef MYSQL_DYNAMIC_PLUGIN

#define thd_key_create(K) (thd_specifics_service->thd_key_create_func(K))
#define thd_key_delete(K) (thd_specifics_service->thd_key_delete_func(K))
#define thd_getspecific(T, K) (thd_specifics_service->thd_getspecific_func(T, K))
#define thd_setspecific(T, K, V) (thd_specifics_service->thd_setspecific_func(T, K, V))

#else


int thd_key_create(MYSQL_THD_KEY_T *key);


void thd_key_delete(MYSQL_THD_KEY_T *key);


void* thd_getspecific(MYSQL_THD thd, MYSQL_THD_KEY_T key);
int thd_setspecific(MYSQL_THD thd, MYSQL_THD_KEY_T key, void *value);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_THD_SPECIFICS_INCLUDED
#endif

