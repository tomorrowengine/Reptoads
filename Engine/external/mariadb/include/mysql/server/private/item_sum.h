#ifndef ITEM_SUM_INCLUDED
#define ITEM_SUM_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include <my_tree.h>
#include "sql_udf.h"                            

class Item_sum;
class Aggregator_distinct;
class Aggregator_simple;



class Aggregator : public Sql_alloc
{
  friend class Item_sum;
  friend class Item_sum_sum;
  friend class Item_sum_count;
  friend class Item_sum_avg;

  
protected:
  
  Item_sum *item_sum;

public:
  Aggregator (Item_sum *arg): item_sum(arg) {}
  virtual ~Aggregator () {}                   

  enum Aggregator_type { SIMPLE_AGGREGATOR, DISTINCT_AGGREGATOR };
  virtual Aggregator_type Aggrtype() = 0;

  
  virtual bool setup(THD *) = 0;

  
  virtual void clear() = 0;

  
  virtual bool add() = 0;

  
  virtual void endup() = 0;

  
  virtual my_decimal *arg_val_decimal(my_decimal * value) = 0;
  
  virtual double arg_val_real() = 0;
  
  virtual bool arg_is_null(bool use_null_value) = 0;
};


class st_select_lex;
class Window_spec;



class Item_sum :public Item_func_or_sum
{
  friend class Aggregator_distinct;
  friend class Aggregator_simple;

protected:
  
  Aggregator *aggr;

private:
  
  bool force_copy_fields;

  
  bool with_distinct;

  
  bool window_func_sum_expr_flag;

public:

  bool has_force_copy_fields() const { return force_copy_fields; }
  bool has_with_distinct()     const { return with_distinct; }

  enum Sumfunctype
  { COUNT_FUNC, COUNT_DISTINCT_FUNC, SUM_FUNC, SUM_DISTINCT_FUNC, AVG_FUNC,
    AVG_DISTINCT_FUNC, MIN_FUNC, MAX_FUNC, STD_FUNC,
    VARIANCE_FUNC, SUM_BIT_FUNC, UDF_SUM_FUNC, GROUP_CONCAT_FUNC,
    ROW_NUMBER_FUNC, RANK_FUNC, DENSE_RANK_FUNC, PERCENT_RANK_FUNC,
    CUME_DIST_FUNC, NTILE_FUNC, FIRST_VALUE_FUNC, LAST_VALUE_FUNC,
    NTH_VALUE_FUNC, LEAD_FUNC, LAG_FUNC, PERCENTILE_CONT_FUNC,
    PERCENTILE_DISC_FUNC, SP_AGGREGATE_FUNC
  };

  Item **ref_by; 
  Item_sum *next; 
  Item_sum *in_sum_func;   
  st_select_lex * aggr_sel;  
  int8 nest_level;        
  int8 aggr_level;        
  int8 max_arg_level;     
  int8 max_sum_func_level;
  bool quick_group;			
  
  List<Item_field> outer_fields;

protected:  
  
  Item **orig_args, *tmp_orig_args[2];
  
  static size_t ram_limitation(THD *thd);

public:  

  void mark_as_sum_func();
  Item_sum(THD *thd): Item_func_or_sum(thd), quick_group(1)
  {
    mark_as_sum_func();
    init_aggregator();
  }
  Item_sum(THD *thd, Item *a): Item_func_or_sum(thd, a), quick_group(1),
    orig_args(tmp_orig_args)
  {
    mark_as_sum_func();
    init_aggregator();
  }
  Item_sum(THD *thd, Item *a, Item *b): Item_func_or_sum(thd, a, b),
    quick_group(1), orig_args(tmp_orig_args)
  {
    mark_as_sum_func();
    init_aggregator();
  }
  Item_sum(THD *thd, List<Item> &list);
  
  Item_sum(THD *thd, Item_sum *item);
  enum Type type() const { return SUM_FUNC_ITEM; }
  virtual enum Sumfunctype sum_func () const=0;
  bool is_aggr_sum_func()
  {
    switch (sum_func()) {
    case COUNT_FUNC:
    case COUNT_DISTINCT_FUNC:
    case SUM_FUNC:
    case SUM_DISTINCT_FUNC:
    case AVG_FUNC:
    case AVG_DISTINCT_FUNC:
    case MIN_FUNC:
    case MAX_FUNC:
    case STD_FUNC:
    case VARIANCE_FUNC:
    case SUM_BIT_FUNC:
    case UDF_SUM_FUNC:
    case GROUP_CONCAT_FUNC:
      return true;
    default:
      return false;
    }
  }
  
  inline bool reset_and_add() 
  { 
    aggregator_clear();
    return aggregator_add();
  };

  
  virtual void reset_field()=0;
  
  virtual void update_field()=0;
  virtual bool fix_length_and_dec()
  { maybe_null=1; null_value=1; return FALSE; }
  virtual Item *result_item(THD *thd, Field *field);

  void update_used_tables ();
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref)
  {
    
    DBUG_ASSERT(0);
    return Item::build_equal_items(thd, inherited, link_item_fields,
                                   cond_equal_ref);
  }
  bool is_null() { return null_value; }
  
  void make_const () 
  { 
    used_tables_cache= 0;
    const_item_cache= true;
  }
  void reset_forced_const() { const_item_cache= false; }
  virtual bool const_during_execution() const { return false; }
  virtual void print(String *str, enum_query_type query_type);
  void fix_num_length_and_dec();

  
  virtual void no_rows_in_result()
  {
    set_aggregator(with_distinct ?
                   Aggregator::DISTINCT_AGGREGATOR :
                   Aggregator::SIMPLE_AGGREGATOR);
    aggregator_clear();
  }
  virtual void make_unique() { force_copy_fields= TRUE; }
  Item *get_tmp_table_item(THD *thd);
  Field *create_tmp_field(bool group, TABLE *table);
  virtual bool collect_outer_ref_processor(void *param);
  bool init_sum_func_check(THD *thd);
  bool check_sum_func(THD *thd, Item **ref);
  bool register_sum_func(THD *thd, Item **ref);
  st_select_lex *depended_from() 
    { return (nest_level == aggr_level ? 0 : aggr_sel); }

  Item *get_arg(uint i) const { return args[i]; }
  Item *set_arg(uint i, THD *thd, Item *new_val);
  uint get_arg_count() const { return arg_count; }
  virtual Item **get_args() { return fixed ? orig_args : args; }

  
  void init_aggregator()
  {
    aggr= NULL;
    with_distinct= FALSE;
    force_copy_fields= FALSE;
  }

  

  inline bool aggregator_setup(THD *thd) { return aggr->setup(thd); };

  

  inline void aggregator_clear() { aggr->clear(); }

  

  inline bool aggregator_add() { return aggr->add(); };

  
  void set_distinct(bool distinct)
  {
    with_distinct= distinct;
    quick_group= with_distinct ? 0 : 1;
  }

  

  int set_aggregator(Aggregator::Aggregator_type aggregator);

  virtual void clear()= 0;
  virtual bool add()= 0;
  virtual bool setup(THD *thd) { return false; }

  virtual bool supports_removal() const { return false; }
  virtual void remove() { DBUG_ASSERT(0); }

  virtual void cleanup();
  bool check_vcol_func_processor(void *arg);
  virtual void setup_window_func(THD *thd, Window_spec *window_spec) {}
  void mark_as_window_func_sum_expr() { window_func_sum_expr_flag= true; }
  bool is_window_func_sum_expr() { return window_func_sum_expr_flag; }
  virtual void setup_caches(THD *thd) {};
};


class Unique;




class Aggregator_distinct : public Aggregator
{
  friend class Item_sum_sum;

  
  bool endup_done;

  
  TABLE *table;
  
  
  uint32 *field_lengths;

  
  TMP_TABLE_PARAM *tmp_table_param;
  
  
  Unique *tree;

    
  uint tree_key_length;

  
  bool always_null;

  
  bool use_distinct_values;

public:
  Aggregator_distinct (Item_sum *sum) :
    Aggregator(sum), table(NULL), tmp_table_param(NULL), tree(NULL),
    always_null(false), use_distinct_values(false) {}
  virtual ~Aggregator_distinct ();
  Aggregator_type Aggrtype() { return DISTINCT_AGGREGATOR; }

  bool setup(THD *);
  void clear(); 
  bool add();
  void endup();
  virtual my_decimal *arg_val_decimal(my_decimal * value);
  virtual double arg_val_real();
  virtual bool arg_is_null(bool use_null_value);

  bool unique_walk_function(void *element);
  bool unique_walk_function_for_count(void *element);
  static int composite_key_cmp(void* arg, uchar* key1, uchar* key2);
};



class Aggregator_simple : public Aggregator
{
public:

  Aggregator_simple (Item_sum *sum) :
    Aggregator(sum) {}
  Aggregator_type Aggrtype() { return Aggregator::SIMPLE_AGGREGATOR; }

  bool setup(THD * thd) { return item_sum->setup(thd); }
  void clear() { item_sum->clear(); }
  bool add() { return item_sum->add(); }
  void endup() {};
  virtual my_decimal *arg_val_decimal(my_decimal * value);
  virtual double arg_val_real();
  virtual bool arg_is_null(bool use_null_value);
};


class Item_sum_num :public Item_sum
{
protected:
  
  bool is_evaluated;
public:
  Item_sum_num(THD *thd): Item_sum(thd), is_evaluated(FALSE) {}
  Item_sum_num(THD *thd, Item *item_par):
    Item_sum(thd, item_par), is_evaluated(FALSE) {}
  Item_sum_num(THD *thd, Item *a, Item* b):
    Item_sum(thd, a, b), is_evaluated(FALSE) {}
  Item_sum_num(THD *thd, List<Item> &list):
    Item_sum(thd, list), is_evaluated(FALSE) {}
  Item_sum_num(THD *thd, Item_sum_num *item):
    Item_sum(thd, item),is_evaluated(item->is_evaluated) {}
  bool fix_fields(THD *, Item **);
  longlong val_int() { return val_int_from_real();   }
  String *val_str(String*str);
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return type_handler()->Item_get_date(this, ltime, fuzzydate);
  }
  void reset_field();
};


class Item_sum_int :public Item_sum_num
{
public:
  Item_sum_int(THD *thd): Item_sum_num(thd) {}
  Item_sum_int(THD *thd, Item *item_par): Item_sum_num(thd, item_par) {}
  Item_sum_int(THD *thd, List<Item> &list): Item_sum_num(thd, list) {}
  Item_sum_int(THD *thd, Item_sum_int *item) :Item_sum_num(thd, item) {}
  double val_real() { DBUG_ASSERT(fixed == 1); return (double) val_int(); }
  String *val_str(String*str);
  my_decimal *val_decimal(my_decimal *);
  const Type_handler *type_handler() const { return &type_handler_longlong; }
  bool fix_length_and_dec()
  { decimals=0; max_length=21; maybe_null=null_value=0; return FALSE; }
};


class Item_sum_sum :public Item_sum_num,
                   public Type_handler_hybrid_field_type 
{
protected:
  bool direct_added;
  bool direct_reseted_field;
  bool direct_sum_is_null;
  double direct_sum_real;
  double sum;
  my_decimal direct_sum_decimal;
  my_decimal dec_buffs[2];
  uint curr_dec_buff;
  bool fix_length_and_dec();

public:
  Item_sum_sum(THD *thd, Item *item_par, bool distinct):
    Item_sum_num(thd, item_par), direct_added(FALSE),
    direct_reseted_field(FALSE)
  {
    set_distinct(distinct);
  }
  Item_sum_sum(THD *thd, Item_sum_sum *item);
  enum Sumfunctype sum_func () const 
  { 
    return has_with_distinct() ? SUM_DISTINCT_FUNC : SUM_FUNC; 
  }
  void cleanup();
  void direct_add(my_decimal *add_sum_decimal);
  void direct_add(double add_sum_real, bool add_sum_is_null);
  void clear();
  bool add();
  double val_real();
  longlong val_int();
  String *val_str(String*str);
  my_decimal *val_decimal(my_decimal *);
  const Type_handler *type_handler() const
  { return Type_handler_hybrid_field_type::type_handler(); }
  void fix_length_and_dec_double();
  void fix_length_and_dec_decimal();
  void reset_field();
  void update_field();
  void no_rows_in_result() {}
  const char *func_name() const 
  { 
    return has_with_distinct() ? "sum(distinct " : "sum("; 
  }
  Item *copy_or_same(THD* thd);
  void remove();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_sum>(thd, this); }

  bool supports_removal() const
  {
    return true;
  }

private:
  void add_helper(bool perform_removal);
  ulonglong count;
};


class Item_sum_count :public Item_sum_int
{
  bool direct_counted;
  bool direct_reseted_field;
  longlong direct_count;
  longlong count;

  friend class Aggregator_distinct;

  void clear();
  bool add();
  void cleanup();
  void remove();

public:
  Item_sum_count(THD *thd, Item *item_par):
    Item_sum_int(thd, item_par), direct_counted(FALSE),
    direct_reseted_field(FALSE), count(0)
  {}

  

  Item_sum_count(THD *thd, List<Item> &list):
    Item_sum_int(thd, list), direct_counted(FALSE),
    direct_reseted_field(FALSE), count(0)
  {
    set_distinct(TRUE);
  }
  Item_sum_count(THD *thd, Item_sum_count *item):
    Item_sum_int(thd, item), direct_counted(FALSE),
    direct_reseted_field(FALSE), count(item->count)
  {}
  enum Sumfunctype sum_func () const 
  { 
    return has_with_distinct() ? COUNT_DISTINCT_FUNC : COUNT_FUNC; 
  }
  void no_rows_in_result() { count=0; }
  void make_const(longlong count_arg) 
  { 
    count=count_arg;
    Item_sum::make_const();
  }
  longlong val_int();
  void reset_field();
  void update_field();
  void direct_add(longlong add_count);
  const char *func_name() const 
  { 
    return has_with_distinct() ? "count(distinct " : "count(";
  }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_count>(thd, this); }

  bool supports_removal() const
  {
    return true;
  }
};


class Item_sum_avg :public Item_sum_sum
{
public:
  
  
  ulonglong count;
  uint prec_increment;
  uint f_precision, f_scale, dec_bin_size;

  Item_sum_avg(THD *thd, Item *item_par, bool distinct):
    Item_sum_sum(thd, item_par, distinct), count(0)
  {}
  Item_sum_avg(THD *thd, Item_sum_avg *item)
    :Item_sum_sum(thd, item), count(item->count),
    prec_increment(item->prec_increment) {}

  void fix_length_and_dec_double();
  void fix_length_and_dec_decimal();
  bool fix_length_and_dec();
  enum Sumfunctype sum_func () const 
  {
    return has_with_distinct() ? AVG_DISTINCT_FUNC : AVG_FUNC;
  }
  void clear();
  bool add();
  void remove();
  double val_real();
  
  longlong val_int() { return val_int_from_real(); }
  my_decimal *val_decimal(my_decimal *);
  String *val_str(String *str);
  void reset_field();
  void update_field();
  Item *result_item(THD *thd, Field *field);
  void no_rows_in_result() {}
  const char *func_name() const 
  { 
    return has_with_distinct() ? "avg(distinct " : "avg("; 
  }
  Item *copy_or_same(THD* thd);
  Field *create_tmp_field(bool group, TABLE *table);
  void cleanup()
  {
    count= 0;
    Item_sum_sum::cleanup();
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_avg>(thd, this); }

  bool supports_removal() const
  {
    return true;
  }
};




class Item_sum_variance : public Item_sum_num
{
  bool fix_length_and_dec();

public:
  double recurrence_m, recurrence_s;    
  ulonglong count;
  uint sample;
  uint prec_increment;

  Item_sum_variance(THD *thd, Item *item_par, uint sample_arg):
    Item_sum_num(thd, item_par), count(0),
    sample(sample_arg)
    {}
  Item_sum_variance(THD *thd, Item_sum_variance *item);
  enum Sumfunctype sum_func () const { return VARIANCE_FUNC; }
  void fix_length_and_dec_double();
  void fix_length_and_dec_decimal();
  void clear();
  bool add();
  double val_real();
  my_decimal *val_decimal(my_decimal *);
  void reset_field();
  void update_field();
  Item *result_item(THD *thd, Field *field);
  void no_rows_in_result() {}
  const char *func_name() const
    { return sample ? "var_samp(" : "variance("; }
  Item *copy_or_same(THD* thd);
  Field *create_tmp_field(bool group, TABLE *table);
  const Type_handler *type_handler() const { return &type_handler_double; }
  void cleanup()
  {
    count= 0;
    Item_sum_num::cleanup();
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_variance>(thd, this); }
};



class Item_sum_std :public Item_sum_variance
{
  public:
  Item_sum_std(THD *thd, Item *item_par, uint sample_arg):
    Item_sum_variance(thd, item_par, sample_arg) {}
  Item_sum_std(THD *thd, Item_sum_std *item)
    :Item_sum_variance(thd, item)
    {}
  enum Sumfunctype sum_func () const { return STD_FUNC; }
  double val_real();
  Item *result_item(THD *thd, Field *field);
  const char *func_name() const { return "std("; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_std>(thd, this); }
};


class Arg_comparator;
class Item_cache;
class Item_sum_hybrid :public Item_sum, public Type_handler_hybrid_field_type
{
protected:
  bool direct_added;
  Item *direct_item;
  Item_cache *value, *arg_cache;
  Arg_comparator *cmp;
  int cmp_sign;
  bool was_values;  
  bool was_null_value;

  public:
  Item_sum_hybrid(THD *thd, Item *item_par,int sign):
    Item_sum(thd, item_par),
    Type_handler_hybrid_field_type(&type_handler_longlong),
    direct_added(FALSE), value(0), arg_cache(0), cmp(0),
    cmp_sign(sign), was_values(TRUE)
  { collation.set(&my_charset_bin); }
  Item_sum_hybrid(THD *thd, Item_sum_hybrid *item)
    :Item_sum(thd, item),
    Type_handler_hybrid_field_type(item),
    direct_added(FALSE), value(item->value), arg_cache(0),
    cmp_sign(item->cmp_sign), was_values(item->was_values)
  { }
  bool fix_fields(THD *, Item **);
  bool fix_length_and_dec();
  void setup_hybrid(THD *thd, Item *item, Item *value_arg);
  void clear();
  void direct_add(Item *item);
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  void reset_field();
  String *val_str(String *);
  const Type_handler *real_type_handler() const
  {
    return get_arg(0)->real_type_handler();
  }
  const Type_handler *type_handler() const
  { return Type_handler_hybrid_field_type::type_handler(); }
  TYPELIB *get_typelib() const { return args[0]->get_typelib(); }
  void update_field();
  void min_max_update_str_field();
  void min_max_update_real_field();
  void min_max_update_int_field();
  void min_max_update_decimal_field();
  void cleanup();
  bool any_value() { return was_values; }
  void no_rows_in_result();
  void restore_to_before_no_rows_in_result();
  Field *create_tmp_field(bool group, TABLE *table);
  void setup_caches(THD *thd) { setup_hybrid(thd, arguments()[0], NULL); }
};


class Item_sum_min :public Item_sum_hybrid
{
public:
  Item_sum_min(THD *thd, Item *item_par): Item_sum_hybrid(thd, item_par, 1) {}
  Item_sum_min(THD *thd, Item_sum_min *item) :Item_sum_hybrid(thd, item) {}
  enum Sumfunctype sum_func () const {return MIN_FUNC;}

  bool add();
  const char *func_name() const { return "min("; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_min>(thd, this); }
};


class Item_sum_max :public Item_sum_hybrid
{
public:
  Item_sum_max(THD *thd, Item *item_par): Item_sum_hybrid(thd, item_par, -1) {}
  Item_sum_max(THD *thd, Item_sum_max *item) :Item_sum_hybrid(thd, item) {}
  enum Sumfunctype sum_func () const {return MAX_FUNC;}

  bool add();
  const char *func_name() const { return "max("; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_max>(thd, this); }
};


class Item_sum_bit :public Item_sum_int
{
public:
  Item_sum_bit(THD *thd, Item *item_par, ulonglong reset_arg):
    Item_sum_int(thd, item_par), reset_bits(reset_arg), bits(reset_arg),
    as_window_function(FALSE), num_values_added(0) {}
  Item_sum_bit(THD *thd, Item_sum_bit *item):
    Item_sum_int(thd, item), reset_bits(item->reset_bits), bits(item->bits),
    as_window_function(item->as_window_function),
    num_values_added(item->num_values_added)
  {
    if (as_window_function)
      memcpy(bit_counters, item->bit_counters, sizeof(bit_counters));
  }
  enum Sumfunctype sum_func () const {return SUM_BIT_FUNC;}
  void clear();
  longlong val_int();
  void reset_field();
  void update_field();
  bool fix_length_and_dec()
  {
    decimals= 0; max_length=21; unsigned_flag= 1; maybe_null= null_value= 0;
    return FALSE;
  }
  void cleanup()
  {
    bits= reset_bits;
    if (as_window_function)
      clear_as_window();
    Item_sum_int::cleanup();
  }
  void setup_window_func(THD *thd __attribute__((unused)),
                         Window_spec *window_spec __attribute__((unused)))
  {
    as_window_function= TRUE;
    clear_as_window();
  }
  void remove()
  {
    if (as_window_function)
    {
      remove_as_window(args[0]->val_int());
      return;
    }
    
    DBUG_ASSERT(0);
  }

  bool supports_removal() const
  {
    return true;
  }

protected:
  enum bit_counters { NUM_BIT_COUNTERS= 64 };
  ulonglong reset_bits,bits;
  
  bool as_window_function;
  
  
  ulonglong num_values_added;
  ulonglong bit_counters[NUM_BIT_COUNTERS];
  bool add_as_window(ulonglong value);
  bool remove_as_window(ulonglong value);
  bool clear_as_window();
  virtual void set_bits_from_counters()= 0;
};


class Item_sum_or :public Item_sum_bit
{
public:
  Item_sum_or(THD *thd, Item *item_par): Item_sum_bit(thd, item_par, 0) {}
  Item_sum_or(THD *thd, Item_sum_or *item) :Item_sum_bit(thd, item) {}
  bool add();
  const char *func_name() const { return "bit_or("; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_or>(thd, this); }

private:
  void set_bits_from_counters();
};


class Item_sum_and :public Item_sum_bit
{
public:
  Item_sum_and(THD *thd, Item *item_par):
    Item_sum_bit(thd, item_par, ULONGLONG_MAX) {}
  Item_sum_and(THD *thd, Item_sum_and *item) :Item_sum_bit(thd, item) {}
  bool add();
  const char *func_name() const { return "bit_and("; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_and>(thd, this); }

private:
  void set_bits_from_counters();
};

class Item_sum_xor :public Item_sum_bit
{
public:
  Item_sum_xor(THD *thd, Item *item_par): Item_sum_bit(thd, item_par, 0) {}
  Item_sum_xor(THD *thd, Item_sum_xor *item) :Item_sum_bit(thd, item) {}
  bool add();
  const char *func_name() const { return "bit_xor("; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_xor>(thd, this); }

private:
  void set_bits_from_counters();
};

class sp_head;
class sp_name;
class Query_arena;
struct st_sp_security_context;


class Item_sum_sp :public Item_sum,
                   public Item_sp
{
 private:
  bool execute();

public:
  Item_sum_sp(THD *thd, Name_resolution_context *context_arg, sp_name *name,
              sp_head *sp);

  Item_sum_sp(THD *thd, Name_resolution_context *context_arg, sp_name *name,
              sp_head *sp, List<Item> &list);
  Item_sum_sp(THD *thd, Item_sum_sp *item);

  enum Sumfunctype sum_func () const
  {
    return SP_AGGREGATE_FUNC;
  }
  Field *create_field_for_create_select(TABLE *table)
  {
    return create_table_field_from_handler(table);
  }
  bool fix_length_and_dec();
  bool fix_fields(THD *thd, Item **ref);
  const char *func_name() const;
  const Type_handler *type_handler() const;
  bool add();

  
  longlong val_int()
  {
    if(execute())
      return 0;
    return sp_result_field->val_int();
  }

  double val_real()
  {
    if(execute())
      return 0.0;
    return sp_result_field->val_real();
  }

  my_decimal *val_decimal(my_decimal *dec_buf)
  {
    if(execute())
      return NULL;
    return sp_result_field->val_decimal(dec_buf);
  }

  String *val_str(String *str)
  {
    String buf;
    char buff[20];
    buf.set(buff, 20, str->charset());
    buf.length(0);
    if (execute())
      return NULL;
    
    sp_result_field->val_str(&buf);
    str->copy(buf);
    return str;
  }
  void reset_field(){DBUG_ASSERT(0);}
  void update_field(){DBUG_ASSERT(0);}
  void clear();
  void cleanup();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return execute() || sp_result_field->get_date(ltime, fuzzydate);
  }
  inline Field *get_sp_result_field()
  {
    return sp_result_field;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_sp>(thd, this); }
  Item *copy_or_same(THD *thd);
};



class Item_sum_field :public Item
{
protected:
  Field *field;
public:
  Item_sum_field(THD *thd, Item_sum *item)
    :Item(thd), field(item->result_field)
  {
    name= item->name;
    maybe_null= true;
    decimals= item->decimals;
    max_length= item->max_length;
    unsigned_flag= item->unsigned_flag;
    fixed= true;
  }
  table_map used_tables() const { return (table_map) 1L; }
  void save_in_result_field(bool no_conversions) { DBUG_ASSERT(0); }
  bool check_vcol_func_processor(void *arg)
  {
    return mark_unsupported_function(name.str, arg, VCOL_IMPOSSIBLE);
  }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return type_handler()->Item_get_date(this, ltime, fuzzydate);
  }
};


class Item_avg_field :public Item_sum_field
{
protected:
  uint prec_increment;
public:
  Item_avg_field(THD *thd, Item_sum_avg *item)
   :Item_sum_field(thd, item), prec_increment(item->prec_increment)
  { }
  enum Type type() const { return FIELD_AVG_ITEM; }
  bool is_null() { update_null_value(); return null_value; }
};


class Item_avg_field_double :public Item_avg_field
{
public:
  Item_avg_field_double(THD *thd, Item_sum_avg *item)
   :Item_avg_field(thd, item)
  { }
  const Type_handler *type_handler() const { return &type_handler_double; }
  longlong val_int() { return val_int_from_real(); }
  my_decimal *val_decimal(my_decimal *dec) { return val_decimal_from_real(dec); }
  String *val_str(String *str) { return val_string_from_real(str); }
  double val_real();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_avg_field_double>(thd, this); }
};


class Item_avg_field_decimal :public Item_avg_field
{
  uint f_precision, f_scale, dec_bin_size;
public:
  Item_avg_field_decimal(THD *thd, Item_sum_avg *item)
   :Item_avg_field(thd, item),
    f_precision(item->f_precision),
    f_scale(item->f_scale),
    dec_bin_size(item->dec_bin_size)
  { }
  const Type_handler *type_handler() const { return &type_handler_newdecimal; }
  double val_real() { return val_real_from_decimal(); }
  longlong val_int() { return val_int_from_decimal(); }
  String *val_str(String *str) { return val_string_from_decimal(str); }
  my_decimal *val_decimal(my_decimal *);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_avg_field_decimal>(thd, this); }
};


class Item_variance_field :public Item_sum_field
{
  uint sample;
public:
  Item_variance_field(THD *thd, Item_sum_variance *item)
   :Item_sum_field(thd, item), sample(item->sample)
  { }
  enum Type type() const {return FIELD_VARIANCE_ITEM; }
  double val_real();
  longlong val_int() { return val_int_from_real(); }
  String *val_str(String *str)
  { return val_string_from_real(str); }
  my_decimal *val_decimal(my_decimal *dec_buf)
  { return val_decimal_from_real(dec_buf); }
  bool is_null() { update_null_value(); return null_value; }
  const Type_handler *type_handler() const { return &type_handler_double; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_variance_field>(thd, this); }
};


class Item_std_field :public Item_variance_field
{
public:
  Item_std_field(THD *thd, Item_sum_std *item)
   :Item_variance_field(thd, item)
  { }
  enum Type type() const { return FIELD_STD_ITEM; }
  double val_real();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_std_field>(thd, this); }
};




#ifdef HAVE_DLOPEN

class Item_udf_sum : public Item_sum
{
protected:
  udf_handler udf;

public:
  Item_udf_sum(THD *thd, udf_func *udf_arg):
    Item_sum(thd), udf(udf_arg)
  { quick_group=0; }
  Item_udf_sum(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_sum(thd, list), udf(udf_arg)
  { quick_group=0;}
  Item_udf_sum(THD *thd, Item_udf_sum *item)
    :Item_sum(thd, item), udf(item->udf)
  { udf.not_original= TRUE; }
  const char *func_name() const { return udf.name(); }
  bool fix_fields(THD *thd, Item **ref)
  {
    DBUG_ASSERT(fixed == 0);

    if (init_sum_func_check(thd))
      return TRUE;

    fixed= 1;
    
    DBUG_ASSERT(!const_item_cache);
    if (udf.fix_fields(thd, this, this->arg_count, this->args))
      return TRUE;
    
    const_item_cache= false;
    memcpy (orig_args, args, sizeof (Item *) * arg_count);
    return check_sum_func(thd, ref);
  }
  enum Sumfunctype sum_func () const { return UDF_SUM_FUNC; }
  virtual bool have_field_update(void) const { return 0; }

  void clear();
  bool add();
  void reset_field() {};
  void update_field() {};
  void cleanup();
  virtual void print(String *str, enum_query_type query_type);
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return type_handler()->Item_get_date(this, ltime, fuzzydate);
  }
};


class Item_sum_udf_float :public Item_udf_sum
{
 public:
  Item_sum_udf_float(THD *thd, udf_func *udf_arg):
    Item_udf_sum(thd, udf_arg) {}
  Item_sum_udf_float(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_udf_sum(thd, udf_arg, list) {}
  Item_sum_udf_float(THD *thd, Item_sum_udf_float *item)
    :Item_udf_sum(thd, item) {}
  longlong val_int() { return val_int_from_real(); }
  double val_real();
  String *val_str(String*str);
  my_decimal *val_decimal(my_decimal *);
  const Type_handler *type_handler() const { return &type_handler_double; }
  bool fix_length_and_dec() { fix_num_length_and_dec(); return FALSE; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_udf_float>(thd, this); }
};


class Item_sum_udf_int :public Item_udf_sum
{
public:
  Item_sum_udf_int(THD *thd, udf_func *udf_arg):
    Item_udf_sum(thd, udf_arg) {}
  Item_sum_udf_int(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_udf_sum(thd, udf_arg, list) {}
  Item_sum_udf_int(THD *thd, Item_sum_udf_int *item)
    :Item_udf_sum(thd, item) {}
  longlong val_int();
  double val_real()
    { DBUG_ASSERT(fixed == 1); return (double) Item_sum_udf_int::val_int(); }
  String *val_str(String*str);
  my_decimal *val_decimal(my_decimal *);
  const Type_handler *type_handler() const { return &type_handler_longlong; }
  bool fix_length_and_dec() { decimals=0; max_length=21; return FALSE; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_udf_int>(thd, this); }
};


class Item_sum_udf_str :public Item_udf_sum
{
public:
  Item_sum_udf_str(THD *thd, udf_func *udf_arg):
    Item_udf_sum(thd, udf_arg) {}
  Item_sum_udf_str(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_udf_sum(thd, udf_arg, list) {}
  Item_sum_udf_str(THD *thd, Item_sum_udf_str *item)
    :Item_udf_sum(thd, item) {}
  String *val_str(String *);
  double val_real()
  {
    int err_not_used;
    char *end_not_used;
    String *res;
    res=val_str(&str_value);
    return res ? my_strntod(res->charset(),(char*) res->ptr(),res->length(),
			    &end_not_used, &err_not_used) : 0.0;
  }
  longlong val_int()
  {
    int err_not_used;
    char *end;
    String *res;
    CHARSET_INFO *cs;

    if (!(res= val_str(&str_value)))
      return 0;                                 
    cs= res->charset();
    end= (char*) res->ptr()+res->length();
    return cs->cset->strtoll10(cs, res->ptr(), &end, &err_not_used);
  }
  my_decimal *val_decimal(my_decimal *dec);
  const Type_handler *type_handler() const { return string_type_handler(); }
  bool fix_length_and_dec();
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_udf_str>(thd, this); }
};


class Item_sum_udf_decimal :public Item_udf_sum
{
public:
  Item_sum_udf_decimal(THD *thd, udf_func *udf_arg):
    Item_udf_sum(thd, udf_arg) {}
  Item_sum_udf_decimal(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_udf_sum(thd, udf_arg, list) {}
  Item_sum_udf_decimal(THD *thd, Item_sum_udf_decimal *item)
    :Item_udf_sum(thd, item) {}
  String *val_str(String *);
  double val_real();
  longlong val_int();
  my_decimal *val_decimal(my_decimal *);
  const Type_handler *type_handler() const { return &type_handler_newdecimal; }
  bool fix_length_and_dec() { fix_num_length_and_dec(); return FALSE; }
  Item *copy_or_same(THD* thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_sum_udf_decimal>(thd, this); }
};

#else 

class Item_sum_udf_float :public Item_sum_num
{
 public:
  Item_sum_udf_float(THD *thd, udf_func *udf_arg):
    Item_sum_num(thd) {}
  Item_sum_udf_float(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_sum_num(thd) {}
  Item_sum_udf_float(THD *thd, Item_sum_udf_float *item)
    :Item_sum_num(thd, item) {}
  enum Sumfunctype sum_func () const { return UDF_SUM_FUNC; }
  double val_real() { DBUG_ASSERT(fixed == 1); return 0.0; }
  void clear() {}
  bool add() { return 0; }  
  void update_field() {}
};


class Item_sum_udf_int :public Item_sum_num
{
public:
  Item_sum_udf_int(THD *thd, udf_func *udf_arg):
    Item_sum_num(thd) {}
  Item_sum_udf_int(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_sum_num(thd) {}
  Item_sum_udf_int(THD *thd, Item_sum_udf_int *item)
    :Item_sum_num(thd, item) {}
  enum Sumfunctype sum_func () const { return UDF_SUM_FUNC; }
  longlong val_int() { DBUG_ASSERT(fixed == 1); return 0; }
  double val_real() { DBUG_ASSERT(fixed == 1); return 0; }
  void clear() {}
  bool add() { return 0; }  
  void update_field() {}
};


class Item_sum_udf_decimal :public Item_sum_num
{
 public:
  Item_sum_udf_decimal(THD *thd, udf_func *udf_arg):
    Item_sum_num(thd) {}
  Item_sum_udf_decimal(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_sum_num(thd) {}
  Item_sum_udf_decimal(THD *thd, Item_sum_udf_float *item)
    :Item_sum_num(thd, item) {}
  enum Sumfunctype sum_func () const { return UDF_SUM_FUNC; }
  double val_real() { DBUG_ASSERT(fixed == 1); return 0.0; }
  my_decimal *val_decimal(my_decimal *) { DBUG_ASSERT(fixed == 1); return 0; }
  void clear() {}
  bool add() { return 0; }
  void update_field() {}
};


class Item_sum_udf_str :public Item_sum_num
{
public:
  Item_sum_udf_str(THD *thd, udf_func *udf_arg):
    Item_sum_num(thd) {}
  Item_sum_udf_str(THD *thd, udf_func *udf_arg, List<Item> &list):
    Item_sum_num(thd) {}
  Item_sum_udf_str(THD *thd, Item_sum_udf_str *item)
    :Item_sum_num(thd, item) {}
  String *val_str(String *)
    { DBUG_ASSERT(fixed == 1); null_value=1; return 0; }
  double val_real() { DBUG_ASSERT(fixed == 1); null_value=1; return 0.0; }
  longlong val_int() { DBUG_ASSERT(fixed == 1); null_value=1; return 0; }
  bool fix_length_and_dec() { maybe_null=1; max_length=0; return FALSE; }
  enum Sumfunctype sum_func () const { return UDF_SUM_FUNC; }
  void clear() {}
  bool add() { return 0; }  
  void update_field() {}
};

#endif 

C_MODE_START
int group_concat_key_cmp_with_distinct(void* arg, const void* key1,
                                       const void* key2);
int group_concat_key_cmp_with_order(void* arg, const void* key1,
                                    const void* key2);
int dump_leaf_key(void* key_arg,
                  element_count count __attribute__((unused)),
                  void* item_arg);
C_MODE_END

class Item_func_group_concat : public Item_sum
{
  TMP_TABLE_PARAM *tmp_table_param;
  String result;
  String *separator;
  TREE tree_base;
  TREE *tree;
  Item **ref_pointer_array;

  
  Unique *unique_filter;
  TABLE *table;
  ORDER **order;
  Name_resolution_context *context;
  
  uint arg_count_order;
  
  uint arg_count_field;
  uint row_count;
  bool distinct;
  bool warning_for_row;
  bool always_null;
  bool force_copy_fields;
  bool no_appended;
  
  Item *row_limit;
  
  Item *offset_limit;
  bool limit_clause;
  
  ulonglong copy_offset_limit;
  
  ulonglong copy_row_limit;

  
  Item_func_group_concat *original;

  friend int group_concat_key_cmp_with_distinct(void* arg, const void* key1,
                                                const void* key2);
  friend int group_concat_key_cmp_with_order(void* arg, const void* key1,
					     const void* key2);
  friend int dump_leaf_key(void* key_arg,
                           element_count count __attribute__((unused)),
			   void* item_arg);
public:
  Item_func_group_concat(THD *thd, Name_resolution_context *context_arg,
                         bool is_distinct, List<Item> *is_select,
                         const SQL_I_List<ORDER> &is_order, String *is_separator,
                         bool limit_clause, Item *row_limit, Item *offset_limit);

  Item_func_group_concat(THD *thd, Item_func_group_concat *item);
  ~Item_func_group_concat();
  void cleanup();

  enum Sumfunctype sum_func () const {return GROUP_CONCAT_FUNC;}
  const char *func_name() const { return "group_concat("; }
  const Type_handler *type_handler() const
  {
    if (too_big_for_varchar())
      return &type_handler_blob;
    return &type_handler_varchar;
  }
  void clear();
  bool add();
  void reset_field() { DBUG_ASSERT(0); }        
  void update_field() { DBUG_ASSERT(0); }       
  bool fix_fields(THD *,Item **);
  bool setup(THD *thd);
  void make_unique();
  double val_real()
  {
    int error;
    const char *end;
    String *res;
    if (!(res= val_str(&str_value)))
      return 0.0;
    end= res->ptr() + res->length();
    return (my_strtod(res->ptr(), (char**) &end, &error));
  }
  longlong val_int()
  {
    String *res;
    char *end_ptr;
    int error;
    if (!(res= val_str(&str_value)))
      return (longlong) 0;
    end_ptr= (char*) res->ptr()+ res->length();
    return my_strtoll10(res->ptr(), &end_ptr, &error);
  }
  my_decimal *val_decimal(my_decimal *decimal_value)
  {
    return val_decimal_from_string(decimal_value);
  }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return get_date_from_string(ltime, fuzzydate);
  }
  String* val_str(String* str);
  Item *copy_or_same(THD* thd);
  void no_rows_in_result() {}
  virtual void print(String *str, enum_query_type query_type);
  virtual bool change_context_processor(void *cntx)
    { context= (Name_resolution_context *)cntx; return FALSE; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_group_concat>(thd, this); }
};

#endif 
