#ifndef SQL_SELECT_INCLUDED
#define SQL_SELECT_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "procedure.h"
#include "sql_array.h"                        
#include "records.h"                          
#include "opt_range.h"                
#include "filesort.h"

typedef struct st_join_table JOIN_TAB;

#define KEY_OPTIMIZE_EXISTS		1U
#define KEY_OPTIMIZE_REF_OR_NULL	2U
#define KEY_OPTIMIZE_EQ	                4U

inline uint get_hash_join_key_no() { return MAX_KEY; }

inline bool is_hash_join_key_no(uint key) { return key == MAX_KEY; }

typedef struct keyuse_t {
  TABLE *table;
  Item	*val;				
  table_map used_tables;
  uint	key, keypart, optimize;
  key_part_map keypart_map;
  ha_rows      ref_table_rows;
  
  bool null_rejecting;
  
  bool *cond_guard;
  
  uint         sj_pred_no;

  
  bool *validity_ref;

  bool is_for_hash_join() { return is_hash_join_key_no(key); }
} KEYUSE;


struct KEYUSE_EXT: public KEYUSE
{
  
  table_map needed_in_prefix;
  
  bool validity_var;
};


struct KEY_FIELD {
  Field		*field;
  Item_bool_func *cond;
  Item		*val;			
  uint		level;
  uint		optimize;
  bool		eq_func;
  
  bool          null_rejecting;
  bool         *cond_guard; 
  uint          sj_pred_no; 
};


#define NO_KEYPART ((uint)(-1))

class store_key;

const int NO_REF_PART= uint(-1);

typedef struct st_table_ref
{
  bool		key_err;
  
  bool          has_record;
  uint          key_parts;                
  uint          key_length;               
  int           key;                      
  uchar         *key_buff;                
  uchar         *key_buff2;               
  store_key     **key_copy;               

  
  key_part_map  const_ref_part_map;

  Item          **items;                  
  
  bool          **cond_guards;
  
  key_part_map  null_rejecting;
  table_map	depend_map;		  

  
  uchar          *null_ref_key;
  
  uint           null_ref_part;

  
  ha_rows       use_count;

  
  bool          disable_cache;

  bool tmp_table_index_lookup_init(THD *thd, KEY *tmp_key, Item_iterator &it,
                                   bool value, uint skip= 0);
  bool is_access_triggered();
} TABLE_REF;



enum join_type { JT_UNKNOWN,JT_SYSTEM,JT_CONST,JT_EQ_REF,JT_REF,JT_MAYBE_REF,
		 JT_ALL, JT_RANGE, JT_NEXT, JT_FT, JT_REF_OR_NULL,
		 JT_UNIQUE_SUBQUERY, JT_INDEX_SUBQUERY, JT_INDEX_MERGE,
                 JT_HASH, JT_HASH_RANGE, JT_HASH_NEXT, JT_HASH_INDEX_MERGE};

class JOIN;

enum enum_nested_loop_state
{
  NESTED_LOOP_KILLED= -2, NESTED_LOOP_ERROR= -1,
  NESTED_LOOP_OK= 0, NESTED_LOOP_NO_MORE_ROWS= 1,
  NESTED_LOOP_QUERY_LIMIT= 3, NESTED_LOOP_CURSOR_LIMIT= 4
};



enum sj_strategy_enum
{
  SJ_OPT_NONE=0,
  SJ_OPT_DUPS_WEEDOUT=1,
  SJ_OPT_LOOSE_SCAN  =2,
  SJ_OPT_FIRST_MATCH =3,
  SJ_OPT_MATERIALIZE =4,
  SJ_OPT_MATERIALIZE_SCAN=5
};


#define TAB_INFO_HAVE_VALUE 1U
#define TAB_INFO_USING_INDEX 2U
#define TAB_INFO_USING_WHERE 4U
#define TAB_INFO_FULL_SCAN_ON_NULL 8U

typedef enum_nested_loop_state
(*Next_select_func)(JOIN *, struct st_join_table *, bool);
Next_select_func setup_end_select_func(JOIN *join, JOIN_TAB *tab);
int rr_sequential(READ_RECORD *info);
int rr_sequential_and_unpack(READ_RECORD *info);
Item *remove_pushed_top_conjuncts(THD *thd, Item *cond);

#include "sql_explain.h"



class JOIN_CACHE;
class SJ_TMP_TABLE;
class JOIN_TAB_RANGE;
class AGGR_OP;
class Filesort;
struct SplM_plan_info;
class SplM_opt_info;

typedef struct st_join_table {
  TABLE		*table;
  TABLE_LIST    *tab_list;
  KEYUSE	*keyuse;			
  KEY           *hj_key;       
  SQL_SELECT	*select;
  COND		*select_cond;
  COND          *on_precond;      
  QUICK_SELECT_I *quick;
  
  Item          *pre_idx_push_select_cond;
  
  Item	       **on_expr_ref;
  COND_EQUAL    *cond_equal;    
  st_join_table *first_inner;   
  bool           found;         
  bool           not_null_compl;
  st_join_table *last_inner;    
  st_join_table *first_upper;  
  st_join_table *first_unmatched; 

  
  st_join_table *bush_root_tab;

  
  bool          last_leaf_in_bush;
  
  
  JOIN_TAB_RANGE *bush_children;
  
  
  enum explain_extra_tag info;
  
  Table_access_tracker *tracker;

  Table_access_tracker *jbuf_tracker;
  
  uint          packed_info;

  
  READ_RECORD::Setup_func read_first_record;
  Next_select_func next_select;
  READ_RECORD	read_record;
    
  READ_RECORD::Setup_func save_read_first_record;
  READ_RECORD::Read_func save_read_record;
  double	worst_seeks;
  key_map	const_keys;			
  key_map	checked_keys;			
  key_map	needed_reg;
  key_map       keys;                           

  
  ha_rows	records;
  
  ha_rows       found_records;
  
  double        read_time;
  
  
  double        records_read;
  
   
  double        cond_selectivity;  
  
  
  double        startup_cost;
    
  double        partial_join_cardinality;

  table_map	dependent,key_dependent;
  
  uint		use_quick;
  
  uint          index;
  uint		status;				
  uint		used_fields;
  ulong         used_fieldlength;
  ulong         max_used_fieldlength;
  uint          used_blobs;
  uint          used_null_fields;
  uint          used_uneven_bit_fields;
  enum join_type type;
  bool          cached_eq_ref_table,eq_ref_table;
  bool          shortcut_for_distinct;
  bool          sorted;
   
  ha_rows       limit; 
  TABLE_REF	ref;
  
  bool          icp_other_tables_ok;
  
  bool          idx_cond_fact_out;
  bool          use_join_cache;
  uint          used_join_cache_level;
  ulong         join_buffer_size_limit;
  JOIN_CACHE	*cache;
  
  Item          *cache_idx_cond;
  SQL_SELECT    *cache_select;
  AGGR_OP       *aggr;
  JOIN		*join;
  
  TABLE_LIST    *emb_sj_nest;

  
  struct st_join_table *first_sj_inner_tab;
  struct st_join_table *last_sj_inner_tab;

  
  SJ_TMP_TABLE  *flush_weedout_table;
  SJ_TMP_TABLE  *check_weed_out_table;
  
  SJ_TMP_TABLE  *first_weedout_table;

  
  Explain_table_access *explain_plan;

  
  struct st_join_table  *do_firstmatch;
 
  
  struct st_join_table *loosescan_match_tab;
  
  
  bool inside_loosescan_range;

  
  uchar *loosescan_buf;
  
  
  uint loosescan_key;

  
  uint loosescan_key_len;

  
  bool found_match;
  
  
  int  keep_current_rowid;

  
  nested_join_map embedding_map;

  
  TMP_TABLE_PARAM *tmp_table_param;

  
  Filesort *filesort;
  SORT_INFO *filesort_result;
  
  
  Window_funcs_computation* window_funcs_step;

  
  List<Item> *fields;
  
  List<Item> *all_fields;
  
  Ref_ptr_array *ref_array;

  
  ha_rows send_records;

  
  Item *having;

  
  bool distinct;

  
  enum sj_strategy_enum sj_strategy;

  uint n_sj_tables;

  bool preread_init_done;

  void cleanup();
  inline bool is_using_loose_index_scan()
  {
    const SQL_SELECT *sel= filesort ? filesort->select : select;
    return (sel && sel->quick &&
            (sel->quick->get_type() == QUICK_SELECT_I::QS_TYPE_GROUP_MIN_MAX));
  }
  bool is_using_agg_loose_index_scan ()
  {
    return (is_using_loose_index_scan() &&
            ((QUICK_GROUP_MIN_MAX_SELECT *)select->quick)->is_agg_distinct());
  }
  bool is_inner_table_of_semi_join_with_first_match()
  {
    return first_sj_inner_tab != NULL;
  }
  bool is_inner_table_of_semijoin()
  {
    return emb_sj_nest != NULL;
  }
  bool is_inner_table_of_outer_join()
  {
    return first_inner != NULL;
  }
  bool is_single_inner_of_semi_join_with_first_match()
  {
    return first_sj_inner_tab == this && last_sj_inner_tab == this;            
  }
  bool is_single_inner_of_outer_join()
  {
    return first_inner == this && first_inner->last_inner == this;
  }
  bool is_first_inner_for_outer_join()
  {
    return first_inner && first_inner == this;
  }
  bool use_match_flag()
  {
    return is_first_inner_for_outer_join() || first_sj_inner_tab == this ; 
  }
  bool check_only_first_match()
  {
    return is_inner_table_of_semi_join_with_first_match() ||
           (is_inner_table_of_outer_join() &&
            table->reginfo.not_exists_optimize);
  }
  bool is_last_inner_table()
  {
    return (first_inner && first_inner->last_inner == this) ||
           last_sj_inner_tab == this;
  }
  
  bool is_nested_inner()
  {
    if (first_inner && 
        (first_inner != first_inner->last_inner || first_inner->first_upper))
      return TRUE;
    if (first_sj_inner_tab && first_sj_inner_tab != last_sj_inner_tab)
      return TRUE;
    return FALSE;
  }
  struct st_join_table *get_first_inner_table()
  {
    if (first_inner)
      return first_inner;
    return first_sj_inner_tab; 
  }
  void set_select_cond(COND *to, uint line)
  {
    DBUG_PRINT("info", ("select_cond changes %p -> %p at line %u tab %p",
                        select_cond, to, line, this));
    select_cond= to;
  }
  COND *set_cond(COND *new_cond)
  {
    COND *tmp_select_cond= select_cond;
    set_select_cond(new_cond, __LINE__);
    if (select)
      select->cond= new_cond;
    return tmp_select_cond;
  }
  void calc_used_field_length(bool max_fl);
  ulong get_used_fieldlength()
  {
    if (!used_fieldlength)
      calc_used_field_length(FALSE);
    return used_fieldlength;
  }
  ulong get_max_used_fieldlength()
  {
    if (!max_used_fieldlength)
      calc_used_field_length(TRUE);
    return max_used_fieldlength;
  }
  double get_partial_join_cardinality() { return partial_join_cardinality; }
  bool hash_join_is_possible();
  int make_scan_filter();
  bool is_ref_for_hash_join() { return is_hash_join_key_no(ref.key); }
  KEY *get_keyinfo_by_key_no(uint key) 
  {
    return (is_hash_join_key_no(key) ? hj_key : table->key_info+key);
  }
  double scan_time();
  ha_rows get_examined_rows();
  bool preread_init();

  bool is_sjm_nest() { return MY_TEST(bush_children); }
  
  
  int get_non_merged_semijoin_select() const
  {
    Item_in_subselect *subq;
    if (table->pos_in_table_list && 
        (subq= table->pos_in_table_list->jtbm_subselect))
    {
      return subq->unit->first_select()->select_number;
    }
    return 0; 
  }

  bool access_from_tables_is_allowed(table_map used_tables,
                                     table_map sjm_lookup_tables)
  {
    table_map used_sjm_lookup_tables= used_tables & sjm_lookup_tables;
    return !used_sjm_lookup_tables ||
           (emb_sj_nest && 
            !(used_sjm_lookup_tables & ~emb_sj_nest->sj_inner_tables));
  }

  bool keyuse_is_valid_for_access_in_chosen_plan(JOIN *join, KEYUSE *keyuse);

  void remove_redundant_bnl_scan_conds();

  bool save_explain_data(Explain_table_access *eta, table_map prefix_tables,
                         bool distinct, struct st_join_table *first_top_tab);

  bool use_order() const; 
  bool sort_table();
  bool remove_duplicates();
  void add_keyuses_for_splitting();
  SplM_plan_info *choose_best_splitting(double record_count,
                                        table_map remaining_tables);
  bool fix_splitting(SplM_plan_info *spl_plan, table_map remaining_tables,
                     bool is_const_table);
} JOIN_TAB;


#include "sql_join_cache.h"

enum_nested_loop_state
sub_select_cache(JOIN *join, JOIN_TAB *join_tab, bool end_of_records);
enum_nested_loop_state 
sub_select(JOIN *join, JOIN_TAB *join_tab, bool end_of_records);
enum_nested_loop_state
sub_select_postjoin_aggr(JOIN *join, JOIN_TAB *join_tab, bool end_of_records);

enum_nested_loop_state
end_send_group(JOIN *join, JOIN_TAB *join_tab __attribute__((unused)),
	       bool end_of_records);
enum_nested_loop_state
end_write_group(JOIN *join, JOIN_TAB *join_tab __attribute__((unused)),
		bool end_of_records);


struct st_position;

class Semi_join_strategy_picker
{
public:
  
  virtual void set_empty() = 0;

  
  virtual void set_from_prev(struct st_position *prev) = 0;
  
  virtual bool check_qep(JOIN *join,
                         uint idx,
                         table_map remaining_tables, 
                         const JOIN_TAB *new_join_tab,
                         double *record_count,
                         double *read_time,
                         table_map *handled_fanout,
                         sj_strategy_enum *strategy,
                         struct st_position *loose_scan_pos) = 0;

  virtual void mark_used() = 0;

  virtual ~Semi_join_strategy_picker() {} 
};




class Duplicate_weedout_picker : public Semi_join_strategy_picker
{
  
  uint  first_dupsweedout_table;

  
  table_map dupsweedout_tables;
  
  bool is_used;
public:
  void set_empty()
  {
    dupsweedout_tables= 0;
    first_dupsweedout_table= MAX_TABLES;
    is_used= FALSE;
  }
  void set_from_prev(struct st_position *prev);
  
  bool check_qep(JOIN *join,
                 uint idx,
                 table_map remaining_tables, 
                 const JOIN_TAB *new_join_tab,
                 double *record_count,
                 double *read_time,
                 table_map *handled_fanout,
                 sj_strategy_enum *stratey,
                 struct st_position *loose_scan_pos);

  void mark_used() { is_used= TRUE; }
  friend void fix_semijoin_strategies_for_picked_join_order(JOIN *join);
};


class Firstmatch_picker : public Semi_join_strategy_picker
{
  
  uint first_firstmatch_table;
  
  table_map first_firstmatch_rtbl;
  
  table_map firstmatch_need_tables;

  bool is_used;

  bool in_firstmatch_prefix() { return (first_firstmatch_table != MAX_TABLES); }
  void invalidate_firstmatch_prefix() { first_firstmatch_table= MAX_TABLES; }
public:
  void set_empty()
  {
    invalidate_firstmatch_prefix();
    is_used= FALSE;
  }

  void set_from_prev(struct st_position *prev);
  bool check_qep(JOIN *join,
                 uint idx,
                 table_map remaining_tables, 
                 const JOIN_TAB *new_join_tab,
                 double *record_count,
                 double *read_time,
                 table_map *handled_fanout,
                 sj_strategy_enum *strategy,
                 struct st_position *loose_scan_pos);

  void mark_used() { is_used= TRUE; }
  friend void fix_semijoin_strategies_for_picked_join_order(JOIN *join);
};


class LooseScan_picker : public Semi_join_strategy_picker
{
  
  uint        first_loosescan_table;
  
  table_map   loosescan_need_tables;

  
  uint loosescan_key;  
  uint loosescan_parts; 
  
  bool is_used;
public:
  void set_empty()
  {
    first_loosescan_table= MAX_TABLES; 
    is_used= FALSE;
  }

  void set_from_prev(struct st_position *prev);
  bool check_qep(JOIN *join,
                 uint idx,
                 table_map remaining_tables, 
                 const JOIN_TAB *new_join_tab,
                 double *record_count,
                 double *read_time,
                 table_map *handled_fanout,
                 sj_strategy_enum *strategy,
                 struct st_position *loose_scan_pos);
  void mark_used() { is_used= TRUE; }

  friend class Loose_scan_opt;
  friend void best_access_path(JOIN      *join,
                               JOIN_TAB  *s,
                               table_map remaining_tables,
                               uint      idx,
                               bool      disable_jbuf,
                               double    record_count,
                               struct st_position *pos,
                               struct st_position *loose_scan_pos);
  friend bool get_best_combination(JOIN *join);
  friend int setup_semijoin_loosescan(JOIN *join);
  friend void fix_semijoin_strategies_for_picked_join_order(JOIN *join);
};


class Sj_materialization_picker : public Semi_join_strategy_picker
{
  bool is_used;

  
  uint      sjm_scan_last_inner;
  
  table_map sjm_scan_need_tables;

public:
  void set_empty()
  {
    sjm_scan_need_tables= 0;
    LINT_INIT_STRUCT(sjm_scan_last_inner);
    is_used= FALSE;
  }
  void set_from_prev(struct st_position *prev);
  bool check_qep(JOIN *join,
                 uint idx,
                 table_map remaining_tables, 
                 const JOIN_TAB *new_join_tab,
                 double *record_count,
                 double *read_time,
                 table_map *handled_fanout,
                 sj_strategy_enum *strategy,
                 struct st_position *loose_scan_pos);
  void mark_used() { is_used= TRUE; }

  friend void fix_semijoin_strategies_for_picked_join_order(JOIN *join);
};



typedef struct st_position
{
  
  JOIN_TAB *table;

  
  double records_read;

  
  double cond_selectivity; 

  
  double read_time;

  
  Cost_estimate prefix_cost;
  double    prefix_record_count;

  
  KEYUSE *key;

  
  table_map ref_depend_map;
 
  
  bool use_join_buffer;
 
  
  enum sj_strategy_enum sj_strategy;
  
  
  uint n_sj_tables;

  
  table_map dups_producing_tables;

  table_map inner_tables_handled_with_other_sjs;
   
  Duplicate_weedout_picker  dups_weedout_picker;
  Firstmatch_picker         firstmatch_picker;
  LooseScan_picker          loosescan_picker;
  Sj_materialization_picker sjmat_picker;

    
  SplM_plan_info *spl_plan;
} POSITION;

typedef Bounds_checked_array<Item_null_result*> Item_null_array;

typedef struct st_rollup
{
  enum State { STATE_NONE, STATE_INITED, STATE_READY };
  State state;
  Item_null_array null_items;
  Ref_ptr_array *ref_pointer_arrays;
  List<Item> *fields;
} ROLLUP;


class JOIN_TAB_RANGE: public Sql_alloc
{
public:
  JOIN_TAB *start;
  JOIN_TAB *end;
};

class Pushdown_query;



class AGGR_OP :public Sql_alloc
{
public:
  JOIN_TAB *join_tab;

  AGGR_OP(JOIN_TAB *tab) : join_tab(tab), write_func(NULL)
  {};

  enum_nested_loop_state put_record() { return put_record(false); };
  
  enum_nested_loop_state end_send();
  
  void set_write_func(Next_select_func new_write_func)
  {
    write_func= new_write_func;
  }

private:
  
  Next_select_func write_func;
  enum_nested_loop_state put_record(bool end_of_records);
  bool prepare_tmp_table();
};


class JOIN :public Sql_alloc
{
private:
  JOIN(const JOIN &rhs);                        
  JOIN& operator=(const JOIN &rhs);             

protected:

  
  class Join_plan_state {
  public:
    DYNAMIC_ARRAY keyuse;        
    POSITION *best_positions;    
    
    KEYUSE **join_tab_keyuse;
    
    key_map *join_tab_checked_keys;
    SJ_MATERIALIZATION_INFO **sj_mat_info;
    my_bool error;
  public:
    Join_plan_state(uint tables) : error(0)
    {   
      keyuse.elements= 0;
      keyuse.buffer= NULL;
      keyuse.malloc_flags= 0;
      best_positions= 0;                        
      error= my_multi_malloc(MYF(MY_WME),
                             &best_positions,
                             sizeof(*best_positions) * (tables + 1),
                             &join_tab_keyuse,
                             sizeof(*join_tab_keyuse) * tables,
                             &join_tab_checked_keys,
                             sizeof(*join_tab_checked_keys) * tables,
                             &sj_mat_info,
                             sizeof(sj_mat_info) * tables,
                             NullS) == 0;
    }
    Join_plan_state(JOIN *join);
    ~Join_plan_state()
    {
      delete_dynamic(&keyuse);
      my_free(best_positions);
    }
  };

  
  enum enum_reopt_result {
    REOPT_NEW_PLAN, 
    REOPT_OLD_PLAN, 
    REOPT_ERROR,    
    REOPT_NONE      
  };

  
  enum_reopt_result reoptimize(Item *added_where, table_map join_tables,
                               Join_plan_state *save_to);
  
  bool choose_tableless_subquery_plan();

public:
  void save_query_plan(Join_plan_state *save_to);
  void reset_query_plan();
  void restore_query_plan(Join_plan_state *restore_from);

public:
  JOIN_TAB *join_tab, **best_ref;

  
  List<Item_field> non_agg_fields;

  JOIN_TAB **map2table;    
  List<JOIN_TAB_RANGE> join_tab_ranges;
  
  
  TABLE    **table;
  
  TABLE    *sort_by_table;
  
  uint	   table_count;
  uint     outer_tables;  
  uint     const_tables;
  
  uint     top_join_tab_count;
  uint     aggr_tables;     
  uint	   send_group_parts;
  
  bool	   group;
  bool     need_distinct;

  
  bool     sort_and_group; 
  bool     first_record,full_join, no_field_update;
  bool     hash_join;
  bool	   do_send_rows;
  table_map const_table_map;
  
  table_map sjm_lookup_tables;
  
  table_map sjm_scan_tables;
  
  table_map found_const_table_map;
  
  
  table_map eliminated_tables;
  
  table_map outer_join;
  
  table_map select_list_used_tables;
  ha_rows  send_records,found_records,join_examined_rows;

  
  ha_rows row_limit;

  
  ha_rows select_limit;
  
  ha_rows duplicate_rows;
  
  ha_rows  fetch_limit;

  
  POSITION *best_positions;

  Pushdown_query *pushdown_query;
  JOIN_TAB *original_join_tab;
  uint	   original_table_count;


  
  TABLE_LIST *emb_sjm_nest;
  
  
  POSITION *positions;
  
  
  nested_join_map cur_embedding_map;
  
  
  table_map cur_sj_inner_tables;
  
  


  
  table_map complex_firstmatch_tables;

  Next_select_func first_select;
  
  double   best_read;
  
  double   join_record_count;
  List<Item> *fields;
  List<Cached_item> group_fields, group_fields_cache;
  THD	   *thd;
  Item_sum  **sum_funcs, ***sum_funcs_end;
  
  Item_sum  **sum_funcs2, ***sum_funcs_end2;
  Procedure *procedure;
  Item	    *having;
  Item      *tmp_having; 
  Item      *having_history; 
  ORDER     *group_list_for_estimates;
  bool      having_is_correlated;
  ulonglong  select_options;
  
  uint allowed_join_cache_types;
  bool allowed_semijoin_with_cache;
  bool allowed_outer_join_with_cache;
   
  uint max_allowed_join_cache_level;
  select_result *result;
  TMP_TABLE_PARAM tmp_table_param;
  MYSQL_LOCK *lock;
  
  SELECT_LEX_UNIT *unit;
  
  SELECT_LEX *select_lex;
  
  bool no_const_tables; 
  
  bool no_rows_in_result_called;

  
  bool filesort_found_rows;

  bool subq_exit_fl;
  
  ROLLUP rollup;				
  
  bool mixed_implicit_grouping;
  bool select_distinct;				
  
  bool group_optimized_away;

  
  bool simple_order, simple_group;

  
  enum
  {
    ordered_index_void,       
    ordered_index_group_by,   
    ordered_index_order_by    
  } ordered_index_usage;

  
  bool no_order;
  
  bool          skip_sort_order;

  bool need_tmp; 
  bool hidden_group_fields;
  
  bool cleaned;
  DYNAMIC_ARRAY keyuse;
  Item::cond_result cond_value, having_value;
  
  bool impossible_where; 
  List<Item> all_fields; 
  
  List<Item> tmp_all_fields1, tmp_all_fields2, tmp_all_fields3;
  
  List<Item> tmp_fields_list1, tmp_fields_list2, tmp_fields_list3;
  List<Item> &fields_list; 
  List<Item> procedure_fields_list;
  int error;

  ORDER *order, *group_list, *proc_param; 
  COND *conds;                            
  Item *conds_history;                    
  COND *outer_ref_cond;       
  COND *pseudo_bits_cond;     
  TABLE_LIST *tables_list;           
  List<TABLE_LIST> *join_list;       
  COND_EQUAL *cond_equal;
  COND_EQUAL *having_equal;
  
  Item *exec_const_cond;
  
  List<Item> exec_const_order_group_cond;
  SQL_SELECT *select;                
  JOIN_TAB *return_tab;              

  
  Ref_ptr_array ref_ptrs;
  
  Ref_ptr_array items0, items1, items2, items3;
  
  Ref_ptr_array current_ref_ptrs;

  const char *zero_result_cause; 
  
  bool union_part; 

  enum join_optimization_state { NOT_OPTIMIZED=0,
                                 OPTIMIZATION_IN_PROGRESS=1,
                                 OPTIMIZATION_PHASE_1_DONE=2,
                                 OPTIMIZATION_DONE=3};
  
  enum join_optimization_state optimization_state;
  bool initialized; 

  Explain_select *explain;
  
  enum { QEP_NOT_PRESENT_YET, QEP_AVAILABLE, QEP_DELETED} have_query_plan;

  
  bool tmp_table_keep_current_rowid;

  
  Item *in_to_exists_where;
  Item *in_to_exists_having;
  
  
  List<TABLE> sj_tmp_tables;
  
  List<SJ_MATERIALIZATION_INFO> sjm_info_list;

  
  bool set_group_rpa;
  
  bool group_sent;
  
  bool implicit_grouping; 

  bool with_two_phase_optimization;

  
  Join_plan_state *save_qep;
  
  SplM_opt_info *spl_opt_info;
  
  Dynamic_array<KEYUSE_EXT> *ext_keyuses_for_splitting;

  JOIN_TAB *sort_and_group_aggr_tab;

  JOIN(THD *thd_arg, List<Item> &fields_arg, ulonglong select_options_arg,
       select_result *result_arg)
    :fields_list(fields_arg)
  {
    init(thd_arg, fields_arg, select_options_arg, result_arg);
  }

  void init(THD *thd_arg, List<Item> &fields_arg, ulonglong select_options_arg,
       select_result *result_arg)
  {
    join_tab= 0;
    table= 0;
    table_count= 0;
    top_join_tab_count= 0;
    const_tables= 0;
    const_table_map= found_const_table_map= 0;
    aggr_tables= 0;
    eliminated_tables= 0;
    join_list= 0;
    implicit_grouping= FALSE;
    sort_and_group= 0;
    first_record= 0;
    do_send_rows= 1;
    duplicate_rows= send_records= 0;
    found_records= 0;
    fetch_limit= HA_POS_ERROR;
    thd= thd_arg;
    sum_funcs= sum_funcs2= 0;
    procedure= 0;
    having= tmp_having= having_history= 0;
    having_is_correlated= false;
    group_list_for_estimates= 0;
    select_options= select_options_arg;
    result= result_arg;
    lock= thd_arg->lock;
    select_lex= 0; 
    select_distinct= MY_TEST(select_options & SELECT_DISTINCT);
    no_order= 0;
    simple_order= 0;
    simple_group= 0;
    ordered_index_usage= ordered_index_void;
    need_distinct= 0;
    skip_sort_order= 0;
    with_two_phase_optimization= 0;
    save_qep= 0;
    spl_opt_info= 0;
    ext_keyuses_for_splitting= 0;
    spl_opt_info= 0;
    need_tmp= 0;
    hidden_group_fields= 0; 
    error= 0;
    select= 0;
    return_tab= 0;
    ref_ptrs.reset();
    items0.reset();
    items1.reset();
    items2.reset();
    items3.reset();
    zero_result_cause= 0;
    optimization_state= JOIN::NOT_OPTIMIZED;
    have_query_plan= QEP_NOT_PRESENT_YET;
    initialized= 0;
    cleaned= 0;
    cond_equal= 0;
    having_equal= 0;
    exec_const_cond= 0;
    group_optimized_away= 0;
    no_rows_in_result_called= 0;
    positions= best_positions= 0;
    pushdown_query= 0;
    original_join_tab= 0;
    explain= NULL;
    tmp_table_keep_current_rowid= 0;

    all_fields= fields_arg;
    if (&fields_list != &fields_arg)      
      fields_list= fields_arg;
    non_agg_fields.empty();
    bzero((char*) &keyuse,sizeof(keyuse));
    tmp_table_param.init();
    tmp_table_param.end_write_records= HA_POS_ERROR;
    rollup.state= ROLLUP::STATE_NONE;

    no_const_tables= FALSE;
    first_select= sub_select;
    set_group_rpa= false;
    group_sent= 0;

    outer_ref_cond= pseudo_bits_cond= NULL;
    in_to_exists_where= NULL;
    in_to_exists_having= NULL;
    emb_sjm_nest= NULL;
    sjm_lookup_tables= 0;
    sjm_scan_tables= 0;
  }

  
  bool only_const_tables()  { return const_tables == table_count; }
  
  uint exec_join_tab_cnt() { return tables_list ? top_join_tab_count : 0; }

  
  uint total_join_tab_cnt()
  {
    return exec_join_tab_cnt() + aggr_tables - 1;
  }

  int prepare(TABLE_LIST *tables, uint wind_num,
	      COND *conds, uint og_num, ORDER *order, bool skip_order_by,
              ORDER *group, Item *having, ORDER *proc_param, SELECT_LEX *select,
	      SELECT_LEX_UNIT *unit);
  bool prepare_stage2();
  int optimize();
  int optimize_inner();
  int optimize_stage2();
  bool build_explain();
  int reinit();
  int init_execution();
  void exec();

  void exec_inner();
  bool prepare_result(List<Item> **columns_list);
  int destroy();
  void restore_tmp();
  bool alloc_func_list();
  bool flatten_subqueries();
  bool optimize_unflattened_subqueries();
  bool optimize_constant_subqueries();
  int init_join_caches();
  bool make_sum_func_list(List<Item> &all_fields, List<Item> &send_fields,
			  bool before_group_by, bool recompute= FALSE);

  
  Ref_ptr_array ref_ptr_array_slice(size_t slice_num)
  {
    size_t slice_sz= select_lex->ref_pointer_array.size() / 5U;
    DBUG_ASSERT(select_lex->ref_pointer_array.size() % 5 == 0);
    DBUG_ASSERT(slice_num < 5U);
    return Ref_ptr_array(&select_lex->ref_pointer_array[slice_num * slice_sz],
                         slice_sz);
  }

  
  void copy_ref_ptr_array(Ref_ptr_array dst_arr, Ref_ptr_array src_arr)
  {
    DBUG_ASSERT(dst_arr.size() >= src_arr.size());
    void *dest= dst_arr.array();
    const void *src= src_arr.array();
    memcpy(dest, src, src_arr.size() * src_arr.element_size());
  }

  
  void set_items_ref_array(Ref_ptr_array src_arr)
  {
    copy_ref_ptr_array(ref_ptrs, src_arr);
    current_ref_ptrs= src_arr;
  }

  
  void init_items_ref_array()
  {
    items0= ref_ptr_array_slice(1);
    copy_ref_ptr_array(items0, ref_ptrs);
    current_ref_ptrs= items0;
  }

  bool rollup_init();
  bool rollup_process_const_fields();
  bool rollup_make_fields(List<Item> &all_fields, List<Item> &fields,
			  Item_sum ***func);
  int rollup_send_data(uint idx);
  int rollup_write_data(uint idx, TMP_TABLE_PARAM *tmp_table_param, TABLE *table);
  void join_free();
  
  void cleanup(bool full);
  void clear();
  bool send_row_on_empty_set()
  {
    return (do_send_rows && implicit_grouping && !group_optimized_away &&
            having_value != Item::COND_FALSE);
  }
  bool empty_result() { return (zero_result_cause && !implicit_grouping); }
  bool change_result(select_result *new_result, select_result *old_result);
  bool is_top_level_join() const
  {
    return (unit == &thd->lex->unit && (unit->fake_select_lex == 0 ||
                                        select_lex == unit->fake_select_lex));
  }
  void cache_const_exprs();
  inline table_map all_tables_map()
  {
    return (table_map(1) << table_count) - 1;
  }
  void drop_unused_derived_keys();
  bool get_best_combination();
  bool add_sorting_to_table(JOIN_TAB *tab, ORDER *order);
  inline void eval_select_list_used_tables();
  
  JOIN_TAB *get_sort_by_join_tab()
  {
    return (need_tmp || !sort_by_table || skip_sort_order ||
            ((group || tmp_table_param.sum_func_count) && !group_list)) ?
              NULL : join_tab+const_tables;
  }
  bool setup_subquery_caches();
  bool shrink_join_buffers(JOIN_TAB *jt, 
                           ulonglong curr_space,
                           ulonglong needed_space);
  void set_allowed_join_cache_types();
  bool is_allowed_hash_join_access()
  { 
    return MY_TEST(allowed_join_cache_types & JOIN_CACHE_HASHED_BIT) &&
           max_allowed_join_cache_level > JOIN_CACHE_HASHED_BIT;
  }
  
  bool test_if_need_tmp_table()
  {
    return ((const_tables != table_count &&
	    ((select_distinct || !simple_order || !simple_group) ||
	     (group_list && order) ||
             MY_TEST(select_options & OPTION_BUFFER_RESULT))) ||
            (rollup.state != ROLLUP::STATE_NONE && select_distinct));
  }
  bool choose_subquery_plan(table_map join_tables);
  void get_partial_cost_and_fanout(int end_tab_idx,
                                   table_map filter_map,
                                   double *read_time_arg, 
                                   double *record_count_arg);
  void get_prefix_cost_and_fanout(uint n_tables, 
                                  double *read_time_arg,
                                  double *record_count_arg);
  double get_examined_rows();
  
  bool transform_max_min_subquery();
  
  bool is_in_subquery()
  {
    return (unit->item && unit->item->is_in_predicate());
  }
  bool save_explain_data(Explain_query *output, bool can_overwrite,
                         bool need_tmp_table, bool need_order, bool distinct);
  int save_explain_data_intern(Explain_query *output, bool need_tmp_table,
                               bool need_order, bool distinct,
                               const char *message);
  JOIN_TAB *first_breadth_first_tab() { return join_tab; }
  bool check_two_phase_optimization(THD *thd);
  bool inject_cond_into_where(Item *injected_cond);
  bool check_for_splittable_materialized();
  void add_keyuses_for_splitting();
  bool inject_best_splitting_cond(table_map remaining_tables);
  bool fix_all_splittings_in_plan();

  bool transform_in_predicates_into_in_subq(THD *thd);
private:
  
  bool create_postjoin_aggr_table(JOIN_TAB *tab, List<Item> *tmp_table_fields,
                                  ORDER *tmp_table_group,
                                  bool save_sum_fields,
                                  bool distinct,
                                  bool keep_row_ordermake);
  
  void optimize_distinct();

  void cleanup_item_list(List<Item> &items) const;
  bool add_having_as_table_cond(JOIN_TAB *tab);
  bool make_aggr_tables_info();
  bool add_fields_for_current_rowid(JOIN_TAB *cur, List<Item> *fields);
};

enum enum_with_bush_roots { WITH_BUSH_ROOTS, WITHOUT_BUSH_ROOTS};
enum enum_with_const_tables { WITH_CONST_TABLES, WITHOUT_CONST_TABLES};

JOIN_TAB *first_linear_tab(JOIN *join,
                           enum enum_with_bush_roots include_bush_roots,
                           enum enum_with_const_tables const_tbls);
JOIN_TAB *next_linear_tab(JOIN* join, JOIN_TAB* tab, 
                          enum enum_with_bush_roots include_bush_roots);

JOIN_TAB *first_top_level_tab(JOIN *join, enum enum_with_const_tables with_const);
JOIN_TAB *next_top_level_tab(JOIN *join, JOIN_TAB *tab);

typedef struct st_select_check {
  uint const_ref,reg_ref;
} SELECT_CHECK;

extern const char *join_type_str[];


void count_field_types(SELECT_LEX *select_lex, TMP_TABLE_PARAM *param, 
                       List<Item> &fields, bool reset_with_sum_func);
bool setup_copy_fields(THD *thd, TMP_TABLE_PARAM *param,
		       Ref_ptr_array ref_pointer_array,
		       List<Item> &new_list1, List<Item> &new_list2,
		       uint elements, List<Item> &fields);
void copy_fields(TMP_TABLE_PARAM *param);
bool copy_funcs(Item **func_ptr, const THD *thd);
uint find_shortest_key(TABLE *table, const key_map *usable_keys);
Field* create_tmp_field_from_field(THD *thd, Field* org_field,
                                   LEX_CSTRING *name, TABLE *table,
                                   Item_field *item);

bool is_indexed_agg_distinct(JOIN *join, List<Item_field> *out_args);


bool simple_pred(Item_func *func_item, Item **args, bool *inv_order);
int opt_sum_query(THD* thd,
                  List<TABLE_LIST> &tables, List<Item> &all_fields, COND *conds);


extern "C" int refpos_order_cmp(void* arg, const void *a,const void *b);



class store_key :public Sql_alloc
{
public:
  bool null_key; 
  enum store_key_result { STORE_KEY_OK, STORE_KEY_FATAL, STORE_KEY_CONV };
  enum Type { FIELD_STORE_KEY, ITEM_STORE_KEY, CONST_ITEM_STORE_KEY };
  store_key(THD *thd, Field *field_arg, uchar *ptr, uchar *null, uint length)
    :null_key(0), null_ptr(null), err(0)
  {
    to_field=field_arg->new_key_field(thd->mem_root, field_arg->table,
                                      ptr, length, null, 1);
  }
  store_key(store_key &arg)
    :Sql_alloc(), null_key(arg.null_key), to_field(arg.to_field),
             null_ptr(arg.null_ptr), err(arg.err)

  {}
  virtual ~store_key() {}			
  virtual enum Type type() const=0;
  virtual const char *name() const=0;
  virtual bool store_key_is_const() { return false; }

  
  enum store_key_result copy()
  {
    enum store_key_result result;
    THD *thd= to_field->table->in_use;
    enum_check_fields saved_count_cuted_fields= thd->count_cuted_fields;
    sql_mode_t orig_sql_mode= thd->variables.sql_mode;
    thd->variables.sql_mode&= ~(MODE_NO_ZERO_IN_DATE | MODE_NO_ZERO_DATE);
    thd->variables.sql_mode|= MODE_INVALID_DATES;

    thd->count_cuted_fields= CHECK_FIELD_IGNORE;

    result= copy_inner();

    thd->count_cuted_fields= saved_count_cuted_fields;
    thd->variables.sql_mode= orig_sql_mode;

    return result;
  }

 protected:
  Field *to_field;				
  uchar *null_ptr;
  uchar err;

  virtual enum store_key_result copy_inner()=0;
};


class store_key_field: public store_key
{
  Copy_field copy_field;
  const char *field_name;
 public:
  store_key_field(THD *thd, Field *to_field_arg, uchar *ptr,
                  uchar *null_ptr_arg,
		  uint length, Field *from_field, const char *name_arg)
    :store_key(thd, to_field_arg,ptr,
	       null_ptr_arg ? null_ptr_arg : from_field->maybe_null() ? &err
	       : (uchar*) 0, length), field_name(name_arg)
  {
    if (to_field)
    {
      copy_field.set(to_field,from_field,0);
    }
  }  

  enum Type type() const { return FIELD_STORE_KEY; }
  const char *name() const { return field_name; }

  void change_source_field(Item_field *fld_item)
  {
    copy_field.set(to_field, fld_item->field, 0);
    field_name= fld_item->full_name();
  }

 protected: 
  enum store_key_result copy_inner()
  {
    TABLE *table= copy_field.to_field->table;
    my_bitmap_map *old_map= dbug_tmp_use_all_columns(table,
                                                     table->write_set);

      
    bzero(copy_field.to_ptr,copy_field.to_length);

    copy_field.do_copy(&copy_field);
    dbug_tmp_restore_column_map(table->write_set, old_map);
    null_key= to_field->is_null();
    return err != 0 ? STORE_KEY_FATAL : STORE_KEY_OK;
  }
};


class store_key_item :public store_key
{
 protected:
  Item *item;
  
  bool use_value;
public:
  store_key_item(THD *thd, Field *to_field_arg, uchar *ptr,
                 uchar *null_ptr_arg, uint length, Item *item_arg, bool val)
    :store_key(thd, to_field_arg, ptr,
	       null_ptr_arg ? null_ptr_arg : item_arg->maybe_null ?
	       &err : (uchar*) 0, length), item(item_arg), use_value(val)
  {}
  store_key_item(store_key &arg, Item *new_item, bool val)
    :store_key(arg), item(new_item), use_value(val)
  {}


  enum Type type() const { return ITEM_STORE_KEY; }
  const char *name() const { return "func"; }

 protected:  
  enum store_key_result copy_inner()
  {
    TABLE *table= to_field->table;
    my_bitmap_map *old_map= dbug_tmp_use_all_columns(table,
                                                     table->write_set);
    int res= FALSE;

      
    to_field->reset();

    if (use_value)
      item->save_val(to_field);
    else
      res= item->save_in_field(to_field, 1);
    
    if (!res && table->in_use->is_error())
      res= 1; 
    dbug_tmp_restore_column_map(table->write_set, old_map);
    null_key= to_field->is_null() || item->null_value;
    return ((err != 0 || res < 0 || res > 2) ? STORE_KEY_FATAL : 
            (store_key_result) res);
  }
};


class store_key_const_item :public store_key_item
{
  bool inited;
public:
  store_key_const_item(THD *thd, Field *to_field_arg, uchar *ptr,
		       uchar *null_ptr_arg, uint length,
		       Item *item_arg)
    :store_key_item(thd, to_field_arg, ptr,
		    null_ptr_arg ? null_ptr_arg : item_arg->maybe_null ?
		    &err : (uchar*) 0, length, item_arg, FALSE), inited(0)
  {
  }
  store_key_const_item(store_key &arg, Item *new_item)
    :store_key_item(arg, new_item, FALSE), inited(0)
  {}

  enum Type type() const { return CONST_ITEM_STORE_KEY; }
  const char *name() const { return "const"; }
  bool store_key_is_const() { return true; }

protected:  
  enum store_key_result copy_inner()
  {
    int res;
    if (!inited)
    {
      inited=1;
      TABLE *table= to_field->table;
      my_bitmap_map *old_map= dbug_tmp_use_all_columns(table,
                                                       table->write_set);
      if ((res= item->save_in_field(to_field, 1)))
      {       
        if (!err)
          err= res < 0 ? 1 : res; 
      }
      
      if (!err && to_field->table->in_use->is_error())
        err= 1; 
      dbug_tmp_restore_column_map(table->write_set, old_map);
    }
    null_key= to_field->is_null() || item->null_value;
    return (err > 2 ? STORE_KEY_FATAL : (store_key_result) err);
  }
};

bool cp_buffer_from_ref(THD *thd, TABLE *table, TABLE_REF *ref);
bool error_if_full_join(JOIN *join);
int report_error(TABLE *table, int error);
int safe_index_read(JOIN_TAB *tab);
int get_quick_record(SQL_SELECT *select);
int setup_order(THD *thd, Ref_ptr_array ref_pointer_array, TABLE_LIST *tables,
                List<Item> &fields, List <Item> &all_fields, ORDER *order,
                bool from_window_spec= false);
int setup_group(THD *thd,  Ref_ptr_array ref_pointer_array, TABLE_LIST *tables,
		List<Item> &fields, List<Item> &all_fields, ORDER *order,
		bool *hidden_group_fields, bool from_window_spec= false);
bool fix_inner_refs(THD *thd, List<Item> &all_fields, SELECT_LEX *select,
                    Ref_ptr_array ref_pointer_array);
int join_read_key2(THD *thd, struct st_join_table *tab, TABLE *table,
                   struct st_table_ref *table_ref);

bool handle_select(THD *thd, LEX *lex, select_result *result,
                   ulong setup_tables_done_option);
bool mysql_select(THD *thd,
                  TABLE_LIST *tables, uint wild_num,  List<Item> &list,
                  COND *conds, uint og_num, ORDER *order, ORDER *group,
                  Item *having, ORDER *proc_param, ulonglong select_type, 
                  select_result *result, SELECT_LEX_UNIT *unit, 
                  SELECT_LEX *select_lex);
void free_underlaid_joins(THD *thd, SELECT_LEX *select);
bool mysql_explain_union(THD *thd, SELECT_LEX_UNIT *unit,
                         select_result *result);
Field *create_tmp_field(THD *thd, TABLE *table,Item *item, Item::Type type,
			Item ***copy_func, Field **from_field,
                        Field **def_field,
			bool group, bool modify_item,
			bool table_cant_handle_bit_fields,
                        bool make_copy_field);




class Virtual_tmp_table: public TABLE
{
  
  void destruct_fields()
  {
    for (uint i= 0; i < s->fields; i++)
    {
      field[i]->free();
      delete field[i];  
    }
    s->fields= 0;       
  }

protected:
  
  uint m_alloced_field_count;

  
  void setup_field_pointers();

public:
  
  static void *operator new(size_t size, THD *thd) throw();
  static void operator delete(void *ptr, size_t size) { TRASH_FREE(ptr, size); }
  static void operator delete(void *, THD *) throw(){}

  Virtual_tmp_table(THD *thd) : m_alloced_field_count(0)
  {
    reset();
    temp_pool_slot= MY_BIT_NONE;
    in_use= thd;
    copy_blobs= true;
    alias.set("", 0, &my_charset_bin);
  }

  ~Virtual_tmp_table()
  {
    if (s)
      destruct_fields();
  }

  
  bool init(uint field_count);

  
  bool add(Field *new_field)
  {
    DBUG_ASSERT(s->fields < m_alloced_field_count);
    new_field->init(this);
    field[s->fields]= new_field;
    s->reclength+= new_field->pack_length();
    if (!(new_field->flags & NOT_NULL_FLAG))
      s->null_fields++;
    if (new_field->flags & BLOB_FLAG)
    {
      
      DBUG_ASSERT(s->blob_fields);
      DBUG_ASSERT(s->blob_fields <= m_alloced_field_count);
      s->blob_field[s->blob_fields - 1]= s->fields;
    }
    new_field->field_index= s->fields++;
    return false;
  }

  
  bool add(List<Spvar_definition> &field_list);

  
  bool open();

  void set_all_fields_to_null()
  {
    for (uint i= 0; i < s->fields; i++)
      field[i]->set_null();
  }
  
  bool sp_set_all_fields_from_item_list(THD *thd, List<Item> &items);

  
  bool sp_set_all_fields_from_item(THD *thd, Item *value);

  
  bool sp_find_field_by_name(uint *idx, const LEX_CSTRING &name) const;

  
  bool sp_find_field_by_name_or_error(uint *idx,
                                      const LEX_CSTRING &var_name,
                                      const LEX_CSTRING &field_name) const;
};




inline Virtual_tmp_table *
create_virtual_tmp_table(THD *thd, List<Spvar_definition> &field_list)
{
  Virtual_tmp_table *table;
  if (!(table= new(thd) Virtual_tmp_table(thd)))
    return NULL;

  
  DBUG_EXECUTE_IF("simulate_create_virtual_tmp_table_out_of_memory",
                  DBUG_SET("+d,simulate_out_of_memory"););

  if (table->init(field_list.elements) ||
      table->add(field_list) ||
      table->open())
  {
    delete table;
    return NULL;
  }
  return table;
}



inline TABLE *
create_virtual_tmp_table(THD *thd, Field *field)
{
  Virtual_tmp_table *table;
  DBUG_ASSERT(field);
  if (!(table= new(thd) Virtual_tmp_table(thd)))
    return NULL;
  if (table->init(1) ||
      table->add(field) ||
      table->open())
  {
    delete table;
    return NULL;
  }
  return table;
}


int test_if_item_cache_changed(List<Cached_item> &list);
int join_init_read_record(JOIN_TAB *tab);
int join_read_record_no_init(JOIN_TAB *tab);
void set_position(JOIN *join,uint idx,JOIN_TAB *table,KEYUSE *key);
inline Item * and_items(THD *thd, Item* cond, Item *item)
{
  return (cond ? (new (thd->mem_root) Item_cond_and(thd, cond, item)) : item);
}
inline Item * or_items(THD *thd, Item* cond, Item *item)
{
  return (cond ? (new (thd->mem_root) Item_cond_or(thd, cond, item)) : item);
}
bool choose_plan(JOIN *join, table_map join_tables);
void optimize_wo_join_buffering(JOIN *join, uint first_tab, uint last_tab, 
                                table_map last_remaining_tables, 
                                bool first_alt, uint no_jbuf_before,
                                double *outer_rec_count, double *reopt_cost);
Item_equal *find_item_equal(COND_EQUAL *cond_equal, Field *field,
                            bool *inherited_fl);
extern bool test_if_ref(Item *, 
                 Item_field *left_item,Item *right_item);

inline bool optimizer_flag(THD *thd, uint flag)
{ 
  return (thd->variables.optimizer_switch & flag);
}



uint get_index_for_order(ORDER *order, TABLE *table, SQL_SELECT *select,
                         ha_rows limit, ha_rows *scanned_limit, 
                         bool *need_sort, bool *reverse);
ORDER *simple_remove_const(ORDER *order, COND *where);
bool const_expression_in_where(COND *cond, Item *comp_item,
                               Field *comp_field= NULL,
                               Item **const_item= NULL);
bool cond_is_datetime_is_null(Item *cond);
bool cond_has_datetime_is_null(Item *cond);


void eliminate_tables(JOIN *join);


void push_index_cond(JOIN_TAB *tab, uint keyno);

#define OPT_LINK_EQUAL_FIELDS    1


int print_explain_message_line(select_result_sink *result, 
                               uint8 options, bool is_analyze,
                               uint select_number,
                               const char *select_type,
                               ha_rows *rows,
                               const char *message);
void explain_append_mrr_info(QUICK_RANGE_SELECT *quick, String *res);
int append_possible_keys(MEM_ROOT *alloc, String_list &list, TABLE *table, 
                         key_map possible_keys);



#define STRING_TOTAL_LENGTH_TO_PACK_ROWS 128
#define AVG_STRING_LENGTH_TO_PACK_ROWS   64
#define RATIO_TO_PACK_ROWS	       2
#define MIN_STRING_LENGTH_TO_PACK_ROWS   10

void calc_group_buffer(TMP_TABLE_PARAM *param, ORDER *group);
TABLE *create_tmp_table(THD *thd,TMP_TABLE_PARAM *param,List<Item> &fields,
			ORDER *group, bool distinct, bool save_sum_fields,
			ulonglong select_options, ha_rows rows_limit,
                        const LEX_CSTRING *alias, bool do_not_open=FALSE,
                        bool keep_row_order= FALSE);
void free_tmp_table(THD *thd, TABLE *entry);
bool create_internal_tmp_table_from_heap(THD *thd, TABLE *table,
                                         TMP_ENGINE_COLUMNDEF *start_recinfo,
                                         TMP_ENGINE_COLUMNDEF **recinfo, 
                                         int error, bool ignore_last_dupp_key_error,
                                         bool *is_duplicate);
bool create_internal_tmp_table(TABLE *table, KEY *keyinfo, 
                               TMP_ENGINE_COLUMNDEF *start_recinfo,
                               TMP_ENGINE_COLUMNDEF **recinfo, 
                               ulonglong options);
bool instantiate_tmp_table(TABLE *table, KEY *keyinfo, 
                           TMP_ENGINE_COLUMNDEF *start_recinfo,
                           TMP_ENGINE_COLUMNDEF **recinfo,
                           ulonglong options);
bool open_tmp_table(TABLE *table);
void setup_tmp_table_column_bitmaps(TABLE *table, uchar *bitmaps);
double prev_record_reads(POSITION *positions, uint idx, table_map found_ref);
void fix_list_after_tbl_changes(SELECT_LEX *new_parent, List<TABLE_LIST> *tlist);
double get_tmp_table_lookup_cost(THD *thd, double row_count, uint row_size);
double get_tmp_table_write_cost(THD *thd, double row_count, uint row_size);
void optimize_keyuse(JOIN *join, DYNAMIC_ARRAY *keyuse_array);
bool sort_and_filter_keyuse(THD *thd, DYNAMIC_ARRAY *keyuse,
                            bool skip_unprefixed_keyparts);

struct st_cond_statistic
{
  Item *cond;
  Field *field_arg;
  ulong positive;
};
typedef struct st_cond_statistic COND_STATISTIC;

ulong check_selectivity(THD *thd,
                        ulong rows_to_read,
                        TABLE *table,
                        List<COND_STATISTIC> *conds);

class Pushdown_query: public Sql_alloc
{
public:
  SELECT_LEX *select_lex;
  bool store_data_in_temp_table;
  group_by_handler *handler;
  Item *having;

  Pushdown_query(SELECT_LEX *select_lex_arg, group_by_handler *handler_arg)
    : select_lex(select_lex_arg), store_data_in_temp_table(0),
    handler(handler_arg), having(0) {}

  ~Pushdown_query() { delete handler; }

  
  int execute(JOIN *join);
};

bool test_if_order_compatible(SQL_I_List<ORDER> &a, SQL_I_List<ORDER> &b);
int test_if_group_changed(List<Cached_item> &list);
int create_sort_index(THD *thd, JOIN *join, JOIN_TAB *tab, Filesort *fsort);

JOIN_TAB *first_explain_order_tab(JOIN* join);
JOIN_TAB *next_explain_order_tab(JOIN* join, JOIN_TAB* tab);

#endif 
