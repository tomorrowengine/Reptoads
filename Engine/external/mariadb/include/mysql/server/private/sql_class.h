

#ifndef SQL_CLASS_INCLUDED
#define SQL_CLASS_INCLUDED



#include "dur_prop.h"
#include <waiting_threads.h>
#include "sql_const.h"
#include <mysql/plugin_audit.h>
#include "log.h"
#include "rpl_tblmap.h"
#include "mdl.h"
#include "field.h"                              
#include "probes_mysql.h"
#include "sql_locale.h"     
#include "sql_profile.h"    
#include "scheduler.h"      
#include "protocol.h"       
#include "violite.h"        
#include "thr_lock.h"       
#include "thr_timer.h"
#include "thr_malloc.h"
#include "log_slow.h"      

#include "sql_digest_stream.h"            

#include <mysql/psi/mysql_stage.h>
#include <mysql/psi/mysql_statement.h>
#include <mysql/psi/mysql_idle.h>
#include <mysql/psi/mysql_table.h>
#include <mysql_com_server.h>
#include "session_tracker.h"

extern "C"
void set_thd_stage_info(void *thd,
                        const PSI_stage_info *new_stage,
                        PSI_stage_info *old_stage,
                        const char *calling_func,
                        const char *calling_file,
                        const unsigned int calling_line);

#define THD_STAGE_INFO(thd, stage) \
  (thd)->enter_stage(&stage, __func__, __FILE__, __LINE__)

#include "my_apc.h"
#include "rpl_gtid.h"
#include "wsrep_mysqld.h"

class Reprepare_observer;
class Relay_log_info;
struct rpl_group_info;
class Rpl_filter;
class Query_log_event;
class Load_log_event;
class sp_rcontext;
class sp_cache;
class Lex_input_stream;
class Parser_state;
class Rows_log_event;
class Sroutine_hash_entry;
class user_var_entry;
struct Trans_binlog_info;
class rpl_io_thread_info;
class rpl_sql_thread_info;

enum enum_ha_read_modes { RFIRST, RNEXT, RPREV, RLAST, RKEY, RNEXT_SAME };
enum enum_duplicates { DUP_ERROR, DUP_REPLACE, DUP_UPDATE };
enum enum_delay_key_write { DELAY_KEY_WRITE_NONE, DELAY_KEY_WRITE_ON,
			    DELAY_KEY_WRITE_ALL };
enum enum_slave_exec_mode { SLAVE_EXEC_MODE_STRICT,
                            SLAVE_EXEC_MODE_IDEMPOTENT,
                            SLAVE_EXEC_MODE_LAST_BIT };
enum enum_slave_run_triggers_for_rbr { SLAVE_RUN_TRIGGERS_FOR_RBR_NO,
                                       SLAVE_RUN_TRIGGERS_FOR_RBR_YES,
                                       SLAVE_RUN_TRIGGERS_FOR_RBR_LOGGING};
enum enum_slave_type_conversions { SLAVE_TYPE_CONVERSIONS_ALL_LOSSY,
                                   SLAVE_TYPE_CONVERSIONS_ALL_NON_LOSSY};


enum enum_column_usage
{ COLUMNS_READ, COLUMNS_WRITE, MARK_COLUMNS_READ, MARK_COLUMNS_WRITE};

static inline bool should_mark_column(enum_column_usage column_usage)
{ return column_usage >= MARK_COLUMNS_READ; }

enum enum_filetype { FILETYPE_CSV, FILETYPE_XML };

enum enum_binlog_row_image {
  
  BINLOG_ROW_IMAGE_MINIMAL= 0,
  
  BINLOG_ROW_IMAGE_NOBLOB= 1,
  
  BINLOG_ROW_IMAGE_FULL= 2
};



#define MODE_REAL_AS_FLOAT              (1ULL << 0)
#define MODE_PIPES_AS_CONCAT            (1ULL << 1)
#define MODE_ANSI_QUOTES                (1ULL << 2)
#define MODE_IGNORE_SPACE               (1ULL << 3)
#define MODE_IGNORE_BAD_TABLE_OPTIONS   (1ULL << 4)
#define MODE_ONLY_FULL_GROUP_BY         (1ULL << 5)
#define MODE_NO_UNSIGNED_SUBTRACTION    (1ULL << 6)
#define MODE_NO_DIR_IN_CREATE           (1ULL << 7)
#define MODE_POSTGRESQL                 (1ULL << 8)
#define MODE_ORACLE                     (1ULL << 9)
#define MODE_MSSQL                      (1ULL << 10)
#define MODE_DB2                        (1ULL << 11)
#define MODE_MAXDB                      (1ULL << 12)
#define MODE_NO_KEY_OPTIONS             (1ULL << 13)
#define MODE_NO_TABLE_OPTIONS           (1ULL << 14)
#define MODE_NO_FIELD_OPTIONS           (1ULL << 15)
#define MODE_MYSQL323                   (1ULL << 16)
#define MODE_MYSQL40                    (1ULL << 17)
#define MODE_ANSI                       (1ULL << 18)
#define MODE_NO_AUTO_VALUE_ON_ZERO      (1ULL << 19)
#define MODE_NO_BACKSLASH_ESCAPES       (1ULL << 20)
#define MODE_STRICT_TRANS_TABLES        (1ULL << 21)
#define MODE_STRICT_ALL_TABLES          (1ULL << 22)
#define MODE_NO_ZERO_IN_DATE            (1ULL << 23)
#define MODE_NO_ZERO_DATE               (1ULL << 24)
#define MODE_INVALID_DATES              (1ULL << 25)
#define MODE_ERROR_FOR_DIVISION_BY_ZERO (1ULL << 26)
#define MODE_TRADITIONAL                (1ULL << 27)
#define MODE_NO_AUTO_CREATE_USER        (1ULL << 28)
#define MODE_HIGH_NOT_PRECEDENCE        (1ULL << 29)
#define MODE_NO_ENGINE_SUBSTITUTION     (1ULL << 30)
#define MODE_PAD_CHAR_TO_FULL_LENGTH    (1ULL << 31)
#define MODE_EMPTY_STRING_IS_NULL       (1ULL << 32)
#define MODE_SIMULTANEOUS_ASSIGNMENT    (1ULL << 33)


#define OLD_MODE_NO_DUP_KEY_WARNINGS_WITH_IGNORE	(1 << 0)
#define OLD_MODE_NO_PROGRESS_INFO			(1 << 1)
#define OLD_MODE_ZERO_DATE_TIME_CAST                    (1 << 2)

extern char internal_table_name[2];
extern char empty_c_string[1];
extern MYSQL_PLUGIN_IMPORT const char **errmesg;

extern bool volatile shutdown_in_progress;

extern "C" LEX_STRING * thd_query_string (MYSQL_THD thd);
extern "C" size_t thd_query_safe(MYSQL_THD thd, char *buf, size_t buflen);


class CSET_STRING
{
private:
  LEX_STRING string;
  CHARSET_INFO *cs;
public:
  CSET_STRING() : cs(&my_charset_bin)
  {
    string.str= NULL;
    string.length= 0;
  }
  CSET_STRING(char *str_arg, size_t length_arg, CHARSET_INFO *cs_arg) :
  cs(cs_arg)
  {
    DBUG_ASSERT(cs_arg != NULL);
    string.str= str_arg;
    string.length= length_arg;
  }

  inline char *str() const { return string.str; }
  inline size_t length() const { return string.length; }
  CHARSET_INFO *charset() const { return cs; }

  friend LEX_STRING * thd_query_string (MYSQL_THD thd);
};


#define TC_HEURISTIC_RECOVER_COMMIT   1
#define TC_HEURISTIC_RECOVER_ROLLBACK 2
extern ulong tc_heuristic_recover;

typedef struct st_user_var_events
{
  user_var_entry *user_var_event;
  char *value;
  size_t length;
  Item_result type;
  uint charset_number;
  bool unsigned_flag;
} BINLOG_USER_VAR_EVENT;


typedef struct st_copy_info {
  ha_rows records; 
  ha_rows deleted; 
  ha_rows updated; 
  ha_rows copied;  
  ha_rows error_count;
  ha_rows touched; 
  enum enum_duplicates handle_duplicates;
  int escape_char, last_errno;
  bool ignore;
  
  List<Item> *update_fields;
  List<Item> *update_values;
  
  TABLE_LIST *view;
  TABLE_LIST *table_list;                       
} COPY_INFO;


class Key_part_spec :public Sql_alloc {
public:
  LEX_CSTRING field_name;
  uint length;
  Key_part_spec(const LEX_CSTRING *name, uint len)
    : field_name(*name), length(len)
  {}
  bool operator==(const Key_part_spec& other) const;
  
  Key_part_spec *clone(MEM_ROOT *mem_root) const
  { return new (mem_root) Key_part_spec(*this); }
};


class Alter_drop :public Sql_alloc {
public:
  enum drop_type {KEY, COLUMN, FOREIGN_KEY, CHECK_CONSTRAINT };
  const char *name;
  enum drop_type type;
  bool drop_if_exists;
  Alter_drop(enum drop_type par_type,const char *par_name, bool par_exists)
    :name(par_name), type(par_type), drop_if_exists(par_exists)
  {
    DBUG_ASSERT(par_name != NULL);
  }
  
  Alter_drop *clone(MEM_ROOT *mem_root) const
    { return new (mem_root) Alter_drop(*this); }
  const char *type_name()
  {
    return type == COLUMN ? "COLUMN" :
           type == CHECK_CONSTRAINT ? "CONSTRAINT" :
           type == KEY ? "INDEX" : "FOREIGN KEY";
  }
};


class Alter_column :public Sql_alloc {
public:
  const char *name;
  Virtual_column_info *default_value;
  bool alter_if_exists;
  Alter_column(const char *par_name, Virtual_column_info *expr, bool par_exists)
    :name(par_name), default_value(expr), alter_if_exists(par_exists) {}
  
  Alter_column *clone(MEM_ROOT *mem_root) const
    { return new (mem_root) Alter_column(*this); }
};


class Key :public Sql_alloc, public DDL_options {
public:
  enum Keytype { PRIMARY, UNIQUE, MULTIPLE, FULLTEXT, SPATIAL, FOREIGN_KEY};
  enum Keytype type;
  KEY_CREATE_INFO key_create_info;
  List<Key_part_spec> columns;
  LEX_CSTRING name;
  engine_option_value *option_list;
  bool generated;
  bool invisible;

  Key(enum Keytype type_par, const LEX_CSTRING *name_arg,
      ha_key_alg algorithm_arg, bool generated_arg, DDL_options_st ddl_options)
    :DDL_options(ddl_options),
     type(type_par), key_create_info(default_key_create_info),
    name(*name_arg), option_list(NULL), generated(generated_arg),
    invisible(false)
  {
    key_create_info.algorithm= algorithm_arg;
  }
  Key(enum Keytype type_par, const LEX_CSTRING *name_arg,
      KEY_CREATE_INFO *key_info_arg,
      bool generated_arg, List<Key_part_spec> *cols,
      engine_option_value *create_opt, DDL_options_st ddl_options)
    :DDL_options(ddl_options),
     type(type_par), key_create_info(*key_info_arg), columns(*cols),
    name(*name_arg), option_list(create_opt), generated(generated_arg),
    invisible(false)
  {}
  Key(const Key &rhs, MEM_ROOT *mem_root);
  virtual ~Key() {}
  
  friend bool foreign_key_prefix(Key *a, Key *b);
  
  virtual Key *clone(MEM_ROOT *mem_root) const
    { return new (mem_root) Key(*this, mem_root); }
};


class Foreign_key: public Key {
public:
  enum fk_match_opt { FK_MATCH_UNDEF, FK_MATCH_FULL,
		      FK_MATCH_PARTIAL, FK_MATCH_SIMPLE};
  LEX_CSTRING ref_db;
  LEX_CSTRING ref_table;
  List<Key_part_spec> ref_columns;
  enum enum_fk_option delete_opt, update_opt;
  enum fk_match_opt match_opt;
  Foreign_key(const LEX_CSTRING *name_arg, List<Key_part_spec> *cols,
	      const LEX_CSTRING *ref_db_arg, const LEX_CSTRING *ref_table_arg,
              List<Key_part_spec> *ref_cols,
              enum_fk_option delete_opt_arg, enum_fk_option update_opt_arg,
              fk_match_opt match_opt_arg,
	      DDL_options ddl_options)
    :Key(FOREIGN_KEY, name_arg, &default_key_create_info, 0, cols, NULL,
         ddl_options),
    ref_db(*ref_db_arg), ref_table(*ref_table_arg), ref_columns(*ref_cols),
    delete_opt(delete_opt_arg), update_opt(update_opt_arg),
    match_opt(match_opt_arg)
   {
    
    key_create_info.check_for_duplicate_indexes= false;
  }
 Foreign_key(const Foreign_key &rhs, MEM_ROOT *mem_root);
  
  virtual Key *clone(MEM_ROOT *mem_root) const
  { return new (mem_root) Foreign_key(*this, mem_root); }
  
  bool validate(List<Create_field> &table_fields);
};

typedef struct st_mysql_lock
{
  TABLE **table;
  THR_LOCK_DATA **locks;
  uint table_count,lock_count;
  uint flags;
} MYSQL_LOCK;


class LEX_COLUMN : public Sql_alloc
{
public:
  String column;
  uint rights;
  LEX_COLUMN (const String& x,const  uint& y ): column (x),rights (y) {}
};

class MY_LOCALE;



struct Query_cache_block;

struct Query_cache_tls
{
  
  Query_cache_block *first_query_block;
  void set_first_query_block(Query_cache_block *first_query_block_arg)
  {
    first_query_block= first_query_block_arg;
  }

  Query_cache_tls() :first_query_block(NULL) {}
};




typedef enum enum_diag_condition_item_name
{
  

  DIAG_CLASS_ORIGIN= 0,
  FIRST_DIAG_SET_PROPERTY= DIAG_CLASS_ORIGIN,
  DIAG_SUBCLASS_ORIGIN= 1,
  DIAG_CONSTRAINT_CATALOG= 2,
  DIAG_CONSTRAINT_SCHEMA= 3,
  DIAG_CONSTRAINT_NAME= 4,
  DIAG_CATALOG_NAME= 5,
  DIAG_SCHEMA_NAME= 6,
  DIAG_TABLE_NAME= 7,
  DIAG_COLUMN_NAME= 8,
  DIAG_CURSOR_NAME= 9,
  DIAG_MESSAGE_TEXT= 10,
  DIAG_MYSQL_ERRNO= 11,
  LAST_DIAG_SET_PROPERTY= DIAG_MYSQL_ERRNO
} Diag_condition_item_name;


extern const LEX_CSTRING Diag_condition_item_names[];


enum killed_state
{
  NOT_KILLED= 0,
  KILL_HARD_BIT= 1,                             
  KILL_BAD_DATA= 2,
  KILL_BAD_DATA_HARD= 3,
  KILL_QUERY= 4,
  KILL_QUERY_HARD= 5,
  
  ABORT_QUERY= 6,
  ABORT_QUERY_HARD= 7,
  KILL_TIMEOUT= 8,
  KILL_TIMEOUT_HARD= 9,
  
  KILL_SLAVE_SAME_ID= 10,
  
  KILL_CONNECTION= 12,
  KILL_CONNECTION_HARD= 13,
  KILL_SYSTEM_THREAD= 14,
  KILL_SYSTEM_THREAD_HARD= 15,
  KILL_SERVER= 16,
  KILL_SERVER_HARD= 17,
  
  KILL_WAIT_TIMEOUT= 18,
  KILL_WAIT_TIMEOUT_HARD= 19

};

#define killed_mask_hard(killed) ((killed_state) ((killed) & ~KILL_HARD_BIT))

enum killed_type
{
  KILL_TYPE_ID,
  KILL_TYPE_USER,
  KILL_TYPE_QUERY
};

#include "sql_lex.h"				

class Delayed_insert;
class select_result;
class Time_zone;

#define THD_SENTRY_MAGIC 0xfeedd1ff
#define THD_SENTRY_GONE  0xdeadbeef

#define THD_CHECK_SENTRY(thd) DBUG_ASSERT(thd->dbug_sentry == THD_SENTRY_MAGIC)

typedef struct system_variables
{
  
  ulong dynamic_variables_version;
  char* dynamic_variables_ptr;
  uint dynamic_variables_head;    
  uint dynamic_variables_size;    
  
  ulonglong max_heap_table_size;
  ulonglong tmp_memory_table_size;
  ulonglong tmp_disk_table_size;
  ulonglong long_query_time;
  ulonglong max_statement_time;
  ulonglong optimizer_switch;
  sql_mode_t sql_mode; 
  sql_mode_t old_behavior; 
  ulonglong option_bits; 
  ulonglong join_buff_space_limit;
  ulonglong log_slow_filter; 
  ulonglong log_slow_verbosity; 
  ulonglong log_slow_disabled_statements;
  ulonglong log_disabled_statements;
  ulonglong bulk_insert_buff_size;
  ulonglong join_buff_size;
  ulonglong sortbuff_size;
  ulonglong group_concat_max_len;
  ulonglong default_regex_flags;
  ulonglong max_mem_used;

  
  ulonglong slave_skip_counter;
  ulonglong max_relay_log_size;

  ha_rows select_limit;
  ha_rows max_join_size;
  ha_rows expensive_subquery_limit;
  ulong auto_increment_increment, auto_increment_offset;
#ifdef WITH_WSREP
  
  ulong saved_auto_increment_increment, saved_auto_increment_offset;
#endif 
  uint eq_range_index_dive_limit;
  ulong column_compression_zlib_strategy;
  ulong lock_wait_timeout;
  ulong join_cache_level;
  ulong max_allowed_packet;
  ulong max_error_count;
  ulong max_length_for_sort_data;
  ulong max_recursive_iterations;
  ulong max_sort_length;
  ulong max_tmp_tables;
  ulong max_insert_delayed_threads;
  ulong min_examined_row_limit;
  ulong multi_range_count;
  ulong net_buffer_length;
  ulong net_interactive_timeout;
  ulong net_read_timeout;
  ulong net_retry_count;
  ulong net_wait_timeout;
  ulong net_write_timeout;
  ulong optimizer_prune_level;
  ulong optimizer_search_depth;
  ulong optimizer_selectivity_sampling_limit;
  ulong optimizer_use_condition_selectivity;
  ulong use_stat_tables;
  ulong histogram_size;
  ulong histogram_type;
  ulong preload_buff_size;
  ulong profiling_history_size;
  ulong read_buff_size;
  ulong read_rnd_buff_size;
  ulong mrr_buff_size;
  ulong div_precincrement;
  
  ulong rowid_merge_buff_size;
  ulong max_sp_recursion_depth;
  ulong default_week_format;
  ulong max_seeks_for_key;
  ulong range_alloc_block_size;
  ulong query_alloc_block_size;
  ulong query_prealloc_size;
  ulong trans_alloc_block_size;
  ulong trans_prealloc_size;
  ulong log_warnings;
  
  ulong log_slow_rate_limit; 
  ulong binlog_format; 
  ulong binlog_row_image;
  ulong progress_report_time;
  ulong completion_type;
  ulong query_cache_type;
  ulong tx_isolation;
  ulong updatable_views_with_limit;
  ulong alter_algorithm;
  int max_user_connections;
  ulong server_id;
  
  my_thread_id pseudo_thread_id;
  
  uint32     gtid_domain_id;
  uint64     gtid_seq_no;

  
  my_bool tx_read_only;
  my_bool low_priority_updates;
  my_bool query_cache_wlock_invalidate;
  my_bool keep_files_on_create;

  my_bool old_mode;
  my_bool old_passwords;
  my_bool big_tables;
  my_bool only_standard_compliant_cte;
  my_bool query_cache_strip_comments;
  my_bool sql_log_slow;
  my_bool sql_log_bin;
  
  my_bool sql_log_bin_off;
  my_bool binlog_annotate_row_events;
  my_bool binlog_direct_non_trans_update;
  my_bool column_compression_zlib_wrap;

  plugin_ref table_plugin;
  plugin_ref tmp_table_plugin;
  plugin_ref enforced_table_plugin;

  
  CHARSET_INFO  *character_set_filesystem;
  CHARSET_INFO  *character_set_client;
  CHARSET_INFO  *character_set_results;

  
  CHARSET_INFO	*collation_server;
  CHARSET_INFO	*collation_database;
  CHARSET_INFO  *collation_connection;

  
  LEX_CSTRING default_master_connection;

  
  MY_LOCALE *lc_messages;
  const char ***errmsgs;             

  
  MY_LOCALE *lc_time_names;

  Time_zone *time_zone;

  my_bool sysdate_is_now;

  
  ulong wt_timeout_short, wt_deadlock_search_depth_short;
  ulong wt_timeout_long, wt_deadlock_search_depth_long;

  my_bool wsrep_on;
  my_bool wsrep_causal_reads;
  my_bool wsrep_dirty_reads;
  uint wsrep_sync_wait;
  ulong wsrep_retry_autocommit;
  ulong wsrep_OSU_method;
  double long_query_time_double, max_statement_time_double;

  my_bool pseudo_slave_mode;

  char *session_track_system_variables;
  ulong session_track_transaction_info;
  my_bool session_track_schema;
  my_bool session_track_state_change;

  ulong threadpool_priority;

  uint idle_transaction_timeout;
  uint idle_readonly_transaction_timeout;
  uint idle_write_transaction_timeout;
  uint column_compression_threshold;
  uint column_compression_zlib_level;
  uint in_subquery_conversion_threshold;

  vers_asof_timestamp_t vers_asof_timestamp;
  ulong vers_alter_history;
} SV;



typedef struct system_status_var
{
  ulong column_compressions;
  ulong column_decompressions;
  ulong com_stat[(uint) SQLCOM_END];
  ulong com_create_tmp_table;
  ulong com_drop_tmp_table;
  ulong com_other;
  ulong com_multi;

  ulong com_stmt_prepare;
  ulong com_stmt_reprepare;
  ulong com_stmt_execute;
  ulong com_stmt_send_long_data;
  ulong com_stmt_fetch;
  ulong com_stmt_reset;
  ulong com_stmt_close;

  ulong com_register_slave;
  ulong created_tmp_disk_tables_;
  ulong created_tmp_tables_;
  ulong ha_commit_count;
  ulong ha_delete_count;
  ulong ha_read_first_count;
  ulong ha_read_last_count;
  ulong ha_read_key_count;
  ulong ha_read_next_count;
  ulong ha_read_prev_count;
  ulong ha_read_retry_count;
  ulong ha_read_rnd_count;
  ulong ha_read_rnd_next_count;
  ulong ha_read_rnd_deleted_count;

  
  ulong ha_mrr_init_count;
  ulong ha_mrr_key_refills_count;
  ulong ha_mrr_rowid_refills_count;

  ulong ha_rollback_count;
  ulong ha_update_count;
  ulong ha_write_count;
  
  ulong ha_tmp_update_count;
  ulong ha_tmp_write_count;
  ulong ha_tmp_delete_count;
  ulong ha_prepare_count;
  ulong ha_icp_attempts;
  ulong ha_icp_match;
  ulong ha_discover_count;
  ulong ha_savepoint_count;
  ulong ha_savepoint_rollback_count;
  ulong ha_external_lock_count;

  ulong opened_tables;
  ulong opened_shares;
  ulong opened_views;               

  ulong select_full_join_count_;
  ulong select_full_range_join_count_;
  ulong select_range_count_;
  ulong select_range_check_count_;
  ulong select_scan_count_;
  ulong update_scan_count;
  ulong delete_scan_count;
  ulong executed_triggers;
  ulong long_query_count;
  ulong filesort_merge_passes_;
  ulong filesort_range_count_;
  ulong filesort_rows_;
  ulong filesort_scan_count_;
  ulong filesort_pq_sorts_;

  
  ulong feature_custom_aggregate_functions; 
  ulong feature_dynamic_columns;    
  ulong feature_fulltext;	    
  ulong feature_gis;                
  ulong feature_invisible_columns;     
  ulong feature_json;		    
  ulong feature_locale;		    
  ulong feature_subquery;	    
  ulong feature_system_versioning;  
  ulong feature_timezone;	    
  ulong feature_trigger;	    
  ulong feature_xml;		    
  ulong feature_window_functions;   

  
  ulonglong master_gtid_wait_timeouts;          
  ulonglong master_gtid_wait_time;              
  ulonglong master_gtid_wait_count;

  ulong empty_queries;
  ulong access_denied_errors;
  ulong lost_connections;
  ulong max_statement_time_exceeded;
  
  ulong questions;
  
  ulonglong bytes_received;
  ulonglong bytes_sent;
  ulonglong rows_read;
  ulonglong rows_sent;
  ulonglong rows_tmp_read;
  ulonglong binlog_bytes_written;
  ulonglong table_open_cache_hits;
  ulonglong table_open_cache_misses;
  ulonglong table_open_cache_overflows;
  double last_query_cost;
  double cpu_time, busy_time;
  uint32 threads_running;
  
  
  int64 max_local_memory_used;
  volatile int64 local_memory_used;
  
  volatile int64 global_memory_used;
} STATUS_VAR;



#define last_system_status_var questions
#define last_cleared_system_status_var local_memory_used



extern ulong feature_files_opened_with_delayed_keys, feature_check_constraint;

void add_to_status(STATUS_VAR *to_var, STATUS_VAR *from_var);

void add_diff_to_status(STATUS_VAR *to_var, STATUS_VAR *from_var,
                        STATUS_VAR *dec_var);


inline void update_global_memory_status(int64 size)
{
  DBUG_PRINT("info", ("global memory_used: %lld  size: %lld",
                      (longlong) global_status_var.global_memory_used,
                      size));
  
  int64 volatile * volatile ptr= &global_status_var.global_memory_used;
  my_atomic_add64_explicit(ptr, size, MY_MEMORY_ORDER_RELAXED);
}


inline CHARSET_INFO *
mysqld_collation_get_by_name(const char *name,
                             CHARSET_INFO *name_cs= system_charset_info)
{
  CHARSET_INFO *cs;
  MY_CHARSET_LOADER loader;
  my_charset_loader_init_mysys(&loader);
  if (!(cs= my_collation_get_by_name(&loader, name, MYF(0))))
  {
    ErrConvString err(name, name_cs);
    my_error(ER_UNKNOWN_COLLATION, MYF(0), err.ptr());
    if (loader.error[0])
      push_warning_printf(current_thd,
                          Sql_condition::WARN_LEVEL_WARN,
                          ER_UNKNOWN_COLLATION, "%s", loader.error);
  }
  return cs;
}

inline bool is_supported_parser_charset(CHARSET_INFO *cs)
{
  return MY_TEST(cs->mbminlen == 1);
}

#ifdef MYSQL_SERVER

void free_tmp_table(THD *thd, TABLE *entry);



#ifdef DBUG_ASSERT_EXISTS
#define INIT_ARENA_DBUG_INFO is_backup_arena= 0; is_reprepared= FALSE;
#else
#define INIT_ARENA_DBUG_INFO
#endif

class Query_arena
{
public:
  
  Item *free_list;
  MEM_ROOT *mem_root;                   
#ifdef DBUG_ASSERT_EXISTS
  bool is_backup_arena; 
  bool is_reprepared;
#endif
  
  enum enum_state
  {
    STMT_INITIALIZED= 0, STMT_INITIALIZED_FOR_SP= 1, STMT_PREPARED= 2,
    STMT_CONVENTIONAL_EXECUTION= 3, STMT_EXECUTED= 4, STMT_ERROR= -1
  };

  enum_state state;

public:
  
  enum Type
  {
    STATEMENT, PREPARED_STATEMENT, STORED_PROCEDURE
  };

  Query_arena(MEM_ROOT *mem_root_arg, enum enum_state state_arg) :
    free_list(0), mem_root(mem_root_arg), state(state_arg)
  { INIT_ARENA_DBUG_INFO; }
  
  Query_arena() { INIT_ARENA_DBUG_INFO; }

  virtual Type type() const;
  virtual ~Query_arena() {};

  inline bool is_stmt_prepare() const { return state == STMT_INITIALIZED; }
  inline bool is_stmt_prepare_or_first_sp_execute() const
  { return (int)state < (int)STMT_PREPARED; }
  inline bool is_stmt_prepare_or_first_stmt_execute() const
  { return (int)state <= (int)STMT_PREPARED; }
  inline bool is_stmt_execute() const
  { return state == STMT_PREPARED || state == STMT_EXECUTED; }
  inline bool is_conventional() const
  { return state == STMT_CONVENTIONAL_EXECUTION; }

  inline void* alloc(size_t size) { return alloc_root(mem_root,size); }
  inline void* calloc(size_t size)
  {
    void *ptr;
    if (likely((ptr=alloc_root(mem_root,size))))
      bzero(ptr, size);
    return ptr;
  }
  inline char *strdup(const char *str)
  { return strdup_root(mem_root,str); }
  inline char *strmake(const char *str, size_t size)
  { return strmake_root(mem_root,str,size); }
  inline void *memdup(const void *str, size_t size)
  { return memdup_root(mem_root,str,size); }
  inline void *memdup_w_gap(const void *str, size_t size, size_t gap)
  {
    void *ptr;
    if (likely((ptr= alloc_root(mem_root,size+gap))))
      memcpy(ptr,str,size);
    return ptr;
  }

  void set_query_arena(Query_arena *set);

  void free_items();
  
  virtual void cleanup_stmt();
};


class Query_arena_memroot: public Query_arena, public Sql_alloc
{
public:
  Query_arena_memroot(MEM_ROOT *mem_root_arg, enum enum_state state_arg) :
    Query_arena(mem_root_arg, state_arg)
  {}
  Query_arena_memroot() : Query_arena()
  {}

  virtual ~Query_arena_memroot() {}
};


class Query_arena_stmt
{
  THD *thd;
  Query_arena backup;
  Query_arena *arena;

public:
  Query_arena_stmt(THD *_thd);
  ~Query_arena_stmt();
  bool arena_replaced()
  {
    return arena != NULL;
  }
};


class Server_side_cursor;



class Statement: public ilink, public Query_arena
{
  Statement(const Statement &rhs);              
  Statement &operator=(const Statement &rhs);   
public:
  
   ulong id;

  enum enum_column_usage column_usage;

  LEX_CSTRING name; 
  LEX *lex;                                     
  
  CSET_STRING query_string;
  
  String base_query;


  inline char *query() const { return query_string.str(); }
  inline uint32 query_length() const
  {
    return static_cast<uint32>(query_string.length());
  }
  inline char *query_end() const
  {
    return query_string.str() + query_string.length();
  }
  CHARSET_INFO *query_charset() const { return query_string.charset(); }
  void set_query_inner(const CSET_STRING &string_arg)
  {
    query_string= string_arg;
  }
  void set_query_inner(char *query_arg, uint32 query_length_arg,
                       CHARSET_INFO *cs_arg)
  {
    set_query_inner(CSET_STRING(query_arg, query_length_arg, cs_arg));
  }
  void reset_query_inner()
  {
    set_query_inner(CSET_STRING());
  }
  

  LEX_CSTRING db;

  
  my_bool query_cache_is_applicable;

  
  Statement() {}

  Statement(LEX *lex_arg, MEM_ROOT *mem_root_arg,
            enum enum_state state_arg, ulong id_arg);
  virtual ~Statement();

  
  virtual void set_statement(Statement *stmt);
  void set_n_backup_statement(Statement *stmt, Statement *backup);
  void restore_backup_statement(Statement *stmt, Statement *backup);
  
  virtual Type type() const;
};




class Statement_map
{
public:
  Statement_map();

  int insert(THD *thd, Statement *statement);

  Statement *find_by_name(LEX_CSTRING *name)
  {
    Statement *stmt;
    stmt= (Statement*)my_hash_search(&names_hash, (uchar*)name->str,
                                     name->length);
    return stmt;
  }

  Statement *find(ulong id)
  {
    if (last_found_statement == 0 || id != last_found_statement->id)
    {
      Statement *stmt;
      stmt= (Statement *) my_hash_search(&st_hash, (uchar *) &id, sizeof(id));
      if (stmt && stmt->name.str)
        return NULL;
      last_found_statement= stmt;
    }
    return last_found_statement;
  }
  
  void close_transient_cursors();
  void erase(Statement *statement);
  
  void reset();
  ~Statement_map();
private:
  HASH st_hash;
  HASH names_hash;
  I_List<Statement> transient_cursor_list;
  Statement *last_found_statement;
};

struct st_savepoint {
  struct st_savepoint *prev;
  char                *name;
  uint                 length;
  Ha_trx_info         *ha_list;
  
  MDL_savepoint        mdl_savepoint;
};

enum xa_states {XA_NOTR=0, XA_ACTIVE, XA_IDLE, XA_PREPARED, XA_ROLLBACK_ONLY};
extern const char *xa_state_names[];
class XID_cache_element;

typedef struct st_xid_state {
  
  XID  xid;                           
  enum xa_states xa_state;            
  
  uint rm_error;
  XID_cache_element *xid_cache_element;

  

  bool check_has_uncommitted_xa() const
  {
    if (xa_state == XA_IDLE ||
        xa_state == XA_PREPARED ||
        xa_state == XA_ROLLBACK_ONLY)
    {
      my_error(ER_XAER_RMFAIL, MYF(0), xa_state_names[xa_state]);
      return true;
    }
    return false;
  }
} XID_STATE;

void xid_cache_init(void);
void xid_cache_free(void);
XID_STATE *xid_cache_search(THD *thd, XID *xid);
bool xid_cache_insert(XID *xid, enum xa_states xa_state);
bool xid_cache_insert(THD *thd, XID_STATE *xid_state);
void xid_cache_delete(THD *thd, XID_STATE *xid_state);
int xid_cache_iterate(THD *thd, my_hash_walk_action action, void *argument);



class Security_context {
public:
  Security_context() {}                       
  
  const char *host;
  const char *user, *ip;
  char   priv_user[USERNAME_LENGTH];
  char   proxy_user[USERNAME_LENGTH + MAX_HOSTNAME + 5];
  
  char   priv_host[MAX_HOSTNAME];
  
  char   priv_role[USERNAME_LENGTH];
  
  char   *external_user;
  
  const char *host_or_ip;
  ulong master_access;                 
  ulong db_access;                     

  void init();
  void destroy();
  void skip_grants();
  inline char *priv_host_name()
  {
    return (*priv_host ? priv_host : (char *)"%");
  }

  bool set_user(char *user_arg);

#ifndef NO_EMBEDDED_ACCESS_CHECKS
  bool
  change_security_context(THD *thd,
                          LEX_CSTRING *definer_user,
                          LEX_CSTRING *definer_host,
                          LEX_CSTRING *db,
                          Security_context **backup);

  void
  restore_security_context(THD *thd, Security_context *backup);
#endif
  bool user_matches(Security_context *);
};




struct Item_change_record;
class Item_change_list
{
  I_List<Item_change_record> change_list;
public:
  void nocheck_register_item_tree_change(Item **place, Item *old_value,
                                         MEM_ROOT *runtime_memroot);
  void check_and_register_item_tree_change(Item **place, Item **new_value,
                                           MEM_ROOT *runtime_memroot);
  void rollback_item_tree_changes();
  void move_elements_to(Item_change_list *to)
  {
    change_list.move_elements_to(&to->change_list);
  }
  bool is_empty() { return change_list.is_empty(); }
};


class Item_change_list_savepoint: public Item_change_list
{
public:
  Item_change_list_savepoint(Item_change_list *list)
  {
    list->move_elements_to(this);
  }
  void rollback(Item_change_list *list)
  {
    list->rollback_item_tree_changes();
    move_elements_to(list);
  }
  ~Item_change_list_savepoint()
  {
    DBUG_ASSERT(is_empty());
  }
};




enum enum_locked_tables_mode
{
  LTM_NONE= 0,
  LTM_LOCK_TABLES,
  LTM_PRELOCKED,
  LTM_PRELOCKED_UNDER_LOCK_TABLES,
  LTM_always_last
};


struct TMP_TABLE_SHARE : public TABLE_SHARE
{
private:
  
  TMP_TABLE_SHARE *tmp_next;
  TMP_TABLE_SHARE **tmp_prev;

  friend struct All_tmp_table_shares;

public:
  
  All_share_tables_list all_tmp_tables;
};



struct All_tmp_table_shares
{
  static inline TMP_TABLE_SHARE **next_ptr(TMP_TABLE_SHARE *l)
  {
    return &l->tmp_next;
  }
  static inline TMP_TABLE_SHARE ***prev_ptr(TMP_TABLE_SHARE *l)
  {
    return &l->tmp_prev;
  }
};


typedef I_P_List <TMP_TABLE_SHARE, All_tmp_table_shares> All_tmp_tables_list;



class Open_tables_state
{
public:
  
  Reprepare_observer *m_reprepare_observer;

  
  TABLE *open_tables;

  
  All_tmp_tables_list *temporary_tables;

  
  TABLE *derived_tables;

  
  TABLE *rec_tables;

  
  MYSQL_LOCK *lock;

  
  MYSQL_LOCK *extra_lock;

  
  enum enum_locked_tables_mode locked_tables_mode;
  uint current_tablenr;

  enum enum_flags {
    BACKUPS_AVAIL = (1U << 0)     
  };

  
  uint state_flags;
  
  Open_tables_state() : state_flags(0U) { }

  void set_open_tables_state(Open_tables_state *state)
  {
    *this= *state;
  }

  void reset_open_tables_state(THD *thd)
  {
    open_tables= 0;
    temporary_tables= 0;
    derived_tables= 0;
    rec_tables= 0;
    extra_lock= 0;
    lock= 0;
    locked_tables_mode= LTM_NONE;
    state_flags= 0U;
    m_reprepare_observer= NULL;
  }
};




class Open_tables_backup: public Open_tables_state
{
public:
  
  MDL_savepoint mdl_system_tables_svp;
};





#define SUB_STMT_TRIGGER 1
#define SUB_STMT_FUNCTION 2


class Sub_statement_state
{
public:
  Discrete_interval auto_inc_interval_for_cur_row;
  Discrete_intervals_list auto_inc_intervals_forced;
  SAVEPOINT *savepoints;
  ulonglong option_bits;
  ulonglong first_successful_insert_id_in_prev_stmt;
  ulonglong first_successful_insert_id_in_cur_stmt, insert_id_for_cur_row;
  ulonglong limit_found_rows;
  ulonglong tmp_tables_size;
  ulonglong client_capabilities;
  ulonglong cuted_fields, sent_row_count, examined_row_count;
  ulonglong affected_rows;
  ulonglong bytes_sent_old;
  ulong     tmp_tables_used;
  ulong     tmp_tables_disk_used;
  ulong     query_plan_fsort_passes;
  ulong query_plan_flags; 
  uint in_sub_stmt;    
  bool enable_slow_log;
  bool last_insert_id_used;
  enum enum_check_fields count_cuted_fields;
};



enum enum_thread_type
{
  NON_SYSTEM_THREAD= 0,
  SYSTEM_THREAD_DELAYED_INSERT= 1,
  SYSTEM_THREAD_SLAVE_IO= 2,
  SYSTEM_THREAD_SLAVE_SQL= 4,
  SYSTEM_THREAD_EVENT_SCHEDULER= 8,
  SYSTEM_THREAD_EVENT_WORKER= 16,
  SYSTEM_THREAD_BINLOG_BACKGROUND= 32,
  SYSTEM_THREAD_SLAVE_BACKGROUND= 64,
  SYSTEM_THREAD_GENERIC= 128,
  SYSTEM_THREAD_SEMISYNC_MASTER_BACKGROUND= 256
};

inline char const *
show_system_thread(enum_thread_type thread)
{
#define RETURN_NAME_AS_STRING(NAME) case (NAME): return #NAME
  switch (thread) {
    static char buf[64];
    RETURN_NAME_AS_STRING(NON_SYSTEM_THREAD);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_DELAYED_INSERT);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_SLAVE_IO);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_SLAVE_SQL);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_EVENT_SCHEDULER);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_EVENT_WORKER);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_SLAVE_BACKGROUND);
    RETURN_NAME_AS_STRING(SYSTEM_THREAD_SEMISYNC_MASTER_BACKGROUND);
  default:
    sprintf(buf, "<UNKNOWN SYSTEM THREAD: %d>", thread);
    return buf;
  }
#undef RETURN_NAME_AS_STRING
}



class Internal_error_handler
{
protected:
  Internal_error_handler() :
    m_prev_internal_handler(NULL)
  {}

  virtual ~Internal_error_handler() {}

public:
  
  virtual bool handle_condition(THD *thd,
                                uint sql_errno,
                                const char* sqlstate,
                                Sql_condition::enum_warning_level *level,
                                const char* msg,
                                Sql_condition ** cond_hdl) = 0;

private:
  Internal_error_handler *m_prev_internal_handler;
  friend class THD;
};




class Dummy_error_handler : public Internal_error_handler
{
public:
  bool handle_condition(THD *thd,
                        uint sql_errno,
                        const char* sqlstate,
                        Sql_condition::enum_warning_level *level,
                        const char* msg,
                        Sql_condition ** cond_hdl)
  {
    
    return TRUE;
  }
  Dummy_error_handler() {}                    
};




class Counting_error_handler : public Internal_error_handler
{
public:
  int errors;
  bool handle_condition(THD *thd,
                        uint sql_errno,
                        const char* sqlstate,
                        Sql_condition::enum_warning_level *level,
                        const char* msg,
                        Sql_condition ** cond_hdl)
  {
    if (*level == Sql_condition::WARN_LEVEL_ERROR)
      errors++;
    return false;
  }
  Counting_error_handler() : errors(0) {}
};




class Drop_table_error_handler : public Internal_error_handler
{
public:
  Drop_table_error_handler() {}

public:
  bool handle_condition(THD *thd,
                        uint sql_errno,
                        const char* sqlstate,
                        Sql_condition::enum_warning_level *level,
                        const char* msg,
                        Sql_condition ** cond_hdl);

private:
};




class MDL_deadlock_and_lock_abort_error_handler: public Internal_error_handler
{
public:
  virtual
  bool handle_condition(THD *thd,
                        uint sql_errno,
                        const char *sqlstate,
                        Sql_condition::enum_warning_level *level,
                        const char* msg,
                        Sql_condition **cond_hdl);

  bool need_reopen() const { return m_need_reopen; };
  void init() { m_need_reopen= FALSE; };
private:
  bool m_need_reopen;
};




class Locked_tables_list
{
public:
  MEM_ROOT m_locked_tables_root;
private:
  TABLE_LIST *m_locked_tables;
  TABLE_LIST **m_locked_tables_last;
  
  TABLE **m_reopen_array;
  
  size_t m_locked_tables_count;
public:
  Locked_tables_list()
    :m_locked_tables(NULL),
    m_locked_tables_last(&m_locked_tables),
    m_reopen_array(NULL),
    m_locked_tables_count(0)
  {
    init_sql_alloc(&m_locked_tables_root, "Locked_tables_list",
                   MEM_ROOT_BLOCK_SIZE, 0,
                   MYF(MY_THREAD_SPECIFIC));
  }
  void unlock_locked_tables(THD *thd);
  void unlock_locked_table(THD *thd, MDL_ticket *mdl_ticket);
  ~Locked_tables_list()
  {
    reset();
  }
  void reset();
  bool init_locked_tables(THD *thd);
  TABLE_LIST *locked_tables() { return m_locked_tables; }
  void unlink_from_list(THD *thd, TABLE_LIST *table_list,
                        bool remove_from_locked_tables);
  void unlink_all_closed_tables(THD *thd,
                                MYSQL_LOCK *lock,
                                size_t reopen_count);
  bool reopen_tables(THD *thd, bool need_reopen);
  bool restore_lock(THD *thd, TABLE_LIST *dst_table_list, TABLE *table,
                    MYSQL_LOCK *lock);
  void add_back_last_deleted_lock(TABLE_LIST *dst_table_list);
};




struct Ha_data
{
  
  void *ha_ptr;
  
  Ha_trx_info ha_info[2];
  
  plugin_ref lock;
  Ha_data() :ha_ptr(NULL) {}
};



class Global_read_lock
{
public:
  enum enum_grl_state
  {
    GRL_NONE,
    GRL_ACQUIRED,
    GRL_ACQUIRED_AND_BLOCKS_COMMIT
  };

  Global_read_lock()
    : m_state(GRL_NONE),
      m_mdl_global_shared_lock(NULL),
      m_mdl_blocks_commits_lock(NULL)
  {}

  bool lock_global_read_lock(THD *thd);
  void unlock_global_read_lock(THD *thd);
  
  bool can_acquire_protection() const
  {
    if (m_state)
    {
      my_error(ER_CANT_UPDATE_WITH_READLOCK, MYF(0));
      return TRUE;
    }
    return FALSE;
  }
  bool make_global_read_lock_block_commit(THD *thd);
  bool is_acquired() const { return m_state != GRL_NONE; }
  void set_explicit_lock_duration(THD *thd);
private:
  enum_grl_state m_state;
  
  MDL_ticket *m_mdl_global_shared_lock;
  
  MDL_ticket *m_mdl_blocks_commits_lock;
};



struct wait_for_commit
{
  
  mysql_mutex_t LOCK_wait_commit;
  mysql_cond_t COND_wait_commit;
  
  wait_for_commit *subsequent_commits_list;
  
  wait_for_commit *next_subsequent_commit;
  
  wait_for_commit *waitee;
  
  void *opaque_pointer;
  
  int wakeup_error;
  
  bool wakeup_subsequent_commits_running;
  
  bool commit_started;

  void register_wait_for_prior_commit(wait_for_commit *waitee);
  int wait_for_prior_commit(THD *thd)
  {
    
    if (waitee)
      return wait_for_prior_commit2(thd);
    else
    {
      if (wakeup_error)
        my_error(ER_PRIOR_COMMIT_FAILED, MYF(0));
      return wakeup_error;
    }
  }
  void wakeup_subsequent_commits(int wakeup_error_arg)
  {
    
    if (subsequent_commits_list)
      wakeup_subsequent_commits2(wakeup_error_arg);
  }
  void unregister_wait_for_prior_commit()
  {
    if (waitee)
      unregister_wait_for_prior_commit2();
    else
      wakeup_error= 0;
  }
  
  void remove_from_list(wait_for_commit **next_ptr_ptr)
  {
    wait_for_commit *cur;

    while ((cur= *next_ptr_ptr) != NULL)
    {
      if (cur == this)
      {
        *next_ptr_ptr= this->next_subsequent_commit;
        break;
      }
      next_ptr_ptr= &cur->next_subsequent_commit;
    }
    waitee= NULL;
  }

  void wakeup(int wakeup_error);

  int wait_for_prior_commit2(THD *thd);
  void wakeup_subsequent_commits2(int wakeup_error);
  void unregister_wait_for_prior_commit2();

  wait_for_commit();
  ~wait_for_commit();
  void reinit();
};

extern "C" void my_message_sql(uint error, const char *str, myf MyFlags);



class THD :public Statement,
           
           public Item_change_list,
           public MDL_context_owner,
           public Open_tables_state
{
private:
  inline bool is_stmt_prepare() const
  { DBUG_ASSERT(0); return Statement::is_stmt_prepare(); }

  inline bool is_stmt_prepare_or_first_sp_execute() const
  { DBUG_ASSERT(0); return Statement::is_stmt_prepare_or_first_sp_execute(); }

  inline bool is_stmt_prepare_or_first_stmt_execute() const
  { DBUG_ASSERT(0); return Statement::is_stmt_prepare_or_first_stmt_execute(); }

  inline bool is_conventional() const
  { DBUG_ASSERT(0); return Statement::is_conventional(); }

  void dec_thread_count(void)
  {
    DBUG_ASSERT(thread_count > 0);
    thread_safe_decrement32(&thread_count);
    signal_thd_deleted();
  }


  void inc_thread_count(void)
  {
    thread_safe_increment32(&thread_count);
  }

public:
  MDL_context mdl_context;

  
  Relay_log_info* rli_fake;
  rpl_group_info* rgi_fake;
  
  rpl_group_info* rgi_slave;

  union {
    rpl_io_thread_info *rpl_io_info;
    rpl_sql_thread_info *rpl_sql_info;
  } system_thread_info;

  void reset_for_next_command(bool do_clear_errors= 1);
  
  static const char * const DEFAULT_WHERE;

#ifdef EMBEDDED_LIBRARY
  struct st_mysql  *mysql;
  unsigned long	 client_stmt_id;
  unsigned long  client_param_count;
  struct st_mysql_bind *client_params;
  char *extra_data;
  ulong extra_length;
  struct st_mysql_data *cur_data;
  struct st_mysql_data *first_data;
  struct st_mysql_data **data_tail;
  void clear_data_list();
  struct st_mysql_data *alloc_new_dataset();
  
  struct st_mysql_stmt *current_stmt;
#endif
#ifdef HAVE_QUERY_CACHE
  Query_cache_tls query_cache_tls;
#endif
  NET	  net;				
  
  NET_SERVER m_net_server_extension;
  scheduler_functions *scheduler;       
  Protocol *protocol;			
  Protocol_text   protocol_text;	
  Protocol_binary protocol_binary;	
  HASH    user_vars;			
  String  packet;			
  String  convert_buffer;               
  struct  my_rnd_struct rand;		
  struct  system_variables variables;	
  struct  system_status_var status_var; 
  struct  system_status_var org_status_var; 
  struct  system_status_var *initial_status_var; 
  THR_LOCK_INFO lock_info;              
  
  mysql_mutex_t LOCK_thd_data;
  
  mysql_mutex_t LOCK_thd_kill;

  
  Statement_map stmt_map;

  
  Statement *last_stmt;
  inline void set_last_stmt(Statement *stmt)
  { last_stmt= (is_error() ? NULL : stmt); }
  inline void clear_last_stmt() { last_stmt= NULL; }

  
  char	  *thread_stack;

  
  char *catalog;

  

  Security_context main_security_ctx;
  Security_context *security_ctx;

  
  const char *proc_info;

private:
  unsigned int m_current_stage_key;

public:
  void enter_stage(const PSI_stage_info *stage,
                   const char *calling_func,
                   const char *calling_file,
                   const unsigned int calling_line)
  {
    DBUG_PRINT("THD::enter_stage", ("%s at %s:%d", stage->m_name,
                                    calling_file, calling_line));
    DBUG_ASSERT(stage);
    m_current_stage_key= stage->m_key;
    proc_info= stage->m_name;
#if defined(ENABLED_PROFILING)
    profiling.status_change(proc_info, calling_func, calling_file,
                            calling_line);
#endif
#ifdef HAVE_PSI_THREAD_INTERFACE
    MYSQL_SET_STAGE(m_current_stage_key, calling_file, calling_line);
#endif
  }

  void backup_stage(PSI_stage_info *stage)
  {
    stage->m_key= m_current_stage_key;
    stage->m_name= proc_info;
  }

  const char *get_proc_info() const
  { return proc_info; }

  
  const char *where;

  
  Trans_binlog_info *semisync_info;
  
  bool semi_sync_slave;
  ulonglong client_capabilities;  
  ulong max_client_packet_length;

  HASH		handler_tables_hash;
  
  HASH ull_hash;
  
  HASH sequences;
#ifdef DBUG_ASSERT_EXISTS
  uint dbug_sentry; 
#endif
  struct st_my_thread_var *mysys_var;

  
  CHARSET_INFO *org_charset;
private:
  
  enum enum_server_command m_command;

public:
  uint32     file_id;			
  
  uint16     peer_port;
  my_time_t  start_time;             
  ulong      start_time_sec_part;    
  my_hrtime_t user_time;
  
  ulonglong  prior_thr_create_utime, thr_create_utime;
  ulonglong  start_utime, utime_after_lock, utime_after_query;

  
  struct {
    
    bool       report_to_client;
    
    bool       report;
    uint       stage, max_stage;
    ulonglong  counter, max_counter;
    ulonglong  next_report_time;
    Query_arena *arena;
  } progress;

  thr_lock_type update_lock_default;
  Delayed_insert *di;

  
  uint in_sub_stmt;
  
  bool userstat_running;
  
  bool log_all_errors;

  
  bool skip_wait_timeout;

  bool prepare_derived_at_open;

  
  bool status_in_global;

   
  bool create_tmp_table_for_derived;

  bool save_prep_leaf_list;

  
  Ha_data ha_data[MAX_HA];

#ifndef MYSQL_CLIENT
  binlog_cache_mngr *  binlog_setup_trx_data();

  
  void binlog_start_trans_and_stmt();
  void binlog_set_stmt_begin();
  int binlog_write_table_map(TABLE *table, bool is_transactional,
                             my_bool *with_annotate= 0);
  int binlog_write_row(TABLE* table, bool is_transactional,
                       const uchar *buf);
  int binlog_delete_row(TABLE* table, bool is_transactional,
                        const uchar *buf);
  int binlog_update_row(TABLE* table, bool is_transactional,
                        const uchar *old_data, const uchar *new_data);
  static void binlog_prepare_row_images(TABLE* table);

  void set_server_id(uint32 sid) { variables.server_id = sid; }

  
  template <class RowsEventT> Rows_log_event*
    binlog_prepare_pending_rows_event(TABLE* table, uint32 serv_id,
                                      size_t needed,
                                      bool is_transactional,
                                      RowsEventT* hint);
  Rows_log_event* binlog_get_pending_rows_event(bool is_transactional) const;
  void binlog_set_pending_rows_event(Rows_log_event* ev, bool is_transactional);
  inline int binlog_flush_pending_rows_event(bool stmt_end)
  {
    return (binlog_flush_pending_rows_event(stmt_end, FALSE) || 
            binlog_flush_pending_rows_event(stmt_end, TRUE));
  }
  int binlog_flush_pending_rows_event(bool stmt_end, bool is_transactional);
  int binlog_remove_pending_rows_event(bool clear_maps, bool is_transactional);

  
  int is_current_stmt_binlog_format_row() const {
    DBUG_ASSERT(current_stmt_binlog_format == BINLOG_FORMAT_STMT ||
                current_stmt_binlog_format == BINLOG_FORMAT_ROW);
    return current_stmt_binlog_format == BINLOG_FORMAT_ROW;
  }
  
  inline bool is_current_stmt_binlog_disabled() const
  {
    return (!(variables.option_bits & OPTION_BIN_LOG) ||
            !mysql_bin_log.is_open());
  }

  enum binlog_filter_state
  {
    BINLOG_FILTER_UNKNOWN,
    BINLOG_FILTER_CLEAR,
    BINLOG_FILTER_SET
  };

  inline void reset_binlog_local_stmt_filter()
  {
    m_binlog_filter_state= BINLOG_FILTER_UNKNOWN;
  }

  inline void clear_binlog_local_stmt_filter()
  {
    DBUG_ASSERT(m_binlog_filter_state == BINLOG_FILTER_UNKNOWN);
    m_binlog_filter_state= BINLOG_FILTER_CLEAR;
  }

  inline void set_binlog_local_stmt_filter()
  {
    DBUG_ASSERT(m_binlog_filter_state == BINLOG_FILTER_UNKNOWN);
    m_binlog_filter_state= BINLOG_FILTER_SET;
  }

  inline binlog_filter_state get_binlog_local_stmt_filter()
  {
    return m_binlog_filter_state;
  }

private:
  
  binlog_filter_state m_binlog_filter_state;

  
  enum_binlog_format current_stmt_binlog_format;

  
  uint32 binlog_unsafe_warning_flags;

  
  uint binlog_table_maps;
public:
  void issue_unsafe_warnings();
  void reset_unsafe_warnings()
  { binlog_unsafe_warning_flags= 0; }

  uint get_binlog_table_maps() const {
    return binlog_table_maps;
  }
  void clear_binlog_table_maps() {
    binlog_table_maps= 0;
  }
#endif 

public:

  struct st_transactions {
    SAVEPOINT *savepoints;
    THD_TRANS all;			
    THD_TRANS stmt;			
    bool on;                            
    XID_STATE xid_state;
    WT_THD wt;                          
    Rows_log_event *m_pending_rows_event;

    struct st_trans_time : public timeval
    {
      void reset(THD *thd)
      {
        tv_sec= thd->query_start();
        tv_usec= (long) thd->query_start_sec_part();
      }
    } start_time;

    
    CHANGED_TABLE_LIST* changed_tables;
    MEM_ROOT mem_root; 
    void cleanup()
    {
      DBUG_ENTER("thd::cleanup");
      changed_tables= 0;
      savepoints= 0;
      
      if (!xid_state.rm_error)
        xid_state.xid.null();
      free_root(&mem_root,MYF(MY_KEEP_PREALLOC));
      DBUG_VOID_RETURN;
    }
    my_bool is_active()
    {
      return (all.ha_list != NULL);
    }
    st_transactions()
    {
      bzero((char*)this, sizeof(*this));
      xid_state.xid.null();
      init_sql_alloc(&mem_root, "THD::transactions",
                     ALLOC_ROOT_MIN_BLOCK_SIZE, 0,
                     MYF(MY_THREAD_SPECIFIC));
    }
  } transaction;
  Global_read_lock global_read_lock;
  Field      *dup_field;
#ifndef __WIN__
  sigset_t signals;
#endif
#ifdef SIGNAL_WITH_VIO_CLOSE
  Vio* active_vio;
#endif

  
  Query_arena *stmt_arena;

  void *bulk_param;

  
  table_map table_map_for_update;

  
  bool arg_of_last_insert_id_function;
  
  
  ulonglong  first_successful_insert_id_in_prev_stmt;
  
  ulonglong  first_successful_insert_id_in_prev_stmt_for_binlog;
  
  ulonglong  first_successful_insert_id_in_cur_stmt;
  
  
  bool       stmt_depends_on_first_successful_insert_id_in_prev_stmt;
  
  Discrete_intervals_list auto_inc_intervals_in_cur_stmt_for_binlog;
  
  Discrete_intervals_list auto_inc_intervals_forced;
  
  inline void record_first_successful_insert_id_in_cur_stmt(ulonglong id_arg)
  {
    if (first_successful_insert_id_in_cur_stmt == 0)
      first_successful_insert_id_in_cur_stmt= id_arg;
  }
  inline ulonglong read_first_successful_insert_id_in_prev_stmt(void)
  {
    if (!stmt_depends_on_first_successful_insert_id_in_prev_stmt)
    {
      
      first_successful_insert_id_in_prev_stmt_for_binlog=
        first_successful_insert_id_in_prev_stmt;
      stmt_depends_on_first_successful_insert_id_in_prev_stmt= 1;
    }
    return first_successful_insert_id_in_prev_stmt;
  }
  
  inline void force_one_auto_inc_interval(ulonglong next_id)
  {
    auto_inc_intervals_forced.empty(); 
    auto_inc_intervals_forced.append(next_id, ULONGLONG_MAX, 0);
  }

  ulonglong  limit_found_rows;

private:
  

  longlong m_row_count_func;    

public:
  inline longlong get_row_count_func() const
  {
    return m_row_count_func;
  }

  inline void set_row_count_func(longlong row_count_func)
  {
    m_row_count_func= row_count_func;
  }
  inline void set_affected_rows(longlong row_count_func)
  {
    
    affected_rows+= (row_count_func >= 0 ? row_count_func : 0);
  }

  ha_rows    cuted_fields;

private:
  
  ha_rows    m_sent_row_count;

  
  ha_rows    m_examined_row_count;

public:
  ha_rows get_sent_row_count() const
  { return m_sent_row_count; }

  ha_rows get_examined_row_count() const
  { return m_examined_row_count; }

  ulonglong get_affected_rows() const
  { return affected_rows; }

  void set_sent_row_count(ha_rows count);
  void set_examined_row_count(ha_rows count);

  void inc_sent_row_count(ha_rows count);
  void inc_examined_row_count(ha_rows count);

  void inc_status_created_tmp_disk_tables();
  void inc_status_created_tmp_files();
  void inc_status_created_tmp_tables();
  void inc_status_select_full_join();
  void inc_status_select_full_range_join();
  void inc_status_select_range();
  void inc_status_select_range_check();
  void inc_status_select_scan();
  void inc_status_sort_merge_passes();
  void inc_status_sort_range();
  void inc_status_sort_rows(ha_rows count);
  void inc_status_sort_scan();
  void set_status_no_index_used();
  void set_status_no_good_index_used();

  
  ulonglong accessed_rows_and_keys;

  
  void check_limit_rows_examined()
  {
    if (++accessed_rows_and_keys > lex->limit_rows_examined_cnt)
      set_killed(ABORT_QUERY);
  }

  USER_CONN *user_connect;
  CHARSET_INFO *db_charset;
#if defined(ENABLED_PROFILING)
  PROFILING  profiling;
#endif

  
  sql_digest_state *m_digest;
  
  unsigned char *m_token_array;
  
  sql_digest_state m_digest_state;

  
  PSI_statement_locker *m_statement_psi;
#ifdef HAVE_PSI_STATEMENT_INTERFACE
  
  PSI_statement_locker_state m_statement_state;
#endif 
  
  PSI_idle_locker *m_idle_psi;
#ifdef HAVE_PSI_IDLE_INTERFACE
  
  PSI_idle_locker_state m_idle_state;
#endif 

  
  query_id_t query_id;
  ulong      col_access;

  
  ulong      statement_id_counter;
  ulong	     rand_saved_seed1, rand_saved_seed2;

  
  ulong      query_plan_flags; 
  ulong      query_plan_fsort_passes; 
  ulong      tmp_tables_used;
  ulong      tmp_tables_disk_used;
  ulonglong  tmp_tables_size;
  ulonglong  bytes_sent_old;
  ulonglong  affected_rows;                     

  pthread_t  real_id;                           
  my_thread_id  thread_id, thread_dbug_id;
  uint32      os_thread_id;
  uint	     tmp_table, global_disable_checkpoint;
  uint	     server_status,open_options;
  enum enum_thread_type system_thread;
  
  enum_tx_isolation tx_isolation;
  
  bool              tx_read_only;
  enum_check_fields count_cuted_fields;

  DYNAMIC_ARRAY user_var_events;        
  MEM_ROOT      *user_var_events_alloc; 

  
  enum durability_properties durability_property;
 
  
  killed_state volatile killed;

  
  struct err_info
  {
    int no;
    const char msg[256];
  } *killed_err;

  
  inline bool check_killed(bool dont_send_error_message= 0)
  {
    if (unlikely(killed))
    {
      if (!dont_send_error_message)
        send_kill_message();
      return TRUE;
    }
    if (apc_target.have_apc_requests())
      apc_target.process_apc_requests(); 
    return FALSE;
  }

  
  char	     scramble[SCRAMBLE_LENGTH+1];

  
  LEX_CSTRING connection_name;
  char       default_master_connection_buff[MAX_CONNECTION_NAME+1];
  uint8      password; 
  uint8      failed_com_change_user;
  bool       slave_thread;
  bool       extra_port;                        
  bool	     no_errors;

  
  bool is_fatal_error;
  
  bool       transaction_rollback_request;
  
  bool       is_fatal_sub_stmt_error;
  bool	     rand_used, time_zone_used;
  bool       query_start_sec_part_used;
  
  bool       substitute_null_with_insert_id;
  bool	     in_lock_tables;
  bool       bootstrap, cleanup_done, free_connection_done;

  
  bool       thread_specific_used;
  
private:
  bool       charset_is_system_charset, charset_is_collation_connection;
  bool       charset_is_character_set_filesystem;
public:
  bool       enable_slow_log;    
  bool	     abort_on_warning;
  bool 	     got_warning;       
  
  bool       derived_tables_processing;
  bool       tablespace_op;	
  
  bool	     log_current_statement;
  
  bool       is_slave_error;
  
  bool waiting_on_group_commit;
  
  bool has_waiter;
  
  int	     slave_expected_error;
  enum_sql_command last_sql_command;  

  sp_rcontext *spcont;		
  sp_cache   *sp_proc_cache;
  sp_cache   *sp_func_cache;
  sp_cache   *sp_package_spec_cache;
  sp_cache   *sp_package_body_cache;

  
  uint       query_name_consts;

  
  LOG_INFO*  current_linfo;
  NET*       slave_net;			

  
  
  time_t current_connect_time;
  
  time_t last_global_update_time;
  
  uint select_commands, update_commands, other_commands;
  ulonglong start_cpu_time;
  ulonglong start_bytes_received;

  
  union
  {
    my_bool   my_bool_value;
    int       int_value;
    uint      uint_value;
    long      long_value;
    ulong     ulong_value;
    ulonglong ulonglong_value;
    double    double_value;
    void      *ptr_value;
  } sys_var_tmp;

  struct {
    
    bool do_union;
    
    bool unioned_events;
    
    bool unioned_events_trans;

    
    query_id_t first_query_id;
  } binlog_evt_union;

  mysql_cond_t              COND_wsrep_thd;
  
  Parser_state *m_parser_state;

  Locked_tables_list locked_tables_list;

#ifdef WITH_PARTITION_STORAGE_ENGINE
  partition_info *work_part_info;
#endif

#ifndef EMBEDDED_LIBRARY
  
  DYNAMIC_ARRAY audit_class_plugins;
  
  unsigned long audit_class_mask[MYSQL_AUDIT_CLASS_MASK_SIZE];
#endif

#if defined(ENABLED_DEBUG_SYNC)
  
  struct st_debug_sync_control *debug_sync_control;
#endif 
  
  THD(my_thread_id id, bool is_wsrep_applier= false, bool skip_lock= false);

  ~THD();
  
  void init(bool skip_lock= false);
  
  void init_for_queries();
  void update_all_stats();
  void update_stats(void);
  void change_user(void);
  void cleanup(void);
  void cleanup_after_query();
  void free_connection();
  void reset_for_reuse();
  bool store_globals();
  void reset_globals();
#ifdef SIGNAL_WITH_VIO_CLOSE
  inline void set_active_vio(Vio* vio)
  {
    mysql_mutex_lock(&LOCK_thd_data);
    active_vio = vio;
    mysql_mutex_unlock(&LOCK_thd_data);
  }
  inline void clear_active_vio()
  {
    mysql_mutex_lock(&LOCK_thd_data);
    active_vio = 0;
    mysql_mutex_unlock(&LOCK_thd_data);
  }
  void close_active_vio();
#endif
  void awake_no_mutex(killed_state state_to_set);
  void awake(killed_state state_to_set)
  {
    mysql_mutex_lock(&LOCK_thd_kill);
    awake_no_mutex(state_to_set);
    mysql_mutex_unlock(&LOCK_thd_kill);
  }
 
  
  void disconnect();


  
  Apc_target apc_target;

#ifndef MYSQL_CLIENT
  enum enum_binlog_query_type {
    
    ROW_QUERY_TYPE,
    
    
    STMT_QUERY_TYPE,
    
    QUERY_TYPE_COUNT
  };

  int binlog_query(enum_binlog_query_type qtype,
                   char const *query, ulong query_len, bool is_trans,
                   bool direct, bool suppress_use,
                   int errcode);
#endif

  inline void
  enter_cond(mysql_cond_t *cond, mysql_mutex_t* mutex,
             const PSI_stage_info *stage, PSI_stage_info *old_stage,
             const char *src_function, const char *src_file,
             int src_line)
  {
    mysql_mutex_assert_owner(mutex);
    mysys_var->current_mutex = mutex;
    mysys_var->current_cond = cond;
    if (old_stage)
      backup_stage(old_stage);
    if (stage)
      enter_stage(stage, src_function, src_file, src_line);
  }
  inline void exit_cond(const PSI_stage_info *stage,
                        const char *src_function, const char *src_file,
                        int src_line)
  {
    
    mysql_mutex_unlock(mysys_var->current_mutex);
    mysql_mutex_lock(&mysys_var->mutex);
    mysys_var->current_mutex = 0;
    mysys_var->current_cond = 0;
    if (stage)
      enter_stage(stage, src_function, src_file, src_line);
    mysql_mutex_unlock(&mysys_var->mutex);
    return;
  }
  virtual int is_killed() { return killed; }
  virtual THD* get_thd() { return this; }

  
  virtual bool notify_shared_lock(MDL_context_owner *ctx_in_use,
                                  bool needs_thr_lock_abort);

  

  inline bool is_strict_mode() const
  {
    return (bool) (variables.sql_mode & (MODE_STRICT_TRANS_TABLES |
                                         MODE_STRICT_ALL_TABLES));
  }
  inline bool backslash_escapes() const
  {
    return !MY_TEST(variables.sql_mode & MODE_NO_BACKSLASH_ESCAPES);
  }
  const Type_handler *type_handler_for_date() const;
  bool timestamp_to_TIME(MYSQL_TIME *ltime, my_time_t ts,
                         ulong sec_part, ulonglong fuzzydate);
  inline my_time_t query_start() { return start_time; }
  inline ulong query_start_sec_part()
  { query_start_sec_part_used=1; return start_time_sec_part; }
  MYSQL_TIME query_start_TIME();

private:
  struct {
    my_hrtime_t start;
    my_time_t sec;
    ulong sec_part;
  } system_time;

  void set_system_time()
  {
    my_hrtime_t hrtime= my_hrtime();
    my_time_t sec= hrtime_to_my_time(hrtime);
    ulong sec_part= hrtime_sec_part(hrtime);
    if (sec > system_time.sec ||
        (sec == system_time.sec && sec_part > system_time.sec_part) ||
        hrtime.val < system_time.start.val)
    {
      system_time.sec= sec;
      system_time.sec_part= sec_part;
    }
    else
    {
      if (system_time.sec_part < TIME_MAX_SECOND_PART)
        system_time.sec_part++;
      else
      {
        system_time.sec++;
        system_time.sec_part= 0;
      }
    }
  }

public:
  timeval transaction_time()
  {
    if (!in_multi_stmt_transaction_mode())
      transaction.start_time.reset(this);
    return transaction.start_time;
  }

  inline void set_start_time()
  {
    if (user_time.val)
    {
      start_time= hrtime_to_my_time(user_time);
      start_time_sec_part= hrtime_sec_part(user_time);
    }
    else
    {
      set_system_time();
      start_time= system_time.sec;
      start_time_sec_part= system_time.sec_part;
    }
    PSI_CALL_set_thread_start_time(start_time);
  }
  inline void set_time()
  {
    set_start_time();
    start_utime= utime_after_lock= microsecond_interval_timer();
  }
  
  inline void set_time(my_hrtime_t t)
  {
    user_time= t;
    set_time();
  }
  
  inline void set_time(my_time_t t, ulong sec_part)
  {
    if (opt_secure_timestamp > (slave_thread ? SECTIME_REPL : SECTIME_SUPER))
      set_time();                 
    else
    {
      if (sec_part <= TIME_MAX_SECOND_PART)
      {
        start_time= system_time.sec= t;
        start_time_sec_part= system_time.sec_part= sec_part;
      }
      else if (t != system_time.sec)
      {
        start_time= system_time.sec= t;
        start_time_sec_part= system_time.sec_part= 0;
      }
      else
      {
        start_time= t;
        start_time_sec_part= ++system_time.sec_part;
      }
      user_time.val= hrtime_from_time(start_time) + start_time_sec_part;
      PSI_CALL_set_thread_start_time(start_time);
      start_utime= utime_after_lock= microsecond_interval_timer();
    }
  }
  void set_time_after_lock()
  {
    utime_after_lock= microsecond_interval_timer();
    MYSQL_SET_STATEMENT_LOCK_TIME(m_statement_psi,
                                  (utime_after_lock - start_utime));
  }
  ulonglong current_utime()  { return microsecond_interval_timer(); }

  
  inline void set_time_for_next_stage()
  {
    utime_after_query= current_utime();
  }

  
  void update_server_status()
  {
    set_time_for_next_stage();
    if (utime_after_query >= utime_after_lock + variables.long_query_time)
      server_status|= SERVER_QUERY_WAS_SLOW;
  }
  inline ulonglong found_rows(void)
  {
    return limit_found_rows;
  }
  
  inline bool in_multi_stmt_transaction_mode()
  {
    return variables.option_bits & (OPTION_NOT_AUTOCOMMIT | OPTION_BEGIN);
  }
  
  inline bool in_active_multi_stmt_transaction()
  {
    return server_status & SERVER_STATUS_IN_TRANS;
  }
  inline bool fill_derived_tables()
  {
    return !stmt_arena->is_stmt_prepare() && !lex->only_view_structure();
  }
  inline bool fill_information_schema_tables()
  {
    return !stmt_arena->is_stmt_prepare();
  }
  inline void* trans_alloc(size_t size)
  {
    return alloc_root(&transaction.mem_root,size);
  }

  LEX_STRING *make_lex_string(LEX_STRING *lex_str, const char* str, size_t length)
  {
    if (!(lex_str->str= strmake_root(mem_root, str, length)))
    {
      lex_str->length= 0;
      return 0;
    }
    lex_str->length= length;
    return lex_str;
  }
  LEX_CSTRING *make_lex_string(LEX_CSTRING *lex_str, const char* str, size_t length)
  {
    if (!(lex_str->str= strmake_root(mem_root, str, length)))
    {
      lex_str->length= 0;
      return 0;
    }
    lex_str->length= length;
    return lex_str;
  }
  
  bool quote_unescape(LEX_CSTRING *dst, const LEX_CSTRING *src, char quote)
  {
    const char *tmp= src->str;
    const char *tmpend= src->str + src->length;
    char *to;
    if (!(dst->str= to= (char *) alloc(src->length + 1)))
    {
      dst->length= 0; 
      return true;
    }
    for ( ; tmp < tmpend; )
    {
      if ((*to++= *tmp++) == quote)
        tmp++;                                  
    }
    *to= 0;                                     
    dst->length= to - dst->str;
    return false;
  }

  LEX_CSTRING *make_clex_string(const char* str, size_t length)
  {
    LEX_CSTRING *lex_str;
    char *tmp;
    if (unlikely(!(lex_str= (LEX_CSTRING *)alloc_root(mem_root,
                                                      sizeof(LEX_CSTRING) +
                                                      length+1))))
      return 0;
    tmp= (char*) (lex_str+1);
    lex_str->str= tmp;
    memcpy(tmp, str, length);
    tmp[length]= 0;
    lex_str->length= length;
    return lex_str;
  }
  LEX_CSTRING *make_clex_string(const LEX_CSTRING from)
  {
    return make_clex_string(from.str, from.length);
  }

  
  bool alloc_lex_string(LEX_STRING *dst, size_t length)
  {
    if (likely((dst->str= (char*) alloc(length))))
      return false;
    dst->length= 0;  
    return true;     
  }
  bool convert_string(LEX_STRING *to, CHARSET_INFO *to_cs,
		      const char *from, size_t from_length,
		      CHARSET_INFO *from_cs);
  bool convert_string(LEX_CSTRING *to, CHARSET_INFO *to_cs,
                      const char *from, size_t from_length,
                      CHARSET_INFO *from_cs)
  {
    LEX_STRING tmp;
    bool rc= convert_string(&tmp, to_cs, from, from_length, from_cs);
    to->str= tmp.str;
    to->length= tmp.length;
    return rc;
  }
  bool convert_string(LEX_CSTRING *to, CHARSET_INFO *tocs,
                      const LEX_CSTRING *from, CHARSET_INFO *fromcs,
                      bool simple_copy_is_possible)
  {
    if (!simple_copy_is_possible)
      return unlikely(convert_string(to, tocs, from->str, from->length, fromcs));
    *to= *from;
    return false;
  }
  
  bool convert_fix(CHARSET_INFO *dstcs, LEX_STRING *dst,
                   CHARSET_INFO *srccs, const char *src, size_t src_length,
                   String_copier *status);

  
  bool convert_with_error(CHARSET_INFO *dstcs, LEX_STRING *dst,
                          CHARSET_INFO *srccs,
                          const char *src, size_t src_length);
  
  bool copy_fix(CHARSET_INFO *dstcs, LEX_STRING *dst,
                CHARSET_INFO *srccs, const char *src, size_t src_length,
                String_copier *status);

  
  bool copy_with_error(CHARSET_INFO *dstcs, LEX_STRING *dst,
                       CHARSET_INFO *srccs, const char *src, size_t src_length);

  bool convert_string(String *s, CHARSET_INFO *from_cs, CHARSET_INFO *to_cs);

  
  bool check_string_for_wellformedness(const char *str,
                                       size_t length,
                                       CHARSET_INFO *cs) const;

  bool to_ident_sys_alloc(Lex_ident_sys_st *to, const Lex_ident_cli_st *from);

  
  Item_basic_constant *make_string_literal(const char *str, size_t length,
                                           uint repertoire);
  Item_basic_constant *make_string_literal(const Lex_string_with_metadata_st &str)
  {
    uint repertoire= str.repertoire(variables.character_set_client);
    return make_string_literal(str.str, str.length, repertoire);
  }
  Item_basic_constant *make_string_literal_nchar(const Lex_string_with_metadata_st &str);
  Item_basic_constant *make_string_literal_charset(const Lex_string_with_metadata_st &str,
                                                   CHARSET_INFO *cs);
  bool make_text_string_sys(LEX_CSTRING *to,
                            const Lex_string_with_metadata_st *from)
  {
    return convert_string(to, system_charset_info,
                          from, charset(), charset_is_system_charset);
  }
  bool make_text_string_connection(LEX_CSTRING *to,
                                   const Lex_string_with_metadata_st *from)
  {
    return convert_string(to, variables.collation_connection,
                          from, charset(), charset_is_collation_connection);
  }
  bool make_text_string_filesystem(LEX_CSTRING *to,
                                   const Lex_string_with_metadata_st *from)
  {
    return convert_string(to, variables.character_set_filesystem,
                          from, charset(), charset_is_character_set_filesystem);
  }
  void add_changed_table(TABLE *table);
  void add_changed_table(const char *key, size_t key_length);
  CHANGED_TABLE_LIST * changed_table_dup(const char *key, size_t key_length);
  int prepare_explain_fields(select_result *result, List<Item> *field_list,
                             uint8 explain_flags, bool is_analyze);
  int send_explain_fields(select_result *result, uint8 explain_flags,
                          bool is_analyze);
  void make_explain_field_list(List<Item> &field_list, uint8 explain_flags,
                               bool is_analyze);
  void make_explain_json_field_list(List<Item> &field_list, bool is_analyze);

  
  inline void clear_error(bool clear_diagnostics= 0)
  {
    DBUG_ENTER("clear_error");
    if (get_stmt_da()->is_error() || clear_diagnostics)
      get_stmt_da()->reset_diagnostics_area();
    is_slave_error= 0;
    if (killed == KILL_BAD_DATA)
      reset_killed();
    DBUG_VOID_RETURN;
  }

#ifndef EMBEDDED_LIBRARY
  inline bool vio_ok() const { return net.vio != 0; }
  
  bool is_connected()
  {
    
    return system_thread || (vio_ok() ? vio_is_connected(net.vio) : FALSE);
  }
#else
  inline bool vio_ok() const { return TRUE; }
  inline bool is_connected() { return TRUE; }
#endif
  
  inline void fatal_error()
  {
    DBUG_ASSERT(get_stmt_da()->is_error() || killed);
    is_fatal_error= 1;
    DBUG_PRINT("error",("Fatal error set"));
  }
  
  inline bool is_error() const { return m_stmt_da->is_error(); }
  void set_bulk_execution(void *bulk)
  {
    bulk_param= bulk;
    m_stmt_da->set_bulk_execution(MY_TEST(bulk));
  }
  bool is_bulk_op() const { return MY_TEST(bulk_param); }

  
  Diagnostics_area *get_stmt_da()
  { return m_stmt_da; }

  
  const Diagnostics_area *get_stmt_da() const
  { return m_stmt_da; }

  
  void set_stmt_da(Diagnostics_area *da)
  { m_stmt_da= da; }

  inline CHARSET_INFO *charset() const { return variables.character_set_client; }
  void update_charset();
  void update_charset(CHARSET_INFO *character_set_client,
                      CHARSET_INFO *collation_connection)
  {
    variables.character_set_client= character_set_client;
    variables.collation_connection= collation_connection;
    update_charset();
  }
  void update_charset(CHARSET_INFO *character_set_client,
                      CHARSET_INFO *collation_connection,
                      CHARSET_INFO *character_set_results)
  {
    variables.character_set_client= character_set_client;
    variables.collation_connection= collation_connection;
    variables.character_set_results= character_set_results;
    update_charset();
  }

  inline Query_arena *activate_stmt_arena_if_needed(Query_arena *backup)
  {
    
    if (!stmt_arena->is_conventional() && mem_root != stmt_arena->mem_root)
    {
      set_n_backup_active_arena(stmt_arena, backup);
      return stmt_arena;
    }
    return 0;
  }

  void change_item_tree(Item **place, Item *new_value)
  {
    DBUG_ENTER("THD::change_item_tree");
    DBUG_PRINT("enter", ("Register: %p (%p) <- %p",
                       *place, place, new_value));
    
    if (!stmt_arena->is_conventional())
      nocheck_register_item_tree_change(place, *place, mem_root);
    *place= new_value;
    DBUG_VOID_RETURN;
  }
  
  void check_and_register_item_tree(Item **place, Item **new_value)
  {
    if (!stmt_arena->is_conventional())
      check_and_register_item_tree_change(place, new_value, mem_root);
    
    memcpy((char*) place, new_value, sizeof(*new_value));
  }

  
  void end_statement();

  
  inline void set_killed(killed_state killed_arg,
                         int killed_errno_arg= 0,
                         const char *killed_err_msg_arg= 0)
  {
    mysql_mutex_lock(&LOCK_thd_kill);
    set_killed_no_mutex(killed_arg, killed_errno_arg, killed_err_msg_arg);
    mysql_mutex_unlock(&LOCK_thd_kill);
  }
  
  inline void set_killed_no_mutex(killed_state killed_arg,
                                  int killed_errno_arg= 0,
                                  const char *killed_err_msg_arg= 0)
  {
    if (killed <= killed_arg)
    {
      killed= killed_arg;
      if (killed_errno_arg)
      {
        
        if (likely((killed_err= (err_info*) alloc(sizeof(*killed_err)))))
        {
          killed_err->no= killed_errno_arg;
          ::strmake((char*) killed_err->msg, killed_err_msg_arg,
                    sizeof(killed_err->msg)-1);
        }
      }
    }
  }
  int killed_errno();
  void reset_killed();
  inline void reset_kill_query()
  {
    if (killed < KILL_CONNECTION)
    {
      reset_killed();
      mysys_var->abort= 0;
    }
  }
  inline void send_kill_message()
  {
    mysql_mutex_lock(&LOCK_thd_kill);
    int err= killed_errno();
    if (err)
      my_message(err, killed_err ? killed_err->msg : ER_THD(this, err),
                 MYF(0));
    mysql_mutex_unlock(&LOCK_thd_kill);
  }
  
  inline bool really_abort_on_warning()
  {
    return (abort_on_warning &&
            (!transaction.stmt.modified_non_trans_table ||
             (variables.sql_mode & MODE_STRICT_ALL_TABLES)));
  }
  void set_status_var_init();
  void reset_n_backup_open_tables_state(Open_tables_backup *backup);
  void restore_backup_open_tables_state(Open_tables_backup *backup);
  void reset_sub_statement_state(Sub_statement_state *backup, uint new_state);
  void restore_sub_statement_state(Sub_statement_state *backup);
  void store_slow_query_state(Sub_statement_state *backup);
  void reset_slow_query_state();
  void add_slow_query_state(Sub_statement_state *backup);
  void set_n_backup_active_arena(Query_arena *set, Query_arena *backup);
  void restore_active_arena(Query_arena *set, Query_arena *backup);

  inline void get_binlog_format(enum_binlog_format *format,
                                enum_binlog_format *current_format)
  {
    *format= (enum_binlog_format) variables.binlog_format;
    *current_format= current_stmt_binlog_format;
  }
  inline enum_binlog_format get_current_stmt_binlog_format()
  {
    return current_stmt_binlog_format;
  }
  inline void set_binlog_format(enum_binlog_format format,
                                enum_binlog_format current_format)
  {
    DBUG_ENTER("set_binlog_format");
    variables.binlog_format= format;
    current_stmt_binlog_format= current_format;
    DBUG_VOID_RETURN;
  }
  inline void set_binlog_format_stmt()
  {
    DBUG_ENTER("set_binlog_format_stmt");
    variables.binlog_format=    BINLOG_FORMAT_STMT;
    current_stmt_binlog_format= BINLOG_FORMAT_STMT;
    DBUG_VOID_RETURN;
  }
  
  inline void set_current_stmt_binlog_format_row_if_mixed()
  {
    DBUG_ENTER("set_current_stmt_binlog_format_row_if_mixed");
    
    
    
    if ((wsrep_binlog_format() == BINLOG_FORMAT_MIXED) && (in_sub_stmt == 0))
      set_current_stmt_binlog_format_row();

    DBUG_VOID_RETURN;
  }

  inline void set_current_stmt_binlog_format(enum_binlog_format format)
  {
    current_stmt_binlog_format= format;
  }

  inline void set_current_stmt_binlog_format_row()
  {
    DBUG_ENTER("set_current_stmt_binlog_format_row");
    current_stmt_binlog_format= BINLOG_FORMAT_ROW;
    DBUG_VOID_RETURN;
  }
  
  inline enum_binlog_format set_current_stmt_binlog_format_stmt()
  {
    enum_binlog_format orig_format= current_stmt_binlog_format;
    DBUG_ENTER("set_current_stmt_binlog_format_stmt");
    current_stmt_binlog_format= BINLOG_FORMAT_STMT;
    DBUG_RETURN(orig_format);
  }
  inline void restore_stmt_binlog_format(enum_binlog_format format)
  {
    DBUG_ENTER("restore_stmt_binlog_format");
    DBUG_ASSERT(!is_current_stmt_binlog_format_row());
    current_stmt_binlog_format= format;
    DBUG_VOID_RETURN;
  }
  inline void reset_current_stmt_binlog_format_row()
  {
    DBUG_ENTER("reset_current_stmt_binlog_format_row");
    
    DBUG_PRINT("debug",
               ("temporary_tables: %s, in_sub_stmt: %s, system_thread: %s",
                YESNO(has_thd_temporary_tables()), YESNO(in_sub_stmt),
                show_system_thread(system_thread)));
    if (in_sub_stmt == 0)
    {
      if (wsrep_binlog_format() == BINLOG_FORMAT_ROW)
        set_current_stmt_binlog_format_row();
      else if (!has_thd_temporary_tables())
        set_current_stmt_binlog_format_stmt();
    }
    DBUG_VOID_RETURN;
  }

  
  bool set_db(const LEX_CSTRING *new_db);

  
  void reset_db(const LEX_CSTRING *new_db);

  
  bool copy_db_to(LEX_CSTRING *to)
  {
    if (db.str == NULL)
    {
      
      if (!lex->with_clauses_list)
      {
        my_message(ER_NO_DB_ERROR, ER(ER_NO_DB_ERROR), MYF(0));
        return TRUE;
      }
      
      to->str= NULL;
      to->length= 0;
      return FALSE;
    }

    to->str= strmake(db.str, db.length);
    to->length= db.length;
    return to->str == NULL;                     
  }
  
  const char *get_db()
  { return safe_str(db.str); }

  thd_scheduler event_scheduler;

public:
  inline Internal_error_handler *get_internal_handler()
  { return m_internal_handler; }

  
  void push_internal_handler(Internal_error_handler *handler);

private:
  
  bool handle_condition(uint sql_errno,
                        const char* sqlstate,
                        Sql_condition::enum_warning_level *level,
                        const char* msg,
                        Sql_condition ** cond_hdl);

public:
  
  Internal_error_handler *pop_internal_handler();

  
  void raise_error(uint code);

  
  void raise_error_printf(uint code, ...);

  
  void raise_warning(uint code);

  
  void raise_warning_printf(uint code, ...);

  
  void raise_note(uint code);

  
  void raise_note_printf(uint code, ...);

  
  void parse_error(const char *err_text, const char *yytext)
  {
    Lex_input_stream *lip= &m_parser_state->m_lip;
    if (!yytext && !(yytext= lip->get_tok_start()))
        yytext= "";
    
    ErrConvString err(yytext, strlen(yytext), variables.character_set_client);
    my_printf_error(ER_PARSE_ERROR,  ER_THD(this, ER_PARSE_ERROR), MYF(0),
                    err_text, err.ptr(), lip->yylineno);
  }
  void parse_error(uint err_number, const char *yytext= 0)
  {
    parse_error(ER_THD(this, err_number), yytext);
  }
  void parse_error()
  {
    parse_error(ER_SYNTAX_ERROR);
  }
#ifdef mysqld_error_find_printf_error_used
  void parse_error(const char *t)
  {
  }
#endif
private:
  
  friend class Sql_cmd_common_signal;
  friend class Sql_cmd_signal;
  friend class Sql_cmd_resignal;
  friend void push_warning(THD*, Sql_condition::enum_warning_level, uint, const char*);
  friend void my_message_sql(uint, const char *, myf);

  
  Sql_condition*
  raise_condition(uint sql_errno,
                  const char* sqlstate,
                  Sql_condition::enum_warning_level level,
                  const char* msg)
  {
    return raise_condition(sql_errno, sqlstate, level,
                           Sql_user_condition_identity(), msg);
  }

  
  Sql_condition*
  raise_condition(uint sql_errno,
                  const char* sqlstate,
                  Sql_condition::enum_warning_level level,
                  const Sql_user_condition_identity &ucid,
                  const char* msg);

  Sql_condition*
  raise_condition(const Sql_condition *cond)
  {
    Sql_condition *raised= raise_condition(cond->get_sql_errno(),
                                           cond->get_sqlstate(),
                                           cond->get_level(),
                                           *cond,
                                           cond->get_message_text());
    if (raised)
      raised->copy_opt_attributes(cond);
    return raised;
  }

public:
  
  virtual void set_statement(Statement *stmt);
  void set_command(enum enum_server_command command)
  {
    m_command= command;
#ifdef HAVE_PSI_THREAD_INTERFACE
    PSI_STATEMENT_CALL(set_thread_command)(m_command);
#endif
  }
  inline enum enum_server_command get_command() const
  { return m_command; }

  
  void set_query(char *query_arg, size_t query_length_arg,
                 CHARSET_INFO *cs_arg)
  {
    set_query(CSET_STRING(query_arg, query_length_arg, cs_arg));
  }
  void set_query(char *query_arg, size_t query_length_arg) 
  {
    set_query(CSET_STRING(query_arg, query_length_arg, charset()));
  }
  void set_query(const CSET_STRING &string_arg)
  {
    mysql_mutex_lock(&LOCK_thd_data);
    set_query_inner(string_arg);
    mysql_mutex_unlock(&LOCK_thd_data);

    PSI_CALL_set_thread_info(query(), query_length());
  }
  void reset_query()               
  { set_query(CSET_STRING()); }
  void set_query_and_id(char *query_arg, uint32 query_length_arg,
                        CHARSET_INFO *cs, query_id_t new_query_id);
  void set_query_id(query_id_t new_query_id)
  {
    query_id= new_query_id;
  }
  void set_open_tables(TABLE *open_tables_arg)
  {
    mysql_mutex_lock(&LOCK_thd_data);
    open_tables= open_tables_arg;
    mysql_mutex_unlock(&LOCK_thd_data);
  }
  void set_mysys_var(struct st_my_thread_var *new_mysys_var);
  void enter_locked_tables_mode(enum_locked_tables_mode mode_arg)
  {
    DBUG_ASSERT(locked_tables_mode == LTM_NONE);

    if (mode_arg == LTM_LOCK_TABLES)
    {
      
      mdl_context.set_explicit_duration_for_all_locks();
    }

    locked_tables_mode= mode_arg;
  }
  void leave_locked_tables_mode();
  int decide_logging_format(TABLE_LIST *tables);

  enum need_invoker { INVOKER_NONE=0, INVOKER_USER, INVOKER_ROLE};
  void binlog_invoker(bool role) { m_binlog_invoker= role ? INVOKER_ROLE : INVOKER_USER; }
  enum need_invoker need_binlog_invoker() { return m_binlog_invoker; }
  void get_definer(LEX_USER *definer, bool role);
  void set_invoker(const LEX_CSTRING *user, const LEX_CSTRING *host)
  {
    invoker.user= *user;
    invoker.host= *host;
  }
  LEX_CSTRING get_invoker_user() { return invoker.user; }
  LEX_CSTRING get_invoker_host() { return invoker.host; }
  bool has_invoker() { return invoker.user.length > 0; }

  void print_aborted_warning(uint threshold, const char *reason)
  {
    if (global_system_variables.log_warnings > threshold)
    {
      Security_context *sctx= &main_security_ctx;
      sql_print_warning(ER_THD(this, ER_NEW_ABORTING_CONNECTION),
                        thread_id, (db.str ? db.str : "unconnected"),
                        sctx->user ? sctx->user : "unauthenticated",
                        sctx->host_or_ip, reason);
    }
  }

public:
  void clear_wakeup_ready() { wakeup_ready= false; }
  
  void wait_for_wakeup_ready();
  
  void signal_wakeup_ready();

  void add_status_to_global()
  {
    DBUG_ASSERT(status_in_global == 0);
    mysql_mutex_lock(&LOCK_status);
    add_to_status(&global_status_var, &status_var);
    
    status_var.global_memory_used= 0;
    status_in_global= 1;
    mysql_mutex_unlock(&LOCK_status);
  }

  wait_for_commit *wait_for_commit_ptr;
  int wait_for_prior_commit()
  {
    if (wait_for_commit_ptr)
      return wait_for_commit_ptr->wait_for_prior_commit(this);
    return 0;
  }
  void wakeup_subsequent_commits(int wakeup_error)
  {
    if (wait_for_commit_ptr)
      wait_for_commit_ptr->wakeup_subsequent_commits(wakeup_error);
  }
  wait_for_commit *suspend_subsequent_commits() {
    wait_for_commit *suspended= wait_for_commit_ptr;
    wait_for_commit_ptr= NULL;
    return suspended;
  }
  void resume_subsequent_commits(wait_for_commit *suspended) {
    DBUG_ASSERT(!wait_for_commit_ptr);
    wait_for_commit_ptr= suspended;
  }

  void mark_transaction_to_rollback(bool all);
private:

  
  Internal_error_handler *m_internal_handler;

  
  LEX main_lex;
  
  MEM_ROOT main_mem_root;
  Diagnostics_area main_da;
  Diagnostics_area *m_stmt_da;

  
  enum need_invoker m_binlog_invoker;

  
  AUTHID invoker;

public:
#ifndef EMBEDDED_LIBRARY
  Session_tracker session_tracker;
#endif 
  
  bool wakeup_ready;
  mysql_mutex_t LOCK_wakeup_ready;
  mysql_cond_t COND_wakeup_ready;
  
private:
  rpl_gtid m_last_commit_gtid;

public:
  rpl_gtid get_last_commit_gtid() { return m_last_commit_gtid; }
  void set_last_commit_gtid(rpl_gtid &gtid);


  LF_PINS *tdc_hash_pins;
  LF_PINS *xid_hash_pins;
  bool fix_xid_hash_pins();


public:
  
  enum Temporary_table_state {
    TMP_TABLE_IN_USE,
    TMP_TABLE_NOT_IN_USE,
    TMP_TABLE_ANY
  };
  bool has_thd_temporary_tables();

  TABLE *create_and_open_tmp_table(handlerton *hton,
                                   LEX_CUSTRING *frm,
                                   const char *path,
                                   const char *db,
                                   const char *table_name,
                                   bool open_in_engine,
                                   bool open_internal_tables);

  TABLE *find_temporary_table(const char *db, const char *table_name,
                              Temporary_table_state state= TMP_TABLE_IN_USE);
  TABLE *find_temporary_table(const TABLE_LIST *tl,
                              Temporary_table_state state= TMP_TABLE_IN_USE);

  TMP_TABLE_SHARE *find_tmp_table_share_w_base_key(const char *key,
                                                   uint key_length);
  TMP_TABLE_SHARE *find_tmp_table_share(const char *db,
                                        const char *table_name);
  TMP_TABLE_SHARE *find_tmp_table_share(const TABLE_LIST *tl);
  TMP_TABLE_SHARE *find_tmp_table_share(const char *key, size_t key_length);

  bool open_temporary_table(TABLE_LIST *tl);
  bool open_temporary_tables(TABLE_LIST *tl);

  bool close_temporary_tables();
  bool rename_temporary_table(TABLE *table, const LEX_CSTRING *db,
                              const LEX_CSTRING *table_name);
  bool drop_temporary_table(TABLE *table, bool *is_trans, bool delete_table);
  bool rm_temporary_table(handlerton *hton, const char *path);
  void mark_tmp_tables_as_free_for_reuse();
  void mark_tmp_table_as_free_for_reuse(TABLE *table);

  TMP_TABLE_SHARE* save_tmp_table_share(TABLE *table);
  void restore_tmp_table_share(TMP_TABLE_SHARE *share);
  void close_unused_temporary_table_instances(const TABLE_LIST *tl);

private:
  
  bool m_tmp_tables_locked;

  bool has_temporary_tables();
  uint create_tmp_table_def_key(char *key, const char *db,
                                const char *table_name);
  TMP_TABLE_SHARE *create_temporary_table(handlerton *hton, LEX_CUSTRING *frm,
                                          const char *path, const char *db,
                                          const char *table_name);
  TABLE *find_temporary_table(const char *key, uint key_length,
                              Temporary_table_state state);
  TABLE *open_temporary_table(TMP_TABLE_SHARE *share, const char *alias,
                              bool open_in_engine);
  bool find_and_use_tmp_table(const TABLE_LIST *tl, TABLE **out_table);
  bool use_temporary_table(TABLE *table, TABLE **out_table);
  void close_temporary_table(TABLE *table);
  bool log_events_and_free_tmp_shares();
  void free_tmp_table_share(TMP_TABLE_SHARE *share, bool delete_table);
  void free_temporary_table(TABLE *table);
  bool lock_temporary_tables();
  void unlock_temporary_tables();

  inline uint tmpkeyval(TMP_TABLE_SHARE *share)
  {
    return uint4korr(share->table_cache_key.str +
                     share->table_cache_key.length - 4);
  }

  inline TMP_TABLE_SHARE *tmp_table_share(TABLE *table)
  {
    DBUG_ASSERT(table->s->tmp_table);
    return static_cast<TMP_TABLE_SHARE *>(table->s);
  }

public:
  inline ulong wsrep_binlog_format() const
  {
    return WSREP_FORMAT(variables.binlog_format);
  }

#ifdef WITH_WSREP
  const bool                wsrep_applier; 
  bool                      wsrep_applier_closing; 
  bool                      wsrep_client_thread; 
  bool                      wsrep_PA_safe;
  bool                      wsrep_converted_lock_session;
  bool                      wsrep_apply_toi; 
  enum wsrep_exec_mode      wsrep_exec_mode;
  query_id_t                wsrep_last_query_id;
  enum wsrep_query_state    wsrep_query_state;
  enum wsrep_conflict_state wsrep_conflict_state;
  wsrep_trx_meta_t          wsrep_trx_meta;
  uint32                    wsrep_rand;
  Relay_log_info            *wsrep_rli;
  rpl_group_info            *wsrep_rgi;
  wsrep_ws_handle_t         wsrep_ws_handle;
  ulong                     wsrep_retry_counter; 
  char                      *wsrep_retry_query;
  size_t                    wsrep_retry_query_len;
  enum enum_server_command  wsrep_retry_command;
  enum wsrep_consistency_check_mode
                            wsrep_consistency_check;
  int                       wsrep_mysql_replicated;
  const char                *wsrep_TOI_pre_query; 
  size_t                    wsrep_TOI_pre_query_len;
  wsrep_po_handle_t         wsrep_po_handle;
  size_t                    wsrep_po_cnt;
#ifdef GTID_SUPPORT
  rpl_sid                   wsrep_po_sid;
#endif 
  void                      *wsrep_apply_format;
  char                      wsrep_info[128]; 
  
  bool                      wsrep_ignore_table;
  wsrep_gtid_t              wsrep_sync_wait_gtid;
  ulong                     wsrep_affected_rows;
  bool                      wsrep_replicate_GTID;
  bool                      wsrep_skip_wsrep_GTID;
  
  bool                      wsrep_split_flag;
#endif 

  
  thr_timer_t query_timer;

public:
  void set_query_timer()
  {
#ifndef EMBEDDED_LIBRARY
    
    if (!variables.max_statement_time || spcont  || in_sub_stmt ||
        slave_thread || query_timer.expired == 0)
      return;
    thr_timer_settime(&query_timer, variables.max_statement_time);
#endif
  }
  void reset_query_timer()
  {
#ifndef EMBEDDED_LIBRARY
    if (spcont || in_sub_stmt || slave_thread)
      return;
    if (!query_timer.expired)
      thr_timer_end(&query_timer);
#endif
  }
  void restore_set_statement_var()
  {
    main_lex.restore_set_statement_var();
  }
  
  void merge_unsafe_rollback_flags()
  {
    if (transaction.stmt.modified_non_trans_table)
      transaction.all.modified_non_trans_table= TRUE;
    transaction.all.m_unsafe_rollback_flags|=
      (transaction.stmt.m_unsafe_rollback_flags &
       (THD_TRANS::DID_WAIT | THD_TRANS::CREATED_TEMP_TABLE |
        THD_TRANS::DROPPED_TEMP_TABLE | THD_TRANS::DID_DDL));
  }
  
  inline void reset_current_linfo()
  {
    mysql_mutex_lock(&LOCK_thread_count);
    current_linfo= 0;
    mysql_mutex_unlock(&LOCK_thread_count);
  }


  uint get_net_wait_timeout()
  {
    if (in_active_multi_stmt_transaction())
    {
      if (transaction.all.is_trx_read_write())
      {
        if (variables.idle_write_transaction_timeout > 0)
          return variables.idle_write_transaction_timeout;
      }
      else
      {
        if (variables.idle_readonly_transaction_timeout > 0)
          return variables.idle_readonly_transaction_timeout;
      }

      if (variables.idle_transaction_timeout > 0)
        return variables.idle_transaction_timeout;
    }

    return variables.net_wait_timeout;
  }

  
  void set_local_lex(sp_lex_local *sublex)
  {
    DBUG_ASSERT(lex->sphead);
    lex= sublex;
    
    m_parser_state->m_yacc.reset_before_substatement();
  }

  
  bool restore_from_local_lex_to_old_lex(LEX *oldlex);

  Item *sp_fix_func_item(Item **it_addr);
  Item *sp_prepare_func_item(Item **it_addr, uint cols= 1);
  bool sp_eval_expr(Field *result_field, Item **expr_item_ptr);
};

inline void add_to_active_threads(THD *thd)
{
  mysql_mutex_lock(&LOCK_thread_count);
  threads.append(thd);
  mysql_mutex_unlock(&LOCK_thread_count);
}



inline void unlink_not_visible_thd(THD *thd)
{
  thd->assert_linked();
  mysql_mutex_lock(&LOCK_thread_count);
  thd->unlink();
  mysql_mutex_unlock(&LOCK_thread_count);
}



inline void
my_ok(THD *thd, ulonglong affected_rows_arg= 0, ulonglong id= 0,
        const char *message= NULL)
{
  thd->set_row_count_func(affected_rows_arg);
  thd->set_affected_rows(affected_rows_arg);
  thd->get_stmt_da()->set_ok_status(affected_rows_arg, id, message);
}




inline void
my_eof(THD *thd)
{
  thd->set_row_count_func(-1);
  thd->get_stmt_da()->set_eof_status(thd);

  TRANSACT_TRACKER(add_trx_state(thd, TX_RESULT_SET));
}

#define tmp_disable_binlog(A)                                              \
  {ulonglong tmp_disable_binlog__save_options= (A)->variables.option_bits; \
  (A)->variables.option_bits&= ~OPTION_BIN_LOG;                            \
  (A)->variables.sql_log_bin_off= 1;

#define reenable_binlog(A)                                                  \
  (A)->variables.option_bits= tmp_disable_binlog__save_options;             \
  (A)->variables.sql_log_bin_off= 0;}


inline sql_mode_t sql_mode_for_dates(THD *thd)
{
  return thd->variables.sql_mode &
          (MODE_NO_ZERO_DATE | MODE_NO_ZERO_IN_DATE | MODE_INVALID_DATES);
}



class sql_exchange :public Sql_alloc
{
public:
  enum enum_filetype filetype; 
  const char *file_name;
  String *field_term,*enclosed,*line_term,*line_start,*escaped;
  bool opt_enclosed;
  bool dumpfile;
  ulong skip_lines;
  CHARSET_INFO *cs;
  sql_exchange(const char *name, bool dumpfile_flag,
               enum_filetype filetype_arg= FILETYPE_CSV);
  bool escaped_given(void) const;
};



class JOIN;


class select_result_sink: public Sql_alloc
{
public:
  THD *thd;
  select_result_sink(THD *thd_arg): thd(thd_arg) {}
  
  virtual int send_data(List<Item> &items)=0;
  virtual ~select_result_sink() {};
  void reset(THD *thd_arg) { thd= thd_arg; }
};

class select_result_interceptor;



class select_result :public select_result_sink 
{
protected:
  
  SELECT_LEX_UNIT *unit;
  
public:
  ha_rows est_records;  
 select_result(THD *thd_arg): select_result_sink(thd_arg), est_records(0) {}
  void set_unit(SELECT_LEX_UNIT *unit_arg) { unit= unit_arg; }
  virtual ~select_result() {};
  
  virtual bool change_result(select_result *new_result)
  {
    return false;
  }
  virtual int prepare(List<Item> &list, SELECT_LEX_UNIT *u)
  {
    unit= u;
    return 0;
  }
  virtual int prepare2(JOIN *join) { return 0; }
  
  virtual uint field_count(List<Item> &fields) const
  { return fields.elements; }
  virtual bool send_result_set_metadata(List<Item> &list, uint flags)=0;
  virtual bool initialize_tables (JOIN *join) { return 0; }
  virtual bool send_eof()=0;
  
  virtual bool check_simple_select() const;
  virtual void abort_result_set() {}
  
  virtual void cleanup();
  void set_thd(THD *thd_arg) { thd= thd_arg; }
  void reset(THD *thd_arg)
  {
    select_result_sink::reset(thd_arg);
    unit= NULL;
  }
#ifdef EMBEDDED_LIBRARY
  virtual void begin_dataset() {}
#else
  void begin_dataset() {}
#endif
  virtual void update_used_tables() {}

  
  virtual void prepare_to_read_rows() {}

  void reset_offset_limit()
  {
    unit->offset_limit_cnt= 0;
  }

  
  virtual select_result_interceptor *result_interceptor()=0;

  
  virtual bool view_structure_only() const { return false; }
};




class select_result_explain_buffer : public select_result_sink
{
public:
  select_result_explain_buffer(THD *thd_arg, TABLE *table_arg) : 
    select_result_sink(thd_arg), dst_table(table_arg) {};

  TABLE *dst_table; 

  
  int send_data(List<Item> &items);
};




class select_result_text_buffer : public select_result_sink
{
public:
  select_result_text_buffer(THD *thd_arg): select_result_sink(thd_arg) {}
  int send_data(List<Item> &items);
  bool send_result_set_metadata(List<Item> &fields, uint flag);

  void save_to(String *res);
private:
  int append_row(List<Item> &items, bool send_names);

  List<char*> rows;
  int n_columns;
};




class select_result_interceptor: public select_result
{
public:
  select_result_interceptor(THD *thd_arg):
    select_result(thd_arg), suppress_my_ok(false)
  {
    DBUG_ENTER("select_result_interceptor::select_result_interceptor");
    DBUG_PRINT("enter", ("this %p", this));
    DBUG_VOID_RETURN;
  }              
  uint field_count(List<Item> &fields) const { return 0; }
  bool send_result_set_metadata(List<Item> &fields, uint flag) { return FALSE; }
  select_result_interceptor *result_interceptor() { return this; }

  
  void disable_my_ok_calls() { suppress_my_ok= true; }
  void reset(THD *thd_arg)
  {
    select_result::reset(thd_arg);
    suppress_my_ok= false;
  }
protected:
  bool suppress_my_ok;
};


class sp_cursor_statistics
{
protected:
  ulonglong m_fetch_count; 
  ulonglong m_row_count;   
  bool m_found;            
public:
  sp_cursor_statistics()
   :m_fetch_count(0),
    m_row_count(0),
    m_found(false)
  { }
  bool found() const
  { return m_found; }

  ulonglong row_count() const
  { return m_row_count; }

  ulonglong fetch_count() const
  { return m_fetch_count; }
  void reset() { *this= sp_cursor_statistics(); }
};



class sp_lex_keeper;
class sp_cursor: public sp_cursor_statistics
{
private:
  
  
  class Select_fetch_into_spvars: public select_result_interceptor
  {
    List<sp_variable> *spvar_list;
    uint field_count;
    bool m_view_structure_only;
    bool send_data_to_variable_list(List<sp_variable> &vars, List<Item> &items);
  public:
    Select_fetch_into_spvars(THD *thd_arg, bool view_structure_only)
     :select_result_interceptor(thd_arg),
      m_view_structure_only(view_structure_only)
    {}
    void reset(THD *thd_arg)
    {
      select_result_interceptor::reset(thd_arg);
      spvar_list= NULL;
      field_count= 0;
    }
    uint get_field_count() { return field_count; }
    void set_spvar_list(List<sp_variable> *vars) { spvar_list= vars; }

    virtual bool send_eof() { return FALSE; }
    virtual int send_data(List<Item> &items);
    virtual int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
    virtual bool view_structure_only() const { return m_view_structure_only; }
};

public:
  sp_cursor()
   :result(NULL, false),
    m_lex_keeper(NULL),
    server_side_cursor(NULL)
  { }
  sp_cursor(THD *thd_arg, sp_lex_keeper *lex_keeper, bool view_structure_only)
   :result(thd_arg, view_structure_only),
    m_lex_keeper(lex_keeper),
    server_side_cursor(NULL)
  {}

  virtual ~sp_cursor()
  { destroy(); }

  sp_lex_keeper *get_lex_keeper() { return m_lex_keeper; }

  int open(THD *thd);

  int close(THD *thd);

  my_bool is_open()
  { return MY_TEST(server_side_cursor); }

  int fetch(THD *, List<sp_variable> *vars, bool error_on_no_data);

  bool export_structure(THD *thd, Row_definition_list *list);

  void reset(THD *thd_arg, sp_lex_keeper *lex_keeper)
  {
    sp_cursor_statistics::reset();
    result.reset(thd_arg);
    m_lex_keeper= lex_keeper;
    server_side_cursor= NULL;
  }

private:
  Select_fetch_into_spvars result;
  sp_lex_keeper *m_lex_keeper;
  Server_side_cursor *server_side_cursor;
  void destroy();
};


class select_send :public select_result {
  
  bool is_result_set_started;
public:
  select_send(THD *thd_arg):
    select_result(thd_arg), is_result_set_started(FALSE) {}
  bool send_result_set_metadata(List<Item> &list, uint flags);
  int send_data(List<Item> &items);
  bool send_eof();
  virtual bool check_simple_select() const { return FALSE; }
  void abort_result_set();
  virtual void cleanup();
  select_result_interceptor *result_interceptor() { return NULL; }
};




class select_send_analyze : public select_send
{
  bool send_result_set_metadata(List<Item> &list, uint flags) { return 0; }
  bool send_eof() { return 0; }
  void abort_result_set() {}
public:
  select_send_analyze(THD *thd_arg): select_send(thd_arg) {}
};


class select_to_file :public select_result_interceptor {
protected:
  sql_exchange *exchange;
  File file;
  IO_CACHE cache;
  ha_rows row_count;
  char path[FN_REFLEN];

public:
  select_to_file(THD *thd_arg, sql_exchange *ex):
    select_result_interceptor(thd_arg), exchange(ex), file(-1),row_count(0L)
  { path[0]=0; }
  ~select_to_file();
  bool send_eof();
  void cleanup();
};


#define ESCAPE_CHARS "ntrb0ZN" 



#define NUMERIC_CHARS ".0123456789e+-"


class select_export :public select_to_file {
  uint field_term_length;
  int field_sep_char,escape_char,line_sep_char;
  int field_term_char; 
  
  bool is_ambiguous_field_sep;
  
  bool is_ambiguous_field_term;
  
  bool is_unsafe_field_sep;
  bool fixed_row_size;
  CHARSET_INFO *write_cs; 
public:
  select_export(THD *thd_arg, sql_exchange *ex): select_to_file(thd_arg, ex) {}
  ~select_export();
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  int send_data(List<Item> &items);
};


class select_dump :public select_to_file {
public:
  select_dump(THD *thd_arg, sql_exchange *ex): select_to_file(thd_arg, ex) {}
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  int send_data(List<Item> &items);
};


class select_insert :public select_result_interceptor {
 public:
  TABLE_LIST *table_list;
  TABLE *table;
  List<Item> *fields;
  ulonglong autoinc_value_of_last_inserted_row; 
  COPY_INFO info;
  bool insert_into_view;
  select_insert(THD *thd_arg, TABLE_LIST *table_list_par,
		TABLE *table_par, List<Item> *fields_par,
		List<Item> *update_fields, List<Item> *update_values,
		enum_duplicates duplic, bool ignore);
  ~select_insert();
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  virtual int prepare2(JOIN *join);
  virtual int send_data(List<Item> &items);
  virtual void store_values(List<Item> &values);
  virtual bool can_rollback_data() { return 0; }
  bool prepare_eof();
  bool send_ok_packet();
  bool send_eof();
  virtual void abort_result_set();
  
  void cleanup();
};


class select_create: public select_insert {
  TABLE_LIST *create_table;
  Table_specification_st *create_info;
  TABLE_LIST *select_tables;
  Alter_info *alter_info;
  Field **field;
  
  MYSQL_LOCK *m_lock;
  
  MYSQL_LOCK **m_plock;
  bool       exit_done;
  TMP_TABLE_SHARE *saved_tmp_table_share;

public:
  select_create(THD *thd_arg, TABLE_LIST *table_arg,
                Table_specification_st *create_info_par,
                Alter_info *alter_info_arg,
                List<Item> &select_fields,enum_duplicates duplic, bool ignore,
                TABLE_LIST *select_tables_arg):
    select_insert(thd_arg, table_arg, NULL, &select_fields, 0, 0, duplic,
                  ignore),
    create_table(table_arg),
    create_info(create_info_par),
    select_tables(select_tables_arg),
    alter_info(alter_info_arg),
    m_plock(NULL), exit_done(0),
    saved_tmp_table_share(0)
    {}
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);

  int binlog_show_create_table(TABLE **tables, uint count);
  void store_values(List<Item> &values);
  bool send_eof();
  virtual void abort_result_set();
  virtual bool can_rollback_data() { return 1; }

  
  const THD *get_thd(void) { return thd; }
  const HA_CREATE_INFO *get_create_info() { return create_info; };
  int prepare2(JOIN *join) { return 0; }

private:
  TABLE *create_table_from_items(THD *thd,
                                  List<Item> *items,
                                  MYSQL_LOCK **lock,
                                  TABLEOP_HOOKS *hooks);
};

#include <myisam.h>

#ifdef WITH_ARIA_STORAGE_ENGINE
#include <maria.h>
#else
#undef USE_ARIA_FOR_TMP_TABLES
#endif

#ifdef USE_ARIA_FOR_TMP_TABLES
#define TMP_ENGINE_COLUMNDEF MARIA_COLUMNDEF
#define TMP_ENGINE_HTON maria_hton
#define TMP_ENGINE_NAME "Aria"
inline uint tmp_table_max_key_length() { return maria_max_key_length(); }
inline uint tmp_table_max_key_parts() { return maria_max_key_segments(); }
#else
#define TMP_ENGINE_COLUMNDEF MI_COLUMNDEF
#define TMP_ENGINE_HTON myisam_hton
#define TMP_ENGINE_NAME "MyISAM"
inline uint tmp_table_max_key_length() { return MI_MAX_KEY_LENGTH; }
inline uint tmp_table_max_key_parts() { return MI_MAX_KEY_SEG; }
#endif



class TMP_TABLE_PARAM :public Sql_alloc
{
public:
  List<Item> copy_funcs;
  Copy_field *copy_field, *copy_field_end;
  uchar	    *group_buff;
  Item	    **items_to_copy;			
  TMP_ENGINE_COLUMNDEF *recinfo, *start_recinfo;
  KEY *keyinfo;
  ha_rows end_write_records;
  
  uint	field_count; 
  
  uint  func_count;  
  
  uint  sum_func_count;   
  uint  hidden_field_count;
  uint	group_parts,group_length,group_null_parts;
  uint	quick_group;
  
  bool  using_outer_summary_function;
  CHARSET_INFO *table_charset;
  bool schema_table;
  
  bool materialized_subquery;
  
  bool force_not_null_cols;
  
  bool precomputed_group_by;
  bool force_copy_fields;
  
  bool bit_fields_as_long;
  
  bool skip_create_table;

  TMP_TABLE_PARAM()
    :copy_field(0), group_parts(0),
     group_length(0), group_null_parts(0),
     using_outer_summary_function(0),
     schema_table(0), materialized_subquery(0), force_not_null_cols(0),
     precomputed_group_by(0),
     force_copy_fields(0), bit_fields_as_long(0), skip_create_table(0)
  {}
  ~TMP_TABLE_PARAM()
  {
    cleanup();
  }
  void init(void);
  inline void cleanup(void)
  {
    if (copy_field)				
    {
      delete [] copy_field;
      copy_field= NULL;
      copy_field_end= NULL;
    }
  }
};


class select_unit :public select_result_interceptor
{
  uint curr_step, prev_step, curr_sel;
  enum sub_select_type step;
public:
  Item_int *intersect_mark;
  TMP_TABLE_PARAM tmp_table_param;
  int write_err; 
  TABLE *table;

  select_unit(THD *thd_arg):
    select_result_interceptor(thd_arg),
    intersect_mark(0), table(0)
  {
    init();
    tmp_table_param.init();
  }
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  
  virtual bool postponed_prepare(List<Item> &types)
  { return false; }
  int send_data(List<Item> &items);
  bool send_eof();
  virtual bool flush();
  void cleanup();
  virtual bool create_result_table(THD *thd, List<Item> *column_types,
                                   bool is_distinct, ulonglong options,
                                   const LEX_CSTRING *alias,
                                   bool bit_fields_as_long,
                                   bool create_table,
                                   bool keep_row_order,
                                   uint hidden);
  TMP_TABLE_PARAM *get_tmp_table_param() { return &tmp_table_param; }
  void init()
  {
    curr_step= prev_step= 0;
    curr_sel= UINT_MAX;
    step= UNION_TYPE;
    write_err= 0;
  }
  void change_select();
};

class select_union_recursive :public select_unit
{
 public:
  
  TABLE *incr_table;
  
  TABLE *first_rec_table_to_update;
  
  List<TABLE> rec_tables;
  
  uint cleanup_count;

  select_union_recursive(THD *thd_arg):
    select_unit(thd_arg),
    incr_table(0), first_rec_table_to_update(0), cleanup_count(0) {};

  int send_data(List<Item> &items);
  bool create_result_table(THD *thd, List<Item> *column_types,
                           bool is_distinct, ulonglong options,
                           const LEX_CSTRING *alias,
                           bool bit_fields_as_long,
                           bool create_table,
                           bool keep_row_order,
                           uint hidden);
  void cleanup();
};



class select_union_direct :public select_unit
{
private:
  
  select_result *result;
  
  SELECT_LEX *last_select_lex;

  
  bool done_send_result_set_metadata;
  
  bool done_initialize_tables;

  
  ulonglong limit_found_rows;

  
  ha_rows offset;
  
  ha_rows limit;

public:
  
  ha_rows send_records; 
  select_union_direct(THD *thd_arg, select_result *result_arg,
                      SELECT_LEX *last_select_lex_arg):
  select_unit(thd_arg), result(result_arg),
    last_select_lex(last_select_lex_arg),
    done_send_result_set_metadata(false), done_initialize_tables(false),
    limit_found_rows(0)
    { send_records= 0; }
  bool change_result(select_result *new_result);
  uint field_count(List<Item> &fields) const
  {
    
    DBUG_ASSERT(false); 
    return 0; 
  }
  bool postponed_prepare(List<Item> &types);
  bool send_result_set_metadata(List<Item> &list, uint flags);
  int send_data(List<Item> &items);
  bool initialize_tables (JOIN *join);
  bool send_eof();
  bool flush() { return false; }
  bool check_simple_select() const
  {
    
    DBUG_ASSERT(false); 
    return false; 
  }
  void abort_result_set()
  {
    result->abort_result_set(); 
  }
  void cleanup()
  {
    send_records= 0;
  }
  void set_thd(THD *thd_arg)
  {
    
    DBUG_ASSERT(false); 
  }
  void reset_offset_limit_cnt()
  {
    
    DBUG_ASSERT(false); 
  }
  void begin_dataset()
  {
    
    DBUG_ASSERT(false); 
  }
};



class select_subselect :public select_result_interceptor
{
protected:
  Item_subselect *item;
public:
  select_subselect(THD *thd_arg, Item_subselect *item_arg):
    select_result_interceptor(thd_arg), item(item_arg) {}
  int send_data(List<Item> &items)=0;
  bool send_eof() { return 0; };
};


class select_singlerow_subselect :public select_subselect
{
public:
  select_singlerow_subselect(THD *thd_arg, Item_subselect *item_arg):
    select_subselect(thd_arg, item_arg)
  {}
  int send_data(List<Item> &items);
};




class select_materialize_with_stats : public select_unit
{
protected:
  class Column_statistics
  {
  public:
    
    ha_rows null_count;
    
    ha_rows min_null_row;
    
    ha_rows max_null_row;
  };

  
  Column_statistics* col_stat;

  
  uint max_nulls_in_row;
  
  ha_rows count_rows;

protected:
  void reset();

public:
  select_materialize_with_stats(THD *thd_arg): select_unit(thd_arg)
  { tmp_table_param.init(); }
  bool create_result_table(THD *thd, List<Item> *column_types,
                           bool is_distinct, ulonglong options,
                           const LEX_CSTRING *alias,
                           bool bit_fields_as_long,
                           bool create_table,
                           bool keep_row_order,
                           uint hidden);
  bool init_result_table(ulonglong select_options);
  int send_data(List<Item> &items);
  void cleanup();
  ha_rows get_null_count_of_col(uint idx)
  {
    DBUG_ASSERT(idx < table->s->fields);
    return col_stat[idx].null_count;
  }
  ha_rows get_max_null_of_col(uint idx)
  {
    DBUG_ASSERT(idx < table->s->fields);
    return col_stat[idx].max_null_row;
  }
  ha_rows get_min_null_of_col(uint idx)
  {
    DBUG_ASSERT(idx < table->s->fields);
    return col_stat[idx].min_null_row;
  }
  uint get_max_nulls_in_row() { return max_nulls_in_row; }
};



class select_max_min_finder_subselect :public select_subselect
{
  Item_cache *cache;
  bool (select_max_min_finder_subselect::*op)();
  bool fmax;
  bool is_all;
public:
  select_max_min_finder_subselect(THD *thd_arg, Item_subselect *item_arg,
                                  bool mx, bool all):
    select_subselect(thd_arg, item_arg), cache(0), fmax(mx), is_all(all)
  {}
  void cleanup();
  int send_data(List<Item> &items);
  bool cmp_real();
  bool cmp_int();
  bool cmp_decimal();
  bool cmp_str();
};


class select_exists_subselect :public select_subselect
{
public:
  select_exists_subselect(THD *thd_arg, Item_subselect *item_arg):
    select_subselect(thd_arg, item_arg) {}
  int send_data(List<Item> &items);
};



class SJ_MATERIALIZATION_INFO : public Sql_alloc
{
public:
  
  struct st_position *positions;

  uint tables; 

  
  double rows;

  
  Cost_estimate materialization_cost;

  
  Cost_estimate lookup_cost;
  
  
  Cost_estimate scan_cost;

  
  
  
  bool is_used;
  
  bool materialized; 
  
  bool is_sj_scan; 
  
  
  TMP_TABLE_PARAM sjm_table_param;
  List<Item> sjm_table_cols;
  TABLE *table;

  
  struct st_table_ref *tab_ref;
  Item *in_equality; 

  Item *join_cond; 
  Copy_field *copy_field; 
};



struct SORT_FIELD_ATTR
{
  uint length;          
  uint suffix_length;   
};


struct SORT_FIELD: public SORT_FIELD_ATTR
{
  Field *field;				
  Item	*item;				
  bool reverse;				
};


typedef struct st_sort_buffer {
  uint index;					
  uint sort_orders;
  uint change_pos;				
  char **buff;
  SORT_FIELD *sortorder;
} SORT_BUFFER;



class Table_ident :public Sql_alloc
{
public:
  LEX_CSTRING db;
  LEX_CSTRING table;
  SELECT_LEX_UNIT *sel;
  inline Table_ident(THD *thd, const LEX_CSTRING *db_arg,
                     const LEX_CSTRING *table_arg,
		     bool force)
    :table(*table_arg), sel((SELECT_LEX_UNIT *)0)
  {
    if (!force && (thd->client_capabilities & CLIENT_NO_SCHEMA))
      db= null_clex_str;
    else
      db= *db_arg;
  }
  inline Table_ident(const LEX_CSTRING *table_arg)
    :table(*table_arg), sel((SELECT_LEX_UNIT *)0)
  {
    db= null_clex_str;
  }
  
  inline Table_ident(SELECT_LEX_UNIT *s) : sel(s)
  {
    
    db.str= empty_c_string;                    
    db.length= 0;
    table.str= internal_table_name;
    table.length=1;
  }
  bool is_derived_table() const { return MY_TEST(sel); }
  inline void change_db(LEX_CSTRING *db_name)
  {
    db= *db_name;
  }
  bool resolve_table_rowtype_ref(THD *thd, Row_definition_list &defs);
  bool append_to(THD *thd, String *to) const;
};


class Qualified_column_ident: public Table_ident
{
public:
  LEX_CSTRING m_column;
public:
  Qualified_column_ident(const LEX_CSTRING *column)
    :Table_ident(&null_clex_str),
    m_column(*column)
  { }
  Qualified_column_ident(const LEX_CSTRING *table, const LEX_CSTRING *column)
   :Table_ident(table),
    m_column(*column)
  { }
  Qualified_column_ident(THD *thd,
                         const LEX_CSTRING *db,
                         const LEX_CSTRING *table,
                         const LEX_CSTRING *column)
   :Table_ident(thd, db, table, false),
    m_column(*column)
  { }
  bool resolve_type_ref(THD *thd, Column_definition *def);
  bool append_to(THD *thd, String *to) const;
};



class user_var_entry
{
  CHARSET_INFO *m_charset;
 public:
  user_var_entry() {}                         
  LEX_CSTRING name;
  char *value;
  size_t length;
  query_id_t update_query_id, used_query_id;
  Item_result type;
  bool unsigned_flag;

  double val_real(bool *null_value);
  longlong val_int(bool *null_value) const;
  String *val_str(bool *null_value, String *str, uint decimals);
  my_decimal *val_decimal(bool *null_value, my_decimal *result);
  CHARSET_INFO *charset() const { return m_charset; }
  void set_charset(CHARSET_INFO *cs) { m_charset= cs; }
};

user_var_entry *get_variable(HASH *hash, LEX_CSTRING *name,
				    bool create_if_not_exists);

class SORT_INFO;
class multi_delete :public select_result_interceptor
{
  TABLE_LIST *delete_tables, *table_being_deleted;
  Unique **tempfiles;
  ha_rows deleted, found;
  uint num_of_tables;
  int error;
  bool do_delete;
  
  bool transactional_tables;
  
  bool normal_tables;
  bool delete_while_scanning;
  
  bool error_handled;

public:
  multi_delete(THD *thd_arg, TABLE_LIST *dt, uint num_of_tables);
  ~multi_delete();
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  int send_data(List<Item> &items);
  bool initialize_tables (JOIN *join);
  int do_deletes();
  int do_table_deletes(TABLE *table, SORT_INFO *sort_info, bool ignore);
  bool send_eof();
  inline ha_rows num_deleted() const { return deleted; }
  virtual void abort_result_set();
  void prepare_to_read_rows();
};


class multi_update :public select_result_interceptor
{
  TABLE_LIST *all_tables; 
  List<TABLE_LIST> *leaves;     
  TABLE_LIST *update_tables;
  TABLE **tmp_tables, *main_table, *table_to_update;
  TMP_TABLE_PARAM *tmp_table_param;
  ha_rows updated, found;
  List <Item> *fields, *values;
  List <Item> **fields_for_table, **values_for_table;
  uint table_count;
  
  List <TABLE> unupdated_check_opt_tables;
  Copy_field *copy_field;
  enum enum_duplicates handle_duplicates;
  bool do_update, trans_safe;
  
  bool transactional_tables;
  bool ignore;
  
  bool error_handled;
  
  
  bool prepared;

  
  ha_rows updated_sys_ver;

  bool has_vers_fields;

public:
  multi_update(THD *thd_arg, TABLE_LIST *ut, List<TABLE_LIST> *leaves_list,
	       List<Item> *fields, List<Item> *values,
	       enum_duplicates handle_duplicates, bool ignore);
  ~multi_update();
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  int send_data(List<Item> &items);
  bool initialize_tables (JOIN *join);
  int prepare2(JOIN *join);
  int  do_updates();
  bool send_eof();
  inline ha_rows num_found() const { return found; }
  inline ha_rows num_updated() const { return updated; }
  virtual void abort_result_set();
  void update_used_tables();
  void prepare_to_read_rows();
};

class my_var_sp;
class my_var : public Sql_alloc  {
public:
  const LEX_CSTRING name;
  enum type { SESSION_VAR, LOCAL_VAR, PARAM_VAR };
  type scope;
  my_var(const LEX_CSTRING *j, enum type s) : name(*j), scope(s) { }
  virtual ~my_var() {}
  virtual bool set(THD *thd, Item *val) = 0;
  virtual my_var_sp *get_my_var_sp() { return NULL; }
};

class my_var_sp: public my_var {
  const Sp_rcontext_handler *m_rcontext_handler;
  const Type_handler *m_type_handler;
public:
  uint offset;
  
  sp_head *sp;
  my_var_sp(const Sp_rcontext_handler *rcontext_handler,
            const LEX_CSTRING *j, uint o, const Type_handler *type_handler,
            sp_head *s)
    : my_var(j, LOCAL_VAR),
      m_rcontext_handler(rcontext_handler),
      m_type_handler(type_handler), offset(o), sp(s) { }
  ~my_var_sp() { }
  bool set(THD *thd, Item *val);
  my_var_sp *get_my_var_sp() { return this; }
  const Type_handler *type_handler() const { return m_type_handler; }
  sp_rcontext *get_rcontext(sp_rcontext *local_ctx) const;
};


class my_var_sp_row_field: public my_var_sp
{
  uint m_field_offset;
public:
  my_var_sp_row_field(const Sp_rcontext_handler *rcontext_handler,
                      const LEX_CSTRING *varname, const LEX_CSTRING *fieldname,
                      uint var_idx, uint field_idx, sp_head *s)
   :my_var_sp(rcontext_handler, varname, var_idx,
              &type_handler_double, s),
    m_field_offset(field_idx)
  { }
  bool set(THD *thd, Item *val);
};

class my_var_user: public my_var {
public:
  my_var_user(const LEX_CSTRING *j)
    : my_var(j, SESSION_VAR) { }
  ~my_var_user() { }
  bool set(THD *thd, Item *val);
};

class select_dumpvar :public select_result_interceptor {
  ha_rows row_count;
  my_var_sp *m_var_sp_row; 
  bool send_data_to_var_list(List<Item> &items);
public:
  List<my_var> var_list;
  select_dumpvar(THD *thd_arg)
   :select_result_interceptor(thd_arg), row_count(0), m_var_sp_row(NULL)
  { var_list.empty(); }
  ~select_dumpvar() {}
  int prepare(List<Item> &list, SELECT_LEX_UNIT *u);
  int send_data(List<Item> &items);
  bool send_eof();
  virtual bool check_simple_select() const;
  void cleanup();
};



#define CF_CHANGES_DATA           (1U << 0)
#define CF_REPORT_PROGRESS        (1U << 1)
#define CF_STATUS_COMMAND         (1U << 2)
#define CF_SHOW_TABLE_COMMAND     (1U << 3)
#define CF_WRITE_LOGS_COMMAND     (1U << 4)


#define CF_REEXECUTION_FRAGILE    (1U << 5)

#define CF_IMPLICT_COMMIT_BEGIN   (1U << 6)

#define CF_IMPLICIT_COMMIT_END    (1U << 7)

#define CF_AUTO_COMMIT_TRANS  (CF_IMPLICT_COMMIT_BEGIN | CF_IMPLICIT_COMMIT_END)


#define CF_DIAGNOSTIC_STMT        (1U << 8)


#define CF_CAN_GENERATE_ROW_EVENTS (1U << 9)


#define CF_PREOPEN_TMP_TABLES   (1U << 10)


#define CF_HA_CLOSE             (1U << 11)


#define CF_CAN_BE_EXPLAINED       (1U << 12)


#define CF_OPTIMIZER_TRACE        (1U << 14)


#define CF_DISALLOW_IN_RO_TRANS   (1U << 15)


#define CF_FORCE_ORIGINAL_BINLOG_FORMAT (1U << 16)


#define CF_INSERTS_DATA (1U << 17)


#define CF_UPDATES_DATA (1U << 18)


#define CF_ADMIN_COMMAND (1U << 19)


#define CF_SP_BULK_SAFE (1U << 20)

#define CF_SP_BULK_OPTIMIZED (1U << 21)

#define CF_SCHEMA_CHANGE (1U << 22)

#define CF_DB_CHANGE (1U << 23)




#define CF_SKIP_QUERY_ID        (1U << 0)


#define CF_SKIP_QUESTIONS       (1U << 1)
#ifdef WITH_WSREP

#define CF_SKIP_WSREP_CHECK     (1U << 2)
#else
#define CF_SKIP_WSREP_CHECK     0
#endif 


#define CF_NO_COM_MULTI         (1U << 3)



inline bool add_item_to_list(THD *thd, Item *item)
{
  return thd->lex->current_select->add_item_to_list(thd, item);
}

inline bool add_value_to_list(THD *thd, Item *value)
{
  return thd->lex->value_list.push_back(value, thd->mem_root);
}

inline bool add_order_to_list(THD *thd, Item *item, bool asc)
{
  return thd->lex->current_select->add_order_to_list(thd, item, asc);
}

inline bool add_gorder_to_list(THD *thd, Item *item, bool asc)
{
  return thd->lex->current_select->add_gorder_to_list(thd, item, asc);
}

inline bool add_group_to_list(THD *thd, Item *item, bool asc)
{
  return thd->lex->current_select->add_group_to_list(thd, item, asc);
}

inline Item *and_conds(THD *thd, Item *a, Item *b)
{
  if (!b) return a;
  if (!a) return b;
  return new (thd->mem_root) Item_cond_and(thd, a, b);
}


inline void handler::increment_statistics(ulong SSV::*offset) const
{
  status_var_increment(table->in_use->status_var.*offset);
  table->in_use->check_limit_rows_examined();
}

inline void handler::decrement_statistics(ulong SSV::*offset) const
{
  status_var_decrement(table->in_use->status_var.*offset);
}


inline int handler::ha_ft_read(uchar *buf)
{
  int error= ft_read(buf);
  if (!error)
    update_rows_read();

  table->status=error ? STATUS_NOT_FOUND: 0;
  return error;
}

inline int handler::ha_rnd_pos_by_record(uchar *buf)
{
  int error= rnd_pos_by_record(buf);
  table->status=error ? STATUS_NOT_FOUND: 0;
  return error;
}

inline int handler::ha_read_first_row(uchar *buf, uint primary_key)
{
  int error= read_first_row(buf, primary_key);
  if (!error)
    update_rows_read();
  table->status=error ? STATUS_NOT_FOUND: 0;
  return error;
}

inline int handler::ha_write_tmp_row(uchar *buf)
{
  int error;
  MYSQL_INSERT_ROW_START(table_share->db.str, table_share->table_name.str);
  increment_statistics(&SSV::ha_tmp_write_count);
  TABLE_IO_WAIT(tracker, m_psi, PSI_TABLE_WRITE_ROW, MAX_KEY, 0,
                { error= write_row(buf); })
  MYSQL_INSERT_ROW_DONE(error);
  return error;
}

inline int handler::ha_delete_tmp_row(uchar *buf)
{
  int error;
  MYSQL_DELETE_ROW_START(table_share->db.str, table_share->table_name.str);
  increment_statistics(&SSV::ha_tmp_delete_count);
  TABLE_IO_WAIT(tracker, m_psi, PSI_TABLE_DELETE_ROW, MAX_KEY, 0,
                { error= delete_row(buf); })
  MYSQL_DELETE_ROW_DONE(error);
  return error;
}

inline int handler::ha_update_tmp_row(const uchar *old_data, uchar *new_data)
{
  int error;
  MYSQL_UPDATE_ROW_START(table_share->db.str, table_share->table_name.str);
  increment_statistics(&SSV::ha_tmp_update_count);
  TABLE_IO_WAIT(tracker, m_psi, PSI_TABLE_UPDATE_ROW, active_index, 0,
                { error= update_row(old_data, new_data);})
  MYSQL_UPDATE_ROW_DONE(error);
  return error;
}


extern pthread_attr_t *get_connection_attrib(void);


void thd_enter_cond(MYSQL_THD thd, mysql_cond_t *cond, mysql_mutex_t *mutex,
                    const PSI_stage_info *stage, PSI_stage_info *old_stage,
                    const char *src_function, const char *src_file,
                    int src_line);

#define THD_ENTER_COND(P1, P2, P3, P4, P5) \
  thd_enter_cond(P1, P2, P3, P4, P5, __func__, __FILE__, __LINE__)


void thd_exit_cond(MYSQL_THD thd, const PSI_stage_info *stage,
                   const char *src_function, const char *src_file,
                   int src_line);

#define THD_EXIT_COND(P1, P2) \
  thd_exit_cond(P1, P2, __func__, __FILE__, __LINE__)

inline bool binlog_should_compress(size_t len)
{
  return opt_bin_log_compress &&
    len >= opt_bin_log_compress_min_len;
}



class Sql_mode_save
{
 public:
  Sql_mode_save(THD *thd) : thd(thd), old_mode(thd->variables.sql_mode) {}
  ~Sql_mode_save() { thd->variables.sql_mode = old_mode; }

 private:
  THD *thd;
  sql_mode_t old_mode; 
};



class Database_qualified_name
{
public:
  LEX_CSTRING m_db;
  LEX_CSTRING m_name;
  Database_qualified_name(const LEX_CSTRING *db, const LEX_CSTRING *name)
   :m_db(*db), m_name(*name)
  { }
  Database_qualified_name(const LEX_CSTRING &db, const LEX_CSTRING &name)
   :m_db(db), m_name(name)
  { }
  Database_qualified_name(const char *db, size_t db_length,
                          const char *name, size_t name_length)
  {
    m_db.str= db;
    m_db.length= db_length;
    m_name.str= name;
    m_name.length= name_length;
  }

  bool eq(const Database_qualified_name *other) const
  {
    CHARSET_INFO *cs= lower_case_table_names ?
                      &my_charset_utf8_general_ci :
                      &my_charset_utf8_bin;
    return
      m_db.length == other->m_db.length &&
      m_name.length == other->m_name.length &&
      !my_strnncoll(cs,
                    (const uchar *) m_db.str, m_db.length,
                    (const uchar *) other->m_db.str, other->m_db.length) &&
      !my_strnncoll(cs,
                    (const uchar *) m_name.str, m_name.length,
                    (const uchar *) other->m_name.str, other->m_name.length);
  }
  void copy(MEM_ROOT *mem_root, const LEX_CSTRING &db,
                                const LEX_CSTRING &name);
  
  size_t make_qname(char *dst, size_t dstlen) const
  {
    return my_snprintf(dst, dstlen, "%.*s.%.*s",
                       (int) m_db.length, m_db.str,
                       (int) m_name.length, m_name.str);
  }
  
  bool make_qname(MEM_ROOT *mem_root, LEX_CSTRING *dst) const
  {
    const uint dot= !!m_db.length;
    char *tmp;
    
    dst->length= m_db.length + dot + m_name.length;
    if (unlikely(!(dst->str= tmp= (char*) alloc_root(mem_root,
                                                     dst->length + 1))))
      return true;
    sprintf(tmp, "%.*s%.*s%.*s",
            (int) m_db.length, (m_db.length ? m_db.str : ""),
            dot, ".",
            (int) m_name.length, m_name.str);
    DBUG_SLOW_ASSERT(ok_for_lower_case_names(m_db.str));
    return false;
  }

  bool make_package_routine_name(MEM_ROOT *mem_root,
                                 const LEX_CSTRING &package,
                                 const LEX_CSTRING &routine)
  {
    char *tmp;
    size_t length= package.length + 1 + routine.length + 1;
    if (unlikely(!(tmp= (char *) alloc_root(mem_root, length))))
      return true;
    m_name.length= my_snprintf(tmp, length, "%.*s.%.*s",
                               (int) package.length, package.str,
                               (int) routine.length, routine.str);
    m_name.str= tmp;
    return false;
  }

  bool make_package_routine_name(MEM_ROOT *mem_root,
                                 const LEX_CSTRING &db,
                                 const LEX_CSTRING &package,
                                 const LEX_CSTRING &routine)
  {
    if (unlikely(make_package_routine_name(mem_root, package, routine)))
      return true;
    if (unlikely(!(m_db.str= strmake_root(mem_root, db.str, db.length))))
      return true;
    m_db.length= db.length;
    return false;
  }
};


class ErrConvDQName: public ErrConv
{
  const Database_qualified_name *m_name;
public:
  ErrConvDQName(const Database_qualified_name *name)
   :m_name(name)
  { }
  const char *ptr() const
  {
    m_name->make_qname(err_buffer, sizeof(err_buffer));
    return err_buffer;
  }
};

class Type_holder: public Sql_alloc,
                   public Item_args,
                   public Type_handler_hybrid_field_type,
                   public Type_all_attributes,
                   public Type_geometry_attributes
{
  TYPELIB *m_typelib;
  bool m_maybe_null;
public:
  Type_holder()
   :m_typelib(NULL),
    m_maybe_null(false)
  { }

  void set_maybe_null(bool maybe_null_arg) { m_maybe_null= maybe_null_arg; }
  bool get_maybe_null() const { return m_maybe_null; }

  uint decimal_precision() const
  {
    
    DBUG_ASSERT(0);
    return 0;
  }
  void set_geometry_type(uint type)
  {
    Type_geometry_attributes::set_geometry_type(type);
  }
  uint uint_geometry_type() const
  {
    return Type_geometry_attributes::get_geometry_type();
  }
  void set_typelib(TYPELIB *typelib)
  {
    m_typelib= typelib;
  }
  TYPELIB *get_typelib() const
  {
    return m_typelib;
  }

  bool aggregate_attributes(THD *thd)
  {
    for (uint i= 0; i < arg_count; i++)
      m_maybe_null|= args[i]->maybe_null;
    return
       type_handler()->Item_hybrid_func_fix_attributes(thd,
                                                       "UNION", this, this,
                                                       args, arg_count);
  }
};



class Sp_eval_expr_state
{
  THD *m_thd;
  enum_check_fields m_count_cuted_fields;
  bool m_abort_on_warning;
  bool m_stmt_modified_non_trans_table;
  void start()
  {
    m_thd->count_cuted_fields= CHECK_FIELD_ERROR_FOR_NULL;
    m_thd->abort_on_warning= m_thd->is_strict_mode();
    m_thd->transaction.stmt.modified_non_trans_table= false;
  }
  void stop()
  {
    m_thd->count_cuted_fields= m_count_cuted_fields;
    m_thd->abort_on_warning= m_abort_on_warning;
    m_thd->transaction.stmt.modified_non_trans_table=
      m_stmt_modified_non_trans_table;
  }
public:
  Sp_eval_expr_state(THD *thd)
   :m_thd(thd),
    m_count_cuted_fields(thd->count_cuted_fields),
    m_abort_on_warning(thd->abort_on_warning),
    m_stmt_modified_non_trans_table(thd->transaction.stmt.
                                    modified_non_trans_table)
  {
    start();
  }
  ~Sp_eval_expr_state()
  {
    stop();
  }
};


#ifndef DBUG_OFF
void dbug_serve_apcs(THD *thd, int n_calls);
#endif 

class ScopedStatementReplication
{
public:
  ScopedStatementReplication(THD *thd) : thd(thd)
  {
    if (thd)
      saved_binlog_format= thd->set_current_stmt_binlog_format_stmt();
  }
  ~ScopedStatementReplication()
  {
    if (thd)
      thd->restore_stmt_binlog_format(saved_binlog_format);
  }

private:
  enum_binlog_format saved_binlog_format;
  THD *thd;
};

#endif 
#endif 
