

#ifndef _my_audit_h
#define _my_audit_h



#include "plugin.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MYSQL_AUDIT_CLASS_MASK_SIZE 1

#define MYSQL_AUDIT_INTERFACE_VERSION 0x0302




#define MYSQL_AUDIT_GENERAL_CLASS 0
#define MYSQL_AUDIT_GENERAL_CLASSMASK (1 << MYSQL_AUDIT_GENERAL_CLASS)
#define MYSQL_AUDIT_GENERAL_LOG 0
#define MYSQL_AUDIT_GENERAL_ERROR 1
#define MYSQL_AUDIT_GENERAL_RESULT 2
#define MYSQL_AUDIT_GENERAL_STATUS 3

struct mysql_event_general
{
  unsigned int event_subclass;
  int general_error_code;
  unsigned long general_thread_id;
  const char *general_user;
  unsigned int general_user_length;
  const char *general_command;
  unsigned int general_command_length;
  const char *general_query;
  unsigned int general_query_length;
  const struct charset_info_st *general_charset;
  unsigned long long general_time;
  unsigned long long general_rows;
  
  unsigned long long query_id;
  MYSQL_CONST_LEX_STRING database;
};




#define MYSQL_AUDIT_CONNECTION_CLASS 1
#define MYSQL_AUDIT_CONNECTION_CLASSMASK (1 << MYSQL_AUDIT_CONNECTION_CLASS)
#define MYSQL_AUDIT_CONNECTION_CONNECT 0
#define MYSQL_AUDIT_CONNECTION_DISCONNECT 1
#define MYSQL_AUDIT_CONNECTION_CHANGE_USER 2

struct mysql_event_connection
{
  unsigned int event_subclass;
  int status;
  unsigned long thread_id;
  const char *user;
  unsigned int user_length;
  const char *priv_user;
  unsigned int priv_user_length;
  const char *external_user;
  unsigned int external_user_length;
  const char *proxy_user;
  unsigned int proxy_user_length;
  const char *host;
  unsigned int host_length;
  const char *ip;
  unsigned int ip_length;
  MYSQL_CONST_LEX_STRING database;
};



#define MYSQL_AUDIT_TABLE_CLASS 15
#define MYSQL_AUDIT_TABLE_CLASSMASK (1 << MYSQL_AUDIT_TABLE_CLASS)
#define MYSQL_AUDIT_TABLE_LOCK   0
#define MYSQL_AUDIT_TABLE_CREATE 1
#define MYSQL_AUDIT_TABLE_DROP   2
#define MYSQL_AUDIT_TABLE_RENAME 3
#define MYSQL_AUDIT_TABLE_ALTER  4

struct mysql_event_table
{
  unsigned int event_subclass;
  unsigned long thread_id;
  const char *user;
  const char *priv_user;
  const char *priv_host;
  const char *external_user;
  const char *proxy_user;
  const char *host;
  const char *ip;
  MYSQL_CONST_LEX_STRING database;
  MYSQL_CONST_LEX_STRING table;
  
  MYSQL_CONST_LEX_STRING new_database;
  MYSQL_CONST_LEX_STRING new_table;
  
  int read_only;
  
  unsigned long long query_id;
};



struct st_mysql_audit
{
  int interface_version;
  void (*release_thd)(MYSQL_THD);
  void (*event_notify)(MYSQL_THD, unsigned int, const void *);
  unsigned long class_mask[MYSQL_AUDIT_CLASS_MASK_SIZE];
};


#ifdef __cplusplus
}
#endif

#endif
