

#ifndef MYSQL_SERVICE_THD_WAIT_INCLUDED
#define MYSQL_SERVICE_THD_WAIT_INCLUDED



#ifdef __cplusplus
extern "C" {
#endif


typedef enum _thd_wait_type_e {
  THD_WAIT_SLEEP= 1,
  THD_WAIT_DISKIO= 2,
  THD_WAIT_ROW_LOCK= 3,
  THD_WAIT_GLOBAL_LOCK= 4,
  THD_WAIT_META_DATA_LOCK= 5,
  THD_WAIT_TABLE_LOCK= 6,
  THD_WAIT_USER_LOCK= 7,
  THD_WAIT_BINLOG= 8,
  THD_WAIT_GROUP_COMMIT= 9,
  THD_WAIT_SYNC= 10,
  THD_WAIT_NET= 11,
  THD_WAIT_LAST= 12
} thd_wait_type;

extern struct thd_wait_service_st {
  void (*thd_wait_begin_func)(MYSQL_THD, int);
  void (*thd_wait_end_func)(MYSQL_THD);
} *thd_wait_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define thd_wait_begin(_THD, _WAIT_TYPE) \
  thd_wait_service->thd_wait_begin_func(_THD, _WAIT_TYPE)
#define thd_wait_end(_THD) thd_wait_service->thd_wait_end_func(_THD)

#else

void thd_wait_begin(MYSQL_THD thd, int wait_type);
void thd_wait_end(MYSQL_THD thd);

#endif

#ifdef __cplusplus
}
#endif

#endif

