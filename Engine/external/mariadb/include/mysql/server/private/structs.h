#ifndef STRUCTS_INCLUDED
#define STRUCTS_INCLUDED







#include "sql_plugin.h"                         
#include "sql_const.h"                          
#include "my_time.h"                   
#include "thr_lock.h"                  
#include "my_base.h"                   
#include <mysql_com.h>                  

struct TABLE;
class Type_handler;
class Field;
class Index_statistics;

class THD;

typedef struct st_date_time_format {
  uchar positions[8];
  char  time_separator;			
  uint flag;				
  LEX_CSTRING format;
} DATE_TIME_FORMAT;


typedef struct st_keyfile_info {	
  uchar ref[MAX_REFLENGTH];		
  uchar dupp_ref[MAX_REFLENGTH];	
  uint ref_length;			
  uint block_size;			
  File filenr;				
  ha_rows records;			
  ha_rows deleted;			
  ulonglong data_file_length;		
  ulonglong max_data_file_length;	
  ulonglong index_file_length;
  ulonglong max_index_file_length;
  ulonglong delete_length;		
  ulonglong auto_increment_value;
  int errkey,sortkey;			
  time_t create_time;			
  time_t check_time;
  time_t update_time;
  ulong mean_rec_length;		
} KEYFILE_INFO;


typedef struct st_key_part_info {	
  Field *field;                         
  uint  offset;                         
  uint  null_offset;                    
  
  uint16 length;
  
  uint16 store_length;
  uint16 key_type;
  uint16 fieldnr;                       
  uint16 key_part_flag;                 
  uint8 type;
  uint8 null_bit;			
} KEY_PART_INFO ;

class engine_option_value;
struct ha_index_option_struct;

typedef struct st_key {
  uint	key_length;			
  ulong flags;                          
  uint	user_defined_key_parts;	   
  uint	usable_key_parts; 
  uint ext_key_parts;              
  ulong ext_key_flags;             
  
  key_part_map ext_key_part_map;
  LEX_CSTRING name;
  uint  block_size;
  enum  ha_key_alg algorithm;
  
  bool is_statistics_from_stat_tables;
  
  union
  {
    plugin_ref parser;                  
    LEX_CSTRING *parser_name;           
  };
  KEY_PART_INFO *key_part;
  
  uchar *cache_name;
  
  ulong *rec_per_key;

   
  Index_statistics *read_stats;
  
  Index_statistics *collected_stats;
 
  TABLE *table;
  LEX_CSTRING comment;
  
  engine_option_value *option_list;
  ha_index_option_struct *option_struct;                  

  double actual_rec_per_key(uint i);

} KEY;


struct st_join_table;

typedef struct st_reginfo {		
  struct st_join_table *join_tab;	
  enum thr_lock_type lock_type;		
  bool not_exists_optimize;
  
  bool impossible_range;
} REGINFO;




typedef enum enum_mysql_timestamp_type timestamp_type;


typedef struct {
  ulong year,month,day,hour;
  ulonglong minute,second,second_part;
  bool neg;
} INTERVAL;


typedef struct st_known_date_time_format {
  const char *format_name;
  const char *date_format;
  const char *datetime_format;
  const char *time_format;
} KNOWN_DATE_TIME_FORMAT;

extern const char *show_comp_option_name[];

typedef int *(*update_var)(THD *, struct st_mysql_show_var *);


struct AUTHID
{
  LEX_CSTRING user, host;
  void init() { memset(this, 0, sizeof(*this)); }
  void copy(MEM_ROOT *root, const LEX_CSTRING *usr, const LEX_CSTRING *host);
  bool is_role() const { return user.str[0] && !host.str[0]; }
  void set_lex_string(LEX_CSTRING *l, char *buf)
  {
    if (is_role())
      *l= user;
    else
    {
      l->str= buf;
      l->length= strxmov(buf, user.str, "@", host.str, NullS) - buf;
    }
  }
  void parse(const char *str, size_t length);
  bool read_from_mysql_proc_row(THD *thd, TABLE *table);
};


struct LEX_USER: public AUTHID
{
  LEX_CSTRING plugin, auth;
  LEX_CSTRING pwtext, pwhash;
  void reset_auth()
  {
    pwtext.length= pwhash.length= plugin.length= auth.length= 0;
    pwtext.str= pwhash.str= 0;
    plugin.str= auth.str= "";
  }
};


typedef struct user_resources {
  
  uint questions;
  
  uint updates;
  
  uint conn_per_hour;
  
  int user_conn;
  
  double max_statement_time;

  
  enum {QUERIES_PER_HOUR= 1, UPDATES_PER_HOUR= 2, CONNECTIONS_PER_HOUR= 4,
        USER_CONNECTIONS= 8, MAX_STATEMENT_TIME= 16};
  uint specified_limits;
} USER_RESOURCES;



typedef struct  user_conn {
  
  char *user;
  
  char *host;
  
  ulonglong reset_utime;
  
  uint len;
  
  int connections;
  
  uint conn_per_hour, updates, questions;
  
  USER_RESOURCES user_resources;
} USER_CONN;

typedef struct st_user_stats
{
  char user[MY_MAX(USERNAME_LENGTH, LIST_PROCESS_HOST_LEN) + 1];
  
  
  char priv_user[MY_MAX(USERNAME_LENGTH, LIST_PROCESS_HOST_LEN) + 1];
  uint user_name_length;
  uint total_connections;
  uint total_ssl_connections;
  uint concurrent_connections;
  time_t connected_time;  
  ha_rows rows_read, rows_sent;
  ha_rows rows_updated, rows_deleted, rows_inserted;
  ulonglong bytes_received;
  ulonglong bytes_sent;
  ulonglong binlog_bytes_written;
  ulonglong select_commands, update_commands, other_commands;
  ulonglong commit_trans, rollback_trans;
  ulonglong denied_connections, lost_connections, max_statement_time_exceeded;
  ulonglong access_denied_errors;
  ulonglong empty_queries;
  double busy_time;       
  double cpu_time;        
} USER_STATS;

typedef struct st_table_stats
{
  char table[NAME_LEN * 2 + 2];  
  size_t table_name_length;
  ulonglong rows_read, rows_changed;
  ulonglong rows_changed_x_indexes;
  
  int engine_type;
} TABLE_STATS;

typedef struct st_index_stats
{
  
  char index[NAME_LEN * 3 + 3];
  size_t index_name_length;                       
  ulonglong rows_read;
} INDEX_STATS;


	
#define REG_MAKE_DUPP		1U	
#define REG_NEW_RECORD		2U	
#define REG_UPDATE		4U	
#define REG_DELETE		8U	
#define REG_PROG		16U	
#define REG_CLEAR_AFTER_WRITE	32U
#define REG_MAY_BE_UPDATED	64U
#define REG_AUTO_UPDATE		64U	
#define REG_OVERWRITE		128U
#define REG_SKIP_DUP		256U

	
#define STATUS_NO_RECORD	(1U+2U)	
#define STATUS_GARBAGE		1U
#define STATUS_NOT_FOUND	2U	
#define STATUS_NO_PARENT	4U	
#define STATUS_NOT_READ		8U	
#define STATUS_UPDATED		16U	
#define STATUS_NULL_ROW		32U	
#define STATUS_DELETED		64U


class Discrete_interval {
private:
  ulonglong interval_min;
  ulonglong interval_values;
  ulonglong  interval_max;    
public:
  Discrete_interval *next;    
  void replace(ulonglong start, ulonglong val, ulonglong incr)
  {
    interval_min=    start;
    interval_values= val;
    interval_max=    (val == ULONGLONG_MAX) ? val : start + val * incr;
  }
  Discrete_interval(ulonglong start, ulonglong val, ulonglong incr) :
    next(NULL) { replace(start, val, incr); };
  Discrete_interval() : next(NULL) { replace(0, 0, 0); };
  ulonglong minimum() const { return interval_min;    };
  ulonglong values()  const { return interval_values; };
  ulonglong maximum() const { return interval_max;    };
  
  bool merge_if_contiguous(ulonglong start, ulonglong val, ulonglong incr)
  {
    if (interval_max == start)
    {
      if (val == ULONGLONG_MAX)
      {
        interval_values=   interval_max= val;
      }
      else
      {
        interval_values+=  val;
        interval_max=      start + val * incr;
      }
      return 0;
    }
    return 1;
  };
};


class Discrete_intervals_list {
private:
  Discrete_interval        *head;
  Discrete_interval        *tail;
  
  Discrete_interval        *current;
  uint                  elements; 
  void set_members(Discrete_interval *h, Discrete_interval *t,
                   Discrete_interval *c, uint el)
  {  
    head= h;
    tail= t;
    current= c;
    elements= el;
  }
  void operator=(Discrete_intervals_list &);  
  Discrete_intervals_list(const Discrete_intervals_list &);

public:
  Discrete_intervals_list() : head(NULL), current(NULL), elements(0) {};
  void empty_no_free()
  {
    set_members(NULL, NULL, NULL, 0);
  }
  void empty()
  {
    for (Discrete_interval *i= head; i;)
    {
      Discrete_interval *next= i->next;
      delete i;
      i= next;
    }
    empty_no_free();
  }
  void copy_shallow(const Discrete_intervals_list * dli)
  {
    head= dli->get_head();
    tail= dli->get_tail();
    current= dli->get_current();
    elements= dli->nb_elements();
  }
  void swap (Discrete_intervals_list * dli)
  {
    Discrete_interval *h, *t, *c;
    uint el;
    h= dli->get_head();
    t= dli->get_tail();
    c= dli->get_current();
    el= dli->nb_elements();
    dli->copy_shallow(this);
    set_members(h, t, c, el);
  }
  const Discrete_interval* get_next()
  {
    Discrete_interval *tmp= current;
    if (current != NULL)
      current= current->next;
    return tmp;
  }
  ~Discrete_intervals_list() { empty(); };
  bool append(ulonglong start, ulonglong val, ulonglong incr);
  bool append(Discrete_interval *interval);
  ulonglong minimum()     const { return (head ? head->minimum() : 0); };
  ulonglong maximum()     const { return (head ? tail->maximum() : 0); };
  uint      nb_elements() const { return elements; }
  Discrete_interval* get_head() const { return head; };
  Discrete_interval* get_tail() const { return tail; };
  Discrete_interval* get_current() const { return current; };
};



struct DDL_options_st
{
public:
  enum Options
  {
    OPT_NONE= 0,
    OPT_IF_NOT_EXISTS= 2,              
    OPT_LIKE= 4,                       
    OPT_OR_REPLACE= 16,                
    OPT_OR_REPLACE_SLAVE_GENERATED= 32,
                                       
    OPT_IF_EXISTS= 64
  };

private:
  Options m_options;

public:
  Options create_like_options() const
  {
    return (DDL_options_st::Options)
           (((uint) m_options) & (OPT_IF_NOT_EXISTS | OPT_OR_REPLACE));
  }
  void init() { m_options= OPT_NONE; }
  void init(Options options) { m_options= options; }
  void set(Options other)
  {
    m_options= other;
  }
  void set(const DDL_options_st other)
  {
    m_options= other.m_options;
  }
  bool if_not_exists() const { return m_options & OPT_IF_NOT_EXISTS; }
  bool or_replace() const { return m_options & OPT_OR_REPLACE; }
  bool or_replace_slave_generated() const
  { return m_options & OPT_OR_REPLACE_SLAVE_GENERATED; }
  bool like() const { return m_options & OPT_LIKE; }
  bool if_exists() const { return m_options & OPT_IF_EXISTS; }
  void add(const DDL_options_st::Options other)
  {
    m_options= (Options) ((uint) m_options | (uint) other);
  }
  void add(const DDL_options_st &other)
  {
    add(other.m_options);
  }
  DDL_options_st operator|(const DDL_options_st &other)
  {
    add(other.m_options);
    return *this;
  }
  DDL_options_st operator|=(DDL_options_st::Options other)
  {
    add(other);
    return *this;
  }
};


class DDL_options: public DDL_options_st
{
public:
  DDL_options() { init(); }
  DDL_options(Options options) { init(options); }
  DDL_options(const DDL_options_st &options)
  { DDL_options_st::operator=(options); }
};


struct Lex_length_and_dec_st
{
private:
  const char *m_length;
  const char *m_dec;
public:
  void set(const char *length, const char *dec)
  {
    m_length= length;
    m_dec= dec;
  }
  const char *length() const { return m_length; }
  const char *dec() const { return m_dec; }
};


struct Lex_field_type_st: public Lex_length_and_dec_st
{
private:
  const Type_handler *m_handler;
  void set(const Type_handler *handler, const char *length, const char *dec)
  {
    m_handler= handler;
    Lex_length_and_dec_st::set(length, dec);
  }
public:
  void set(const Type_handler *handler, Lex_length_and_dec_st length_and_dec)
  {
    m_handler= handler;
    Lex_length_and_dec_st::operator=(length_and_dec);
  }
  void set(const Type_handler *handler, const char *length)
  {
    set(handler, length, 0);
  }
  void set(const Type_handler *handler)
  {
    set(handler, 0, 0);
  }
  const Type_handler *type_handler() const { return m_handler; }
};


struct Lex_dyncol_type_st: public Lex_length_and_dec_st
{
private:
  int m_type; 
public:
  void set(int type, const char *length, const char *dec)
  {
    m_type= type;
    Lex_length_and_dec_st::set(length, dec);
  }
  void set(int type, Lex_length_and_dec_st length_and_dec)
  {
    m_type= type;
    Lex_length_and_dec_st::operator=(length_and_dec);
  }
  void set(int type, const char *length)
  {
    set(type, length, 0);
  }
  void set(int type)
  {
    set(type, 0, 0);
  }
  int dyncol_type() const { return m_type; }
};


struct Lex_spblock_handlers_st
{
public:
  int hndlrs;
  void init(int count) { hndlrs= count; }
};


struct Lex_spblock_st: public Lex_spblock_handlers_st
{
public:
  int vars;
  int conds;
  int curs;
  void init()
  {
    vars= conds= hndlrs= curs= 0;
  }
  void init_using_vars(uint nvars)
  {
    vars= nvars;
    conds= hndlrs= curs= 0;
  }
  void join(const Lex_spblock_st &b1, const Lex_spblock_st &b2)
  {
    vars= b1.vars + b2.vars;
    conds= b1.conds + b2.conds;
    hndlrs= b1.hndlrs + b2.hndlrs;
    curs= b1.curs + b2.curs;
  }
};


class Lex_spblock: public Lex_spblock_st
{
public:
  Lex_spblock() { init(); }
  Lex_spblock(const Lex_spblock_handlers_st &other)
  {
    vars= conds= curs= 0;
    hndlrs= other.hndlrs;
  }
};


struct Lex_for_loop_bounds_st
{
public:
  class sp_assignment_lex *m_index;  
  class sp_assignment_lex *m_target_bound; 
  int8 m_direction;
  bool m_implicit_cursor;
  bool is_for_loop_cursor() const { return m_target_bound == NULL; }
};


class Lex_for_loop_bounds_intrange: public Lex_for_loop_bounds_st
{
public:
  Lex_for_loop_bounds_intrange(int8 direction,
                               class sp_assignment_lex *left_expr,
                               class sp_assignment_lex *right_expr)
  {
    m_direction= direction;
    m_index=        direction > 0 ? left_expr  : right_expr;
    m_target_bound= direction > 0 ? right_expr : left_expr;
    m_implicit_cursor= false;
  }
};


struct Lex_for_loop_st
{
public:
  class sp_variable *m_index;  
  class sp_variable *m_target_bound; 
  int m_cursor_offset;
  int8 m_direction;
  bool m_implicit_cursor;
  void init()
  {
    m_index= 0;
    m_target_bound= 0;
    m_direction= 0;
    m_implicit_cursor= false;
  }
  void init(const Lex_for_loop_st &other)
  {
    *this= other;
  }
  bool is_for_loop_cursor() const { return m_target_bound == NULL; }
  bool is_for_loop_explicit_cursor() const
  {
    return is_for_loop_cursor() && !m_implicit_cursor;
  }
};


enum trim_spec { TRIM_LEADING, TRIM_TRAILING, TRIM_BOTH };

struct Lex_trim_st
{
  Item *m_remove;
  Item *m_source;
  trim_spec m_spec;
public:
  void set(trim_spec spec, Item *remove, Item *source)
  {
    m_spec= spec;
    m_remove= remove;
    m_source= source;
  }
  void set(trim_spec spec, Item *source)
  {
    set(spec, NULL, source);
  }
  Item *make_item_func_trim_std(THD *thd) const;
  Item *make_item_func_trim_oracle(THD *thd) const;
  Item *make_item_func_trim(THD *thd) const;
};


class Lex_trim: public Lex_trim_st
{
public:
  Lex_trim(trim_spec spec, Item *source) { set(spec, source); }
};


class Load_data_param
{
protected:
  CHARSET_INFO *m_charset;   
  ulonglong m_fixed_length;  
  bool m_is_fixed_length;
  bool m_use_blobs;
public:
  Load_data_param(CHARSET_INFO *cs, bool is_fixed_length):
    m_charset(cs),
    m_fixed_length(0),
    m_is_fixed_length(is_fixed_length),
    m_use_blobs(false)
  { }
  bool add_outvar_field(THD *thd, const Field *field);
  bool add_outvar_user_var(THD *thd);
  CHARSET_INFO *charset() const { return m_charset; }
  bool is_fixed_length() const { return m_is_fixed_length; }
  bool use_blobs() const { return m_use_blobs; }
};


class Load_data_outvar
{
public:
  virtual ~Load_data_outvar() {}
  virtual bool load_data_set_null(THD *thd, const Load_data_param *param)= 0;
  virtual bool load_data_set_value(THD *thd, const char *pos, uint length,
                                   const Load_data_param *param)= 0;
  virtual bool load_data_set_no_data(THD *thd, const Load_data_param *param)= 0;
  virtual void load_data_print_for_log_event(THD *thd, class String *to) const= 0;
  virtual bool load_data_add_outvar(THD *thd, Load_data_param *param) const= 0;
  virtual uint load_data_fixed_length() const= 0;
};


#endif 
