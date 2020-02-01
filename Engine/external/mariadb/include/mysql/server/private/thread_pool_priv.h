#error don't use


#ifndef THREAD_POOL_PRIV_INCLUDED
#define THREAD_POOL_PRIV_INCLUDED


#include <mysqld_error.h> 
#define MYSQL_SERVER 1
#include <scheduler.h>
#include <debug_sync.h>
#include <sql_profile.h>
#include <table.h>
#include <sql_list.h>


void* thd_get_scheduler_data(THD *thd);
void thd_set_scheduler_data(THD *thd, void *data);
PSI_thread* thd_get_psi(THD *thd);
void thd_set_psi(THD *thd, PSI_thread *psi);


void thd_set_killed(THD *thd);
void thd_clear_errors(THD *thd);
void thd_set_thread_stack(THD *thd, char *stack_start);
void thd_lock_thread_count(THD *thd);
void thd_close_connection(THD *thd);
THD *thd_get_current_thd();
void thd_lock_data(THD *thd);
void thd_unlock_data(THD *thd);
bool thd_is_transaction_active(THD *thd);
int thd_connection_has_data(THD *thd);
void thd_set_net_read_write(THD *thd, uint val);
uint thd_get_net_read_write(THD *thd);
void thd_set_mysys_var(THD *thd, st_my_thread_var *mysys_var);
my_socket thd_get_fd(THD *thd);
int thd_store_globals(THD* thd);

THD *first_global_thread();
THD *next_global_thread(THD *thd);


void sql_print_error(const char *format, ...);


bool schema_table_store_record(THD *thd, TABLE *table);


bool do_command(THD *thd);



bool init_new_connection_handler_thread();

bool setup_connection_thread_globals(THD *thd);

bool thd_prepare_connection(THD *thd);

void mysql_audit_release(THD *thd);

bool thd_is_connection_alive(THD *thd);

void close_connection(THD *thd, uint errcode);

void dec_connection_count();

void delete_thd(THD *thd);


void inc_thread_created(void);
ulong get_max_connections(void);
pthread_attr_t *get_connection_attrib(void);
#endif
