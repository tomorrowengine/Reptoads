

#ifndef SQL_ERROR_H
#define SQL_ERROR_H

#include "sql_list.h" 	
#include "sql_type_int.h" 
#include "sql_string.h"                        
#include "sql_plist.h" 
#include "mysql_com.h" 
#include "my_time.h"   
#include "decimal.h"

class THD;
class my_decimal;
class sp_condition_value;



class Sql_state
{
protected:
  
  char m_sqlstate[SQLSTATE_LENGTH + 1];
public:
  Sql_state()
  {
    memset(m_sqlstate, 0, sizeof(m_sqlstate));
  }

  Sql_state(const char *sqlstate)
  {
    set_sqlstate(sqlstate);
  }

  const char* get_sqlstate() const
  { return m_sqlstate; }

  void set_sqlstate(const Sql_state *other)
  {
    *this= *other;
  }
  void set_sqlstate(const char *sqlstate)
  {
    memcpy(m_sqlstate, sqlstate, SQLSTATE_LENGTH);
    m_sqlstate[SQLSTATE_LENGTH]= '\0';
  }
  bool eq(const Sql_state *other) const
  {
    return strcmp(m_sqlstate, other->m_sqlstate) == 0;
  }

  bool has_sql_state() const { return m_sqlstate[0] != '\0'; }

  
  inline bool is_warning() const
  { return m_sqlstate[0] == '0' && m_sqlstate[1] == '1'; }


  
  inline bool is_not_found() const
  { return m_sqlstate[0] == '0' && m_sqlstate[1] == '2'; }


  
  inline bool is_exception() const
  { return m_sqlstate[0] != '0' || m_sqlstate[1] > '2'; }

};


class Sql_state_errno: public Sql_state
{
protected:
  
  uint m_sql_errno;

public:
  Sql_state_errno()
   :m_sql_errno(0)
  { }
  Sql_state_errno(uint sql_errno)
   :m_sql_errno(sql_errno)
  { }
  Sql_state_errno(uint sql_errno, const char *sql_state)
   :Sql_state(sql_state),
    m_sql_errno(sql_errno)
  { }
  
  uint get_sql_errno() const
  { return m_sql_errno; }

  void set(uint sql_errno, const char *sqlstate)
  {
    m_sql_errno= sql_errno;
    set_sqlstate(sqlstate);
  }
  void clear()
  {
    m_sql_errno= 0;
  }
};


class Sql_state_errno_level: public Sql_state_errno
{
public:
  
  enum enum_warning_level
  { WARN_LEVEL_NOTE, WARN_LEVEL_WARN, WARN_LEVEL_ERROR, WARN_LEVEL_END};

protected:
  
  enum_warning_level m_level;

  void assign_defaults(const Sql_state_errno *value);

public:
  
  enum_warning_level get_level() const
  { return m_level; }

   Sql_state_errno_level()
    :m_level(WARN_LEVEL_ERROR)
  { }

  Sql_state_errno_level(uint sqlerrno, const char* sqlstate,
                         enum_warning_level level)
   :Sql_state_errno(sqlerrno, sqlstate),
    m_level(level)
  { }
  Sql_state_errno_level(const Sql_state_errno &state_errno,
                         enum_warning_level level)
   :Sql_state_errno(state_errno),
    m_level(level)
  { }
  void clear()
  {
    m_level= WARN_LEVEL_ERROR;
    Sql_state_errno::clear();
  }
};



class Sql_user_condition_identity
{
protected:
  const sp_condition_value *m_user_condition_value;
public:
  Sql_user_condition_identity()
   :m_user_condition_value(NULL)
  { }
  Sql_user_condition_identity(const sp_condition_value *value)
   :m_user_condition_value(value)
  { }
  const sp_condition_value *get_user_condition_value() const
  { return m_user_condition_value; }

  void set(const Sql_user_condition_identity &identity)
  {
    *this= identity;
  }
  void clear()
  {
    m_user_condition_value= NULL;
  }
};



class Sql_condition_identity: public Sql_state_errno_level,
                              public Sql_user_condition_identity
{
public:
  Sql_condition_identity()
  { }
  Sql_condition_identity(const Sql_state_errno_level &st,
                         const Sql_user_condition_identity &ucid)
   :Sql_state_errno_level(st),
    Sql_user_condition_identity(ucid)
  { }
  Sql_condition_identity(const Sql_state_errno &st,
                         enum_warning_level level,
                         const Sql_user_condition_identity &ucid)
   :Sql_state_errno_level(st, level),
    Sql_user_condition_identity(ucid)
  { }
  Sql_condition_identity(uint sqlerrno,
                         const char* sqlstate,
                         enum_warning_level level,
                         const Sql_user_condition_identity &ucid)
    :Sql_state_errno_level(sqlerrno, sqlstate, level),
     Sql_user_condition_identity(ucid)
  { }
  void clear()
  {
    Sql_state_errno_level::clear();
    Sql_user_condition_identity::clear();
  }
};


class Sql_condition_items
{
protected:
  
  String m_class_origin;

  
  String m_subclass_origin;

  
  String m_constraint_catalog;

  
  String m_constraint_schema;

  
  String m_constraint_name;

  
  String m_catalog_name;

  
  String m_schema_name;

  
  String m_table_name;

  
  String m_column_name;

  
  String m_cursor_name;

  Sql_condition_items()
   :m_class_origin((const char*) NULL, 0, & my_charset_utf8_bin),
    m_subclass_origin((const char*) NULL, 0, & my_charset_utf8_bin),
    m_constraint_catalog((const char*) NULL, 0, & my_charset_utf8_bin),
    m_constraint_schema((const char*) NULL, 0, & my_charset_utf8_bin),
    m_constraint_name((const char*) NULL, 0, & my_charset_utf8_bin),
    m_catalog_name((const char*) NULL, 0, & my_charset_utf8_bin),
    m_schema_name((const char*) NULL, 0, & my_charset_utf8_bin),
    m_table_name((const char*) NULL, 0, & my_charset_utf8_bin),
    m_column_name((const char*) NULL, 0, & my_charset_utf8_bin),
    m_cursor_name((const char*) NULL, 0, & my_charset_utf8_bin)
  { }

  void clear()
  {
    m_class_origin.length(0);
    m_subclass_origin.length(0);
    m_constraint_catalog.length(0);
    m_constraint_schema.length(0);
    m_constraint_name.length(0);
    m_catalog_name.length(0);
    m_schema_name.length(0);
    m_table_name.length(0);
    m_column_name.length(0);
    m_cursor_name.length(0);
  }
};



class Sql_condition : public Sql_alloc,
                      public Sql_condition_identity,
                      public Sql_condition_items
{
public:

  
  static enum_warning_level time_warn_level(uint warnings)
  {
    return MYSQL_TIME_WARN_HAVE_WARNINGS(warnings) ?
           WARN_LEVEL_WARN : WARN_LEVEL_NOTE;
  }

  
  const char* get_message_text() const;

  
  int get_message_octet_length() const;

private:
  
  friend class THD;
  friend class Warning_info;
  friend class Sql_cmd_common_signal;
  friend class Sql_cmd_signal;
  friend class Sql_cmd_resignal;
  friend class sp_rcontext;
  friend class Condition_information_item;

  
  Sql_condition()
   :m_mem_root(NULL)
  { }

  
  void init(MEM_ROOT *mem_root)
  {
    DBUG_ASSERT(mem_root != NULL);
    DBUG_ASSERT(m_mem_root == NULL);
    m_mem_root= mem_root;
  }

  
  Sql_condition(MEM_ROOT *mem_root)
   :m_mem_root(mem_root)
  {
    DBUG_ASSERT(mem_root != NULL);
  }

  Sql_condition(MEM_ROOT *mem_root, const Sql_user_condition_identity &ucid)
   :Sql_condition_identity(Sql_state_errno_level(), ucid),
    m_mem_root(mem_root)
  {
    DBUG_ASSERT(mem_root != NULL);
  }
  
  Sql_condition(MEM_ROOT *mem_root,
                const Sql_condition_identity &value,
                const char *msg)
   :Sql_condition_identity(value),
    m_mem_root(mem_root)
  {
    DBUG_ASSERT(mem_root != NULL);
    DBUG_ASSERT(value.get_sql_errno() != 0);
    DBUG_ASSERT(msg != NULL);
    set_builtin_message_text(msg);
  }

  
  ~Sql_condition()
  {}

  
  void copy_opt_attributes(const Sql_condition *cond);

  
  void set_builtin_message_text(const char* str);

  
  void set_class_origin();

  
  void set_subclass_origin();

  
  void assign_defaults(THD *thd, const Sql_state_errno *from);

  
  void clear()
  {
    Sql_condition_identity::clear();
    Sql_condition_items::clear();
    m_message_text.length(0);
  }

private:
  
  String m_message_text;

  
  Sql_condition *next_in_wi;
  Sql_condition **prev_in_wi;

  
  MEM_ROOT *m_mem_root;
};




class Warning_info
{
  
  typedef I_P_List<Sql_condition,
                   I_P_List_adapter<Sql_condition,
                                    &Sql_condition::next_in_wi,
                                    &Sql_condition::prev_in_wi>,
                   I_P_List_counter,
                   I_P_List_fast_push_back<Sql_condition> >
          Sql_condition_list;

  
  MEM_ROOT           m_warn_root;

  
  Sql_condition_list   m_warn_list;

  
  uint	             m_warn_count[(uint) Sql_condition::WARN_LEVEL_END];

  
  uint	             m_current_statement_warn_count;

  
  ulong              m_current_row_for_warning;

  
  ulonglong          m_warn_id;

  
  const Sql_condition *m_error_condition;

  
  bool               m_allow_unlimited_warnings;
  bool		     initialized;    

  
  bool m_read_only;

  
  Warning_info *m_next_in_da;
  Warning_info **m_prev_in_da;

  List<Sql_condition> m_marked_sql_conditions;

public:
  Warning_info(ulonglong warn_id_arg, bool allow_unlimited_warnings,
               bool initialized);
  ~Warning_info();
  
  void init();
  void free_memory();

private:
  Warning_info(const Warning_info &rhs); 
  Warning_info& operator=(const Warning_info &rhs); 

  
  bool has_sql_condition(const char *message_str, size_t message_length) const;

  
  void clear(ulonglong new_id);

  
  void opt_clear(ulonglong query_id)
  {
    if (query_id != m_warn_id)
      clear(query_id);
  }

  
  void append_warning_info(THD *thd, const Warning_info *source);

  
  void reset_for_next_command()
  { m_current_statement_warn_count= 0; }

  
  void mark_sql_conditions_for_removal();

  
  void unmark_sql_conditions_from_removal()
  { m_marked_sql_conditions.empty(); }

  
  void remove_marked_sql_conditions();

  
  bool is_marked_for_removal(const Sql_condition *cond) const;

  
  void mark_condition_for_removal(Sql_condition *cond)
  { m_marked_sql_conditions.push_back(cond, &m_warn_root); }

  
  ulong warn_count() const
  {
    
    return (m_warn_count[(uint) Sql_condition::WARN_LEVEL_NOTE] +
            m_warn_count[(uint) Sql_condition::WARN_LEVEL_ERROR] +
            m_warn_count[(uint) Sql_condition::WARN_LEVEL_WARN]);
  }

  
  ulong error_count() const
  { return m_warn_count[(uint) Sql_condition::WARN_LEVEL_ERROR]; }

  
  uint cond_count() const
  {
    return m_warn_list.elements();
  }

  
  ulonglong id() const { return m_warn_id; }

  
  void id(ulonglong id_arg) { m_warn_id= id_arg; }

  
  bool is_empty() const { return m_warn_list.is_empty(); }

  
  void inc_current_row_for_warning() { m_current_row_for_warning++; }

  
  void reset_current_row_for_warning() { m_current_row_for_warning= 1; }

  
  ulong current_row_for_warning() const { return m_current_row_for_warning; }

  
  ulong current_statement_warn_count() const
  { return m_current_statement_warn_count; }

  
  void reserve_space(THD *thd, uint count);

  
  Sql_condition *push_warning(THD *thd,
                              const Sql_condition_identity *identity,
                              const char* msg);

  
  Sql_condition *push_warning(THD *thd, const Sql_condition *sql_condition);

  
  void set_read_only(bool read_only_arg)
  { m_read_only= read_only_arg; }

  
  bool is_read_only() const
  { return m_read_only; }

  
  const Sql_condition *get_error_condition() const
  { return m_error_condition; }

  
  void set_error_condition(const Sql_condition *error_condition)
  { m_error_condition= error_condition; }

  
  void clear_error_condition()
  { m_error_condition= NULL; }

  
  
  
  friend class Diagnostics_area;
};


extern char *err_conv(char *buff, uint to_length, const char *from,
                      uint from_length, CHARSET_INFO *from_cs);

class ErrConv
{
protected:
  mutable char err_buffer[MYSQL_ERRMSG_SIZE];
public:
  ErrConv() {}
  virtual ~ErrConv() {}
  virtual const char *ptr() const = 0;
};

class ErrConvString : public ErrConv
{
  const char *str;
  size_t len;
  CHARSET_INFO *cs;
public:
  ErrConvString(const char *str_arg, size_t len_arg, CHARSET_INFO *cs_arg)
    : ErrConv(), str(str_arg), len(len_arg), cs(cs_arg) {}
  ErrConvString(const char *str_arg, CHARSET_INFO *cs_arg)
    : ErrConv(), str(str_arg), len(strlen(str_arg)), cs(cs_arg) {}
  ErrConvString(const String *s)
    : ErrConv(), str(s->ptr()), len(s->length()), cs(s->charset()) {}
  const char *ptr() const
  {
    DBUG_ASSERT(len < UINT_MAX32);
    return err_conv(err_buffer, (uint) sizeof(err_buffer), str, (uint) len, cs);
  }
};

class ErrConvInteger : public ErrConv, public Longlong_hybrid
{
public:
  ErrConvInteger(longlong num_arg, bool unsigned_flag= false) :
    ErrConv(), Longlong_hybrid(num_arg, unsigned_flag) {}
  const char *ptr() const
  {
    return m_unsigned ? ullstr(m_value, err_buffer) :
                         llstr(m_value, err_buffer);
  }
};

class ErrConvDouble: public ErrConv
{
  double num;
public:
  ErrConvDouble(double num_arg) : ErrConv(), num(num_arg) {}
  const char *ptr() const
  {
    my_gcvt(num, MY_GCVT_ARG_DOUBLE, sizeof(err_buffer), err_buffer, 0);
    return err_buffer;
  }
};

class ErrConvTime : public ErrConv
{
  const MYSQL_TIME *ltime;
public:
  ErrConvTime(const MYSQL_TIME *ltime_arg) : ErrConv(), ltime(ltime_arg) {}
  const char *ptr() const
  {
    my_TIME_to_str(ltime, err_buffer, AUTO_SEC_PART_DIGITS);
    return err_buffer;
  }
};

class ErrConvDecimal : public ErrConv
{
  const decimal_t *d;
public:
  ErrConvDecimal(const decimal_t *d_arg) : ErrConv(), d(d_arg) {}
  const char *ptr() const
  {
    int len= sizeof(err_buffer);
    decimal2string(d, err_buffer, &len, 0, 0, ' ');
    return err_buffer;
  }
};





class Diagnostics_area: public Sql_state_errno,
                        public Sql_user_condition_identity
{
private:
  
  typedef I_P_List<Warning_info,
                   I_P_List_adapter<Warning_info,
                                    &Warning_info::m_next_in_da,
                                    &Warning_info::m_prev_in_da>,
                   I_P_List_counter,
                   I_P_List_fast_push_back<Warning_info> >
          Warning_info_list;

public:
  
  typedef Warning_info::Sql_condition_list::Const_Iterator
    Sql_condition_iterator;

  enum enum_diagnostics_status
  {
    
    DA_EMPTY= 0,
    
    DA_OK,
    
    DA_EOF,
    
    DA_OK_BULK,
    
    DA_ERROR,
    
    DA_DISABLED
  };

  void set_overwrite_status(bool can_overwrite_status)
  { m_can_overwrite_status= can_overwrite_status; }

  
  bool is_sent() const { return m_is_sent; }

  void set_is_sent(bool is_sent_arg) { m_is_sent= is_sent_arg; }

  void set_ok_status(ulonglong affected_rows,
                     ulonglong last_insert_id,
                     const char *message);

  void set_eof_status(THD *thd);

  void set_error_status(uint sql_errno);

  void set_error_status(uint sql_errno,
                        const char *message,
                        const char *sqlstate,
                        const Sql_user_condition_identity &ucid,
                        const Sql_condition *error_condition);

  void set_error_status(uint sql_errno,
                        const char *message,
                        const char *sqlstate,
                        const Sql_condition *error_condition)
  {
    set_error_status(sql_errno, message, sqlstate,
                     Sql_user_condition_identity(),
                     error_condition);
  }

  void disable_status();

  void reset_diagnostics_area();

  bool is_set() const { return m_status != DA_EMPTY; }

  bool is_error() const { return m_status == DA_ERROR; }

  bool is_eof() const { return m_status == DA_EOF; }

  bool is_ok() const { return m_status == DA_OK; }

  bool is_disabled() const { return m_status == DA_DISABLED; }

  void set_bulk_execution(bool bulk) { is_bulk_execution= bulk; }

  bool is_bulk_op() const { return is_bulk_execution; }

  enum_diagnostics_status status() const { return m_status; }

  const char *message() const
  { DBUG_ASSERT(m_status == DA_ERROR || m_status == DA_OK ||
                m_status == DA_OK_BULK); return m_message; }

  bool skip_flush() const
  {
    DBUG_ASSERT(m_status == DA_OK || m_status == DA_OK_BULK);
    return m_skip_flush;
  }

  void set_skip_flush()
  { m_skip_flush= TRUE; }

  uint sql_errno() const
  {
    DBUG_ASSERT(m_status == DA_ERROR);
    return Sql_state_errno::get_sql_errno();
  }

  const char* get_sqlstate() const
  { DBUG_ASSERT(m_status == DA_ERROR); return Sql_state::get_sqlstate(); }

  ulonglong affected_rows() const
  {
    DBUG_ASSERT(m_status == DA_OK || m_status == DA_OK_BULK);
    return m_affected_rows;
  }

  ulonglong last_insert_id() const
  {
    DBUG_ASSERT(m_status == DA_OK || m_status == DA_OK_BULK);
    return m_last_insert_id;
  }

  uint statement_warn_count() const
  {
    DBUG_ASSERT(m_status == DA_OK || m_status == DA_OK_BULK ||
                m_status == DA_EOF);
    return m_statement_warn_count;
  }

  
  Sql_condition_identity get_error_condition_identity() const
  {
    DBUG_ASSERT(m_status == DA_ERROR);
    return Sql_condition_identity(*this ,
                                  Sql_condition::WARN_LEVEL_ERROR,
                                  *this );
  }

  
  void increment_warning()
  {
    if (m_status != DA_EMPTY)
      m_statement_warn_count++;
  }

  Diagnostics_area(bool initialize);
  Diagnostics_area(ulonglong warning_info_id, bool allow_unlimited_warnings,
                   bool initialize);
  void init() { m_main_wi.init() ; }
  void free_memory() { m_main_wi.free_memory() ; }

  void push_warning_info(Warning_info *wi)
  { m_wi_stack.push_front(wi); }

  void pop_warning_info()
  {
    DBUG_ASSERT(m_wi_stack.elements() > 0);
    m_wi_stack.remove(m_wi_stack.front());
  }

  void set_warning_info_id(ulonglong id)
  { get_warning_info()->id(id); }

  ulonglong warning_info_id() const
  { return get_warning_info()->id(); }

  
  bool warning_info_changed(const Warning_info *wi) const
  { return get_warning_info()->id() != wi->id(); }

  bool is_warning_info_empty() const
  { return get_warning_info()->is_empty(); }

  ulong current_statement_warn_count() const
  { return get_warning_info()->current_statement_warn_count(); }

  bool has_sql_condition(const char *message_str, size_t message_length) const
  { return get_warning_info()->has_sql_condition(message_str, message_length); }

  void reset_for_next_command()
  { get_warning_info()->reset_for_next_command(); }

  void clear_warning_info(ulonglong id)
  { get_warning_info()->clear(id); }

  void opt_clear_warning_info(ulonglong query_id)
  { get_warning_info()->opt_clear(query_id); }

  ulong current_row_for_warning() const
  { return get_warning_info()->current_row_for_warning(); }

  void inc_current_row_for_warning()
  { get_warning_info()->inc_current_row_for_warning(); }

  void reset_current_row_for_warning()
  { get_warning_info()->reset_current_row_for_warning(); }

  bool is_warning_info_read_only() const
  { return get_warning_info()->is_read_only(); }

  void set_warning_info_read_only(bool read_only_arg)
  { get_warning_info()->set_read_only(read_only_arg); }

  ulong error_count() const
  { return get_warning_info()->error_count(); }

  ulong warn_count() const
  { return get_warning_info()->warn_count(); }

  uint cond_count() const
  { return get_warning_info()->cond_count(); }

  Sql_condition_iterator sql_conditions() const
  { return get_warning_info()->m_warn_list; }

  void reserve_space(THD *thd, uint count)
  { get_warning_info()->reserve_space(thd, count); }

  Sql_condition *push_warning(THD *thd, const Sql_condition *sql_condition)
  { return get_warning_info()->push_warning(thd, sql_condition); }

  Sql_condition *push_warning(THD *thd,
                              uint sql_errno_arg,
                              const char* sqlstate,
                              Sql_condition::enum_warning_level level,
                              const Sql_user_condition_identity &ucid,
                              const char* msg)
  {
    Sql_condition_identity tmp(sql_errno_arg, sqlstate, level, ucid);
    return get_warning_info()->push_warning(thd, &tmp, msg);
  }

  Sql_condition *push_warning(THD *thd,
                              uint sqlerrno,
                              const char* sqlstate,
                              Sql_condition::enum_warning_level level,
                              const char* msg)
  {
    return push_warning(thd, sqlerrno, sqlstate, level,
                        Sql_user_condition_identity(), msg);
  }
  void mark_sql_conditions_for_removal()
  { get_warning_info()->mark_sql_conditions_for_removal(); }

  void unmark_sql_conditions_from_removal()
  { get_warning_info()->unmark_sql_conditions_from_removal(); }

  void remove_marked_sql_conditions()
  { get_warning_info()->remove_marked_sql_conditions(); }

  const Sql_condition *get_error_condition() const
  { return get_warning_info()->get_error_condition(); }

  void copy_sql_conditions_to_wi(THD *thd, Warning_info *dst_wi) const
  { dst_wi->append_warning_info(thd, get_warning_info()); }

  void copy_sql_conditions_from_wi(THD *thd, const Warning_info *src_wi)
  { get_warning_info()->append_warning_info(thd, src_wi); }

  void copy_non_errors_from_wi(THD *thd, const Warning_info *src_wi);

private:
  Warning_info *get_warning_info() { return m_wi_stack.front(); }

  const Warning_info *get_warning_info() const { return m_wi_stack.front(); }

private:
  
  bool m_is_sent;

  
  bool m_can_overwrite_status;

  
  bool m_skip_flush;

  
  char m_message[MYSQL_ERRMSG_SIZE];

  
  ulonglong    m_affected_rows;

  

  ulonglong   m_last_insert_id;
  
  uint	     m_statement_warn_count;

  enum_diagnostics_status m_status;

  my_bool is_bulk_execution;

  Warning_info m_main_wi;

  Warning_info_list m_wi_stack;
};




void push_warning(THD *thd, Sql_condition::enum_warning_level level,
                  uint code, const char *msg);

void push_warning_printf(THD *thd, Sql_condition::enum_warning_level level,
                         uint code, const char *format, ...);

bool mysqld_show_warnings(THD *thd, ulong levels_to_show);

size_t convert_error_message(char *to, size_t to_length,
                             CHARSET_INFO *to_cs,
                             const char *from, size_t from_length,
                             CHARSET_INFO *from_cs, uint *errors);

extern const LEX_CSTRING warning_level_names[];

bool is_sqlstate_valid(const LEX_CSTRING *sqlstate);

inline bool is_sqlstate_completion(const char *s)
{ return s[0] == '0' && s[1] == '0'; }


#endif 
