

#ifndef RPL_MI_H
#define RPL_MI_H

#ifdef HAVE_REPLICATION

#include "rpl_rli.h"
#include "rpl_reporting.h"
#include <my_sys.h>
#include "rpl_filter.h"
#include "keycaches.h"

typedef struct st_mysql MYSQL;


class Domain_id_filter
{
private:
  
  bool m_filter;

  
  DYNAMIC_ARRAY m_domain_ids[2];

public:
  
  enum enum_list_type {
    DO_DOMAIN_IDS= 0,
    IGNORE_DOMAIN_IDS
  };

  Domain_id_filter();

  ~Domain_id_filter();

  
  bool is_group_filtered() { return m_filter; }

  
  void do_filter(ulong domain_id);

  
  void reset_filter();

  
  bool update_ids(DYNAMIC_ARRAY *do_ids, DYNAMIC_ARRAY *ignore_ids,
                  bool using_gtid);

  
  void store_ids(THD *thd);

  
  bool init_ids(IO_CACHE *f, enum_list_type type);

  
  char *as_string(enum_list_type type);

};


extern TYPELIB slave_parallel_mode_typelib;

typedef struct st_rows_event_tracker
{
  char binlog_file_name[FN_REFLEN];
  my_off_t first_seen;
  my_off_t last_seen;
  bool stmt_end_seen;
  void update(const char* file_name, my_off_t pos,
              const char* buf,
              const Format_description_log_event *fdle);
  void reset();
  bool check_and_report(const char* file_name, my_off_t pos);
} Rows_event_tracker;



class Master_info : public Slave_reporting_capability
{
 public:
  enum enum_using_gtid {
    USE_GTID_NO= 0, USE_GTID_CURRENT_POS= 1, USE_GTID_SLAVE_POS= 2
  };

  Master_info(LEX_CSTRING *connection_name, bool is_slave_recovery);
  ~Master_info();
  bool shall_ignore_server_id(ulong s_id);
  void clear_in_memory_info(bool all);
  bool error()
  {
    
    return connection_name.str == 0;
  }
  static const char *using_gtid_astext(enum enum_using_gtid arg);
  bool using_parallel()
  {
    return opt_slave_parallel_threads > 0 &&
      parallel_mode > SLAVE_PARALLEL_NONE;
  }
  void release();
  void wait_until_free();
  void lock_slave_threads();
  void unlock_slave_threads();

  
  char master_log_name[FN_REFLEN+6]; 
  char host[HOSTNAME_LENGTH*SYSTEM_CHARSET_MBMAXLEN+1];
  char user[USERNAME_LENGTH+1];
  char password[MAX_PASSWORD_LENGTH*SYSTEM_CHARSET_MBMAXLEN+1];
  LEX_CSTRING connection_name; 		
  LEX_CSTRING cmp_connection_name;	
  bool ssl; 
  char ssl_ca[FN_REFLEN], ssl_capath[FN_REFLEN], ssl_cert[FN_REFLEN];
  char ssl_cipher[FN_REFLEN], ssl_key[FN_REFLEN];
  char ssl_crl[FN_REFLEN], ssl_crlpath[FN_REFLEN];
  bool ssl_verify_server_cert;

  my_off_t master_log_pos;
  File fd; 
  IO_CACHE file;

  mysql_mutex_t data_lock, run_lock, sleep_lock, start_stop_lock;
  mysql_cond_t data_cond, start_cond, stop_cond, sleep_cond;
  THD *io_thd;
  MYSQL* mysql;
  uint32 file_id;				
  Relay_log_info rli;
  uint port;
  Rpl_filter* rpl_filter;      
  
  enum enum_binlog_checksum_alg checksum_alg_before_fd;
  uint connect_retry;
#ifndef DBUG_OFF
  int events_till_disconnect;

  
  bool dbug_do_disconnect;
  int dbug_event_counter;
#endif
  bool inited;
  volatile bool abort_slave;
  volatile uint slave_running;
  volatile ulong slave_run_id;
  
  long clock_diff_with_master;
  
  uint sync_counter;
  float heartbeat_period;         
  ulonglong received_heartbeats;  
  DYNAMIC_ARRAY ignore_server_ids;
  ulong master_id;
  
  ulong prev_master_id;
  
  enum enum_using_gtid using_gtid;

  
  slave_connection_state gtid_current_pos;
  
  uint64 events_queued_since_last_gtid;
  
  rpl_gtid last_queued_gtid;
  
  bool last_queued_gtid_standalone;
  
  uint64 gtid_reconnect_event_skip_count;
  
  bool gtid_event_seen;
  
  Rows_event_tracker rows_event_tracker;
  bool in_start_all_slaves, in_stop_all_slaves;
  bool in_flush_all_relay_logs;
  uint users;                                   
  uint killed;


  
  volatile uint64 total_ddl_groups;

  
  volatile uint64 total_non_trans_groups;

  
  volatile uint64 total_trans_groups;

  
  Domain_id_filter domain_id_filter;

  
  enum_slave_parallel_mode parallel_mode;
  
  int semi_ack;
};

int init_master_info(Master_info* mi, const char* master_info_fname,
		     const char* slave_info_fname,
		     bool abort_if_no_master_info_file,
		     int thread_mask);
void end_master_info(Master_info* mi);
int flush_master_info(Master_info* mi, 
                      bool flush_relay_log_cache, 
                      bool need_lock_relay_log);
void copy_filter_setting(Rpl_filter* dst_filter, Rpl_filter* src_filter);
void update_change_master_ids(DYNAMIC_ARRAY *new_ids, DYNAMIC_ARRAY *old_ids);
void prot_store_ids(THD *thd, DYNAMIC_ARRAY *ids);



class Master_info_index
{
private:
  IO_CACHE index_file;
  char index_file_name[FN_REFLEN];

public:
  Master_info_index();
  ~Master_info_index();

  HASH master_info_hash;

  bool init_all_master_info();
  bool write_master_name_to_index_file(LEX_CSTRING *connection_name,
                                       bool do_sync);

  bool check_duplicate_master_info(LEX_CSTRING *connection_name,
                                   const char *host, uint port);
  bool add_master_info(Master_info *mi, bool write_to_file);
  bool remove_master_info(Master_info *mi);
  Master_info *get_master_info(const LEX_CSTRING *connection_name,
                               Sql_condition::enum_warning_level warning);
  bool start_all_slaves(THD *thd);
  bool stop_all_slaves(THD *thd);
  void free_connections();
  bool flush_all_relay_logs();
};



class rpl_io_thread_info
{
public:
};


Master_info *get_master_info(const LEX_CSTRING *connection_name,
                             Sql_condition::enum_warning_level warning);
bool check_master_connection_name(LEX_CSTRING *name);
void create_logfile_name_with_suffix(char *res_file_name, size_t length,
                             const char *info_file, 
                             bool append,
                             LEX_CSTRING *suffix);

uchar *get_key_master_info(Master_info *mi, size_t *length,
                           my_bool not_used __attribute__((unused)));
void free_key_master_info(Master_info *mi);
uint any_slave_sql_running(bool already_locked);
bool give_error_if_slave_running(bool already_lock);

#endif 
#endif 
