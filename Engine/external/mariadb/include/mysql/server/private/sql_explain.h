



#ifndef SQL_EXPLAIN_INCLUDED
#define SQL_EXPLAIN_INCLUDED

class String_list: public List<char>
{
public:
  const char *append_str(MEM_ROOT *mem_root, const char *str);
};

class Json_writer;




const int FAKE_SELECT_LEX_ID= (int)UINT_MAX;

class Explain_query;


class Explain_node : public Sql_alloc
{
public:
  Explain_node(MEM_ROOT *root) :
    cache_tracker(NULL),
    connection_type(EXPLAIN_NODE_OTHER),
    children(root)
  {}
  
  enum explain_node_type 
  {
    EXPLAIN_UNION, 
    EXPLAIN_SELECT,
    EXPLAIN_BASIC_JOIN,
    EXPLAIN_UPDATE,
    EXPLAIN_DELETE, 
    EXPLAIN_INSERT
  };
  
  
  enum explain_connection_type {
    EXPLAIN_NODE_OTHER,
    EXPLAIN_NODE_DERIVED, 
    EXPLAIN_NODE_NON_MERGED_SJ 
  };

  virtual enum explain_node_type get_type()= 0;
  virtual int get_select_id()= 0;

  
  Expression_cache_tracker* cache_tracker;

  
  enum explain_connection_type connection_type;

  
  Dynamic_array<int> children;
  void add_child(int select_no)
  {
    children.append(select_no);
  }

  virtual int print_explain(Explain_query *query, select_result_sink *output, 
                            uint8 explain_flags, bool is_analyze)=0;
  virtual void print_explain_json(Explain_query *query, Json_writer *writer, 
                                  bool is_analyze)= 0;

  int print_explain_for_children(Explain_query *query, select_result_sink *output, 
                                 uint8 explain_flags, bool is_analyze);
  void print_explain_json_for_children(Explain_query *query,
                                       Json_writer *writer, bool is_analyze);
  bool print_explain_json_cache(Json_writer *writer, bool is_analyze);
  virtual ~Explain_node(){}
};


class Explain_table_access;



class Explain_basic_join : public Explain_node
{
public:
  enum explain_node_type get_type() { return EXPLAIN_BASIC_JOIN; }
  
  Explain_basic_join(MEM_ROOT *root) : Explain_node(root), join_tabs(NULL) {}
  ~Explain_basic_join();

  bool add_table(Explain_table_access *tab, Explain_query *query);

  int get_select_id() { return select_id; }

  int select_id;

  int print_explain(Explain_query *query, select_result_sink *output,
                    uint8 explain_flags, bool is_analyze);
  void print_explain_json(Explain_query *query, Json_writer *writer, 
                          bool is_analyze);

  void print_explain_json_interns(Explain_query *query, Json_writer *writer,
                                  bool is_analyze);

  
  Explain_table_access** join_tabs;
  uint n_join_tabs;
};


class Explain_aggr_node;


class Explain_select : public Explain_basic_join
{
public:
  enum explain_node_type get_type() { return EXPLAIN_SELECT; }

  Explain_select(MEM_ROOT *root, bool is_analyze) : 
  Explain_basic_join(root),
#ifndef DBUG_OFF
    select_lex(NULL),
#endif
    linkage(UNSPECIFIED_TYPE),
    message(NULL),
    having(NULL), having_value(Item::COND_UNDEF),
    using_temporary(false), using_filesort(false),
    time_tracker(is_analyze),
    aggr_tree(NULL)
  {}

  void add_linkage(Json_writer *writer);

public:
#ifndef DBUG_OFF
  SELECT_LEX *select_lex;
#endif
  const char *select_type;
  enum sub_select_type linkage;

  
  const char *message;

  
  Item *exec_const_cond;
  Item *outer_ref_cond;

  
  Item *having;
  Item::cond_result having_value;

  
  bool using_temporary;
  bool using_filesort;

  
  Time_and_counter_tracker time_tracker;
  
  
  Explain_aggr_node* aggr_tree;

  int print_explain(Explain_query *query, select_result_sink *output, 
                    uint8 explain_flags, bool is_analyze);
  void print_explain_json(Explain_query *query, Json_writer *writer, 
                          bool is_analyze);
  
  Table_access_tracker *get_using_temporary_read_tracker()
  {
    return &using_temporary_read_tracker;
  }
private:
  Table_access_tracker using_temporary_read_tracker;
};




typedef enum 
{
  AGGR_OP_TEMP_TABLE,
  AGGR_OP_FILESORT,
  
  AGGR_OP_REMOVE_DUPLICATES,
  AGGR_OP_WINDOW_FUNCS
  
} enum_explain_aggr_node_type;


class Explain_aggr_node : public Sql_alloc
{
public:
  virtual enum_explain_aggr_node_type get_type()= 0;
  virtual ~Explain_aggr_node() {}
  Explain_aggr_node *child;
};

class Explain_aggr_filesort : public Explain_aggr_node
{
  List<Item> sort_items;
  List<ORDER::enum_order> sort_directions;
public:
  enum_explain_aggr_node_type get_type() { return AGGR_OP_FILESORT; }
  Filesort_tracker tracker;

  Explain_aggr_filesort(MEM_ROOT *mem_root, bool is_analyze, 
                        Filesort *filesort);

  void print_json_members(Json_writer *writer, bool is_analyze);
};

class Explain_aggr_tmp_table : public Explain_aggr_node
{
public:
  enum_explain_aggr_node_type get_type() { return AGGR_OP_TEMP_TABLE; }
};

class Explain_aggr_remove_dups : public Explain_aggr_node
{
public:
  enum_explain_aggr_node_type get_type() { return AGGR_OP_REMOVE_DUPLICATES; }
};

class Explain_aggr_window_funcs : public Explain_aggr_node
{
  List<Explain_aggr_filesort> sorts;
public:
  enum_explain_aggr_node_type get_type() { return AGGR_OP_WINDOW_FUNCS; }

  void print_json_members(Json_writer *writer, bool is_analyze);
  friend class Window_funcs_computation;
};



extern const char *unit_operation_text[4];



class Explain_union : public Explain_node
{
public:
  Explain_union(MEM_ROOT *root, bool is_analyze) : 
    Explain_node(root),
    is_recursive_cte(false),
    fake_select_lex_explain(root, is_analyze)
  {}

  enum explain_node_type get_type() { return EXPLAIN_UNION; }
  unit_common_op operation;

  int get_select_id()
  {
    DBUG_ASSERT(union_members.elements() > 0);
    return union_members.at(0);
  }
  
  Dynamic_array<int> union_members;

  void add_select(int select_no)
  {
    union_members.append(select_no);
  }
  int print_explain(Explain_query *query, select_result_sink *output, 
                    uint8 explain_flags, bool is_analyze);
  void print_explain_json(Explain_query *query, Json_writer *writer, 
                          bool is_analyze);

  const char *fake_select_type;
  bool using_filesort;
  bool using_tmp;
  bool is_recursive_cte;
  
  
  Explain_select fake_select_lex_explain;

  Table_access_tracker *get_fake_select_lex_tracker()
  {
    return &fake_select_lex_tracker;
  }
  Table_access_tracker *get_tmptable_read_tracker()
  {
    return &tmptable_read_tracker;
  }
private:
  uint make_union_table_name(char *buf);
  
  Table_access_tracker fake_select_lex_tracker;
  
  Table_access_tracker tmptable_read_tracker; 
};


class Explain_update;
class Explain_delete;
class Explain_insert;




class Explain_query : public Sql_alloc
{
public:
  Explain_query(THD *thd, MEM_ROOT *root);
  ~Explain_query();

  
  void add_node(Explain_node *node);
  void add_insert_plan(Explain_insert *insert_plan_arg);
  void add_upd_del_plan(Explain_update *upd_del_plan_arg);

  
  Explain_node *get_node(uint select_id);

  
  Explain_select *get_select(uint select_id);
  
  Explain_union *get_union(uint select_id);
 
  
  int print_explain(select_result_sink *output, uint8 explain_flags, 
                    bool is_analyze);
  
  
  int send_explain(THD *thd);
  
  
  bool print_explain_str(THD *thd, String *out_str, bool is_analyze);

  void print_explain_json(select_result_sink *output, bool is_analyze);

  
  bool have_query_plan() { return insert_plan || upd_del_plan|| get_node(1) != NULL; }

  void query_plan_ready();

  MEM_ROOT *mem_root;

  Explain_update *get_upd_del_plan() { return upd_del_plan; }
private:
  
  Explain_update *upd_del_plan;

  
  Explain_insert *insert_plan;

  Dynamic_array<Explain_union*> unions;
  Dynamic_array<Explain_select*> selects;
  
  THD *thd; 
  bool apc_enabled;
  
  longlong operations;
};



enum explain_extra_tag
{
  ET_none= 0, 
  ET_USING_INDEX_CONDITION,
  ET_USING_INDEX_CONDITION_BKA,
  ET_USING, 
  ET_RANGE_CHECKED_FOR_EACH_RECORD,
  ET_USING_WHERE_WITH_PUSHED_CONDITION,
  ET_USING_WHERE,
  ET_NOT_EXISTS,

  ET_USING_INDEX,
  ET_FULL_SCAN_ON_NULL_KEY,
  ET_SKIP_OPEN_TABLE,
  ET_OPEN_FRM_ONLY,
  ET_OPEN_FULL_TABLE,

  ET_SCANNED_0_DATABASES,
  ET_SCANNED_1_DATABASE,
  ET_SCANNED_ALL_DATABASES,

  ET_USING_INDEX_FOR_GROUP_BY,

  ET_USING_MRR, 

  ET_DISTINCT,
  ET_LOOSESCAN,
  ET_START_TEMPORARY,
  ET_END_TEMPORARY,
  ET_FIRST_MATCH,
  
  ET_USING_JOIN_BUFFER,

  ET_CONST_ROW_NOT_FOUND,
  ET_UNIQUE_ROW_NOT_FOUND,
  ET_IMPOSSIBLE_ON_CONDITION,

  ET_total
};




class EXPLAIN_BKA_TYPE
{
public:
  EXPLAIN_BKA_TYPE() : join_alg(NULL) {}

  size_t join_buffer_size;

  bool incremental;

  
  const char *join_alg;

  
  StringBuffer<64> mrr_type;
  
  bool is_using_jbuf() { return (join_alg != NULL); }
};



class Explain_index_use : public Sql_alloc
{
  char *key_name;
  uint key_len;
public:
  String_list key_parts_list;
  
  Explain_index_use()
  {
    clear();
  }

  void clear()
  {
    key_name= NULL;
    key_len= (uint)-1;
  }
  bool set(MEM_ROOT *root, KEY *key_name, uint key_len_arg);
  bool set_pseudo_key(MEM_ROOT *root, const char *key_name);

  inline const char *get_key_name() const { return key_name; }
  inline uint get_key_len() const { return key_len; }
};



class Explain_quick_select : public Sql_alloc
{
public:
  Explain_quick_select(int quick_type_arg) : quick_type(quick_type_arg) 
  {}

  const int quick_type;

  bool is_basic() 
  {
    return (quick_type == QUICK_SELECT_I::QS_TYPE_RANGE || 
            quick_type == QUICK_SELECT_I::QS_TYPE_RANGE_DESC ||
            quick_type == QUICK_SELECT_I::QS_TYPE_GROUP_MIN_MAX);
  }
  
  
  Explain_index_use range;
  
  
  List<Explain_quick_select> children;
  
  void print_extra(String *str);
  void print_key(String *str);
  void print_key_len(String *str);

  void print_json(Json_writer *writer);

  void print_extra_recursive(String *str);
private:
  const char *get_name_by_type();
};




typedef const char* NAME;

class Explain_range_checked_fer : public Sql_alloc
{
public:
  String_list key_set;
  key_map keys_map;
private:
  ha_rows full_scan, index_merge;
  ha_rows *keys_stat;
  NAME *keys_stat_names;
  uint keys;

public:
  Explain_range_checked_fer()
    :Sql_alloc(), full_scan(0), index_merge(0),
    keys_stat(0), keys_stat_names(0), keys(0)
  {}

  int append_possible_keys_stat(MEM_ROOT *alloc,
                                TABLE *table, key_map possible_keys);
  void collect_data(QUICK_SELECT_I *quick);
  void print_json(Json_writer *writer, bool is_analyze);
};



class Explain_table_access : public Sql_alloc
{
public:
  Explain_table_access(MEM_ROOT *root) :
    derived_select_number(0),
    non_merged_sjm_number(0),
    extra_tags(root),
    range_checked_fer(NULL),
    full_scan_on_null_key(false),
    start_dups_weedout(false),
    end_dups_weedout(false),
    where_cond(NULL),
    cache_cond(NULL),
    pushed_index_cond(NULL),
    sjm_nest(NULL),
    pre_join_sort(NULL)
  {}
  ~Explain_table_access() { delete sjm_nest; }

  void push_extra(enum explain_extra_tag extra_tag);

  

  
  StringBuffer<32> table_name;
  StringBuffer<32> used_partitions;
  String_list used_partitions_list;
  
  StringBuffer<32> mrr_type;
  StringBuffer<32> firstmatch_table_name;

  
  int derived_select_number;
  
  int non_merged_sjm_number;

  enum join_type type;

  bool used_partitions_set;
  
  
  String_list possible_keys;

  bool rows_set; 
  bool filtered_set; 
  
  bool loose_scan_is_scanning;
  
  
  Explain_index_use key;
  
  
  Explain_index_use hash_next_key;
  
  String_list ref_list;

  ha_rows rows;
  double filtered;

  
  Dynamic_array<enum explain_extra_tag> extra_tags;

  
  Explain_quick_select *quick_info;
  
  
  Explain_range_checked_fer *range_checked_fer;
 
  bool full_scan_on_null_key;

  
  EXPLAIN_BKA_TYPE bka_type;

  bool start_dups_weedout;
  bool end_dups_weedout;
  
  
  Item *where_cond;
  Item *cache_cond;
  
  
  Item *pushed_index_cond;

  Explain_basic_join *sjm_nest;
  
  
  Explain_aggr_filesort *pre_join_sort;

  

  
  Table_access_tracker tracker;
  Exec_time_tracker op_tracker;
  Table_access_tracker jbuf_tracker;
  
  int print_explain(select_result_sink *output, uint8 explain_flags, 
                    bool is_analyze,
                    uint select_id, const char *select_type,
                    bool using_temporary, bool using_filesort);
  void print_explain_json(Explain_query *query, Json_writer *writer,
                          bool is_analyze);

private:
  void append_tag_name(String *str, enum explain_extra_tag tag);
  void fill_key_str(String *key_str, bool is_json) const;
  void fill_key_len_str(String *key_len_str) const;
  double get_r_filtered();
  void tag_to_json(Json_writer *writer, enum explain_extra_tag tag);
};




class Explain_update : public Explain_node
{
public:

  Explain_update(MEM_ROOT *root, bool is_analyze) : 
    Explain_node(root),
    filesort_tracker(NULL),
    command_tracker(is_analyze)
  {}

  virtual enum explain_node_type get_type() { return EXPLAIN_UPDATE; }
  virtual int get_select_id() { return 1;  }

  const char *select_type;

  StringBuffer<32> used_partitions;
  String_list used_partitions_list;
  bool used_partitions_set;

  bool impossible_where;
  bool no_partitions;
  StringBuffer<64> table_name;

  enum join_type jtype;
  String_list possible_keys;

  
  Explain_index_use key;

  
  StringBuffer<64> mrr_type;
  
  Explain_quick_select *quick_info;

  bool using_where;
  Item *where_cond;

  ha_rows rows;

  bool using_io_buffer;

  
  Table_access_tracker buf_tracker;
  
  bool is_using_filesort() { return filesort_tracker? true: false; }
  
  Filesort_tracker *filesort_tracker;

  
  Table_access_tracker tracker;

  
  Time_and_counter_tracker command_tracker;
  
  
  Exec_time_tracker table_tracker;

  virtual int print_explain(Explain_query *query, select_result_sink *output, 
                            uint8 explain_flags, bool is_analyze);
  virtual void print_explain_json(Explain_query *query, Json_writer *writer,
                                  bool is_analyze);
};




class Explain_insert : public Explain_node
{
public:
  Explain_insert(MEM_ROOT *root) : 
  Explain_node(root)
  {}

  StringBuffer<64> table_name;

  enum explain_node_type get_type() { return EXPLAIN_INSERT; }
  int get_select_id() { return 1;  }

  int print_explain(Explain_query *query, select_result_sink *output, 
                    uint8 explain_flags, bool is_analyze);
  void print_explain_json(Explain_query *query, Json_writer *writer, 
                          bool is_analyze);
};




class Explain_delete: public Explain_update
{
public:
  Explain_delete(MEM_ROOT *root, bool is_analyze) : 
  Explain_update(root, is_analyze)
  {}

  
  bool deleting_all_rows;

  virtual enum explain_node_type get_type() { return EXPLAIN_DELETE; }
  virtual int get_select_id() { return 1;  }

  virtual int print_explain(Explain_query *query, select_result_sink *output, 
                            uint8 explain_flags, bool is_analyze);
  virtual void print_explain_json(Explain_query *query, Json_writer *writer,
                                  bool is_analyze);
};


#endif 
