

#ifndef RPL_RLI_H
#define RPL_RLI_H

#include "rpl_tblmap.h"
#include "rpl_reporting.h"
#include "rpl_utility.h"
#include "log.h"                         
#include "sql_class.h"                   
#include "log_event.h"
#include "rpl_parallel.h"

struct RPL_TABLE_LIST;
class Master_info;
class Rpl_filter;




struct rpl_group_info;
struct inuse_relaylog;

class Relay_log_info : public Slave_reporting_capability
{
public:
  
  enum enum_state_flag {
    
    IN_STMT=1,
    
    IN_TRANSACTION=2
  };

  
  inline bool belongs_to_client()
  {
    DBUG_ASSERT(sql_driver_thd);
    return !sql_driver_thd->slave_thread;
  }

  
  bool replicate_same_server_id;

  

  
  File info_fd,cur_log_fd;

  
  MYSQL_BIN_LOG relay_log;
  LOG_INFO linfo;

  
  IO_CACHE cache_buf,*cur_log;

   
  uint sync_counter;

   
  bool is_relay_log_recovery;

  

  
  IO_CACHE info_file;

  
  All_tmp_tables_list *save_temporary_tables;

  
  mysql_mutex_t data_lock, run_lock;
  
  mysql_cond_t start_cond, stop_cond, data_cond;
  
  Master_info *mi;

  
  inuse_relaylog *inuse_relaylog_list;
  inuse_relaylog *last_inuse_relaylog;

  
  uint32 cur_log_old_open_count;

  
  bool error_on_rli_init_info;

  
  char group_relay_log_name[FN_REFLEN];
  ulonglong group_relay_log_pos;
  char event_relay_log_name[FN_REFLEN];
  ulonglong event_relay_log_pos;
  ulonglong future_event_relay_log_pos;
  
  char future_event_master_log_name[FN_REFLEN];

  
  char group_master_log_name[FN_REFLEN];
  volatile my_off_t group_master_log_pos;

  
  ulonglong log_space_limit,log_space_total;
  bool ignore_log_space_limit;

  
  bool sql_force_rotate_relay;

  time_t last_master_timestamp;
  
  bool sql_thread_caught_up;

  void clear_until_condition();
  
  void clear_sql_delay()
  {
    sql_delay= 0;
  }


  
  volatile ulonglong slave_skip_counter;
  ulonglong max_relay_log_size;

  volatile ulong abort_pos_wait;	
  volatile ulong slave_run_id;		
  mysql_mutex_t log_space_lock;
  mysql_cond_t log_space_cond;
  
  THD *sql_driver_thd;
#ifndef DBUG_OFF
  int events_till_abort;
#endif  

  enum_gtid_skip_type gtid_skip_flag;

  
  volatile bool inited;
  volatile bool abort_slave;
  volatile bool stop_for_until;
  volatile uint slave_running;

  
  
  enum {
    UNTIL_NONE= 0, UNTIL_MASTER_POS, UNTIL_RELAY_POS, UNTIL_GTID
  } until_condition;
  char until_log_name[FN_REFLEN];
  ulonglong until_log_pos;
  
  ulong until_log_name_extension;   
  
  enum 
  { 
    UNTIL_LOG_NAMES_CMP_UNKNOWN= -2, UNTIL_LOG_NAMES_CMP_LESS= -1,
    UNTIL_LOG_NAMES_CMP_EQUAL= 0, UNTIL_LOG_NAMES_CMP_GREATER= 1
  } until_log_names_cmp_result;
  
  slave_connection_state until_gtid_pos;

  
  ulong retried_trans;
  
  int64 executed_entries;

  
  char ign_master_log_name_end[FN_REFLEN];
  ulonglong ign_master_log_pos_end;
  
  slave_connection_state ign_gtids;

   
  char slave_patternload_file[FN_REFLEN]; 
  size_t slave_patternload_file_size;  

  rpl_parallel parallel;
  
  rpl_binlog_state relay_log_state;
  
  slave_connection_state restart_gtid_pos;

  Relay_log_info(bool is_slave_recovery);
  ~Relay_log_info();

  
  inline void notify_group_relay_log_name_update()
  {
    if (until_condition==UNTIL_RELAY_POS)
      until_log_names_cmp_result= UNTIL_LOG_NAMES_CMP_UNKNOWN;
  }

  
  inline void notify_group_master_log_name_update()
  {
    if (until_condition==UNTIL_MASTER_POS)
      until_log_names_cmp_result= UNTIL_LOG_NAMES_CMP_UNKNOWN;
  }

  void inc_group_relay_log_pos(ulonglong log_pos,
			       rpl_group_info *rgi,
			       bool skip_lock=0);

  int wait_for_pos(THD* thd, String* log_name, longlong log_pos, 
		   longlong timeout);
  void close_temporary_tables();

  
  bool is_until_satisfied(my_off_t);
  inline ulonglong until_pos()
  {
    DBUG_ASSERT(until_condition == UNTIL_MASTER_POS ||
                until_condition == UNTIL_RELAY_POS);
    return ((until_condition == UNTIL_MASTER_POS) ? group_master_log_pos :
	    group_relay_log_pos);
  }

  
  bool stmt_done(my_off_t event_log_pos, THD *thd, rpl_group_info *rgi);
  int alloc_inuse_relaylog(const char *name);
  void free_inuse_relaylog(inuse_relaylog *ir);
  void reset_inuse_relaylog();
  int update_relay_log_state(rpl_gtid *gtid_list, uint32 count);

  
  bool is_in_group() const {
    return (m_flags & (IN_STMT | IN_TRANSACTION));
  }

  
  void set_flag(enum_state_flag flag)
  {
    m_flags|= flag;
  }

  
  bool get_flag(enum_state_flag flag)
  {
    return m_flags & flag;
  }

  
  void clear_flag(enum_state_flag flag)
  {
    m_flags&= ~flag;
  }

  
  static const char *const state_delaying_string;

  bool flush();

  
  int init(const char* info_filename);

  
  void start_sql_delay(time_t delay_end)
  {
    mysql_mutex_assert_owner(&data_lock);
    sql_delay_end= delay_end;
    thd_proc_info(sql_driver_thd, state_delaying_string);
  }

  int32 get_sql_delay() { return sql_delay; }
  void set_sql_delay(int32 _sql_delay) { sql_delay= _sql_delay; }
  time_t get_sql_delay_end() { return sql_delay_end; }

private:


  
  int sql_delay;

  
  time_t sql_delay_end;

  
  static const int LINES_IN_RELAY_LOG_INFO_WITH_DELAY= 5;

  
  uint32 m_flags;
};



struct inuse_relaylog {
  inuse_relaylog *next;
  Relay_log_info *rli;
  
  rpl_gtid *relay_log_state;
  uint32 relay_log_state_count;
  
  int64 queued_count;
  
  volatile int64 dequeued_count;
  
  bool completed;
  char name[FN_REFLEN];
};




struct rpl_group_info
{
  rpl_group_info *next;             
  Relay_log_info *rli;
  THD *thd;
  
  uint64 gtid_sub_id;
  rpl_gtid current_gtid;
  uint64 commit_id;
  
  wait_for_commit commit_orderer;
  
  uint64 wait_commit_sub_id;
  rpl_group_info *wait_commit_group_info;
  
  group_commit_orderer *gco;

  struct rpl_parallel_entry *parallel_entry;

  
  Deferred_log_events *deferred_events;

  
  bool deferred_events_collecting;

  Annotate_rows_log_event *m_annotate_event;

  RPL_TABLE_LIST *tables_to_lock;           
  uint tables_to_lock_count;        
  table_mapping m_table_map;      
  mysql_mutex_t sleep_lock;
  mysql_cond_t sleep_cond;

  
  ulong trans_retries;

  
  time_t last_event_start_time;

  char *event_relay_log_name;
  char event_relay_log_name_buf[FN_REFLEN];
  ulonglong event_relay_log_pos;
  ulonglong future_event_relay_log_pos;
  
  char future_event_master_log_name[FN_REFLEN];
  bool is_parallel_exec;
  
  bool gtid_pending;
  int worker_error;
  
  bool did_mark_start_commit;
  
  uchar gtid_ev_flags2;
  enum {
    GTID_DUPLICATE_NULL=0,
    GTID_DUPLICATE_IGNORE=1,
    GTID_DUPLICATE_OWNER=2
  };
  
  uint8 gtid_ignore_duplicate_state;

  
  longlong row_stmt_start_timestamp;
  bool long_find_row_note_printed;
  
  char gtid_info_buf[5+10+1+10+1+20+1];

  
  rpl_slave_state::list_element *pending_gtid_delete_list;
  
  uint32 pending_gtid_delete_list_domain;

  
  time_t last_master_timestamp;

  
  inuse_relaylog *relay_log;
  uint64 retry_start_offset;
  uint64 retry_event_count;
  
  enum enum_speculation {
    
    SPECULATE_NO,
    
    SPECULATE_OPTIMISTIC,
    
    SPECULATE_WAIT
  } speculation;
  enum enum_retry_killed {
    RETRY_KILL_NONE = 0,
    RETRY_KILL_PENDING,
    RETRY_KILL_KILLED
  };
  uchar killed_for_retry;

  rpl_group_info(Relay_log_info *rli_);
  ~rpl_group_info();
  void reinit(Relay_log_info *rli);

  
  bool is_deferred_event(Log_event * ev)
  {
    return deferred_events_collecting ? deferred_events->is_last(ev) : false;
  };
  
  inline void cleanup_after_query()
  {
    if (deferred_events)
      deferred_events->rewind();
  };
  
  void cleanup_after_session()
  {
    if (deferred_events)
    {
      delete deferred_events;
      deferred_events= NULL;
    }
  };

  
  inline void set_annotate_event(Annotate_rows_log_event *event)
  {
    DBUG_ASSERT(m_annotate_event == NULL);
    m_annotate_event= event;
    this->thd->variables.binlog_annotate_row_events= 1;
  }

  
  inline Annotate_rows_log_event* get_annotate_event()
  {
    return m_annotate_event;
  }

  
  inline void free_annotate_event()
  {
    if (m_annotate_event)
    {
      this->thd->variables.binlog_annotate_row_events= 0;
      delete m_annotate_event;
      m_annotate_event= 0;
    }
  }

  bool get_table_data(TABLE *table_arg, table_def **tabledef_var, TABLE **conv_table_var) const
  {
    DBUG_ASSERT(tabledef_var && conv_table_var);
    for (TABLE_LIST *ptr= tables_to_lock ; ptr != NULL ; ptr= ptr->next_global)
      if (ptr->table == table_arg)
      {
        *tabledef_var= &static_cast<RPL_TABLE_LIST*>(ptr)->m_tabledef;
        *conv_table_var= static_cast<RPL_TABLE_LIST*>(ptr)->m_conv_table;
        DBUG_PRINT("debug", ("Fetching table data for table %s.%s:"
                             " tabledef: %p, conv_table: %p",
                             table_arg->s->db.str, table_arg->s->table_name.str,
                             *tabledef_var, *conv_table_var));
        return true;
      }
    return false;
  }

  void clear_tables_to_lock();
  void cleanup_context(THD *, bool);
  void slave_close_thread_tables(THD *);
  void mark_start_commit_no_lock();
  void mark_start_commit();
  char *gtid_info();
  void unmark_start_commit();

  static void pending_gtid_deletes_free(rpl_slave_state::list_element *list);
  void pending_gtid_deletes_save(uint32 domain_id,
                                 rpl_slave_state::list_element *list);
  void pending_gtid_deletes_put_back();
  void pending_gtid_deletes_clear();

  longlong get_row_stmt_start_timestamp()
  {
    return row_stmt_start_timestamp;
  }

  void set_row_stmt_start_timestamp()
  {
    if (row_stmt_start_timestamp == 0)
      row_stmt_start_timestamp= microsecond_interval_timer();
  }

  void reset_row_stmt_start_timestamp()
  {
    row_stmt_start_timestamp= 0;
  }

  void set_long_find_row_note_printed()
  {
    long_find_row_note_printed= true;
  }

  void unset_long_find_row_note_printed()
  {
    long_find_row_note_printed= false;
  }

  bool is_long_find_row_note_printed()
  {
    return long_find_row_note_printed;
  }

  inline void inc_event_relay_log_pos()
  {
    if (!is_parallel_exec)
      rli->event_relay_log_pos= future_event_relay_log_pos;
  }
};



class rpl_sql_thread_info
{
public:
  char cached_charset[6];
  Rpl_filter* rpl_filter;

  rpl_sql_thread_info(Rpl_filter *filter);

  
  void cached_charset_invalidate();
  bool cached_charset_compare(char *charset) const;
};


extern struct rpl_slave_state *rpl_global_gtid_slave_state;
extern gtid_waiting rpl_global_gtid_waiting;

int rpl_load_gtid_slave_state(THD *thd);
int find_gtid_slave_pos_tables(THD *thd);
int event_group_new_gtid(rpl_group_info *rgi, Gtid_log_event *gev);
void delete_or_keep_event_post_apply(rpl_group_info *rgi,
                                     Log_event_type typ, Log_event *ev);

#endif 
