#ifndef MYSQL_SERVICE_PROGRESS_REPORT_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

#define thd_proc_info(thd, msg)  set_thd_proc_info(thd, msg, \
                                                   __func__, __FILE__, __LINE__)

extern struct progress_report_service_st {
  void (*thd_progress_init_func)(MYSQL_THD thd, unsigned int max_stage);
  void (*thd_progress_report_func)(MYSQL_THD thd,
                                   unsigned long long progress,
                                   unsigned long long max_progress);
  void (*thd_progress_next_stage_func)(MYSQL_THD thd);
  void (*thd_progress_end_func)(MYSQL_THD thd);
  const char *(*set_thd_proc_info_func)(MYSQL_THD, const char *info,
                                        const char *func,
                                        const char *file,
                                        unsigned int line);
} *progress_report_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define thd_progress_init(thd,max_stage) (progress_report_service->thd_progress_init_func((thd),(max_stage)))
#define thd_progress_report(thd, progress, max_progress) (progress_report_service->thd_progress_report_func((thd), (progress), (max_progress)))
#define thd_progress_next_stage(thd) (progress_report_service->thd_progress_next_stage_func(thd))
#define thd_progress_end(thd) (progress_report_service->thd_progress_end_func(thd))
#define set_thd_proc_info(thd,info,func,file,line) (progress_report_service->set_thd_proc_info_func((thd),(info),(func),(file),(line)))

#else


void thd_progress_init(MYSQL_THD thd, unsigned int max_stage);
void thd_progress_report(MYSQL_THD thd,
                         unsigned long long progress,
                         unsigned long long max_progress);
void thd_progress_next_stage(MYSQL_THD thd);
void thd_progress_end(MYSQL_THD thd);
const char *set_thd_proc_info(MYSQL_THD, const char * info, const char *func,
                              const char *file, unsigned int line);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_PROGRESS_REPORT_INCLUDED
#endif

