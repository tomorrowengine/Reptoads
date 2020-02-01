#ifndef ITEM_SUBSELECT_INCLUDED
#define ITEM_SUBSELECT_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include <queues.h>

class st_select_lex;
class st_select_lex_unit;
class JOIN;
class select_result_interceptor;
class subselect_engine;
class subselect_hash_sj_engine;
class Item_bool_func2;
class Comp_creator;
class With_element;

typedef class st_select_lex SELECT_LEX;


typedef Comp_creator* (*chooser_compare_func_creator)(bool invert);
class Cached_item;



class Item_subselect :public Item_result_field,
                      protected Used_tables_and_const_cache
{
  bool value_assigned;   
  bool own_engine;  
protected:
  
  THD *thd;
  
  subselect_engine *old_engine;
  
  uint max_columns;
  
  enum_parsing_place parsing_place;
  
  bool have_to_be_excluded;
  
  bool inside_first_fix_fields;
  bool done_first_fix_fields;
  Item *expr_cache;
  
  bool forced_const;
#ifndef DBUG_OFF
  
  uint exec_counter;
#endif
public:
  
  
  Item *substitution;
  
  
  subselect_engine *engine;
  
  st_select_lex_unit *unit;
  
  Filesort_buffer filesort_buffer;
  LEX_STRING sortbuffer;
  
  class Ref_to_outside : public Sql_alloc
  {
  public:
    st_select_lex *select; 
     
    Item *item; 
  };
  
  List<Ref_to_outside> upper_refs;
  st_select_lex *parent_select;

  
  bool eliminated;
  
  
  bool changed;

  
  bool is_correlated;

  
  bool with_recursive_reference; 

   
  Item_subselect *next_with_rec_ref;

  enum subs_type {UNKNOWN_SUBS, SINGLEROW_SUBS,
		  EXISTS_SUBS, IN_SUBS, ALL_SUBS, ANY_SUBS};

  Item_subselect(THD *thd);

  virtual subs_type substype() { return UNKNOWN_SUBS; }
  bool is_in_predicate()
  {
    return (substype() == Item_subselect::IN_SUBS ||
            substype() == Item_subselect::ALL_SUBS ||
            substype() == Item_subselect::ANY_SUBS);
  }

  
  virtual void init (st_select_lex *select_lex,
		     select_result_interceptor *result);

  ~Item_subselect();
  void cleanup();
  virtual void reset()
  {
    eliminated= FALSE;
    null_value= 1;
  }
  
  void no_rows_in_result()= 0;
  virtual bool select_transformer(JOIN *join);
  bool assigned() { return value_assigned; }
  void assigned(bool a) { value_assigned= a; }
  enum Type type() const;
  bool is_null()
  {
    update_null_value();
    return null_value;
  }
  bool fix_fields(THD *thd, Item **ref);
  bool with_subquery() const { DBUG_ASSERT(fixed); return true; }
  bool mark_as_dependent(THD *thd, st_select_lex *select, Item *item);
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  void recalc_used_tables(st_select_lex *new_parent, bool after_pullout);
  virtual bool exec();
  
  void make_const()
  { 
    used_tables_cache= 0;
    const_item_cache= 0;
    forced_const= TRUE; 
  }
  virtual bool fix_length_and_dec();
  table_map used_tables() const;
  table_map not_null_tables() const { return 0; }
  bool const_item() const;
  inline table_map get_used_tables_cache() { return used_tables_cache; }
  Item *get_tmp_table_item(THD *thd);
  void update_used_tables();
  virtual void print(String *str, enum_query_type query_type);
  virtual bool have_guarded_conds() { return FALSE; }
  bool change_engine(subselect_engine *eng)
  {
    old_engine= engine;
    engine= eng;
    return eng == 0;
  }
  bool engine_changed(subselect_engine *eng) { return engine != eng; }
  
  bool is_evaluated() const;
  bool is_uncacheable() const;
  bool is_expensive();

  
  virtual void reset_value_registration() {}
  enum_parsing_place place() { return parsing_place; }
  bool walk(Item_processor processor, bool walk_subquery, void *arg);
  bool mark_as_eliminated_processor(void *arg);
  bool eliminate_subselect_processor(void *arg);
  bool set_fake_select_as_master_processor(void *arg);
  bool enumerate_field_refs_processor(void *arg);
  bool check_vcol_func_processor(void *arg) 
  {
    return mark_unsupported_function("select ...", arg, VCOL_IMPOSSIBLE);
  }
  
  bool is_expensive_processor(void *arg) { return is_expensive(); }

  
  st_select_lex* get_select_lex();
  virtual bool expr_cache_is_needed(THD *);
  virtual void get_cache_parameters(List<Item> &parameters);
  virtual bool is_subquery_processor (void *opt_arg) { return 1; }
  bool exists2in_processor(void *opt_arg) { return 0; }
  bool limit_index_condition_pushdown_processor(void *opt_arg) 
  {
    return TRUE;
  }

  void register_as_with_rec_ref(With_element *with_elem);
  void init_expr_cache_tracker(THD *thd);
  
  Item* build_clone(THD *thd) { return 0; }
  Item* get_copy(THD *thd) { return 0; }

  bool wrap_tvc_in_derived_table(THD *thd, st_select_lex *tvc_sl);

  friend class select_result_interceptor;
  friend class Item_in_optimizer;
  friend bool Item_field::fix_fields(THD *, Item **);
  friend int  Item_field::fix_outer_field(THD *, Field **, Item **);
  friend bool Item_ref::fix_fields(THD *, Item **);
  friend void mark_select_range_as_dependent(THD*,
                                             st_select_lex*, st_select_lex*,
                                             Field*, Item*, Item_ident*);
  friend bool convert_join_subqueries_to_semijoins(JOIN *join);
};



class Item_cache;
class Item_singlerow_subselect :public Item_subselect
{
protected:
  Item_cache *value, **row;
public:
  Item_singlerow_subselect(THD *thd_arg, st_select_lex *select_lex);
  Item_singlerow_subselect(THD *thd_arg): Item_subselect(thd_arg), value(0), row (0)
  {}

  void cleanup();
  subs_type substype() { return SINGLEROW_SUBS; }

  void reset();
  void no_rows_in_result();
  bool select_transformer(JOIN *join);
  void store(uint i, Item* item);
  double val_real();
  longlong val_int ();
  String *val_str (String *);
  my_decimal *val_decimal(my_decimal *);
  bool val_bool();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate);
  const Type_handler *type_handler() const;
  bool fix_length_and_dec();

  uint cols() const;
  Item* element_index(uint i) { return reinterpret_cast<Item*>(row[i]); }
  Item** addr(uint i) { return (Item**)row + i; }
  bool check_cols(uint c);
  bool null_inside();
  void bring_value();

  
  st_select_lex* invalidate_and_restore_select_lex();

  Item* expr_cache_insert_transformer(THD *thd, uchar *unused);

  friend class select_singlerow_subselect;
};


class select_max_min_finder_subselect;
class Item_maxmin_subselect :public Item_singlerow_subselect
{
protected:
  bool max;
  bool was_values;  
public:
  Item_maxmin_subselect(THD *thd, Item_subselect *parent,
			st_select_lex *select_lex, bool max);
  virtual void print(String *str, enum_query_type query_type);
  void cleanup();
  bool any_value() { return was_values; }
  void register_value() { was_values= TRUE; }
  void reset_value_registration() { was_values= FALSE; }
  void no_rows_in_result();
};



class Item_exists_subselect :public Item_subselect
{
protected:
  Item_func_not *upper_not;
  bool value; 
  bool abort_on_null;

  void init_length_and_dec();
  bool select_prepare_to_be_in();

public:
  
  TABLE_LIST *emb_on_expr_nest;
  
  Item_in_optimizer *optimizer;
  
  bool exists_transformed;

  Item_exists_subselect(THD *thd_arg, st_select_lex *select_lex);
  Item_exists_subselect(THD *thd_arg):
    Item_subselect(thd_arg), upper_not(NULL), abort_on_null(0),
    emb_on_expr_nest(NULL), optimizer(0), exists_transformed(0)
  {}

  subs_type substype() { return EXISTS_SUBS; }
  void reset() 
  {
    eliminated= FALSE;
    value= 0;
  }
  void no_rows_in_result();

  const Type_handler *type_handler() const { return &type_handler_longlong; }
  longlong val_int();
  double val_real();
  String *val_str(String*);
  my_decimal *val_decimal(my_decimal *);
  bool val_bool();
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  { return get_date_from_int(ltime, fuzzydate); }
  bool fix_fields(THD *thd, Item **ref);
  bool fix_length_and_dec();
  void print(String *str, enum_query_type query_type);
  bool select_transformer(JOIN *join);
  void top_level_item() { abort_on_null=1; }
  inline bool is_top_level_item() { return abort_on_null; }
  bool exists2in_processor(void *opt_arg);

  Item* expr_cache_insert_transformer(THD *thd, uchar *unused);

  void mark_as_condition_AND_part(TABLE_LIST *embedding)
  {
    emb_on_expr_nest= embedding;
  }
  virtual void under_not(Item_func_not *upper) { upper_not= upper; };

  void set_exists_transformed() { exists_transformed= TRUE; }

  friend class select_exists_subselect;
  friend class subselect_uniquesubquery_engine;
  friend class subselect_indexsubquery_engine;
};


TABLE_LIST * const NO_JOIN_NEST=(TABLE_LIST*)0x1;


#define SUBS_NOT_TRANSFORMED 0 

#define SUBS_STRATEGY_CHOSEN 1
#define SUBS_SEMI_JOIN 2       
#define SUBS_IN_TO_EXISTS 4    
#define SUBS_MATERIALIZATION 8 

#define SUBS_PARTIAL_MATCH_ROWID_MERGE 16
#define SUBS_PARTIAL_MATCH_TABLE_SCAN 32


#define SUBS_MAXMIN_INJECTED 64

#define SUBS_MAXMIN_ENGINE 128




class Item_in_subselect :public Item_exists_subselect
{
protected:
  
  List<Cached_item> *left_expr_cache;
  bool first_execution;

  
  Item *expr;
  bool was_null;
  
  uchar in_strategy;
protected:
  
  bool *pushed_cond_guards;
  Comp_creator *func;

protected:
  bool init_cond_guards();
  bool select_in_like_transformer(JOIN *join);
  bool single_value_transformer(JOIN *join);
  bool row_value_transformer(JOIN * join);
  bool fix_having(Item *having, st_select_lex *select_lex);
  bool create_single_in_to_exists_cond(JOIN * join,
                                       Item **where_item,
                                       Item **having_item);
  bool create_row_in_to_exists_cond(JOIN * join,
                                    Item **where_item,
                                    Item **having_item);
public:
  Item *left_expr;
  
  Item *left_expr_orig;
  
  int sj_convert_priority;
  
  bool do_not_convert_to_sj; 
  
  bool types_allow_materialization;

  
  bool sjm_scan_allowed;

  
  
  
  bool is_jtbm_merged;
  
  
  double jtbm_read_time;

  
  double jtbm_record_count;   
  
  
  bool is_jtbm_const_tab;
  
  
  bool jtbm_const_row_found;
  
  
  bool is_flattenable_semijoin;

  
  bool is_registered_semijoin;
  
  
  Item *original_item()
  {
    return (is_flattenable_semijoin && !exists_transformed ?
            (Item*)this :
            (Item*)optimizer);
  }
  
  bool *get_cond_guard(int i)
  {
    return pushed_cond_guards ? pushed_cond_guards + i : NULL;
  }
  void set_cond_guard_var(int i, bool v) 
  { 
    if ( pushed_cond_guards)
      pushed_cond_guards[i]= v;
  }
  bool have_guarded_conds() { return MY_TEST(pushed_cond_guards); }

  Item_func_not_all *upper_item; 

  Item_in_subselect(THD *thd_arg, Item * left_expr, st_select_lex *select_lex);
  Item_in_subselect(THD *thd_arg):
    Item_exists_subselect(thd_arg), left_expr_cache(0), first_execution(TRUE),
    in_strategy(SUBS_NOT_TRANSFORMED),
    pushed_cond_guards(NULL), func(NULL), do_not_convert_to_sj(FALSE),
    is_jtbm_merged(FALSE), is_jtbm_const_tab(FALSE), upper_item(0) {}
  void cleanup();
  subs_type substype() { return IN_SUBS; }
  void reset() 
  {
    eliminated= FALSE;
    value= 0;
    null_value= 0;
    was_null= 0;
  }
  bool select_transformer(JOIN *join);
  bool create_in_to_exists_cond(JOIN *join_arg);
  bool inject_in_to_exists_cond(JOIN *join_arg);

  virtual bool exec();
  longlong val_int();
  double val_real();
  String *val_str(String*);
  my_decimal *val_decimal(my_decimal *);
  void update_null_value () { (void) val_bool(); }
  bool val_bool();
  bool test_limit(st_select_lex_unit *unit);
  void print(String *str, enum_query_type query_type);
  enum precedence precedence() const { return CMP_PRECEDENCE; }
  bool fix_fields(THD *thd, Item **ref);
  bool fix_length_and_dec();
  void fix_after_pullout(st_select_lex *new_parent, Item **ref, bool merge);
  bool const_item() const
  {
    return Item_subselect::const_item() && left_expr->const_item();
  }
  void update_used_tables();
  bool setup_mat_engine();
  bool init_left_expr_cache();
  
  void set_first_execution() { if (first_execution) first_execution= FALSE; }
  bool expr_cache_is_needed(THD *thd);
  inline bool left_expr_has_null();

  void disable_cond_guard_for_const_null_left_expr(int i)
  {
    if (left_expr->const_item() && !left_expr->is_expensive())
    {
      if (left_expr->element_index(i)->is_null())
        set_cond_guard_var(i,FALSE);
    }
  }

  int optimize(double *out_rows, double *cost);
  
  int get_identifier();

  void block_conversion_to_sj () { do_not_convert_to_sj= TRUE; }

  bool test_strategy(uchar strategy)
  { return MY_TEST(in_strategy & strategy); }

  
  bool test_set_strategy(uchar strategy)
  {
    DBUG_ASSERT(strategy == SUBS_SEMI_JOIN ||
                strategy == SUBS_IN_TO_EXISTS ||
                strategy == SUBS_MATERIALIZATION ||
                strategy == SUBS_PARTIAL_MATCH_ROWID_MERGE ||
                strategy == SUBS_PARTIAL_MATCH_TABLE_SCAN ||
                strategy == SUBS_MAXMIN_INJECTED ||
                strategy == SUBS_MAXMIN_ENGINE);
    return ((in_strategy & SUBS_STRATEGY_CHOSEN) &&
            (in_strategy & ~SUBS_STRATEGY_CHOSEN) == strategy);
  }

  bool is_set_strategy()
  { return MY_TEST(in_strategy & SUBS_STRATEGY_CHOSEN); }

  bool has_strategy()
  { return in_strategy != SUBS_NOT_TRANSFORMED; }

  void add_strategy (uchar strategy)
  {
    DBUG_ENTER("Item_in_subselect::add_strategy");
    DBUG_PRINT("enter", ("current: %u  add: %u",
                         (uint) in_strategy, (uint) strategy));
    DBUG_ASSERT(strategy != SUBS_NOT_TRANSFORMED);
    DBUG_ASSERT(!(strategy & SUBS_STRATEGY_CHOSEN));
    
    in_strategy|= strategy;
    DBUG_VOID_RETURN;
  }

  void reset_strategy(uchar strategy)
  {
    DBUG_ENTER("Item_in_subselect::reset_strategy");
    DBUG_PRINT("enter", ("current: %u  new: %u",
                         (uint) in_strategy, (uint) strategy));
    DBUG_ASSERT(strategy != SUBS_NOT_TRANSFORMED);
    in_strategy= strategy;
    DBUG_VOID_RETURN;
  }

  void set_strategy(uchar strategy)
  {
    DBUG_ENTER("Item_in_subselect::set_strategy");
    DBUG_PRINT("enter", ("current: %u  set: %u",
                         (uint) in_strategy,
                         (uint) (SUBS_STRATEGY_CHOSEN | strategy)));
    
    DBUG_ASSERT(strategy == SUBS_SEMI_JOIN ||
                strategy == SUBS_IN_TO_EXISTS ||
                strategy == SUBS_MATERIALIZATION ||
                strategy == SUBS_PARTIAL_MATCH_ROWID_MERGE ||
                strategy == SUBS_PARTIAL_MATCH_TABLE_SCAN ||
                strategy == SUBS_MAXMIN_INJECTED ||
                strategy == SUBS_MAXMIN_ENGINE);
    in_strategy= (SUBS_STRATEGY_CHOSEN | strategy);
    DBUG_VOID_RETURN;
  }

  bool walk(Item_processor processor, bool walk_subquery, void *arg)
  {
    return left_expr->walk(processor, walk_subquery, arg) ||
           Item_subselect::walk(processor, walk_subquery, arg);
  }

  bool exists2in_processor(void *opt_arg __attribute__((unused)))
  {
    return 0;
  };

  friend class Item_ref_null_helper;
  friend class Item_is_not_null_test;
  friend class Item_in_optimizer;
  friend class subselect_indexsubquery_engine;
  friend class subselect_hash_sj_engine;
  friend class subselect_partial_match_engine;
  friend class Item_exists_subselect;
};



class Item_allany_subselect :public Item_in_subselect
{
public:
  chooser_compare_func_creator func_creator;
  bool all;

  Item_allany_subselect(THD *thd_arg, Item * left_expr,
                        chooser_compare_func_creator fc,
                        st_select_lex *select_lex, bool all);

  void cleanup();
  
  subs_type substype() { return all?ALL_SUBS:ANY_SUBS; }
  bool select_transformer(JOIN *join);
  void create_comp_func(bool invert) { func= func_creator(invert); }
  void print(String *str, enum_query_type query_type);
  bool is_maxmin_applicable(JOIN *join);
  bool transform_into_max_min(JOIN *join);
  void no_rows_in_result();
};


class subselect_engine: public Sql_alloc,
                        public Type_handler_hybrid_field_type
{
protected:
  select_result_interceptor *result; 
  THD *thd; 
  Item_subselect *item; 
  bool maybe_null; 
public:

  enum enum_engine_type {ABSTRACT_ENGINE, SINGLE_SELECT_ENGINE,
                         UNION_ENGINE, UNIQUESUBQUERY_ENGINE,
                         INDEXSUBQUERY_ENGINE, HASH_SJ_ENGINE,
                         ROWID_MERGE_ENGINE, TABLE_SCAN_ENGINE};

  subselect_engine(Item_subselect *si,
                   select_result_interceptor *res):
    Type_handler_hybrid_field_type(&type_handler_varchar),
    thd(NULL)
  {
    result= res;
    item= si;
    maybe_null= 0;
  }
  virtual ~subselect_engine() {}; 
  virtual void cleanup()= 0;

  
  void set_thd(THD *thd_arg);
  THD * get_thd() { return thd ? thd : current_thd; }
  virtual int prepare(THD *)= 0;
  virtual bool fix_length_and_dec(Item_cache** row)= 0;
  
  virtual int exec()= 0;
  virtual uint cols() const= 0; 
  virtual uint8 uncacheable()= 0; 
  virtual void exclude()= 0;
  virtual bool may_be_null() { return maybe_null; };
  virtual table_map upper_select_const_tables()= 0;
  static table_map calc_const_tables(TABLE_LIST *);
  static table_map calc_const_tables(List<TABLE_LIST> &list);
  virtual void print(String *str, enum_query_type query_type)= 0;
  virtual bool change_result(Item_subselect *si,
                             select_result_interceptor *result,
                             bool temp= FALSE)= 0;
  virtual bool no_tables()= 0;
  virtual bool is_executed() const { return FALSE; }
  
  virtual bool no_rows() = 0;
  virtual enum_engine_type engine_type() { return ABSTRACT_ENGINE; }
  virtual int get_identifier() { DBUG_ASSERT(0); return 0; }
  virtual void force_reexecution() {}
protected:
  bool set_row(List<Item> &item_list, Item_cache **row);
};


class subselect_single_select_engine: public subselect_engine
{
  bool prepared;       
  bool executed;       
  st_select_lex *select_lex; 
  JOIN * join; 
public:
  subselect_single_select_engine(st_select_lex *select,
				 select_result_interceptor *result,
				 Item_subselect *item);
  void cleanup();
  int prepare(THD *thd);
  bool fix_length_and_dec(Item_cache** row);
  int exec();
  uint cols() const;
  uint8 uncacheable();
  void exclude();
  table_map upper_select_const_tables();
  void print (String *str, enum_query_type query_type);
  bool change_result(Item_subselect *si,
                     select_result_interceptor *result,
                     bool temp);
  bool no_tables();
  bool may_be_null();
  bool is_executed() const { return executed; }
  bool no_rows();
  virtual enum_engine_type engine_type() { return SINGLE_SELECT_ENGINE; }
  int get_identifier();
  void force_reexecution();
  void change_select(st_select_lex *new_select) { select_lex= new_select; }

  friend class subselect_hash_sj_engine;
  friend class Item_in_subselect;
  friend bool setup_jtbm_semi_joins(JOIN *join, List<TABLE_LIST> *join_list,
                                    Item **join_where);

};


class subselect_union_engine: public subselect_engine
{
  st_select_lex_unit *unit;  
public:
  subselect_union_engine(st_select_lex_unit *u,
			 select_result_interceptor *result,
			 Item_subselect *item);
  void cleanup();
  int prepare(THD *);
  bool fix_length_and_dec(Item_cache** row);
  int exec();
  uint cols() const;
  uint8 uncacheable();
  void exclude();
  table_map upper_select_const_tables();
  void print (String *str, enum_query_type query_type);
  bool change_result(Item_subselect *si,
                     select_result_interceptor *result,
                     bool temp= FALSE);
  bool no_tables();
  bool is_executed() const;
  void force_reexecution();
  bool no_rows();
  virtual enum_engine_type engine_type() { return UNION_ENGINE; }
};


struct st_join_table;




class subselect_uniquesubquery_engine: public subselect_engine
{
protected:
  st_join_table *tab;
  Item *cond; 
  
  bool empty_result_set;
public:

  
  subselect_uniquesubquery_engine(THD *thd_arg, st_join_table *tab_arg,
				  Item_subselect *subs, Item *where)
    :subselect_engine(subs, 0), tab(tab_arg), cond(where)
  {}
  ~subselect_uniquesubquery_engine();
  void cleanup();
  int prepare(THD *);
  bool fix_length_and_dec(Item_cache** row);
  int exec();
  uint cols() const { return 1; }
  uint8 uncacheable() { return UNCACHEABLE_DEPENDENT_INJECTED; }
  void exclude();
  table_map upper_select_const_tables() { return 0; }
  void print (String *str, enum_query_type query_type);
  bool change_result(Item_subselect *si,
                     select_result_interceptor *result,
                     bool temp= FALSE);
  bool no_tables();
  int index_lookup(); 
  int scan_table();
  bool copy_ref_key(bool skip_constants);
  bool no_rows() { return empty_result_set; }
  virtual enum_engine_type engine_type() { return UNIQUESUBQUERY_ENGINE; }
};


class subselect_indexsubquery_engine: public subselect_uniquesubquery_engine
{
  
  bool check_null;
  
  Item *having;
public:

  
  subselect_indexsubquery_engine(THD *thd_arg, st_join_table *tab_arg,
				 Item_subselect *subs, Item *where,
                                 Item *having_arg, bool chk_null)
    :subselect_uniquesubquery_engine(thd_arg, tab_arg, subs, where),
     check_null(chk_null),
     having(having_arg)
  {}
  int exec();
  void print (String *str, enum_query_type query_type);
  virtual enum_engine_type engine_type() { return INDEXSUBQUERY_ENGINE; }
};


Item * all_any_subquery_creator(THD *thd, Item *left_expr,
                                chooser_compare_func_creator cmp,
                                bool all,
                                SELECT_LEX *select_lex);


inline bool Item_subselect::is_evaluated() const
{
  return engine->is_executed();
}


inline bool Item_subselect::is_uncacheable() const
{
  return engine->uncacheable();
}



class subselect_hash_sj_engine : public subselect_engine
{
public:
  
  TABLE *tmp_table;
  
  bool is_materialized;
  
  subselect_single_select_engine *materialize_engine;
  
  JOIN *materialize_join;
  
  Item_cond_and *semi_join_conds;
  Name_resolution_context *semi_join_conds_context;


  subselect_hash_sj_engine(THD *thd_arg, Item_subselect *in_predicate,
                           subselect_single_select_engine *old_engine)
    : subselect_engine(in_predicate, NULL),
      tmp_table(NULL), is_materialized(FALSE), materialize_engine(old_engine),
      materialize_join(NULL),  semi_join_conds(NULL), lookup_engine(NULL),
      count_partial_match_columns(0), count_null_only_columns(0),
      count_columns_with_nulls(0), strategy(UNDEFINED)
  {}
  ~subselect_hash_sj_engine();

  bool init(List<Item> *tmp_columns, uint subquery_id);
  void cleanup();
  int prepare(THD *);
  int exec();
  void print(String *str, enum_query_type query_type);
  uint cols() const { return materialize_engine->cols(); }
  uint8 uncacheable() { return materialize_engine->uncacheable(); }
  table_map upper_select_const_tables() { return 0; }
  bool no_rows() { return !tmp_table->file->stats.records; }
  virtual enum_engine_type engine_type() { return HASH_SJ_ENGINE; }
  
  bool fix_length_and_dec(Item_cache** row);
  void exclude(); 
  
  bool change_result(Item_subselect *si,
                     select_result_interceptor *result,
                     bool temp= FALSE);
  bool no_tables();

protected:
  
  subselect_engine *lookup_engine;
  
  MY_BITMAP non_null_key_parts;
  
  MY_BITMAP partial_match_key_parts;
  uint count_partial_match_columns;
  uint count_null_only_columns;
  uint count_columns_with_nulls;
  
  enum exec_strategy {
    UNDEFINED,
    COMPLETE_MATCH, 
    PARTIAL_MATCH,  
    PARTIAL_MATCH_MERGE, 
    PARTIAL_MATCH_SCAN,  
    IMPOSSIBLE      
  };
  
  exec_strategy strategy;
  exec_strategy get_strategy_using_schema();
  exec_strategy get_strategy_using_data();
  ulonglong rowid_merge_buff_size(bool has_non_null_key,
                                  bool has_covering_null_row,
                                  MY_BITMAP *partial_match_key_parts);
  void choose_partial_match_strategy(bool has_non_null_key,
                                     bool has_covering_null_row,
                                     MY_BITMAP *partial_match_key_parts);
  bool make_semi_join_conds();
  subselect_uniquesubquery_engine* make_unique_engine();

};



typedef ha_rows rownum_t;




class Ordered_key : public Sql_alloc
{
protected:
  
  uint keyid;
  
  TABLE *tbl;
  
  Item_field **key_columns;
  
  uint key_column_count;
  
  Item *search_key;


  
  rownum_t *key_buff;
  
  ha_rows key_buff_elements;
  
  ha_rows cur_key_idx;
  
  uchar *row_num_to_rowid;
  
  Item_func_lt **compare_pred;


  MY_BITMAP null_key;
  
  ha_rows null_count;
  
  rownum_t min_null_row;
  
  rownum_t max_null_row;

protected:
  bool alloc_keys_buffers();
  
  int cmp_keys_by_row_data(rownum_t a, rownum_t b);
  static int cmp_keys_by_row_data_and_rownum(Ordered_key *key,
                                             rownum_t* a, rownum_t* b);

  int cmp_key_with_search_key(rownum_t row_num);

public:
  Ordered_key(uint keyid_arg, TABLE *tbl_arg,
              Item *search_key_arg, ha_rows null_count_arg,
              ha_rows min_null_row_arg, ha_rows max_null_row_arg,
              uchar *row_num_to_rowid_arg);
  ~Ordered_key();
  void cleanup();
  
  bool init(MY_BITMAP *columns_to_index);
  
  bool init(int col_idx);

  uint get_column_count() { return key_column_count; }
  uint get_keyid() { return keyid; }
  Field *get_field(uint i)
  {
    DBUG_ASSERT(i < key_column_count);
    return key_columns[i]->field;
  }
  rownum_t get_min_null_row() { return min_null_row; }
  rownum_t get_max_null_row() { return max_null_row; }
  MY_BITMAP * get_null_key() { return &null_key; }
  ha_rows get_null_count() { return null_count; }
  
  Item *get_search_key(uint i)
  {
    return search_key->element_index(key_columns[i]->field->field_index);
  }
  void add_key(rownum_t row_num)
  {
    
    DBUG_ASSERT(key_buff_elements && cur_key_idx < key_buff_elements);
    key_buff[cur_key_idx]= row_num;
    ++cur_key_idx;
  }

  void sort_keys();
  double null_selectivity();

  
  bool lookup();
  
  void first()
  {
    DBUG_ASSERT(key_buff_elements);
    cur_key_idx= 0;
  }
  
  bool next_same();
  
  bool next()
  {
    DBUG_ASSERT(key_buff_elements);
    if (cur_key_idx < key_buff_elements - 1)
    {
      ++cur_key_idx;
      return TRUE;
    }
    return FALSE;
  };
  
  rownum_t current()
  {
    DBUG_ASSERT(key_buff_elements && cur_key_idx < key_buff_elements);
    return key_buff[cur_key_idx];
  }

  void set_null(rownum_t row_num)
  {
    bitmap_set_bit(&null_key, (uint)row_num);
  }
  bool is_null(rownum_t row_num)
  {
    
    if (null_count == tbl->file->stats.records)
    {
      DBUG_ASSERT(tbl->file->stats.records == null_key.n_bits);
      return TRUE;
    }
    if (row_num > max_null_row || row_num < min_null_row)
      return FALSE;
    return bitmap_is_set(&null_key, (uint)row_num);
  }
  void print(String *str);
};


class subselect_partial_match_engine : public subselect_engine
{
protected:
  
  TABLE *tmp_table;
  
  subselect_uniquesubquery_engine *lookup_engine;
  
  List<Item> *equi_join_conds;
  
  bool has_covering_null_row;

  
  bool has_covering_null_columns;
  uint count_columns_with_nulls;

protected:
  virtual bool partial_match()= 0;
public:
  subselect_partial_match_engine(subselect_uniquesubquery_engine *engine_arg,
                                 TABLE *tmp_table_arg, Item_subselect *item_arg,
                                 select_result_interceptor *result_arg,
                                 List<Item> *equi_join_conds_arg,
                                 bool has_covering_null_row_arg,
                                 bool has_covering_null_columns_arg,
                                 uint count_columns_with_nulls_arg);
  int prepare(THD *thd_arg) { set_thd(thd_arg); return 0; }
  int exec();
  bool fix_length_and_dec(Item_cache**) { return FALSE; }
  uint cols() const {  return 1; }
  uint8 uncacheable() { return UNCACHEABLE_DEPENDENT; }
  void exclude() {}
  table_map upper_select_const_tables() { return 0; }
  bool change_result(Item_subselect*,
                     select_result_interceptor*,
                     bool temp= FALSE)
  { DBUG_ASSERT(FALSE); return false; }
  bool no_tables() { return false; }
  bool no_rows()
  {
    
    return !(((Item_in_subselect *) item)->null_value);
  }
  void print(String*, enum_query_type);

  friend void subselect_hash_sj_engine::cleanup();
};


class subselect_rowid_merge_engine: public subselect_partial_match_engine
{
protected:
  
  uchar *row_num_to_rowid;
  
  MY_BITMAP matching_keys;
  
  MY_BITMAP matching_outer_cols;
  
  Ordered_key **merge_keys;
  
  uint merge_keys_count;
  
  MY_BITMAP   **null_bitmaps;
  
  Ordered_key *non_null_key;
  
  QUEUE pq;
protected:
  
  static int cmp_keys_by_null_selectivity(Ordered_key **k1, Ordered_key **k2);
  
  static int cmp_keys_by_cur_rownum(void *arg, uchar *k1, uchar *k2);

  bool test_null_row(rownum_t row_num);
  bool exists_complementing_null_row(MY_BITMAP *keys_to_complement);
  bool partial_match();
public:
  subselect_rowid_merge_engine(subselect_uniquesubquery_engine *engine_arg,
                               TABLE *tmp_table_arg, uint merge_keys_count_arg,
                               bool has_covering_null_row_arg,
                               bool has_covering_null_columns_arg,
                               uint count_columns_with_nulls_arg,
                               Item_subselect *item_arg,
                               select_result_interceptor *result_arg,
                               List<Item> *equi_join_conds_arg)
    :subselect_partial_match_engine(engine_arg, tmp_table_arg,
                                    item_arg, result_arg, equi_join_conds_arg,
                                    has_covering_null_row_arg,
                                    has_covering_null_columns_arg,
                                    count_columns_with_nulls_arg),
    merge_keys_count(merge_keys_count_arg), non_null_key(NULL)
  {}
  ~subselect_rowid_merge_engine();
  bool init(MY_BITMAP *non_null_key_parts, MY_BITMAP *partial_match_key_parts);
  void cleanup();
  virtual enum_engine_type engine_type() { return ROWID_MERGE_ENGINE; }
};


class subselect_table_scan_engine: public subselect_partial_match_engine
{
protected:
  bool partial_match();
public:
  subselect_table_scan_engine(subselect_uniquesubquery_engine *engine_arg,
                              TABLE *tmp_table_arg, Item_subselect *item_arg,
                              select_result_interceptor *result_arg,
                              List<Item> *equi_join_conds_arg,
                              bool has_covering_null_row_arg,
                              bool has_covering_null_columns_arg,
                              uint count_columns_with_nulls_arg);
  void cleanup();
  virtual enum_engine_type engine_type() { return TABLE_SCAN_ENGINE; }
};
#endif 
