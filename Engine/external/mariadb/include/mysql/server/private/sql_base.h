

#ifndef SQL_BASE_INCLUDED
#define SQL_BASE_INCLUDED

#include "sql_class.h"                          
#include "sql_trigger.h"                        
#include "mysqld.h"                             
#include "table_cache.h"

class Item_ident;
struct Name_resolution_context;
class Open_table_context;
class Open_tables_state;
class Prelocking_strategy;
struct TABLE_LIST;
class THD;
struct handlerton;
struct TABLE;

typedef class st_select_lex SELECT_LEX;

typedef struct st_lock_param_type ALTER_PARTITION_PARAM_TYPE;


enum enum_resolution_type {
  NOT_RESOLVED=0,
  RESOLVED_IGNORING_ALIAS,
  RESOLVED_BEHIND_ALIAS,
  RESOLVED_WITH_NO_ALIAS,
  RESOLVED_AGAINST_ALIAS
};

enum find_item_error_report_type {REPORT_ALL_ERRORS, REPORT_EXCEPT_NOT_FOUND,
				  IGNORE_ERRORS, REPORT_EXCEPT_NON_UNIQUE,
                                  IGNORE_EXCEPT_NON_UNIQUE};


#define CHECK_DUP_ALLOW_DIFFERENT_ALIAS 1
#define CHECK_DUP_FOR_CREATE 2
#define CHECK_DUP_SKIP_TEMP_TABLE 4

uint get_table_def_key(const TABLE_LIST *table_list, const char **key);
TABLE *open_ltable(THD *thd, TABLE_LIST *table_list, thr_lock_type update,
                   uint lock_flags);


#define MYSQL_OPEN_IGNORE_GLOBAL_READ_LOCK      0x0001
#define MYSQL_OPEN_IGNORE_FLUSH                 0x0002

#define MYSQL_LOCK_IGNORE_GLOBAL_READ_ONLY      0x0008
#define MYSQL_LOCK_LOG_TABLE                    0x0010

#define MYSQL_OPEN_HAS_MDL_LOCK                 0x0020

#define MYSQL_OPEN_GET_NEW_TABLE                0x0040


#define MYSQL_OPEN_FAIL_ON_MDL_CONFLICT         0x0100

#define MYSQL_OPEN_FORCE_SHARED_MDL             0x0200

#define MYSQL_OPEN_FORCE_SHARED_HIGH_PRIO_MDL   0x0400

#define MYSQL_LOCK_IGNORE_TIMEOUT               0x0800

#define MYSQL_OPEN_SKIP_SCOPED_MDL_LOCK         0x1000
#define MYSQL_LOCK_NOT_TEMPORARY		0x2000
#define MYSQL_LOCK_USE_MALLOC                   0x4000

#define MYSQL_OPEN_IGNORE_KILLED                0x8000

#define MYSQL_OPEN_IGNORE_REPAIR                0x10000


#define MYSQL_OPEN_REOPEN  (MYSQL_OPEN_IGNORE_FLUSH |\
                            MYSQL_OPEN_IGNORE_GLOBAL_READ_LOCK |\
                            MYSQL_LOCK_IGNORE_GLOBAL_READ_ONLY |\
                            MYSQL_LOCK_IGNORE_TIMEOUT |\
                            MYSQL_OPEN_GET_NEW_TABLE |\
                            MYSQL_OPEN_HAS_MDL_LOCK)

bool is_locked_view(THD *thd, TABLE_LIST *t);
bool open_table(THD *thd, TABLE_LIST *table_list, Open_table_context *ot_ctx);

bool get_key_map_from_key_list(key_map *map, TABLE *table,
                               List<String> *index_list);
TABLE *find_locked_table(TABLE *list, const char *db, const char *table_name);
TABLE *find_write_locked_table(TABLE *list, const char *db,
                               const char *table_name);
thr_lock_type read_lock_type_for_table(THD *thd,
                                       Query_tables_list *prelocking_ctx,
                                       TABLE_LIST *table_list,
                                       bool routine_modifies_data);

my_bool mysql_rm_tmp_tables(void);
void close_tables_for_reopen(THD *thd, TABLE_LIST **tables,
                             const MDL_savepoint &start_of_statement_svp);
bool table_already_fk_prelocked(TABLE_LIST *tl, LEX_CSTRING *db,
                                LEX_CSTRING *table, thr_lock_type lock_type);
TABLE_LIST *find_table_in_list(TABLE_LIST *table,
                               TABLE_LIST *TABLE_LIST::*link,
                               const LEX_CSTRING *db_name,
                               const LEX_CSTRING *table_name);
void close_thread_tables(THD *thd);
void switch_to_nullable_trigger_fields(List<Item> &items, TABLE *);
void switch_defaults_to_nullable_trigger_fields(TABLE *table);
bool fill_record_n_invoke_before_triggers(THD *thd, TABLE *table,
                                          List<Item> &fields,
                                          List<Item> &values,
                                          bool ignore_errors,
                                          enum trg_event_type event);
bool fill_record_n_invoke_before_triggers(THD *thd, TABLE *table,
                                          Field **field,
                                          List<Item> &values,
                                          bool ignore_errors,
                                          enum trg_event_type event);
bool insert_fields(THD *thd, Name_resolution_context *context,
		   const char *db_name, const char *table_name,
                   List_iterator<Item> *it, bool any_privileges,
                   uint *hidden_bit_fields);
void make_leaves_list(THD *thd, List<TABLE_LIST> &list, TABLE_LIST *tables,
                      bool full_table_list, TABLE_LIST *boundary);
int setup_wild(THD *thd, TABLE_LIST *tables, List<Item> &fields,
	       List<Item> *sum_func_list, uint wild_num, uint * hidden_bit_fields);
bool setup_fields(THD *thd, Ref_ptr_array ref_pointer_array,
                  List<Item> &item, enum_column_usage column_usage,
                  List<Item> *sum_func_list, List<Item> *pre_fix,
                  bool allow_sum_func);
void unfix_fields(List<Item> &items);
bool fill_record(THD * thd, TABLE *table_arg, List<Item> &fields,
                 List<Item> &values, bool ignore_errors, bool update);
bool fill_record(THD *thd, TABLE *table, Field **field, List<Item> &values,
                 bool ignore_errors, bool use_value);

Field *
find_field_in_tables(THD *thd, Item_ident *item,
                     TABLE_LIST *first_table, TABLE_LIST *last_table,
                     Item **ref, find_item_error_report_type report_error,
                     bool check_privileges, bool register_tree_change);
Field *
find_field_in_table_ref(THD *thd, TABLE_LIST *table_list,
                        const char *name, size_t length,
                        const char *item_name, const char *db_name,
                        const char *table_name, Item **ref,
                        bool check_privileges, bool allow_rowid,
                        uint *cached_field_index_ptr,
                        bool register_tree_change, TABLE_LIST **actual_table);
Field *
find_field_in_table(THD *thd, TABLE *table, const char *name, size_t length,
                    bool allow_rowid, uint *cached_field_index_ptr);
Field *
find_field_in_table_sef(TABLE *table, const char *name);
Item ** find_item_in_list(Item *item, List<Item> &items, uint *counter,
                          find_item_error_report_type report_error,
                          enum_resolution_type *resolution, uint limit= 0);
bool setup_tables(THD *thd, Name_resolution_context *context,
                  List<TABLE_LIST> *from_clause, TABLE_LIST *tables,
                  List<TABLE_LIST> &leaves, bool select_insert,
                  bool full_table_list);
bool setup_tables_and_check_access(THD *thd,
                                   Name_resolution_context *context,
                                   List<TABLE_LIST> *from_clause,
                                   TABLE_LIST *tables,
                                   List<TABLE_LIST> &leaves, 
                                   bool select_insert,
                                   ulong want_access_first,
                                   ulong want_access,
                                   bool full_table_list);
bool wait_while_table_is_used(THD *thd, TABLE *table,
                              enum ha_extra_function function);

void drop_open_table(THD *thd, TABLE *table, const LEX_CSTRING *db_name,
                     const LEX_CSTRING *table_name);
void update_non_unique_table_error(TABLE_LIST *update,
                                   const char *operation,
                                   TABLE_LIST *duplicate);
int setup_conds(THD *thd, TABLE_LIST *tables, List<TABLE_LIST> &leaves,
		COND **conds);
void wrap_ident(THD *thd, Item **conds);
int setup_ftfuncs(SELECT_LEX* select);
void cleanup_ftfuncs(SELECT_LEX *select_lex);
int init_ftfuncs(THD *thd, SELECT_LEX* select, bool no_order);
bool lock_table_names(THD *thd, const DDL_options_st &options,
                      TABLE_LIST *table_list,
                      TABLE_LIST *table_list_end, ulong lock_wait_timeout,
                      uint flags);
static inline bool
lock_table_names(THD *thd, TABLE_LIST *table_list,
                 TABLE_LIST *table_list_end, ulong lock_wait_timeout,
                 uint flags)
{
  return lock_table_names(thd, thd->lex->create_info, table_list,
                          table_list_end, lock_wait_timeout, flags);
}
bool open_tables(THD *thd, const DDL_options_st &options,
                 TABLE_LIST **tables, uint *counter, uint flags,
                 Prelocking_strategy *prelocking_strategy);
static inline bool
open_tables(THD *thd, TABLE_LIST **tables, uint *counter, uint flags,
            Prelocking_strategy *prelocking_strategy)
{
  return open_tables(thd, thd->lex->create_info, tables, counter, flags,
                     prelocking_strategy);
}

bool open_and_lock_tables(THD *thd, const DDL_options_st &options,
                          TABLE_LIST *tables,
                          bool derived, uint flags,
                          Prelocking_strategy *prelocking_strategy);
static inline bool
open_and_lock_tables(THD *thd, TABLE_LIST *tables,
                     bool derived, uint flags,
                     Prelocking_strategy *prelocking_strategy)
{
  return open_and_lock_tables(thd, thd->lex->create_info,
                              tables, derived, flags, prelocking_strategy);
}

TABLE *open_n_lock_single_table(THD *thd, TABLE_LIST *table_l,
                                thr_lock_type lock_type, uint flags,
                                Prelocking_strategy *prelocking_strategy);
bool open_normal_and_derived_tables(THD *thd, TABLE_LIST *tables, uint flags,
                                    uint dt_phases);
bool open_tables_only_view_structure(THD *thd, TABLE_LIST *tables,
                                     bool can_deadlock);
bool open_and_lock_internal_tables(TABLE *table, bool lock);
bool lock_tables(THD *thd, TABLE_LIST *tables, uint counter, uint flags);
int decide_logging_format(THD *thd, TABLE_LIST *tables);
void close_thread_table(THD *thd, TABLE **table_ptr);
TABLE_LIST *unique_table(THD *thd, TABLE_LIST *table, TABLE_LIST *table_list,
                         uint check_flag);
bool is_equal(const LEX_CSTRING *a, const LEX_CSTRING *b);

class Open_tables_backup;

bool open_system_tables_for_read(THD *thd, TABLE_LIST *table_list,
                                 Open_tables_backup *backup);
void close_system_tables(THD *thd, Open_tables_backup *backup);
void close_mysql_tables(THD *thd);
TABLE *open_system_table_for_update(THD *thd, TABLE_LIST *one_table);
TABLE *open_log_table(THD *thd, TABLE_LIST *one_table, Open_tables_backup *backup);
void close_log_table(THD *thd, Open_tables_backup *backup);

TABLE *open_performance_schema_table(THD *thd, TABLE_LIST *one_table,
                                     Open_tables_state *backup);
void close_performance_schema_table(THD *thd, Open_tables_state *backup);

bool close_cached_tables(THD *thd, TABLE_LIST *tables,
                         bool wait_for_refresh, ulong timeout);
bool close_cached_connection_tables(THD *thd, LEX_CSTRING *connect_string);
void close_all_tables_for_name(THD *thd, TABLE_SHARE *share,
                               ha_extra_function extra,
                               TABLE *skip_table);
OPEN_TABLE_LIST *list_open_tables(THD *thd, const char *db, const char *wild);
bool tdc_open_view(THD *thd, TABLE_LIST *table_list, uint flags);

TABLE *find_table_for_mdl_upgrade(THD *thd, const char *db,
                                  const char *table_name,
                                  int *p_error);
void mark_tmp_table_for_reuse(TABLE *table);

int dynamic_column_error_message(enum_dyncol_func_result rc);


int open_and_lock_tables_derived(THD *thd, TABLE_LIST *tables, bool derived);

extern "C" int simple_raw_key_cmp(void* arg, const void* key1,
                                  const void* key2);
extern "C" int count_distinct_walk(void *elem, element_count count, void *arg);
int simple_str_key_cmp(void* arg, uchar* key1, uchar* key2);

extern Item **not_found_item;
extern Field *not_found_field;
extern Field *view_ref_found;




inline void setup_table_map(TABLE *table, TABLE_LIST *table_list, uint tablenr)
{
  table->used_fields= 0;
  table_list->reset_const_table();
  table->null_row= 0;
  table->status= STATUS_NO_RECORD;
  table->maybe_null= table_list->outer_join;
  TABLE_LIST *embedding= table_list->embedding;
  while (!table->maybe_null && embedding)
  {
    table->maybe_null= embedding->outer_join;
    embedding= embedding->embedding;
  }
  table->tablenr= tablenr;
  table->map= (table_map) 1 << tablenr;
  table->force_index= table_list->force_index;
  table->force_index_order= table->force_index_group= 0;
  table->covering_keys= table->s->keys_for_keyread;
  TABLE_LIST *orig= table_list->select_lex ?
    table_list->select_lex->master_unit()->derived : 0;
  if (!orig || !orig->is_merged_derived())
  {
    
    table->covering_keys= table->s->keys_for_keyread;
  }
}

inline TABLE_LIST *find_table_in_global_list(TABLE_LIST *table,
                                             LEX_CSTRING *db_name,
                                             LEX_CSTRING *table_name)
{
  return find_table_in_list(table, &TABLE_LIST::next_global,
                            db_name, table_name);
}

inline bool setup_fields_with_no_wrap(THD *thd, Ref_ptr_array ref_pointer_array,
                                      List<Item> &item,
                                      enum_column_usage column_usage,
                                      List<Item> *sum_func_list,
                                      bool allow_sum_func)
{
  bool res;
  thd->lex->select_lex.no_wrap_view_item= TRUE;
  res= setup_fields(thd, ref_pointer_array, item, column_usage,
                    sum_func_list, NULL,  allow_sum_func);
  thd->lex->select_lex.no_wrap_view_item= FALSE;
  return res;
}



class Prelocking_strategy
{
public:
  virtual ~Prelocking_strategy() { }

  virtual bool handle_routine(THD *thd, Query_tables_list *prelocking_ctx,
                              Sroutine_hash_entry *rt, sp_head *sp,
                              bool *need_prelocking) = 0;
  virtual bool handle_table(THD *thd, Query_tables_list *prelocking_ctx,
                            TABLE_LIST *table_list, bool *need_prelocking) = 0;
  virtual bool handle_view(THD *thd, Query_tables_list *prelocking_ctx,
                           TABLE_LIST *table_list, bool *need_prelocking)= 0;
};




class DML_prelocking_strategy : public Prelocking_strategy
{
public:
  virtual bool handle_routine(THD *thd, Query_tables_list *prelocking_ctx,
                              Sroutine_hash_entry *rt, sp_head *sp,
                              bool *need_prelocking);
  virtual bool handle_table(THD *thd, Query_tables_list *prelocking_ctx,
                            TABLE_LIST *table_list, bool *need_prelocking);
  virtual bool handle_view(THD *thd, Query_tables_list *prelocking_ctx,
                           TABLE_LIST *table_list, bool *need_prelocking);
};




class Lock_tables_prelocking_strategy : public DML_prelocking_strategy
{
  virtual bool handle_table(THD *thd, Query_tables_list *prelocking_ctx,
                            TABLE_LIST *table_list, bool *need_prelocking);
};




class Alter_table_prelocking_strategy : public Prelocking_strategy
{
public:
  virtual bool handle_routine(THD *thd, Query_tables_list *prelocking_ctx,
                              Sroutine_hash_entry *rt, sp_head *sp,
                              bool *need_prelocking);
  virtual bool handle_table(THD *thd, Query_tables_list *prelocking_ctx,
                            TABLE_LIST *table_list, bool *need_prelocking);
  virtual bool handle_view(THD *thd, Query_tables_list *prelocking_ctx,
                           TABLE_LIST *table_list, bool *need_prelocking);
};


inline bool
open_tables(THD *thd, const DDL_options_st &options,
            TABLE_LIST **tables, uint *counter, uint flags)
{
  DML_prelocking_strategy prelocking_strategy;

  return open_tables(thd, options, tables, counter, flags,
                     &prelocking_strategy);
}
inline bool
open_tables(THD *thd, TABLE_LIST **tables, uint *counter, uint flags)
{
  DML_prelocking_strategy prelocking_strategy;

  return open_tables(thd, thd->lex->create_info, tables, counter, flags,
                     &prelocking_strategy);
}

inline TABLE *open_n_lock_single_table(THD *thd, TABLE_LIST *table_l,
                                       thr_lock_type lock_type, uint flags)
{
  DML_prelocking_strategy prelocking_strategy;

  return open_n_lock_single_table(thd, table_l, lock_type, flags,
                                  &prelocking_strategy);
}



inline bool open_and_lock_tables(THD *thd,
                                 const DDL_options_st &options,
                                 TABLE_LIST *tables,
                                 bool derived, uint flags)
{
  DML_prelocking_strategy prelocking_strategy;

  return open_and_lock_tables(thd, options, tables, derived, flags,
                              &prelocking_strategy);
}
inline bool open_and_lock_tables(THD *thd, TABLE_LIST *tables,
                                  bool derived, uint flags)
{
  DML_prelocking_strategy prelocking_strategy;

  return open_and_lock_tables(thd, thd->lex->create_info,
                              tables, derived, flags,
                              &prelocking_strategy);
}


bool restart_trans_for_tables(THD *thd, TABLE_LIST *table);



class Open_table_context
{
public:
  enum enum_open_table_action
  {
    OT_NO_ACTION= 0,
    OT_BACKOFF_AND_RETRY,
    OT_REOPEN_TABLES,
    OT_DISCOVER,
    OT_REPAIR
  };
  Open_table_context(THD *thd, uint flags);

  bool recover_from_failed_open();
  bool request_backoff_action(enum_open_table_action action_arg,
                              TABLE_LIST *table);

  bool can_recover_from_failed_open() const
  { return m_action != OT_NO_ACTION; }

  
  const MDL_savepoint &start_of_statement_svp() const
  {
    return m_start_of_statement_svp;
  }

  inline ulong get_timeout() const
  {
    return m_timeout;
  }

  uint get_flags() const { return m_flags; }

  
  void set_has_protection_against_grl()
  {
    m_has_protection_against_grl= TRUE;
  }

  bool has_protection_against_grl() const
  {
    return m_has_protection_against_grl;
  }

private:
  
  THD *m_thd;
  
  TABLE_LIST *m_failed_table;
  MDL_savepoint m_start_of_statement_svp;
  
  ulong m_timeout;
  
  uint m_flags;
  
  enum enum_open_table_action m_action;
  
  bool m_has_locks;
  
  bool m_has_protection_against_grl;
};




inline bool is_temporary_table(TABLE_LIST *tl)
{
  if (tl->view || tl->schema_table)
    return FALSE;

  if (!tl->table)
    return FALSE;

  

  if (!tl->table->s)
    return FALSE;

  return tl->table->s->tmp_table != NO_TMP_TABLE;
}




class No_such_table_error_handler : public Internal_error_handler
{
public:
  No_such_table_error_handler()
    : m_handled_errors(0), m_unhandled_errors(0)
  {}

  bool handle_condition(THD *thd,
                        uint sql_errno,
                        const char* sqlstate,
                        Sql_condition::enum_warning_level *level,
                        const char* msg,
                        Sql_condition ** cond_hdl);

  
  bool safely_trapped_errors();

private:
  int m_handled_errors;
  int m_unhandled_errors;
};


#endif 
