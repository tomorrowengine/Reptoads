

#include <my_config.h>

#ifndef WSREP_INCLUDED
#define WSREP_INCLUDED

#ifdef WITH_WSREP
#define IF_WSREP(A,B) A
#define DBUG_ASSERT_IF_WSREP(A) DBUG_ASSERT(A)

#define WSREP_MYSQL_DB (char *)"mysql"
#define WSREP_TO_ISOLATION_BEGIN(db_, table_, table_list_)                   \
  if (WSREP_ON && WSREP(thd) && wsrep_to_isolation_begin(thd, db_, table_, table_list_)) \
    goto wsrep_error_label;

#define WSREP_TO_ISOLATION_BEGIN_ALTER(db_, table_, table_list_, alter_info_) \
  if (WSREP_ON && WSREP(thd) && wsrep_to_isolation_begin(thd, db_, table_,    \
                                             table_list_, alter_info_))       \
    goto wsrep_error_label;

#define WSREP_TO_ISOLATION_END                                              \
  if (WSREP_ON && (WSREP(thd) || (thd && thd->wsrep_exec_mode==TOTAL_ORDER))) \
    wsrep_to_isolation_end(thd);


#define WSREP_TO_ISOLATION_BEGIN_WRTCHK(db_, table_, table_list_)                   \
  if (WSREP(thd) && !thd->lex->no_write_to_binlog                                   \
         && wsrep_to_isolation_begin(thd, db_, table_, table_list_)) goto wsrep_error_label;

#define WSREP_DEBUG(...)                                                \
    if (wsrep_debug)     WSREP_LOG(sql_print_information, ##__VA_ARGS__)
#define WSREP_INFO(...)  WSREP_LOG(sql_print_information, ##__VA_ARGS__)
#define WSREP_WARN(...)  WSREP_LOG(sql_print_warning,     ##__VA_ARGS__)
#define WSREP_ERROR(...) WSREP_LOG(sql_print_error,       ##__VA_ARGS__)

#define WSREP_SYNC_WAIT(thd_, before_)                            \
    do { if (WSREP_CLIENT(thd_) &&                                \
          wsrep_sync_wait(thd_, before_)) goto wsrep_error_label; } while(0)
#define WSREP_ERROR_LABEL  wsrep_error_label
#else
#define IF_WSREP(A,B) B
#define DBUG_ASSERT_IF_WSREP(A)
#define WSREP_DEBUG(...)
#define WSREP_INFO(...)
#define WSREP_WARN(...)
#define WSREP_ERROR(...)
#define WSREP_TO_ISOLATION_BEGIN(db_, table_, table_list_)
#define WSREP_TO_ISOLATION_BEGIN_ALTER(db_, table_, table_list_, alter_info_)
#define WSREP_TO_ISOLATION_END
#define WSREP_TO_ISOLATION_BEGIN_WRTCHK(db_, table_, table_list_)
#define WSREP_SYNC_WAIT(thd_, before_) do { } while(0)
#define WSREP_ERROR_LABEL goto wsrep_error_label; wsrep_error_label

#endif 

#endif 
