



#ifndef SQL_LEX_INCLUDED
#define SQL_LEX_INCLUDED

#include "violite.h"                            
#include "sql_trigger.h"
#include "thr_lock.h"                  
#include "mem_root_array.h"
#include "sql_cmd.h"
#include "sql_alter.h"                
#include "sql_window.h"
#include "sql_trigger.h"
#include "sp.h"                       
#include "sql_tvc.h"
#include "item.h"


#define SELECT_NESTING_MAP_SIZE 64
typedef Bitmap<SELECT_NESTING_MAP_SIZE> nesting_map;





struct Lex_string_with_metadata_st: public LEX_CSTRING
{
private:
  bool m_is_8bit; 
  char m_quote;   
public:
  void set_8bit(bool is_8bit) { m_is_8bit= is_8bit; }
  void set_metadata(bool is_8bit, char quote)
  {
    m_is_8bit= is_8bit;
    m_quote= quote;
  }
  void set(const char *s, size_t len, bool is_8bit, char quote)
  {
    str= s;
    length= len;
    set_metadata(is_8bit, quote);
  }
  void set(const LEX_CSTRING *s, bool is_8bit, char quote)
  {
    ((LEX_CSTRING &)*this)= *s;
    set_metadata(is_8bit, quote);
  }
  bool is_8bit() const { return m_is_8bit; }
  bool is_quoted() const { return m_quote != '\0'; }
  char quote() const { return m_quote; }
  
  uint repertoire(CHARSET_INFO *cs) const
  {
    return !m_is_8bit && my_charset_is_ascii_based(cs) ?
           MY_REPERTOIRE_ASCII : MY_REPERTOIRE_UNICODE30;
  }
  
  uint repertoire() const
  {
    return !m_is_8bit ? MY_REPERTOIRE_ASCII : MY_REPERTOIRE_UNICODE30;
  }
};



struct Lex_ident_cli_st: public Lex_string_with_metadata_st
{
public:
  void set_keyword(const char *s, size_t len)
  {
    set(s, len, false, '\0');
  }
  void set_ident(const char *s, size_t len, bool is_8bit)
  {
    set(s, len, is_8bit, '\0');
  }
  void set_ident_quoted(const char *s, size_t len, bool is_8bit, char quote)
  {
    set(s, len, is_8bit, quote);
  }
  void set_unquoted(const LEX_CSTRING *s, bool is_8bit)
  {
    set(s, is_8bit, '\0');
  }
  const char *pos() const { return str - is_quoted(); }
  const char *end() const { return str + length + is_quoted(); }
};


class Lex_ident_cli: public Lex_ident_cli_st
{
public:
  Lex_ident_cli(const LEX_CSTRING *s, bool is_8bit)
  {
    set_unquoted(s, is_8bit);
  }
  Lex_ident_cli(const char *s, size_t len)
  {
    set_ident(s, len, false);
  }
};


struct Lex_ident_sys_st: public LEX_CSTRING
{
public:
  bool copy_ident_cli(THD *thd, const Lex_ident_cli_st *str);
  bool copy_keyword(THD *thd, const Lex_ident_cli_st *str);
  bool copy_sys(THD *thd, const LEX_CSTRING *str);
  bool convert(THD *thd, const LEX_CSTRING *str, CHARSET_INFO *cs);
  bool copy_or_convert(THD *thd, const Lex_ident_cli_st *str, CHARSET_INFO *cs);
  bool is_null() const { return str == NULL; }
  bool to_size_number(ulonglong *to) const;
};


class Lex_ident_sys: public Lex_ident_sys_st
{
public:
  Lex_ident_sys(THD *thd, const Lex_ident_cli_st *str)
  {
    if (copy_ident_cli(thd, str))
      ((LEX_CSTRING &) *this)= null_clex_str;
  }
  Lex_ident_sys()
  {
    ((LEX_CSTRING &) *this)= null_clex_str;
  }
};


enum sub_select_type
{
  UNSPECIFIED_TYPE,
  
  UNION_TYPE, INTERSECT_TYPE, EXCEPT_TYPE,
  GLOBAL_OPTIONS_TYPE, DERIVED_TABLE_TYPE, OLAP_TYPE
};
enum unit_common_op {OP_MIX, OP_UNION, OP_INTERSECT, OP_EXCEPT};

enum enum_view_suid
{
  VIEW_SUID_INVOKER= 0,
  VIEW_SUID_DEFINER= 1,
  VIEW_SUID_DEFAULT= 2
};


enum plsql_cursor_attr_t
{
  PLSQL_CURSOR_ATTR_ISOPEN,
  PLSQL_CURSOR_ATTR_FOUND,
  PLSQL_CURSOR_ATTR_NOTFOUND,
  PLSQL_CURSOR_ATTR_ROWCOUNT
};



class Table_ident;
class sql_exchange;
class LEX_COLUMN;
class sp_head;
class sp_name;
class sp_instr;
class sp_pcontext;
class sp_variable;
class sp_assignment_lex;
class st_alter_tablespace;
class partition_info;
class Event_parse_data;
class set_var_base;
class sys_var;
class Item_func_match;
class File_parser;
class Key_part_spec;
class Item_window_func;
struct sql_digest_state;
class With_clause;
class my_var;

#define ALLOC_ROOT_SET 1024

#ifdef MYSQL_SERVER

extern uint binlog_unsafe_map[256];

void binlog_unsafe_map_init();
#endif

struct LEX_TYPE
{
  enum enum_field_types type;
  char *length, *dec;
  CHARSET_INFO *charset;
  void set(int t, char *l, char *d, CHARSET_INFO *cs)
  { type= (enum_field_types)t; length= l; dec= d; charset= cs; }
};

#ifdef MYSQL_SERVER

#ifdef MYSQL_YACC
#define LEX_YYSTYPE void *
#else
#include "lex_symbol.h"
#ifdef MYSQL_LEX
#include "item_func.h"            
#include "sql_get_diagnostics.h"  
#include "sp_pcontext.h"
#include "sql_yacc.hh"
#define LEX_YYSTYPE YYSTYPE *
#else
#define LEX_YYSTYPE void *
#endif
#endif
#endif


#define DESCRIBE_NORMAL         1
#define DESCRIBE_EXTENDED       2

#define DESCRIBE_PARTITIONS     4

#ifdef MYSQL_SERVER

extern const LEX_STRING  empty_lex_str;
extern MYSQL_PLUGIN_IMPORT const LEX_CSTRING empty_clex_str;
extern const LEX_CSTRING star_clex_str;
extern const LEX_CSTRING param_clex_str;

enum enum_sp_suid_behaviour
{
  SP_IS_DEFAULT_SUID= 0,
  SP_IS_NOT_SUID,
  SP_IS_SUID
};

enum enum_sp_data_access
{
  SP_DEFAULT_ACCESS= 0,
  SP_CONTAINS_SQL,
  SP_NO_SQL,
  SP_READS_SQL_DATA,
  SP_MODIFIES_SQL_DATA
};

enum enum_sp_aggregate_type
{
  DEFAULT_AGGREGATE= 0,
  NOT_AGGREGATE,
  GROUP_AGGREGATE
};

const LEX_CSTRING sp_data_access_name[]=
{
  { STRING_WITH_LEN("") },
  { STRING_WITH_LEN("CONTAINS SQL") },
  { STRING_WITH_LEN("NO SQL") },
  { STRING_WITH_LEN("READS SQL DATA") },
  { STRING_WITH_LEN("MODIFIES SQL DATA") }
};

#define DERIVED_SUBQUERY        1
#define DERIVED_VIEW            2
#define DERIVED_WITH            4

enum enum_view_create_mode
{
  VIEW_CREATE_NEW,              
  VIEW_ALTER,                   
  VIEW_CREATE_OR_REPLACE        
};


class Create_view_info: public Sql_alloc
{
public:
  LEX_CSTRING select;              
  enum enum_view_create_mode mode;
  uint16 algorithm;
  uint8 check;
  enum enum_view_suid suid;
  Create_view_info(enum_view_create_mode mode_arg,
                   uint16 algorithm_arg,
                   enum_view_suid suid_arg)
   :select(null_clex_str),
    mode(mode_arg),
    algorithm(algorithm_arg),
    check(VIEW_CHECK_NONE),
    suid(suid_arg)
  { }
};


enum enum_drop_mode
{
  DROP_DEFAULT, 
  DROP_CASCADE, 
  DROP_RESTRICT 
};


#define TL_OPTION_UPDATING      1
#define TL_OPTION_FORCE_INDEX   2
#define TL_OPTION_IGNORE_LEAVES 4
#define TL_OPTION_ALIAS         8
#define TL_OPTION_SEQUENCE      16

typedef List<Item> List_item;
typedef Mem_root_array<ORDER*, true> Group_list_ptrs;


typedef struct st_lex_server_options
{
  long port;
  LEX_CSTRING server_name, host, db, username, password, scheme, socket, owner;
  void reset(LEX_CSTRING name)
  {
    server_name= name;
    host= db= username= password= scheme= socket= owner= null_clex_str;
    port= -1;
  }
} LEX_SERVER_OPTIONS;



struct LEX_MASTER_INFO
{
  DYNAMIC_ARRAY repl_ignore_server_ids;
  DYNAMIC_ARRAY repl_do_domain_ids;
  DYNAMIC_ARRAY repl_ignore_domain_ids;
  const char *host, *user, *password, *log_file_name;
  const char *ssl_key, *ssl_cert, *ssl_ca, *ssl_capath, *ssl_cipher;
  const char *ssl_crl, *ssl_crlpath;
  const char *relay_log_name;
  LEX_CSTRING connection_name;
  
  LEX_CSTRING gtid_pos_str;
  ulonglong pos;
  ulong relay_log_pos;
  ulong server_id;
  uint port, connect_retry;
  float heartbeat_period;
  int sql_delay;
  
  enum {LEX_MI_UNCHANGED= 0, LEX_MI_DISABLE, LEX_MI_ENABLE}
    ssl, ssl_verify_server_cert, heartbeat_opt, repl_ignore_server_ids_opt,
    repl_do_domain_ids_opt, repl_ignore_domain_ids_opt;
  enum {
    LEX_GTID_UNCHANGED, LEX_GTID_NO, LEX_GTID_CURRENT_POS, LEX_GTID_SLAVE_POS
  } use_gtid_opt;

  void init()
  {
    bzero(this, sizeof(*this));
    my_init_dynamic_array(&repl_ignore_server_ids,
                          sizeof(::server_id), 0, 16, MYF(0));
    my_init_dynamic_array(&repl_do_domain_ids,
                          sizeof(ulong), 0, 16, MYF(0));
    my_init_dynamic_array(&repl_ignore_domain_ids,
                          sizeof(ulong), 0, 16, MYF(0));
    sql_delay= -1;
  }
  void reset(bool is_change_master)
  {
    if (unlikely(is_change_master))
    {
      delete_dynamic(&repl_ignore_server_ids);
      
      delete_dynamic(&repl_do_domain_ids);
      delete_dynamic(&repl_ignore_domain_ids);
    }

    host= user= password= log_file_name= ssl_key= ssl_cert= ssl_ca=
      ssl_capath= ssl_cipher= relay_log_name= 0;
    pos= relay_log_pos= server_id= port= connect_retry= 0;
    heartbeat_period= 0;
    ssl= ssl_verify_server_cert= heartbeat_opt=
      repl_ignore_server_ids_opt= repl_do_domain_ids_opt=
      repl_ignore_domain_ids_opt= LEX_MI_UNCHANGED;
    gtid_pos_str= null_clex_str;
    use_gtid_opt= LEX_GTID_UNCHANGED;
    sql_delay= -1;
  }
};

typedef struct st_lex_reset_slave
{
  bool all;
} LEX_RESET_SLAVE;

enum olap_type 
{
  UNSPECIFIED_OLAP_TYPE, CUBE_TYPE, ROLLUP_TYPE
};


extern const char * index_hint_type_name[];
typedef uchar index_clause_map;


#define INDEX_HINT_MASK_JOIN  (1)
#define INDEX_HINT_MASK_GROUP (1 << 1)
#define INDEX_HINT_MASK_ORDER (1 << 2)

#define INDEX_HINT_MASK_ALL (INDEX_HINT_MASK_JOIN | INDEX_HINT_MASK_GROUP | \
                             INDEX_HINT_MASK_ORDER)

class select_result_sink;


class Index_hint : public Sql_alloc
{
public:
  
  enum index_hint_type type;
  
  index_clause_map clause;
   
  LEX_CSTRING key_name;

  Index_hint (enum index_hint_type type_arg, index_clause_map clause_arg,
              const char *str, size_t length) :
    type(type_arg), clause(clause_arg)
  {
    key_name.str= str;
    key_name.length= length;
  }

  void print(THD *thd, String *str);
}; 




struct LEX;
class st_select_lex;
class st_select_lex_unit;


class st_select_lex_node {
protected:
  st_select_lex_node *next, **prev,   
    *master, *slave,                  
    *link_next, **link_prev;          

  void init_query_common();
public:

  ulonglong options;

  
  enum e_sql_cache { SQL_CACHE_UNSPECIFIED, SQL_NO_CACHE, SQL_CACHE };
  e_sql_cache sql_cache;

  
  uint8 uncacheable;
  enum sub_select_type linkage;
  bool is_linkage_set() const
  {
    return linkage == UNION_TYPE || linkage == INTERSECT_TYPE || linkage == EXCEPT_TYPE;
  }
  bool no_table_names_allowed; 

  static void *operator new(size_t size, MEM_ROOT *mem_root) throw ()
  { return (void*) alloc_root(mem_root, (uint) size); }
  static void operator delete(void *ptr,size_t size) { TRASH_FREE(ptr, size); }
  static void operator delete(void *ptr, MEM_ROOT *mem_root) {}

  
  st_select_lex_node()
    : next(NULL), prev(NULL),
      master(NULL), slave(NULL),
      link_next(NULL), link_prev(NULL),
      linkage(UNSPECIFIED_TYPE)
  {
  }

  inline st_select_lex_node* get_master() { return master; }
  void include_down(st_select_lex_node *upper);
  void add_slave(st_select_lex_node *slave_arg);
  void include_neighbour(st_select_lex_node *before);
  void include_standalone(st_select_lex_node *sel, st_select_lex_node **ref);
  void include_global(st_select_lex_node **plink);
  void exclude();
  void exclude_from_tree();

  void set_slave(st_select_lex_node *slave_arg) { slave= slave_arg; }
  void move_node(st_select_lex_node *where_to_move)
  {
    if (where_to_move == this)
      return;
    if (next)
      next->prev= prev;
    *prev= next;
    *where_to_move->prev= this;
    next= where_to_move;
  }
  st_select_lex_node *insert_chain_before(st_select_lex_node **ptr_pos_to_insert,
                                          st_select_lex_node *end_chain_node);
  void move_as_slave(st_select_lex_node *new_master);
  friend class st_select_lex_unit;
  friend bool mysql_new_select(LEX *lex, bool move_down, SELECT_LEX *sel);
  friend bool mysql_make_view(THD *thd, TABLE_SHARE *share, TABLE_LIST *table,
                              bool open_view_no_parse);
private:
  void fast_exclude();
};
typedef class st_select_lex_node SELECT_LEX_NODE;


class THD;
class select_result;
class JOIN;
class select_unit;
class Procedure;
class Explain_query;

void delete_explain_query(LEX *lex);
void create_explain_query(LEX *lex, MEM_ROOT *mem_root);
void create_explain_query_if_not_exists(LEX *lex, MEM_ROOT *mem_root);
bool print_explain_for_slow_log(LEX *lex, THD *thd, String *str);

class st_select_lex_unit: public st_select_lex_node {
protected:
  TABLE_LIST result_table_list;
  select_unit *union_result;
  ulonglong found_rows_for_union;
  bool saved_error;

  bool prepare_join(THD *thd, SELECT_LEX *sl, select_result *result,
                    ulong additional_options,
                    bool is_union_select);
  bool join_union_item_types(THD *thd, List<Item> &types, uint count);
  bool join_union_type_handlers(THD *thd,
                                class Type_holder *holders, uint count);
  bool join_union_type_attributes(THD *thd,
                                  class Type_holder *holders, uint count);
public:
  
  st_select_lex_unit()
    : union_result(NULL), table(NULL), result(NULL),
      cleaned(false),
      fake_select_lex(NULL)
  {
  }


  TABLE *table; 
  select_result *result;
  bool  prepared, 
    optimized, 
    optimized_2,
    executed, 
    cleaned;

  bool optimize_started;

  
  List<Item> item_list;
  
  List<Item> types;
  
  Item_int *intersect_mark;
  
  inline st_select_lex *global_parameters()
  {
    if (fake_select_lex != NULL)
      return fake_select_lex;
    else if (saved_fake_select_lex != NULL)
      return saved_fake_select_lex;
    return first_select();
  };
  
  st_select_lex *return_to;
  
  ha_rows select_limit_cnt, offset_limit_cnt;
  
  Item_subselect *item;
  
  TABLE_LIST *derived;
  bool is_view;
  
  With_clause *with_clause;
  
  With_element *with_element;
  
  THD *thd;
  
  st_select_lex *fake_select_lex;
  
  st_select_lex *saved_fake_select_lex;

  st_select_lex *union_distinct; 
  bool describe; 
  Procedure *last_procedure;     

  bool columns_are_renamed;

  void init_query();
  st_select_lex* outer_select();
  st_select_lex* first_select()
  {
    return reinterpret_cast<st_select_lex*>(slave);
  }
  inline void set_with_clause(With_clause *with_cl);
  st_select_lex_unit* next_unit()
  {
    return reinterpret_cast<st_select_lex_unit*>(next);
  }
  st_select_lex* return_after_parsing() { return return_to; }
  void exclude_level();
  
  bool is_excluded() { return prev == NULL; }

  
  bool prepare(TABLE_LIST *derived_arg, select_result *sel_result,
               ulong additional_options);
  bool optimize();
  bool exec();
  bool exec_recursive();
  bool cleanup();
  inline void unclean() { cleaned= 0; }
  void reinit_exec_mechanism();

  void print(String *str, enum_query_type query_type);

  bool add_fake_select_lex(THD *thd);
  void init_prepare_fake_select_lex(THD *thd, bool first_execution);
  inline bool is_prepared() { return prepared; }
  bool change_result(select_result_interceptor *result,
                     select_result_interceptor *old_result);
  void set_limit(st_select_lex *values);
  void set_thd(THD *thd_arg) { thd= thd_arg; }
  inline bool is_unit_op ();
  bool union_needs_tmp_table();

  void set_unique_exclude();

  friend struct LEX;
  friend int subselect_union_engine::exec();

  List<Item> *get_column_types(bool for_cursor);

  select_unit *get_union_result() { return union_result; }
  int save_union_explain(Explain_query *output);
  int save_union_explain_part2(Explain_query *output);
  unit_common_op common_op();
};

typedef class st_select_lex_unit SELECT_LEX_UNIT;
typedef Bounds_checked_array<Item*> Ref_ptr_array;




class Grouping_tmp_field :public Sql_alloc
{
public:
  Field *tmp_field;
  Item *producing_item;
  Grouping_tmp_field(Field *fld, Item *item) 
     :tmp_field(fld), producing_item(item) {}
};


#define TOUCHED_SEL_COND 1
#define TOUCHED_SEL_DERIVED (1<<1)


class st_select_lex: public st_select_lex_node
{
public:
  Name_resolution_context context;
  LEX_CSTRING db;
  Item *where, *having;                         
  Item *prep_where; 
  Item *prep_having;
  Item *cond_pushed_into_where;  
  Item *cond_pushed_into_having; 
  
  Item::cond_result cond_value, having_value;
  
  LEX *parent_lex;
  enum olap_type olap;
  
  SQL_I_List<TABLE_LIST>  table_list;

  
  SQL_I_List<ORDER>       group_list;
  Group_list_ptrs        *group_list_ptrs;

  List<Item>          item_list;  
  List<Item>          pre_fix; 
  bool                is_item_list_lookup;
  
  List<Item_func_match> *ftfunc_list;
  List<Item_func_match> ftfunc_list_alloc;
  
  List<Item_sum> min_max_opt_list;
  JOIN *join; 
  List<TABLE_LIST> top_join_list; 
  List<TABLE_LIST> *join_list;    
  TABLE_LIST *embedding;          
  List<TABLE_LIST> sj_nests;      
  
  
  List<Item_in_subselect> sj_subselects;
  
  List<Item_func_in> in_funcs;
  
  uint curr_tvc_name;
  
  
  bool have_merged_subqueries;

  List<TABLE_LIST> leaf_tables;
  List<TABLE_LIST> leaf_tables_exec;
  List<TABLE_LIST> leaf_tables_prep;
  enum leaf_list_state {UNINIT, READY, SAVED};
  enum leaf_list_state prep_leaf_list_state;
  uint insert_tables;
  st_select_lex *merged_into; 
                              

  const char *type;               

  SQL_I_List<ORDER> order_list;   
  SQL_I_List<ORDER> gorder_list;
  Item *select_limit, *offset_limit;  

  
  Ref_ptr_array ref_pointer_array;

  
  uint select_n_having_items;
  uint cond_count;    
  uint between_count; 
  uint max_equal_elems;    
  
  uint select_n_where_fields;
  
  uint select_n_reserved;
  
  uint hidden_bit_fields;
  enum_parsing_place parsing_place; 
  enum_parsing_place context_analysis_place; 
  bool with_sum_func;   

  ulong table_join_options;
  uint in_sum_expr;
  uint select_number; 

  
  SELECT_LEX_UNIT *nest_level_base;
  int nest_level;     
  Item_sum *inner_sum_func_list;  
  uint with_wild; 
  bool braces;    
  bool automatic_brackets; 
  
  bool having_fix_field;
  
  bool having_fix_field_for_pushed_cond;
  
  List<Item_outer_ref> inner_refs_list;
  
  uint n_sum_items;
  
  uint n_child_sum_items;

  
  bool explicit_limit;
  
  bool expr_cache_may_be_used[PARSING_PLACE_SIZE];
  
  bool subquery_in_having;
  
  bool with_all_modifier;  
  bool is_correlated;
  
  uint8 changed_elements; 
  
  bool first_natural_join_processing;
  bool first_cond_optimization;
  
  bool no_wrap_view_item;
  
  bool exclude_from_table_unique_test;
  
  int cur_pos_in_select_list;

  List<udf_func>     udf_list;                  

  
  List<String> *prev_join_using;

  
  table_map select_list_tables;

  
  nesting_map name_visibility_map;
  
  table_map with_dep;
  List<Grouping_tmp_field> grouping_tmp_fields;

  
  thr_lock_type lock_type;
  
  table_value_constr *tvc;
  bool in_tvc;

  
public:
  uint versioned_tables;
  int vers_setup_conds(THD *thd, TABLE_LIST *tables);
  
  bool vers_push_field(THD *thd, TABLE_LIST *table, const LEX_CSTRING field_name);

  void init_query();
  void init_select();
  st_select_lex_unit* master_unit() { return (st_select_lex_unit*) master; }
  st_select_lex_unit* first_inner_unit() 
  { 
    return (st_select_lex_unit*) slave; 
  }
  st_select_lex* outer_select();
  st_select_lex* next_select() { return (st_select_lex*) next; }
  st_select_lex* next_select_in_list() 
  {
    return (st_select_lex*) link_next;
  }
  st_select_lex_node** next_select_in_list_addr()
  {
    return &link_next;
  }
  st_select_lex* return_after_parsing()
  {
    return master_unit()->return_after_parsing();
  }
  inline bool is_subquery_function() { return master_unit()->item != 0; }

  bool mark_as_dependent(THD *thd, st_select_lex *last, Item *dependency);

  void set_braces(bool value)
  {
    braces= value;
  }
  bool inc_in_sum_expr();
  uint get_in_sum_expr();

  bool add_item_to_list(THD *thd, Item *item);
  bool add_group_to_list(THD *thd, Item *item, bool asc);
  bool add_ftfunc_to_list(THD *thd, Item_func_match *func);
  bool add_order_to_list(THD *thd, Item *item, bool asc);
  bool add_gorder_to_list(THD *thd, Item *item, bool asc);
  TABLE_LIST* add_table_to_list(THD *thd, Table_ident *table,
                                LEX_CSTRING *alias,
                                ulong table_options,
                                thr_lock_type flags= TL_UNLOCK,
                                enum_mdl_type mdl_type= MDL_SHARED_READ,
                                List<Index_hint> *hints= 0,
                                List<String> *partition_names= 0,
                                LEX_STRING *option= 0);
  TABLE_LIST* get_table_list();
  bool init_nested_join(THD *thd);
  TABLE_LIST *end_nested_join(THD *thd);
  TABLE_LIST *nest_last_join(THD *thd);
  void add_joined_table(TABLE_LIST *table);
  TABLE_LIST *convert_right_join();
  List<Item>* get_item_list();
  ulong get_table_join_options();
  void set_lock_for_tables(thr_lock_type lock_type);
  inline void init_order()
  {
    order_list.elements= 0;
    order_list.first= 0;
    order_list.next= &order_list.first;
  }
  
  void cut_subtree() { slave= 0; }
  bool test_limit();
  
  ha_rows get_offset();
  
  ha_rows get_limit();

  friend struct LEX;
  st_select_lex() : group_list_ptrs(NULL), braces(0), automatic_brackets(0),
  n_sum_items(0), n_child_sum_items(0)
  {}
  void make_empty_select()
  {
    init_query();
    init_select();
  }
  bool setup_ref_array(THD *thd, uint order_group_num);
  void print(THD *thd, String *str, enum_query_type query_type);
  static void print_order(String *str,
                          ORDER *order,
                          enum_query_type query_type);
  void print_limit(THD *thd, String *str, enum_query_type query_type);
  void fix_prepare_information(THD *thd, Item **conds, Item **having_conds);
  
  bool cleanup();
  
  void cleanup_all_joins(bool full);

  void set_index_hint_type(enum index_hint_type type, index_clause_map clause);

  
  bool add_index_hint (THD *thd, const char *str, size_t length);

  
  void alloc_index_hints (THD *thd);
  
  List<Index_hint>* pop_index_hints(void) 
  {
    List<Index_hint> *hints= index_hints;
    index_hints= NULL;
    return hints;
  }

  void clear_index_hints(void) { index_hints= NULL; }
  bool is_part_of_union() { return master_unit()->is_unit_op(); }
  bool is_top_level_node() 
  { 
    return (select_number == 1) && !is_part_of_union();
  }
  bool optimize_unflattened_subqueries(bool const_only);
  
  void set_explain_type(bool on_the_fly);
  bool handle_derived(LEX *lex, uint phases);
  void append_table_to_list(TABLE_LIST *TABLE_LIST::*link, TABLE_LIST *table);
  bool get_free_table_map(table_map *map, uint *tablenr);
  void replace_leaf_table(TABLE_LIST *table, List<TABLE_LIST> &tbl_list);
  void remap_tables(TABLE_LIST *derived, table_map map,
                    uint tablenr, st_select_lex *parent_lex);
  bool merge_subquery(THD *thd, TABLE_LIST *derived, st_select_lex *subq_lex,
                      uint tablenr, table_map map);
  inline bool is_mergeable()
  {
    return (next_select() == 0 && group_list.elements == 0 &&
            having == 0 && with_sum_func == 0 &&
            table_list.elements >= 1 && !(options & SELECT_DISTINCT) &&
            select_limit == 0);
  }
  void mark_as_belong_to_derived(TABLE_LIST *derived);
  void increase_derived_records(ha_rows records);
  void update_used_tables();
  void update_correlated_cache();
  void mark_const_derived(bool empty);

  bool save_leaf_tables(THD *thd);
  bool save_prep_leaf_tables(THD *thd);

  bool is_merged_child_of(st_select_lex *ancestor);

  
  bool non_agg_field_used() const { return m_non_agg_field_used; }
  bool agg_func_used()      const { return m_agg_func_used; }
  bool custom_agg_func_used() const { return m_custom_agg_func_used; }

  void set_non_agg_field_used(bool val) { m_non_agg_field_used= val; }
  void set_agg_func_used(bool val)      { m_agg_func_used= val; }
  void set_custom_agg_func_used(bool val) { m_custom_agg_func_used= val; }
  inline void set_with_clause(With_clause *with_clause);
  With_clause *get_with_clause()
  {
    return master_unit()->with_clause;
  }
  With_element *get_with_element()
  {
    return master_unit()->with_element;
  }
  With_element *find_table_def_in_with_clauses(TABLE_LIST *table);
  bool check_unrestricted_recursive(bool only_standard_compliant);
  bool check_subqueries_with_recursive_references();
  void collect_grouping_fields(THD *thd, ORDER *grouping_list); 
  void check_cond_extraction_for_grouping_fields(Item *cond,
                                                 TABLE_LIST *derived);
  Item *build_cond_for_grouping_fields(THD *thd, Item *cond,
                                       bool no_to_clones);
  
  List<Window_spec> window_specs;
  void prepare_add_window_spec(THD *thd);
  bool add_window_def(THD *thd, LEX_CSTRING *win_name, LEX_CSTRING *win_ref,
                      SQL_I_List<ORDER> win_partition_list,
                      SQL_I_List<ORDER> win_order_list,
                      Window_frame *win_frame);
  bool add_window_spec(THD *thd, LEX_CSTRING *win_ref,
                       SQL_I_List<ORDER> win_partition_list,
                       SQL_I_List<ORDER> win_order_list,
                       Window_frame *win_frame);
  List<Item_window_func> window_funcs;
  bool add_window_func(Item_window_func *win_func)
  {
    return window_funcs.push_back(win_func);
  }

  bool have_window_funcs() const { return (window_funcs.elements !=0); }
  ORDER *find_common_window_func_partition_fields(THD *thd);

  bool cond_pushdown_is_allowed() const
  { return !olap && !explicit_limit && !tvc; }
  
private:
  bool m_non_agg_field_used;
  bool m_agg_func_used;
  bool m_custom_agg_func_used;

  
  enum index_hint_type current_index_hint_type;
  index_clause_map current_index_hint_clause;
  
  List<Index_hint> *index_hints;

public:
  inline void add_where_field(st_select_lex *sel)
  {
    DBUG_ASSERT(this != sel);
    select_n_where_fields+= sel->select_n_where_fields;
  }
};
typedef class st_select_lex SELECT_LEX;

inline bool st_select_lex_unit::is_unit_op ()
{
  if (!first_select()->next_select())
  {
    if (first_select()->tvc)
      return 1;
    else
      return 0;
  }

  enum sub_select_type linkage= first_select()->next_select()->linkage;
  return linkage == UNION_TYPE || linkage == INTERSECT_TYPE ||
    linkage == EXCEPT_TYPE;
}


struct st_sp_chistics
{
  LEX_CSTRING comment;
  enum enum_sp_suid_behaviour suid;
  bool detistic;
  enum enum_sp_data_access daccess;
  enum enum_sp_aggregate_type agg_type;
  void init() { bzero(this, sizeof(*this)); }
  void set(const st_sp_chistics &other) { *this= other; }
  bool read_from_mysql_proc_row(THD *thd, TABLE *table);
};


class Sp_chistics: public st_sp_chistics
{
public:
  Sp_chistics() { init(); }
};


struct st_trg_chistics: public st_trg_execution_order
{
  enum trg_action_time_type action_time;
  enum trg_event_type event;

  const char *ordering_clause_begin;
  const char *ordering_clause_end;

};

enum xa_option_words {XA_NONE, XA_JOIN, XA_RESUME, XA_ONE_PHASE,
                      XA_SUSPEND, XA_FOR_MIGRATE};

class Sroutine_hash_entry;



class Query_tables_list
{
public:
  
  enum_sql_command sql_command;
  
  TABLE_LIST *query_tables;
  
  TABLE_LIST **query_tables_last;
  
  TABLE_LIST **query_tables_own_last;
  
  enum { START_SROUTINES_HASH_SIZE= 16 };
  HASH sroutines;
  
  SQL_I_List<Sroutine_hash_entry> sroutines_list;
  Sroutine_hash_entry **sroutines_list_own_last;
  uint sroutines_list_own_elements;

  
  enum enum_lock_tables_state {
    LTS_NOT_LOCKED = 0,
    LTS_LOCKED
  };
  enum_lock_tables_state lock_tables_state;
  bool is_query_tables_locked()
  {
    return (lock_tables_state == LTS_LOCKED);
  }

  
  uint table_count;

   
  Query_tables_list() {}
  ~Query_tables_list() {}

  
  void reset_query_tables_list(bool init);
  void destroy_query_tables_list();
  void set_query_tables_list(Query_tables_list *state)
  {
    *this= *state;
  }

  
  void add_to_query_tables(TABLE_LIST *table)
  {
    *(table->prev_global= query_tables_last)= table;
    query_tables_last= &table->next_global;
  }
  bool requires_prelocking()
  {
    return MY_TEST(query_tables_own_last);
  }
  void mark_as_requiring_prelocking(TABLE_LIST **tables_own_last)
  {
    query_tables_own_last= tables_own_last;
  }
  
  TABLE_LIST* first_not_own_table()
  {
    return ( query_tables_own_last ? *query_tables_own_last : 0);
  }
  void chop_off_not_own_tables()
  {
    if (query_tables_own_last)
    {
      *query_tables_own_last= 0;
      query_tables_last= query_tables_own_last;
      query_tables_own_last= 0;
    }
  }

  
  TABLE_LIST *last_table()
  {
    
    return query_tables ?
           (TABLE_LIST*) ((char*) query_tables_last -
                          ((char*) &(query_tables->next_global) -
                           (char*) query_tables)) :
           0;
  }

  
  enum enum_binlog_stmt_unsafe {
    
    BINLOG_STMT_UNSAFE_LIMIT= 0,
    
    BINLOG_STMT_UNSAFE_INSERT_DELAYED,
    
    BINLOG_STMT_UNSAFE_SYSTEM_TABLE,
    
    BINLOG_STMT_UNSAFE_AUTOINC_COLUMNS,
    
    BINLOG_STMT_UNSAFE_UDF,
    
    BINLOG_STMT_UNSAFE_SYSTEM_VARIABLE,
    
    BINLOG_STMT_UNSAFE_SYSTEM_FUNCTION,

    
    BINLOG_STMT_UNSAFE_NONTRANS_AFTER_TRANS,

    
    BINLOG_STMT_UNSAFE_MULTIPLE_ENGINES_AND_SELF_LOGGING_ENGINE,

    
    BINLOG_STMT_UNSAFE_MIXED_STATEMENT,

    
    BINLOG_STMT_UNSAFE_INSERT_IGNORE_SELECT,

    
    BINLOG_STMT_UNSAFE_INSERT_SELECT_UPDATE,

    
    BINLOG_STMT_UNSAFE_WRITE_AUTOINC_SELECT,

    
    BINLOG_STMT_UNSAFE_REPLACE_SELECT,

    
    BINLOG_STMT_UNSAFE_CREATE_IGNORE_SELECT,

    
    BINLOG_STMT_UNSAFE_CREATE_REPLACE_SELECT,

    
    BINLOG_STMT_UNSAFE_CREATE_SELECT_AUTOINC,

    
    BINLOG_STMT_UNSAFE_UPDATE_IGNORE,

    
    BINLOG_STMT_UNSAFE_INSERT_TWO_KEYS,

    
    BINLOG_STMT_UNSAFE_AUTOINC_NOT_FIRST,

    
    BINLOG_STMT_UNSAFE_COUNT
  };
  
  static const uint32 BINLOG_STMT_UNSAFE_ALL_FLAGS=
    ((1U << BINLOG_STMT_UNSAFE_COUNT) - 1);

  
  static const int binlog_stmt_unsafe_errcode[BINLOG_STMT_UNSAFE_COUNT];

  
  inline bool is_stmt_unsafe() const {
    return get_stmt_unsafe_flags() != 0;
  }

  inline bool is_stmt_unsafe(enum_binlog_stmt_unsafe unsafe)
  {
    return binlog_stmt_flags & (1 << unsafe);
  }

  
  inline void set_stmt_unsafe(enum_binlog_stmt_unsafe unsafe_type) {
    DBUG_ENTER("set_stmt_unsafe");
    DBUG_ASSERT(unsafe_type >= 0 && unsafe_type < BINLOG_STMT_UNSAFE_COUNT);
    binlog_stmt_flags|= (1U << unsafe_type);
    DBUG_VOID_RETURN;
  }

  
  inline void set_stmt_unsafe_flags(uint32 flags) {
    DBUG_ENTER("set_stmt_unsafe_flags");
    DBUG_ASSERT((flags & ~BINLOG_STMT_UNSAFE_ALL_FLAGS) == 0);
    binlog_stmt_flags|= flags;
    DBUG_VOID_RETURN;
  }

  
  inline uint32 get_stmt_unsafe_flags() const {
    DBUG_ENTER("get_stmt_unsafe_flags");
    DBUG_RETURN(binlog_stmt_flags & BINLOG_STMT_UNSAFE_ALL_FLAGS);
  }

  
  inline void clear_stmt_unsafe() {
    DBUG_ENTER("clear_stmt_unsafe");
    binlog_stmt_flags&= ~BINLOG_STMT_UNSAFE_ALL_FLAGS;
    DBUG_VOID_RETURN;
  }

  
  inline bool is_stmt_row_injection() const {
    return binlog_stmt_flags &
      (1U << (BINLOG_STMT_UNSAFE_COUNT + BINLOG_STMT_TYPE_ROW_INJECTION));
  }

  
  inline void set_stmt_row_injection() {
    DBUG_ENTER("set_stmt_row_injection");
    binlog_stmt_flags|=
      (1U << (BINLOG_STMT_UNSAFE_COUNT + BINLOG_STMT_TYPE_ROW_INJECTION));
    DBUG_VOID_RETURN;
  }

  enum enum_stmt_accessed_table
  {
    
    STMT_READS_TRANS_TABLE= 0,
    
    STMT_READS_NON_TRANS_TABLE,
    
    STMT_READS_TEMP_TRANS_TABLE,
    
    STMT_READS_TEMP_NON_TRANS_TABLE,
    
    STMT_WRITES_TRANS_TABLE,
    
    STMT_WRITES_NON_TRANS_TABLE,
    
    STMT_WRITES_TEMP_TRANS_TABLE,
    
    STMT_WRITES_TEMP_NON_TRANS_TABLE,
    
    STMT_ACCESS_TABLE_COUNT
  };

#ifndef DBUG_OFF
  static inline const char *stmt_accessed_table_string(enum_stmt_accessed_table accessed_table)
  {
    switch (accessed_table)
    {
      case STMT_READS_TRANS_TABLE:
         return "STMT_READS_TRANS_TABLE";
      break;
      case STMT_READS_NON_TRANS_TABLE:
        return "STMT_READS_NON_TRANS_TABLE";
      break;
      case STMT_READS_TEMP_TRANS_TABLE:
        return "STMT_READS_TEMP_TRANS_TABLE";
      break;
      case STMT_READS_TEMP_NON_TRANS_TABLE:
        return "STMT_READS_TEMP_NON_TRANS_TABLE";
      break;  
      case STMT_WRITES_TRANS_TABLE:
        return "STMT_WRITES_TRANS_TABLE";
      break;
      case STMT_WRITES_NON_TRANS_TABLE:
        return "STMT_WRITES_NON_TRANS_TABLE";
      break;
      case STMT_WRITES_TEMP_TRANS_TABLE:
        return "STMT_WRITES_TEMP_TRANS_TABLE";
      break;
      case STMT_WRITES_TEMP_NON_TRANS_TABLE:
        return "STMT_WRITES_TEMP_NON_TRANS_TABLE";
      break;
      case STMT_ACCESS_TABLE_COUNT:
      default:
        DBUG_ASSERT(0);
      break;
    }
    MY_ASSERT_UNREACHABLE();
    return "";
  }
#endif  
               
  #define BINLOG_DIRECT_ON 0xF0    

  #define BINLOG_DIRECT_OFF 0xF    

  #define TRX_CACHE_EMPTY 0x33     

  #define TRX_CACHE_NOT_EMPTY 0xCC 

  #define IL_LT_REPEATABLE 0xAA    

  #define IL_GTE_REPEATABLE 0x55   
  
  
  inline void set_stmt_accessed_table(enum_stmt_accessed_table accessed_table)
  {
    DBUG_ENTER("LEX::set_stmt_accessed_table");

    DBUG_ASSERT(accessed_table >= 0 && accessed_table < STMT_ACCESS_TABLE_COUNT);
    stmt_accessed_table_flag |= (1U << accessed_table);

    DBUG_VOID_RETURN;
  }

  
  inline bool stmt_accessed_table(enum_stmt_accessed_table accessed_table)
  {
    DBUG_ENTER("LEX::stmt_accessed_table");

    DBUG_ASSERT(accessed_table >= 0 && accessed_table < STMT_ACCESS_TABLE_COUNT);

    DBUG_RETURN((stmt_accessed_table_flag & (1U << accessed_table)) != 0);
  }

  
  inline bool stmt_accessed_non_trans_temp_table()
  {
    DBUG_ENTER("THD::stmt_accessed_non_trans_temp_table");

    DBUG_RETURN((stmt_accessed_table_flag &
                ((1U << STMT_READS_TEMP_NON_TRANS_TABLE) |
                 (1U << STMT_WRITES_TEMP_NON_TRANS_TABLE))) != 0);
  }

  
  inline bool is_mixed_stmt_unsafe(bool in_multi_stmt_transaction_mode,
                                   bool binlog_direct,
                                   bool trx_cache_is_not_empty,
                                   uint tx_isolation)
  {
    bool unsafe= FALSE;

    if (in_multi_stmt_transaction_mode)
    {
       uint condition=
         (binlog_direct ? BINLOG_DIRECT_ON : BINLOG_DIRECT_OFF) &
         (trx_cache_is_not_empty ? TRX_CACHE_NOT_EMPTY : TRX_CACHE_EMPTY) &
         (tx_isolation >= ISO_REPEATABLE_READ ? IL_GTE_REPEATABLE : IL_LT_REPEATABLE);

      unsafe= (binlog_unsafe_map[stmt_accessed_table_flag] & condition);

#if !defined(DBUG_OFF)
      DBUG_PRINT("LEX::is_mixed_stmt_unsafe", ("RESULT %02X %02X %02X\n", condition,
              binlog_unsafe_map[stmt_accessed_table_flag],
              (binlog_unsafe_map[stmt_accessed_table_flag] & condition)));
 
      int type_in= 0;
      for (; type_in < STMT_ACCESS_TABLE_COUNT; type_in++)
      {
        if (stmt_accessed_table((enum_stmt_accessed_table) type_in))
          DBUG_PRINT("LEX::is_mixed_stmt_unsafe", ("ACCESSED %s ",
                  stmt_accessed_table_string((enum_stmt_accessed_table) type_in)));
      }
#endif
    }

    if (stmt_accessed_table(STMT_WRITES_NON_TRANS_TABLE) &&
      stmt_accessed_table(STMT_READS_TRANS_TABLE) &&
      tx_isolation < ISO_REPEATABLE_READ)
      unsafe= TRUE;
    else if (stmt_accessed_table(STMT_WRITES_TEMP_NON_TRANS_TABLE) &&
      stmt_accessed_table(STMT_READS_TRANS_TABLE) &&
      tx_isolation < ISO_REPEATABLE_READ)
      unsafe= TRUE;

    return(unsafe);
  }

  
  bool uses_stored_routines() const
  { return sroutines_list.elements != 0; }

private:

  
  enum enum_binlog_stmt_type {
    
    BINLOG_STMT_TYPE_ROW_INJECTION = 0,

    
    BINLOG_STMT_TYPE_COUNT
  };

  
  uint32 binlog_stmt_flags;

  
  uint32 stmt_accessed_table_flag;
};




struct st_parsing_options
{
  bool allows_variable;

  st_parsing_options() { reset(); }
  void reset();
};



enum enum_comment_state
{
  
  NO_COMMENT,
  
  PRESERVE_COMMENT,
  
  DISCARD_COMMENT
};




class Lex_input_stream
{
  size_t unescape(CHARSET_INFO *cs, char *to,
                  const char *str, const char *end, int sep);
  my_charset_conv_wc_mb get_escape_func(THD *thd, my_wc_t sep) const;
public:
  Lex_input_stream()
  {
  }

  ~Lex_input_stream()
  {
  }

  
  bool init(THD *thd, char *buff, size_t length);

  void reset(char *buff, size_t length);

  
  int lex_token(union YYSTYPE *yylval, THD *thd);

  void reduce_digest_token(uint token_left, uint token_right);

private:
  
  void set_echo(bool echo)
  {
    m_echo= echo;
  }

  void save_in_comment_state()
  {
    m_echo_saved= m_echo;
    in_comment_saved= in_comment;
  }

  void restore_in_comment_state()
  {
    m_echo= m_echo_saved;
    in_comment= in_comment_saved;
  }

  
  void skip_binary(int n)
  {
    if (m_echo)
    {
      memcpy(m_cpp_ptr, m_ptr, n);
      m_cpp_ptr += n;
    }
    m_ptr += n;
  }

  
  unsigned char yyGet()
  {
    char c= *m_ptr++;
    if (m_echo)
      *m_cpp_ptr++ = c;
    return c;
  }

  
  unsigned char yyGetLast()
  {
    return m_ptr[-1];
  }

  
  unsigned char yyPeek()
  {
    return m_ptr[0];
  }

  
  unsigned char yyPeekn(int n)
  {
    return m_ptr[n];
  }

  
  void yyUnget()
  {
    m_ptr--;
    if (m_echo)
      m_cpp_ptr--;
  }

  
  void yySkip()
  {
    if (m_echo)
      *m_cpp_ptr++ = *m_ptr++;
    else
      m_ptr++;
  }

  
  void yySkipn(int n)
  {
    if (m_echo)
    {
      memcpy(m_cpp_ptr, m_ptr, n);
      m_cpp_ptr += n;
    }
    m_ptr += n;
  }

  
  char *yyUnput(char ch)
  {
    *--m_ptr= ch;
    if (m_echo)
      m_cpp_ptr--;
    return m_ptr;
  }

  
  bool eof(int n)
  {
    return ((m_ptr + n) >= m_end_of_query);
  }

  
  void start_token()
  {
    m_tok_start_prev= m_tok_start;
    m_tok_start= m_ptr;
    m_tok_end= m_ptr;

    m_cpp_tok_start_prev= m_cpp_tok_start;
    m_cpp_tok_start= m_cpp_ptr;
    m_cpp_tok_end= m_cpp_ptr;
  }

  
  void restart_token()
  {
    m_tok_start= m_ptr;
    m_cpp_tok_start= m_cpp_ptr;
  }

  
  size_t get_body_utf8_maximum_length(THD *thd);

  
  uint yyLength()
  {
    
    DBUG_ASSERT(m_ptr > m_tok_start);
    return (uint) ((m_ptr - m_tok_start) - 1);
  }

  
  bool has_lookahead() const
  {
    return lookahead_token >= 0;
  }

public:

  
  bool eof()
  {
    return (m_ptr >= m_end_of_query);
  }

  
  const char *get_buf()
  {
    return m_buf;
  }

  
  const char *get_cpp_buf()
  {
    return m_cpp_buf;
  }

  
  const char *get_end_of_query()
  {
    return m_end_of_query;
  }

  
  const char *get_tok_start()
  {
    return has_lookahead() ? m_tok_start_prev : m_tok_start;
  }

  void set_cpp_tok_start(const char *pos)
  {
    m_cpp_tok_start= pos;
  }

  
  const char *get_tok_end()
  {
    return m_tok_end;
  }

  
  const char *get_ptr()
  {
    return m_ptr;
  }

  
  const char *get_cpp_tok_start()
  {
    return has_lookahead() ? m_cpp_tok_start_prev : m_cpp_tok_start;
  }

  
  const char *get_cpp_tok_end()
  {
    return m_cpp_tok_end;
  }

  
  const char *get_cpp_tok_end_rtrim()
  {
    const char *p;
    for (p= m_cpp_tok_end;
         p > m_cpp_buf && my_isspace(system_charset_info, p[-1]);
         p--)
    { }
    return p;
  }

  
  const char *get_cpp_ptr()
  {
    return m_cpp_ptr;
  }

  
  const char *get_cpp_ptr_rtrim()
  {
    const char *p;
    for (p= m_cpp_ptr;
         p > m_cpp_buf && my_isspace(system_charset_info, p[-1]);
         p--)
    { }
    return p;
  }
  
  const char *get_body_utf8_str()
  {
    return m_body_utf8;
  }

  
  size_t get_body_utf8_length()
  {
    return (size_t) (m_body_utf8_ptr - m_body_utf8);
  }

  void body_utf8_start(THD *thd, const char *begin_ptr);
  void body_utf8_append(const char *ptr);
  void body_utf8_append(const char *ptr, const char *end_ptr);
  void body_utf8_append_ident(THD *thd,
                              const Lex_string_with_metadata_st *txt,
                              const char *end_ptr);
  void body_utf8_append_escape(THD *thd,
                               const LEX_CSTRING *txt,
                               CHARSET_INFO *txt_cs,
                               const char *end_ptr,
                               my_wc_t sep);

private:
  
  int lookahead_token;

  
  LEX_YYSTYPE lookahead_yylval;

  bool get_text(Lex_string_with_metadata_st *to,
                uint sep, int pre_skip, int post_skip);

  void add_digest_token(uint token, LEX_YYSTYPE yylval);

  bool consume_comment(int remaining_recursions_permitted);
  int lex_one_token(union YYSTYPE *yylval, THD *thd);
  int find_keyword(Lex_ident_cli_st *str, uint len, bool function);
  LEX_CSTRING get_token(uint skip, uint length);
  int scan_ident_sysvar(THD *thd, Lex_ident_cli_st *str);
  int scan_ident_start(THD *thd, Lex_ident_cli_st *str);
  int scan_ident_middle(THD *thd, Lex_ident_cli_st *str,
                        CHARSET_INFO **cs, my_lex_states *);
  int scan_ident_delimited(THD *thd, Lex_ident_cli_st *str);
  bool get_7bit_or_8bit_ident(THD *thd, uchar *last_char);

  
  THD *m_thd;

  
  char *m_ptr;

  
  const char *m_tok_start;

  
  const char *m_tok_end;

  
  const char *m_end_of_query;

  
  const char *m_tok_start_prev;

  
  const char *m_buf;

  
  size_t m_buf_length;

  
  bool m_echo;
  bool m_echo_saved;

  
  char *m_cpp_buf;

  
  char *m_cpp_ptr;

  
  const char *m_cpp_tok_start;

  
  const char *m_cpp_tok_start_prev;

  
  const char *m_cpp_tok_end;

  
  char *m_body_utf8;

  
  char *m_body_utf8_ptr;

  
  const char *m_cpp_utf8_processed_ptr;

public:

  
  enum my_lex_states next_state;

  
  const char *found_semicolon;

  
  bool ignore_space;

  
  bool stmt_prepare_mode;
  
  bool multi_statements;

  
  uint yylineno;

  
  sql_digest_state* m_digest;

private:
  
  enum_comment_state in_comment;
  enum_comment_state in_comment_saved;

  
  const char *m_cpp_text_start;

  
  const char *m_cpp_text_end;

  
  CHARSET_INFO *m_underscore_cs;
};



class Sql_statement : public Sql_alloc
{
public:
  
  virtual bool execute(THD *thd) = 0;

protected:
  
  Sql_statement(LEX *lex)
    : m_lex(lex)
  {}

  
  virtual ~Sql_statement()
  {
    
    DBUG_ASSERT(FALSE);
  }

protected:
  
  LEX *m_lex;
};


class Delete_plan;
class SQL_SELECT;

class Explain_query;
class Explain_update;
class Explain_delete;



class Update_plan
{
protected:
  bool impossible_where;
  bool no_partitions;
public:
  
  bool updating_a_view;
   
  
  MEM_ROOT *mem_root;

  TABLE *table;
  SQL_SELECT *select;
  uint index;
  ha_rows scanned_rows;
  
  SELECT_LEX *select_lex;
  
  key_map possible_keys;
  bool using_filesort;
  bool using_io_buffer;
  
  
  void set_impossible_where() { impossible_where= true; }
  void set_no_partitions() { no_partitions= true; }

  Explain_update* save_explain_update_data(MEM_ROOT *mem_root, THD *thd);
protected:
  bool save_explain_data_intern(MEM_ROOT *mem_root, Explain_update *eu, bool is_analyze);
public:
  virtual ~Update_plan() {}

  Update_plan(MEM_ROOT *mem_root_arg) : 
    impossible_where(false), no_partitions(false), 
    mem_root(mem_root_arg), 
    using_filesort(false), using_io_buffer(false)
  {}
};



class Delete_plan : public Update_plan
{
  bool deleting_all_rows;
public:

  
  Delete_plan(MEM_ROOT *mem_root_arg) : 
    Update_plan(mem_root_arg), 
    deleting_all_rows(false)
  {}

  
  void set_delete_all_rows(ha_rows rows_arg) 
  { 
    deleting_all_rows= true;
    scanned_rows= rows_arg;
  }
  void cancel_delete_all_rows()
  {
    deleting_all_rows= false;
  }

  Explain_delete* save_explain_delete_data(MEM_ROOT *mem_root, THD *thd);
};


class Query_arena_memroot;


struct LEX: public Query_tables_list
{
  SELECT_LEX_UNIT unit;                         
  SELECT_LEX select_lex;                        
  
  SELECT_LEX *current_select;
  
  SELECT_LEX *all_selects_list;
  
  With_clause *curr_with_clause;  
  
  With_clause *with_clauses_list;
  
  With_clause **with_clauses_list_last_next;
  
  my_ptrdiff_t clone_spec_offset;

  Create_view_info *create_view;

  
  Explain_query *explain;

  
  CHARSET_INFO *charset;
  
  LEX *stmt_lex;

  LEX_CSTRING name;
  const char *help_arg;
  const char *backup_dir;                       
  const char* to_log;                           
  const char* x509_subject,*x509_issuer,*ssl_cipher;
  String *wild;  
  sql_exchange *exchange;
  select_result *result;
  
  LEX_CSTRING comment, ident;
  LEX_USER *grant_user;
  XID *xid;
  THD *thd;

  
  DYNAMIC_ARRAY plugins;
  plugin_ref plugins_static_buffer[INITIAL_LEX_PLUGIN_LIST_SIZE];

  
  LEX_STRING create_view_select;

  uint current_select_number; 

  
  const char* raw_trg_on_table_name_begin;
  
  const char* raw_trg_on_table_name_end;

  
  partition_info *part_info;

  
  LEX_USER *definer;

  Table_type table_type;                        
  List<Key_part_spec> ref_list;
  List<LEX_USER>      users_list;
  List<LEX_COLUMN>    columns;
  List<Item>          *insert_list,field_list,value_list,update_list;
  List<List_item>     many_values;
  List<set_var_base>  var_list;
  List<set_var_base>  stmt_var_list; 
  List<set_var_base>  old_var_list; 
private:
  Query_arena_memroot *arena_for_set_stmt;
  MEM_ROOT *mem_root_for_set_stmt;
  bool sp_block_finalize(THD *thd, const Lex_spblock_st spblock,
                                   class sp_label **splabel);
  bool sp_change_context(THD *thd, const sp_pcontext *ctx, bool exclusive);
  bool sp_exit_block(THD *thd, sp_label *lab);
  bool sp_exit_block(THD *thd, sp_label *lab, Item *when);

  bool sp_continue_loop(THD *thd, sp_label *lab);
  bool sp_continue_loop(THD *thd, sp_label *lab, Item *when);

  bool sp_for_loop_condition(THD *thd, const Lex_for_loop_st &loop);
  bool sp_for_loop_increment(THD *thd, const Lex_for_loop_st &loop);

public:
  void parse_error(uint err_number= ER_SYNTAX_ERROR);
  inline bool is_arena_for_set_stmt() {return arena_for_set_stmt != 0;}
  bool set_arena_for_set_stmt(Query_arena *backup);
  void reset_arena_for_set_stmt(Query_arena *backup);
  void free_arena_for_set_stmt();

  List<Item_func_set_user_var> set_var_list; 
  List<Item_param>    param_list;
  List<LEX_CSTRING>   view_list; 
  List<LEX_CSTRING>   with_column_list; 
  List<LEX_STRING>   *column_list; 
  List<LEX_STRING>   *index_list;  
  
  List<Name_resolution_context> context_stack;

  SQL_I_List<ORDER> proc_list;
  SQL_I_List<TABLE_LIST> auxiliary_table_list, save_list;
  Column_definition *last_field;
  Item_sum *in_sum_func;
  udf_func udf;
  HA_CHECK_OPT   check_opt;                        
  Table_specification_st create_info;
  Key *last_key;
  LEX_MASTER_INFO mi;                              
  LEX_SERVER_OPTIONS server_options;
  LEX_CSTRING relay_log_connection_name;
  USER_RESOURCES mqh;
  LEX_RESET_SLAVE reset_slave_info;
  ulonglong type;
  ulong next_binlog_file_number;
  
  killed_state kill_signal;
  killed_type  kill_type;
  
  nesting_map allow_sum_func;

  Sql_cmd *m_sql_cmd;

  
  bool expr_allows_subselect;
  
  bool parse_vcol_expr;

  enum SSL_type ssl_type;                       
  enum enum_duplicates duplicates;
  enum enum_tx_isolation tx_isolation;
  enum enum_ha_read_modes ha_read_mode;
  union {
    enum ha_rkey_function ha_rkey_mode;
    enum xa_option_words xa_opt;
    bool with_admin_option;                     
    bool with_persistent_for_clause; 
  };
  enum enum_var_type option_type;
  enum enum_drop_mode drop_mode;

  uint profile_query_id;
  uint profile_options;
  uint grant, grant_tot_col, which_columns;
  enum Foreign_key::fk_match_opt fk_match_option;
  enum_fk_option fk_update_opt;
  enum_fk_option fk_delete_opt;
  uint slave_thd_opt, start_transaction_opt;
  int nest_level;
  
  uint table_count;
  uint8 describe;
  bool  analyze_stmt; 
  bool  explain_json;
  
  uint8 derived_tables;
  uint8 context_analysis_only;
  bool local_file;
  bool check_exists;
  bool autocommit;
  bool verbose, no_write_to_binlog;

  enum enum_yes_no_unknown tx_chain, tx_release;
  bool safe_to_cache_query;
  bool ignore;
  st_parsing_options parsing_options;
  Alter_info alter_info;
  
  TABLE_LIST *create_last_non_select_table;
  
  LEX_CSTRING prepared_stmt_name; 
  
  Item *prepared_stmt_code;
  
  List<Item> prepared_stmt_params;
  sp_head *sphead;
  sp_name *spname;
  bool sp_lex_in_use;   
  bool all_privileges;

  sp_pcontext *spcont;

  st_sp_chistics sp_chistics;

  Event_parse_data *event_parse_data;

  
  bool empty_field_list_on_rset;
  
  st_trg_chistics trg_chistics;
  
  SQL_I_List<Item_trigger_field> trg_table_fields;

  
  union {
    const char *stmt_definition_begin;
    uint keyword_delayed_begin_offset;
  };

  union {
    const char *stmt_definition_end;
    uint keyword_delayed_end_offset;
  };

  
  engine_option_value *option_list;

  
  engine_option_value *option_list_last;

  
  bool use_only_table_context;

  
  st_alter_tablespace *alter_tablespace_info;
  
  bool escape_used;
  bool default_used;    
  bool is_lex_started; 

  
  table_map  used_tables;
  
  Item *limit_rows_examined;
  ulonglong limit_rows_examined_cnt;
  
  DYNAMIC_ARRAY delete_gtid_domain;
  static const ulong initial_gtid_domain_buffer_size= 16;
  uint32 gtid_domain_static_buffer[initial_gtid_domain_buffer_size];

  inline void set_limit_rows_examined()
  {
    if (limit_rows_examined)
      limit_rows_examined_cnt= limit_rows_examined->val_uint();
    else
      limit_rows_examined_cnt= ULONGLONG_MAX;
  }


  SQL_I_List<ORDER> save_group_list;
  SQL_I_List<ORDER> save_order_list;
  LEX_CSTRING *win_ref;
  Window_frame *win_frame;
  Window_frame_bound *frame_top_bound;
  Window_frame_bound *frame_bottom_bound;
  Window_spec *win_spec;

  
  vers_select_conds_t vers_conditions;

  inline void free_set_stmt_mem_root()
  {
    DBUG_ASSERT(!is_arena_for_set_stmt());
    if (mem_root_for_set_stmt)
    {
      free_root(mem_root_for_set_stmt, MYF(0));
      delete mem_root_for_set_stmt;
      mem_root_for_set_stmt= 0;
    }
  }

  LEX();

  virtual ~LEX()
  {
    free_set_stmt_mem_root();
    destroy_query_tables_list();
    plugin_unlock_list(NULL, (plugin_ref *)plugins.buffer, plugins.elements);
    delete_dynamic(&plugins);
  }

  virtual class Query_arena *query_arena()
  {
    DBUG_ASSERT(0);
    return NULL;
  }

  void start(THD *thd);

  inline bool is_ps_or_view_context_analysis()
  {
    return (context_analysis_only &
            (CONTEXT_ANALYSIS_ONLY_PREPARE |
             CONTEXT_ANALYSIS_ONLY_VCOL_EXPR |
             CONTEXT_ANALYSIS_ONLY_VIEW));
  }

  inline bool is_view_context_analysis()
  {
    return (context_analysis_only & CONTEXT_ANALYSIS_ONLY_VIEW);
  }

  inline void uncacheable(uint8 cause)
  {
    safe_to_cache_query= 0;

    if (current_select) 
    {
      
      SELECT_LEX *sl;
      SELECT_LEX_UNIT *un;
      for (sl= current_select, un= sl->master_unit();
          un != &unit;
          sl= sl->outer_select(), un= sl->master_unit())
      {
        sl->uncacheable|= cause;
        un->uncacheable|= cause;
      }
      select_lex.uncacheable|= cause;
    }
  }
  void set_trg_event_type_for_tables();

  TABLE_LIST *unlink_first_table(bool *link_to_local);
  void link_first_table_back(TABLE_LIST *first, bool link_to_local);
  void first_lists_tables_same();

  bool can_be_merged();
  bool can_use_merged();
  bool can_not_use_merged();
  bool only_view_structure();
  bool need_correct_ident();
  uint8 get_effective_with_check(TABLE_LIST *view);
  
  inline bool which_check_option_applicable()
  {
    switch (sql_command) {
    case SQLCOM_UPDATE:
    case SQLCOM_UPDATE_MULTI:
    case SQLCOM_DELETE:
    case SQLCOM_DELETE_MULTI:
    case SQLCOM_INSERT:
    case SQLCOM_INSERT_SELECT:
    case SQLCOM_REPLACE:
    case SQLCOM_REPLACE_SELECT:
    case SQLCOM_LOAD:
      return TRUE;
    default:
      return FALSE;
    }
  }

  void cleanup_after_one_table_open();

  bool push_context(Name_resolution_context *context, MEM_ROOT *mem_root)
  {
    return context_stack.push_front(context, mem_root);
  }

  void pop_context()
  {
    context_stack.pop();
  }

  bool copy_db_to(LEX_CSTRING *to);

  Name_resolution_context *current_context()
  {
    return context_stack.head();
  }
  
  static void cleanup_lex_after_parse_error(THD *thd);

  void reset_n_backup_query_tables_list(Query_tables_list *backup);
  void restore_backup_query_tables_list(Query_tables_list *backup);

  bool table_or_sp_used();

  bool is_partition_management() const;
  bool part_values_current(THD *thd);
  bool part_values_history(THD *thd);

  
  bool is_single_level_stmt() 
  { 
     
    if (&select_lex == all_selects_list && !sroutines.records)
    {
      DBUG_ASSERT(!all_selects_list->next_select_in_list());
      return TRUE;
    }
    return FALSE;
  }

  bool save_prep_leaf_tables();

  int print_explain(select_result_sink *output, uint8 explain_flags,
                    bool is_analyze, bool *printed_anything);
  void restore_set_statement_var();

  void init_last_field(Column_definition *field, const LEX_CSTRING *name,
                       const CHARSET_INFO *cs);
  bool last_field_generated_always_as_row_start_or_end(Lex_ident *p,
                                                       const char *type,
                                                       uint flags);
  bool last_field_generated_always_as_row_start();
  bool last_field_generated_always_as_row_end();
  bool set_bincmp(CHARSET_INFO *cs, bool bin);

  bool get_dynamic_sql_string(LEX_CSTRING *dst, String *buffer);
  bool prepared_stmt_params_fix_fields(THD *thd)
  {
    
    List_iterator_fast<Item> param_it(prepared_stmt_params);
    while (Item *param= param_it++)
    {
      if (param->fix_fields_if_needed_for_scalar(thd, 0))
        return true;
    }
    return false;
  }
  sp_variable *sp_param_init(LEX_CSTRING *name);
  bool sp_param_fill_definition(sp_variable *spvar);

  int case_stmt_action_expr(Item* expr);
  int case_stmt_action_when(Item *when, bool simple);
  int case_stmt_action_then();
  bool add_select_to_union_list(bool is_union_distinct,
                                enum sub_select_type type,
                                bool is_top_level);
  bool setup_select_in_parentheses();
  bool set_trigger_new_row(const LEX_CSTRING *name, Item *val);
  bool set_trigger_field(const LEX_CSTRING *name1, const LEX_CSTRING *name2,
                         Item *val);
  bool set_system_variable(enum_var_type var_type, sys_var *var,
                           const LEX_CSTRING *base_name, Item *val);
  bool set_system_variable(enum_var_type var_type, const LEX_CSTRING *name,
                           Item *val);
  bool set_system_variable(THD *thd, enum_var_type var_type,
                           const LEX_CSTRING *name1,
                           const LEX_CSTRING *name2,
                           Item *val);
  bool set_default_system_variable(enum_var_type var_type,
                                   const LEX_CSTRING *name,
                                   Item *val);
  bool set_user_variable(THD *thd, const LEX_CSTRING *name, Item *val);
  void set_stmt_init();
  sp_name *make_sp_name(THD *thd, const LEX_CSTRING *name);
  sp_name *make_sp_name(THD *thd, const LEX_CSTRING *name1,
                                  const LEX_CSTRING *name2);
  sp_name *make_sp_name_package_routine(THD *thd, const LEX_CSTRING *name);
  sp_head *make_sp_head(THD *thd, const sp_name *name, const Sp_handler *sph);
  sp_head *make_sp_head_no_recursive(THD *thd, const sp_name *name,
                                     const Sp_handler *sph);
  sp_head *make_sp_head_no_recursive(THD *thd,
                                     DDL_options_st options, sp_name *name,
                                     const Sp_handler *sph)
  {
    if (add_create_options_with_check(options))
      return NULL;
    return make_sp_head_no_recursive(thd, name, sph);
  }
  bool sp_body_finalize_function(THD *);
  bool sp_body_finalize_procedure(THD *);
  sp_package *create_package_start(THD *thd,
                                   enum_sql_command command,
                                   const Sp_handler *sph,
                                   const sp_name *name,
                                   DDL_options_st options);
  bool create_package_finalize(THD *thd,
                               const sp_name *name,
                               const sp_name *name2,
                               const char *body_start,
                               const char *body_end);
  bool call_statement_start(THD *thd, sp_name *name);
  bool call_statement_start(THD *thd, const LEX_CSTRING *name);
  bool call_statement_start(THD *thd, const LEX_CSTRING *name1,
                                      const LEX_CSTRING *name2);
  sp_variable *find_variable(const LEX_CSTRING *name,
                             sp_pcontext **ctx,
                             const Sp_rcontext_handler **rh) const;
  sp_variable *find_variable(const LEX_CSTRING *name,
                             const Sp_rcontext_handler **rh) const
  {
    sp_pcontext *not_used_ctx;
    return find_variable(name, &not_used_ctx, rh);
  }
  bool set_variable(const LEX_CSTRING *name, Item *item);
  bool set_variable(const LEX_CSTRING *name1, const LEX_CSTRING *name2,
                    Item *item);
  void sp_variable_declarations_init(THD *thd, int nvars);
  bool sp_variable_declarations_finalize(THD *thd, int nvars,
                                         const Column_definition *cdef,
                                         Item *def);
  bool sp_variable_declarations_set_default(THD *thd, int nvars, Item *def);
  bool sp_variable_declarations_row_finalize(THD *thd, int nvars,
                                             Row_definition_list *row,
                                             Item *def);
  bool sp_variable_declarations_with_ref_finalize(THD *thd, int nvars,
                                                  Qualified_column_ident *col,
                                                  Item *def);
  bool sp_variable_declarations_rowtype_finalize(THD *thd, int nvars,
                                                 Qualified_column_ident *,
                                                 Item *def);
  bool sp_variable_declarations_cursor_rowtype_finalize(THD *thd, int nvars,
                                                        uint offset,
                                                        Item *def);
  bool sp_variable_declarations_table_rowtype_finalize(THD *thd, int nvars,
                                                       const LEX_CSTRING &db,
                                                       const LEX_CSTRING &table,
                                                       Item *def);
  bool sp_variable_declarations_column_type_finalize(THD *thd, int nvars,
                                                     Qualified_column_ident *ref,
                                                     Item *def);
  bool sp_variable_declarations_vartype_finalize(THD *thd, int nvars,
                                                 const LEX_CSTRING &name,
                                                 Item *def);
  bool sp_variable_declarations_copy_type_finalize(THD *thd, int nvars,
                                                   const Column_definition &ref,
                                                   Row_definition_list *fields,
                                                   Item *def);
  bool sp_handler_declaration_init(THD *thd, int type);
  bool sp_handler_declaration_finalize(THD *thd, int type);

  bool sp_declare_cursor(THD *thd, const LEX_CSTRING *name,
                         class sp_lex_cursor *cursor_stmt,
                         sp_pcontext *param_ctx, bool add_cpush_instr);

  bool sp_open_cursor(THD *thd, const LEX_CSTRING *name,
                      List<sp_assignment_lex> *parameters);
  Item_splocal *create_item_for_sp_var(const Lex_ident_cli_st *name,
                                       sp_variable *spvar);

  Item *create_item_qualified_asterisk(THD *thd, const Lex_ident_sys_st *name);
  Item *create_item_qualified_asterisk(THD *thd,
                                       const Lex_ident_sys_st *a,
                                       const Lex_ident_sys_st *b);
  Item *create_item_qualified_asterisk(THD *thd, const Lex_ident_cli_st *cname)
  {
    Lex_ident_sys name(thd, cname);
    if (name.is_null())
      return NULL; 
    return create_item_qualified_asterisk(thd, &name);
  }
  Item *create_item_qualified_asterisk(THD *thd,
                                       const Lex_ident_cli_st *ca,
                                       const Lex_ident_cli_st *cb)
  {
    Lex_ident_sys a(thd, ca), b(thd, cb);
    if (a.is_null() || b.is_null())
      return NULL; 
    return create_item_qualified_asterisk(thd, &a, &b);
  }

  Item *create_item_ident_nosp(THD *thd, Lex_ident_sys_st *name);
  Item *create_item_ident_sp(THD *thd, Lex_ident_sys_st *name,
                             const char *start, const char *end);
  Item *create_item_ident(THD *thd, Lex_ident_cli_st *cname)
  {
    Lex_ident_sys name(thd, cname);
    if (name.is_null())
      return NULL; 
    return sphead ?
           create_item_ident_sp(thd, &name, cname->pos(), cname->end()) :
           create_item_ident_nosp(thd, &name);
  }
  
  Item *create_item_ident_nospvar(THD *thd,
                                  const Lex_ident_sys_st *a,
                                  const Lex_ident_sys_st *b);
  
  Item_splocal *create_item_spvar_row_field(THD *thd,
                                            const Sp_rcontext_handler *rh,
                                            const Lex_ident_sys *var,
                                            const Lex_ident_sys *field,
                                            sp_variable *spvar,
                                            const char *start,
                                            const char *end);
  
  Item *create_item_ident(THD *thd,
                          const Lex_ident_cli_st *a,
                          const Lex_ident_cli_st *b);
  
  Item *create_item_ident(THD *thd,
                          const Lex_ident_sys_st *a,
                          const Lex_ident_sys_st *b,
                          const Lex_ident_sys_st *c);

  Item *create_item_ident(THD *thd,
                          const Lex_ident_cli_st *ca,
                          const Lex_ident_cli_st *cb,
                          const Lex_ident_cli_st *cc)
  {
    Lex_ident_sys b(thd, cb), c(thd, cc);
    if (b.is_null() || c.is_null())
      return NULL;
    if (ca->pos() == cb->pos())  
    {
      DBUG_ASSERT(ca->length == 0);
      Lex_ident_sys none;
      return create_item_ident(thd, &none, &b, &c);
    }
    Lex_ident_sys a(thd, ca);
    return a.is_null() ? NULL : create_item_ident(thd, &a, &b, &c);
  }

  
  Item *create_item_func_nextval(THD *thd, Table_ident *ident);
  Item *create_item_func_nextval(THD *thd, const LEX_CSTRING *db,
                                           const LEX_CSTRING *name);
  
  Item *create_item_func_lastval(THD *thd, Table_ident *ident);
  Item *create_item_func_lastval(THD *thd, const LEX_CSTRING *db,
                                           const LEX_CSTRING *name);
  
  
  Item *create_item_func_setval(THD *thd, Table_ident *ident, longlong value,
                                ulonglong round, bool is_used);

  
  Item *create_item_limit(THD *thd, const Lex_ident_cli_st *var_name);

  
  Item *create_item_limit(THD *thd,
                          const Lex_ident_cli_st *var_name,
                          const Lex_ident_cli_st *field_name);

  Item *make_item_func_replace(THD *thd, Item *org, Item *find, Item *replace);
  Item *make_item_func_substr(THD *thd, Item *a, Item *b, Item *c);
  Item *make_item_func_substr(THD *thd, Item *a, Item *b);
  Item *make_item_func_call_generic(THD *thd, Lex_ident_cli_st *db,
                                    Lex_ident_cli_st *name, List<Item> *args);
  my_var *create_outvar(THD *thd, const LEX_CSTRING *name);

  
  my_var *create_outvar(THD *thd,
                        const LEX_CSTRING *var_name,
                        const LEX_CSTRING *field_name);

  bool is_trigger_new_or_old_reference(const LEX_CSTRING *name) const;

  Item *create_and_link_Item_trigger_field(THD *thd, const LEX_CSTRING *name,
                                           bool new_row);
  
  Item *make_item_colon_ident_ident(THD *thd,
                                    const Lex_ident_cli_st *a,
                                    const Lex_ident_cli_st *b);
  
  Item *make_item_plsql_cursor_attr(THD *thd, const LEX_CSTRING *name,
                                    plsql_cursor_attr_t attr);

  
  Item *make_item_sysvar(THD *thd,
                         enum_var_type type,
                         const LEX_CSTRING *name)
  {
    return make_item_sysvar(thd, type, name, &null_clex_str);
  }
  Item *make_item_sysvar(THD *thd,
                         enum_var_type type,
                         const LEX_CSTRING *name,
                         const LEX_CSTRING *component);
  void sp_block_init(THD *thd, const LEX_CSTRING *label);
  void sp_block_init(THD *thd)
  {
    
    sp_block_init(thd, &empty_clex_str);
  }
  bool sp_block_finalize(THD *thd, const Lex_spblock_st spblock)
  {
    class sp_label *tmp;
    return sp_block_finalize(thd, spblock, &tmp);
  }
  bool sp_block_finalize(THD *thd)
  {
    return sp_block_finalize(thd, Lex_spblock());
  }
  bool sp_block_finalize(THD *thd, const Lex_spblock_st spblock,
                                   const LEX_CSTRING *end_label);
  bool sp_block_finalize(THD *thd, const LEX_CSTRING *end_label)
  {
    return sp_block_finalize(thd, Lex_spblock(), end_label);
  }
  bool sp_declarations_join(Lex_spblock_st *res,
                            const Lex_spblock_st b1,
                            const Lex_spblock_st b2) const
  {
    if ((b2.vars || b2.conds) && (b1.curs || b1.hndlrs))
    {
      my_error(ER_SP_VARCOND_AFTER_CURSHNDLR, MYF(0));
      return true;
    }
    if (b2.curs && b1.hndlrs)
    {
      my_error(ER_SP_CURSOR_AFTER_HANDLER, MYF(0));
      return true;
    }
    res->join(b1, b2);
    return false;
  }
  bool sp_block_with_exceptions_finalize_declarations(THD *thd);
  bool sp_block_with_exceptions_finalize_executable_section(THD *thd,
                                                  uint executable_section_ip);
  bool sp_block_with_exceptions_finalize_exceptions(THD *thd,
                                                  uint executable_section_ip,
                                                  uint exception_count);
  bool sp_block_with_exceptions_add_empty(THD *thd);
  bool sp_exit_statement(THD *thd, Item *when);
  bool sp_exit_statement(THD *thd, const LEX_CSTRING *label_name, Item *item);
  bool sp_leave_statement(THD *thd, const LEX_CSTRING *label_name);
  bool sp_goto_statement(THD *thd, const LEX_CSTRING *label_name);

  bool sp_continue_statement(THD *thd, Item *when);
  bool sp_continue_statement(THD *thd, const LEX_CSTRING *label_name, Item *when);
  bool sp_iterate_statement(THD *thd, const LEX_CSTRING *label_name);

  bool maybe_start_compound_statement(THD *thd);
  bool sp_push_loop_label(THD *thd, const LEX_CSTRING *label_name);
  bool sp_push_loop_empty_label(THD *thd);
  bool sp_pop_loop_label(THD *thd, const LEX_CSTRING *label_name);
  void sp_pop_loop_empty_label(THD *thd);
  bool sp_while_loop_expression(THD *thd, Item *expr);
  bool sp_while_loop_finalize(THD *thd);
  bool sp_push_goto_label(THD *thd, const LEX_CSTRING *label_name);

  Item_param *add_placeholder(THD *thd, const LEX_CSTRING *name,
                              const char *start, const char *end);

  
  sp_variable *sp_add_for_loop_variable(THD *thd, const LEX_CSTRING *name,
                                        Item *value);
  sp_variable *sp_add_for_loop_target_bound(THD *thd, Item *value)
  {
    LEX_CSTRING name= { STRING_WITH_LEN("[target_bound]") };
    return sp_add_for_loop_variable(thd, &name, value);
  }
  bool sp_for_loop_intrange_declarations(THD *thd, Lex_for_loop_st *loop,
                                        const LEX_CSTRING *index,
                                        const Lex_for_loop_bounds_st &bounds);
  bool sp_for_loop_intrange_condition_test(THD *thd, const Lex_for_loop_st &loop);
  bool sp_for_loop_intrange_finalize(THD *thd, const Lex_for_loop_st &loop);

  
  bool sp_for_loop_cursor_declarations(THD *thd, Lex_for_loop_st *loop,
                                       const LEX_CSTRING *index,
                                       const Lex_for_loop_bounds_st &bounds);
  sp_variable *sp_add_for_loop_cursor_variable(THD *thd,
                                               const LEX_CSTRING *name,
                                               const class sp_pcursor *cur,
                                               uint coffset,
                                               sp_assignment_lex *param_lex,
                                               Item_args *parameters);
  bool sp_for_loop_implicit_cursor_statement(THD *thd,
                                             Lex_for_loop_bounds_st *bounds,
                                             sp_lex_cursor *cur);
  bool sp_for_loop_cursor_condition_test(THD *thd, const Lex_for_loop_st &loop);
  bool sp_for_loop_cursor_finalize(THD *thd, const Lex_for_loop_st &);

  

  
  bool sp_for_loop_declarations(THD *thd, Lex_for_loop_st *loop,
                                const LEX_CSTRING *index,
                                const Lex_for_loop_bounds_st &bounds)
  {
    return bounds.is_for_loop_cursor() ?
           sp_for_loop_cursor_declarations(thd, loop, index, bounds) :
           sp_for_loop_intrange_declarations(thd, loop, index, bounds);
  }

  
  bool sp_for_loop_condition_test(THD *thd, const Lex_for_loop_st &loop)
  {
    return loop.is_for_loop_cursor() ?
           sp_for_loop_cursor_condition_test(thd, loop) :
           sp_for_loop_intrange_condition_test(thd, loop);
  }

  
  bool sp_for_loop_finalize(THD *thd, const Lex_for_loop_st &loop)
  {
    return loop.is_for_loop_cursor() ?
           sp_for_loop_cursor_finalize(thd, loop) :
           sp_for_loop_intrange_finalize(thd, loop);
  }
  bool sp_for_loop_outer_block_finalize(THD *thd, const Lex_for_loop_st &loop);
  

  bool add_signal_statement(THD *thd, const class sp_condition_value *value);
  bool add_resignal_statement(THD *thd, const class sp_condition_value *value);

  
  bool check_add_key(DDL_options_st ddl)
  {
    if (ddl.if_not_exists() && sql_command != SQLCOM_ALTER_TABLE)
    {
      parse_error();
      return true;
    }
    return false;
  }
  
  bool add_key(Key::Keytype key_type, const LEX_CSTRING *key_name,
               ha_key_alg algorithm, DDL_options_st ddl)
  {
    if (check_add_key(ddl) ||
        !(last_key= new Key(key_type, key_name, algorithm, false, ddl)))
      return true;
    alter_info.key_list.push_back(last_key);
    return false;
  }
  
  bool add_create_index(Key::Keytype key_type, const LEX_CSTRING *key_name,
                        ha_key_alg algorithm, DDL_options_st ddl)
  {
    if (check_create_options(ddl) ||
       !(last_key= new Key(key_type, key_name, algorithm, false, ddl)))
      return true;
    alter_info.key_list.push_back(last_key);
    return false;
  }
  bool add_create_index_prepare(Table_ident *table)
  {
    sql_command= SQLCOM_CREATE_INDEX;
    if (!current_select->add_table_to_list(thd, table, NULL,
                                           TL_OPTION_UPDATING,
                                           TL_READ_NO_INSERT,
                                           MDL_SHARED_UPGRADABLE))
      return true;
    alter_info.reset();
    alter_info.flags= ALTER_ADD_INDEX;
    option_list= NULL;
    return false;
  }
  
  void add_key_to_list(LEX_CSTRING *field_name,
                       enum Key::Keytype type, bool check_exists);
  
  bool add_constraint(LEX_CSTRING *name, Virtual_column_info *constr,
                      bool if_not_exists)
  {
    constr->name= *name;
    constr->flags= if_not_exists ?
                   Alter_info::CHECK_CONSTRAINT_IF_NOT_EXISTS : 0;
    alter_info.check_constraint_list.push_back(constr);
    return false;
  }
  bool add_alter_list(const char *par_name, Virtual_column_info *expr,
                      bool par_exists);
  void set_command(enum_sql_command command,
                   DDL_options_st options)
  {
    sql_command= command;
    create_info.set(options);
  }
  void set_command(enum_sql_command command,
                   uint scope,
                   DDL_options_st options)
  {
    set_command(command, options);
    create_info.options|= scope; 
  }
  bool check_create_options(DDL_options_st options)
  {
    if (options.or_replace() && options.if_not_exists())
    {
      my_error(ER_WRONG_USAGE, MYF(0), "OR REPLACE", "IF NOT EXISTS");
      return true;
    }
    return false;
  }
  bool set_create_options_with_check(DDL_options_st options)
  {
    create_info.set(options);
    return check_create_options(create_info);
  }
  bool add_create_options_with_check(DDL_options_st options)
  {
    create_info.add(options);
    return check_create_options(create_info);
  }
  bool sp_add_cfetch(THD *thd, const LEX_CSTRING *name);

  bool set_command_with_check(enum_sql_command command,
                              uint scope,
                              DDL_options_st options)
  {
    set_command(command, scope, options);
    return check_create_options(options);
  }
  bool set_command_with_check(enum_sql_command command, DDL_options_st options)
  {
    set_command(command, options);
    return check_create_options(options);
  }
  
  bool tmp_table() const { return create_info.tmp_table(); }
  bool if_exists() const { return create_info.if_exists(); }

  
  bool handle_list_of_derived(TABLE_LIST *table_list, uint phases)
  {
    for (TABLE_LIST *tl= table_list; tl; tl= tl->next_local)
    {
      if (tl->is_view_or_derived() && tl->handle_derived(this, phases))
        return true;
    }
    return false;
  }

  SELECT_LEX *exclude_last_select();
  bool add_unit_in_brackets(SELECT_LEX *nselect);
  void check_automatic_up(enum sub_select_type type);
  bool create_or_alter_view_finalize(THD *thd, Table_ident *table_ident);
  bool add_alter_view(THD *thd, uint16 algorithm, enum_view_suid suid,
                      Table_ident *table_ident);
  bool add_create_view(THD *thd, DDL_options_st ddl,
                       uint16 algorithm, enum_view_suid suid,
                       Table_ident *table_ident);

  bool add_grant_command(THD *thd, enum_sql_command sql_command_arg,
                         stored_procedure_type type_arg);

  Vers_parse_info &vers_get_info()
  {
    return create_info.vers_info;
  }
  sp_package *get_sp_package() const;

  
  bool check_simple_select(const LEX_CSTRING *option)
  {
    if (current_select != &select_lex)
    {
      char command[80];
      strmake(command, option->str, MY_MIN(option->length, sizeof(command)-1));
      my_error(ER_CANT_USE_OPTION_HERE, MYF(0), command);
      return true;
    }
    return false;
  }

  void tvc_start()
  {
    field_list.empty();
    many_values.empty();
    insert_list= 0;
  }
  bool tvc_finalize();
  bool tvc_finalize_derived();
};



class Set_signal_information
{
public:
  
 Set_signal_information() {} 

  
  Set_signal_information(const Set_signal_information& set);

  
  ~Set_signal_information()
  {}

  
  void clear();

  
  Item *m_item[LAST_DIAG_SET_PROPERTY+1];
};



class Yacc_state
{
public:
  Yacc_state()
  {
    reset();
  }

  void reset()
  {
    yacc_yyss= NULL;
    yacc_yyvs= NULL;
    m_set_signal_info.clear();
    m_lock_type= TL_READ_DEFAULT;
    m_mdl_type= MDL_SHARED_READ;
  }

  ~Yacc_state();

  
  void reset_before_substatement()
  {
    m_lock_type= TL_READ_DEFAULT;
    m_mdl_type= MDL_SHARED_READ;
  }

  
  uchar *yacc_yyss;

  
  uchar *yacc_yyvs;

  
  Set_signal_information m_set_signal_info;

  
  thr_lock_type m_lock_type;

  
  enum_mdl_type m_mdl_type;

  
};


class Parser_state
{
public:
  Parser_state()
    : m_yacc()
  {}

  
  bool init(THD *thd, char *buff, size_t length)
  {
    return m_lip.init(thd, buff, length);
  }

  ~Parser_state()
  {}

  Lex_input_stream m_lip;
  Yacc_state m_yacc;

  
  PSI_digest_locker* m_digest_psi;

  void reset(char *found_semicolon, unsigned int length)
  {
    m_lip.reset(found_semicolon, length);
    m_yacc.reset();
  }
};


extern sql_digest_state *
digest_add_token(sql_digest_state *state, uint token, LEX_YYSTYPE yylval);

extern sql_digest_state *
digest_reduce_token(sql_digest_state *state, uint token_left, uint token_right);

struct st_lex_local: public LEX, public Sql_alloc
{
};



class sp_lex_local: public st_lex_local
{
public:
  sp_lex_local(THD *thd, const LEX *oldlex)
  {
    
    start(thd);
    
    sphead= oldlex->sphead;
    spcont= oldlex->spcont;
    
    trg_chistics= oldlex->trg_chistics;
    trg_table_fields.empty();
    sp_lex_in_use= false;
  }
};



class sp_assignment_lex: public sp_lex_local
{
  Item *m_item;       
  Item *m_free_list;  
public:
  sp_assignment_lex(THD *thd, LEX *oldlex)
   :sp_lex_local(thd, oldlex),
    m_item(NULL),
    m_free_list(NULL)
  { }
  void set_item_and_free_list(Item *item, Item *free_list)
  {
    m_item= item;
    m_free_list= free_list;
  }
  Item *get_item() const
  {
    return m_item;
  }
  Item *get_free_list() const
  {
    return m_free_list;
  }
};


extern void lex_init(void);
extern void lex_free(void);
extern void lex_start(THD *thd);
extern void lex_end(LEX *lex);
extern void lex_end_stage1(LEX *lex);
extern void lex_end_stage2(LEX *lex);
void end_lex_with_single_table(THD *thd, TABLE *table, LEX *old_lex);
int init_lex_with_single_table(THD *thd, TABLE *table, LEX *lex);
extern int MYSQLlex(union YYSTYPE *yylval, THD *thd);
extern int ORAlex(union YYSTYPE *yylval, THD *thd);

extern void trim_whitespace(CHARSET_INFO *cs, LEX_CSTRING *str, size_t * prefix_length = 0);

extern bool is_lex_native_function(const LEX_CSTRING *name); 
extern bool is_native_function(THD *thd, const LEX_CSTRING *name);
extern bool is_native_function_with_warn(THD *thd, const LEX_CSTRING *name);



void my_missing_function_error(const LEX_CSTRING &token, const char *name);
bool is_keyword(const char *name, uint len);
int set_statement_var_if_exists(THD *thd, const char *var_name,
                                size_t var_name_length, ulonglong value);

Virtual_column_info *add_virtual_expression(THD *thd, Item *expr);
Item* handle_sql2003_note184_exception(THD *thd, Item* left, bool equal,
                                       Item *expr);

void sp_create_assignment_lex(THD *thd, bool no_lookahead);
bool sp_create_assignment_instr(THD *thd, bool no_lookahead);

#endif 
#endif 
