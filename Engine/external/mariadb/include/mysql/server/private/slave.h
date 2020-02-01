

#ifndef SLAVE_H
#define SLAVE_H


#define MASTER_DELAY_MAX (0x7FFFFFFF)
#if INT_MAX < 0x7FFFFFFF
#error "don't support platforms where INT_MAX < 0x7FFFFFFF"
#endif






#define SLAVE_MAX_HEARTBEAT_PERIOD 4294967

#ifdef HAVE_REPLICATION

#include "log.h"
#include "my_list.h"
#include "rpl_filter.h"
#include "rpl_tblmap.h"
#include "rpl_gtid.h"

#define SLAVE_NET_TIMEOUT  60

#define MAX_SLAVE_ERROR    ER_ERROR_LAST+1

#define MAX_REPLICATION_THREAD 64


class Relay_log_info;
class Master_info;
class Master_info_index;
struct rpl_group_info;
struct rpl_parallel_thread;

int init_intvar_from_file(int* var, IO_CACHE* f, int default_val);
int init_strvar_from_file(char *var, int max_size, IO_CACHE *f,
                          const char *default_val);
int init_floatvar_from_file(float* var, IO_CACHE* f, float default_val);
int init_dynarray_intvar_from_file(DYNAMIC_ARRAY* arr, IO_CACHE* f);





extern ulong master_retry_count;
extern MY_BITMAP slave_error_mask;
extern char slave_skip_error_names[];
extern bool use_slave_mask;
extern char slave_transaction_retry_error_names[];
extern uint *slave_transaction_retry_errors;
extern uint slave_transaction_retry_error_length;
extern char *slave_load_tmpdir;
extern char *master_info_file;
extern MYSQL_PLUGIN_IMPORT char *relay_log_info_file;
extern char *opt_relay_logname, *opt_relaylog_index_name;
extern my_bool opt_skip_slave_start, opt_reckless_slave;
extern my_bool opt_log_slave_updates;
extern char *opt_slave_skip_errors;
extern char *opt_slave_transaction_retry_errors;
extern my_bool opt_replicate_annotate_row_events;
extern ulonglong relay_log_space_limit;
extern ulonglong opt_read_binlog_speed_limit;
extern ulonglong slave_skipped_errors;
extern const char *relay_log_index;
extern const char *relay_log_basename;


#define MYSQL_SLAVE_NOT_RUN         0
#define MYSQL_SLAVE_RUN_NOT_CONNECT 1
#define MYSQL_SLAVE_RUN_CONNECT     2
#define MYSQL_SLAVE_RUN_READING     3

#define RPL_LOG_NAME (rli->group_master_log_name[0] ? rli->group_master_log_name :\
 "FIRST")
#define IO_RPL_LOG_NAME (mi->master_log_name[0] ? mi->master_log_name :\
 "FIRST")


#define SLAVE_FORCE_ALL 4


#define RPL_SKIP_REPLICATE 0
#define RPL_SKIP_FILTER_ON_SLAVE 1
#define RPL_SKIP_FILTER_ON_MASTER 2


int init_slave();
int init_recovery(Master_info* mi, const char** errmsg);
bool init_slave_skip_errors(const char* arg);
bool init_slave_transaction_retry_errors(const char* arg);
int register_slave_on_master(MYSQL* mysql);
int terminate_slave_threads(Master_info* mi, int thread_mask,
			     bool skip_lock = 0);
int start_slave_threads(THD *thd,
                        bool need_slave_mutex, bool wait_for_start,
			Master_info* mi, const char* master_info_fname,
			const char* slave_info_fname, int thread_mask);

int start_slave_thread(
#ifdef HAVE_PSI_INTERFACE
                       PSI_thread_key thread_key,
#endif
                       pthread_handler h_func,
                       mysql_mutex_t *start_lock,
                       mysql_mutex_t *cond_lock,
                       mysql_cond_t *start_cond,
                       volatile uint *slave_running,
                       volatile ulong *slave_run_id,
                       Master_info *mi);


int mysql_table_dump(THD* thd, const char* db,
		     const char* tbl_name, int fd = -1);


int fetch_master_table(THD* thd, const char* db_name, const char* table_name,
		       Master_info* mi, MYSQL* mysql, bool overwrite);

void show_master_info_get_fields(THD *thd, List<Item> *field_list,
                                     bool full, size_t gtid_pos_length);
bool show_master_info(THD* thd, Master_info* mi, bool full);
bool show_all_master_info(THD* thd);
void show_binlog_info_get_fields(THD *thd, List<Item> *field_list);
bool show_binlog_info(THD* thd);
bool rpl_master_has_bug(const Relay_log_info *rli, uint bug_id, bool report,
                        bool (*pred)(const void *), const void *param);
bool rpl_master_erroneous_autoinc(THD* thd);

const char *print_slave_db_safe(const char *db);
void skip_load_data_infile(NET* net);

void slave_prepare_for_shutdown();
void end_slave(); 
void close_active_mi(); 
void clear_until_condition(Relay_log_info* rli);
void clear_slave_error(Relay_log_info* rli);
void end_relay_log_info(Relay_log_info* rli);
void init_thread_mask(int* mask,Master_info* mi,bool inverse);
Format_description_log_event *
read_relay_log_description_event(IO_CACHE *cur_log, ulonglong start_pos,
                                 const char **errmsg);

int init_relay_log_pos(Relay_log_info* rli,const char* log,ulonglong pos,
		       bool need_data_lock, const char** errmsg,
                       bool look_for_description_event);

int purge_relay_logs(Relay_log_info* rli, THD *thd, bool just_reset,
		     const char** errmsg);
void set_slave_thread_options(THD* thd);
void set_slave_thread_default_charset(THD *thd, rpl_group_info *rgi);
int rotate_relay_log(Master_info* mi);
int has_temporary_error(THD *thd);
int sql_delay_event(Log_event *ev, THD *thd, rpl_group_info *rgi);
int apply_event_and_update_pos(Log_event* ev, THD* thd,
                               struct rpl_group_info *rgi);
int apply_event_and_update_pos_for_parallel(Log_event* ev, THD* thd,
                                            struct rpl_group_info *rgi);

int init_intvar_from_file(int* var, IO_CACHE* f, int default_val);
int init_floatvar_from_file(float* var, IO_CACHE* f, float default_val);
int init_strvar_from_file(char *var, int max_size, IO_CACHE *f,
                          const char *default_val);
int init_dynarray_intvar_from_file(DYNAMIC_ARRAY* arr, IO_CACHE* f);

pthread_handler_t handle_slave_io(void *arg);
void slave_output_error_info(rpl_group_info *rgi, THD *thd);
pthread_handler_t handle_slave_sql(void *arg);
bool net_request_file(NET* net, const char* fname);
void slave_background_kill_request(THD *to_kill);
void slave_background_gtid_pos_create_request
        (rpl_slave_state::gtid_pos_table *table_entry);

extern bool volatile abort_loop;
extern Master_info *active_mi; 
extern Master_info *default_master_info; 
extern Master_info_index *master_info_index;
extern LEX_CSTRING default_master_connection_name;
extern my_bool replicate_same_server_id;

extern int disconnect_slave_event_count, abort_slave_event_count ;


extern uint report_port;
extern char *master_info_file, *report_user;
extern char *report_host, *report_password;

extern I_List<THD> threads;

#else
#define close_active_mi() 
#endif 


#define SLAVE_IO  1
#define SLAVE_SQL 2



#endif
