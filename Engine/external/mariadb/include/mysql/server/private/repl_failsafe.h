#ifndef REPL_FAILSAFE_INCLUDED
#define REPL_FAILSAFE_INCLUDED



#ifdef HAVE_REPLICATION

#include "mysql.h"
#include <my_sys.h>
#include "slave.h"

typedef enum {RPL_AUTH_MASTER=0,RPL_IDLE_SLAVE,RPL_ACTIVE_SLAVE,
	      RPL_LOST_SOLDIER,RPL_TROOP_SOLDIER,
	      RPL_RECOVERY_CAPTAIN,RPL_NULL ,
	      RPL_ANY  } RPL_STATUS;
extern ulong rpl_status;

extern mysql_mutex_t LOCK_rpl_status;
extern mysql_cond_t COND_rpl_status;
extern TYPELIB rpl_role_typelib;
extern const char* rpl_role_type[], *rpl_status_type[];

void change_rpl_status(ulong from_status, ulong to_status);
int find_recovery_captain(THD* thd, MYSQL* mysql);

extern HASH slave_list;

bool show_slave_hosts(THD* thd);
void init_slave_list();
void end_slave_list();
int register_slave(THD* thd, uchar* packet, size_t packet_length);
void unregister_slave(THD* thd, bool only_mine, bool need_mutex);

#endif 
#endif 
