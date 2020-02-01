#ifndef SQL_ITEM_INCLUDED
#define SQL_ITEM_INCLUDED




#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "sql_priv.h"                
#include "unireg.h"
#include "sql_const.h"                 
#include "field.h"                              
#include "sql_type.h"
#include "sql_time.h"
#include "mem_root_array.h"

C_MODE_START
#include <ma_dyncol.h>


struct st_value
{
  enum enum_dynamic_column_type m_type;
  union
  {
    longlong m_longlong;
    double m_double;
    MYSQL_TIME m_time;
  } value;
  String m_string;
  my_decimal m_decimal;
};

C_MODE_END


class Value: public st_value
{
public:
  bool is_null() const { return m_type == DYN_COL_NULL; }
  bool is_longlong() const
  {
    return m_type == DYN_COL_UINT || m_type == DYN_COL_INT;
  }
  bool is_double() const { return m_type == DYN_COL_DOUBLE; }
  bool is_temporal() const { return m_type == DYN_COL_DATETIME; }
  bool is_string() const { return m_type == DYN_COL_STRING; }
  bool is_decimal() const { return m_type == DYN_COL_DECIMAL; }
};


template<size_t buffer_size>
class ValueBuffer: public Value
{
  char buffer[buffer_size];
  void reset_buffer()
  {
    m_string.set(buffer, buffer_size, &my_charset_bin);
  }
public:
  ValueBuffer()
  {
    reset_buffer();
  }
};


#ifdef DBUG_OFF
static inline const char *dbug_print_item(Item *item) { return NULL; }
#else
const char *dbug_print_item(Item *item);
#endif

class Virtual_tmp_table;
class sp_head;
class Protocol;
struct TABLE_LIST;
void item_init(void);			
class Item_field;
class Item_param;
class user_var_entry;
class JOIN;
struct KEY_FIELD;
struct SARGABLE_PARAM;
class RANGE_OPT_PARAM;
class SEL_TREE;

enum precedence {
  LOWEST_PRECEDENCE,
  ASSIGN_PRECEDENCE,    
  OR_PRECEDENCE,        
  XOR_PRECEDENCE,       
  AND_PRECEDENCE,       
  NOT_PRECEDENCE,       
  BETWEEN_PRECEDENCE,   
  CMP_PRECEDENCE,       
  BITOR_PRECEDENCE,     
  BITAND_PRECEDENCE,    
  SHIFT_PRECEDENCE,     
  ADDINTERVAL_PRECEDENCE, 
  ADD_PRECEDENCE,       
  MUL_PRECEDENCE,       
  BITXOR_PRECEDENCE,    
  PIPES_PRECEDENCE,     
  NEG_PRECEDENCE,       
  BANG_PRECEDENCE,      
  COLLATE_PRECEDENCE,   
  INTERVAL_PRECEDENCE,  
  DEFAULT_PRECEDENCE,
  HIGHEST_PRECEDENCE
};

bool mark_unsupported_function(const char *where, void *store, uint result);


bool mark_unsupported_function(const char *w1, const char *w2,
                               void *store, uint result);


#define SPLIT_SUM_SKIP_REGISTERED 1     
#define SPLIT_SUM_SELECT 2		


#define NO_EXTRACTION_FL              (1 << 6)
#define FULL_EXTRACTION_FL            (1 << 7)
#define SUBSTITUTION_FL               (1 << 8)
#define EXTRACTION_MASK               (NO_EXTRACTION_FL | FULL_EXTRACTION_FL)

extern const char *item_empty_name;

void dummy_error_processor(THD *thd, void *data);

void view_error_processor(THD *thd, void *data);


struct Name_resolution_context: Sql_alloc
{
  
  Name_resolution_context *outer_context;

  
  TABLE_LIST *table_list;
  
  TABLE_LIST *first_name_resolution_table;
  
  TABLE_LIST *last_name_resolution_table;

  
  TABLE_LIST *natural_join_first_table;
  
  st_select_lex *select_lex;

  
  void (*error_processor)(THD *, void *);
  void *error_processor_data;

  
  bool resolve_in_select_list;

  
  Security_context *security_ctx;

  Name_resolution_context()
    :outer_context(0), table_list(0), select_lex(0),
    error_processor_data(0),
    security_ctx(0)
    {}

  void init()
  {
    resolve_in_select_list= FALSE;
    error_processor= &dummy_error_processor;
    first_name_resolution_table= NULL;
    last_name_resolution_table= NULL;
  }

  void resolve_in_table_list_only(TABLE_LIST *tables)
  {
    table_list= first_name_resolution_table= tables;
    resolve_in_select_list= FALSE;
  }

  void process_error(THD *thd)
  {
    (*error_processor)(thd, error_processor_data);
  }
  st_select_lex *outer_select()
  {
    return (outer_context ?
            outer_context->select_lex :
            NULL);
  }
};




class Name_resolution_context_state
{
private:
  TABLE_LIST *save_table_list;
  TABLE_LIST *save_first_name_resolution_table;
  TABLE_LIST *save_next_name_resolution_table;
  bool        save_resolve_in_select_list;
  TABLE_LIST *save_next_local;

public:
  Name_resolution_context_state() {}          

public:
  
  void save_state(Name_resolution_context *context, TABLE_LIST *table_list)
  {
    save_table_list=                  context->table_list;
    save_first_name_resolution_table= context->first_name_resolution_table;
    save_resolve_in_select_list=      context->resolve_in_select_list;
    save_next_local=                  table_list->next_local;
    save_next_name_resolution_table=  table_list->next_name_resolution_table;
  }

  
  void restore_state(Name_resolution_context *context, TABLE_LIST *table_list)
  {
    table_list->next_local=                save_next_local;
    table_list->next_name_resolution_table= save_next_name_resolution_table;
    context->table_list=                   save_table_list;
    context->first_name_resolution_table=  save_first_name_resolution_table;
    context->resolve_in_select_list=       save_resolve_in_select_list;
  }

  TABLE_LIST *get_first_name_resolution_table()
  {
    return save_first_name_resolution_table;
  }
};

class Name_resolution_context_backup
{
  Name_resolution_context &ctx;
  TABLE_LIST &table_list;
  table_map save_map;
  Name_resolution_context_state ctx_state;

public:
  Name_resolution_context_backup(Name_resolution_context &_ctx, TABLE_LIST &_table_list)
    : ctx(_ctx), table_list(_table_list), save_map(_table_list.map)
  {
    ctx_state.save_state(&ctx, &table_list);
    ctx.table_list= &table_list;
    ctx.first_name_resolution_table= &table_list;
  }
  ~Name_resolution_context_backup()
  {
    ctx_state.restore_state(&ctx, &table_list);
    table_list.map= save_map;
  }
};




typedef enum monotonicity_info 
{
   NON_MONOTONIC,              
   MONOTONIC_INCREASING,       
   MONOTONIC_INCREASING_NOT_NULL,  
   MONOTONIC_STRICT_INCREASING,
   MONOTONIC_STRICT_INCREASING_NOT_NULL  
} enum_monotonicity_info;



class sp_rcontext;



class Sp_rcontext_handler
{
public:
  virtual ~Sp_rcontext_handler() {}
  
  virtual const LEX_CSTRING *get_name_prefix() const= 0;
  
  virtual sp_rcontext *get_rcontext(sp_rcontext *ctx) const= 0;
};


class Sp_rcontext_handler_local: public Sp_rcontext_handler
{
public:
  const LEX_CSTRING *get_name_prefix() const;
  sp_rcontext *get_rcontext(sp_rcontext *ctx) const;
};


class Sp_rcontext_handler_package_body: public Sp_rcontext_handler
{
public:
  const LEX_CSTRING *get_name_prefix() const;
  sp_rcontext *get_rcontext(sp_rcontext *ctx) const;
};


extern MYSQL_PLUGIN_IMPORT
  Sp_rcontext_handler_local sp_rcontext_handler_local;


extern MYSQL_PLUGIN_IMPORT
  Sp_rcontext_handler_package_body sp_rcontext_handler_package_body;



class Item_equal;

struct st_join_table* const NO_PARTICULAR_TAB= (struct st_join_table*)0x1;

typedef struct replace_equal_field_arg 
{
  Item_equal *item_equal;
  struct st_join_table *context_tab;
} REPLACE_EQUAL_FIELD_ARG;

class Settable_routine_parameter
{
public:
  
  Settable_routine_parameter() {}
  virtual ~Settable_routine_parameter() {}
  virtual void set_required_privilege(bool rw) {};

  
  virtual bool set_value(THD *thd, sp_rcontext *ctx, Item **it)= 0;

  virtual void set_out_param_info(Send_field *info) {}

  virtual const Send_field *get_out_param_info() const
  { return NULL; }

  virtual Item_param *get_item_param() { return 0; }
};



class Query_fragment
{
  uint m_pos;
  uint m_length;
  void set(size_t pos, size_t length)
  {
    DBUG_ASSERT(pos < UINT_MAX32);
    DBUG_ASSERT(length < UINT_MAX32);
    m_pos= (uint) pos;
    m_length= (uint) length;
  }
public:
  Query_fragment(THD *thd, sp_head *sphead, const char *start, const char *end);
  uint pos() const { return m_pos; }
  uint length() const { return m_length; }
};



class Rewritable_query_parameter
{
  public:
  
  my_ptrdiff_t pos_in_query;

  
  uint len_in_query;

  bool limit_clause_param;

  Rewritable_query_parameter(uint pos_in_q= 0, uint len_in_q= 0)
    : pos_in_query(pos_in_q), len_in_query(len_in_q),
      limit_clause_param(false)
  { }

  virtual ~Rewritable_query_parameter() { }

  virtual bool append_for_log(THD *thd, String *str) = 0;
};

class Copy_query_with_rewrite
{
  THD *thd;
  const char *src;
  size_t src_len, from;
  String *dst;

  bool copy_up_to(size_t bytes)
  {
    DBUG_ASSERT(bytes >= from);
    return dst->append(src + from, uint32(bytes - from));
  }

public:

  Copy_query_with_rewrite(THD *t, const char *s, size_t l, String *d)
    :thd(t), src(s), src_len(l), from(0), dst(d) { }

  bool append(Rewritable_query_parameter *p)
  {
    if (copy_up_to(p->pos_in_query) || p->append_for_log(thd, dst))
      return true;
    from= p->pos_in_query + p->len_in_query;
    return false;
  }

  bool finalize()
  { return copy_up_to(src_len); }
};

struct st_dyncall_create_def
{
  Item  *key, *value;
  CHARSET_INFO *cs;
  uint len, frac;
  DYNAMIC_COLUMN_TYPE type;
};

typedef struct st_dyncall_create_def DYNCALL_CREATE_DEF;


typedef bool (Item::*Item_processor) (void *arg);

typedef bool (Item::*Item_analyzer) (uchar **argp);
typedef Item* (Item::*Item_transformer) (THD *thd, uchar *arg);
typedef void (*Cond_traverser) (const Item *item, void *arg);

struct st_cond_statistic;

struct find_selective_predicates_list_processor_data
{
  TABLE *table;
  List<st_cond_statistic> list;
};

class MY_LOCALE;

class Item_equal;
class COND_EQUAL;

class st_select_lex_unit;

class Item_func_not;
class Item_splocal;


class String_copier_for_item: public String_copier
{
  THD *m_thd;
public:
  bool copy_with_warn(CHARSET_INFO *dstcs, String *dst,
                      CHARSET_INFO *srccs, const char *src,
                      uint32 src_length, uint32 nchars);
  String_copier_for_item(THD *thd): m_thd(thd) { }
};

class Item: public Value_source,
            public Type_all_attributes
{
  void operator=(Item &);
  
  uint join_tab_idx;

  static void *operator new(size_t size);

public:
  static void *operator new(size_t size, MEM_ROOT *mem_root) throw ()
  { return alloc_root(mem_root, size); }
  static void operator delete(void *ptr,size_t size) { TRASH_FREE(ptr, size); }
  static void operator delete(void *ptr, MEM_ROOT *mem_root) {}

  enum Type {FIELD_ITEM= 0, FUNC_ITEM, SUM_FUNC_ITEM,
             WINDOW_FUNC_ITEM, STRING_ITEM,
	     INT_ITEM, REAL_ITEM, NULL_ITEM, VARBIN_ITEM,
	     COPY_STR_ITEM, FIELD_AVG_ITEM, DEFAULT_VALUE_ITEM,
	     PROC_ITEM,COND_ITEM, REF_ITEM, FIELD_STD_ITEM,
	     FIELD_VARIANCE_ITEM, INSERT_VALUE_ITEM,
             SUBSELECT_ITEM, ROW_ITEM, CACHE_ITEM, TYPE_HOLDER,
             PARAM_ITEM, TRIGGER_FIELD_ITEM, DECIMAL_ITEM,
             XPATH_NODESET, XPATH_NODESET_CMP,
             VIEW_FIXER_ITEM, EXPR_CACHE_ITEM,
             DATE_ITEM};

  enum cond_result { COND_UNDEF,COND_OK,COND_TRUE,COND_FALSE };

  enum traverse_order { POSTFIX, PREFIX };
  
  
  int8 is_expensive_cache;
  
  
  uint rsize;

protected:
  
  String str_value;

  SEL_TREE *get_mm_tree_for_const(RANGE_OPT_PARAM *param);

  
  Field *create_table_field_from_handler(TABLE *table)
  {
    const Type_handler *h= type_handler();
    return h->make_and_init_table_field(&name, Record_addr(maybe_null),
                                        *this, table);
  }
  
  Field *tmp_table_field_from_field_type(TABLE *table)
  {
    const Type_handler *h= type_handler()->type_handler_for_tmp_table(this);
    return h->make_and_init_table_field(&name, Record_addr(maybe_null),
                                        *this, table);
  }
  Field *create_tmp_field_int(TABLE *table, uint convert_int_length);

  void push_note_converted_to_negative_complement(THD *thd);
  void push_note_converted_to_positive_complement(THD *thd);

  
  double val_real_from_item(Item *item)
  {
    DBUG_ASSERT(fixed == 1);
    double value= item->val_real();
    null_value= item->null_value;
    return value;
  }
  longlong val_int_from_item(Item *item)
  {
    DBUG_ASSERT(fixed == 1);
    longlong value= item->val_int();
    null_value= item->null_value;
    return value;
  }
  String *val_str_from_item(Item *item, String *str)
  {
    DBUG_ASSERT(fixed == 1);
    String *res= item->val_str(str);
    if (res)
      res->set_charset(collation.collation);
    if ((null_value= item->null_value))
      res= NULL;
    return res;
  }
  my_decimal *val_decimal_from_item(Item *item, my_decimal *decimal_value)
  {
    DBUG_ASSERT(fixed == 1);
    my_decimal *value= item->val_decimal(decimal_value);
    if ((null_value= item->null_value))
      value= NULL;
    return value;
  }
  bool get_date_from_item(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    bool rc= item->get_date(ltime, fuzzydate);
    null_value= MY_TEST(rc || item->null_value);
    return rc;
  }
  
  bool make_zero_date(MYSQL_TIME *ltime, ulonglong fuzzydate);

public:
  
  String *val_str() { return val_str(&str_value); }

  const MY_LOCALE *locale_from_val_str();

  LEX_CSTRING name;			
  
  const char *orig_name;
  
  Item *next;
  int  marker;
  bool maybe_null;			
  bool in_rollup;                        
  bool null_value;			
  bool with_sum_func;                   
  bool with_param;                      
  bool with_window_func;             
  
  bool with_field;
  bool fixed;                           
  bool is_autogenerated_name;           
  
  Item(THD *thd);
  
  Item(THD *thd, Item *item);
  virtual ~Item()
  {
#ifdef EXTRA_DEBUG
    name.str= 0;
    name.length= 0;
#endif
  }		
  void set_name(THD *thd, const char *str, size_t length, CHARSET_INFO *cs);
  void set_name_no_truncate(THD *thd, const char *str, uint length,
                            CHARSET_INFO *cs);
  void init_make_send_field(Send_field *tmp_field,enum enum_field_types type);
  virtual void cleanup();
  virtual void make_send_field(THD *thd, Send_field *field);

  bool fix_fields_if_needed(THD *thd, Item **ref)
  {
    return fixed ? false : fix_fields(thd, ref);
  }
  bool fix_fields_if_needed_for_scalar(THD *thd, Item **ref)
  {
    return fix_fields_if_needed(thd, ref) || check_cols(1);
  }
  bool fix_fields_if_needed_for_bool(THD *thd, Item **ref)
  {
    return fix_fields_if_needed_for_scalar(thd, ref);
  }
  bool fix_fields_if_needed_for_order_by(THD *thd, Item **ref)
  {
    return fix_fields_if_needed_for_scalar(thd, ref);
  }
  virtual bool fix_fields(THD *, Item **);
  
  virtual void fix_after_pullout(st_select_lex *new_parent, Item **ref,
                                 bool merge)
    {};

  
  virtual inline void quick_fix_field() { fixed= 1; }

  bool save_in_value(struct st_value *value)
  {
    return type_handler()->Item_save_in_value(this, value);
  }

  
  int save_in_field_no_warnings(Field *field, bool no_conversions);
  virtual int save_in_field(Field *field, bool no_conversions);
  virtual bool save_in_param(THD *thd, Item_param *param);
  virtual void save_org_in_field(Field *field,
                                 fast_field_copier data
                                 __attribute__ ((__unused__)))
  { (void) save_in_field(field, 1); }
  virtual fast_field_copier setup_fast_field_copier(Field *field)
  { return NULL; }
  virtual int save_safe_in_field(Field *field)
  { return save_in_field(field, 1); }
  virtual bool send(Protocol *protocol, st_value *buffer)
  {
    return type_handler()->Item_send(this, protocol, buffer);
  }
  virtual bool eq(const Item *, bool binary_cmp) const;
  enum_field_types field_type() const
  {
    return type_handler()->field_type();
  }
  virtual const Type_handler *type_handler() const= 0;
  const Type_handler *type_handler_for_comparison() const
  {
    return type_handler()->type_handler_for_comparison();
  }
  virtual const Type_handler *real_type_handler() const
  {
    return type_handler();
  }
  virtual const Type_handler *cast_to_int_type_handler() const
  {
    return type_handler();
  }
  virtual const Type_handler *type_handler_for_system_time() const
  {
    return real_type_handler();
  }
  
  Item_result result_type() const
  {
    return type_handler()->result_type();
  }
  
  Item_result cmp_type() const
  {
    return type_handler()->cmp_type();
  }
  const Type_handler *string_type_handler() const
  {
    return Type_handler::string_type_handler(max_length);
  }
  
  virtual uint32 max_display_length() const
  {
    return type_handler()->max_display_length(this);
  }
  TYPELIB *get_typelib() const { return NULL; }
  void set_maybe_null(bool maybe_null_arg) { maybe_null= maybe_null_arg; }
  void set_typelib(TYPELIB *typelib)
  {
    
    DBUG_ASSERT(0);
  }
  Item_cache* get_cache(THD *thd) const
  {
    return type_handler()->Item_get_cache(thd, this);
  }
  virtual enum Type type() const =0;
  
  virtual enum Type real_type() const { return type(); }
  
  
  virtual enum_monotonicity_info get_monotonicity_info() const
  { return NON_MONOTONIC; }

  
  virtual longlong val_int_endpoint(bool left_endp, bool *incl_endp)
  { DBUG_ASSERT(0); return 0; }


  
  
  virtual double val_real()=0;
  
  virtual longlong val_int()=0;
  Longlong_hybrid to_longlong_hybrid()
  {
    return Longlong_hybrid(val_int(), unsigned_flag);
  }
  
  virtual longlong val_int_signed_typecast()
  {
    return cast_to_int_type_handler()->Item_val_int_signed_typecast(this);
  }
  longlong val_int_signed_typecast_from_str();
  
  virtual longlong val_int_unsigned_typecast()
  {
    return cast_to_int_type_handler()->Item_val_int_unsigned_typecast(this);
  }
  longlong val_int_unsigned_typecast_from_decimal();
  longlong val_int_unsigned_typecast_from_int();
  longlong val_int_unsigned_typecast_from_str();
  
  inline ulonglong val_uint() { return (ulonglong) val_int(); }

  
  virtual String *val_str(String *str)=0;

  
  virtual String *val_str_ascii(String *str);

  
  String *val_str_ascii_revert_empty_string_is_null(THD *thd, String *str);

  
  String *val_str(String *str, String *converter, CHARSET_INFO *to);

  virtual String *val_json(String *str) { return val_str(str); }
  
  virtual my_decimal *val_decimal(my_decimal *decimal_buffer)= 0;
  
  virtual bool val_bool()
  {
    return type_handler()->Item_val_bool(this);
  }
  virtual String *val_nodeset(String*) { return 0; }

  bool eval_const_cond()
  {
    DBUG_ASSERT(const_item());
    DBUG_ASSERT(!is_expensive());
    return val_bool();
  }

  
  virtual void save_val(Field *to) { save_org_in_field(to, NULL); }
  
  virtual void save_result(Field *to) { save_val(to); }
  
  String *val_string_from_real(String *str);
  String *val_string_from_int(String *str);
  String *val_string_from_decimal(String *str);
  String *val_string_from_date(String *str);
  my_decimal *val_decimal_from_real(my_decimal *decimal_value);
  my_decimal *val_decimal_from_int(my_decimal *decimal_value);
  my_decimal *val_decimal_from_string(my_decimal *decimal_value);
  my_decimal *val_decimal_from_date(my_decimal *decimal_value);
  my_decimal *val_decimal_from_time(my_decimal *decimal_value);
  longlong val_int_from_decimal();
  longlong val_int_from_date();
  longlong val_int_from_real()
  {
    DBUG_ASSERT(fixed == 1);
    return Converter_double_to_longlong_with_warn(val_real(), false).result();
  }
  longlong val_int_from_str(int *error);
  double val_real_from_decimal();
  double val_real_from_date();

  
  bool get_temporal_with_sql_mode(MYSQL_TIME *ltime);
  
  bool is_null_from_temporal();

  int save_time_in_field(Field *field, bool no_conversions);
  int save_date_in_field(Field *field, bool no_conversions);
  int save_str_in_field(Field *field, bool no_conversions);
  int save_real_in_field(Field *field, bool no_conversions);
  int save_int_in_field(Field *field, bool no_conversions);
  int save_decimal_in_field(Field *field, bool no_conversions);

  int save_str_value_in_field(Field *field, String *result);

  virtual Field *get_tmp_table_field() { return 0; }
  virtual Field *create_field_for_create_select(TABLE *table);
  virtual Field *create_field_for_schema(THD *thd, TABLE *table);
  virtual const char *full_name() const { return name.str ? name.str : "???"; }
  const char *field_name_or_null()
  { return real_item()->type() == Item::FIELD_ITEM ? name.str : NULL; }
  const TABLE_SHARE *field_table_or_null();

  
  virtual double  val_result() { return val_real(); }
  virtual longlong val_int_result() { return val_int(); }
  virtual String *str_result(String* tmp) { return val_str(tmp); }
  virtual my_decimal *val_decimal_result(my_decimal *val)
  { return val_decimal(val); }
  virtual bool val_bool_result() { return val_bool(); }
  virtual bool is_null_result() { return is_null(); }
  
  virtual bool dynamic_result() { return 0; }
  
  virtual table_map used_tables() const { return (table_map) 0L; }
  virtual table_map all_used_tables() const { return used_tables(); }
  
  virtual table_map not_null_tables() const { return used_tables(); }
  
  virtual bool basic_const_item() const { return 0; }
  
  virtual Item *clone_item(THD *thd) { return 0; }
  virtual Item* build_clone(THD *thd) { return get_copy(thd); }
  virtual cond_result eq_cmp_result() const { return COND_OK; }
  inline uint float_length(uint decimals_par) const
  { return decimals < FLOATING_POINT_DECIMALS ? (DBL_DIG+2+decimals_par) : DBL_DIG+8;}
  
  virtual uint decimal_precision() const
  {
    return type_handler()->Item_decimal_precision(this);
  }
  
  inline int decimal_int_part() const
  { return my_decimal_int_part(decimal_precision(), decimals); }
  
  uint decimal_scale() const
  {
    return type_handler()->Item_decimal_scale(this);
  }
  
  uint divisor_precision_increment() const
  {
    return type_handler()->Item_divisor_precision_increment(this);
  }
  
  uint time_precision()
  {
    return const_item() ? type_handler()->Item_time_precision(this) :
                          MY_MIN(decimals, TIME_SECOND_PART_DIGITS);
  }
  uint datetime_precision()
  {
    return const_item() ? type_handler()->Item_datetime_precision(this) :
                          MY_MIN(decimals, TIME_SECOND_PART_DIGITS);
  }
  virtual longlong val_int_min() const
  {
    return LONGLONG_MIN;
  }
  
  virtual bool const_item() const { return used_tables() == 0; }
  
  virtual bool const_during_execution() const 
  { return (used_tables() & ~PARAM_TABLE_BIT) == 0; }

  
  virtual enum precedence precedence() const { return DEFAULT_PRECEDENCE; }
  void print_parenthesised(String *str, enum_query_type query_type,
                           enum precedence parent_prec);
  
  void print_for_table_def(String *str)
  {
    print_parenthesised(str,
                     (enum_query_type)(QT_ITEM_ORIGINAL_FUNC_NULLIF |
                                       QT_ITEM_IDENT_SKIP_DB_NAMES |
                                       QT_ITEM_IDENT_SKIP_TABLE_NAMES |
                                       QT_NO_DATA_EXPANSION |
                                       QT_TO_SYSTEM_CHARSET),
                     LOWEST_PRECEDENCE);
  }
  virtual void print(String *str, enum_query_type query_type);

  class Print: public String
  {
  public:
    Print(Item *item, enum_query_type type)
    {
      item->print(this, type);
    }
  };

  void print_item_w_name(String *str, enum_query_type query_type);
  void print_value(String *str);

  virtual void update_used_tables() {}
  virtual COND *build_equal_items(THD *thd, COND_EQUAL *inheited,
                                  bool link_item_fields,
                                  COND_EQUAL **cond_equal_ref)
  {
    update_used_tables();
    DBUG_ASSERT(!cond_equal_ref || !cond_equal_ref[0]);
    return this;
  }
  virtual COND *remove_eq_conds(THD *thd, Item::cond_result *cond_value,
                                bool top_level);
  virtual void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                              uint *and_level,
                              table_map usable_tables,
                              SARGABLE_PARAM **sargables)
  {
    return;
  }
   
   virtual SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  
  virtual bool check_equality(THD *thd, COND_EQUAL *cond, List<Item> *eq_list)
  {
    return false;
  }
  virtual void split_sum_func(THD *thd, Ref_ptr_array ref_pointer_array,
                              List<Item> &fields, uint flags) {}
  
  void split_sum_func2(THD *thd, Ref_ptr_array ref_pointer_array,
                       List<Item> &fields,
                       Item **ref, uint flags);
  virtual bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)= 0;
  bool get_date_from_int(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool get_date_from_year(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool get_date_from_real(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool get_date_from_decimal(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool get_date_from_string(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool get_time(MYSQL_TIME *ltime)
  { return get_date(ltime, Time::flags_for_get_date()); }
  
  bool get_time_with_conversion(THD *thd, MYSQL_TIME *ltime,
                                ulonglong fuzzydate);
  
  virtual longlong val_datetime_packed()
  {
    ulonglong fuzzydate= TIME_FUZZY_DATES | TIME_INVALID_DATES;
    Datetime dt(current_thd, this, fuzzydate);
    return dt.is_valid_datetime() ? pack_time(dt.get_mysql_time()) : 0;
  }
  
  virtual longlong val_time_packed()
  {
    Time tm(this, Time::comparison_flags_for_get_date());
    return tm.is_valid_time() ? pack_time(tm.get_mysql_time()) : 0;
  }
  longlong val_datetime_packed_result();
  longlong val_time_packed_result()
  {
    MYSQL_TIME ltime;
    ulonglong fuzzydate= Time::comparison_flags_for_get_date();
    return get_date_result(&ltime, fuzzydate) ? 0 : pack_time(&ltime);
  }

  
  longlong val_temporal_packed(enum_field_types f_type)
  {
    return f_type == MYSQL_TYPE_TIME ? val_time_packed() :
                                       val_datetime_packed();
  }
  bool get_seconds(ulonglong *sec, ulong *sec_part);
  virtual bool get_date_result(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date(ltime,fuzzydate); }
  
  virtual bool is_null() { return 0; }

  
  virtual void update_null_value ()
  {
    switch (cmp_type()) {
    case INT_RESULT:
      (void) val_int();
      break;
    case REAL_RESULT:
      (void) val_real();
      break;
    case DECIMAL_RESULT:
      {
        my_decimal tmp;
        (void) val_decimal(&tmp);
      }
      break;
    case TIME_RESULT:
      {
        MYSQL_TIME ltime;
        (void) get_temporal_with_sql_mode(&ltime);
      }
      break;
    case STRING_RESULT:
      {
        StringBuffer<MAX_FIELD_WIDTH> tmp;
        (void) val_str(&tmp);
      }
      break;
    case ROW_RESULT:
      DBUG_ASSERT(0);
      null_value= true;
    }
  }

  
  virtual void top_level_item() {}
  
  virtual void set_result_field(Field *field) {}
  virtual bool is_result_field() { return 0; }
  virtual bool is_bool_type() { return false; }
  virtual bool is_json_type() { return false; }
  
  virtual bool need_parentheses_in_default() { return false; }
  virtual void save_in_result_field(bool no_conversions) {}
  
  virtual void no_rows_in_result() {}
  virtual void restore_to_before_no_rows_in_result() {}
  virtual Item *copy_or_same(THD *thd) { return this; }
  virtual Item *copy_andor_structure(THD *thd) { return this; }
  virtual Item *real_item() { return this; }
  virtual Item *get_tmp_table_item(THD *thd) { return copy_or_same(thd); }
  virtual Item *make_odbc_literal(THD *thd, const LEX_CSTRING *typestr)
  {
    return this;
  }

  static CHARSET_INFO *default_charset();

  CHARSET_INFO *charset_for_protocol(void) const
  {
    return type_handler()->charset_for_protocol(this);
  };

  virtual bool walk(Item_processor processor, bool walk_subquery, void *arg)
  {
    return (this->*processor)(arg);
  }

  virtual Item* transform(THD *thd, Item_transformer transformer, uchar *arg);

  
  virtual Item* compile(THD *thd, Item_analyzer analyzer, uchar **arg_p,
                        Item_transformer transformer, uchar *arg_t)
  {
    if ((this->*analyzer) (arg_p))
      return ((this->*transformer) (thd, arg_t));
    return 0;
  }

   virtual void traverse_cond(Cond_traverser traverser,
                              void *arg, traverse_order order)
   {
     (*traverser)(this, arg);
   }

  
  virtual bool remove_dependence_processor(void *arg) { return 0; }
  virtual bool cleanup_processor(void *arg);
  virtual bool cleanup_excluding_fields_processor(void *arg) { return cleanup_processor(arg); }
  virtual bool cleanup_excluding_const_fields_processor(void *arg) { return cleanup_processor(arg); }
  virtual bool collect_item_field_processor(void *arg) { return 0; }
  virtual bool collect_outer_ref_processor(void *arg) {return 0; }
  virtual bool check_inner_refs_processor(void *arg) { return 0; }
  virtual bool find_item_in_field_list_processor(void *arg) { return 0; }
  virtual bool find_item_processor(void *arg);
  virtual bool change_context_processor(void *arg) { return 0; }
  virtual bool reset_query_id_processor(void *arg) { return 0; }
  virtual bool is_expensive_processor(void *arg) { return 0; }

  
  virtual bool add_field_to_set_processor(void *arg) { return 0; }
  virtual bool register_field_in_read_map(void *arg) { return 0; }
  virtual bool register_field_in_write_map(void *arg) { return 0; }
  virtual bool register_field_in_bitmap(void *arg) { return 0; }
  virtual bool update_table_bitmaps_processor(void *arg) { return 0; }

  virtual bool enumerate_field_refs_processor(void *arg) { return 0; }
  virtual bool mark_as_eliminated_processor(void *arg) { return 0; }
  virtual bool eliminate_subselect_processor(void *arg) { return 0; }
  virtual bool set_fake_select_as_master_processor(void *arg) { return 0; }
  virtual bool view_used_tables_processor(void *arg) { return 0; }
  virtual bool eval_not_null_tables(void *arg) { return 0; }
  virtual bool is_subquery_processor(void *arg) { return 0; }
  virtual bool count_sargable_conds(void *arg) { return 0; }
  virtual bool limit_index_condition_pushdown_processor(void *arg) { return 0; }
  virtual bool exists2in_processor(void *arg) { return 0; }
  virtual bool find_selective_predicates_list_processor(void *arg) { return 0; }
  bool cleanup_is_expensive_cache_processor(void *arg)
  {
    is_expensive_cache= (int8)(-1);
    return 0;
  }

  
  virtual bool excl_dep_on_table(table_map tab_map) { return false; }
  
  virtual bool excl_dep_on_grouping_fields(st_select_lex *sel) { return false; }

  virtual bool switch_to_nullable_fields_processor(void *arg) { return 0; }
  virtual bool find_function_processor (void *arg) { return 0; }
  
  virtual bool check_partition_func_processor(void *arg) { return 1;}
  virtual bool post_fix_fields_part_expr_processor(void *arg) { return 0; }
  virtual bool rename_fields_processor(void *arg) { return 0; }
  
  struct vcol_func_processor_result
  {
    uint errors;                                
    const char *name;                           
  };
  struct func_processor_rename
  {
    LEX_CSTRING db_name;
    LEX_CSTRING table_name;
    List<Create_field> fields;
  };
  virtual bool check_vcol_func_processor(void *arg)
  {
    return mark_unsupported_function(full_name(), arg, VCOL_IMPOSSIBLE);
  }
  virtual bool check_field_expression_processor(void *arg) { return 0; }
  virtual bool check_func_default_processor(void *arg) { return 0; }
  
  virtual bool check_valid_arguments_processor(void *arg) { return 0; }
  virtual bool update_vcol_processor(void *arg) { return 0; }
  virtual bool set_fields_as_dependent_processor(void *arg) { return 0; }
  

  virtual Item *get_copy(THD *thd)=0;

  bool cache_const_expr_analyzer(uchar **arg);
  Item* cache_const_expr_transformer(THD *thd, uchar *arg);

  virtual Item* propagate_equal_fields(THD*, const Context &, COND_EQUAL *)
  {
    return this;
  };

  Item* propagate_equal_fields_and_change_item_tree(THD *thd,
                                                    const Context &ctx,
                                                    COND_EQUAL *cond,
                                                    Item **place);

  
  virtual Item *replace_equal_field(THD *thd, uchar *arg) { return this; }

  struct Collect_deps_prm
  {
    List<Item> *parameters;
    
    st_select_lex_unit *nest_level_base;
    uint count;
    int nest_level;
    bool collect;
  };

  
  virtual Item *this_item() { return this; }
  virtual const Item *this_item() const { return this; }

  
  virtual Item **this_item_addr(THD *thd, Item **addr_arg) { return addr_arg; }

  
  virtual uint cols() const { return 1; }
  virtual Item* element_index(uint i) { return this; }
  virtual Item** addr(uint i) { return 0; }
  virtual bool check_cols(uint c);
  bool check_type_traditional_scalar(const char *opname) const;
  bool check_type_scalar(const char *opname) const;
  bool check_type_or_binary(const char *opname, const Type_handler *handler) const;
  bool check_type_general_purpose_string(const char *opname) const;
  bool check_type_can_return_int(const char *opname) const;
  bool check_type_can_return_decimal(const char *opname) const;
  bool check_type_can_return_real(const char *opname) const;
  bool check_type_can_return_str(const char *opname) const;
  bool check_type_can_return_text(const char *opname) const;
  bool check_type_can_return_date(const char *opname) const;
  bool check_type_can_return_time(const char *opname) const;
  
  virtual bool null_inside() { return 0; }
  
  virtual void bring_value() {}

  const Type_handler *type_handler_long_or_longlong() const
  {
    return Type_handler::type_handler_long_or_longlong(max_char_length());
  }

  virtual Field *create_tmp_field(bool group, TABLE *table)
  {
    return tmp_table_field_from_field_type(table);
  }

  virtual Item_field *field_for_view_update() { return 0; }

  virtual Item *neg_transformer(THD *thd) { return NULL; }
  virtual Item *update_value_transformer(THD *thd, uchar *select_arg)
  { return this; }
  virtual Item *expr_cache_insert_transformer(THD *thd, uchar *unused)
  { return this; }
  virtual Item *derived_field_transformer_for_having(THD *thd, uchar *arg)
  { return this; }
  virtual Item *derived_field_transformer_for_where(THD *thd, uchar *arg)
  { return this; }
  virtual Item *derived_grouping_field_transformer_for_where(THD *thd,
                                                             uchar *arg)
  { return this; }
  virtual Item *in_predicate_to_in_subs_transformer(THD *thd, uchar *arg)
  { return this; }
  virtual bool expr_cache_is_needed(THD *) { return FALSE; }
  virtual Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs);
  bool needs_charset_converter(uint32 length, CHARSET_INFO *tocs) const
  {
    
    if (!String::needs_conversion_on_storage(length,
                                             collation.collation, tocs))
      return false;
    
    if (collation.derivation == DERIVATION_NUMERIC &&
        collation.repertoire == MY_REPERTOIRE_ASCII &&
        !(collation.collation->state & MY_CS_NONASCII) &&
        !(tocs->state & MY_CS_NONASCII))
      return false;
    return true;
  }
  bool needs_charset_converter(CHARSET_INFO *tocs)
  {
    
    return needs_charset_converter(1, tocs);
  }
  Item *const_charset_converter(THD *thd, CHARSET_INFO *tocs, bool lossless,
                                const char *func_name);
  Item *const_charset_converter(THD *thd, CHARSET_INFO *tocs, bool lossless)
  { return const_charset_converter(thd, tocs, lossless, NULL); }
  void delete_self()
  {
    cleanup();
    delete this;
  }

  virtual Item_splocal *get_item_splocal() { return 0; }
  virtual Rewritable_query_parameter *get_rewritable_query_parameter()
  { return 0; }

  
  virtual Settable_routine_parameter *get_settable_routine_parameter()
  {
    return 0;
  }

  virtual Load_data_outvar *get_load_data_outvar()
  {
    return 0;
  }
  Load_data_outvar *get_load_data_outvar_or_error()
  {
    Load_data_outvar *dst= get_load_data_outvar();
    if (dst)
      return dst;
    my_error(ER_NONUPDATEABLE_COLUMN, MYF(0), name.str);
    return NULL;
  }

  
  virtual bool is_expensive()
  {
    if (is_expensive_cache < 0)
      is_expensive_cache= walk(&Item::is_expensive_processor, 0, NULL);
    return MY_TEST(is_expensive_cache);
  }
  virtual Field::geometry_type get_geometry_type() const
    { return Field::GEOM_GEOMETRY; };
  uint uint_geometry_type() const
  { return get_geometry_type(); }
  void set_geometry_type(uint type)
  {
    DBUG_ASSERT(0);
  }
  String *check_well_formed_result(String *str, bool send_error= 0);
  bool eq_by_collation(Item *item, bool binary_cmp, CHARSET_INFO *cs); 
  bool too_big_for_varchar() const
  { return max_char_length() > CONVERT_IF_BIGGER_TO_BLOB; }
  void fix_length_and_charset(uint32 max_char_length_arg, CHARSET_INFO *cs)
  {
    max_length= char_to_byte_length_safe(max_char_length_arg, cs->mbmaxlen);
    collation.collation= cs;
  }
  void fix_char_length(size_t max_char_length_arg)
  {
    max_length= char_to_byte_length_safe(max_char_length_arg,
                                         collation.collation->mbmaxlen);
  }
  
  virtual bool is_outer_field() const { DBUG_ASSERT(fixed); return FALSE; }

  
  virtual bool with_subquery() const { DBUG_ASSERT(fixed); return false; }

  Item* set_expr_cache(THD *thd);

  virtual Item_equal *get_item_equal() { return NULL; }
  virtual void set_item_equal(Item_equal *item_eq) {};
  virtual Item_equal *find_item_equal(COND_EQUAL *cond_equal) { return NULL; }
  
  virtual void set_join_tab_idx(uint join_tab_idx_arg)
  {
    if (join_tab_idx_arg < join_tab_idx)
      join_tab_idx= join_tab_idx_arg;
  }
  virtual uint get_join_tab_idx() { return join_tab_idx; }

  table_map view_used_tables(TABLE_LIST *view)
  {
    view->view_used_tables= 0;
    walk(&Item::view_used_tables_processor, 0, view);
    return view->view_used_tables;
  }

  
  virtual void get_cache_parameters(List<Item> &parameters) { };

  virtual void mark_as_condition_AND_part(TABLE_LIST *embedding) {};

  
  virtual uint exists2in_reserved_items() { return 0; };

  virtual Item *neg(THD *thd);

  
  virtual void under_not(Item_func_not * upper
                         __attribute__((unused))) {};
	

  void register_in(THD *thd);	 
  
  bool depends_only_on(table_map view_map) 
  { return marker & FULL_EXTRACTION_FL; }
  int get_extraction_flag()
  { return  marker & EXTRACTION_MASK; }
  void set_extraction_flag(int flags) 
  { 
    marker &= ~EXTRACTION_MASK;
    marker|= flags; 
  }
  void clear_extraction_flag()
  {
    marker &= ~EXTRACTION_MASK;
  }
};

MEM_ROOT *get_thd_memroot(THD *thd);

template <class T>
inline Item* get_item_copy (THD *thd, T* item)
{
  Item *copy= new (get_thd_memroot(thd)) T(*item);
  if (likely(copy))
    copy->register_in(thd);
  return copy;
}	



class With_subquery_cache
{
protected:
  bool m_with_subquery;
public:
  With_subquery_cache(): m_with_subquery(false) { }
  void join(const Item *item) { m_with_subquery|= item->with_subquery(); }
};


class Type_geometry_attributes
{
  uint m_geometry_type;
  static const uint m_geometry_type_unknown= Field::GEOM_GEOMETRYCOLLECTION + 1;
  void copy(const Type_handler *handler, const Type_all_attributes *gattr)
  {
    
    m_geometry_type= handler == &type_handler_geometry ?
                     gattr->uint_geometry_type() :
                     m_geometry_type_unknown;
  }
public:
  Type_geometry_attributes()
   :m_geometry_type(m_geometry_type_unknown)
  { }
  Type_geometry_attributes(const Type_handler *handler,
                           const Type_all_attributes *gattr)
   :m_geometry_type(m_geometry_type_unknown)
  {
    copy(handler, gattr);
  }
  void join(const Item *item)
  {
    
    if (m_geometry_type == m_geometry_type_unknown)
      copy(item->type_handler(), item);
    else if (item->type_handler() == &type_handler_geometry)
    {
      m_geometry_type=
        Field_geom::geometry_type_merge((Field_geom::geometry_type)
                                         m_geometry_type,
                                        (Field_geom::geometry_type)
                                         item->uint_geometry_type());
    }
  }
  Field::geometry_type get_geometry_type() const
  {
    return m_geometry_type == m_geometry_type_unknown ?
           Field::GEOM_GEOMETRY :
           (Field::geometry_type) m_geometry_type;
  }
  void set_geometry_type(uint type)
  {
    DBUG_ASSERT(type <= m_geometry_type_unknown);
    m_geometry_type= type;
  }
};





bool cmp_items(Item *a, Item *b);



class Item_args
{
protected:
  Item **args, *tmp_arg[2];
  uint arg_count;
  void set_arguments(THD *thd, List<Item> &list);
  bool walk_args(Item_processor processor, bool walk_subquery, void *arg)
  {
    for (uint i= 0; i < arg_count; i++)
    {
      if (args[i]->walk(processor, walk_subquery, arg))
        return true;
    }
    return false;
  }
  bool transform_args(THD *thd, Item_transformer transformer, uchar *arg);
  void propagate_equal_fields(THD *, const Item::Context &, COND_EQUAL *);
  bool excl_dep_on_table(table_map tab_map)
  {
    for (uint i= 0; i < arg_count; i++)
    {
      if (args[i]->const_item())
        continue;
      if (!args[i]->excl_dep_on_table(tab_map))
        return false;
    }
    return true;
  }
  bool excl_dep_on_grouping_fields(st_select_lex *sel)
  {
    for (uint i= 0; i < arg_count; i++)
    {
      if (args[i]->const_item())
        continue;
      if (!args[i]->excl_dep_on_grouping_fields(sel))
        return false;
    }
    return true;
  }
  bool eq(const Item_args *other, bool binary_cmp) const
  {
    for (uint i= 0; i < arg_count ; i++)
    {
      if (!args[i]->eq(other->args[i], binary_cmp))
        return false;
    }
    return true;
  }
public:
  Item_args(void)
    :args(NULL), arg_count(0)
  { }
  Item_args(Item *a)
    :args(tmp_arg), arg_count(1)
  {
    args[0]= a;
  }
  Item_args(Item *a, Item *b)
    :args(tmp_arg), arg_count(2)
  {
    args[0]= a; args[1]= b;
  }
  Item_args(THD *thd, Item *a, Item *b, Item *c)
  {
    arg_count= 0;
    if (likely((args= (Item**) thd_alloc(thd, sizeof(Item*) * 3))))
    {
      arg_count= 3;
      args[0]= a; args[1]= b; args[2]= c;
    }
  }
  Item_args(THD *thd, Item *a, Item *b, Item *c, Item *d)
  {
    arg_count= 0;
    if (likely((args= (Item**) thd_alloc(thd, sizeof(Item*) * 4))))
    {
      arg_count= 4;
      args[0]= a; args[1]= b; args[2]= c; args[3]= d;
    }
  }
  Item_args(THD *thd, Item *a, Item *b, Item *c, Item *d, Item* e)
  {
    arg_count= 5;
    if (likely((args= (Item**) thd_alloc(thd, sizeof(Item*) * 5))))
    {
      arg_count= 5;
      args[0]= a; args[1]= b; args[2]= c; args[3]= d; args[4]= e;
    }
  }
  Item_args(THD *thd, List<Item> &list)
  {
    set_arguments(thd, list);
  }
  Item_args(THD *thd, const Item_args *other);
  bool alloc_arguments(THD *thd, uint count);
  void add_argument(Item *item)
  {
    args[arg_count++]= item;
  }
  inline Item **arguments() const { return args; }
  inline uint argument_count() const { return arg_count; }
  inline void remove_arguments() { arg_count=0; }
};




class Field_enumerator
{
public:
  virtual void visit_field(Item_field *field)= 0;
  virtual ~Field_enumerator() {};             
  Field_enumerator() {}                       
};

class Item_string;



class Item_basic_value :public Item
{
  bool is_basic_value(const Item *item, Type type_arg) const
  {
    return item->basic_const_item() && item->type() == type_arg;
  }
  bool is_basic_value(Type type_arg) const
  {
    return basic_const_item() && type() == type_arg;
  }
  bool str_eq(const String *value,
              const String *other, CHARSET_INFO *cs, bool binary_cmp) const
  {
    return binary_cmp ?
      value->bin_eq(other) :
      collation.collation == cs && value->eq(other, collation.collation);
  }

protected:
  
  class Metadata: private MY_STRING_METADATA
  {
  public:
    Metadata(const String *str)
    {
      my_string_metadata_get(this, str->charset(), str->ptr(), str->length());
    }
    Metadata(const String *str, uint repertoire_arg)
    {
      MY_STRING_METADATA::repertoire= repertoire_arg;
      MY_STRING_METADATA::char_length= str->numchars();
    }
    uint repertoire() const { return MY_STRING_METADATA::repertoire; }
    size_t char_length() const { return MY_STRING_METADATA::char_length; }
  };
  void fix_charset_and_length(CHARSET_INFO *cs,
                              Derivation dv, Metadata metadata)
  {
    
    collation.set(cs, dv, metadata.repertoire());
    fix_char_length(metadata.char_length());
    decimals= NOT_FIXED_DEC;
  }
  void fix_charset_and_length_from_str_value(const String &str, Derivation dv)
  {
    fix_charset_and_length(str.charset(), dv, Metadata(&str));
  }
  Item_basic_value(THD *thd): Item(thd) {}
  
  bool null_eq(const Item *item) const
  {
    DBUG_ASSERT(is_basic_value(NULL_ITEM));
    return item->type() == NULL_ITEM;
  }
  bool str_eq(const String *value, const Item *item, bool binary_cmp) const
  {
    DBUG_ASSERT(is_basic_value(STRING_ITEM));
    return is_basic_value(item, STRING_ITEM) &&
           str_eq(value, ((Item_basic_value*)item)->val_str(NULL),
                  item->collation.collation, binary_cmp);
  }
  bool real_eq(double value, const Item *item) const
  {
    DBUG_ASSERT(is_basic_value(REAL_ITEM));
    return is_basic_value(item, REAL_ITEM) &&
           value == ((Item_basic_value*)item)->val_real();
  }
  bool int_eq(longlong value, const Item *item) const
  {
    DBUG_ASSERT(is_basic_value(INT_ITEM));
    return is_basic_value(item, INT_ITEM) &&
           value == ((Item_basic_value*)item)->val_int() &&
           (value >= 0 || item->unsigned_flag == unsigned_flag);
  }
};


class Item_basic_constant :public Item_basic_value
{
  table_map used_table_map;
public:
  Item_basic_constant(THD *thd): Item_basic_value(thd), used_table_map(0) {};
  void set_used_tables(table_map map) { used_table_map= map; }
  table_map used_tables() const { return used_table_map; }
  bool check_vcol_func_processor(void *arg) { return FALSE;}
  virtual Item_basic_constant *make_string_literal_concat(THD *thd,
                                                          const LEX_CSTRING *)
  {
    DBUG_ASSERT(0);
    return this;
  }
  
  void cleanup()
  {
    
    if (orig_name)
    {
      name.str=    orig_name;
      name.length= strlen(orig_name);
    }
  }
};




class Item_sp_variable :public Item
{
protected:
  
  THD *m_thd;

  bool fix_fields_from_item(THD *thd, Item **, const Item *);
public:
  LEX_CSTRING m_name;

public:
#ifdef DBUG_ASSERT_EXISTS
  
  const sp_head *m_sp;
#endif

public:
  Item_sp_variable(THD *thd, const LEX_CSTRING *sp_var_name);

public:
  bool fix_fields(THD *thd, Item **)= 0;

  double val_real();
  longlong val_int();
  String *val_str(String *sp);
  my_decimal *val_decimal(my_decimal *decimal_value);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool is_null();

public:
  void make_send_field(THD *thd, Send_field *field);
  
  inline bool const_item() const;
  
  inline int save_in_field(Field *field, bool no_conversions);
  inline bool send(Protocol *protocol, st_value *buffer);
  bool check_vcol_func_processor(void *arg) 
  {
    return mark_unsupported_function(m_name.str, arg, VCOL_IMPOSSIBLE);
  }
}; 



inline bool Item_sp_variable::const_item() const
{
  return TRUE;
}

inline int Item_sp_variable::save_in_field(Field *field, bool no_conversions)
{
  return this_item()->save_in_field(field, no_conversions);
}

inline bool Item_sp_variable::send(Protocol *protocol, st_value *buffer)
{
  return this_item()->send(protocol, buffer);
}




class Item_splocal :public Item_sp_variable,
                    private Settable_routine_parameter,
                    public Rewritable_query_parameter,
                    public Type_handler_hybrid_field_type
{
protected:
  const Sp_rcontext_handler *m_rcontext_handler;

  uint m_var_idx;

  Type m_type;

  bool append_value_for_log(THD *thd, String *str);

  sp_rcontext *get_rcontext(sp_rcontext *local_ctx) const;
  Item_field *get_variable(sp_rcontext *ctx) const;

public:
  Item_splocal(THD *thd, const Sp_rcontext_handler *rh,
               const LEX_CSTRING *sp_var_name, uint sp_var_idx,
               const Type_handler *handler,
               uint pos_in_q= 0, uint len_in_q= 0);

  bool fix_fields(THD *, Item **);
  Item *this_item();
  const Item *this_item() const;
  Item **this_item_addr(THD *thd, Item **);

  virtual void print(String *str, enum_query_type query_type);

public:
  inline const LEX_CSTRING *my_name() const;

  inline uint get_var_idx() const;

  inline enum Type type() const;
  const Type_handler *type_handler() const
  { return Type_handler_hybrid_field_type::type_handler(); }
  uint cols() const { return this_item()->cols(); }
  Item* element_index(uint i) { return this_item()->element_index(i); }
  Item** addr(uint i) { return this_item()->addr(i); }
  bool check_cols(uint c);

private:
  bool set_value(THD *thd, sp_rcontext *ctx, Item **it);

public:
  Item_splocal *get_item_splocal() { return this; }

  Rewritable_query_parameter *get_rewritable_query_parameter()
  { return this; }

  Settable_routine_parameter *get_settable_routine_parameter()
  { return this; }

  bool append_for_log(THD *thd, String *str);
  
  Item *get_copy(THD *thd) { return 0; }

  
  Field *create_field_for_create_select(TABLE *table)
  { return create_table_field_from_handler(table); }
};



class Item_splocal_with_delayed_data_type: public Item_splocal
{
public:
  Item_splocal_with_delayed_data_type(THD *thd,
                                      const Sp_rcontext_handler *rh,
                                      const LEX_CSTRING *sp_var_name,
                                      uint sp_var_idx,
                                      uint pos_in_q, uint len_in_q)
   :Item_splocal(thd, rh, sp_var_name, sp_var_idx, &type_handler_null,
                 pos_in_q, len_in_q)
  { }
};



class Item_splocal_row_field :public Item_splocal
{
protected:
  LEX_CSTRING m_field_name;
  uint m_field_idx;
  bool set_value(THD *thd, sp_rcontext *ctx, Item **it);
public:
  Item_splocal_row_field(THD *thd,
                         const Sp_rcontext_handler *rh,
                         const LEX_CSTRING *sp_var_name,
                         const LEX_CSTRING *sp_field_name,
                         uint sp_var_idx, uint sp_field_idx,
                         const Type_handler *handler,
                         uint pos_in_q= 0, uint len_in_q= 0)
   :Item_splocal(thd, rh, sp_var_name, sp_var_idx, handler, pos_in_q, len_in_q),
    m_field_name(*sp_field_name),
    m_field_idx(sp_field_idx)
  { }
  bool fix_fields(THD *thd, Item **);
  Item *this_item();
  const Item *this_item() const;
  Item **this_item_addr(THD *thd, Item **);
  bool append_for_log(THD *thd, String *str);
  void print(String *str, enum_query_type query_type);
};


class Item_splocal_row_field_by_name :public Item_splocal_row_field
{
  bool set_value(THD *thd, sp_rcontext *ctx, Item **it);
public:
  Item_splocal_row_field_by_name(THD *thd,
                                 const Sp_rcontext_handler *rh,
                                 const LEX_CSTRING *sp_var_name,
                                 const LEX_CSTRING *sp_field_name,
                                 uint sp_var_idx,
                                 const Type_handler *handler,
                                 uint pos_in_q= 0, uint len_in_q= 0)
   :Item_splocal_row_field(thd, rh, sp_var_name, sp_field_name,
                           sp_var_idx, 0 ,
                           handler, pos_in_q, len_in_q)
  { }
  bool fix_fields(THD *thd, Item **it);
  void print(String *str, enum_query_type query_type);
};




inline const LEX_CSTRING *Item_splocal::my_name() const
{
  return &m_name;
}

inline uint Item_splocal::get_var_idx() const
{
  return m_var_idx;
}

inline enum Item::Type Item_splocal::type() const
{
  return m_type;
}



class Item_case_expr :public Item_sp_variable
{
public:
  Item_case_expr(THD *thd, uint case_expr_id);

public:
  bool fix_fields(THD *thd, Item **);
  Item *this_item();
  const Item *this_item() const;
  Item **this_item_addr(THD *thd, Item **);

  inline enum Type type() const;
  const Type_handler *type_handler() const { return this_item()->type_handler(); }

public:
  
  virtual void print(String *str, enum_query_type query_type);
  Item *get_copy(THD *thd) { return 0; }

private:
  uint m_case_expr_id;
};



inline enum Item::Type Item_case_expr::type() const
{
  return this_item()->type();
}



class Item_name_const : public Item
{
  Item *value_item;
  Item *name_item;
  bool valid_args;
public:
  Item_name_const(THD *thd, Item *name_arg, Item *val);

  bool fix_fields(THD *, Item **);

  enum Type type() const;
  double val_real();
  longlong val_int();
  String *val_str(String *sp);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool is_null();
  virtual void print(String *str, enum_query_type query_type);

  const Type_handler *type_handler() const
  {
    return value_item->type_handler();
  }

  bool const_item() const
  {
    return TRUE;
  }

  int save_in_field(Field *field, bool no_conversions)
  {
    return  value_item->save_in_field(field, no_conversions);
  }

  bool send(Protocol *protocol, st_value *buffer)
  {
    return value_item->send(protocol, buffer);
  }
  bool check_vcol_func_processor(void *arg) 
  {
    return mark_unsupported_function("name_const()", arg, VCOL_IMPOSSIBLE);
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_name_const>(thd, this); }
};

class Item_num: public Item_basic_constant
{
public:
  Item_num(THD *thd): Item_basic_constant(thd) { collation.set_numeric(); }
  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs);
  bool check_partition_func_processor(void *int_arg) { return FALSE;}
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return type_handler()->Item_get_date(this, ltime, fuzzydate);
  }
};

#define NO_CACHED_FIELD_INDEX ((uint)(-1))

class st_select_lex;


class Item_result_field :public Item	
{
public:
  Field *result_field;				
  Item_result_field(THD *thd): Item(thd), result_field(0) {}
  
  Item_result_field(THD *thd, Item_result_field *item):
    Item(thd, item), result_field(item->result_field)
  {}
  ~Item_result_field() {}			
  Field *get_tmp_table_field() { return result_field; }
  
  table_map used_tables() const { return 1; }
  void set_result_field(Field *field) { result_field= field; }
  bool is_result_field() { return true; }
  void save_in_result_field(bool no_conversions)
  {
    save_in_field(result_field, no_conversions);
  }
  void cleanup();
  bool check_vcol_func_processor(void *arg) { return FALSE;}
};


class Item_ident :public Item_result_field
{
protected:
  
  const char *orig_db_name;
  const char *orig_table_name;
  LEX_CSTRING orig_field_name;

public:
  Name_resolution_context *context;
  const char *db_name;
  const char *table_name;
  LEX_CSTRING field_name;
  bool alias_name_used; 
  
  uint cached_field_index;
  
  TABLE_LIST *cached_table;
  st_select_lex *depended_from;
  
  bool can_be_depended;
  Item_ident(THD *thd, Name_resolution_context *context_arg,
             const char *db_name_arg, const char *table_name_arg,
             const LEX_CSTRING *field_name_arg);
  Item_ident(THD *thd, Item_ident *item);
  Item_ident(THD *thd, TABLE_LIST *view_arg, const LEX_CSTRING *field_name_arg);
  const char *full_name() const;
  void cleanup();
  st_select_lex *get_depended_from() const;
  bool remove_dependence_processor(void * arg);
  virtual void print(String *str, enum_query_type query_type);
  virtual bool change_context_processor(void *cntx)
    { context= (Name_resolution_context *)cntx; return FALSE; }
  
  virtual bool collect_outer_ref_processor(void *arg);
  friend bool insert_fields(THD *thd, Name_resolution_context *context,
                            const char *db_name,
                            const char *table_name, List_iterator<Item> *it,
                            bool any_privileges);
};


class Item_ident_for_show :public Item
{
public:
  Field *field;
  const char *db_name;
  const char *table_name;

  Item_ident_for_show(THD *thd, Field *par_field, const char *db_arg,
                      const char *table_name_arg):
    Item(thd), field(par_field), db_name(db_arg), table_name(table_name_arg)
  {
    Type_std_attributes::set(par_field->type_std_attributes());
  }
  enum Type type() const { return FIELD_ITEM; }
  double val_real() { return field->val_real(); }
  longlong val_int() { return field->val_int(); }
  String *val_str(String *str) { return field->val_str(str); }
  my_decimal *val_decimal(my_decimal *dec) { return field->val_decimal(dec); }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return field->get_date(ltime, fuzzydate);
  }
  void make_send_field(THD *thd, Send_field *tmp_field);
  const Type_handler *type_handler() const
  {
    const Type_handler *handler= field->type_handler();
    return handler->type_handler_for_item_field();
  }
  Item* get_copy(THD *thd)
  { return get_item_copy<Item_ident_for_show>(thd, this); }
};


class Item_field :public Item_ident,
                  public Load_data_outvar
{
protected:
  void set_field(Field *field);
public:
  Field *field;
  Item_equal *item_equal;
  
  uint have_privileges;
  
  bool any_privileges;
  Item_field(THD *thd, Name_resolution_context *context_arg,
             const char *db_arg,const char *table_name_arg,
	     const LEX_CSTRING *field_name_arg);
  
  Item_field(THD *thd, Item_field *item);
  
  Item_field(THD *thd, Name_resolution_context *context_arg, Field *field);
  
  Item_field(THD *thd, Field *field);
  enum Type type() const { return FIELD_ITEM; }
  bool eq(const Item *item, bool binary_cmp) const;
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *);
  String *val_str(String*);
  void save_result(Field *to);
  double val_result();
  longlong val_int_result();
  String *str_result(String* tmp);
  my_decimal *val_decimal_result(my_decimal *);
  bool val_bool_result();
  bool is_null_result();
  bool send(Protocol *protocol, st_value *buffer);
  Load_data_outvar *get_load_data_outvar()
  {
    return this;
  }
  bool load_data_set_null(THD *thd, const Load_data_param *param)
  {
    return field->load_data_set_null(thd);
  }
  bool load_data_set_value(THD *thd, const char *pos, uint length,
                           const Load_data_param *param)
  {
    field->load_data_set_value(pos, length, param->charset());
    return false;
  }
  bool load_data_set_no_data(THD *thd, const Load_data_param *param);
  void load_data_print_for_log_event(THD *thd, String *to) const;
  bool load_data_add_outvar(THD *thd, Load_data_param *param) const
  {
    return param->add_outvar_field(thd, field);
  }
  uint load_data_fixed_length() const
  {
    return field->field_length;
  }
  void reset_field(Field *f);
  bool fix_fields(THD *, Item **);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  void make_send_field(THD *thd, Send_field *tmp_field);
  int save_in_field(Field *field,bool no_conversions);
  void save_org_in_field(Field *field, fast_field_copier optimizer_data);
  fast_field_copier setup_fast_field_copier(Field *field);
  table_map used_tables() const;
  table_map all_used_tables() const; 
  const Type_handler *type_handler() const
  {
    const Type_handler *handler= field->type_handler();
    return handler->type_handler_for_item_field();
  }
  const Type_handler *cast_to_int_type_handler() const
  {
    return field->type_handler()->cast_to_int_type_handler();
  }
  const Type_handler *real_type_handler() const
  {
    if (field->is_created_from_null_item)
      return &type_handler_null;
    return field->type_handler();
  }
  TYPELIB *get_typelib() const { return field->get_typelib(); }
  enum_monotonicity_info get_monotonicity_info() const
  {
    return MONOTONIC_STRICT_INCREASING;
  }
  longlong val_int_endpoint(bool left_endp, bool *incl_endp);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool get_date_result(MYSQL_TIME *ltime,ulonglong fuzzydate);
  bool is_null() { return field->is_null(); }
  void update_null_value();
  void update_table_bitmaps()
  {
    if (field && field->table)
    {
      TABLE *tab= field->table;
      tab->covering_keys.intersect(field->part_of_key);
      if (tab->read_set)
        bitmap_fast_test_and_set(tab->read_set, field->field_index);
      
      if (field->vcol_info && tab->vcol_set)
        tab->mark_virtual_col(field);
    }
  }
  void update_used_tables()
  {
    update_table_bitmaps();
  }
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref)
  {
    
    DBUG_ASSERT(type() != FIELD_ITEM);
    return Item_ident::build_equal_items(thd, inherited, link_item_fields,
                                         cond_equal_ref);
  }
  bool is_result_field() { return false; }
  void save_in_result_field(bool no_conversions);
  Item *get_tmp_table_item(THD *thd);
  bool collect_item_field_processor(void * arg);
  bool add_field_to_set_processor(void * arg);
  bool find_item_in_field_list_processor(void *arg);
  bool register_field_in_read_map(void *arg);
  bool register_field_in_write_map(void *arg);
  bool register_field_in_bitmap(void *arg);
  bool check_partition_func_processor(void *int_arg) {return FALSE;}
  bool post_fix_fields_part_expr_processor(void *bool_arg);
  bool check_valid_arguments_processor(void *bool_arg);
  bool check_field_expression_processor(void *arg);
  bool enumerate_field_refs_processor(void *arg);
  bool update_table_bitmaps_processor(void *arg);
  bool switch_to_nullable_fields_processor(void *arg);
  bool update_vcol_processor(void *arg);
  bool rename_fields_processor(void *arg);
  bool check_vcol_func_processor(void *arg)
  {
    context= 0;
    if (field && (field->unireg_check == Field::NEXT_NUMBER))
    {
      
      return mark_unsupported_function(field_name.str, arg, VCOL_FIELD_REF | VCOL_AUTO_INC);
    }
    return mark_unsupported_function(field_name.str, arg, VCOL_FIELD_REF);
  }
  bool set_fields_as_dependent_processor(void *arg)
  {
    if (!(used_tables() & OUTER_REF_TABLE_BIT))
    {
      depended_from= (st_select_lex *) arg;
      item_equal= NULL;
    }
    return 0;
  }
  void cleanup();
  Item_equal *get_item_equal() { return item_equal; }
  void set_item_equal(Item_equal *item_eq) { item_equal= item_eq; }
  Item_equal *find_item_equal(COND_EQUAL *cond_equal);
  Item* propagate_equal_fields(THD *, const Context &, COND_EQUAL *);
  Item *replace_equal_field(THD *thd, uchar *arg);
  uint32 max_display_length() const { return field->max_display_length(); }
  Item_field *field_for_view_update() { return this; }
  int fix_outer_field(THD *thd, Field **field, Item **reference);
  virtual Item *update_value_transformer(THD *thd, uchar *select_arg);
  Item *derived_field_transformer_for_having(THD *thd, uchar *arg);
  Item *derived_field_transformer_for_where(THD *thd, uchar *arg);
  Item *derived_grouping_field_transformer_for_where(THD *thd, uchar *arg);
  virtual void print(String *str, enum_query_type query_type);
  bool excl_dep_on_table(table_map tab_map);
  bool excl_dep_on_grouping_fields(st_select_lex *sel);
  bool cleanup_excluding_fields_processor(void *arg)
  { return field ? 0 : cleanup_processor(arg); }
  bool cleanup_excluding_const_fields_processor(void *arg)
  { return field && const_item() ? 0 : cleanup_processor(arg); }
  
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_field>(thd, this); }
  bool is_outer_field() const
  {
    DBUG_ASSERT(fixed);
    return field->table->pos_in_table_list->outer_join;
  }
  Field::geometry_type get_geometry_type() const
  {
    DBUG_ASSERT(field_type() == MYSQL_TYPE_GEOMETRY);
    return field->get_geometry_type();
  }
  friend class Item_default_value;
  friend class Item_insert_value;
  friend class st_select_lex_unit;
};



class Item_field_row: public Item_field,
                      public Item_args
{
public:
  Item_field_row(THD *thd, Field *field)
   :Item_field(thd, field),
    Item_args()
  { }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_field_row>(thd, this); }

  const Type_handler *type_handler() const { return &type_handler_row; }
  uint cols() const { return arg_count; }
  Item* element_index(uint i) { return arg_count ? args[i] : this; }
  Item** addr(uint i) { return arg_count ? args + i : NULL; }
  bool check_cols(uint c)
  {
    if (cols() != c)
    {
      my_error(ER_OPERAND_COLUMNS, MYF(0), c);
      return true;
    }
    return false;
  }
  bool row_create_items(THD *thd, List<Spvar_definition> *list);
};




class Item_temptable_field :public Item_field
{
public:
  Item_temptable_field(THD *thd, Name_resolution_context *context_arg, Field *field)
   : Item_field(thd, context_arg, field) {}

  Item_temptable_field(THD *thd, Field *field)
   : Item_field(thd, field) {}

  Item_temptable_field(THD *thd, Item_field *item) : Item_field(thd, item) {};

  virtual void print(String *str, enum_query_type query_type);
};


class Item_null :public Item_basic_constant
{
public:
  Item_null(THD *thd, const char *name_par=0, CHARSET_INFO *cs= &my_charset_bin):
    Item_basic_constant(thd)
  {
    maybe_null= null_value= TRUE;
    max_length= 0;
    name.str= name_par ? name_par : "NULL";
    name.length= strlen(name.str);
    fixed= 1;
    collation.set(cs, DERIVATION_IGNORABLE, MY_REPERTOIRE_ASCII);
  }
  enum Type type() const { return NULL_ITEM; }
  bool eq(const Item *item, bool binary_cmp) const { return null_eq(item); }
  double val_real();
  longlong val_int();
  String *val_str(String *str);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  int save_in_field(Field *field, bool no_conversions);
  int save_safe_in_field(Field *field);
  bool send(Protocol *protocol, st_value *buffer);
  const Type_handler *type_handler() const { return &type_handler_null; }
  bool basic_const_item() const { return 1; }
  Item *clone_item(THD *thd);
  bool is_null() { return 1; }

  virtual inline void print(String *str, enum_query_type query_type)
  {
    str->append(STRING_WITH_LEN("NULL"));
  }

  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs);
  bool check_partition_func_processor(void *int_arg) {return FALSE;}
  Item_basic_constant *make_string_literal_concat(THD *thd,
                                                  const LEX_CSTRING *);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_null>(thd, this); }
};

class Item_null_result :public Item_null
{
public:
  Field *result_field;
  Item_null_result(THD *thd): Item_null(thd), result_field(0) {}
  bool is_result_field() { return result_field != 0; }
  enum_field_types field_type() const
  {
    return result_field->type();
  }
  void save_in_result_field(bool no_conversions)
  {
    save_in_field(result_field, no_conversions);
  }
  bool check_partition_func_processor(void *int_arg) {return TRUE;}
  bool check_vcol_func_processor(void *arg)
  {
    return mark_unsupported_function(full_name(), arg, VCOL_IMPOSSIBLE);
  }
};



class Item_param :public Item_basic_value,
                  private Settable_routine_parameter,
                  public Rewritable_query_parameter,
                  private Type_handler_hybrid_field_type,
                  public Type_geometry_attributes
{
  
  enum enum_item_param_state
  {
    NO_VALUE, NULL_VALUE, SHORT_DATA_VALUE, LONG_DATA_VALUE,
    DEFAULT_VALUE, IGNORE_VALUE
  } state;

  enum Type item_type;

  void fix_type(Type type)
  {
    item_type= type;
    fixed= true;
  }

  void fix_temporal(uint32 max_length_arg, uint decimals_arg);

  struct CONVERSION_INFO
  {
    
    CHARSET_INFO *character_set_client;
    CHARSET_INFO *character_set_of_placeholder;
    
    CHARSET_INFO *final_character_set_of_str_value;
  private:
    bool needs_conversion() const
    {
      return final_character_set_of_str_value !=
             character_set_of_placeholder;
    }
    bool convert(THD *thd, String *str);
  public:
    void set(THD *thd, CHARSET_INFO *cs);
    bool convert_if_needed(THD *thd, String *str)
    {
      
      if (needs_conversion())
        return convert(thd, str);
      str->set_charset(final_character_set_of_str_value);
      return false;
    }
  };

  bool m_empty_string_is_null;

  class PValue_simple
  {
  public:
    union
    {
      longlong integer;
      double   real;
      CONVERSION_INFO cs_info;
      MYSQL_TIME     time;
    };
    void swap(PValue_simple &other)
    {
      swap_variables(PValue_simple, *this, other);
    }
  };

  class PValue: public Type_handler_hybrid_field_type,
                public PValue_simple,
                public Value_source
  {
  public:
    PValue(): Type_handler_hybrid_field_type(&type_handler_null) {}
    my_decimal m_decimal;
    String m_string;
    
    String m_string_ptr;

    void swap(PValue &other)
    {
      Type_handler_hybrid_field_type::swap(other);
      PValue_simple::swap(other);
      m_decimal.swap(other.m_decimal);
      m_string.swap(other.m_string);
      m_string_ptr.swap(other.m_string_ptr);
    }
    double val_real() const;
    longlong val_int(const Type_std_attributes *attr) const;
    my_decimal *val_decimal(my_decimal *dec, const Type_std_attributes *attr);
    String *val_str(String *str, const Type_std_attributes *attr);
  };

  PValue value;

  const String *value_query_val_str(THD *thd, String* str) const;
  bool value_eq(const Item *item, bool binary_cmp) const;
  Item *value_clone_item(THD *thd);
  bool can_return_value() const;

public:
  
  enum enum_indicator_type indicator;

  const Type_handler *type_handler() const
  { return Type_handler_hybrid_field_type::type_handler(); }

  Field::geometry_type get_geometry_type() const
  { return Type_geometry_attributes::get_geometry_type(); };

  void set_geometry_type(uint type)
  { Type_geometry_attributes::set_geometry_type(type); }

  Item_param(THD *thd, const LEX_CSTRING *name_arg,
             uint pos_in_query_arg, uint len_in_query_arg);

  enum Type type() const
  {
    DBUG_ASSERT(fixed || state == NO_VALUE);
    return item_type;
  }

  double val_real()
  {
    return can_return_value() ? value.val_real() : 0e0;
  }
  longlong val_int()
  {
    return can_return_value() ? value.val_int(this) : 0;
  }
  my_decimal *val_decimal(my_decimal *dec)
  {
    return can_return_value() ? value.val_decimal(dec, this) : NULL;
  }
  String *val_str(String *str)
  {
    return can_return_value() ? value.val_str(str, this) : NULL;
  }
  bool get_date(MYSQL_TIME *tm, ulonglong fuzzydate);
  int  save_in_field(Field *field, bool no_conversions);

  void set_default();
  void set_ignore();
  void set_null();
  void set_int(longlong i, uint32 max_length_arg);
  void set_double(double i);
  void set_decimal(const char *str, ulong length);
  void set_decimal(const my_decimal *dv, bool unsigned_arg);
  bool set_str(const char *str, ulong length,
               CHARSET_INFO *fromcs, CHARSET_INFO *tocs);
  bool set_longdata(const char *str, ulong length);
  void set_time(MYSQL_TIME *tm, timestamp_type type, uint32 max_length_arg);
  void set_time(const MYSQL_TIME *tm, uint32 max_length_arg, uint decimals_arg);
  bool set_from_item(THD *thd, Item *item);
  void reset();

  void set_param_tiny(uchar **pos, ulong len);
  void set_param_short(uchar **pos, ulong len);
  void set_param_int32(uchar **pos, ulong len);
  void set_param_int64(uchar **pos, ulong len);
  void set_param_float(uchar **pos, ulong len);
  void set_param_double(uchar **pos, ulong len);
  void set_param_decimal(uchar **pos, ulong len);
  void set_param_time(uchar **pos, ulong len);
  void set_param_datetime(uchar **pos, ulong len);
  void set_param_date(uchar **pos, ulong len);
  void set_param_str(uchar **pos, ulong len);

  void setup_conversion(THD *thd, uchar param_type);
  void setup_conversion_blob(THD *thd);
  void setup_conversion_string(THD *thd, CHARSET_INFO *fromcs);

  
  void set_param_func(uchar **pos, ulong len)
  {
    
    const Type_handler *h= Item_param::type_handler();
    value.set_handler(h);
    h->Item_param_set_param_func(this, pos, len);
  }

  bool set_value(THD *thd, const Type_all_attributes *attr,
                 const st_value *val, const Type_handler *h)
  {
    value.set_handler(h); 
    return h->Item_param_set_from_value(thd, this, attr, val);
  }

  bool set_limit_clause_param(longlong nr)
  {
    value.set_handler(&type_handler_longlong);
    set_int(nr, MY_INT64_NUM_DECIMAL_DIGITS);
    return !unsigned_flag && value.integer < 0;
  }
  const String *query_val_str(THD *thd, String *str) const;

  bool convert_str_value(THD *thd);

  
  virtual table_map used_tables() const
  { return state != NO_VALUE ? (table_map)0 : PARAM_TABLE_BIT; }
  virtual void print(String *str, enum_query_type query_type);
  bool is_null()
  { DBUG_ASSERT(state != NO_VALUE); return state == NULL_VALUE; }
  bool basic_const_item() const;
  bool has_no_value() const
  {
    return state == NO_VALUE;
  }
  bool has_long_data_value() const
  {
    return state == LONG_DATA_VALUE;
  }
  bool has_int_value() const
  {
    return state == SHORT_DATA_VALUE &&
           value.type_handler()->cmp_type() == INT_RESULT;
  }
  
  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs);
  Item *clone_item(THD *thd);
  
  bool eq(const Item *item, bool binary_cmp) const;
  void set_param_type_and_swap_value(Item_param *from);

  Rewritable_query_parameter *get_rewritable_query_parameter()
  { return this; }
  Settable_routine_parameter *get_settable_routine_parameter()
  { return m_is_settable_routine_parameter ? this : NULL; }

  bool append_for_log(THD *thd, String *str);
  bool check_vcol_func_processor(void *int_arg) {return FALSE;}
  Item *get_copy(THD *thd) { return 0; }

  bool add_as_clone(THD *thd);
  void sync_clones();
  bool register_clone(Item_param *i) { return m_clones.push_back(i); }

private:
  void invalid_default_param() const;

  virtual bool set_value(THD *thd, sp_rcontext *ctx, Item **it);

  virtual void set_out_param_info(Send_field *info);

public:
  virtual const Send_field *get_out_param_info() const;

  Item_param *get_item_param() { return this; }

  virtual void make_send_field(THD *thd, Send_field *field);

private:
  Send_field *m_out_param_info;
  bool m_is_settable_routine_parameter;
  
  Mem_root_array<Item_param *, true> m_clones;
};


class Item_int :public Item_num
{
public:
  longlong value;
  Item_int(THD *thd, int32 i,size_t length= MY_INT32_NUM_DECIMAL_DIGITS):
    Item_num(thd), value((longlong) i)
    { max_length=(uint32)length; fixed= 1; }
  Item_int(THD *thd, longlong i,size_t length= MY_INT64_NUM_DECIMAL_DIGITS):
    Item_num(thd), value(i)
    { max_length=(uint32)length; fixed= 1; }
  Item_int(THD *thd, ulonglong i, size_t length= MY_INT64_NUM_DECIMAL_DIGITS):
    Item_num(thd), value((longlong)i)
    { max_length=(uint32)length; fixed= 1; unsigned_flag= 1; }
  Item_int(THD *thd, const char *str_arg,longlong i,size_t length):
    Item_num(thd), value(i)
    {
      max_length=(uint32)length;
      name.str= str_arg; name.length= safe_strlen(name.str);
      fixed= 1;
    }
  Item_int(THD *thd, const char *str_arg,longlong i,size_t length, bool flag):
    Item_num(thd), value(i)
    {
      max_length=(uint32)length;
      name.str= str_arg; name.length= safe_strlen(name.str);
      fixed= 1;
      unsigned_flag= flag;
    }
  Item_int(THD *thd, const char *str_arg, size_t length=64);
  enum Type type() const { return INT_ITEM; }
  const Type_handler *type_handler() const
  { return type_handler_long_or_longlong(); }
  Field *create_tmp_field(bool group, TABLE *table)
  { return tmp_table_field_from_field_type(table); }
  Field *create_field_for_create_select(TABLE *table)
  { return tmp_table_field_from_field_type(table); }
  longlong val_int() { DBUG_ASSERT(fixed == 1); return value; }
  longlong val_int_min() const { DBUG_ASSERT(fixed == 1); return value; }
  double val_real() { DBUG_ASSERT(fixed == 1); return (double) value; }
  my_decimal *val_decimal(my_decimal *);
  String *val_str(String*);
  int save_in_field(Field *field, bool no_conversions);
  bool basic_const_item() const { return 1; }
  Item *clone_item(THD *thd);
  virtual void print(String *str, enum_query_type query_type);
  Item *neg(THD *thd);
  uint decimal_precision() const
  { return (uint) (max_length - MY_TEST(value < 0)); }
  bool eq(const Item *item, bool binary_cmp) const
  { return int_eq(value, item); }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_int>(thd, this); }
};



class Item_bool :public Item_int
{
public:
  Item_bool(THD *thd, const char *str_arg, longlong i):
    Item_int(thd, str_arg, i, 1) {}
  bool is_bool_type() { return true; }
  Item *neg_transformer(THD *thd);
};


class Item_uint :public Item_int
{
public:
  Item_uint(THD *thd, const char *str_arg, size_t length);
  Item_uint(THD *thd, ulonglong i): Item_int(thd, i, 10) {}
  Item_uint(THD *thd, const char *str_arg, longlong i, uint length);
  double val_real()
    { DBUG_ASSERT(fixed == 1); return ulonglong2double((ulonglong)value); }
  String *val_str(String*);
  Item *clone_item(THD *thd);
  virtual void print(String *str, enum_query_type query_type);
  Item *neg(THD *thd);
  uint decimal_precision() const { return max_length; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_uint>(thd, this); }
};


class Item_datetime :public Item_int
{
protected:
  MYSQL_TIME ltime;
public:
  Item_datetime(THD *thd): Item_int(thd, 0) { unsigned_flag=0; }
  int save_in_field(Field *field, bool no_conversions);
  longlong val_int();
  double val_real() { return (double)val_int(); }
  void set(longlong packed, enum_mysql_timestamp_type ts_type);
  bool get_date(MYSQL_TIME *to, ulonglong fuzzydate)
  {
    *to= ltime;
    return false;
  }
};



class Item_decimal :public Item_num
{
protected:
  my_decimal decimal_value;
public:
  Item_decimal(THD *thd, const char *str_arg, size_t length,
               CHARSET_INFO *charset);
  Item_decimal(THD *thd, const char *str, const my_decimal *val_arg,
               uint decimal_par, uint length);
  Item_decimal(THD *thd, my_decimal *value_par);
  Item_decimal(THD *thd, longlong val, bool unsig);
  Item_decimal(THD *thd, double val, int precision, int scale);
  Item_decimal(THD *thd, const uchar *bin, int precision, int scale);

  enum Type type() const { return DECIMAL_ITEM; }
  const Type_handler *type_handler() const { return &type_handler_newdecimal; }
  longlong val_int();
  double val_real();
  String *val_str(String*);
  my_decimal *val_decimal(my_decimal *val) { return &decimal_value; }
  int save_in_field(Field *field, bool no_conversions);
  bool basic_const_item() const { return 1; }
  Item *clone_item(THD *thd);
  virtual void print(String *str, enum_query_type query_type);
  Item *neg(THD *thd);
  uint decimal_precision() const { return decimal_value.precision(); }
  bool eq(const Item *, bool binary_cmp) const;
  void set_decimal_value(my_decimal *value_par);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_decimal>(thd, this); }
};


class Item_float :public Item_num
{
  const char *presentation;
public:
  double value;
  Item_float(THD *thd, const char *str_arg, size_t length);
  Item_float(THD *thd, const char *str, double val_arg, uint decimal_par,
             uint length): Item_num(thd), value(val_arg)
  {
    presentation= name.str= str;
    name.length= safe_strlen(str);
    decimals=(uint8) decimal_par;
    max_length= length;
    fixed= 1;
  }
  Item_float(THD *thd, double value_par, uint decimal_par):
    Item_num(thd), presentation(0), value(value_par)
  {
    decimals= (uint8) decimal_par;
    fixed= 1;
  }
  int save_in_field(Field *field, bool no_conversions);
  enum Type type() const { return REAL_ITEM; }
  const Type_handler *type_handler() const { return &type_handler_double; }
  double val_real() { DBUG_ASSERT(fixed == 1); return value; }
  longlong val_int()
  {
    DBUG_ASSERT(fixed == 1);
    if (value <= (double) LONGLONG_MIN)
    {
       return LONGLONG_MIN;
    }
    else if (value >= (double) (ulonglong) LONGLONG_MAX)
    {
      return LONGLONG_MAX;
    }
    return (longlong) rint(value);
  }
  String *val_str(String*);
  my_decimal *val_decimal(my_decimal *);
  bool basic_const_item() const { return 1; }
  Item *clone_item(THD *thd);
  Item *neg(THD *thd);
  virtual void print(String *str, enum_query_type query_type);
  bool eq(const Item *item, bool binary_cmp) const
  { return real_eq(value, item); }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_float>(thd, this); }
};


class Item_static_float_func :public Item_float
{
  const char *func_name;
public:
  Item_static_float_func(THD *thd, const char *str, double val_arg,
                         uint decimal_par, uint length):
    Item_float(thd, NullS, val_arg, decimal_par, length), func_name(str)
  {}

  virtual inline void print(String *str, enum_query_type query_type)
  {
    str->append(func_name);
  }

  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs)
  {
    return const_charset_converter(thd, tocs, true, func_name);
  }
};


class Item_string :public Item_basic_constant
{
protected:
  void fix_from_value(Derivation dv, const Metadata metadata)
  {
    fix_charset_and_length(str_value.charset(), dv, metadata);
    
    fixed= 1;
  }
  void fix_and_set_name_from_value(THD *thd, Derivation dv,
                                   const Metadata metadata)
  {
    fix_from_value(dv, metadata);
    set_name(thd, str_value.ptr(), str_value.length(), str_value.charset());
  }
protected:
  
  Item_string(THD *thd, CHARSET_INFO *cs, Derivation dv= DERIVATION_COERCIBLE):
    Item_basic_constant(thd)
  {
    collation.set(cs, dv);
    max_length= 0;
    set_name(thd, NULL, 0, system_charset_info);
    decimals= NOT_FIXED_DEC;
    fixed= 1;
  }
public:
  Item_string(THD *thd, CHARSET_INFO *csi, const char *str_arg, uint length_arg):
    Item_basic_constant(thd)
  {
    collation.set(csi, DERIVATION_COERCIBLE);
    set_name(thd, NULL, 0, system_charset_info);
    decimals= NOT_FIXED_DEC;
    fixed= 1;
    str_value.copy(str_arg, length_arg, csi);
    max_length= str_value.numchars() * csi->mbmaxlen;
  }
  
  Item_string(THD *thd, const char *str, uint length, CHARSET_INFO *cs,
              Derivation dv, uint repertoire): Item_basic_constant(thd)
  {
    str_value.set_or_copy_aligned(str, length, cs);
    fix_and_set_name_from_value(thd, dv, Metadata(&str_value, repertoire));
  }
  Item_string(THD *thd, const char *str, size_t length,
              CHARSET_INFO *cs, Derivation dv= DERIVATION_COERCIBLE):
    Item_basic_constant(thd)
  {
    str_value.set_or_copy_aligned(str, length, cs);
    fix_and_set_name_from_value(thd, dv, Metadata(&str_value));
  }
  Item_string(THD *thd, const String *str, CHARSET_INFO *tocs, uint *conv_errors,
              Derivation dv, uint repertoire): Item_basic_constant(thd)
  {
    if (str_value.copy(str, tocs, conv_errors))
      str_value.set("", 0, tocs); 
    str_value.mark_as_const();
    fix_and_set_name_from_value(thd, dv, Metadata(&str_value, repertoire));
  }
  
  Item_string(THD *thd, const char *name_par, const char *str, size_t length,
              CHARSET_INFO *cs, Derivation dv= DERIVATION_COERCIBLE):
    Item_basic_constant(thd)
  {
    str_value.set_or_copy_aligned(str, length, cs);
    fix_from_value(dv, Metadata(&str_value));
    set_name(thd, name_par,safe_strlen(name_par), system_charset_info);
  }
  Item_string(THD *thd, const char *name_par, const char *str, size_t length,
              CHARSET_INFO *cs, Derivation dv, uint repertoire):
    Item_basic_constant(thd)
  {
    str_value.set_or_copy_aligned(str, length, cs);
    fix_from_value(dv, Metadata(&str_value, repertoire));
    set_name(thd, name_par, safe_strlen(name_par), system_charset_info);
  }
  void print_value(String *to) const
  {
    str_value.print(to);
  }
  enum Type type() const { return STRING_ITEM; }
  double val_real();
  longlong val_int();
  String *val_str(String*)
  {
    DBUG_ASSERT(fixed == 1);
    return (String*) &str_value;
  }
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return get_date_from_string(ltime, fuzzydate);
  }
  int save_in_field(Field *field, bool no_conversions);
  const Type_handler *type_handler() const { return &type_handler_varchar; }
  bool basic_const_item() const { return 1; }
  bool eq(const Item *item, bool binary_cmp) const
  {
    return str_eq(&str_value, item, binary_cmp);
  }
  Item *clone_item(THD *thd);
  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs)
  {
    return const_charset_converter(thd, tocs, true);
  }
  inline void append(const char *str, uint length)
  {
    str_value.append(str, length);
    max_length= str_value.numchars() * collation.collation->mbmaxlen;
  }
  virtual void print(String *str, enum_query_type query_type);
  bool check_partition_func_processor(void *int_arg) {return FALSE;}

  
  virtual bool is_cs_specified() const
  {
    return false;
  }

  String *check_well_formed_result(bool send_error)
  { return Item::check_well_formed_result(&str_value, send_error); }

  enum_field_types odbc_temporal_literal_type(const LEX_CSTRING *type_str) const
  {
    
    if (collation.repertoire == MY_REPERTOIRE_ASCII &&
        str_value.length() < MAX_DATE_STRING_REP_LENGTH * 4)
    {
      if (type_str->length == 1)
      {
        if (type_str->str[0] == 'd')  
          return MYSQL_TYPE_DATE;
        else if (type_str->str[0] == 't') 
          return MYSQL_TYPE_TIME;
      }
      else if (type_str->length == 2) 
      {
        if (type_str->str[0] == 't' && type_str->str[1] == 's')
          return MYSQL_TYPE_DATETIME;
      }
    }
    return MYSQL_TYPE_STRING; 
  }
  Item_basic_constant *make_string_literal_concat(THD *thd,
                                                  const LEX_CSTRING *);
  Item *make_odbc_literal(THD *thd, const LEX_CSTRING *typestr);

  Item *get_copy(THD *thd)
  { return get_item_copy<Item_string>(thd, this); }

};


class Item_string_with_introducer :public Item_string
{
public:
  Item_string_with_introducer(THD *thd, const char *str, uint length,
                              CHARSET_INFO *cs):
    Item_string(thd, str, length, cs)
  { }
  Item_string_with_introducer(THD *thd, const char *name_arg,
                              const char *str, uint length, CHARSET_INFO *tocs):
    Item_string(thd, name_arg, str, length, tocs)
  { }
  virtual bool is_cs_specified() const
  {
    return true;
  }
};


class Item_string_sys :public Item_string
{
public:
  Item_string_sys(THD *thd, const char *str, uint length):
    Item_string(thd, str, length, system_charset_info)
  { }
  Item_string_sys(THD *thd, const char *str):
    Item_string(thd, str, (uint) strlen(str), system_charset_info)
  { }
};


class Item_string_ascii :public Item_string
{
public:
  Item_string_ascii(THD *thd, const char *str, uint length):
    Item_string(thd, str, length, &my_charset_latin1,
                DERIVATION_COERCIBLE, MY_REPERTOIRE_ASCII)
  { }
  Item_string_ascii(THD *thd, const char *str):
    Item_string(thd, str, (uint) strlen(str), &my_charset_latin1,
                DERIVATION_COERCIBLE, MY_REPERTOIRE_ASCII)
  { }
};


class Item_static_string_func :public Item_string
{
  const char *func_name;
public:
  Item_static_string_func(THD *thd, const char *name_par, const char *str,
                          uint length, CHARSET_INFO *cs,
                          Derivation dv= DERIVATION_COERCIBLE):
    Item_string(thd, NullS, str, length, cs, dv), func_name(name_par)
  {}
  Item_static_string_func(THD *thd, const char *name_par,
                          const String *str,
                          CHARSET_INFO *tocs, uint *conv_errors,
                          Derivation dv, uint repertoire):
    Item_string(thd, str, tocs, conv_errors, dv, repertoire),
    func_name(name_par)
  {}
  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs)
  {
    return const_charset_converter(thd, tocs, true, func_name);
  }

  virtual inline void print(String *str, enum_query_type query_type)
  {
    str->append(func_name);
  }

  bool check_partition_func_processor(void *int_arg) {return TRUE;}

  bool check_vcol_func_processor(void *arg)
  { 
    
    return mark_unsupported_function(func_name, arg, VCOL_TIME_FUNC);
  }
};



class Item_partition_func_safe_string: public Item_string
{
public:
  Item_partition_func_safe_string(THD *thd, const char *name_arg, uint length,
                                  CHARSET_INFO *cs= NULL):
    Item_string(thd, name_arg, length, cs)
  {}
  bool check_vcol_func_processor(void *arg) 
  {
    return mark_unsupported_function("safe_string", arg, VCOL_IMPOSSIBLE);
  }
};


class Item_return_date_time :public Item_partition_func_safe_string
{
  enum_field_types date_time_field_type;
public:
  Item_return_date_time(THD *thd, const char *name_arg, uint length_arg,
                        enum_field_types field_type_arg, uint dec_arg= 0):
    Item_partition_func_safe_string(thd, name_arg, length_arg, &my_charset_bin),
    date_time_field_type(field_type_arg)
  { decimals= dec_arg; }
  const Type_handler *type_handler() const
  {
    return Type_handler::get_handler_by_field_type(date_time_field_type);
  }
};


class Item_blob :public Item_partition_func_safe_string
{
public:
  Item_blob(THD *thd, const char *name_arg, uint length):
    Item_partition_func_safe_string(thd, name_arg, (uint) safe_strlen(name_arg),
                                    &my_charset_bin)
  { max_length= length; }
  enum Type type() const { return TYPE_HOLDER; }
  const Type_handler *type_handler() const
  {
    return Type_handler::blob_type_handler(max_length);
  }
  const Type_handler *real_type_handler() const
  {
    
    DBUG_ASSERT(0);
    return &type_handler_varchar;
  }
  Field *create_field_for_schema(THD *thd, TABLE *table)
  { return tmp_table_field_from_field_type(table); }
};




class Item_empty_string :public Item_partition_func_safe_string
{
public:
  Item_empty_string(THD *thd, const char *header,uint length,
                    CHARSET_INFO *cs= NULL):
    Item_partition_func_safe_string(thd, "", 0,
                                    cs ? cs : &my_charset_utf8_general_ci)
    {
      name.str=    header;
      name.length= strlen(name.str);
      max_length= length * collation.collation->mbmaxlen;
    }
  void make_send_field(THD *thd, Send_field *field);
};


class Item_return_int :public Item_int
{
  enum_field_types int_field_type;
public:
  Item_return_int(THD *thd, const char *name_arg, uint length,
		  enum_field_types field_type_arg, longlong value_arg= 0):
    Item_int(thd, name_arg, value_arg, length), int_field_type(field_type_arg)
  {
    unsigned_flag=1;
  }
  const Type_handler *type_handler() const
  {
    return Type_handler::get_handler_by_field_type(int_field_type);
  }
};



class Item_hex_constant: public Item_basic_constant
{
private:
  void hex_string_init(THD *thd, const char *str, size_t str_length);
public:
  Item_hex_constant(THD *thd): Item_basic_constant(thd)
  {
    hex_string_init(thd, "", 0);
  }
  Item_hex_constant(THD *thd, const char *str, size_t str_length):
    Item_basic_constant(thd)
  {
    hex_string_init(thd, str, str_length);
  }
  enum Type type() const { return VARBIN_ITEM; }
  const Type_handler *type_handler() const { return &type_handler_varchar; }
  virtual Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs)
  {
    return const_charset_converter(thd, tocs, true);
  }
  bool check_partition_func_processor(void *int_arg) {return FALSE;}
  bool basic_const_item() const { return 1; }
  bool eq(const Item *item, bool binary_cmp) const
  {
    return item->basic_const_item() && item->type() == type() &&
           item->cast_to_int_type_handler() == cast_to_int_type_handler() &&
           str_value.bin_eq(&((Item_hex_constant*)item)->str_value);
  }
  String *val_str(String*) { DBUG_ASSERT(fixed == 1); return &str_value; }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return type_handler()->Item_get_date(this, ltime, fuzzydate);
  }
};



class Item_hex_hybrid: public Item_hex_constant
{
public:
  Item_hex_hybrid(THD *thd): Item_hex_constant(thd) {}
  Item_hex_hybrid(THD *thd, const char *str, size_t str_length):
    Item_hex_constant(thd, str, str_length) {}
  uint decimal_precision() const;
  double val_real()
  { 
    DBUG_ASSERT(fixed == 1); 
    return (double) (ulonglong) Item_hex_hybrid::val_int();
  }
  longlong val_int()
  {
    
    DBUG_ASSERT(fixed == 1);
    return longlong_from_hex_hybrid(str_value.ptr(), str_value.length());
  }
  my_decimal *val_decimal(my_decimal *decimal_value)
  {
    
    DBUG_ASSERT(fixed == 1);
    longlong value= Item_hex_hybrid::val_int();
    int2my_decimal(E_DEC_FATAL_ERROR, value, TRUE, decimal_value);
    return decimal_value;
  }
  int save_in_field(Field *field, bool no_conversions)
  {
    field->set_notnull();
    return field->store_hex_hybrid(str_value.ptr(), str_value.length());
  }
  const Type_handler *cast_to_int_type_handler() const
  {
    return &type_handler_longlong;
  }
  const Type_handler *type_handler_for_system_time() const
  {
    return &type_handler_longlong;
  }
  void print(String *str, enum_query_type query_type);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_hex_hybrid>(thd, this); }
};



class Item_hex_string: public Item_hex_constant
{
public:
  Item_hex_string(THD *thd): Item_hex_constant(thd) {}
  Item_hex_string(THD *thd, const char *str, size_t str_length):
    Item_hex_constant(thd, str, str_length) {}
  longlong val_int()
  {
    DBUG_ASSERT(fixed == 1);
    return longlong_from_string_with_check(&str_value);
  }
  double val_real()
  { 
    DBUG_ASSERT(fixed == 1);
    return double_from_string_with_check(&str_value);
  }
  my_decimal *val_decimal(my_decimal *decimal_value)
  {
    return val_decimal_from_string(decimal_value);
  }
  int save_in_field(Field *field, bool no_conversions)
  {
    field->set_notnull();
    return field->store(str_value.ptr(), str_value.length(), 
                        collation.collation);
  }
  void print(String *str, enum_query_type query_type);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_hex_string>(thd, this); }
};


class Item_bin_string: public Item_hex_hybrid
{
public:
  Item_bin_string(THD *thd, const char *str, size_t str_length);
};


class Item_temporal_literal :public Item_basic_constant
{
protected:
  MYSQL_TIME cached_time;
public:
  
  Item_temporal_literal(THD *thd, const MYSQL_TIME *ltime)
   :Item_basic_constant(thd)
  {
    collation.set(&my_charset_numeric, DERIVATION_NUMERIC, MY_REPERTOIRE_ASCII);
    decimals= 0;
    cached_time= *ltime;
  }
  Item_temporal_literal(THD *thd, const MYSQL_TIME *ltime, uint dec_arg):
    Item_basic_constant(thd)
  {
    collation.set(&my_charset_numeric, DERIVATION_NUMERIC, MY_REPERTOIRE_ASCII);
    decimals= dec_arg;
    cached_time= *ltime;
  }
  bool basic_const_item() const { return true; }
  bool const_item() const { return true; }
  enum Type type() const { return DATE_ITEM; }
  bool eq(const Item *item, bool binary_cmp) const;

  bool check_partition_func_processor(void *int_arg) {return FALSE;}

  bool is_null()
  { return is_null_from_temporal(); }
  bool get_date_with_sql_mode(MYSQL_TIME *to);
  String *val_str(String *str)
  { return val_string_from_date(str); }
  longlong val_int()
  { return val_int_from_date(); }
  double val_real()
  { return val_real_from_date(); }
  my_decimal *val_decimal(my_decimal *decimal_value)
  { return  val_decimal_from_date(decimal_value); }
  int save_in_field(Field *field, bool no_conversions)
  { return save_date_in_field(field, no_conversions); }
};



class Item_date_literal: public Item_temporal_literal
{
public:
  Item_date_literal(THD *thd, const MYSQL_TIME *ltime)
    :Item_temporal_literal(thd, ltime)
  {
    max_length= MAX_DATE_WIDTH;
    fixed= 1;
    
    maybe_null= !ltime->month || !ltime->day;
  }
  const Type_handler *type_handler() const { return &type_handler_newdate; }
  void print(String *str, enum_query_type query_type);
  Item *clone_item(THD *thd);
  bool get_date(MYSQL_TIME *res, ulonglong fuzzy_date);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_date_literal>(thd, this); }
};



class Item_time_literal: public Item_temporal_literal
{
public:
  Item_time_literal(THD *thd, const MYSQL_TIME *ltime, uint dec_arg):
    Item_temporal_literal(thd, ltime, dec_arg)
  {
    max_length= MIN_TIME_WIDTH + (decimals ? decimals + 1 : 0);
    fixed= 1;
  }
  const Type_handler *type_handler() const { return &type_handler_time2; }
  void print(String *str, enum_query_type query_type);
  Item *clone_item(THD *thd);
  bool get_date(MYSQL_TIME *res, ulonglong fuzzy_date);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_time_literal>(thd, this); }
};



class Item_datetime_literal: public Item_temporal_literal
{
public:
  Item_datetime_literal(THD *thd, const MYSQL_TIME *ltime, uint dec_arg):
    Item_temporal_literal(thd, ltime, dec_arg)
  {
    max_length= MAX_DATETIME_WIDTH + (decimals ? decimals + 1 : 0);
    fixed= 1;
    
    maybe_null= !ltime->month || !ltime->day;
  }
  const Type_handler *type_handler() const { return &type_handler_datetime2; }
  void print(String *str, enum_query_type query_type);
  Item *clone_item(THD *thd);
  bool get_date(MYSQL_TIME *res, ulonglong fuzzy_date);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_datetime_literal>(thd, this); }
};



class Item_date_literal_for_invalid_dates: public Item_date_literal
{
  
public:
  Item_date_literal_for_invalid_dates(THD *thd, const MYSQL_TIME *ltime)
   :Item_date_literal(thd, ltime) { }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzy_date)
  {
    *ltime= cached_time;
    return (null_value= false);
  }
};



class Item_datetime_literal_for_invalid_dates: public Item_datetime_literal
{
public:
  Item_datetime_literal_for_invalid_dates(THD *thd,
                                          const MYSQL_TIME *ltime, uint dec_arg)
   :Item_datetime_literal(thd, ltime, dec_arg) { }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzy_date)
  {
    *ltime= cached_time;
    return (null_value= false);
  }
};


class Used_tables_and_const_cache
{
public:
  
  table_map used_tables_cache;
  bool const_item_cache;

  Used_tables_and_const_cache()
   :used_tables_cache(0),
    const_item_cache(true)
  { }
  Used_tables_and_const_cache(const Used_tables_and_const_cache *other)
   :used_tables_cache(other->used_tables_cache),
    const_item_cache(other->const_item_cache)
  { }
  void used_tables_and_const_cache_init()
  {
    used_tables_cache= 0;
    const_item_cache= true;
  }
  void used_tables_and_const_cache_join(const Item *item)
  {
    used_tables_cache|= item->used_tables();
    const_item_cache&= item->const_item();
  }
  void used_tables_and_const_cache_update_and_join(Item *item)
  {
    item->update_used_tables();
    used_tables_and_const_cache_join(item);
  }
  
  void used_tables_and_const_cache_update_and_join(uint argc, Item **argv)
  {
    for (uint i=0 ; i < argc ; i++)
      used_tables_and_const_cache_update_and_join(argv[i]);
  }
  
  void used_tables_and_const_cache_update_and_join(List<Item> &list)
  {
    List_iterator_fast<Item> li(list);
    Item *item;
    while ((item=li++))
      used_tables_and_const_cache_update_and_join(item);
  }
};



class Item_func_or_sum: public Item_result_field,
                        public Item_args,
                        public Used_tables_and_const_cache,
                        public With_subquery_cache
{
protected:
  bool agg_arg_charsets(DTCollation &c, Item **items, uint nitems,
                        uint flags, int item_sep)
  {
    return Type_std_attributes::agg_arg_charsets(c, func_name(),
                                                 items, nitems,
                                                 flags, item_sep);
  }
  bool agg_arg_charsets_for_string_result(DTCollation &c,
                                          Item **items, uint nitems,
                                          int item_sep= 1)
  {
    return Type_std_attributes::
      agg_arg_charsets_for_string_result(c, func_name(),
                                         items, nitems, item_sep);
  }
  bool agg_arg_charsets_for_string_result_with_comparison(DTCollation &c,
                                                          Item **items,
                                                          uint nitems,
                                                          int item_sep= 1)
  {
    return Type_std_attributes::
      agg_arg_charsets_for_string_result_with_comparison(c, func_name(),
                                                         items, nitems,
                                                         item_sep);
  }

  
  bool agg_arg_charsets_for_comparison(DTCollation &c,
                                       Item **items, uint nitems,
                                       int item_sep= 1)
  {
    return Type_std_attributes::
      agg_arg_charsets_for_comparison(c, func_name(), items, nitems, item_sep);
  }

public:
  
  bool agg_arg_charsets_for_comparison(CHARSET_INFO **cs, Item **a, Item **b)
  {
    DTCollation tmp;
    if (tmp.set((*a)->collation, (*b)->collation, MY_COLL_CMP_CONV) ||
        tmp.derivation == DERIVATION_NONE)
    {
      my_error(ER_CANT_AGGREGATE_2COLLATIONS,MYF(0),
               (*a)->collation.collation->name,
               (*a)->collation.derivation_name(),
               (*b)->collation.collation->name,
               (*b)->collation.derivation_name(),
               func_name());
      return true;
    }
    if (agg_item_set_converter(tmp, func_name(),
                               a, 1, MY_COLL_CMP_CONV, 1) ||
        agg_item_set_converter(tmp, func_name(),
                               b, 1, MY_COLL_CMP_CONV, 1))
      return true;
    *cs= tmp.collation;
    return false;
  }

public:
  Item_func_or_sum(THD *thd): Item_result_field(thd), Item_args() {}
  Item_func_or_sum(THD *thd, Item *a): Item_result_field(thd), Item_args(a) { }
  Item_func_or_sum(THD *thd, Item *a, Item *b):
    Item_result_field(thd), Item_args(a, b) { }
  Item_func_or_sum(THD *thd, Item *a, Item *b, Item *c):
    Item_result_field(thd), Item_args(thd, a, b, c) { }
  Item_func_or_sum(THD *thd, Item *a, Item *b, Item *c, Item *d):
    Item_result_field(thd), Item_args(thd, a, b, c, d) { }
  Item_func_or_sum(THD *thd, Item *a, Item *b, Item *c, Item *d, Item *e):
    Item_result_field(thd), Item_args(thd, a, b, c, d, e) { }
  Item_func_or_sum(THD *thd, Item_func_or_sum *item):
    Item_result_field(thd, item), Item_args(thd, item),
    Used_tables_and_const_cache(item) { }
  Item_func_or_sum(THD *thd, List<Item> &list):
    Item_result_field(thd), Item_args(thd, list) { }
  bool with_subquery() const { DBUG_ASSERT(fixed); return m_with_subquery; }
  bool walk(Item_processor processor, bool walk_subquery, void *arg)
  {
    if (walk_args(processor, walk_subquery, arg))
      return true;
    return (this->*processor)(arg);
  }
  
  virtual const char *func_name() const= 0;
  virtual bool fix_length_and_dec()= 0;
  bool const_item() const { return const_item_cache; }
  table_map used_tables() const { return used_tables_cache; }
  Item* build_clone(THD *thd);
};

class sp_head;
class sp_name;
struct st_sp_security_context;

class Item_sp
{
public:
  Name_resolution_context *context;
  sp_name *m_name;
  sp_head *m_sp;
  TABLE *dummy_table;
  uchar result_buf[64];
  sp_rcontext *func_ctx;
  MEM_ROOT sp_mem_root;
  Query_arena *sp_query_arena;

  
  Field *sp_result_field;
  Item_sp(THD *thd, Name_resolution_context *context_arg, sp_name *name_arg);
  Item_sp(THD *thd, Item_sp *item);
  const char *func_name(THD *thd) const;
  void cleanup();
  bool sp_check_access(THD *thd);
  bool execute(THD *thd, bool *null_value, Item **args, uint arg_count);
  bool execute_impl(THD *thd, Item **args, uint arg_count);
  bool init_result_field(THD *thd, uint max_length, uint maybe_null,
                         bool *null_value, LEX_CSTRING *name);
};

class Item_ref :public Item_ident
{
protected:
  void set_properties();
  bool set_properties_only; 
public:
  enum Ref_Type { REF, DIRECT_REF, VIEW_REF, OUTER_REF, AGGREGATE_REF };
  Item **ref;
  bool reference_trough_name;
  Item_ref(THD *thd, Name_resolution_context *context_arg,
           const char *db_arg, const char *table_name_arg,
           const LEX_CSTRING *field_name_arg):
    Item_ident(thd, context_arg, db_arg, table_name_arg, field_name_arg),
    set_properties_only(0), ref(0), reference_trough_name(1) {}
  
  Item_ref(THD *thd, Name_resolution_context *context_arg, Item **item,
           const char *table_name_arg, const LEX_CSTRING *field_name_arg,
           bool alias_name_used_arg= FALSE);
  Item_ref(THD *thd, TABLE_LIST *view_arg, Item **item,
           const LEX_CSTRING *field_name_arg, bool alias_name_used_arg= FALSE);

  
  Item_ref(THD *thd, Item_ref *item)
    :Item_ident(thd, item), set_properties_only(0), ref(item->ref) {}
  enum Type type() const		{ return REF_ITEM; }
  enum Type real_type() const           { return ref ? (*ref)->type() :
                                          REF_ITEM; }
  bool eq(const Item *item, bool binary_cmp) const
  { 
    Item *it= ((Item *) item)->real_item();
    return ref && (*ref)->eq(it, binary_cmp);
  }
  void save_val(Field *to);
  void save_result(Field *to);
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *);
  bool val_bool();
  String *val_str(String* tmp);
  bool is_null();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  double val_result();
  longlong val_int_result();
  String *str_result(String* tmp);
  my_decimal *val_decimal_result(my_decimal *);
  bool val_bool_result();
  bool is_null_result();
  bool send(Protocol *prot, st_value *buffer);
  void make_send_field(THD *thd, Send_field *field);
  bool fix_fields(THD *, Item **);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  int save_in_field(Field *field, bool no_conversions);
  void save_org_in_field(Field *field, fast_field_copier optimizer_data);
  fast_field_copier setup_fast_field_copier(Field *field)
  { return (*ref)->setup_fast_field_copier(field); }
  const Type_handler *type_handler() const { return (*ref)->type_handler(); }
  const Type_handler *real_type_handler() const
  { return (*ref)->real_type_handler(); }
  Field *get_tmp_table_field()
  { return result_field ? result_field : (*ref)->get_tmp_table_field(); }
  Item *get_tmp_table_item(THD *thd);
  table_map used_tables() const;		
  void update_used_tables(); 
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref)
  {
    
    DBUG_ASSERT(real_type() != FIELD_ITEM);
    return Item_ident::build_equal_items(thd, inherited, link_item_fields,
                                         cond_equal_ref);
  }
  bool const_item() const 
  {
    return (*ref)->const_item();
  }
  table_map not_null_tables() const 
  { 
    return depended_from ? 0 : (*ref)->not_null_tables();
  }
  void save_in_result_field(bool no_conversions)
  {
    (*ref)->save_in_field(result_field, no_conversions);
  }
  Item *real_item()
  {
    return ref ? (*ref)->real_item() : this;
  }
  TYPELIB *get_typelib() const
  {
    return ref ? (*ref)->get_typelib() : NULL;
  }

  bool walk(Item_processor processor, bool walk_subquery, void *arg)
  { 
    if (ref && *ref)
      return (*ref)->walk(processor, walk_subquery, arg) ||
             (this->*processor)(arg); 
    else
      return FALSE;
  }
  Item* transform(THD *thd, Item_transformer, uchar *arg);
  Item* compile(THD *thd, Item_analyzer analyzer, uchar **arg_p,
                Item_transformer transformer, uchar *arg_t);
  bool enumerate_field_refs_processor(void *arg)
  { return (*ref)->enumerate_field_refs_processor(arg); }
  void no_rows_in_result()
  {
    (*ref)->no_rows_in_result();
  }
  void restore_to_before_no_rows_in_result()
  {
    (*ref)->restore_to_before_no_rows_in_result();
  }
  virtual void print(String *str, enum_query_type query_type);
  void cleanup();
  Item_field *field_for_view_update()
    { return (*ref)->field_for_view_update(); }
  Load_data_outvar *get_load_data_outvar()
  {
    return (*ref)->get_load_data_outvar();
  }
  virtual Ref_Type ref_type() { return REF; }

  
  uint cols() const
  {
    return ref && result_type() == ROW_RESULT ? (*ref)->cols() : 1;
  }
  Item* element_index(uint i)
  {
    return ref && result_type() == ROW_RESULT ? (*ref)->element_index(i) : this;
  }
  Item** addr(uint i)
  {
    return ref && result_type() == ROW_RESULT ? (*ref)->addr(i) : 0;
  }
  bool check_cols(uint c)
  {
    return ref && result_type() == ROW_RESULT ? (*ref)->check_cols(c) 
                                              : Item::check_cols(c);
  }
  bool null_inside()
  {
    return ref && result_type() == ROW_RESULT ? (*ref)->null_inside() : 0;
  }
  void bring_value()
  { 
    if (ref && result_type() == ROW_RESULT)
      (*ref)->bring_value();
  }
  bool check_vcol_func_processor(void *arg) 
  {
    return mark_unsupported_function("ref", arg, VCOL_IMPOSSIBLE);
  }
  bool basic_const_item() const { return ref && (*ref)->basic_const_item(); }
  bool is_outer_field() const
  {
    DBUG_ASSERT(fixed);
    DBUG_ASSERT(ref);
    return (*ref)->is_outer_field();
  }
  
  Item* build_clone(THD *thd);

  
  virtual bool with_subquery() const
  {
    return (*ref)->with_subquery();
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_ref>(thd, this); }
  bool excl_dep_on_table(table_map tab_map)
  { 
    table_map used= used_tables();
    if (used & OUTER_REF_TABLE_BIT)
      return false;
    return (used == tab_map) || (*ref)->excl_dep_on_table(tab_map);
  }
  bool excl_dep_on_grouping_fields(st_select_lex *sel)
  { return (*ref)->excl_dep_on_grouping_fields(sel); }
  bool cleanup_excluding_fields_processor(void *arg)
  {
    Item *item= real_item();
    if (item && item->type() == FIELD_ITEM &&
        ((Item_field *)item)->field)
      return 0;
    return cleanup_processor(arg);
  }
  bool cleanup_excluding_const_fields_processor(void *arg)
  { 
    Item *item= real_item();
    if (item && item->type() == FIELD_ITEM &&
        ((Item_field *) item)->field && item->const_item())
      return 0;
    return cleanup_processor(arg);
  }
};



class Item_direct_ref :public Item_ref
{
public:
  Item_direct_ref(THD *thd, Name_resolution_context *context_arg, Item **item,
                  const char *table_name_arg,
                  const LEX_CSTRING *field_name_arg,
                  bool alias_name_used_arg= FALSE):
    Item_ref(thd, context_arg, item, table_name_arg,
             field_name_arg, alias_name_used_arg)
  {}
  
  Item_direct_ref(THD *thd, Item_direct_ref *item) : Item_ref(thd, item) {}
  Item_direct_ref(THD *thd, TABLE_LIST *view_arg, Item **item,
                  const LEX_CSTRING *field_name_arg,
                  bool alias_name_used_arg= FALSE):
    Item_ref(thd, view_arg, item, field_name_arg,
             alias_name_used_arg)
  {}

  bool fix_fields(THD *thd, Item **it)
  {
    if ((*ref)->fix_fields_if_needed_for_scalar(thd, ref))
      return TRUE;
    return Item_ref::fix_fields(thd, it);
  }
  void save_val(Field *to);
  double val_real();
  longlong val_int();
  String *val_str(String* tmp);
  my_decimal *val_decimal(my_decimal *);
  bool val_bool();
  bool is_null();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  virtual Ref_Type ref_type() { return DIRECT_REF; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_direct_ref>(thd, this); }
};




class Item_direct_ref_to_ident :public Item_direct_ref
{
  Item_ident *ident;
public:
  Item_direct_ref_to_ident(THD *thd, Item_ident *item):
    Item_direct_ref(thd, item->context, (Item**)&item, item->table_name,
                    &item->field_name, FALSE)
  {
    ident= item;
    ref= (Item**)&ident;
  }

  bool fix_fields(THD *thd, Item **it)
  {
    DBUG_ASSERT(ident->type() == FIELD_ITEM || ident->type() == REF_ITEM);
    if (ident->fix_fields_if_needed_for_scalar(thd, ref))
      return TRUE;
    set_properties();
    return FALSE;
  }

  virtual void print(String *str, enum_query_type query_type)
  { ident->print(str, query_type); }

};


class Item_cache;
class Expression_cache;
class Expression_cache_tracker;



class Item_cache_wrapper :public Item_result_field,
                          public With_subquery_cache
{
private:
  
  Item *orig_item;
  Expression_cache *expr_cache;
  
  Item_cache *expr_value;

  List<Item> parameters;

  Item *check_cache();
  void cache();
  void init_on_demand();

public:
  Item_cache_wrapper(THD *thd, Item *item_arg);
  ~Item_cache_wrapper();

  enum Type type() const { return EXPR_CACHE_ITEM; }
  enum Type real_type() const { return orig_item->type(); }
  bool with_subquery() const { DBUG_ASSERT(fixed); return m_with_subquery; }

  bool set_cache(THD *thd);
  Expression_cache_tracker* init_tracker(MEM_ROOT *mem_root);

  bool fix_fields(THD *thd, Item **it);
  void cleanup();

  Item *get_orig_item() const { return orig_item; }

  
  void save_val(Field *to);
  double val_real();
  longlong val_int();
  String *val_str(String* tmp);
  my_decimal *val_decimal(my_decimal *);
  bool val_bool();
  bool is_null();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool send(Protocol *protocol, st_value *buffer);
  void save_org_in_field(Field *field,
                         fast_field_copier data __attribute__ ((__unused__)))
  {
    save_val(field);
  }
  void save_in_result_field(bool no_conversions)
  {
    save_val(result_field);
  }
  Item* get_tmp_table_item(THD *thd_arg);

  

  virtual void print(String *str, enum_query_type query_type);
  virtual const char *full_name() const { return orig_item->full_name(); }
  virtual void make_send_field(THD *thd, Send_field *field)
  { orig_item->make_send_field(thd, field); }
  bool eq(const Item *item, bool binary_cmp) const
  {
    Item *it= ((Item *) item)->real_item();
    return orig_item->eq(it, binary_cmp);
  }
  void fix_after_pullout(st_select_lex *new_parent, Item **refptr, bool merge)
  {
    orig_item->fix_after_pullout(new_parent, &orig_item, merge);
  }
  int save_in_field(Field *to, bool no_conversions);
  const Type_handler *type_handler() const { return orig_item->type_handler(); }
  table_map used_tables() const { return orig_item->used_tables(); }
  void update_used_tables()
  {
    orig_item->update_used_tables();
  }
  bool const_item() const { return orig_item->const_item(); }
  table_map not_null_tables() const { return orig_item->not_null_tables(); }
  bool walk(Item_processor processor, bool walk_subquery, void *arg)
  {
    return orig_item->walk(processor, walk_subquery, arg) ||
      (this->*processor)(arg);
  }
  bool enumerate_field_refs_processor(void *arg)
  { return orig_item->enumerate_field_refs_processor(arg); }
  Item_field *field_for_view_update()
  { return orig_item->field_for_view_update(); }

  
  uint cols() const
  { return result_type() == ROW_RESULT ? orig_item->cols() : 1; }
  Item* element_index(uint i)
  { return result_type() == ROW_RESULT ? orig_item->element_index(i) : this; }
  Item** addr(uint i)
  { return result_type() == ROW_RESULT ? orig_item->addr(i) : 0; }
  bool check_cols(uint c)
  {
    return (result_type() == ROW_RESULT ?
            orig_item->check_cols(c) :
            Item::check_cols(c));
  }
  bool null_inside()
  { return result_type() == ROW_RESULT ? orig_item->null_inside() : 0; }
  void bring_value()
  {
    if (result_type() == ROW_RESULT)
      orig_item->bring_value();
  }
  bool is_expensive() { return orig_item->is_expensive(); }
  bool is_expensive_processor(void *arg)
  { return orig_item->is_expensive_processor(arg); }
  bool check_vcol_func_processor(void *arg)
  {
    return mark_unsupported_function("cache", arg, VCOL_IMPOSSIBLE);
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_wrapper>(thd, this); }
  Item *build_clone(THD *thd) { return 0; }
};



class Item_direct_view_ref :public Item_direct_ref
{
  Item_equal *item_equal;
  TABLE_LIST *view;
  TABLE *null_ref_table;

#define NO_NULL_TABLE (reinterpret_cast<TABLE *>(0x1))

  void set_null_ref_table()
  {
    if (!view->is_inner_table_of_outer_join() ||
        !(null_ref_table= view->get_real_join_table()))
      null_ref_table= NO_NULL_TABLE;
  }

  bool check_null_ref()
  {
    DBUG_ASSERT(null_ref_table);
    if (null_ref_table != NO_NULL_TABLE && null_ref_table->null_row)
    {
      null_value= 1;
      return TRUE;
    }
    return FALSE;
  }

public:
  Item_direct_view_ref(THD *thd, Name_resolution_context *context_arg,
                       Item **item,
                       const char *table_name_arg,
                       LEX_CSTRING *field_name_arg,
                       TABLE_LIST *view_arg):
    Item_direct_ref(thd, context_arg, item, table_name_arg, field_name_arg),
    item_equal(0), view(view_arg),
    null_ref_table(NULL)
  {
    if (fixed)
      set_null_ref_table();
  }

  bool fix_fields(THD *, Item **);
  bool eq(const Item *item, bool binary_cmp) const;
  Item *get_tmp_table_item(THD *thd)
  {
    if (const_item())
      return copy_or_same(thd);
    Item *item= Item_ref::get_tmp_table_item(thd);
    item->name= name;
    return item;
  }
  virtual Ref_Type ref_type() { return VIEW_REF; }
  Item_equal *get_item_equal() { return item_equal; }
  void set_item_equal(Item_equal *item_eq) { item_equal= item_eq; }
  Item_equal *find_item_equal(COND_EQUAL *cond_equal);
  Item* propagate_equal_fields(THD *, const Context &, COND_EQUAL *);
  Item *replace_equal_field(THD *thd, uchar *arg);
  table_map used_tables() const;
  void update_used_tables();
  table_map not_null_tables() const;
  bool const_item() const { return used_tables() == 0; }
  bool walk(Item_processor processor, bool walk_subquery, void *arg)
  { 
    return (*ref)->walk(processor, walk_subquery, arg) ||
           (this->*processor)(arg);
  }
  bool view_used_tables_processor(void *arg) 
  {
    TABLE_LIST *view_arg= (TABLE_LIST *) arg;
    if (view_arg == view)
      view_arg->view_used_tables|= (*ref)->used_tables();
    return 0;
  }
  bool excl_dep_on_table(table_map tab_map);
  bool excl_dep_on_grouping_fields(st_select_lex *sel);
  Item *derived_field_transformer_for_having(THD *thd, uchar *arg);
  Item *derived_field_transformer_for_where(THD *thd, uchar *arg);
  Item *derived_grouping_field_transformer_for_where(THD *thd,
                                                     uchar *arg);

  void save_val(Field *to)
  {
    if (check_null_ref())
      to->set_null();
    else
      Item_direct_ref::save_val(to);
  }
  double val_real()
  {
    if (check_null_ref())
      return 0;
    else
      return Item_direct_ref::val_real();
  }
  longlong val_int()
  {
    if (check_null_ref())
      return 0;
    else
      return Item_direct_ref::val_int();
  }
  String *val_str(String* tmp)
  {
    if (check_null_ref())
      return NULL;
    else
      return Item_direct_ref::val_str(tmp);
  }
  my_decimal *val_decimal(my_decimal *tmp)
  {
    if (check_null_ref())
      return NULL;
    else
      return Item_direct_ref::val_decimal(tmp);
  }
  bool val_bool()
  {
    if (check_null_ref())
      return 0;
    else
      return Item_direct_ref::val_bool();
  }
  bool is_null()
  {
    if (check_null_ref())
      return 1;
    else
      return Item_direct_ref::is_null();
  }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    if (check_null_ref())
    {
      bzero((char*) ltime,sizeof(*ltime));
      return 1;
    }
    return Item_direct_ref::get_date(ltime, fuzzydate);
  }
  bool send(Protocol *protocol, st_value *buffer);
  void save_org_in_field(Field *field,
                         fast_field_copier data __attribute__ ((__unused__)))
  {
    if (check_null_ref())
      field->set_null();
    else
      Item_direct_ref::save_val(field);
  }
  void save_in_result_field(bool no_conversions)
  {
    if (check_null_ref())
      result_field->set_null();
    else
      Item_direct_ref::save_in_result_field(no_conversions);
  }

  void cleanup()
  {
    null_ref_table= NULL;
    item_equal= NULL;
    Item_direct_ref::cleanup();
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_direct_view_ref>(thd, this); }
};




class Item_sum;
class Item_outer_ref :public Item_direct_ref
{
public:
  Item *outer_ref;
  
  Item_sum *in_sum_func;
  
  bool found_in_select_list;
  bool found_in_group_by;
  Item_outer_ref(THD *thd, Name_resolution_context *context_arg,
                 Item_field *outer_field_arg):
    Item_direct_ref(thd, context_arg, 0, outer_field_arg->table_name,
                    &outer_field_arg->field_name),
    outer_ref(outer_field_arg), in_sum_func(0),
    found_in_select_list(0), found_in_group_by(0)
  {
    ref= &outer_ref;
    set_properties();
    fixed= 0;                     
  }
  Item_outer_ref(THD *thd, Name_resolution_context *context_arg, Item **item,
                 const char *table_name_arg, LEX_CSTRING *field_name_arg,
                 bool alias_name_used_arg):
    Item_direct_ref(thd, context_arg, item, table_name_arg, field_name_arg,
                    alias_name_used_arg),
    outer_ref(0), in_sum_func(0), found_in_select_list(1), found_in_group_by(0)
  {}
  void save_in_result_field(bool no_conversions)
  {
    outer_ref->save_org_in_field(result_field, NULL);
  }
  bool fix_fields(THD *, Item **);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  table_map used_tables() const
  {
    return (*ref)->const_item() ? 0 : OUTER_REF_TABLE_BIT;
  }
  table_map not_null_tables() const { return 0; }
  virtual Ref_Type ref_type() { return OUTER_REF; }
  bool check_inner_refs_processor(void * arg); 
};


class Item_in_subselect;




class Item_ref_null_helper: public Item_ref
{
protected:
  Item_in_subselect* owner;
public:
  Item_ref_null_helper(THD *thd, Name_resolution_context *context_arg,
                       Item_in_subselect* master, Item **item,
		       const char *table_name_arg,
                       const LEX_CSTRING *field_name_arg):
    Item_ref(thd, context_arg, item, table_name_arg, field_name_arg),
    owner(master) {}
  void save_val(Field *to);
  double val_real();
  longlong val_int();
  String* val_str(String* s);
  my_decimal *val_decimal(my_decimal *);
  bool val_bool();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  virtual void print(String *str, enum_query_type query_type);
  table_map used_tables() const;
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_ref_null_helper>(thd, this); }
};



class Item_int_with_ref :public Item_int
{
  Item *ref;
public:
  Item_int_with_ref(THD *thd, longlong i, Item *ref_arg, bool unsigned_arg):
    Item_int(thd, i), ref(ref_arg)
  {
    unsigned_flag= unsigned_arg;
  }
  int save_in_field(Field *field, bool no_conversions)
  {
    return ref->save_in_field(field, no_conversions);
  }
  Item *clone_item(THD *thd);
  virtual Item *real_item() { return ref; }
};

#ifdef MYSQL_SERVER
#include "gstream.h"
#include "spatial.h"
#include "item_sum.h"
#include "item_func.h"
#include "item_row.h"
#include "item_cmpfunc.h"
#include "item_strfunc.h"
#include "item_geofunc.h"
#include "item_timefunc.h"
#include "item_subselect.h"
#include "item_xmlfunc.h"
#include "item_jsonfunc.h"
#include "item_create.h"
#include "item_vers.h"
#endif



class Item_copy :public Item,
                 public Type_handler_hybrid_field_type
{
protected:  

  

  
  Item *item;

  
  Item_copy(THD *thd, Item *i): Item(thd)
  {
    item= i;
    null_value=maybe_null=item->maybe_null;
    Type_std_attributes::set(item);
    name= item->name;
    set_handler(item->type_handler());
    fixed= item->fixed;
  }

public:

    
  virtual void copy() = 0;

  Item *get_item() { return item; }
  
  enum Type type() const { return COPY_STR_ITEM; }

  const Type_handler *type_handler() const
  { return Type_handler_hybrid_field_type::type_handler(); }

  void make_send_field(THD *thd, Send_field *field)
  { item->make_send_field(thd, field); }
  table_map used_tables() const { return (table_map) 1L; }
  bool const_item() const { return 0; }
  bool is_null() { return null_value; }
  bool check_vcol_func_processor(void *arg) 
  {
    return mark_unsupported_function("copy", arg, VCOL_IMPOSSIBLE);
  }

    

  virtual String *val_str(String*) = 0;
  virtual my_decimal *val_decimal(my_decimal *) = 0;
  virtual double val_real() = 0;
  virtual longlong val_int() = 0;
  virtual int save_in_field(Field *field, bool no_conversions) = 0;
  bool walk(Item_processor processor, bool walk_subquery, void *args)
  {
    return (item->walk(processor, walk_subquery, args)) ||
      (this->*processor)(args);
  }
};

 
class Item_copy_string : public Item_copy
{
public:
  Item_copy_string(THD *thd, Item *item_arg): Item_copy(thd, item_arg) {}

  String *val_str(String*);
  my_decimal *val_decimal(my_decimal *);
  double val_real();
  longlong val_int();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_string(ltime, fuzzydate); }
  void copy();
  int save_in_field(Field *field, bool no_conversions);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_copy_string>(thd, this); }
};




class Cached_item :public Sql_alloc
{
public:
  bool null_value;
  Cached_item() :null_value(0) {}
  
  virtual bool cmp(void)=0;

  
  virtual int  cmp_read_only()=0;

  virtual ~Cached_item(); 
};

class Cached_item_item : public Cached_item
{
protected:
  Item *item;

  Cached_item_item(Item *arg) : item(arg) {}
public:
  void fetch_value_from(Item *new_item)
  {
    Item *save= item;
    item= new_item;
    cmp();
    item= save;
  }
};

class Cached_item_str :public Cached_item_item
{
  uint32 value_max_length;
  String value,tmp_value;
public:
  Cached_item_str(THD *thd, Item *arg);
  bool cmp(void);
  int  cmp_read_only();
  ~Cached_item_str();                           
};


class Cached_item_real :public Cached_item_item
{
  double value;
public:
  Cached_item_real(Item *item_par) :Cached_item_item(item_par),value(0.0) {}
  bool cmp(void);
  int  cmp_read_only();
};

class Cached_item_int :public Cached_item_item
{
  longlong value;
public:
  Cached_item_int(Item *item_par) :Cached_item_item(item_par),value(0) {}
  bool cmp(void);
  int  cmp_read_only();
};


class Cached_item_decimal :public Cached_item_item
{
  my_decimal value;
public:
  Cached_item_decimal(Item *item_par);
  bool cmp(void);
  int  cmp_read_only();
};

class Cached_item_field :public Cached_item
{
  uchar *buff;
  Field *field;
  uint length;

public:
  Cached_item_field(THD *thd, Field *arg_field): field(arg_field)
  {
    field= arg_field;
    
    buff= (uchar*) thd_calloc(thd, length= field->pack_length());
  }
  bool cmp(void);
  int  cmp_read_only();
};

class Item_default_value : public Item_field
{
  void calculate();
public:
  Item *arg;
  Item_default_value(THD *thd, Name_resolution_context *context_arg)
    :Item_field(thd, context_arg, (const char *)NULL, (const char *)NULL,
                &null_clex_str),
     arg(NULL) {}
  Item_default_value(THD *thd, Name_resolution_context *context_arg, Item *a)
    :Item_field(thd, context_arg, (const char *)NULL, (const char *)NULL,
                &null_clex_str),
     arg(a) {}
  Item_default_value(THD *thd, Name_resolution_context *context_arg, Field *a)
    :Item_field(thd, context_arg, (const char *)NULL, (const char *)NULL,
                &null_clex_str),
     arg(NULL) {}
  enum Type type() const { return DEFAULT_VALUE_ITEM; }
  bool eq(const Item *item, bool binary_cmp) const;
  bool fix_fields(THD *, Item **);
  void print(String *str, enum_query_type query_type);
  String *val_str(String *str);
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *decimal_value);
  bool get_date(MYSQL_TIME *ltime,ulonglong fuzzydate);
  bool send(Protocol *protocol, st_value *buffer);
  int save_in_field(Field *field_arg, bool no_conversions);
  bool save_in_param(THD *thd, Item_param *param)
  {
    
    DBUG_ASSERT(arg == NULL);
    param->set_default();
    return false;
  }
  table_map used_tables() const;
  virtual void update_used_tables()
  {
    if (field && field->default_value)
      field->default_value->expr->update_used_tables();
  }
  Field *get_tmp_table_field() { return 0; }
  Item *get_tmp_table_item(THD *thd) { return this; }
  Item_field *field_for_view_update() { return 0; }
  bool update_vcol_processor(void *arg) { return 0; }
  bool check_func_default_processor(void *arg) { return true; }

  bool walk(Item_processor processor, bool walk_subquery, void *args)
  {
    return (arg && arg->walk(processor, walk_subquery, args)) ||
      (this->*processor)(args);
  }

  Item *transform(THD *thd, Item_transformer transformer, uchar *args);
};



class Item_ignore_value : public Item_default_value
{
public:
  Item_ignore_value(THD *thd, Name_resolution_context *context_arg)
    :Item_default_value(thd, context_arg)
  {};

  void print(String *str, enum_query_type query_type);
  int save_in_field(Field *field_arg, bool no_conversions);
  bool save_in_param(THD *thd, Item_param *param)
  {
    param->set_ignore();
    return false;
  }

  String *val_str(String *str);
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *decimal_value);
  bool get_date(MYSQL_TIME *ltime,ulonglong fuzzydate);
  bool send(Protocol *protocol, st_value *buffer);
};




class Item_insert_value : public Item_field
{
public:
  Item *arg;
  Item_insert_value(THD *thd, Name_resolution_context *context_arg, Item *a)
    :Item_field(thd, context_arg, (const char *)NULL, (const char *)NULL,
                &null_clex_str),
     arg(a) {}
  bool eq(const Item *item, bool binary_cmp) const;
  bool fix_fields(THD *, Item **);
  virtual void print(String *str, enum_query_type query_type);
  int save_in_field(Field *field_arg, bool no_conversions)
  {
    return Item_field::save_in_field(field_arg, no_conversions);
  }
  enum Type type() const { return INSERT_VALUE_ITEM; }
  
  table_map used_tables() const { return RAND_TABLE_BIT; }

  Item_field *field_for_view_update() { return 0; }

  bool walk(Item_processor processor, bool walk_subquery, void *args)
  {
    return arg->walk(processor, walk_subquery, args) ||
	    (this->*processor)(args);
  }
  bool check_partition_func_processor(void *int_arg) {return TRUE;}
  bool update_vcol_processor(void *arg) { return 0; }
  bool check_vcol_func_processor(void *arg)
  {
    return mark_unsupported_function("value()", arg, VCOL_IMPOSSIBLE);
  }
};


class Table_triggers_list;


class Item_trigger_field : public Item_field,
                           private Settable_routine_parameter
{
public:
  
  enum row_version_type {OLD_ROW, NEW_ROW};
  row_version_type row_version;
  
  Item_trigger_field *next_trg_field;
  
  uint field_idx;
  
  Table_triggers_list *triggers;

  Item_trigger_field(THD *thd, Name_resolution_context *context_arg,
                     row_version_type row_ver_arg,
                     const LEX_CSTRING *field_name_arg,
                     ulong priv, const bool ro)
    :Item_field(thd, context_arg,
               (const char *)NULL, (const char *)NULL, field_name_arg),
     row_version(row_ver_arg), field_idx((uint)-1), original_privilege(priv),
     want_privilege(priv), table_grants(NULL), read_only (ro)
  {}
  void setup_field(THD *thd, TABLE *table, GRANT_INFO *table_grant_info);
  enum Type type() const { return TRIGGER_FIELD_ITEM; }
  bool eq(const Item *item, bool binary_cmp) const;
  bool fix_fields(THD *, Item **);
  virtual void print(String *str, enum_query_type query_type);
  table_map used_tables() const { return (table_map)0L; }
  Field *get_tmp_table_field() { return 0; }
  Item *copy_or_same(THD *thd) { return this; }
  Item *get_tmp_table_item(THD *thd) { return copy_or_same(thd); }
  void cleanup();

private:
  void set_required_privilege(bool rw);
  bool set_value(THD *thd, sp_rcontext *ctx, Item **it);

public:
  Settable_routine_parameter *get_settable_routine_parameter()
  {
    return (read_only ? 0 : this);
  }

  bool set_value(THD *thd, Item **it)
  {
    return set_value(thd, NULL, it);
  }

private:
  
  ulong original_privilege;
  ulong want_privilege;
  GRANT_INFO *table_grants;
  
  bool read_only;
public:
  bool check_vcol_func_processor(void *arg);
};




class Item_cache: public Item_basic_constant,
                  public Type_handler_hybrid_field_type
{
protected:
  Item *example;
    
  Field *cached_field;
  
  bool value_cached;
public:
  Item_cache(THD *thd):
    Item_basic_constant(thd),
    Type_handler_hybrid_field_type(&type_handler_string),
    example(0), cached_field(0),
    value_cached(0)
  {
    fixed= 1;
    maybe_null= 1;
    null_value= 1;
  }
protected:
  Item_cache(THD *thd, const Type_handler *handler):
    Item_basic_constant(thd),
    Type_handler_hybrid_field_type(handler),
    example(0), cached_field(0),
    value_cached(0)
  {
    fixed= 1;
    maybe_null= 1;
    null_value= 1;
  }

public:
  virtual bool allocate(THD *thd, uint i) { return 0; }
  virtual bool setup(THD *thd, Item *item)
  {
    example= item;
    Type_std_attributes::set(item);
    if (item->type() == FIELD_ITEM)
      cached_field= ((Item_field *)item)->field;
    return 0;
  };
  enum Type type() const { return CACHE_ITEM; }

  const Type_handler *type_handler() const
  { return Type_handler_hybrid_field_type::type_handler(); }

  virtual void keep_array() {}
  virtual void print(String *str, enum_query_type query_type);
  bool eq_def(const Field *field) 
  { 
    return cached_field ? cached_field->eq_def (field) : FALSE;
  }
  bool eq(const Item *item, bool binary_cmp) const
  {
    return this == item;
  }
  bool check_vcol_func_processor(void *arg) 
  {
    if (example)
    {
      Item::vcol_func_processor_result *res= (Item::vcol_func_processor_result*)arg;
      example->check_vcol_func_processor(arg);
      
      if (res->errors & VCOL_NOT_STRICTLY_DETERMINISTIC)
        res->errors|= VCOL_SESSION_FUNC;
      return false;
    }
    return mark_unsupported_function("cache", arg, VCOL_IMPOSSIBLE);
  }
  void cleanup()
  {
    clear();
    Item_basic_constant::cleanup();
  }
  
  bool has_value()
  {
    return (value_cached || cache_value()) && !null_value;
  }

  virtual void store(Item *item);
  virtual Item *get_item() { return example; }
  virtual bool cache_value()= 0;
  bool basic_const_item() const
  { return example && example->basic_const_item(); }
  virtual void clear() { null_value= TRUE; value_cached= FALSE; }
  bool is_null() { return !has_value(); }
  virtual bool is_expensive()
  {
    if (value_cached)
      return false;
    return example->is_expensive();
  }
  bool is_expensive_processor(void *arg)
  {
    DBUG_ASSERT(example);
    if (value_cached)
      return false;
    return example->is_expensive_processor(arg);
  }
  virtual void set_null();
  bool walk(Item_processor processor, bool walk_subquery, void *arg)
  {
    if (example && example->walk(processor, walk_subquery, arg))
      return TRUE;
    return (this->*processor)(arg);
  }
  virtual Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs);
  void split_sum_func2_example(THD *thd,  Ref_ptr_array ref_pointer_array,
                               List<Item> &fields, uint flags)
  {
    example->split_sum_func2(thd, ref_pointer_array, fields, &example, flags);
  }
  Item *get_example() const { return example; }

  virtual Item *convert_to_basic_const_item(THD *thd) { return 0; };
  Item *derived_field_transformer_for_having(THD *thd, uchar *arg)
  { return convert_to_basic_const_item(thd); }
  Item *derived_field_transformer_for_where(THD *thd, uchar *arg)
  { return convert_to_basic_const_item(thd); }
  Item *derived_grouping_field_transformer_for_where(THD *thd, uchar *arg)
  { return convert_to_basic_const_item(thd); }
};


class Item_cache_int: public Item_cache
{
protected:
  longlong value;
public:
  Item_cache_int(THD *thd): Item_cache(thd, &type_handler_longlong),
    value(0) {}
  Item_cache_int(THD *thd, const Type_handler *handler):
    Item_cache(thd, handler), value(0) {}

  double val_real();
  longlong val_int();
  String* val_str(String *str);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_int(ltime, fuzzydate); }
  bool cache_value();
  int save_in_field(Field *field, bool no_conversions);
  Item *convert_to_basic_const_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_int>(thd, this); }
};


class Item_cache_year: public Item_cache_int
{
public:
  Item_cache_year(THD *thd): Item_cache_int(thd, &type_handler_year) { }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_year(ltime, fuzzydate); }
};


class Item_cache_temporal: public Item_cache_int
{
protected:
  Item_cache_temporal(THD *thd, const Type_handler *handler);
public:
  String* val_str(String *str);
  my_decimal *val_decimal(my_decimal *);
  longlong val_int();
  longlong val_datetime_packed();
  longlong val_time_packed();
  double val_real();
  bool cache_value();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  int save_in_field(Field *field, bool no_conversions);
  void store_packed(longlong val_arg, Item *example);
  
  Item *clone_item(THD *thd);
  Item *convert_to_basic_const_item(THD *thd);
  virtual Item *make_literal(THD *) =0;
};


class Item_cache_time: public Item_cache_temporal
{
public:
  Item_cache_time(THD *thd)
   :Item_cache_temporal(thd, &type_handler_time2) { }
  bool cache_value();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_time>(thd, this); }
  Item *make_literal(THD *);
};


class Item_cache_datetime: public Item_cache_temporal
{
public:
  Item_cache_datetime(THD *thd)
   :Item_cache_temporal(thd, &type_handler_datetime2) { }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_datetime>(thd, this); }
  Item *make_literal(THD *);
};


class Item_cache_date: public Item_cache_temporal
{
public:
  Item_cache_date(THD *thd)
   :Item_cache_temporal(thd, &type_handler_newdate) { }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_date>(thd, this); }
  Item *make_literal(THD *);
};


class Item_cache_real: public Item_cache
{
  double value;
public:
  Item_cache_real(THD *thd): Item_cache(thd, &type_handler_double),
    value(0) {}

  double val_real();
  longlong val_int();
  String* val_str(String *str);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_real(ltime, fuzzydate); }
  bool cache_value();
  Item *convert_to_basic_const_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_real>(thd, this); }
};


class Item_cache_decimal: public Item_cache
{
protected:
  my_decimal decimal_value;
public:
  Item_cache_decimal(THD *thd): Item_cache(thd, &type_handler_newdecimal) {}

  double val_real();
  longlong val_int();
  String* val_str(String *str);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_decimal(ltime, fuzzydate); }
  bool cache_value();
  Item *convert_to_basic_const_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_decimal>(thd, this); }
};


class Item_cache_str: public Item_cache
{
  char buffer[STRING_BUFFER_USUAL_SIZE];
  String *value, value_buff;
  bool is_varbinary;
  
public:
  Item_cache_str(THD *thd, const Item *item):
    Item_cache(thd, item->type_handler()), value(0),
    is_varbinary(item->type() == FIELD_ITEM &&
                 Item_cache_str::field_type() == MYSQL_TYPE_VARCHAR &&
                 !((const Item_field *) item)->field->has_charset())
  {
    collation.set(const_cast<DTCollation&>(item->collation));
  }
  double val_real();
  longlong val_int();
  String* val_str(String *);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_string(ltime, fuzzydate); }
  CHARSET_INFO *charset() const { return value->charset(); };
  int save_in_field(Field *field, bool no_conversions);
  bool cache_value();
  Item *convert_to_basic_const_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_str>(thd, this); }
};


class Item_cache_str_for_nullif: public Item_cache_str
{
public:
  Item_cache_str_for_nullif(THD *thd, const Item *item)
   :Item_cache_str(thd, item)
  { }
  Item *safe_charset_converter(THD *thd, CHARSET_INFO *tocs)
  {
    
    return Item::safe_charset_converter(thd, tocs);
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_str_for_nullif>(thd, this); }
};


class Item_cache_row: public Item_cache
{
  Item_cache  **values;
  uint item_count;
  bool save_array;
public:
  Item_cache_row(THD *thd):
    Item_cache(thd), values(0), item_count(2),
    save_array(0) {}
  
  
  bool allocate(THD *thd, uint num);
  
  bool setup(THD *thd, Item *item);
  void store(Item *item);
  void illegal_method_call(const char *);
  void make_send_field(THD *thd, Send_field *)
  {
    illegal_method_call((const char*)"make_send_field");
  };
  double val_real()
  {
    illegal_method_call((const char*)"val");
    return 0;
  };
  longlong val_int()
  {
    illegal_method_call((const char*)"val_int");
    return 0;
  };
  String *val_str(String *)
  {
    illegal_method_call((const char*)"val_str");
    return 0;
  };
  my_decimal *val_decimal(my_decimal *val)
  {
    illegal_method_call((const char*)"val_decimal");
    return 0;
  };
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    illegal_method_call((const char*)"val_decimal");
    return true;
  }

  uint cols() const { return item_count; }
  Item *element_index(uint i) { return values[i]; }
  Item **addr(uint i) { return (Item **) (values + i); }
  bool check_cols(uint c);
  bool null_inside();
  void bring_value();
  void keep_array() { save_array= 1; }
  void cleanup()
  {
    DBUG_ENTER("Item_cache_row::cleanup");
    Item_cache::cleanup();
    if (save_array)
      bzero(values, item_count*sizeof(Item**));
    else
      values= 0;
    DBUG_VOID_RETURN;
  }
  bool cache_value();
  virtual void set_null();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cache_row>(thd, this); }
};



class Item_type_holder: public Item,
                        public Type_handler_hybrid_field_type,
                        public Type_geometry_attributes
{
protected:
  TYPELIB *enum_set_typelib;
public:
  Item_type_holder(THD *thd, Item *item)
   :Item(thd, item),
    Type_handler_hybrid_field_type(item->real_type_handler()),
    enum_set_typelib(0)
  {
    DBUG_ASSERT(item->fixed);
    maybe_null= item->maybe_null;
  }
  Item_type_holder(THD *thd,
                   Item *item,
                   const Type_handler *handler,
                   const Type_all_attributes *attr,
                   bool maybe_null_arg)
   :Item(thd),
    Type_handler_hybrid_field_type(handler),
    Type_geometry_attributes(handler, attr),
    enum_set_typelib(attr->get_typelib())
  {
    name= item->name;
    Type_std_attributes::set(*attr);
    maybe_null= maybe_null_arg;
  }

  const Type_handler *type_handler() const
  {
    return Type_handler_hybrid_field_type::type_handler()->
             type_handler_for_item_field();
  }
  const Type_handler *real_type_handler() const
  {
    return Type_handler_hybrid_field_type::type_handler();
  }

  enum Type type() const { return TYPE_HOLDER; }
  TYPELIB *get_typelib() const { return enum_set_typelib; }
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *);
  String *val_str(String*);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  Field *create_tmp_field(bool group, TABLE *table)
  {
    return Item_type_holder::real_type_handler()->
           make_and_init_table_field(&name, Record_addr(maybe_null),
                                     *this, table);
  }
  Field::geometry_type get_geometry_type() const
  {
    return Type_geometry_attributes::get_geometry_type();
  }
  void set_geometry_type(uint type)
  {
    Type_geometry_attributes::set_geometry_type(type);
  }
  Item* get_copy(THD *thd) { return 0; }

};


class st_select_lex;
void mark_select_range_as_dependent(THD *thd,
                                    st_select_lex *last_select,
                                    st_select_lex *current_sel,
                                    Field *found_field, Item *found_item,
                                    Item_ident *resolved_item);

extern Cached_item *new_Cached_item(THD *thd, Item *item,
                                    bool pass_through_ref);
extern Item_result item_cmp_type(Item_result a,Item_result b);
extern void resolve_const_item(THD *thd, Item **ref, Item *cmp_item);
extern int stored_field_cmp_to_item(THD *thd, Field *field, Item *item);

extern const String my_null_string;



class Item_iterator
{
public:
  
  virtual void open()= 0;
  
  virtual Item *next()= 0;
  
  virtual void close()= 0;

  virtual ~Item_iterator() {}
};




class Item_iterator_ref_list: public Item_iterator
{
  List_iterator<Item*> list;
public:
  Item_iterator_ref_list(List_iterator<Item*> &arg_list):
    list(arg_list) {}
  void open() { list.rewind(); }
  Item *next() { return *(list++); }
  void close() {}
};




class Item_iterator_list: public Item_iterator
{
  List_iterator<Item> list;
public:
  Item_iterator_list(List_iterator<Item> &arg_list):
    list(arg_list) {}
  void open() { list.rewind(); }
  Item *next() { return (list++); }
  void close() {}
};




class Item_iterator_row: public Item_iterator
{
  Item *base_item;
  uint current;
public:
  Item_iterator_row(Item *base) : base_item(base), current(0) {}
  void open() { current= 0; }
  Item *next()
  {
    if (current >= base_item->cols())
      return NULL;
    return base_item->element_index(current++);
  }
  void close() {}
};



bool fix_escape_item(THD *thd, Item *escape_item, String *tmp_str,
                     bool escape_used_in_parsing, CHARSET_INFO *cmp_cs,
                     int *escape);

inline bool Virtual_column_info::is_equal(const Virtual_column_info* vcol) const
{
  return field_type == vcol->get_real_type()
      && stored_in_db == vcol->is_stored()
      && expr->eq(vcol->expr, true);
}

inline void Virtual_column_info::print(String* str)
{
  expr->print_for_table_def(str);
}

#endif 
