

#ifndef LOG_H
#define LOG_H

#include "handler.h"                            
#include "wsrep.h"
#include "wsrep_mysqld.h"
#include "rpl_constants.h"

class Relay_log_info;

class Format_description_log_event;

void setup_log_handling();
bool trans_has_updated_trans_table(const THD* thd);
bool stmt_has_updated_trans_table(const THD *thd);
bool use_trans_cache(const THD* thd, bool is_transactional);
bool ending_trans(THD* thd, const bool all);
bool ending_single_stmt_trans(THD* thd, const bool all);
bool trans_has_updated_non_trans_table(const THD* thd);
bool stmt_has_updated_non_trans_table(const THD* thd);


class TC_LOG
{
  public:
  int using_heuristic_recover();
  TC_LOG() {}
  virtual ~TC_LOG() {}

  virtual int open(const char *opt_name)=0;
  virtual void close()=0;
  
  virtual int log_and_order(THD *thd, my_xid xid, bool all,
                            bool need_prepare_ordered,
                            bool need_commit_ordered) = 0;
  virtual int unlog(ulong cookie, my_xid xid)=0;
  virtual void commit_checkpoint_notify(void *cookie)= 0;

protected:
  
  void run_prepare_ordered(THD *thd, bool all);
  void run_commit_ordered(THD *thd, bool all);
};


extern mysql_mutex_t LOCK_prepare_ordered;
extern mysql_cond_t COND_prepare_ordered;
extern mysql_mutex_t LOCK_after_binlog_sync;
extern mysql_mutex_t LOCK_commit_ordered;
#ifdef HAVE_PSI_INTERFACE
extern PSI_mutex_key key_LOCK_prepare_ordered, key_LOCK_commit_ordered;
extern PSI_mutex_key key_LOCK_after_binlog_sync;
extern PSI_cond_key key_COND_prepare_ordered;
#endif

class TC_LOG_DUMMY: public TC_LOG 
{
public:
  TC_LOG_DUMMY() {}
  int open(const char *opt_name)        { return 0; }
  void close()                          { }
  
  int log_and_order(THD *thd, my_xid xid, bool all,
                    bool need_prepare_ordered, bool need_commit_ordered)
  {
    DBUG_ASSERT(0);
    return 1;
  }
  int unlog(ulong cookie, my_xid xid)  { return 0; }
  void commit_checkpoint_notify(void *cookie) { DBUG_ASSERT(0); };
};

#define TC_LOG_PAGE_SIZE   8192

#ifdef HAVE_MMAP
class TC_LOG_MMAP: public TC_LOG
{
  public:                
  typedef enum {
    PS_POOL,                 
    PS_ERROR,                
    PS_DIRTY                 
  } PAGE_STATE;

  struct pending_cookies {
    uint count;
    uint pending_count;
    ulong cookies[1];
  };

  private:
  typedef struct st_page {
    struct st_page *next; 
    my_xid *start, *end;  
    my_xid *ptr;          
    int size, free;       
    int waiters;          
    PAGE_STATE state;     
    mysql_mutex_t lock; 
    mysql_cond_t  cond; 
  } PAGE;

  
  struct commit_entry
  {
    struct commit_entry *next;
    THD *thd;
  };

  char logname[FN_REFLEN];
  File fd;
  my_off_t file_length;
  uint npages, inited;
  uchar *data;
  struct st_page *pages, *syncing, *active, *pool, **pool_last_ptr;
  
  mysql_mutex_t LOCK_active, LOCK_pool, LOCK_sync, LOCK_pending_checkpoint;
  mysql_cond_t COND_pool, COND_active;
  
  commit_entry *commit_ordered_queue;
  
  mysql_cond_t COND_queue_busy;
  my_bool commit_ordered_queue_busy;
  pending_cookies* pending_checkpoint;

  public:
  TC_LOG_MMAP(): inited(0), pending_checkpoint(0) {}
  int open(const char *opt_name);
  void close();
  int log_and_order(THD *thd, my_xid xid, bool all,
                    bool need_prepare_ordered, bool need_commit_ordered);
  int unlog(ulong cookie, my_xid xid);
  void commit_checkpoint_notify(void *cookie);
  int recover();

  private:
  int log_one_transaction(my_xid xid);
  void get_active_from_pool();
  int sync();
  int overflow();
  int delete_entry(ulong cookie);
};
#else
#define TC_LOG_MMAP TC_LOG_DUMMY
#endif

extern TC_LOG *tc_log;
extern TC_LOG_MMAP tc_log_mmap;
extern TC_LOG_DUMMY tc_log_dummy;


#define LOG_INFO_EOF -1
#define LOG_INFO_IO  -2
#define LOG_INFO_INVALID -3
#define LOG_INFO_SEEK -4
#define LOG_INFO_MEM -6
#define LOG_INFO_FATAL -7
#define LOG_INFO_IN_USE -8
#define LOG_INFO_EMFILE -9



#define LOG_CLOSE_INDEX		1
#define LOG_CLOSE_TO_BE_OPENED	2
#define LOG_CLOSE_STOP_EVENT	4
#define LOG_CLOSE_DELAYED_CLOSE 8


#define MAX_LOG_UNIQUE_FN_EXT 0x7FFFFFFF


#define LOG_WARN_UNIQUE_FN_EXT_LEFT 1000

class Relay_log_info;

#ifdef HAVE_PSI_INTERFACE
extern PSI_mutex_key key_LOG_INFO_lock;
#endif


typedef struct st_log_info
{
  char log_file_name[FN_REFLEN];
  my_off_t index_file_offset, index_file_start_offset;
  my_off_t pos;
  bool fatal; 
  mysql_mutex_t lock;
  st_log_info() : index_file_offset(0), index_file_start_offset(0),
      pos(0), fatal(0)
  {
    DBUG_ENTER("LOG_INFO");
    log_file_name[0] = '\0';
    mysql_mutex_init(key_LOG_INFO_lock, &lock, MY_MUTEX_INIT_FAST);
    DBUG_VOID_RETURN;
  }
  ~st_log_info()
  {
    DBUG_ENTER("~LOG_INFO");
    mysql_mutex_destroy(&lock);
    DBUG_VOID_RETURN;
  }
} LOG_INFO;


#define MAX_LOG_HANDLERS_NUM 3


#define LOG_NONE       1U
#define LOG_FILE       2U
#define LOG_TABLE      4U

class Log_event;
class Rows_log_event;

enum enum_log_type { LOG_UNKNOWN, LOG_NORMAL, LOG_BIN };
enum enum_log_state { LOG_OPENED, LOG_CLOSED, LOG_TO_BE_OPENED };



class MYSQL_LOG
{
public:
  MYSQL_LOG();
  void init_pthread_objects();
  void cleanup();
  bool open(
#ifdef HAVE_PSI_INTERFACE
            PSI_file_key log_file_key,
#endif
            const char *log_name,
            enum_log_type log_type,
            const char *new_name, ulong next_file_number,
            enum cache_type io_cache_type_arg);
  bool init_and_set_log_file_name(const char *log_name,
                                  const char *new_name,
                                  ulong next_log_number,
                                  enum_log_type log_type_arg,
                                  enum cache_type io_cache_type_arg);
  void init(enum_log_type log_type_arg,
            enum cache_type io_cache_type_arg);
  void close(uint exiting);
  inline bool is_open() { return log_state != LOG_CLOSED; }
  const char *generate_name(const char *log_name,
                            const char *suffix,
                            bool strip_ext, char *buff);
  int generate_new_name(char *new_name, const char *log_name,
                        ulong next_log_number);
 protected:
  
  mysql_mutex_t LOCK_log;
  char *name;
  char log_file_name[FN_REFLEN];
  char time_buff[20], db[NAME_LEN + 1];
  bool write_error, inited;
  IO_CACHE log_file;
  enum_log_type log_type;
  volatile enum_log_state log_state;
  enum cache_type io_cache_type;
  friend class Log_event;
#ifdef HAVE_PSI_INTERFACE
  
  PSI_file_key m_log_file_key;
#endif
  
};


#define SEMI_SYNC_SLAVE_DELAY_SYNC 1

#define SEMI_SYNC_NEED_ACK  2

class MYSQL_QUERY_LOG: public MYSQL_LOG
{
public:
  MYSQL_QUERY_LOG() : last_time(0) {}
  void reopen_file();
  bool write(time_t event_time, const char *user_host, size_t user_host_len, my_thread_id thread_id,
             const char *command_type, size_t command_type_len,
             const char *sql_text, size_t sql_text_len);
  bool write(THD *thd, time_t current_time,
             const char *user_host, size_t user_host_len,
             ulonglong query_utime, ulonglong lock_utime, bool is_command,
             const char *sql_text, size_t sql_text_len);
  bool open_slow_log(const char *log_name)
  {
    char buf[FN_REFLEN];
    return open(
#ifdef HAVE_PSI_INTERFACE
                key_file_slow_log,
#endif
                generate_name(log_name, "-slow.log", 0, buf),
                LOG_NORMAL, 0, 0, WRITE_CACHE);
  }
  bool open_query_log(const char *log_name)
  {
    char buf[FN_REFLEN];
    return open(
#ifdef HAVE_PSI_INTERFACE
                key_file_query_log,
#endif
                generate_name(log_name, ".log", 0, buf),
                LOG_NORMAL, 0, 0, WRITE_CACHE);
  }

private:
  time_t last_time;
};


#define BINLOG_COOKIE_ERROR_RETURN 0
#define BINLOG_COOKIE_DUMMY_ID 1
#define BINLOG_COOKIE_BASE 2
#define BINLOG_COOKIE_DUMMY(error_flag) \
  ( (BINLOG_COOKIE_DUMMY_ID<<1) | ((error_flag)&1) )
#define BINLOG_COOKIE_MAKE(id, error_flag) \
  ( (((id)+BINLOG_COOKIE_BASE)<<1) | ((error_flag)&1) )
#define BINLOG_COOKIE_GET_ERROR_FLAG(c) ((c) & 1)
#define BINLOG_COOKIE_GET_ID(c) ( ((ulong)(c)>>1) - BINLOG_COOKIE_BASE )
#define BINLOG_COOKIE_IS_DUMMY(c) \
  ( ((ulong)(c)>>1) == BINLOG_COOKIE_DUMMY_ID )

class binlog_cache_mngr;
class binlog_cache_data;
struct rpl_gtid;
struct wait_for_commit;

class MYSQL_BIN_LOG: public TC_LOG, private MYSQL_LOG
{
 private:
#ifdef HAVE_PSI_INTERFACE
  
  PSI_mutex_key m_key_LOCK_index;
  
  PSI_cond_key m_key_relay_log_update;
  
  PSI_cond_key m_key_bin_log_update;
  
  PSI_file_key m_key_file_log;
  
  PSI_file_key m_key_file_log_index;

  PSI_file_key m_key_COND_queue_busy;
  
  PSI_mutex_key m_key_LOCK_binlog_end_pos;
#endif

  struct group_commit_entry
  {
    struct group_commit_entry *next;
    THD *thd;
    binlog_cache_mngr *cache_mngr;
    bool using_stmt_cache;
    bool using_trx_cache;
    
    Log_event *end_event;
    Log_event *incident_event;
    
    int error;
    int commit_errno;
    IO_CACHE *error_cache;
    
    bool all;
    
    bool need_unlog;
    
    bool check_purge;
    
    bool queued_by_other;
    ulong binlog_id;
  };

  
  uint reset_master_pending;
  ulong mark_xid_done_waiting;

  
  mysql_mutex_t LOCK_index;
  mysql_mutex_t LOCK_binlog_end_pos;
  mysql_mutex_t LOCK_xid_list;
  mysql_cond_t  COND_xid_list;
  mysql_cond_t  COND_relay_log_updated, COND_bin_log_updated;
  ulonglong bytes_written;
  IO_CACHE index_file;
  char index_file_name[FN_REFLEN];
  
  IO_CACHE purge_index_file;
  char purge_index_file_name[FN_REFLEN];
  
  ulong max_size;
  
  uint file_id;
  uint open_count;				
  int readers_count;
  
  group_commit_entry *group_commit_queue;
  
  my_bool group_commit_queue_busy;
  mysql_cond_t COND_queue_busy;
  
  ulonglong num_commits;
  
  ulonglong num_group_commits;
  
  ulonglong group_commit_trigger_count, group_commit_trigger_timeout;
  ulonglong group_commit_trigger_lock_wait;

  
  struct Binlog_crypt_data crypto;

  
  uint *sync_period_ptr;
  uint sync_counter;
  bool state_file_deleted;
  bool binlog_state_recover_done;

  inline uint get_sync_period()
  {
    return *sync_period_ptr;
  }

  int write_to_file(IO_CACHE *cache);
  
  int new_file_without_locking();
  int new_file_impl(bool need_lock);
  void do_checkpoint_request(ulong binlog_id);
  void purge();
  int write_transaction_or_stmt(group_commit_entry *entry, uint64 commit_id);
  int queue_for_group_commit(group_commit_entry *entry);
  bool write_transaction_to_binlog_events(group_commit_entry *entry);
  void trx_group_commit_leader(group_commit_entry *leader);
  bool is_xidlist_idle_nolock();
#ifdef WITH_WSREP
  
  inline bool is_gtid_cached(THD *thd);
#endif
public:
  
  struct xid_count_per_binlog : public ilink {
    char *binlog_name;
    uint binlog_name_len;
    ulong binlog_id;
    
    long xid_count;
    long notify_count;
    
    xid_count_per_binlog *next_in_queue;
    xid_count_per_binlog();   
  };
  I_List<xid_count_per_binlog> binlog_xid_count_list;
  mysql_mutex_t LOCK_binlog_background_thread;
  mysql_cond_t COND_binlog_background_thread;
  mysql_cond_t COND_binlog_background_thread_end;

  void stop_background_thread();

  using MYSQL_LOG::generate_name;
  using MYSQL_LOG::is_open;

  
  bool is_relay_log;
  ulong relay_signal_cnt;  
  enum enum_binlog_checksum_alg checksum_alg_reset; 
  
  enum enum_binlog_checksum_alg relay_log_checksum_alg;
  
  Format_description_log_event *description_event_for_exec,
    *description_event_for_queue;
  
  char last_commit_pos_file[FN_REFLEN];
  my_off_t last_commit_pos_offset;
  ulong current_binlog_id;

  MYSQL_BIN_LOG(uint *sync_period);
  

#ifdef HAVE_PSI_INTERFACE
  void set_psi_keys(PSI_mutex_key key_LOCK_index,
                    PSI_cond_key key_relay_log_update,
                    PSI_cond_key key_bin_log_update,
                    PSI_file_key key_file_log,
                    PSI_file_key key_file_log_index,
                    PSI_file_key key_COND_queue_busy,
                    PSI_mutex_key key_LOCK_binlog_end_pos)
  {
    m_key_LOCK_index= key_LOCK_index;
    m_key_relay_log_update=  key_relay_log_update;
    m_key_bin_log_update=    key_bin_log_update;
    m_key_file_log= key_file_log;
    m_key_file_log_index= key_file_log_index;
    m_key_COND_queue_busy= key_COND_queue_busy;
    m_key_LOCK_binlog_end_pos= key_LOCK_binlog_end_pos;
  }
#endif

  int open(const char *opt_name);
  void close();
  int log_and_order(THD *thd, my_xid xid, bool all,
                    bool need_prepare_ordered, bool need_commit_ordered);
  int unlog(ulong cookie, my_xid xid);
  void commit_checkpoint_notify(void *cookie);
  int recover(LOG_INFO *linfo, const char *last_log_name, IO_CACHE *first_log,
              Format_description_log_event *fdle, bool do_xa);
  int do_binlog_recovery(const char *opt_name, bool do_xa_recovery);
#if !defined(MYSQL_CLIENT)

  int flush_and_set_pending_rows_event(THD *thd, Rows_log_event* event,
                                       bool is_transactional);
  int remove_pending_rows_event(THD *thd, bool is_transactional);

#endif 
  void reset_bytes_written()
  {
    bytes_written = 0;
  }
  void harvest_bytes_written(ulonglong* counter)
  {
#ifndef DBUG_OFF
    char buf1[22],buf2[22];
#endif
    DBUG_ENTER("harvest_bytes_written");
    (*counter)+=bytes_written;
    DBUG_PRINT("info",("counter: %s  bytes_written: %s", llstr(*counter,buf1),
		       llstr(bytes_written,buf2)));
    bytes_written=0;
    DBUG_VOID_RETURN;
  }
  void set_max_size(ulong max_size_arg);

  
  void signal_relay_log_update()
  {
    mysql_mutex_assert_owner(&LOCK_log);
    DBUG_ASSERT(is_relay_log);
    DBUG_ENTER("MYSQL_BIN_LOG::signal_relay_log_update");
    relay_signal_cnt++;
    mysql_cond_broadcast(&COND_relay_log_updated);
    DBUG_VOID_RETURN;
  }
  void signal_bin_log_update()
  {
    mysql_mutex_assert_owner(&LOCK_binlog_end_pos);
    DBUG_ASSERT(!is_relay_log);
    DBUG_ENTER("MYSQL_BIN_LOG::signal_bin_log_update");
    mysql_cond_broadcast(&COND_bin_log_updated);
    DBUG_VOID_RETURN;
  }
  void update_binlog_end_pos()
  {
    if (is_relay_log)
      signal_relay_log_update();
    else
    {
      lock_binlog_end_pos();
      binlog_end_pos= my_b_safe_tell(&log_file);
      signal_bin_log_update();
      unlock_binlog_end_pos();
    }
  }
  void update_binlog_end_pos(my_off_t pos)
  {
    mysql_mutex_assert_owner(&LOCK_log);
    mysql_mutex_assert_not_owner(&LOCK_binlog_end_pos);
    lock_binlog_end_pos();
    
    DBUG_ASSERT(pos >= binlog_end_pos);
    binlog_end_pos= pos;
    signal_bin_log_update();
    unlock_binlog_end_pos();
  }

  void wait_for_sufficient_commits();
  void binlog_trigger_immediate_group_commit();
  void wait_for_update_relay_log(THD* thd);
  void init(ulong max_size);
  void init_pthread_objects();
  void cleanup();
  bool open(const char *log_name,
            enum_log_type log_type,
            const char *new_name,
            ulong next_log_number,
	    enum cache_type io_cache_type_arg,
	    ulong max_size,
            bool null_created,
            bool need_mutex);
  bool open_index_file(const char *index_file_name_arg,
                       const char *log_name, bool need_mutex);
  
  int new_file();

  bool write(Log_event* event_info,
             my_bool *with_annotate= 0); 
  bool write_transaction_to_binlog(THD *thd, binlog_cache_mngr *cache_mngr,
                                   Log_event *end_ev, bool all,
                                   bool using_stmt_cache, bool using_trx_cache);

  bool write_incident_already_locked(THD *thd);
  bool write_incident(THD *thd);
  void write_binlog_checkpoint_event_already_locked(const char *name, uint len);
  int  write_cache(THD *thd, IO_CACHE *cache);
  void set_write_error(THD *thd, bool is_transactional);
  bool check_write_error(THD *thd);

  void start_union_events(THD *thd, query_id_t query_id_param);
  void stop_union_events(THD *thd);
  bool is_query_in_union(THD *thd, query_id_t query_id_param);

  bool write_event(Log_event *ev, binlog_cache_data *data, IO_CACHE *file);
  bool write_event(Log_event *ev) { return write_event(ev, 0, &log_file); }

  bool write_event_buffer(uchar* buf,uint len);
  bool append(Log_event* ev);
  bool append_no_lock(Log_event* ev);

  void mark_xids_active(ulong cookie, uint xid_count);
  void mark_xid_done(ulong cookie, bool write_checkpoint);
  void make_log_name(char* buf, const char* log_ident);
  bool is_active(const char* log_file_name);
  bool can_purge_log(const char *log_file_name);
  int update_log_index(LOG_INFO* linfo, bool need_update_threads);
  int rotate(bool force_rotate, bool* check_purge);
  void checkpoint_and_purge(ulong binlog_id);
  int rotate_and_purge(bool force_rotate, DYNAMIC_ARRAY* drop_gtid_domain= NULL);
  
  bool flush_and_sync(bool *synced);
  int purge_logs(const char *to_log, bool included,
                 bool need_mutex, bool need_update_threads,
                 ulonglong *decrease_log_space);
  int purge_logs_before_date(time_t purge_time);
  int purge_first_log(Relay_log_info* rli, bool included);
  int set_purge_index_file_name(const char *base_file_name);
  int open_purge_index_file(bool destroy);
  bool is_inited_purge_index_file();
  int close_purge_index_file();
  int clean_purge_index_file();
  int sync_purge_index_file();
  int register_purge_index_entry(const char* entry);
  int register_create_index_entry(const char* entry);
  int purge_index_entry(THD *thd, ulonglong *decrease_log_space,
                        bool need_mutex);
  bool reset_logs(THD* thd, bool create_new_log,
                  rpl_gtid *init_state, uint32 init_state_len,
                  ulong next_log_number);
  void wait_for_last_checkpoint_event();
  void close(uint exiting);
  void clear_inuse_flag_when_closing(File file);

  
  int find_log_pos(LOG_INFO* linfo, const char* log_name,
		   bool need_mutex);
  int find_next_log(LOG_INFO* linfo, bool need_mutex);
  int get_current_log(LOG_INFO* linfo);
  int raw_get_current_log(LOG_INFO* linfo);
  uint next_file_id();
  inline char* get_index_fname() { return index_file_name;}
  inline char* get_log_fname() { return log_file_name; }
  inline char* get_name() { return name; }
  inline mysql_mutex_t* get_log_lock() { return &LOCK_log; }
  inline mysql_cond_t* get_bin_log_cond() { return &COND_bin_log_updated; }
  inline IO_CACHE* get_log_file() { return &log_file; }

  inline void lock_index() { mysql_mutex_lock(&LOCK_index);}
  inline void unlock_index() { mysql_mutex_unlock(&LOCK_index);}
  inline IO_CACHE *get_index_file() { return &index_file;}
  inline uint32 get_open_count() { return open_count; }
  void set_status_variables(THD *thd);
  bool is_xidlist_idle();
  bool write_gtid_event(THD *thd, bool standalone, bool is_transactional,
                        uint64 commit_id);
  int read_state_from_file();
  int write_state_to_file();
  int get_most_recent_gtid_list(rpl_gtid **list, uint32 *size);
  bool append_state_pos(String *str);
  bool append_state(String *str);
  bool is_empty_state();
  bool find_in_binlog_state(uint32 domain_id, uint32 server_id,
                            rpl_gtid *out_gtid);
  bool lookup_domain_in_binlog_state(uint32 domain_id, rpl_gtid *out_gtid);
  int bump_seq_no_counter_if_needed(uint32 domain_id, uint64 seq_no);
  bool check_strict_gtid_sequence(uint32 domain_id, uint32 server_id,
                                  uint64 seq_no);

  
  void reset_binlog_end_pos(const char file_name[FN_REFLEN], my_off_t pos)
  {
    mysql_mutex_assert_owner(&LOCK_log);
    mysql_mutex_assert_not_owner(&LOCK_binlog_end_pos);
    lock_binlog_end_pos();
    binlog_end_pos= pos;
    strcpy(binlog_end_pos_file, file_name);
    signal_bin_log_update();
    unlock_binlog_end_pos();
  }

  
  my_off_t get_binlog_end_pos(char file_name_buf[FN_REFLEN]) const
  {
    mysql_mutex_assert_not_owner(&LOCK_log);
    mysql_mutex_assert_owner(&LOCK_binlog_end_pos);
    strcpy(file_name_buf, binlog_end_pos_file);
    return binlog_end_pos;
  }
  void lock_binlog_end_pos() { mysql_mutex_lock(&LOCK_binlog_end_pos); }
  void unlock_binlog_end_pos() { mysql_mutex_unlock(&LOCK_binlog_end_pos); }
  mysql_mutex_t* get_binlog_end_pos_lock() { return &LOCK_binlog_end_pos; }

  int wait_for_update_binlog_end_pos(THD* thd, struct timespec * timeout);

  
  my_off_t binlog_end_pos;
  char binlog_end_pos_file[FN_REFLEN];
};

class Log_event_handler
{
public:
  Log_event_handler() {}
  virtual bool init()= 0;
  virtual void cleanup()= 0;

  virtual bool log_slow(THD *thd, my_hrtime_t current_time,
                        const char *user_host, size_t user_host_len, ulonglong query_utime,
                        ulonglong lock_utime, bool is_command,
                        const char *sql_text, size_t sql_text_len)= 0;
  virtual bool log_error(enum loglevel level, const char *format,
                         va_list args)= 0;
  virtual bool log_general(THD *thd, my_hrtime_t event_time, const char *user_host, size_t user_host_len, my_thread_id thread_id,
                           const char *command_type, size_t command_type_len,
                           const char *sql_text, size_t sql_text_len,
                           CHARSET_INFO *client_cs)= 0;
  virtual ~Log_event_handler() {}
};


int check_if_log_table(const TABLE_LIST *table, bool check_if_opened,
                       const char *errmsg);

class Log_to_csv_event_handler: public Log_event_handler
{
  friend class LOGGER;

public:
  Log_to_csv_event_handler();
  ~Log_to_csv_event_handler();
  virtual bool init();
  virtual void cleanup();

  virtual bool log_slow(THD *thd, my_hrtime_t current_time,
                        const char *user_host, size_t user_host_len, ulonglong query_utime,
                        ulonglong lock_utime, bool is_command,
                        const char *sql_text, size_t sql_text_len);
  virtual bool log_error(enum loglevel level, const char *format,
                         va_list args);
  virtual bool log_general(THD *thd, my_hrtime_t event_time, const char *user_host, size_t user_host_len, my_thread_id thread_id,
                           const char *command_type, size_t command_type_len,
                           const char *sql_text, size_t sql_text_len,
                           CHARSET_INFO *client_cs);

  int activate_log(THD *thd, uint log_type);
};



#define QUERY_LOG_SLOW 1
#define QUERY_LOG_GENERAL 2

class Log_to_file_event_handler: public Log_event_handler
{
  MYSQL_QUERY_LOG mysql_log;
  MYSQL_QUERY_LOG mysql_slow_log;
  bool is_initialized;
public:
  Log_to_file_event_handler(): is_initialized(FALSE)
  {}
  virtual bool init();
  virtual void cleanup();

  virtual bool log_slow(THD *thd, my_hrtime_t current_time,
                        const char *user_host, size_t user_host_len, ulonglong query_utime,
                        ulonglong lock_utime, bool is_command,
                        const char *sql_text, size_t sql_text_len);
  virtual bool log_error(enum loglevel level, const char *format,
                         va_list args);
  virtual bool log_general(THD *thd, my_hrtime_t event_time, const char *user_host, size_t user_host_len, my_thread_id thread_id,
                           const char *command_type, size_t command_type_len,
                           const char *sql_text, size_t sql_text_len,
                           CHARSET_INFO *client_cs);
  void flush();
  void init_pthread_objects();
  MYSQL_QUERY_LOG *get_mysql_slow_log() { return &mysql_slow_log; }
  MYSQL_QUERY_LOG *get_mysql_log() { return &mysql_log; }
};



class LOGGER
{
  mysql_rwlock_t LOCK_logger;
  
  uint inited;

  
  Log_to_csv_event_handler *table_log_handler;
  Log_to_file_event_handler *file_log_handler;

  
  Log_event_handler *error_log_handler_list[MAX_LOG_HANDLERS_NUM + 1];
  Log_event_handler *slow_log_handler_list[MAX_LOG_HANDLERS_NUM + 1];
  Log_event_handler *general_log_handler_list[MAX_LOG_HANDLERS_NUM + 1];

public:

  bool is_log_tables_initialized;

  LOGGER() : inited(0), table_log_handler(NULL),
             file_log_handler(NULL), is_log_tables_initialized(FALSE)
  {}
  void lock_shared() { mysql_rwlock_rdlock(&LOCK_logger); }
  void lock_exclusive() { mysql_rwlock_wrlock(&LOCK_logger); }
  void unlock() { mysql_rwlock_unlock(&LOCK_logger); }
  bool is_log_table_enabled(uint log_table_type);
  bool log_command(THD *thd, enum enum_server_command command);

  
  void init_base();
  void init_log_tables();
  bool flush_slow_log();
  bool flush_general_log();
  
  void cleanup_base();
  
  void cleanup_end();
  bool error_log_print(enum loglevel level, const char *format,
                      va_list args);
  bool slow_log_print(THD *thd, const char *query, size_t query_length,
                      ulonglong current_utime);
  bool general_log_print(THD *thd,enum enum_server_command command,
                         const char *format, va_list args);
  bool general_log_write(THD *thd, enum enum_server_command command,
                         const char *query, size_t query_length);

  
  int set_handlers(ulonglong error_log_printer,
                   ulonglong slow_log_printer,
                   ulonglong general_log_printer);
  void init_error_log(ulonglong error_log_printer);
  void init_slow_log(ulonglong slow_log_printer);
  void init_general_log(ulonglong general_log_printer);
  void deactivate_log_handler(THD* thd, uint log_type);
  bool activate_log_handler(THD* thd, uint log_type);
  MYSQL_QUERY_LOG *get_slow_log_file_handler() const
  { 
    if (file_log_handler)
      return file_log_handler->get_mysql_slow_log();
    return NULL;
  }
  MYSQL_QUERY_LOG *get_log_file_handler() const
  { 
    if (file_log_handler)
      return file_log_handler->get_mysql_log();
    return NULL;
  }
};

enum enum_binlog_format {
  BINLOG_FORMAT_MIXED= 0, 
  BINLOG_FORMAT_STMT=  1, 
  BINLOG_FORMAT_ROW=   2, 
  BINLOG_FORMAT_UNSPEC=3  
};

int query_error_code(THD *thd, bool not_killed);
uint purge_log_get_error_code(int res);

int vprint_msg_to_log(enum loglevel level, const char *format, va_list args);
void sql_print_error(const char *format, ...);
void sql_print_warning(const char *format, ...);
void sql_print_information(const char *format, ...);
void sql_print_information_v(const char *format, va_list ap);
typedef void (*sql_print_message_func)(const char *format, ...);
extern sql_print_message_func sql_print_message_handlers[];

int error_log_print(enum loglevel level, const char *format,
                    va_list args);

bool slow_log_print(THD *thd, const char *query, uint query_length,
                    ulonglong current_utime);

bool general_log_print(THD *thd, enum enum_server_command command,
                       const char *format,...);

bool general_log_write(THD *thd, enum enum_server_command command,
                       const char *query, size_t query_length);

void binlog_report_wait_for(THD *thd, THD *other_thd);
void sql_perror(const char *message);
bool flush_error_log();

File open_binlog(IO_CACHE *log, const char *log_file_name,
                 const char **errmsg);

void make_default_log_name(char **out, const char* log_ext, bool once);
void binlog_reset_cache(THD *thd);

extern MYSQL_PLUGIN_IMPORT MYSQL_BIN_LOG mysql_bin_log;
extern handlerton *binlog_hton;
extern LOGGER logger;

extern const char *log_bin_index;
extern const char *log_bin_basename;



inline bool normalize_binlog_name(char *to, const char *from, bool is_relay_log)
{
  DBUG_ENTER("normalize_binlog_name");
  bool error= false;
  char buff[FN_REFLEN];
  char *ptr= (char*) from;
  char *opt_name= is_relay_log ? opt_relay_logname : opt_bin_logname;

  DBUG_ASSERT(from);

  
  if (opt_name && opt_name[0] && from && !test_if_hard_path(from))
  {
    
    
    char log_dirpart[FN_REFLEN], log_dirname[FN_REFLEN];
    size_t log_dirpart_len, log_dirname_len;
    dirname_part(log_dirpart, opt_name, &log_dirpart_len);
    dirname_part(log_dirname, from, &log_dirname_len);

    
    if (log_dirpart_len > 0)
    {
      
      if(fn_format(buff, from+log_dirname_len, log_dirpart, "",
                   MYF(MY_UNPACK_FILENAME | MY_SAFE_PATH)) == NULL)
      {
        error= true;
        goto end;
      }

      ptr= buff;
    }
  }

  DBUG_ASSERT(ptr);

  if (ptr)
    strmake(to, ptr, strlen(ptr));

end:
  DBUG_RETURN(error);
}

static inline TC_LOG *get_tc_log_implementation()
{
  if (total_ha_2pc <= 1)
    return &tc_log_dummy;
  if (opt_bin_log)
    return &mysql_bin_log;
  return &tc_log_mmap;
}


class Gtid_list_log_event;
const char *
get_gtid_list_event(IO_CACHE *cache, Gtid_list_log_event **out_gtid_list);

#endif 
