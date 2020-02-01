#ifndef ITEM_CMPFUNC_INCLUDED
#define ITEM_CMPFUNC_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "item_func.h"             
#define PCRE_STATIC 1             
#include "pcre.h"                 
#include "item.h"

extern Item_result item_cmp_type(Item_result a,Item_result b);
inline Item_result item_cmp_type(const Item *a, const Item *b)
{
  return item_cmp_type(a->cmp_type(), b->cmp_type());
}
inline Item_result item_cmp_type(Item_result a, const Item *b)
{
  return item_cmp_type(a, b->cmp_type());
}
class Item_bool_func2;
class Arg_comparator;

typedef int (Arg_comparator::*arg_cmp_func)();

typedef int (*Item_field_cmpfunc)(Item *f1, Item *f2, void *arg); 

class Arg_comparator: public Sql_alloc
{
  Item **a, **b;
  const Type_handler *m_compare_handler;
  CHARSET_INFO *m_compare_collation;
  arg_cmp_func func;
  Item_func_or_sum *owner;
  bool set_null;                   
  Arg_comparator *comparators;   
  double precision;
  
  Item *a_cache, *b_cache;         
                                   

  int set_cmp_func(Item_func_or_sum *owner_arg, Item **a1, Item **a2);

  int compare_not_null_values(longlong val1, longlong val2)
  {
    if (set_null)
      owner->null_value= false;
    if (val1 < val2) return -1;
    if (val1 == val2) return 0;
    return 1;
  }
public:
  
  String value1, value2;

  Arg_comparator():
    m_compare_handler(&type_handler_null),
    m_compare_collation(&my_charset_bin),
    set_null(TRUE), comparators(0),
    a_cache(0), b_cache(0) {};
  Arg_comparator(Item **a1, Item **a2): a(a1), b(a2),
    m_compare_handler(&type_handler_null),
    m_compare_collation(&my_charset_bin),
    set_null(TRUE), comparators(0),
    a_cache(0), b_cache(0) {};

public:
  bool set_cmp_func_for_row_arguments();
  bool set_cmp_func_row();
  bool set_cmp_func_string();
  bool set_cmp_func_time();
  bool set_cmp_func_datetime();
  bool set_cmp_func_int();
  bool set_cmp_func_real();
  bool set_cmp_func_decimal();

  inline int set_cmp_func(Item_func_or_sum *owner_arg,
			  Item **a1, Item **a2, bool set_null_arg)
  {
    set_null= set_null_arg;
    return set_cmp_func(owner_arg, a1, a2);
  }
  inline int compare() { return (this->*func)(); }

  int compare_string();		 
  int compare_real();            
  int compare_decimal();         
  int compare_int_signed();      
  int compare_int_signed_unsigned();
  int compare_int_unsigned_signed();
  int compare_int_unsigned();
  int compare_row();             
  int compare_e_string();	 
  int compare_e_real();          
  int compare_e_decimal();       
  int compare_e_int();           
  int compare_e_int_diff_signedness();
  int compare_e_row();           
  int compare_real_fixed();
  int compare_e_real_fixed();
  int compare_datetime();
  int compare_e_datetime();
  int compare_time();
  int compare_e_time();
  int compare_json_str_basic(Item *j, Item *s);
  int compare_json_str();
  int compare_str_json();
  int compare_e_json_str_basic(Item *j, Item *s);
  int compare_e_json_str();
  int compare_e_str_json();

  Item** cache_converted_constant(THD *thd, Item **value, Item **cache,
                                  const Type_handler *type);
  inline bool is_owner_equal_func()
  {
    return (owner->type() == Item::FUNC_ITEM &&
           ((Item_func*)owner)->functype() == Item_func::EQUAL_FUNC);
  }
  const Type_handler *compare_type_handler() const { return m_compare_handler; }
  Item_result compare_type() const { return m_compare_handler->cmp_type(); }
  CHARSET_INFO *compare_collation() const { return m_compare_collation; }
  Arg_comparator *subcomparators() const { return comparators; }
  void cleanup()
  {
    delete [] comparators;
    comparators= 0;
  }
  friend class Item_func;
  friend class Item_bool_rowready_func2;
};


class SEL_ARG;
struct KEY_PART;

class Item_bool_func :public Item_int_func
{
protected:
  
  virtual SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                                     Field *field, Item *value)
  {
    DBUG_ENTER("Item_bool_func::get_func_mm_tree");
    DBUG_ASSERT(0);
    DBUG_RETURN(0);
  }
  
  SEL_TREE *get_full_func_mm_tree(RANGE_OPT_PARAM *param,
                                  Item_field *field_item, Item *value);
  
  SEL_TREE *get_full_func_mm_tree_for_args(RANGE_OPT_PARAM *param,
                                           Item *item, Item *value)
  {
    DBUG_ENTER("Item_bool_func::get_full_func_mm_tree_for_args");
    Item *field= item->real_item();
    if (field->type() == Item::FIELD_ITEM && !field->const_item() &&
        (!value || !value->is_expensive()))
      DBUG_RETURN(get_full_func_mm_tree(param, (Item_field *) field, value));
    DBUG_RETURN(NULL);
  }
  SEL_TREE *get_mm_parts(RANGE_OPT_PARAM *param, Field *field,
                         Item_func::Functype type, Item *value);
  SEL_TREE *get_ne_mm_tree(RANGE_OPT_PARAM *param,
                           Field *field, Item *lt_value, Item *gt_value);
  virtual SEL_ARG *get_mm_leaf(RANGE_OPT_PARAM *param, Field *field,
                               KEY_PART *key_part,
                               Item_func::Functype type, Item *value);
public:
  Item_bool_func(THD *thd): Item_int_func(thd) {}
  Item_bool_func(THD *thd, Item *a): Item_int_func(thd, a) {}
  Item_bool_func(THD *thd, Item *a, Item *b): Item_int_func(thd, a, b) {}
  Item_bool_func(THD *thd, Item *a, Item *b, Item *c): Item_int_func(thd, a, b, c) {}
  Item_bool_func(THD *thd, List<Item> &list): Item_int_func(thd, list) { }
  Item_bool_func(THD *thd, Item_bool_func *item) :Item_int_func(thd, item) {}
  const Type_handler *type_handler() const { return &type_handler_long; }
  bool is_bool_type() { return true; }
  virtual CHARSET_INFO *compare_collation() const { return NULL; }
  bool fix_length_and_dec() { decimals=0; max_length=1; return FALSE; }
  uint decimal_precision() const { return 1; }
  bool need_parentheses_in_default() { return true; }
};




class Item_func_truth : public Item_bool_func
{
public:
  virtual bool val_bool();
  virtual longlong val_int();
  virtual bool fix_length_and_dec();
  virtual void print(String *str, enum_query_type query_type);
  enum precedence precedence() const { return CMP_PRECEDENCE; }

protected:
  Item_func_truth(THD *thd, Item *a, bool a_value, bool a_affirmative):
    Item_bool_func(thd, a), value(a_value), affirmative(a_affirmative)
  {}

  ~Item_func_truth()
  {}
private:
  
  const bool value;
  
  const bool affirmative;
};




class Item_func_istrue : public Item_func_truth
{
public:
  Item_func_istrue(THD *thd, Item *a): Item_func_truth(thd, a, true, true) {}
  ~Item_func_istrue() {}
  virtual const char* func_name() const { return "istrue"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_istrue>(thd, this); }
};




class Item_func_isnottrue : public Item_func_truth
{
public:
  Item_func_isnottrue(THD *thd, Item *a):
    Item_func_truth(thd, a, true, false) {}
  ~Item_func_isnottrue() {}
  virtual const char* func_name() const { return "isnottrue"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_isnottrue>(thd, this); }
};




class Item_func_isfalse : public Item_func_truth
{
public:
  Item_func_isfalse(THD *thd, Item *a): Item_func_truth(thd, a, false, true) {}
  ~Item_func_isfalse() {}
  virtual const char* func_name() const { return "isfalse"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_isfalse>(thd, this); }
};




class Item_func_isnotfalse : public Item_func_truth
{
public:
  Item_func_isnotfalse(THD *thd, Item *a):
    Item_func_truth(thd, a, false, false) {}
  ~Item_func_isnotfalse() {}
  virtual const char* func_name() const { return "isnotfalse"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_isnotfalse>(thd, this); }
};


class Item_cache;
#define UNKNOWN (-1)




class Item_in_optimizer: public Item_bool_func
{
protected:
  Item_cache *cache;
  Item *expr_cache;
  bool save_cache;
  
  int result_for_null_param;
public:
  Item_in_optimizer(THD *thd, Item *a, Item *b):
    Item_bool_func(thd, a, b), cache(0), expr_cache(0),
    save_cache(0), result_for_null_param(UNKNOWN)
  { m_with_subquery= true; }
  bool fix_fields(THD *, Item **);
  bool fix_left(THD *thd);
  table_map not_null_tables() const { return 0; }
  bool is_null();
  longlong val_int();
  void cleanup();
  enum Functype functype() const   { return IN_OPTIMIZER_FUNC; }
  const char *func_name() const { return "<in_optimizer>"; }
  Item_cache **get_cache() { return &cache; }
  void keep_top_level_cache();
  Item *transform(THD *thd, Item_transformer transformer, uchar *arg);
  virtual Item *expr_cache_insert_transformer(THD *thd, uchar *unused);
  bool is_expensive_processor(void *arg);
  bool is_expensive();
  void set_join_tab_idx(uint join_tab_idx_arg)
  { args[1]->set_join_tab_idx(join_tab_idx_arg); }
  virtual void get_cache_parameters(List<Item> &parameters);
  bool is_top_level_item();
  bool eval_not_null_tables(void *opt_arg);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  bool invisible_mode();
  void reset_cache() { cache= NULL; }
  virtual void print(String *str, enum_query_type query_type);
  void restore_first_argument();
  Item* get_wrapped_in_subselect_item()
  { return args[1]; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_in_optimizer>(thd, this); }
};



class Item_bool_func2 :public Item_bool_func
{                                              
protected:
  void add_key_fields_optimize_op(JOIN *join, KEY_FIELD **key_fields,
                                  uint *and_level, table_map usable_tables,
                                  SARGABLE_PARAM **sargables, bool equal_func);
public:
  Item_bool_func2(THD *thd, Item *a, Item *b):
    Item_bool_func(thd, a, b) { }

  bool is_null() { return MY_TEST(args[0]->is_null() || args[1]->is_null()); }
  COND *remove_eq_conds(THD *thd, Item::cond_result *cond_value,
                        bool top_level);
  bool count_sargable_conds(void *arg);
  
  virtual const Type_handler *compare_type_handler() const
  {
    
    return &type_handler_varchar;
  }
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr)
  {
    DBUG_ENTER("Item_bool_func2::get_mm_tree");
    DBUG_ASSERT(arg_count == 2);
    SEL_TREE *ftree= get_full_func_mm_tree_for_args(param, args[0], args[1]);
    if (!ftree)
      ftree= Item_func::get_mm_tree(param, cond_ptr);
    DBUG_RETURN(ftree);
  }
};



class Item_bool_func2_with_rev :public Item_bool_func2
{
protected:
  SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                             Field *field, Item *value)
  {
    DBUG_ENTER("Item_bool_func2_with_rev::get_func_mm_tree");
    Item_func::Functype func_type=
      (value != arguments()[0]) ? functype() : rev_functype();
    DBUG_RETURN(get_mm_parts(param, field, func_type, value));
  }
public:
  Item_bool_func2_with_rev(THD *thd, Item *a, Item *b):
    Item_bool_func2(thd, a, b) { }
  virtual enum Functype rev_functype() const= 0;
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr)
  {
    DBUG_ENTER("Item_bool_func2_with_rev::get_mm_tree");
    DBUG_ASSERT(arg_count == 2);
    SEL_TREE *ftree;
    
    if (!(ftree= get_full_func_mm_tree_for_args(param, args[0], args[1])) &&
        !(ftree= get_full_func_mm_tree_for_args(param, args[1], args[0])))
      ftree= Item_func::get_mm_tree(param, cond_ptr);
    DBUG_RETURN(ftree);
  }
};


class Item_bool_rowready_func2 :public Item_bool_func2_with_rev
{
protected:
  Arg_comparator cmp;
  bool check_arguments() const
  {
    return check_argument_types_like_args0();
  }
public:
  Item_bool_rowready_func2(THD *thd, Item *a, Item *b):
    Item_bool_func2_with_rev(thd, a, b), cmp(tmp_arg, tmp_arg + 1)
  { }
  void print(String *str, enum_query_type query_type)
  {
    Item_func::print_op(str, query_type);
  }
  enum precedence precedence() const { return CMP_PRECEDENCE; }
  Item *neg_transformer(THD *thd);
  virtual Item *negated_item(THD *thd);
  Item* propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    Item_args::propagate_equal_fields(thd,
                                      Context(ANY_SUBST,
                                              cmp.compare_type_handler(),
                                              compare_collation()),
                                      cond);
    return this;
  }
  bool fix_length_and_dec();
  int set_cmp_func()
  {
    return cmp.set_cmp_func(this, tmp_arg, tmp_arg + 1, true);
  }
  CHARSET_INFO *compare_collation() const { return cmp.compare_collation(); }
  const Type_handler *compare_type_handler() const
  {
    return cmp.compare_type_handler();
  }
  Arg_comparator *get_comparator() { return &cmp; }
  void cleanup()
  {
    Item_bool_func2::cleanup();
    cmp.cleanup();
  }
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables)
  {
    return add_key_fields_optimize_op(join, key_fields, and_level,
                                      usable_tables, sargables, false);
  }
  Item *build_clone(THD *thd)
  {
    Item_bool_rowready_func2 *clone=
      (Item_bool_rowready_func2 *) Item_func::build_clone(thd);
    if (clone)
    {
      clone->cmp.comparators= 0;
    }
    return clone;
  }
};


class Item_func_xor :public Item_bool_func
{
public:
  Item_func_xor(THD *thd, Item *i1, Item *i2): Item_bool_func(thd, i1, i2) {}
  enum Functype functype() const { return XOR_FUNC; }
  const char *func_name() const { return "xor"; }
  enum precedence precedence() const { return XOR_PRECEDENCE; }
  void print(String *str, enum_query_type query_type)
  { Item_func::print_op(str, query_type); }
  longlong val_int();
  Item *neg_transformer(THD *thd);
  Item* propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    Item_args::propagate_equal_fields(thd, Context_boolean(), cond);
    return this;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_xor>(thd, this); }
};

class Item_func_not :public Item_bool_func
{
  bool abort_on_null;
public:
  Item_func_not(THD *thd, Item *a):
    Item_bool_func(thd, a), abort_on_null(FALSE) {}
  virtual void top_level_item() { abort_on_null= 1; }
  bool is_top_level_item() { return abort_on_null; }
  longlong val_int();
  enum Functype functype() const { return NOT_FUNC; }
  const char *func_name() const { return "not"; }
  enum precedence precedence() const { return BANG_PRECEDENCE; }
  Item *neg_transformer(THD *thd);
  bool fix_fields(THD *, Item **);
  virtual void print(String *str, enum_query_type query_type);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_not>(thd, this); }
};

class Item_maxmin_subselect;



class Item_func_trig_cond: public Item_bool_func
{
  bool *trig_var;
public:
  Item_func_trig_cond(THD *thd, Item *a, bool *f): Item_bool_func(thd, a)
  { trig_var= f; }
  longlong val_int() { return *trig_var ? args[0]->val_int() : 1; }
  enum Functype functype() const { return TRIG_COND_FUNC; };
  const char *func_name() const { return "trigcond"; };
  bool const_item() const { return FALSE; }
  bool *get_trig_var() { return trig_var; }
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_trig_cond>(thd, this); }
};

class Item_func_not_all :public Item_func_not
{
  
  Item_sum_hybrid *test_sum_item;
  Item_maxmin_subselect *test_sub_item;

public:
  bool show;

  Item_func_not_all(THD *thd, Item *a):
    Item_func_not(thd, a), test_sum_item(0), test_sub_item(0), show(0)
    {}
  table_map not_null_tables() const { return 0; }
  longlong val_int();
  enum Functype functype() const { return NOT_ALL_FUNC; }
  const char *func_name() const { return "<not>"; }
  bool fix_fields(THD *thd, Item **ref)
    {return Item_func::fix_fields(thd, ref);}
  virtual void print(String *str, enum_query_type query_type);
  void set_sum_test(Item_sum_hybrid *item) { test_sum_item= item; test_sub_item= 0; };
  void set_sub_test(Item_maxmin_subselect *item) { test_sub_item= item; test_sum_item= 0;};
  bool empty_underlying_subquery();
  Item *neg_transformer(THD *thd);
};


class Item_func_nop_all :public Item_func_not_all
{
public:

  Item_func_nop_all(THD *thd, Item *a): Item_func_not_all(thd, a) {}
  longlong val_int();
  const char *func_name() const { return "<nop>"; }
  Item *neg_transformer(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_nop_all>(thd, this); }
};


class Item_func_eq :public Item_bool_rowready_func2
{
  bool abort_on_null;
public:
  Item_func_eq(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b),
    abort_on_null(false), in_equality_no(UINT_MAX)
  {}
  longlong val_int();
  enum Functype functype() const { return EQ_FUNC; }
  enum Functype rev_functype() const { return EQ_FUNC; }
  cond_result eq_cmp_result() const { return COND_TRUE; }
  const char *func_name() const { return "="; }
  void top_level_item() { abort_on_null= true; }
  Item *negated_item(THD *thd);
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref);
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables)
  {
    return add_key_fields_optimize_op(join, key_fields, and_level,
                                      usable_tables, sargables, true);
  }
  bool check_equality(THD *thd, COND_EQUAL *cond, List<Item> *eq_list);
  
  uint in_equality_no;
  virtual uint exists2in_reserved_items() { return 1; };
  friend class  Arg_comparator;
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_eq>(thd, this); }
};

class Item_func_equal :public Item_bool_rowready_func2
{
public:
  Item_func_equal(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b) {}
  longlong val_int();
  bool fix_length_and_dec();
  table_map not_null_tables() const { return 0; }
  enum Functype functype() const { return EQUAL_FUNC; }
  enum Functype rev_functype() const { return EQUAL_FUNC; }
  cond_result eq_cmp_result() const { return COND_TRUE; }
  const char *func_name() const { return "<=>"; }
  Item *neg_transformer(THD *thd) { return 0; }
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables)
  {
    return add_key_fields_optimize_op(join, key_fields, and_level,
                                      usable_tables, sargables, true);
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_equal>(thd, this); }
};


class Item_func_ge :public Item_bool_rowready_func2
{
public:
  Item_func_ge(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b) {};
  longlong val_int();
  enum Functype functype() const { return GE_FUNC; }
  enum Functype rev_functype() const { return LE_FUNC; }
  cond_result eq_cmp_result() const { return COND_TRUE; }
  const char *func_name() const { return ">="; }
  Item *negated_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_ge>(thd, this); }
};


class Item_func_gt :public Item_bool_rowready_func2
{
public:
  Item_func_gt(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b) {};
  longlong val_int();
  enum Functype functype() const { return GT_FUNC; }
  enum Functype rev_functype() const { return LT_FUNC; }
  cond_result eq_cmp_result() const { return COND_FALSE; }
  const char *func_name() const { return ">"; }
  Item *negated_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_gt>(thd, this); }
};


class Item_func_le :public Item_bool_rowready_func2
{
public:
  Item_func_le(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b) {};
  longlong val_int();
  enum Functype functype() const { return LE_FUNC; }
  enum Functype rev_functype() const { return GE_FUNC; }
  cond_result eq_cmp_result() const { return COND_TRUE; }
  const char *func_name() const { return "<="; }
  Item *negated_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_le>(thd, this); }
};


class Item_func_lt :public Item_bool_rowready_func2
{
public:
  Item_func_lt(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b) {}
  longlong val_int();
  enum Functype functype() const { return LT_FUNC; }
  enum Functype rev_functype() const { return GT_FUNC; }
  cond_result eq_cmp_result() const { return COND_FALSE; }
  const char *func_name() const { return "<"; }
  Item *negated_item(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_lt>(thd, this); }
};


class Item_func_ne :public Item_bool_rowready_func2
{
protected:
  SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                             Field *field, Item *value)
  {
    DBUG_ENTER("Item_func_ne::get_func_mm_tree");
    DBUG_RETURN(get_ne_mm_tree(param, field, value, value));
  }
public:
  Item_func_ne(THD *thd, Item *a, Item *b):
    Item_bool_rowready_func2(thd, a, b) {}
  longlong val_int();
  enum Functype functype() const { return NE_FUNC; }
  enum Functype rev_functype() const { return NE_FUNC; }
  cond_result eq_cmp_result() const { return COND_FALSE; }
  const char *func_name() const { return "<>"; }
  Item *negated_item(THD *thd);
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields, uint *and_level,
                      table_map usable_tables, SARGABLE_PARAM **sargables);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_ne>(thd, this); }
};




class Item_func_opt_neg :public Item_bool_func
{
protected:
  
  Type_handler_hybrid_field_type m_comparator;
  
  DTCollation cmp_collation;
public:
  bool negated;     
  bool pred_level;  
public:
  Item_func_opt_neg(THD *thd, Item *a, Item *b, Item *c):
    Item_bool_func(thd, a, b, c), negated(0), pred_level(0) {}
  Item_func_opt_neg(THD *thd, List<Item> &list):
    Item_bool_func(thd, list), negated(0), pred_level(0) {}
public:
  inline void top_level_item() { pred_level= 1; }
  bool is_top_level_item() const { return pred_level; }
  Item *neg_transformer(THD *thd)
  {
    negated= !negated;
    return this;
  }
  bool eq(const Item *item, bool binary_cmp) const;
  CHARSET_INFO *compare_collation() const { return cmp_collation.collation; }
  Item* propagate_equal_fields(THD *, const Context &, COND_EQUAL *) = 0;
};


class Item_func_between :public Item_func_opt_neg
{
protected:
  SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                             Field *field, Item *value);
public:
  String value0,value1,value2;
  Item_func_between(THD *thd, Item *a, Item *b, Item *c):
    Item_func_opt_neg(thd, a, b, c) { }
  longlong val_int()
  {
    DBUG_ASSERT(fixed);
    return m_comparator.type_handler()->Item_func_between_val_int(this);
  }
  enum Functype functype() const   { return BETWEEN; }
  const char *func_name() const { return "between"; }
  enum precedence precedence() const { return BETWEEN_PRECEDENCE; }
  bool fix_length_and_dec();
  bool fix_length_and_dec_string(THD *)
  {
    return agg_arg_charsets_for_comparison(cmp_collation, args, 3);
  }
  bool fix_length_and_dec_temporal(THD *);
  bool fix_length_and_dec_numeric(THD *);
  virtual void print(String *str, enum_query_type query_type);
  bool eval_not_null_tables(void *opt_arg);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  bool count_sargable_conds(void *arg);
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  Item* propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    Item_args::propagate_equal_fields(thd,
                                      Context(ANY_SUBST,
                                              m_comparator.type_handler(),
                                              compare_collation()),
                                      cond);
    return this;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_between>(thd, this); }

  longlong val_int_cmp_string();
  longlong val_int_cmp_temporal();
  longlong val_int_cmp_int();
  longlong val_int_cmp_real();
  longlong val_int_cmp_decimal();
};


class Item_func_strcmp :public Item_long_func
{
  bool check_arguments() const
  { return check_argument_types_can_return_str(0, 2); }
  String value1, value2;
  DTCollation cmp_collation;
public:
  Item_func_strcmp(THD *thd, Item *a, Item *b):
    Item_long_func(thd, a, b) {}
  longlong val_int();
  uint decimal_precision() const { return 1; }
  const char *func_name() const { return "strcmp"; }
  bool fix_length_and_dec()
  {
    if (agg_arg_charsets_for_comparison(cmp_collation, args, 2))
      return TRUE;
    fix_char_length(2);
    return FALSE;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_strcmp>(thd, this); }
};


struct interval_range
{
  Item_result type;
  double dbl;
  my_decimal dec;
};

class Item_func_interval :public Item_long_func
{
  Item_row *row;
  bool use_decimal_comparison;
  interval_range *intervals;
  bool check_arguments() const
  {
    return check_argument_types_like_args0();
  }
public:
  Item_func_interval(THD *thd, Item_row *a):
    Item_long_func(thd, a), row(a), intervals(0)
  { }
  bool fix_fields(THD *, Item **);
  longlong val_int();
  bool fix_length_and_dec();
  const char *func_name() const { return "interval"; }
  uint decimal_precision() const { return 2; }
  void print(String *str, enum_query_type query_type)
  {
    str->append(func_name());
    print_args(str, 0, query_type);
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_interval>(thd, this); }
};


class Item_func_coalesce :public Item_func_case_expression
{
public:
  Item_func_coalesce(THD *thd, Item *a, Item *b):
    Item_func_case_expression(thd, a, b) {}
  Item_func_coalesce(THD *thd, List<Item> &list):
    Item_func_case_expression(thd, list) {}
  double real_op();
  longlong int_op();
  String *str_op(String *);
  my_decimal *decimal_op(my_decimal *);
  bool date_op(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool time_op(MYSQL_TIME *ltime);
  bool fix_length_and_dec()
  {
    if (aggregate_for_result(func_name(), args, arg_count, true))
      return TRUE;
    fix_attributes(args, arg_count);
    return FALSE;
  }
  const char *func_name() const { return "coalesce"; }
  table_map not_null_tables() const { return 0; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_coalesce>(thd, this); }
};



class Item_func_case_abbreviation2 :public Item_func_case_expression
{
protected:
  bool fix_length_and_dec2(Item **items)
  {
    if (aggregate_for_result(func_name(), items, 2, true))
      return TRUE;
    fix_attributes(items, 2);
    return FALSE;
  }

  void cache_type_info(const Item *source, bool maybe_null_arg)
  {
    Type_std_attributes::set(source);
    set_handler(source->type_handler());
    maybe_null= maybe_null_arg;
  }

  bool fix_length_and_dec2_eliminate_null(Item **items)
  {
    
    if (items[0]->type() == NULL_ITEM)
    {
      cache_type_info(items[1], true);
      
      if (items[1]->type() == NULL_ITEM)
        set_handler(&type_handler_string);
    }
    else if (items[1]->type() == NULL_ITEM)
    {
      cache_type_info(items[0], true);
    }
    else
    {
      if (fix_length_and_dec2(items))
        return TRUE;
    }
    return FALSE;
  }

public:
  Item_func_case_abbreviation2(THD *thd, Item *a, Item *b):
    Item_func_case_expression(thd, a, b) { }
  Item_func_case_abbreviation2(THD *thd, Item *a, Item *b, Item *c):
    Item_func_case_expression(thd, a, b, c) { }
};


class Item_func_ifnull :public Item_func_case_abbreviation2
{
public:
  Item_func_ifnull(THD *thd, Item *a, Item *b):
    Item_func_case_abbreviation2(thd, a, b) {}
  double real_op();
  longlong int_op();
  String *str_op(String *str);
  my_decimal *decimal_op(my_decimal *);
  bool date_op(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool time_op(MYSQL_TIME *ltime);
  bool fix_length_and_dec()
  {
    if (Item_func_case_abbreviation2::fix_length_and_dec2(args))
      return TRUE;
    maybe_null= args[1]->maybe_null;
    return FALSE;
  }
  const char *func_name() const { return "ifnull"; }

  table_map not_null_tables() const { return 0; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_ifnull>(thd, this); }
};



class Item_func_case_abbreviation2_switch: public Item_func_case_abbreviation2
{
protected:
  virtual Item *find_item() const= 0;

public:
  Item_func_case_abbreviation2_switch(THD *thd, Item *a, Item *b, Item *c)
    :Item_func_case_abbreviation2(thd, a, b, c)
  { }

  bool date_op(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    Datetime dt(current_thd, find_item(), fuzzydate);
    return (null_value= dt.copy_to_mysql_time(ltime, mysql_timestamp_type()));
  }
  bool time_op(MYSQL_TIME *ltime)
  {
    return (null_value= Time(find_item()).copy_to_mysql_time(ltime));
  }
  longlong int_op()
  {
    return val_int_from_item(find_item());
  }
  double real_op()
  {
    return val_real_from_item(find_item());
  }
  my_decimal *decimal_op(my_decimal *decimal_value)
  {
    return val_decimal_from_item(find_item(), decimal_value);
  }
  String *str_op(String *str)
  {
    return val_str_from_item(find_item(), str);
  }
};


class Item_func_if :public Item_func_case_abbreviation2_switch
{
protected:
  Item *find_item() const { return args[0]->val_bool() ? args[1] : args[2]; }

public:
  Item_func_if(THD *thd, Item *a, Item *b, Item *c):
    Item_func_case_abbreviation2_switch(thd, a, b, c)
  {}
  bool fix_fields(THD *, Item **);
  bool fix_length_and_dec()
  {
    return fix_length_and_dec2_eliminate_null(args + 1);
  }
  const char *func_name() const { return "if"; }
  bool eval_not_null_tables(void *opt_arg);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_if>(thd, this); }
private:
  void cache_type_info(Item *source);
};


class Item_func_nvl2 :public Item_func_case_abbreviation2_switch
{
protected:
  Item *find_item() const { return args[0]->is_null() ? args[2] : args[1]; }

public:
  Item_func_nvl2(THD *thd, Item *a, Item *b, Item *c):
    Item_func_case_abbreviation2_switch(thd, a, b, c)
  {}
  const char *func_name() const { return "nvl2"; }
  bool fix_length_and_dec()
  {
    return fix_length_and_dec2_eliminate_null(args + 1);
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_nvl2>(thd, this); }
};


class Item_func_nullif :public Item_func_case_expression
{
  Arg_comparator cmp;
  
  Item_cache *m_cache;
  int compare();
  void reset_first_arg_if_needed()
  { 
    if (arg_count == 3 && args[0] != args[2])
      args[0]= args[2];
  }
  Item *m_arg0;
public:
  
  Item_func_nullif(THD *thd, Item *a, Item *b):
    Item_func_case_expression(thd, a, b, a),
    m_cache(NULL),
    m_arg0(NULL)
  { arg_count--; }
  void cleanup()
  {
    Item_func_hybrid_field_type::cleanup();
    arg_count= 2; 
  }
  bool date_op(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool time_op(MYSQL_TIME *ltime);
  double real_op();
  longlong int_op();
  String *str_op(String *str);
  my_decimal *decimal_op(my_decimal *);
  bool fix_length_and_dec();
  bool walk(Item_processor processor, bool walk_subquery, void *arg);
  const char *func_name() const { return "nullif"; }
  void print(String *str, enum_query_type query_type);
  void split_sum_func(THD *thd, Ref_ptr_array ref_pointer_array, 
                      List<Item> &fields, uint flags);
  void update_used_tables();
  table_map not_null_tables() const { return 0; }
  bool is_null();
  Item* propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    Context cmpctx(ANY_SUBST, cmp.compare_type_handler(),
                              cmp.compare_collation());
    const Item *old0= args[0];
    args[0]->propagate_equal_fields_and_change_item_tree(thd, cmpctx,
                                                         cond, &args[0]);
    args[1]->propagate_equal_fields_and_change_item_tree(thd, cmpctx,
                                                         cond, &args[1]);
    
    if (old0 != args[0])
      args[2]->propagate_equal_fields_and_change_item_tree(thd,
                                                           Context_identity(),
                                                           cond, &args[2]);
    return this;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_nullif>(thd, this); }
  Item *derived_field_transformer_for_having(THD *thd, uchar *arg)
  { reset_first_arg_if_needed(); return this; }
  Item *derived_field_transformer_for_where(THD *thd, uchar *arg)
  { reset_first_arg_if_needed(); return this; }
  Item *derived_grouping_field_transformer_for_where(THD *thd, uchar *arg)
  { reset_first_arg_if_needed(); return this; }
};







class in_vector :public Sql_alloc
{
public:
  char *base;
  uint size;
  qsort2_cmp compare;
  CHARSET_INFO *collation;
  uint count;
  uint used_count;
  in_vector() {}
  in_vector(THD *thd, uint elements, uint element_length, qsort2_cmp cmp_func,
  	    CHARSET_INFO *cmp_coll)
    :base((char*) thd_calloc(thd, elements * element_length)),
     size(element_length), compare(cmp_func), collation(cmp_coll),
     count(elements), used_count(elements) {}
  virtual ~in_vector() {}
  virtual void set(uint pos,Item *item)=0;
  virtual uchar *get_value(Item *item)=0;
  void sort()
  {
    my_qsort2(base,used_count,size,compare,(void*)collation);
  }
  bool find(Item *item);
  
  
  virtual Item* create_item(THD *thd) { return NULL; }
  
  
  virtual void value_to_item(uint pos, Item *item) { }
  
  
  bool compare_elems(uint pos1, uint pos2)
  {
    return MY_TEST(compare(collation, base + pos1 * size, base + pos2 * size));
  }
  virtual const Type_handler *type_handler() const= 0;
};

class in_string :public in_vector
{
  char buff[STRING_BUFFER_USUAL_SIZE];
  String tmp;
  class Item_string_for_in_vector: public Item_string
  {
  public:
    Item_string_for_in_vector(THD *thd, CHARSET_INFO *cs):
      Item_string(thd, cs)
    { }
    void set_value(const String *str)
    {
      str_value= *str;
      collation.set(str->charset());
    }
  };
public:
  in_string(THD *thd, uint elements, qsort2_cmp cmp_func, CHARSET_INFO *cs);
  ~in_string();
  void set(uint pos,Item *item);
  uchar *get_value(Item *item);
  Item* create_item(THD *thd);
  void value_to_item(uint pos, Item *item)
  {    
    String *str=((String*) base)+pos;
    Item_string_for_in_vector *to= (Item_string_for_in_vector*) item;
    to->set_value(str);
  }
  const Type_handler *type_handler() const { return &type_handler_varchar; }
};

class in_longlong :public in_vector
{
protected:
  
  struct packed_longlong 
  {
    longlong val;
    longlong unsigned_flag;  
  } tmp;
public:
  in_longlong(THD *thd, uint elements);
  void set(uint pos,Item *item);
  uchar *get_value(Item *item);
  Item* create_item(THD *thd);
  void value_to_item(uint pos, Item *item)
  {
    ((Item_int*) item)->value= ((packed_longlong*) base)[pos].val;
    ((Item_int*) item)->unsigned_flag= (bool)
      ((packed_longlong*) base)[pos].unsigned_flag;
  }
  const Type_handler *type_handler() const { return &type_handler_longlong; }

  friend int cmp_longlong(void *cmp_arg, packed_longlong *a,packed_longlong *b);
};



class in_temporal :public in_longlong
{
protected:
  uchar *get_value_internal(Item *item, enum_field_types f_type);
public:
  

  in_temporal(THD *thd, uint elements)
    :in_longlong(thd, elements) {};
  Item *create_item(THD *thd);
  void value_to_item(uint pos, Item *item)
  {
    packed_longlong *val= reinterpret_cast<packed_longlong*>(base)+pos;
    Item_datetime *dt= static_cast<Item_datetime*>(item);
    dt->set(val->val, type_handler()->mysql_timestamp_type());
  }
  uchar *get_value(Item *item)
  { return get_value_internal(item, type_handler()->field_type()); }
  friend int cmp_longlong(void *cmp_arg, packed_longlong *a,packed_longlong *b);
};


class in_datetime :public in_temporal
{
public:
  in_datetime(THD *thd, uint elements)
   :in_temporal(thd, elements)
  {}
  void set(uint pos,Item *item);
  const Type_handler *type_handler() const { return &type_handler_datetime2; }
};


class in_time :public in_temporal
{
public:
  in_time(THD *thd, uint elements)
   :in_temporal(thd, elements)
  {}
  void set(uint pos,Item *item);
  const Type_handler *type_handler() const { return &type_handler_time2; }
};


class in_double :public in_vector
{
  double tmp;
public:
  in_double(THD *thd, uint elements);
  void set(uint pos,Item *item);
  uchar *get_value(Item *item);
  Item *create_item(THD *thd);
  void value_to_item(uint pos, Item *item)
  {
    ((Item_float*)item)->value= ((double*) base)[pos];
  }
  const Type_handler *type_handler() const { return &type_handler_double; }
};


class in_decimal :public in_vector
{
  my_decimal val;
public:
  in_decimal(THD *thd, uint elements);
  void set(uint pos, Item *item);
  uchar *get_value(Item *item);
  Item *create_item(THD *thd);
  void value_to_item(uint pos, Item *item)
  {
    my_decimal *dec= ((my_decimal *)base) + pos;
    Item_decimal *item_dec= (Item_decimal*)item;
    item_dec->set_decimal_value(dec);
  }
  const Type_handler *type_handler() const { return &type_handler_newdecimal; }
};




class cmp_item :public Sql_alloc
{
public:
  CHARSET_INFO *cmp_charset;
  cmp_item() { cmp_charset= &my_charset_bin; }
  virtual ~cmp_item() {}
  virtual void store_value(Item *item)= 0;
  
  virtual int cmp(Item *item)= 0;
  virtual int cmp_not_null(const Value *value)= 0;
  
  virtual int compare(cmp_item *item)= 0;
  virtual cmp_item *make_same()= 0;
  virtual void store_value_by_template(THD *thd, cmp_item *tmpl, Item *item)
  {
    store_value(item);
  }
};


class cmp_item_scalar : public cmp_item
{
protected:
  bool m_null_value;                            
};

class cmp_item_string : public cmp_item_scalar
{
protected:
  String *value_res;
public:
  cmp_item_string () {}
  cmp_item_string (CHARSET_INFO *cs) { cmp_charset= cs; }
  void set_charset(CHARSET_INFO *cs) { cmp_charset= cs; }
  friend class cmp_item_sort_string;
  friend class cmp_item_sort_string_in_static;
};

class cmp_item_sort_string :public cmp_item_string
{
protected:
  char value_buff[STRING_BUFFER_USUAL_SIZE];
  String value;
public:
  cmp_item_sort_string():
    cmp_item_string() {}
  cmp_item_sort_string(CHARSET_INFO *cs):
    cmp_item_string(cs),
    value(value_buff, sizeof(value_buff), cs) {}
  void store_value(Item *item)
  {
    value_res= item->val_str(&value);
    m_null_value= item->null_value;
    
    if (value_res && value_res != &value)
    {
      if (value.copy(*value_res))
        value.set("", 0, item->collation.collation);
      value_res= &value;
    }
  }
  int cmp_not_null(const Value *val)
  {
    DBUG_ASSERT(!val->is_null());
    DBUG_ASSERT(val->is_string());
    return sortcmp(value_res, &val->m_string, cmp_charset) != 0;
  }
  int cmp(Item *arg)
  {
    char buff[STRING_BUFFER_USUAL_SIZE];
    String tmp(buff, sizeof(buff), cmp_charset), *res= arg->val_str(&tmp);
    if (m_null_value || arg->null_value)
      return UNKNOWN;
    if (value_res && res)
      return sortcmp(value_res, res, cmp_charset) != 0;
    else if (!value_res && !res)
      return FALSE;
    else
      return TRUE;
  }
  int compare(cmp_item *ci)
  {
    cmp_item_string *l_cmp= (cmp_item_string *) ci;
    return sortcmp(value_res, l_cmp->value_res, cmp_charset);
  } 
  cmp_item *make_same();
  void set_charset(CHARSET_INFO *cs)
  {
    cmp_charset= cs;
    value.set_quick(value_buff, sizeof(value_buff), cs);
  }
};

class cmp_item_int : public cmp_item_scalar
{
  longlong value;
public:
  cmp_item_int() {}                           
  void store_value(Item *item)
  {
    value= item->val_int();
    m_null_value= item->null_value;
  }
  int cmp_not_null(const Value *val)
  {
    DBUG_ASSERT(!val->is_null());
    DBUG_ASSERT(val->is_longlong());
    return value != val->value.m_longlong;
  }
  int cmp(Item *arg)
  {
    const bool rc= value != arg->val_int();
    return (m_null_value || arg->null_value) ? UNKNOWN : rc;
  }
  int compare(cmp_item *ci)
  {
    cmp_item_int *l_cmp= (cmp_item_int *)ci;
    return (value < l_cmp->value) ? -1 : ((value == l_cmp->value) ? 0 : 1);
  }
  cmp_item *make_same();
};


class cmp_item_temporal: public cmp_item_scalar
{
protected:
  longlong value;
  void store_value_internal(Item *item, enum_field_types type);
public:
  cmp_item_temporal() {}
  int compare(cmp_item *ci);
};


class cmp_item_datetime: public cmp_item_temporal
{
public:
  cmp_item_datetime()
   :cmp_item_temporal()
  { }
  void store_value(Item *item)
  {
    store_value_internal(item, MYSQL_TYPE_DATETIME);
  }
  int cmp_not_null(const Value *val);
  int cmp(Item *arg);
  cmp_item *make_same();
};


class cmp_item_time: public cmp_item_temporal
{
public:
  cmp_item_time()
   :cmp_item_temporal()
  { }
  void store_value(Item *item)
  {
    store_value_internal(item, MYSQL_TYPE_TIME);
  }
  int cmp_not_null(const Value *val);
  int cmp(Item *arg);
  cmp_item *make_same();
};

class cmp_item_real : public cmp_item_scalar
{
  double value;
public:
  cmp_item_real() {}                          
  void store_value(Item *item)
  {
    value= item->val_real();
    m_null_value= item->null_value;
  }
  int cmp_not_null(const Value *val)
  {
    DBUG_ASSERT(!val->is_null());
    DBUG_ASSERT(val->is_double());
    return value != val->value.m_double;
  }
  int cmp(Item *arg)
  {
    const bool rc= value != arg->val_real();
    return (m_null_value || arg->null_value) ? UNKNOWN : rc;
  }
  int compare(cmp_item *ci)
  {
    cmp_item_real *l_cmp= (cmp_item_real *) ci;
    return (value < l_cmp->value)? -1 : ((value == l_cmp->value) ? 0 : 1);
  }
  cmp_item *make_same();
};


class cmp_item_decimal : public cmp_item_scalar
{
  my_decimal value;
public:
  cmp_item_decimal() {}                       
  void store_value(Item *item);
  int cmp(Item *arg);
  int cmp_not_null(const Value *val);
  int compare(cmp_item *c);
  cmp_item *make_same();
};




class cmp_item_sort_string_in_static :public cmp_item_string
{
 protected:
  String value;
public:
  cmp_item_sort_string_in_static(CHARSET_INFO *cs):
    cmp_item_string(cs) {}
  void store_value(Item *item)
  {
    value_res= item->val_str(&value);
    m_null_value= item->null_value;
  }
  int cmp_not_null(const Value *val)
  {
    DBUG_ASSERT(false);
    return TRUE;
  }
  int cmp(Item *item)
  {
    
    DBUG_ASSERT(false);
    return TRUE;
  }
  int compare(cmp_item *ci)
  {
    cmp_item_string *l_cmp= (cmp_item_string *) ci;
    return sortcmp(value_res, l_cmp->value_res, cmp_charset);
  }
  cmp_item *make_same()
  {
    return new cmp_item_sort_string_in_static(cmp_charset);
  }
};



class Predicant_to_list_comparator
{
  
  bool alloc_comparators(THD *thd, uint nvalues);

  
  bool find_handler(uint *idx, const Type_handler *handler, uint count)
  {
    DBUG_ASSERT(count < m_comparator_count);
    for (uint i= 0 ; i < count; i++)
    {
      if (m_comparators[i].m_handler == handler)
      {
        *idx= i;
        return false;
      }
    }
    return true;
  }

  
  void detect_unique_handlers(Type_handler_hybrid_field_type *compatible,
                              uint *unique_cnt, uint *found_types);
  

  
  int cmp_arg(Item_args *args, uint i)
  {
    Predicant_to_value_comparator *cmp=
      &m_comparators[m_comparators[i].m_handler_index];
    cmp_item *in_item= cmp->m_cmp_item;
    DBUG_ASSERT(in_item);
    
    if (m_comparators[i].m_handler_index == i)
      in_item->store_value(args->arguments()[m_predicant_index]);
    
    if (args->arguments()[m_predicant_index]->null_value &&
        m_comparators[i].m_handler != &type_handler_row)
      return UNKNOWN;
    return in_item->cmp(args->arguments()[m_comparators[i].m_arg_index]);
  }
  int cmp_args_nulls_equal(Item_args *args, uint i)
  {
    Predicant_to_value_comparator *cmp=
      &m_comparators[m_comparators[i].m_handler_index];
    cmp_item *in_item= cmp->m_cmp_item;
    DBUG_ASSERT(in_item);
    Item *predicant= args->arguments()[m_predicant_index];
    Item *arg= args->arguments()[m_comparators[i].m_arg_index];
    ValueBuffer<MAX_FIELD_WIDTH> val;
    if (m_comparators[i].m_handler_index == i)
      in_item->store_value(predicant);
    m_comparators[i].m_handler->Item_save_in_value(arg, &val);
    if (predicant->null_value && val.is_null())
      return FALSE; 
    if (predicant->null_value || val.is_null())
      return UNKNOWN;
    return in_item->cmp_not_null(&val);
  }
  
  struct Predicant_to_value_comparator
  {
    const Type_handler *m_handler;
    cmp_item *m_cmp_item;
    uint m_arg_index;
    uint m_handler_index;
    void cleanup()
    {
      if (m_cmp_item)
        delete m_cmp_item;
      memset(this, 0, sizeof(*this));
    }
  };

  Predicant_to_value_comparator *m_comparators; 
  uint m_comparator_count;
  uint m_predicant_index; 
                          
                          
                          
                          

public:
  Predicant_to_list_comparator(THD *thd, uint nvalues)
   :m_comparator_count(0),
    m_predicant_index(0)
  {
    alloc_comparators(thd, nvalues);
  }

  uint comparator_count() const { return m_comparator_count; }
  const Type_handler *get_comparator_type_handler(uint i) const
  {
    DBUG_ASSERT(i < m_comparator_count);
    return m_comparators[i].m_handler;
  }
  uint get_comparator_arg_index(uint i) const
  {
    DBUG_ASSERT(i < m_comparator_count);
    return m_comparators[i].m_arg_index;
  }
  cmp_item *get_comparator_cmp_item(uint i) const
  {
    DBUG_ASSERT(i < m_comparator_count);
    return m_comparators[i].m_cmp_item;
  }

#ifndef DBUG_OFF
  void debug_print(THD *thd)
  {
    for (uint i= 0; i < m_comparator_count; i++)
    {
      DBUG_EXECUTE_IF("Predicant_to_list_comparator",
                      push_warning_printf(thd, Sql_condition::WARN_LEVEL_NOTE,
                      ER_UNKNOWN_ERROR, "DBUG: [%d] arg=%d handler=%d (%s)", i,
                      m_comparators[i].m_arg_index,
                      m_comparators[i].m_handler_index,
                      m_comparators[m_comparators[i].m_handler_index].
                                    m_handler->name().ptr()););
    }
  }
#endif

  void add_predicant(Item_args *args, uint predicant_index)
  {
    DBUG_ASSERT(m_comparator_count == 0); 
    DBUG_ASSERT(m_predicant_index == 0);  
    DBUG_ASSERT(predicant_index < args->argument_count());
    m_predicant_index= predicant_index;
  }
  
  bool add_value(const char *funcname, Item_args *args, uint value_index);

  
  bool add_value_skip_null(const char *funcname,
                           Item_args *args, uint value_index,
                           bool *nulls_found);

  
  void all_values_added(Type_handler_hybrid_field_type *compatible,
                        uint *unique_cnt, uint *found_types)
  {
    detect_unique_handlers(compatible, unique_cnt, found_types);
  }
  
  bool make_unique_cmp_items(THD *thd, CHARSET_INFO *cs);
  void cleanup()
  {
    DBUG_ASSERT(m_comparators);
    for (uint i= 0; i < m_comparator_count; i++)
      m_comparators[i].cleanup();
    memset(m_comparators, 0, sizeof(m_comparators[0]) * m_comparator_count);
    m_comparator_count= 0;
    m_predicant_index= 0;
  }
  bool init_clone(THD *thd, uint nvalues)
  {
    m_comparator_count= 0;
    m_predicant_index= 0;
    return alloc_comparators(thd, nvalues);
  }
  
  bool cmp(Item_args *args, uint *idx, bool *found_unknown_values)
  {
    for (uint i= 0 ; i < m_comparator_count ; i++)
    {
      DBUG_ASSERT(m_comparators[i].m_handler != NULL);
      const int rc= cmp_arg(args, i);
      if (rc == FALSE)
      {
        *idx= m_comparators[i].m_arg_index;
        return false; 
      }
      if (rc == UNKNOWN)
      {
        if (!found_unknown_values)
          return true;
        *found_unknown_values= true;
      }
    }
    return true; 
  }
  
  bool cmp_nulls_equal(Item_args *args, uint *idx)
  {
    for (uint i= 0 ; i < m_comparator_count ; i++)
    {
      DBUG_ASSERT(m_comparators[i].m_handler != NULL);
      if (cmp_args_nulls_equal(args, i) == FALSE)
      {
        *idx= m_comparators[i].m_arg_index;
        return false; 
      }
    }
    return true; 
  }
};




class Item_func_case :public Item_func_case_expression
{
protected:
  String tmp_value;
  DTCollation cmp_collation;
  bool aggregate_then_and_else_arguments(THD *thd, uint count);
  virtual Item **else_expr_addr() const= 0;
  virtual Item *find_item()= 0;
  void print_when_then_arguments(String *str, enum_query_type query_type,
                                 Item **items, uint count);
  void print_else_argument(String *str, enum_query_type query_type, Item *item);
  void reorder_args(uint start);
public:
  Item_func_case(THD *thd, List<Item> &list)
   :Item_func_case_expression(thd, list)
  { }
  double real_op();
  longlong int_op();
  String *str_op(String *);
  my_decimal *decimal_op(my_decimal *);
  bool date_op(MYSQL_TIME *ltime, ulonglong fuzzydate);
  bool time_op(MYSQL_TIME *ltime);
  bool fix_fields(THD *thd, Item **ref);
  table_map not_null_tables() const { return 0; }
  const char *func_name() const { return "case"; }
  enum precedence precedence() const { return BETWEEN_PRECEDENCE; }
  CHARSET_INFO *compare_collation() const { return cmp_collation.collation; }
  bool need_parentheses_in_default() { return true; }
};



class Item_func_case_searched: public Item_func_case
{
  uint when_count() const { return arg_count / 2; }
  bool with_else() const { return arg_count % 2; }
  Item **else_expr_addr() const { return with_else() ? &args[arg_count - 1] : 0; }
public:
  Item_func_case_searched(THD *thd, List<Item> &list)
   :Item_func_case(thd, list)
  {
    DBUG_ASSERT(arg_count >= 2);
    reorder_args(0);
  }
  enum Functype functype() const   { return CASE_SEARCHED_FUNC; }
  void print(String *str, enum_query_type query_type);
  bool fix_length_and_dec();
  Item *propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    
    Item_args::propagate_equal_fields(thd, Context_identity(), cond);
    return this;
  }
  Item *find_item();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_case_searched>(thd, this); }
};



class Item_func_case_simple: public Item_func_case,
                             public Predicant_to_list_comparator
{
protected:
  uint m_found_types;
  uint when_count() const { return (arg_count - 1) / 2; }
  bool with_else() const { return arg_count % 2 == 0; }
  Item **else_expr_addr() const { return with_else() ? &args[arg_count - 1] : 0; }
  bool aggregate_switch_and_when_arguments(THD *thd, bool nulls_equal);
  bool prepare_predicant_and_values(THD *thd, uint *found_types,
                                              bool nulls_equal);
public:
  Item_func_case_simple(THD *thd, List<Item> &list)
   :Item_func_case(thd, list),
    Predicant_to_list_comparator(thd, arg_count),
    m_found_types(0)
  {
    DBUG_ASSERT(arg_count >= 3);
    reorder_args(1);
  }
  void cleanup()
  {
    DBUG_ENTER("Item_func_case_simple::cleanup");
    Item_func::cleanup();
    Predicant_to_list_comparator::cleanup();
    DBUG_VOID_RETURN;
  }
  enum Functype functype() const   { return CASE_SIMPLE_FUNC; }
  void print(String *str, enum_query_type query_type);
  bool fix_length_and_dec();
  Item *propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond);
  Item *find_item();
  Item *build_clone(THD *thd)
  {
    Item_func_case_simple *clone= (Item_func_case_simple *)
                                  Item_func_case::build_clone(thd);
    uint ncases= when_count();
    if (clone && clone->Predicant_to_list_comparator::init_clone(thd, ncases))
      return NULL;
    return clone;
  } 
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_case_simple>(thd, this); }
};


class Item_func_decode_oracle: public Item_func_case_simple
{
public:
  Item_func_decode_oracle(THD *thd, List<Item> &list)
   :Item_func_case_simple(thd, list)
  { }
  const char *func_name() const { return "decode_oracle"; }
  void print(String *str, enum_query_type query_type);
  bool fix_length_and_dec();
  Item *find_item();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_decode_oracle>(thd, this); }
};



class Item_func_in :public Item_func_opt_neg,
                    public Predicant_to_list_comparator
{
  
  bool list_contains_null();
  bool all_items_are_consts(Item **items, uint nitems) const
  {
    for (uint i= 0; i < nitems; i++)
    {
      if (!items[i]->const_item() || items[i]->is_expensive())
        return false;
    }
    return true;
  }
  bool prepare_predicant_and_values(THD *thd, uint *found_types);
  bool check_arguments() const
  {
    return check_argument_types_like_args0();
  }
protected:
  SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                             Field *field, Item *value);
  bool transform_into_subq;
public:
  
  in_vector *array;
  
  bool have_null;
  
  bool arg_types_compatible;

  TABLE_LIST *emb_on_expr_nest;

  Item_func_in(THD *thd, List<Item> &list):
    Item_func_opt_neg(thd, list),
    Predicant_to_list_comparator(thd, arg_count - 1),
    transform_into_subq(false),
    array(0), have_null(0),
    arg_types_compatible(FALSE), emb_on_expr_nest(0)
  { }
  longlong val_int();
  bool fix_fields(THD *, Item **);
  bool fix_length_and_dec();
  bool compatible_types_scalar_bisection_possible()
  {
    DBUG_ASSERT(m_comparator.cmp_type() != ROW_RESULT);
    return all_items_are_consts(args + 1, arg_count - 1);     
  }
  bool compatible_types_row_bisection_possible()
  {
    DBUG_ASSERT(m_comparator.cmp_type() == ROW_RESULT);
    return all_items_are_consts(args + 1, arg_count - 1) &&   
           ((is_top_level_item() && !negated) ||              
            (!list_contains_null() && !args[0]->maybe_null)); 
  }
  bool agg_all_arg_charsets_for_comparison()
  {
    return agg_arg_charsets_for_comparison(cmp_collation, args, arg_count);
  }
  void fix_in_vector();
  bool value_list_convert_const_to_int(THD *thd);
  bool fix_for_scalar_comparison_using_bisection(THD *thd)
  {
    array= m_comparator.type_handler()->make_in_vector(thd, this, arg_count - 1);
    if (!array)      
      return true;
    fix_in_vector();
    return false;
  }
  bool fix_for_scalar_comparison_using_cmp_items(THD *thd, uint found_types);

  bool fix_for_row_comparison_using_cmp_items(THD *thd);
  bool fix_for_row_comparison_using_bisection(THD *thd);

  void cleanup()
  {
    DBUG_ENTER("Item_func_in::cleanup");
    Item_int_func::cleanup();
    delete array;
    array= 0;
    Predicant_to_list_comparator::cleanup();
    DBUG_VOID_RETURN;
  }
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields, uint *and_level,
                      table_map usable_tables, SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  SEL_TREE *get_func_row_mm_tree(RANGE_OPT_PARAM *param, Item_row *key_row); 
  Item* propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    
    if (arg_types_compatible)
    {
      Context cmpctx(ANY_SUBST, m_comparator.type_handler(),
                     Item_func_in::compare_collation());
      args[0]->propagate_equal_fields_and_change_item_tree(thd, cmpctx,
                                                           cond, &args[0]);
    }
    for (uint i= 0; i < comparator_count(); i++)
    {
      Context cmpctx(ANY_SUBST, get_comparator_type_handler(i),
                     Item_func_in::compare_collation());
      uint idx= get_comparator_arg_index(i);
      args[idx]->propagate_equal_fields_and_change_item_tree(thd, cmpctx,
                                                             cond, &args[idx]);
    }
    return this;
  }
  virtual void print(String *str, enum_query_type query_type);
  enum Functype functype() const { return IN_FUNC; }
  const char *func_name() const { return "in"; }
  enum precedence precedence() const { return CMP_PRECEDENCE; }
  bool eval_not_null_tables(void *opt_arg);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  bool count_sargable_conds(void *arg);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_in>(thd, this); }
  Item *build_clone(THD *thd)
  {
    Item_func_in *clone= (Item_func_in *) Item_func::build_clone(thd);
    if (clone)
    {
      clone->array= 0;
      if (clone->Predicant_to_list_comparator::init_clone(thd, arg_count - 1))
        return NULL;
    }
    return clone;
  }
  void mark_as_condition_AND_part(TABLE_LIST *embedding);
  bool to_be_transformed_into_in_subq(THD *thd);
  bool create_value_list_for_tvc(THD *thd, List< List<Item> > *values);
  Item *in_predicate_to_in_subs_transformer(THD *thd, uchar *arg);
};

class cmp_item_row :public cmp_item
{
  cmp_item **comparators;
  uint n;
public:
  cmp_item_row(): comparators(0), n(0) {}
  ~cmp_item_row();
  void store_value(Item *item);
  bool alloc_comparators(THD *thd, uint n);
  bool prepare_comparators(THD *, Item **args, uint arg_count);
  int cmp(Item *arg);
  int cmp_not_null(const Value *val)
  {
    DBUG_ASSERT(false);
    return TRUE;
  }
  int compare(cmp_item *arg);
  cmp_item *make_same();
  void store_value_by_template(THD *thd, cmp_item *tmpl, Item *);
  friend class Item_func_in;
  cmp_item *get_comparator(uint i) { return comparators[i]; }
};


class in_row :public in_vector
{
  cmp_item_row tmp;
public:
  in_row(THD *thd, uint elements, Item *);
  ~in_row();
  void set(uint pos,Item *item);
  uchar *get_value(Item *item);
  friend class Item_func_in;
  const Type_handler *type_handler() const { return &type_handler_row; }
  cmp_item *get_cmp_item() { return &tmp; }
};


class Item_func_null_predicate :public Item_bool_func
{
protected:
  SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                             Field *field, Item *value)
  {
    DBUG_ENTER("Item_func_null_predicate::get_func_mm_tree");
    DBUG_RETURN(get_mm_parts(param, field, functype(), value));
  }
  SEL_ARG *get_mm_leaf(RANGE_OPT_PARAM *param, Field *field,
                       KEY_PART *key_part,
                       Item_func::Functype type, Item *value);
public:
  Item_func_null_predicate(THD *thd, Item *a): Item_bool_func(thd, a) { }
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields, uint *and_level,
                      table_map usable_tables, SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr)
  {
    DBUG_ENTER("Item_func_null_predicate::get_mm_tree");
    SEL_TREE *ftree= get_full_func_mm_tree_for_args(param, args[0], NULL);
    if (!ftree)
      ftree= Item_func::get_mm_tree(param, cond_ptr);
    DBUG_RETURN(ftree);
  }
  CHARSET_INFO *compare_collation() const
  { return args[0]->collation.collation; }
  bool fix_length_and_dec()
  {
    decimals=0; max_length=1; maybe_null=0;
    return FALSE;
  }
  bool count_sargable_conds(void *arg);
};


class Item_func_isnull :public Item_func_null_predicate
{
public:
  Item_func_isnull(THD *thd, Item *a): Item_func_null_predicate(thd, a) {}
  longlong val_int();
  enum Functype functype() const { return ISNULL_FUNC; }
  const char *func_name() const { return "isnull"; }
  void print(String *str, enum_query_type query_type);
  enum precedence precedence() const { return CMP_PRECEDENCE; }

  bool arg_is_datetime_notnull_field()
  {
    Item **args= arguments();
    if (args[0]->real_item()->type() == Item::FIELD_ITEM)
    {
      Field *field=((Item_field*) args[0]->real_item())->field;

      if (((field->type() == MYSQL_TYPE_DATE) ||
          (field->type() == MYSQL_TYPE_DATETIME)) &&
          (field->flags & NOT_NULL_FLAG))
        return true;
    }
    return false;
  }

  
  virtual void update_used_tables()
  {
    if (!args[0]->maybe_null && !arg_is_datetime_notnull_field())
    {
      used_tables_cache= 0;			
      const_item_cache= 1;
    }
    else
    {
      args[0]->update_used_tables();
      used_tables_cache= args[0]->used_tables();
      const_item_cache= args[0]->const_item();
    }
  }
  COND *remove_eq_conds(THD *thd, Item::cond_result *cond_value,
                        bool top_level);
  table_map not_null_tables() const { return 0; }
  Item *neg_transformer(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_isnull>(thd, this); }
};



class Item_in_subselect;


class Item_is_not_null_test :public Item_func_isnull
{
  Item_in_subselect* owner;
public:
  Item_is_not_null_test(THD *thd, Item_in_subselect* ow, Item *a):
    Item_func_isnull(thd, a), owner(ow)
  {}
  enum Functype functype() const { return ISNOTNULLTEST_FUNC; }
  longlong val_int();
  const char *func_name() const { return "<is_not_null_test>"; }
  void update_used_tables();
  
  table_map used_tables() const
    { return used_tables_cache | RAND_TABLE_BIT; }
  bool const_item() const { return FALSE; }
};


class Item_func_isnotnull :public Item_func_null_predicate
{
  bool abort_on_null;
public:
  Item_func_isnotnull(THD *thd, Item *a):
    Item_func_null_predicate(thd, a), abort_on_null(0)
  { }
  longlong val_int();
  enum Functype functype() const { return ISNOTNULL_FUNC; }
  const char *func_name() const { return "isnotnull"; }
  enum precedence precedence() const { return CMP_PRECEDENCE; }
  table_map not_null_tables() const
  { return abort_on_null ? not_null_tables_cache : 0; }
  Item *neg_transformer(THD *thd);
  void print(String *str, enum_query_type query_type);
  void top_level_item() { abort_on_null=1; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_isnotnull>(thd, this); }
};


class Item_func_like :public Item_bool_func2
{
  
  bool        canDoTurboBM;	
  const char* pattern;
  int         pattern_len;

  
  int* bmGs; 
  int* bmBc; 

  void turboBM_compute_suffixes(int* suff);
  void turboBM_compute_good_suffix_shifts(int* suff);
  void turboBM_compute_bad_character_shifts();
  bool turboBM_matches(const char* text, int text_len) const;
  enum { alphabet_size = 256 };

  Item *escape_item;

  bool escape_used_in_parsing;
  bool use_sampling;
  bool negated;

  DTCollation cmp_collation;
  String cmp_value1, cmp_value2;
  bool with_sargable_pattern() const;
protected:
  SEL_TREE *get_func_mm_tree(RANGE_OPT_PARAM *param,
                             Field *field, Item *value)
  {
    DBUG_ENTER("Item_func_like::get_func_mm_tree");
    DBUG_RETURN(get_mm_parts(param, field, LIKE_FUNC, value));
  }
  SEL_ARG *get_mm_leaf(RANGE_OPT_PARAM *param, Field *field,
                       KEY_PART *key_part,
                       Item_func::Functype type, Item *value);
public:
  int escape;

  Item_func_like(THD *thd, Item *a, Item *b, Item *escape_arg, bool escape_used):
    Item_bool_func2(thd, a, b), canDoTurboBM(FALSE), pattern(0), pattern_len(0),
    bmGs(0), bmBc(0), escape_item(escape_arg),
    escape_used_in_parsing(escape_used), use_sampling(0), negated(0) {}
  longlong val_int();
  enum Functype functype() const { return LIKE_FUNC; }
  void print(String *str, enum_query_type query_type);
  CHARSET_INFO *compare_collation() const
  { return cmp_collation.collation; }
  cond_result eq_cmp_result() const
  {
    
    return compare_collation() == &my_charset_bin ? COND_TRUE : COND_OK;
  }
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields, uint *and_level,
                      table_map usable_tables, SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  Item* propagate_equal_fields(THD *thd, const Context &ctx, COND_EQUAL *cond)
  {
    
    uint flags= Item_func_like::compare_collation()->state;
    if ((flags & MY_CS_NOPAD) && !(flags & MY_CS_NON1TO1))
      Item_args::propagate_equal_fields(thd,
                                        Context(ANY_SUBST,
                                                &type_handler_long_blob,
                                                compare_collation()),
                                        cond);
    return this;
  }
  const char *func_name() const { return "like"; }
  enum precedence precedence() const { return CMP_PRECEDENCE; }
  bool fix_fields(THD *thd, Item **ref);
  bool fix_length_and_dec()
  {
    max_length= 1;
    return agg_arg_charsets_for_comparison(cmp_collation, args, 2);
  }
  void cleanup();

  Item *neg_transformer(THD *thd)
  {
    negated= !negated;
    return this;
  }

  bool find_selective_predicates_list_processor(void *arg);
  
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_like>(thd, this); }
};


class Regexp_processor_pcre
{
  pcre *m_pcre;
  pcre_extra m_pcre_extra;
  bool m_conversion_is_needed;
  bool m_is_const;
  int m_library_flags;
  CHARSET_INFO *m_data_charset;
  CHARSET_INFO *m_library_charset;
  String m_prev_pattern;
  int m_pcre_exec_rc;
  int m_SubStrVec[30];
  void pcre_exec_warn(int rc) const;
  int pcre_exec_with_warn(const pcre *code, const pcre_extra *extra,
                          const char *subject, int length, int startoffset,
                          int options, int *ovector, int ovecsize);
public:
  String *convert_if_needed(String *src, String *converter);
  String subject_converter;
  String pattern_converter;
  String replace_converter;
  Regexp_processor_pcre() :
    m_pcre(NULL), m_conversion_is_needed(true), m_is_const(0),
    m_library_flags(0),
    m_data_charset(&my_charset_utf8_general_ci),
    m_library_charset(&my_charset_utf8_general_ci)
  {
    m_pcre_extra.flags= PCRE_EXTRA_MATCH_LIMIT_RECURSION;
    m_pcre_extra.match_limit_recursion= 100L;
  }
  int default_regex_flags();
  void set_recursion_limit(THD *);
  void init(CHARSET_INFO *data_charset, int extra_flags)
  {
    m_library_flags= default_regex_flags() | extra_flags |
                    (data_charset != &my_charset_bin ?
                     (PCRE_UTF8 | PCRE_UCP) : 0) |
                    ((data_charset->state &
                     (MY_CS_BINSORT | MY_CS_CSSORT)) ? 0 : PCRE_CASELESS);

    
    m_library_charset= data_charset == &my_charset_bin ?
                       &my_charset_bin : &my_charset_utf8_general_ci;

    m_conversion_is_needed= (data_charset != &my_charset_bin) &&
                            !my_charset_same(data_charset, m_library_charset);
  }
  void fix_owner(Item_func *owner, Item *subject_arg, Item *pattern_arg);
  bool compile(String *pattern, bool send_error);
  bool compile(Item *item, bool send_error);
  bool recompile(Item *item)
  {
    return !m_is_const && compile(item, false);
  }
  bool exec(const char *str, size_t length, size_t offset);
  bool exec(String *str, int offset, uint n_result_offsets_to_convert);
  bool exec(Item *item, int offset, uint n_result_offsets_to_convert);
  bool match() const { return m_pcre_exec_rc < 0 ? 0 : 1; }
  int nsubpatterns() const { return m_pcre_exec_rc <= 0 ? 0 : m_pcre_exec_rc; }
  int subpattern_start(int n) const
  {
    return m_pcre_exec_rc <= 0 ? 0 : m_SubStrVec[n * 2];
  }
  int subpattern_end(int n) const
  {
    return m_pcre_exec_rc <= 0 ? 0 : m_SubStrVec[n * 2 + 1];
  }
  int subpattern_length(int n) const
  {
    return subpattern_end(n) - subpattern_start(n);
  }
  void reset()
  {
    m_pcre= NULL;
    m_prev_pattern.length(0);
  }
  void cleanup()
  {
    pcre_free(m_pcre);
    reset();
  }
  bool is_compiled() const { return m_pcre != NULL; }
  bool is_const() const { return m_is_const; }
  void set_const(bool arg) { m_is_const= arg; }
  CHARSET_INFO * library_charset() const { return m_library_charset; }
};


class Item_func_regex :public Item_bool_func
{
  Regexp_processor_pcre re;
  DTCollation cmp_collation;
public:
  Item_func_regex(THD *thd, Item *a, Item *b): Item_bool_func(thd, a, b)
  {}
  void cleanup()
  {
    DBUG_ENTER("Item_func_regex::cleanup");
    Item_bool_func::cleanup();
    re.cleanup();
    DBUG_VOID_RETURN;
  }
  longlong val_int();
  bool fix_fields(THD *thd, Item **ref);
  bool fix_length_and_dec();
  const char *func_name() const { return "regexp"; }
  enum precedence precedence() const { return CMP_PRECEDENCE; }
  Item *get_copy(THD *) { return 0; }
  void print(String *str, enum_query_type query_type)
  {
    print_op(str, query_type);
  }

  CHARSET_INFO *compare_collation() const { return cmp_collation.collation; }
};



class Item_func_regexp_instr :public Item_long_func
{
  bool check_arguments() const
  {
    return args[0]->check_type_can_return_str(func_name()) ||
           args[1]->check_type_can_return_text(func_name());
  }
  Regexp_processor_pcre re;
  DTCollation cmp_collation;
public:
  Item_func_regexp_instr(THD *thd, Item *a, Item *b)
   :Item_long_func(thd, a, b)
  {}
  void cleanup()
  {
    DBUG_ENTER("Item_func_regexp_instr::cleanup");
    Item_int_func::cleanup();
    re.cleanup();
    DBUG_VOID_RETURN;
  }
  longlong val_int();
  bool fix_fields(THD *thd, Item **ref);
  bool fix_length_and_dec();
  const char *func_name() const { return "regexp_instr"; }
  Item *get_copy(THD *thd) { return 0; }
};


typedef class Item COND;

class Item_cond :public Item_bool_func
{
protected:
  List<Item> list;
  bool abort_on_null;
  table_map and_tables_cache;

public:
  
  Item_cond(THD *thd): Item_bool_func(thd), abort_on_null(1)
  { const_item_cache=0; }
  Item_cond(THD *thd, Item *i1, Item *i2);
  Item_cond(THD *thd, Item_cond *item);
  Item_cond(THD *thd, List<Item> &nlist):
    Item_bool_func(thd), list(nlist), abort_on_null(0) {}
  bool add(Item *item, MEM_ROOT *root)
  {
    DBUG_ASSERT(item);
    return list.push_back(item, root);
  }
  bool add_at_head(Item *item, MEM_ROOT *root)
  {
    DBUG_ASSERT(item);
    return list.push_front(item, root);
  }
  void add_at_head(List<Item> *nlist)
  {
    DBUG_ASSERT(nlist->elements);
    list.prepend(nlist);
  }
  void add_at_end(List<Item> *nlist)
  {
    DBUG_ASSERT(nlist->elements);
    list.append(nlist);
  }
  bool fix_fields(THD *, Item **ref);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);

  enum Type type() const { return COND_ITEM; }
  List<Item>* argument_list() { return &list; }
  table_map used_tables() const;
  void update_used_tables()
  {
    used_tables_and_const_cache_init();
    used_tables_and_const_cache_update_and_join(list);
  }
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref);
  COND *remove_eq_conds(THD *thd, Item::cond_result *cond_value,
                        bool top_level);
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  virtual void print(String *str, enum_query_type query_type);
  void split_sum_func(THD *thd, Ref_ptr_array ref_pointer_array,
                      List<Item> &fields, uint flags);
  friend int setup_conds(THD *thd, TABLE_LIST *tables, TABLE_LIST *leaves,
                         COND **conds);
  void top_level_item() { abort_on_null=1; }
  bool top_level() { return abort_on_null; }
  void copy_andor_arguments(THD *thd, Item_cond *item);
  bool walk(Item_processor processor, bool walk_subquery, void *arg);
  Item *transform(THD *thd, Item_transformer transformer, uchar *arg);
  void traverse_cond(Cond_traverser, void *arg, traverse_order order);
  void neg_arguments(THD *thd);
  Item* propagate_equal_fields(THD *, const Context &, COND_EQUAL *);
  Item *compile(THD *thd, Item_analyzer analyzer, uchar **arg_p,
                Item_transformer transformer, uchar *arg_t);
  bool eval_not_null_tables(void *opt_arg);
  Item *build_clone(THD *thd);
  bool excl_dep_on_table(table_map tab_map);
  bool excl_dep_on_grouping_fields(st_select_lex *sel);
};

template <template<class> class LI, class T> class Item_equal_iterator;



class Item_equal: public Item_bool_func
{
   
  List<Item> equal_items; 
  
  bool with_const;        
    
  cmp_item *eval_item;
  
  bool cond_false;
  
  bool cond_true;
  
  Item_field *context_field;

  bool link_equal_fields;

  const Type_handler *m_compare_handler;
  CHARSET_INFO *m_compare_collation;
  String cmp_value1, cmp_value2;
public:

  COND_EQUAL *upper_levels;       

  Item_equal(THD *thd, const Type_handler *handler,
             Item *f1, Item *f2, bool with_const_item);
  Item_equal(THD *thd, Item_equal *item_equal);
  
  inline Item* get_const() { return with_const ? equal_items.head() : NULL; }
  void add_const(THD *thd, Item *c);
  
  void add(Item *f, MEM_ROOT *root) { equal_items.push_back(f, root); }
  bool contains(Field *field);
  Item* get_first(struct st_join_table *context, Item *field);
     
  uint n_field_items() { return equal_items.elements - MY_TEST(with_const); }
  void merge(THD *thd, Item_equal *item);
  bool merge_with_check(THD *thd, Item_equal *equal_item, bool save_merged);
  void merge_into_list(THD *thd, List<Item_equal> *list, bool save_merged,
                      bool only_intersected);
  void update_const(THD *thd);
  enum Functype functype() const { return MULT_EQUAL_FUNC; }
  longlong val_int(); 
  const char *func_name() const { return "multiple equal"; }
  void sort(Item_field_cmpfunc compare, void *arg);
  bool fix_length_and_dec();
  bool fix_fields(THD *thd, Item **ref);
  void cleanup()
  {
    delete eval_item;
    eval_item= NULL;
  }
  void update_used_tables();
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref);
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields,
                      uint *and_level, table_map usable_tables,
                      SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  bool walk(Item_processor processor, bool walk_subquery, void *arg);
  Item *transform(THD *thd, Item_transformer transformer, uchar *arg);
  virtual void print(String *str, enum_query_type query_type);
  const Type_handler *compare_type_handler() const { return m_compare_handler; }
  CHARSET_INFO *compare_collation() const { return m_compare_collation; }

  void set_context_field(Item_field *ctx_field) { context_field= ctx_field; }
  void set_link_equal_fields(bool flag) { link_equal_fields= flag; }
  Item* get_copy(THD *thd) { return 0; }
  
  bool excl_dep_on_table(table_map tab_map)
  {
    return used_tables() & tab_map;
  }
  friend class Item_equal_fields_iterator;
  bool count_sargable_conds(void *arg);
  friend class Item_equal_iterator<List_iterator_fast,Item>;
  friend class Item_equal_iterator<List_iterator,Item>;
  friend Item *eliminate_item_equal(THD *thd, COND *cond,
                                    COND_EQUAL *upper_levels,
                                    Item_equal *item_equal);
  friend bool setup_sj_materialization_part1(struct st_join_table *tab);
  friend bool setup_sj_materialization_part2(struct st_join_table *tab);
}; 

class COND_EQUAL: public Sql_alloc
{
public:
  uint max_members;                
  COND_EQUAL *upper_levels;       
  List<Item_equal> current_level; 
  COND_EQUAL()
  { 
    upper_levels= 0;
  }
  COND_EQUAL(Item_equal *item, MEM_ROOT *mem_root)
   :upper_levels(0)
  {
    current_level.push_back(item, mem_root);
  }
  void copy(COND_EQUAL &cond_equal)
  {
    max_members= cond_equal.max_members;
    upper_levels= cond_equal.upper_levels;
    if (cond_equal.current_level.is_empty())
      current_level.empty();
    else
      current_level= cond_equal.current_level;
  }
};


 

template <template<class> class LI, typename T> class Item_equal_iterator
  : public LI<T>
{
protected:
  Item_equal *item_equal;
  Item *curr_item;
public:
  Item_equal_iterator<LI,T>(Item_equal &item_eq) 
    :LI<T> (item_eq.equal_items)
  {
    curr_item= NULL;
    item_equal= &item_eq;
    if (item_eq.with_const)
    {
      LI<T> *list_it= this;
      curr_item=  (*list_it)++;
    }
  }
  Item* operator++(int)
  { 
    LI<T> *list_it= this;
    curr_item= (*list_it)++;
    return curr_item;
  }
  void rewind(void) 
  { 
    LI<T> *list_it= this;
    list_it->rewind();
    if (item_equal->with_const)
      curr_item= (*list_it)++;
  }  
  Field *get_curr_field()
  {
    Item_field *item= (Item_field *) (curr_item->real_item());
     return item->field;
  }  
};

typedef  Item_equal_iterator<List_iterator_fast,Item >  Item_equal_iterator_fast;

class Item_equal_fields_iterator
  :public Item_equal_iterator_fast
{
public:
  Item_equal_fields_iterator(Item_equal &item_eq) 
    :Item_equal_iterator_fast(item_eq)
  { }
  Item ** ref()
  {
    return List_iterator_fast<Item>::ref();
  }
};

typedef Item_equal_iterator<List_iterator,Item > Item_equal_iterator_iterator_slow;

class Item_equal_fields_iterator_slow
  :public Item_equal_iterator_iterator_slow
{
public:
  Item_equal_fields_iterator_slow(Item_equal &item_eq) 
    :Item_equal_iterator_iterator_slow(item_eq)
  { }
  void remove()
  {
    List_iterator<Item>::remove();
  }
};


class Item_cond_and :public Item_cond
{
public:
  COND_EQUAL m_cond_equal;    
  Item_cond_and(THD *thd): Item_cond(thd) {}
  Item_cond_and(THD *thd, Item *i1,Item *i2): Item_cond(thd, i1, i2) {}
  Item_cond_and(THD *thd, Item_cond_and *item): Item_cond(thd, item) {}
  Item_cond_and(THD *thd, List<Item> &list_arg): Item_cond(thd, list_arg) {}
  enum Functype functype() const { return COND_AND_FUNC; }
  longlong val_int();
  const char *func_name() const { return "and"; }
  enum precedence precedence() const { return AND_PRECEDENCE; }
  table_map not_null_tables() const
  { return abort_on_null ? not_null_tables_cache: and_tables_cache; }
  Item *copy_andor_structure(THD *thd);
  Item *neg_transformer(THD *thd);
  void mark_as_condition_AND_part(TABLE_LIST *embedding);
  virtual uint exists2in_reserved_items() { return list.elements; };
  COND *build_equal_items(THD *thd, COND_EQUAL *inherited,
                          bool link_item_fields,
                          COND_EQUAL **cond_equal_ref);
  void add_key_fields(JOIN *join, KEY_FIELD **key_fields, uint *and_level,
                      table_map usable_tables, SARGABLE_PARAM **sargables);
  SEL_TREE *get_mm_tree(RANGE_OPT_PARAM *param, Item **cond_ptr);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cond_and>(thd, this); }
};

inline bool is_cond_and(Item *item)
{
  if (item->type() != Item::COND_ITEM)
    return FALSE;

  Item_cond *cond_item= (Item_cond*) item;
  return (cond_item->functype() == Item_func::COND_AND_FUNC);
}

class Item_cond_or :public Item_cond
{
public:
  Item_cond_or(THD *thd): Item_cond(thd) {}
  Item_cond_or(THD *thd, Item *i1,Item *i2): Item_cond(thd, i1, i2) {}
  Item_cond_or(THD *thd, Item_cond_or *item): Item_cond(thd, item) {}
  Item_cond_or(THD *thd, List<Item> &list_arg): Item_cond(thd, list_arg) {}
  enum Functype functype() const { return COND_OR_FUNC; }
  longlong val_int();
  const char *func_name() const { return "or"; }
  enum precedence precedence() const { return OR_PRECEDENCE; }
  table_map not_null_tables() const { return and_tables_cache; }
  Item *copy_andor_structure(THD *thd);
  Item *neg_transformer(THD *thd);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_cond_or>(thd, this); }
};

class Item_func_dyncol_check :public Item_bool_func
{
public:
  Item_func_dyncol_check(THD *thd, Item *str): Item_bool_func(thd, str) {}
  longlong val_int();
  const char *func_name() const { return "column_check"; }
  bool need_parentheses_in_default() { return false; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_dyncol_check>(thd, this); }
};

class Item_func_dyncol_exists :public Item_bool_func
{
public:
  Item_func_dyncol_exists(THD *thd, Item *str, Item *num):
    Item_bool_func(thd, str, num) {}
  longlong val_int();
  const char *func_name() const { return "column_exists"; }
  bool need_parentheses_in_default() { return false; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_dyncol_exists>(thd, this); }
};


class Item_func_cursor_bool_attr: public Item_bool_func, public Cursor_ref
{
public:
  Item_func_cursor_bool_attr(THD *thd, const LEX_CSTRING *name, uint offset)
   :Item_bool_func(thd), Cursor_ref(name, offset)
  { }
  bool check_vcol_func_processor(void *arg)
  {
    return mark_unsupported_function(func_name(), arg, VCOL_SESSION_FUNC);
  }
  void print(String *str, enum_query_type query_type)
  {
    Cursor_ref::print_func(str, func_name());
  }
};


class Item_func_cursor_isopen: public Item_func_cursor_bool_attr
{
public:
  Item_func_cursor_isopen(THD *thd, const LEX_CSTRING *name, uint offset)
   :Item_func_cursor_bool_attr(thd, name, offset) { }
  const char *func_name() const { return "%ISOPEN"; }
  longlong val_int();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_cursor_isopen>(thd, this); }
};


class Item_func_cursor_found: public Item_func_cursor_bool_attr
{
public:
  Item_func_cursor_found(THD *thd, const LEX_CSTRING *name, uint offset)
   :Item_func_cursor_bool_attr(thd, name, offset) { maybe_null= true; }
  const char *func_name() const { return "%FOUND"; }
  longlong val_int();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_cursor_found>(thd, this); }
};


class Item_func_cursor_notfound: public Item_func_cursor_bool_attr
{
public:
  Item_func_cursor_notfound(THD *thd, const LEX_CSTRING *name, uint offset)
   :Item_func_cursor_bool_attr(thd, name, offset) { maybe_null= true; }
  const char *func_name() const { return "%NOTFOUND"; }
  longlong val_int();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_cursor_notfound>(thd, this); }
};



inline bool is_cond_or(Item *item)
{
  if (item->type() != Item::COND_ITEM)
    return FALSE;

  Item_cond *cond_item= (Item_cond*) item;
  return (cond_item->functype() == Item_func::COND_OR_FUNC);
}

Item *and_expressions(Item *a, Item *b, Item **org_item);

class Comp_creator
{
public:
  Comp_creator() {}                           
  virtual ~Comp_creator() {}                  
  
  virtual Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b)
                                           const = 0;
  
  virtual Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b)
                                                const = 0;
  virtual const char* symbol(bool invert) const = 0;
  virtual bool eqne_op() const = 0;
  virtual bool l_op() const = 0;
};

class Eq_creator :public Comp_creator
{
public:
  Eq_creator() {}                             
  virtual ~Eq_creator() {}                    
  Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b) const;
  Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b) const;
  const char* symbol(bool invert) const { return invert? "<>" : "="; }
  bool eqne_op() const { return 1; }
  bool l_op() const { return 0; }
};

class Ne_creator :public Comp_creator
{
public:
  Ne_creator() {}                             
  virtual ~Ne_creator() {}                    
  Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b) const;
  Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b) const;
  const char* symbol(bool invert) const { return invert? "=" : "<>"; }
  bool eqne_op() const { return 1; }
  bool l_op() const { return 0; }
};

class Gt_creator :public Comp_creator
{
public:
  Gt_creator() {}                             
  virtual ~Gt_creator() {}                    
  Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b) const;
  Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b) const;
  const char* symbol(bool invert) const { return invert? "<=" : ">"; }
  bool eqne_op() const { return 0; }
  bool l_op() const { return 0; }
};

class Lt_creator :public Comp_creator
{
public:
  Lt_creator() {}                             
  virtual ~Lt_creator() {}                    
  Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b) const;
  Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b) const;
  const char* symbol(bool invert) const { return invert? ">=" : "<"; }
  bool eqne_op() const { return 0; }
  bool l_op() const { return 1; }
};

class Ge_creator :public Comp_creator
{
public:
  Ge_creator() {}                             
  virtual ~Ge_creator() {}                    
  Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b) const;
  Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b) const;
  const char* symbol(bool invert) const { return invert? "<" : ">="; }
  bool eqne_op() const { return 0; }
  bool l_op() const { return 0; }
};

class Le_creator :public Comp_creator
{
public:
  Le_creator() {}                             
  virtual ~Le_creator() {}                    
  Item_bool_rowready_func2* create(THD *thd, Item *a, Item *b) const;
  Item_bool_rowready_func2* create_swap(THD *thd, Item *a, Item *b) const;
  const char* symbol(bool invert) const { return invert? ">" : "<="; }
  bool eqne_op() const { return 0; }
  bool l_op() const { return 1; }
};


extern Eq_creator eq_creator;
extern Ne_creator ne_creator;
extern Gt_creator gt_creator;
extern Lt_creator lt_creator;
extern Ge_creator ge_creator;
extern Le_creator le_creator;

#endif 
