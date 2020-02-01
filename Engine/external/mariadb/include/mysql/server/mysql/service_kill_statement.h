

#ifndef MYSQL_SERVICE_KILL_STATEMENT_INCLUDED
#define MYSQL_SERVICE_KILL_STATEMENT_INCLUDED



#ifdef __cplusplus
extern "C" {
#endif

enum thd_kill_levels {
  THD_IS_NOT_KILLED=0,
  THD_ABORT_SOFTLY=50, 
  THD_ABORT_ASAP=100,  
};

extern struct kill_statement_service_st {
  enum thd_kill_levels (*thd_kill_level_func)(const MYSQL_THD);
} *thd_kill_statement_service;


#define thd_killed(THD)   (thd_kill_level(THD) == THD_ABORT_ASAP)

#ifdef MYSQL_DYNAMIC_PLUGIN

#define thd_kill_level(THD) \
        thd_kill_statement_service->thd_kill_level_func(THD)

#else

enum thd_kill_levels thd_kill_level(const MYSQL_THD);

#endif

#ifdef __cplusplus
}
#endif

#endif

