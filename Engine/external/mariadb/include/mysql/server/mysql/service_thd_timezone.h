#ifndef MYSQL_SERVICE_THD_TIMEZONE_INCLUDED




#ifndef MYSQL_ABI_CHECK

#endif

#include "mysql_time.h"

#ifdef __cplusplus
extern "C" {
#endif


extern struct thd_timezone_service_st {
  my_time_t (*thd_TIME_to_gmt_sec)(MYSQL_THD thd, const MYSQL_TIME *ltime, unsigned int *errcode);
  void (*thd_gmt_sec_to_TIME)(MYSQL_THD thd, MYSQL_TIME *ltime, my_time_t t);
} *thd_timezone_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define thd_TIME_to_gmt_sec(thd, ltime, errcode) \
  (thd_timezone_service->thd_TIME_to_gmt_sec((thd), (ltime), (errcode)))

#define thd_gmt_sec_to_TIME(thd, ltime, t) \
  (thd_timezone_service->thd_gmt_sec_to_TIME((thd), (ltime), (t)))

#else

my_time_t thd_TIME_to_gmt_sec(MYSQL_THD thd, const MYSQL_TIME *ltime, unsigned int *errcode);
void thd_gmt_sec_to_TIME(MYSQL_THD thd, MYSQL_TIME *ltime, my_time_t t);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_THD_TIMEZONE_INCLUDED
#endif
