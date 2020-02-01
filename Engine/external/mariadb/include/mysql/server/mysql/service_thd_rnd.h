#ifndef MYSQL_SERVICE_THD_RND_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

#ifndef MYSQL_ABI_CHECK
#include <stdlib.h>
#endif

extern struct thd_rnd_service_st {
  double (*thd_rnd_ptr)(MYSQL_THD thd);
  void   (*thd_c_r_p_ptr)(MYSQL_THD thd, char *to, size_t length);
} *thd_rnd_service;

#ifdef MYSQL_DYNAMIC_PLUGIN
#define thd_rnd(A) thd_rnd_service->thd_rnd_ptr(A)
#define thd_create_random_password(A,B,C) thd_rnd_service->thd_c_r_p_ptr(A,B,C)
#else

double thd_rnd(MYSQL_THD thd);


void thd_create_random_password(MYSQL_THD thd, char *to, size_t length);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_THD_RND_INCLUDED
#endif
