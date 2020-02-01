#ifndef SET_VAR_INCLUDED
#define SET_VAR_INCLUDED




#ifdef USE_PRAGMA_INTERFACE
#pragma interface                       
#endif

#include <my_getopt.h>

class sys_var;
class set_var;
class sys_var_pluginvar;
class PolyLock;
class Item_func_set_user_var;


#include "item.h"                          
#include "sql_class.h"                     

extern TYPELIB bool_typelib;

struct sys_var_chain
{
  sys_var *first;
  sys_var *last;
};

int mysql_add_sys_var_chain(sys_var *chain);
int mysql_del_sys_var_chain(sys_var *chain);



class sys_var: protected Value_source 
{
public:
  sys_var *next;
  LEX_CSTRING name;
  bool *test_load;
  enum flag_enum { GLOBAL, SESSION, ONLY_SESSION, SCOPE_MASK=1023,
                   READONLY=1024, ALLOCATED=2048, PARSE_EARLY=4096,
                   NO_SET_STATEMENT=8192, AUTO_SET=16384};
  enum { NO_GETOPT=-1, GETOPT_ONLY_HELP=-2 };
  enum where { CONFIG, AUTO, SQL, COMPILE_TIME, ENV };

      
  enum binlog_status_enum { VARIABLE_NOT_IN_BINLOG,
                            SESSION_VARIABLE_IN_BINLOG } binlog_status;

  my_option option;     
  enum where value_origin;

protected:
  typedef bool (*on_check_function)(sys_var *self, THD *thd, set_var *var);
  typedef bool (*on_update_function)(sys_var *self, THD *thd, enum_var_type type);

  int flags;            
  const SHOW_TYPE show_val_type; 
  PolyLock *guard;      
  ptrdiff_t offset;     
  on_check_function on_check;
  on_update_function on_update;
  const char *const deprecation_substitute;
  bool is_os_charset; 

public:
  sys_var(sys_var_chain *chain, const char *name_arg, const char *comment,
          int flag_args, ptrdiff_t off, int getopt_id,
          enum get_opt_arg_type getopt_arg_type, SHOW_TYPE show_val_type_arg,
          longlong def_val, PolyLock *lock, enum binlog_status_enum binlog_status_arg,
          on_check_function on_check_func, on_update_function on_update_func,
          const char *substitute);

  virtual ~sys_var() {}

  
  virtual void cleanup() {}
  
  virtual sys_var_pluginvar *cast_pluginvar() { return 0; }

  bool check(THD *thd, set_var *var);
  uchar *value_ptr(THD *thd, enum_var_type type, const LEX_CSTRING *base);

  
  bool set_default(THD *thd, set_var *var);
  bool update(THD *thd, set_var *var);

  String *val_str_nolock(String *str, THD *thd, const uchar *value);
  longlong val_int(bool *is_null, THD *thd, enum_var_type type, const LEX_CSTRING *base);
  String *val_str(String *str, THD *thd, enum_var_type type, const LEX_CSTRING *base);
  double val_real(bool *is_null, THD *thd, enum_var_type type, const LEX_CSTRING *base);

  SHOW_TYPE show_type() { return show_val_type; }
  int scope() const { return flags & SCOPE_MASK; }
  CHARSET_INFO *charset(THD *thd);
  bool is_readonly() const { return flags & READONLY; }
  
  bool is_struct() { return option.var_type & GET_ASK_ADDR; }
  bool is_set_stmt_ok() const { return !(flags & NO_SET_STATEMENT); }
  bool is_written_to_binlog(enum_var_type type)
  { return type != OPT_GLOBAL && binlog_status == SESSION_VARIABLE_IN_BINLOG; }
  bool check_update_type(const Item *item)
  {
    Item_result type= item->result_type();
    switch (option.var_type & GET_TYPE_MASK) {
    case GET_INT:
    case GET_UINT:
    case GET_LONG:
    case GET_ULONG:
    case GET_LL:
    case GET_ULL:
      return type != INT_RESULT &&
             (type != DECIMAL_RESULT || item->decimals != 0);
    case GET_STR:
    case GET_STR_ALLOC:
      return type != STRING_RESULT;
    case GET_ENUM:
    case GET_BOOL:
    case GET_SET:
    case GET_FLAGSET:
    case GET_BIT:
      return type != STRING_RESULT && type != INT_RESULT;
    case GET_DOUBLE:
      return type != INT_RESULT && type != REAL_RESULT && type != DECIMAL_RESULT;
    default:
      return true;
    }
  }

  bool check_type(enum_var_type type)
  {
    switch (scope())
    {
    case GLOBAL:       return type != OPT_GLOBAL;
    case SESSION:      return false; 
    case ONLY_SESSION: return type == OPT_GLOBAL;
    }
    return true; 
  }
  bool register_option(DYNAMIC_ARRAY *array, int parse_flags)
  {
    DBUG_ASSERT(parse_flags == GETOPT_ONLY_HELP ||
                parse_flags == PARSE_EARLY || parse_flags == 0);
    if (option.id == NO_GETOPT)
      return 0;
    if (parse_flags == GETOPT_ONLY_HELP)
    {
      if (option.id != GETOPT_ONLY_HELP)
        return 0;
    }
    else
    {
      if (option.id == GETOPT_ONLY_HELP)
        return 0;
      if ((flags & PARSE_EARLY) != parse_flags)
        return 0;
    }
    return insert_dynamic(array, (uchar*)&option);
  }
  void do_deprecated_warning(THD *thd);
  
  virtual bool session_is_default(THD *thd) { return false; }

  virtual uchar *default_value_ptr(THD *thd)
  { return (uchar*)&option.def_value; }

private:
  virtual bool do_check(THD *thd, set_var *var) = 0;
  
  virtual void session_save_default(THD *thd, set_var *var) = 0;
  
  virtual void global_save_default(THD *thd, set_var *var) = 0;
  virtual bool session_update(THD *thd, set_var *var) = 0;
  virtual bool global_update(THD *thd, set_var *var) = 0;

protected:
  
  virtual uchar *session_value_ptr(THD *thd, const LEX_CSTRING *base);
  virtual uchar *global_value_ptr(THD *thd, const LEX_CSTRING *base);

  
  uchar *session_var_ptr(THD *thd)
  { return ((uchar*)&(thd->variables)) + offset; }

  uchar *global_var_ptr()
  { return ((uchar*)&global_system_variables) + offset; }

  void *max_var_ptr()
  {
    return scope() == SESSION ? (((uchar*)&max_system_variables) + offset) :
                                0;
  }

  friend class Session_sysvars_tracker;
  friend class Session_tracker;
};

#include "sql_plugin.h"                    





class set_var_base :public Sql_alloc
{
public:
  set_var_base() {}
  virtual ~set_var_base() {}
  virtual int check(THD *thd)=0;           
  virtual int update(THD *thd)=0;                  
  virtual int light_check(THD *thd) { return check(thd); }   
  virtual bool is_system() { return FALSE; }
};



class set_var :public set_var_base
{
public:
  sys_var *var; 
  Item *value;  
  enum_var_type type;
  union 
  {
    ulonglong ulonglong_value;          
    longlong longlong_value;            
    double double_value;                
    plugin_ref plugin;                  
    plugin_ref *plugins;                
    Time_zone *time_zone;               
    LEX_STRING string_value;            
    const void *ptr;                    
  } save_result;
  LEX_CSTRING base; 

  set_var(THD *thd, enum_var_type type_arg, sys_var *var_arg,
          const LEX_CSTRING *base_name_arg, Item *value_arg);
  virtual bool is_system() { return 1; }
  int check(THD *thd);
  int update(THD *thd);
  int light_check(THD *thd);
};



class set_var_user: public set_var_base
{
  Item_func_set_user_var *user_var_item;
public:
  set_var_user(Item_func_set_user_var *item)
    :user_var_item(item)
  {}
  int check(THD *thd);
  int update(THD *thd);
  int light_check(THD *thd);
};



class set_var_password: public set_var_base
{
  LEX_USER *user;
public:
  set_var_password(LEX_USER *user_arg) :user(user_arg)
  {}
  int check(THD *thd);
  int update(THD *thd);
};



class set_var_role: public set_var_base
{
  LEX_CSTRING role;
  ulonglong access;
public:
  set_var_role(LEX_CSTRING role_arg) : role(role_arg) {}
  int check(THD *thd);
  int update(THD *thd);
};



class set_var_default_role: public set_var_base
{
  LEX_USER *user, *real_user;
  LEX_CSTRING role;
public:
  set_var_default_role(LEX_USER *user_arg, LEX_CSTRING role_arg) :
    user(user_arg), role(role_arg) {}
  int check(THD *thd);
  int update(THD *thd);
};



class set_var_collation_client: public set_var_base
{
  CHARSET_INFO *character_set_client;
  CHARSET_INFO *character_set_results;
  CHARSET_INFO *collation_connection;
public:
  set_var_collation_client(CHARSET_INFO *client_coll_arg,
                           CHARSET_INFO *connection_coll_arg,
                           CHARSET_INFO *result_coll_arg)
    :character_set_client(client_coll_arg),
     character_set_results(result_coll_arg),
     collation_connection(connection_coll_arg)
  {}
  int check(THD *thd);
  int update(THD *thd);
};



extern SHOW_COMP_OPTION have_csv, have_innodb;
extern SHOW_COMP_OPTION have_ndbcluster, have_partitioning;
extern SHOW_COMP_OPTION have_profiling;

extern SHOW_COMP_OPTION have_ssl, have_symlink, have_dlopen;
extern SHOW_COMP_OPTION have_query_cache;
extern SHOW_COMP_OPTION have_geometry, have_rtree_keys;
extern SHOW_COMP_OPTION have_crypt;
extern SHOW_COMP_OPTION have_compress;
extern SHOW_COMP_OPTION have_openssl;



SHOW_VAR* enumerate_sys_vars(THD *thd, bool sorted, enum enum_var_type type);
int fill_sysvars(THD *thd, TABLE_LIST *tables, COND *cond);

sys_var *find_sys_var(THD *thd, const char *str, size_t length=0);
int sql_set_variables(THD *thd, List<set_var_base> *var_list, bool free);

#define SYSVAR_AUTOSIZE(VAR,VAL)                        \
  do {                                                  \
    VAR= (VAL);                                         \
    set_sys_var_value_origin(&VAR, sys_var::AUTO);      \
  } while(0)

#define SYSVAR_AUTOSIZE_IF_CHANGED(VAR,VAL,TYPE)        \
  do {                                                  \
    TYPE tmp= (VAL);                                    \
    if (VAR != tmp)                                     \
    {                                                   \
      VAR= (VAL);                                       \
      set_sys_var_value_origin(&VAR, sys_var::AUTO);    \
    }                                                   \
  } while(0)

void set_sys_var_value_origin(void *ptr, enum sys_var::where here);

enum sys_var::where get_sys_var_value_origin(void *ptr);
inline bool IS_SYSVAR_AUTOSIZE(void *ptr)
{
  enum sys_var::where res= get_sys_var_value_origin(ptr);
  return (res == sys_var::AUTO || res == sys_var::COMPILE_TIME);
}

bool fix_delay_key_write(sys_var *self, THD *thd, enum_var_type type);

sql_mode_t expand_sql_mode(sql_mode_t sql_mode);
bool sql_mode_string_representation(THD *thd, sql_mode_t sql_mode,
                                    LEX_CSTRING *ls);
int default_regex_flags_pcre(const THD *thd);

extern sys_var *Sys_autocommit_ptr, *Sys_last_gtid_ptr,
  *Sys_character_set_client_ptr, *Sys_character_set_connection_ptr,
  *Sys_character_set_results_ptr;

CHARSET_INFO *get_old_charset_by_name(const char *old_name);

int sys_var_init();
uint sys_var_elements();
int sys_var_add_options(DYNAMIC_ARRAY *long_options, int parse_flags);
void sys_var_end(void);
bool check_has_super(sys_var *self, THD *thd, set_var *var);
plugin_ref *resolve_engine_list(THD *thd, const char *str_arg, size_t str_arg_len,
                                bool error_on_unknown_engine, bool temp_copy);
void free_engine_list(plugin_ref *list);
plugin_ref *copy_engine_list(plugin_ref *list);
plugin_ref *temp_copy_engine_list(THD *thd, plugin_ref *list);
char *pretty_print_engine_list(THD *thd, plugin_ref *list);

#endif
