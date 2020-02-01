#ifndef MYSQL_SERVICE_THD_AUTOINC_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

extern struct thd_autoinc_service_st {
  void (*thd_get_autoinc_func)(const MYSQL_THD thd,
                               unsigned long* off, unsigned long* inc);
} *thd_autoinc_service;

#ifdef MYSQL_DYNAMIC_PLUGIN
#define thd_get_autoinc(thd, off, inc) \
  (thd_autoinc_service->thd_get_autoinc_func((thd), (off), (inc)))
#else

void thd_get_autoinc(const MYSQL_THD thd,
                     unsigned long* off, unsigned long* inc);
#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_THD_AUTOINC_INCLUDED
#endif
