#ifndef MYSQL_SERVICE_THD_STMT_DA_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif


extern struct thd_error_context_service_st {
  const char *(*thd_get_error_message_func)(const MYSQL_THD thd);
  unsigned int (*thd_get_error_number_func)(const MYSQL_THD thd);
  unsigned long (*thd_get_error_row_func)(const MYSQL_THD thd);
  void (*thd_inc_error_row_func)(MYSQL_THD thd);
  char *(*thd_get_error_context_description_func)(MYSQL_THD thd,
                                                  char *buffer,
                                                  unsigned int length,
                                                  unsigned int max_query_length);
} *thd_error_context_service;

#ifdef MYSQL_DYNAMIC_PLUGIN
#define thd_get_error_message(thd) \
  (thd_error_context_service->thd_get_error_message_func((thd)))
#define thd_get_error_number(thd) \
  (thd_error_context_service->thd_get_error_number_func((thd)))
#define thd_get_error_row(thd) \
  (thd_error_context_service->thd_get_error_row_func((thd)))
#define thd_inc_error_row(thd) \
  (thd_error_context_service->thd_inc_error_row_func((thd)))
#define thd_get_error_context_description(thd, buffer, length, max_query_len) \
  (thd_error_context_service->thd_get_error_context_description_func((thd), \
                                                                (buffer), \
                                                                (length), \
                                                                (max_query_len)))
#else

const char *thd_get_error_message(const MYSQL_THD thd);

unsigned int thd_get_error_number(const MYSQL_THD thd);

unsigned long thd_get_error_row(const MYSQL_THD thd);

void thd_inc_error_row(MYSQL_THD thd);

char *thd_get_error_context_description(MYSQL_THD thd,
                                        char *buffer, unsigned int length,
                                        unsigned int max_query_length);
#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_THD_STMT_DA_INCLUDED
#endif
