#ifndef TABLE_INCLUDED
#define TABLE_INCLUDED


#include "sql_plist.h"
#include "sql_list.h"                           
#include "mdl.h"
#include "datadict.h"
#include "sql_string.h"                         
#include "lex_string.h"

#ifndef MYSQL_CLIENT

#include "hash.h"                               
#include "handler.h"                
#include "mysql_com.h"              
#include "thr_lock.h"                  
#include "filesort_utils.h"
#include "parse_file.h"



class Item;				
typedef Item (*Item_ptr);
class Item_subselect;
class Item_field;
class GRANT_TABLE;
class st_select_lex_unit;
class st_select_lex;
class partition_info;
class COND_EQUAL;
class Security_context;
struct TABLE_LIST;
class ACL_internal_schema_access;
class ACL_internal_table_access;
class Field;
class Table_statistics;
class With_element;
struct TDC_element;
class Virtual_column_info;
class Table_triggers_list;
class TMP_TABLE_PARAM;
class SEQUENCE;


typedef ulonglong nested_join_map;


#define tmp_file_prefix "#sql"			
#define tmp_file_prefix_length 4
#define TMP_TABLE_KEY_EXTRA 8



enum enum_table_ref_type
{
  
  TABLE_REF_NULL= 0,
  TABLE_REF_VIEW,
  TABLE_REF_BASE_TABLE,
  TABLE_REF_I_S_TABLE,
  TABLE_REF_TMP_TABLE
};






class Object_creation_ctx
{
public:
  Object_creation_ctx *set_n_backup(THD *thd);

  void restore_env(THD *thd, Object_creation_ctx *backup_ctx);

protected:
  Object_creation_ctx() {}
  virtual Object_creation_ctx *create_backup_ctx(THD *thd) const = 0;

  virtual void change_env(THD *thd) const = 0;

public:
  virtual ~Object_creation_ctx()
  { }
};





class Default_object_creation_ctx : public Object_creation_ctx
{
public:
  CHARSET_INFO *get_client_cs()
  {
    return m_client_cs;
  }

  CHARSET_INFO *get_connection_cl()
  {
    return m_connection_cl;
  }

protected:
  Default_object_creation_ctx(THD *thd);

  Default_object_creation_ctx(CHARSET_INFO *client_cs,
                              CHARSET_INFO *connection_cl);

protected:
  virtual Object_creation_ctx *create_backup_ctx(THD *thd) const;

  virtual void change_env(THD *thd) const;

protected:
  
  CHARSET_INFO *m_client_cs;

  
  CHARSET_INFO *m_connection_cl;
};

class Query_arena;





class View_creation_ctx : public Default_object_creation_ctx,
                          public Sql_alloc
{
public:
  static View_creation_ctx *create(THD *thd);

  static View_creation_ctx *create(THD *thd,
                                   TABLE_LIST *view);

private:
  View_creation_ctx(THD *thd)
    : Default_object_creation_ctx(thd)
  { }
};





typedef int (*fast_field_copier)(Field *to, Field *from);


typedef struct st_order {
  struct st_order *next;
  Item	 **item;			
  Item	 *item_ptr;			
  
  fast_field_copier fast_field_copier_func;
  
  Field  *fast_field_copier_setup;
  int    counter;                       
  enum enum_order {
    ORDER_NOT_RELEVANT,
    ORDER_ASC,
    ORDER_DESC
  };

  enum_order direction;                 
  bool	 in_field_list;			
  bool   counter_used;                  
  Field  *field;			
  char	 *buff;				
  table_map used; 
  table_map depend_map;
} ORDER;


struct st_grant_internal_info
{
  
  bool m_schema_lookup_done;
  
  const ACL_internal_schema_access *m_schema_access;
  
  bool m_table_lookup_done;
  
  const ACL_internal_table_access *m_table_access;
};
typedef struct st_grant_internal_info GRANT_INTERNAL_INFO;


typedef struct st_grant_info
{
  
  GRANT_TABLE *grant_table_user;
  GRANT_TABLE *grant_table_role;
  
  uint version;
  
  ulong privilege;
  
  ulong want_privilege;
  
  ulong orig_want_privilege;
  
  GRANT_INTERNAL_INFO m_internal;
} GRANT_INFO;

enum tmp_table_type
{
  NO_TMP_TABLE, NON_TRANSACTIONAL_TMP_TABLE, TRANSACTIONAL_TMP_TABLE,
  INTERNAL_TMP_TABLE, SYSTEM_TMP_TABLE
};
enum release_type { RELEASE_NORMAL, RELEASE_WAIT_FOR_DROP };

enum enum_vcol_update_mode
{
  VCOL_UPDATE_FOR_READ= 0,
  VCOL_UPDATE_FOR_WRITE,
  VCOL_UPDATE_FOR_DELETE,
  VCOL_UPDATE_INDEXED,
  VCOL_UPDATE_INDEXED_FOR_UPDATE,
  VCOL_UPDATE_FOR_REPLACE
};



enum field_visibility_t {
  VISIBLE= 0,
  INVISIBLE_USER,
  
  INVISIBLE_SYSTEM,
  INVISIBLE_FULL
};

#define INVISIBLE_MAX_BITS 3



enum enum_table_category
{
  
  TABLE_UNKNOWN_CATEGORY=0,

  
  TABLE_CATEGORY_TEMPORARY=1,

  
  TABLE_CATEGORY_USER=2,

  
  TABLE_CATEGORY_SYSTEM=3,

  
  
  TABLE_CATEGORY_INFORMATION=4,

  
  TABLE_CATEGORY_LOG=5,

  
  TABLE_CATEGORY_PERFORMANCE=6
};
typedef enum enum_table_category TABLE_CATEGORY;

TABLE_CATEGORY get_table_category(const LEX_CSTRING *db,
                                  const LEX_CSTRING *name);


typedef struct st_table_field_type
{
  LEX_CSTRING name;
  LEX_CSTRING type;
  LEX_CSTRING cset;
} TABLE_FIELD_TYPE;


typedef struct st_table_field_def
{
  uint count;
  const TABLE_FIELD_TYPE *field;
  uint primary_key_parts;
  const uint *primary_key_columns;
} TABLE_FIELD_DEF;


class Table_check_intact
{
protected:
  bool has_keys;
  virtual void report_error(uint code, const char *fmt, ...)= 0;

public:
  Table_check_intact(bool keys= false) : has_keys(keys) {}
  virtual ~Table_check_intact() {}

  
  bool check(TABLE *table, const TABLE_FIELD_DEF *table_def);
};



class Table_check_intact_log_error : public Table_check_intact
{
protected:
  void report_error(uint, const char *fmt, ...);
public:
  Table_check_intact_log_error() : Table_check_intact(true) {}
};




class Wait_for_flush : public MDL_wait_for_subgraph
{
  MDL_context *m_ctx;
  TABLE_SHARE *m_share;
  uint m_deadlock_weight;
public:
  Wait_for_flush(MDL_context *ctx_arg, TABLE_SHARE *share_arg,
               uint deadlock_weight_arg)
    : m_ctx(ctx_arg), m_share(share_arg),
      m_deadlock_weight(deadlock_weight_arg)
  {}

  MDL_context *get_ctx() const { return m_ctx; }

  virtual bool accept_visitor(MDL_wait_for_graph_visitor *dvisitor);

  virtual uint get_deadlock_weight() const;

  
  Wait_for_flush *next_in_share;
  Wait_for_flush **prev_in_share;
};


typedef I_P_List <Wait_for_flush,
                  I_P_List_adapter<Wait_for_flush,
                                   &Wait_for_flush::next_in_share,
                                   &Wait_for_flush::prev_in_share> >
                 Wait_for_flush_list;


enum open_frm_error {
  OPEN_FRM_OK = 0,
  OPEN_FRM_OPEN_ERROR,
  OPEN_FRM_READ_ERROR,
  OPEN_FRM_CORRUPTED,
  OPEN_FRM_DISCOVER,
  OPEN_FRM_ERROR_ALREADY_ISSUED,
  OPEN_FRM_NOT_A_VIEW,
  OPEN_FRM_NOT_A_TABLE,
  OPEN_FRM_NEEDS_REBUILD
};



struct TABLE_STATISTICS_CB
{
  MEM_ROOT  mem_root; 
  Table_statistics *table_stats; 
  bool stats_can_be_read;        
  bool stats_is_read;            
  bool histograms_can_be_read;
  bool histograms_are_read;   
};



struct TABLE_SHARE
{
  TABLE_SHARE() {}                    

  
  TABLE_CATEGORY table_category;

  
  HASH	name_hash;			
  MEM_ROOT mem_root;
  TYPELIB keynames;			
  TYPELIB fieldnames;			
  TYPELIB *intervals;			
  mysql_mutex_t LOCK_ha_data;           
  mysql_mutex_t LOCK_share;             

  TDC_element *tdc;

  LEX_CUSTRING tabledef_version;

  engine_option_value *option_list;     
  ha_table_option_struct *option_struct; 

  
  Field **field;
  Field **found_next_number_field;
  KEY  *key_info;			
  Virtual_column_info **check_constraints;
  uint	*blob_field;			
  LEX_CUSTRING vcol_defs;              

  TABLE_STATISTICS_CB stats_cb;

  uchar	*default_values;		
  LEX_CSTRING comment;			
  CHARSET_INFO *table_charset;		

  MY_BITMAP *check_set;                 
  MY_BITMAP all_set;
  
  LEX_CSTRING table_cache_key;
  LEX_CSTRING db;                        
  LEX_CSTRING table_name;                
  LEX_CSTRING path;                	
  LEX_CSTRING normalized_path;		
  LEX_CSTRING connect_string;

  
  key_map keys_in_use;
  key_map keys_for_keyread;
  ha_rows min_rows, max_rows;		
  ulong   avg_row_length;		
  ulong   mysql_version;		
  ulong   reclength;			
  
  ulong   stored_rec_length;            

  plugin_ref db_plugin;			
  inline handlerton *db_type() const	
  { 
    return is_view   ? view_pseudo_hton :
           db_plugin ? plugin_hton(db_plugin) : NULL;
  }
  enum row_type row_type;		
  enum Table_type table_type;
  enum tmp_table_type tmp_table;

  
  enum ha_choice transactional;
  
  enum ha_choice page_checksum;

  uint key_block_size;			
  uint stats_sample_pages;		
  enum_stats_auto_recalc stats_auto_recalc; 
  uint null_bytes, last_null_bit_pos;
  
  uint null_bytes_for_compare;
  uint fields;                          
  
  uint stored_fields;
  uint virtual_fields;                  
  
  uint virtual_not_stored_blob_fields;
  uint null_fields;                     
  uint blob_fields;                     
  uint varchar_fields;                  
  uint default_fields;                  
  uint visible_fields;                  

  uint default_expressions;
  uint table_check_constraints, field_check_constraints;

  uint rec_buff_length;                 
  uint keys, key_parts;
  uint ext_key_parts;       
  uint max_key_length, max_unique_length, total_key_length;
  uint uniques;                         
  uint db_create_options;		
  uint db_options_in_use;		
  uint db_record_offset;		
  uint rowid_field_offset;		
  
  uint primary_key;                     
  uint next_number_index;               
  uint next_number_key_offset;          
  uint next_number_keypart;             
  enum open_frm_error error;            
  uint open_errno;                      
  uint column_bitmap_size;
  uchar frm_version;

  enum enum_v_keys { NOT_INITIALIZED=0, NO_V_KEYS, V_KEYS };
  enum_v_keys check_set_initialized;

  bool use_ext_keys;                    
  bool null_field_first;
  bool system;                          
  bool not_usable_by_query_cache;
  bool no_replicate;
  bool crashed;
  bool is_view;
  bool can_cmp_whole_record;
  bool table_creation_was_logged;
  bool non_determinstic_insert;
  bool vcols_need_refixing;
  bool has_update_default_function;
  bool can_do_row_logging;              

  ulong table_map_id;                   

  
  ulong incompatible_version;

  
  const File_parser *view_def;

  
  SEQUENCE *sequence;

#ifdef WITH_PARTITION_STORAGE_ENGINE
  
  bool auto_partitioned;
  char *partition_info_str;
  uint  partition_info_str_len;
  uint  partition_info_buffer_size;
  plugin_ref default_part_plugin;
#endif

  

  vers_sys_type_t versioned;
  uint16 row_start_field;
  uint16 row_end_field;

  Field *vers_start_field()
  {
    return field[row_start_field];
  }

  Field *vers_end_field()
  {
    return field[row_end_field];
  }

  
  const TABLE_FIELD_DEF *table_field_def_cache;

  
  Handler_share *ha_share;

  
  PSI_table_share *m_psi;

  inline void reset() { bzero((void*)this, sizeof(*this)); }

  

  void set_table_cache_key(char *key_buff, uint key_length)
  {
    table_cache_key.str= key_buff;
    table_cache_key.length= key_length;
    
    db.str=            table_cache_key.str;
    db.length=         strlen(db.str);
    table_name.str=    db.str + db.length + 1;
    table_name.length= strlen(table_name.str);
  }


  

  void set_table_cache_key(char *key_buff, const char *key, uint key_length)
  {
    memcpy(key_buff, key, key_length);
    set_table_cache_key(key_buff, key_length);
  }

  inline bool require_write_privileges()
  {
    return (table_category == TABLE_CATEGORY_LOG);
  }

  inline ulong get_table_def_version()
  {
    return table_map_id;
  }

  
  enum enum_table_ref_type get_table_ref_type() const
  {
    if (is_view)
      return TABLE_REF_VIEW;
    switch (tmp_table) {
    case NO_TMP_TABLE:
      return TABLE_REF_BASE_TABLE;
    case SYSTEM_TMP_TABLE:
      return TABLE_REF_I_S_TABLE;
    default:
      return TABLE_REF_TMP_TABLE;
    }
  }
  
  ulong get_table_ref_version() const
  {
    return (tmp_table == SYSTEM_TMP_TABLE) ? 0 : table_map_id;
  }

  bool visit_subgraph(Wait_for_flush *waiting_ticket,
                      MDL_wait_for_graph_visitor *gvisitor);

  bool wait_for_old_version(THD *thd, struct timespec *abstime,
                            uint deadlock_weight);
  
  void destroy();

  void set_use_ext_keys_flag(bool fl) 
  {
    use_ext_keys= fl;
  }
  
  uint actual_n_key_parts(THD *thd);

  LEX_CUSTRING *frm_image; 

  
  int init_from_binary_frm_image(THD *thd, bool write,
                                 const uchar *frm_image, size_t frm_length);

  
  int init_from_sql_statement_string(THD *thd, bool write,
                                     const char *sql, size_t sql_length);
  
  bool write_frm_image(const uchar *frm_image, size_t frm_length);

  bool write_frm_image(void)
  { return frm_image ? write_frm_image(frm_image->str, frm_image->length) : 0; }

  
  bool read_frm_image(const uchar **frm_image, size_t *frm_length);

  
  void free_frm_image(const uchar *frm);
};




class Blob_mem_storage: public Sql_alloc
{
private:
  MEM_ROOT storage;
  
  bool truncated_value;
public:
  Blob_mem_storage() :truncated_value(false)
  {
    init_alloc_root(&storage, "Blob_mem_storage", MAX_FIELD_VARCHARLENGTH, 0,
                    MYF(0));
  }
  ~ Blob_mem_storage()
  {
    free_root(&storage, MYF(0));
  }
  void reset()
  {
    free_root(&storage, MYF(MY_MARK_BLOCKS_FREE));
    truncated_value= false;
  }
  
  char *store(const char *from, size_t length)
  {
    return (char*) memdup_root(&storage, from, length);
  }
  void set_truncated_value(bool is_truncated_value)
  {
    truncated_value= is_truncated_value;
  }
  bool is_truncated_value() { return truncated_value; }
};



enum index_hint_type
{
  INDEX_HINT_IGNORE,
  INDEX_HINT_USE,
  INDEX_HINT_FORCE
};

struct st_cond_statistic;

#define      CHECK_ROW_FOR_NULLS_TO_REJECT   (1 << 0)
#define      REJECT_ROW_DUE_TO_NULL_FIELDS   (1 << 1)


typedef Bitmap<MAX_FIELDS> Field_map;

class SplM_opt_info;

struct TABLE
{
  TABLE() {}                               

  TABLE_SHARE	*s;
  handler	*file;
  TABLE *next, *prev;

private:
  
  TABLE *share_all_next, **share_all_prev;
  TABLE *global_free_next, **global_free_prev;
  friend struct All_share_tables;
  friend struct Table_cache_instance;

public:

  uint32 instance; 
  THD	*in_use;                        

  uchar *record[3];			
  uchar *write_row_record;		
  uchar *insert_values;                  
  
  key_map covering_keys;
  key_map quick_keys, intersect_keys;
  
  key_map keys_in_use_for_query;
  
  key_map keys_in_use_for_group_by;
  
  key_map keys_in_use_for_order_by;
  KEY  *key_info;			

  Field **field;                        
  Field **vfield;                       
  Field **default_field;                
  Field *next_number_field;		
  Field *found_next_number_field;	
  Virtual_column_info **check_constraints;

  
  Table_triggers_list *triggers;
  TABLE_LIST *pos_in_table_list;
  
  TABLE_LIST *pos_in_locked_tables;
  
  TABLE_LIST *internal_tables;

  
  ORDER		*group;
  String	alias;            	  
  uchar		*null_flags;
  MY_BITMAP     def_read_set, def_write_set, tmp_set;
  MY_BITMAP     def_rpl_write_set;
  MY_BITMAP     eq_join_set;         
  MY_BITMAP     cond_set;   
  
  MY_BITMAP     *read_set, *write_set, *rpl_write_set;
  
  MY_BITMAP     *vcol_set, *def_vcol_set;
  
  MY_BITMAP	has_value_set;

  
  query_id_t	query_id;

  
  Table_statistics *collected_stats;

   
  ha_rows used_stat_records;

  
  ha_rows	quick_rows[MAX_KEY];
  double 	quick_costs[MAX_KEY];

  
  key_part_map  const_key_parts[MAX_KEY];

  uint		quick_key_parts[MAX_KEY];
  uint		quick_n_ranges[MAX_KEY];

  
  ha_rows       quick_condition_rows;

  double cond_selectivity;
  List<st_cond_statistic> *cond_selectivity_sampling_explain;

  table_map	map;                    

  uint          lock_position;          
  uint          lock_data_start;        
  uint          lock_count;             
  uint		tablenr,used_fields;
  uint          temp_pool_slot;		
  uint		status;                 
  uint		db_stat;		
  
  uint          derived_select_number;
  
  uint maybe_null;
  int		current_lock;           
  bool copy_blobs;			
  
  bool next_number_field_updated;

  
  bool null_row;
  
  bool no_rows_with_nulls;
         
  uint8 null_catch_flags;

  
  bool force_index;

  
  bool force_index_order;

  
  bool force_index_group;
  
  bool distinct;
  bool const_table,no_rows, used_for_duplicate_elimination;
  
  bool keep_row_order;

  bool no_keyread;
  
  bool locked_by_logger;
  bool locked_by_name;
  bool fulltext_searched;
  bool no_cache;
  
  bool open_by_handler;
  
  bool auto_increment_field_not_null;
  bool insert_or_update;             
  bool alias_name_used;              
  bool get_fields_in_item_tree;      
  bool m_needs_reopen;
private:
  bool created;    
public:
#ifdef HAVE_REPLICATION
  
  bool master_had_triggers;
#endif

  REGINFO reginfo;			
  MEM_ROOT mem_root;
  
  Blob_mem_storage *blob_storage;
  GRANT_INFO grant;
  
  Query_arena *expr_arena;
#ifdef WITH_PARTITION_STORAGE_ENGINE
  partition_info *part_info;            
  
  bool all_partitions_pruned_away;
#endif
  uint max_keys; 
  bool stats_is_read;     
  bool histograms_are_read;
  MDL_ticket *mdl_ticket;

  
  SplM_opt_info *spl_opt_info;
  key_map keys_usable_for_splitting;


  inline void reset() { bzero((void*)this, sizeof(*this)); }
  void init(THD *thd, TABLE_LIST *tl);
  bool fill_item_list(List<Item> *item_list) const;
  void reset_item_list(List<Item> *item_list, uint skip) const;
  void clear_column_bitmaps(void);
  void prepare_for_position(void);
  MY_BITMAP *prepare_for_keyread(uint index, MY_BITMAP *map);
  MY_BITMAP *prepare_for_keyread(uint index)
  { return prepare_for_keyread(index, &tmp_set); }
  void mark_columns_used_by_index(uint index, MY_BITMAP *map);
  void mark_columns_used_by_index_no_reset(uint index, MY_BITMAP *map);
  void restore_column_maps_after_keyread(MY_BITMAP *backup);
  void mark_auto_increment_column(void);
  void mark_columns_needed_for_update(void);
  void mark_columns_needed_for_delete(void);
  void mark_columns_needed_for_insert(void);
  void mark_columns_per_binlog_row_image(void);
  bool mark_virtual_col(Field *field);
  bool mark_virtual_columns_for_write(bool insert_fl);
  bool check_virtual_columns_marked_for_read();
  bool check_virtual_columns_marked_for_write();
  void mark_default_fields_for_write(bool insert_fl);
  void mark_columns_used_by_virtual_fields(void);
  void mark_check_constraint_columns_for_read(void);
  int verify_constraints(bool ignore_failure);
  inline void column_bitmaps_set(MY_BITMAP *read_set_arg)
  {
    read_set= read_set_arg;
    if (file)
      file->column_bitmaps_signal();
  }
  inline void column_bitmaps_set(MY_BITMAP *read_set_arg,
                                 MY_BITMAP *write_set_arg)
  {
    read_set= read_set_arg;
    write_set= write_set_arg;
    if (file)
      file->column_bitmaps_signal();
  }
  inline void column_bitmaps_set(MY_BITMAP *read_set_arg,
                                 MY_BITMAP *write_set_arg,
                                 MY_BITMAP *vcol_set_arg)
  {
    read_set= read_set_arg;
    write_set= write_set_arg;
    vcol_set= vcol_set_arg;
    if (file)
      file->column_bitmaps_signal();
  }
  inline void column_bitmaps_set_no_signal(MY_BITMAP *read_set_arg,
                                           MY_BITMAP *write_set_arg)
  {
    read_set= read_set_arg;
    write_set= write_set_arg;
  }
  inline void column_bitmaps_set_no_signal(MY_BITMAP *read_set_arg,
                                           MY_BITMAP *write_set_arg,
                                           MY_BITMAP *vcol_set_arg)
  {
    read_set= read_set_arg;
    write_set= write_set_arg;
    vcol_set= vcol_set_arg;
  }
  inline void use_all_columns()
  {
    column_bitmaps_set(&s->all_set, &s->all_set);
  }
  inline void default_column_bitmaps()
  {
    read_set= &def_read_set;
    write_set= &def_write_set;
    vcol_set= def_vcol_set;                     
    rpl_write_set= 0;
  }
  
  inline bool needs_reopen()
  { return !db_stat || m_needs_reopen; }

  bool alloc_keys(uint key_count);
  bool check_tmp_key(uint key, uint key_parts,
                     uint (*next_field_no) (uchar *), uchar *arg);
  bool add_tmp_key(uint key, uint key_parts,
                   uint (*next_field_no) (uchar *), uchar *arg,
                   bool unique);
  void create_key_part_by_field(KEY_PART_INFO *key_part_info,
                                Field *field, uint fieldnr);
  void use_index(int key_to_save);
  void set_table_map(table_map map_arg, uint tablenr_arg)
  {
    map= map_arg;
    tablenr= tablenr_arg;
  }

  
  bool is_created() const { return created; }

  
  void set_created()
  {
    if (created)
      return;
    if (file->keyread_enabled())
      file->extra(HA_EXTRA_KEYREAD);
    created= true;
  }

  
  bool is_filled_at_execution();

  bool update_const_key_parts(COND *conds);

  my_ptrdiff_t default_values_offset() const
  { return (my_ptrdiff_t) (s->default_values - record[0]); }

  void move_fields(Field **ptr, const uchar *to, const uchar *from);
  void remember_blob_values(String *blob_storage);
  void restore_blob_values(String *blob_storage);

  uint actual_n_key_parts(KEY *keyinfo);
  ulong actual_key_flags(KEY *keyinfo);
  int update_virtual_field(Field *vf);
  int update_virtual_fields(handler *h, enum_vcol_update_mode update_mode);
  int update_default_fields(bool update, bool ignore_errors);
  void reset_default_fields();
  inline ha_rows stat_records() { return used_stat_records; }

  void prepare_triggers_for_insert_stmt_or_event();
  bool prepare_triggers_for_delete_stmt_or_event();
  bool prepare_triggers_for_update_stmt_or_event();

  Field **field_to_fill();
  bool validate_default_values_of_unset_fields(THD *thd) const;

  bool insert_all_rows_into_tmp_table(THD *thd, 
                                      TABLE *tmp_table,
                                      TMP_TABLE_PARAM *tmp_table_param,
                                      bool with_cleanup);
  Field *find_field_by_name(LEX_CSTRING *str) const;
  bool export_structure(THD *thd, class Row_definition_list *defs);
  bool is_splittable() { return spl_opt_info != NULL; }
  void set_spl_opt_info(SplM_opt_info *spl_info);
  void deny_splitting();
  double get_materialization_cost(); 
  void add_splitting_info_for_key_field(struct KEY_FIELD *key_field);

  
  bool vers_write;

  bool versioned() const
  {
    DBUG_ASSERT(s);
    return s->versioned;
  }

  bool versioned(vers_sys_type_t type) const
  {
    DBUG_ASSERT(s);
    DBUG_ASSERT(type);
    return s->versioned == type;
  }

  bool versioned_write(vers_sys_type_t type= VERS_UNDEFINED) const
  {
    DBUG_ASSERT(versioned() || !vers_write);
    return versioned(type) ? vers_write : false;
  }

  Field *vers_start_field() const
  {
    DBUG_ASSERT(s && s->versioned);
    return field[s->row_start_field];
  }

  Field *vers_end_field() const
  {
    DBUG_ASSERT(s && s->versioned);
    return field[s->row_end_field];
  }

  ulonglong vers_start_id() const;
  ulonglong vers_end_id() const;

  int delete_row();
  void vers_update_fields();
  void vers_update_end();


#define VERSIONING_FIELDS 2
};




struct TABLE_share
{
  static inline TABLE **next_ptr(TABLE *l)
  {
    return &l->next;
  }
  static inline TABLE ***prev_ptr(TABLE *l)
  {
    return (TABLE ***) &l->prev;
  }
};

struct All_share_tables
{
  static inline TABLE **next_ptr(TABLE *l)
  {
    return &l->share_all_next;
  }
  static inline TABLE ***prev_ptr(TABLE *l)
  {
    return &l->share_all_prev;
  }
};

typedef I_P_List <TABLE, All_share_tables> All_share_tables_list;

enum enum_schema_table_state
{ 
  NOT_PROCESSED= 0,
  PROCESSED_BY_CREATE_SORT_INDEX,
  PROCESSED_BY_JOIN_EXEC
};

enum enum_fk_option { FK_OPTION_UNDEF, FK_OPTION_RESTRICT, FK_OPTION_CASCADE,
               FK_OPTION_SET_NULL, FK_OPTION_NO_ACTION, FK_OPTION_SET_DEFAULT};

typedef struct st_foreign_key_info
{
  LEX_CSTRING *foreign_id;
  LEX_CSTRING *foreign_db;
  LEX_CSTRING *foreign_table;
  LEX_CSTRING *referenced_db;
  LEX_CSTRING *referenced_table;
  enum_fk_option update_method;
  enum_fk_option delete_method;
  LEX_CSTRING *referenced_key_name;
  List<LEX_CSTRING> foreign_fields;
  List<LEX_CSTRING> referenced_fields;
} FOREIGN_KEY_INFO;

LEX_CSTRING *fk_option_name(enum_fk_option opt);
bool fk_modifies_child(enum_fk_option opt);

#define MY_I_S_MAYBE_NULL 1U
#define MY_I_S_UNSIGNED   2U


#define SKIP_OPEN_TABLE 0U               
#define OPEN_FRM_ONLY   1U               
#define OPEN_FULL_TABLE 2U               

typedef struct st_field_info
{
  
  const char* field_name;
  
  uint field_length;
  
  enum enum_field_types field_type;
  int value;
  
  uint field_flags;        
  const char* old_name;
  
  uint open_method;
} ST_FIELD_INFO;


struct TABLE_LIST;
typedef class Item COND;

typedef struct st_schema_table
{
  const char *table_name;
  ST_FIELD_INFO *fields_info;
  
  int (*reset_table) ();
  
  int (*fill_table) (THD *thd, TABLE_LIST *tables, COND *cond);
  
  int (*old_format) (THD *thd, struct st_schema_table *schema_table);
  int (*process_table) (THD *thd, TABLE_LIST *tables, TABLE *table,
                        bool res, const LEX_CSTRING *db_name,
                        const LEX_CSTRING *table_name);
  int idx_field1, idx_field2; 
  bool hidden;
  uint i_s_requested_object;  
} ST_SCHEMA_TABLE;

class IS_table_read_plan;


#define DTYPE_ALGORITHM_UNDEFINED    0U
#define DTYPE_VIEW                   1U
#define DTYPE_TABLE                  2U
#define DTYPE_MERGE                  4U
#define DTYPE_MATERIALIZE            8U
#define DTYPE_MULTITABLE             16U
#define DTYPE_MASK                   (DTYPE_VIEW|DTYPE_TABLE|DTYPE_MULTITABLE)


#define DT_INIT             1U
#define DT_PREPARE          2U
#define DT_OPTIMIZE         4U
#define DT_MERGE            8U
#define DT_MERGE_FOR_INSERT 16U
#define DT_CREATE           32U
#define DT_FILL             64U
#define DT_REINIT           128U
#define DT_PHASES           8U

#define DT_COMMON       (DT_INIT + DT_PREPARE + DT_REINIT + DT_OPTIMIZE)

#define DT_MATERIALIZE  (DT_CREATE + DT_FILL)

#define DT_PHASES_MERGE (DT_COMMON | DT_MERGE | DT_MERGE_FOR_INSERT)
#define DT_PHASES_MATERIALIZE (DT_COMMON | DT_MATERIALIZE)

#define VIEW_ALGORITHM_UNDEFINED 0

#define VIEW_ALGORITHM_INHERIT   DTYPE_VIEW
#define VIEW_ALGORITHM_MERGE    (DTYPE_VIEW | DTYPE_MERGE)
#define VIEW_ALGORITHM_TMPTABLE (DTYPE_VIEW | DTYPE_MATERIALIZE)



#define VIEW_ALGORITHM_UNDEFINED_FRM  0U
#define VIEW_ALGORITHM_MERGE_FRM      1U
#define VIEW_ALGORITHM_TMPTABLE_FRM   2U

#define JOIN_TYPE_LEFT	1U
#define JOIN_TYPE_RIGHT	2U
#define JOIN_TYPE_OUTER 4U	


#define VIEW_CHECK_NONE       0
#define VIEW_CHECK_LOCAL      1
#define VIEW_CHECK_CASCADED   2


#define VIEW_CHECK_OK         0
#define VIEW_CHECK_ERROR      1
#define VIEW_CHECK_SKIP       2


#define MAX_TDC_BLOB_SIZE 65536

class select_unit;
class TMP_TABLE_PARAM;

Item *create_view_field(THD *thd, TABLE_LIST *view, Item **field_ref,
                        LEX_CSTRING *name);

struct Field_translator
{
  Item *item;
  LEX_CSTRING name;
};




class Natural_join_column: public Sql_alloc
{
public:
  Field_translator *view_field;  
  Item_field       *table_field; 
  TABLE_LIST *table_ref; 
  
  bool is_common;
public:
  Natural_join_column(Field_translator *field_param, TABLE_LIST *tab);
  Natural_join_column(Item_field *field_param, TABLE_LIST *tab);
  LEX_CSTRING *name();
  Item *create_item(THD *thd);
  Field *field();
  const char *safe_table_name();
  const char *safe_db_name();
  GRANT_INFO *grant();
};




enum enum_open_type
{
  OT_TEMPORARY_OR_BASE= 0, OT_TEMPORARY_ONLY, OT_BASE_ONLY
};


class SJ_MATERIALIZATION_INFO;
class Index_hint;
class Item_in_subselect;


struct vers_history_point_t
{
  vers_sys_type_t unit;
  Item *item;
};

class Vers_history_point : public vers_history_point_t
{
  void fix_item();

public:
  Vers_history_point() { empty(); }
  Vers_history_point(vers_sys_type_t unit_arg, Item *item_arg)
  {
    unit= unit_arg;
    item= item_arg;
    fix_item();
  }
  Vers_history_point(vers_history_point_t p)
  {
    unit= p.unit;
    item= p.item;
    fix_item();
  }
  void empty() { unit= VERS_UNDEFINED; item= NULL; }
  void print(String *str, enum_query_type, const char *prefix, size_t plen) const;
  bool resolve_unit(THD *thd);
  bool resolve_unit_trx_id(THD *thd)
  {
    if (unit == VERS_UNDEFINED)
      unit= VERS_TRX_ID;
    return false;
  }
  bool resolve_unit_timestamp(THD *thd)
  {
    if (unit == VERS_UNDEFINED)
      unit= VERS_TIMESTAMP;
    return false;
  }
  void bad_expression_data_type_error(const char *type) const;
  bool eq(const vers_history_point_t &point) const;
};

struct vers_select_conds_t
{
  vers_system_time_t type;
  bool used:1;
  Vers_history_point start;
  Vers_history_point end;

  void empty()
  {
    type= SYSTEM_TIME_UNSPECIFIED;
    used= false;
    start.empty();
    end.empty();
  }

  void init(vers_system_time_t _type,
            Vers_history_point _start= Vers_history_point(),
            Vers_history_point _end= Vers_history_point())
  {
    type= _type;
    used= false;
    start= _start;
    end= _end;
  }

  void print(String *str, enum_query_type query_type) const;

  bool init_from_sysvar(THD *thd);

  bool is_set() const
  {
    return type != SYSTEM_TIME_UNSPECIFIED;
  }
  bool resolve_units(THD *thd);
  bool eq(const vers_select_conds_t &conds) const;
};





struct LEX;
class Index_hint;
struct TABLE_LIST
{
  TABLE_LIST() {}                          

  enum prelocking_types
  {
    PRELOCK_NONE, PRELOCK_ROUTINE, PRELOCK_FK
  };

  
  inline void reset() { bzero((void*)this, sizeof(*this)); }
  inline void init_one_table(const LEX_CSTRING *db_arg,
                             const LEX_CSTRING *table_name_arg,
                             const LEX_CSTRING *alias_arg,
                             enum thr_lock_type lock_type_arg)
  {
    enum enum_mdl_type mdl_type;
    if (lock_type_arg >= TL_WRITE_ALLOW_WRITE)
      mdl_type= MDL_SHARED_WRITE;
    else if (lock_type_arg == TL_READ_NO_INSERT)
      mdl_type= MDL_SHARED_NO_WRITE;
    else
      mdl_type= MDL_SHARED_READ;

    reset();
    DBUG_ASSERT(!db_arg->str || strlen(db_arg->str) == db_arg->length);
    DBUG_ASSERT(!table_name_arg->str || strlen(table_name_arg->str) == table_name_arg->length);
    DBUG_ASSERT(!alias_arg || strlen(alias_arg->str) == alias_arg->length);
    db= *db_arg;
    table_name= *table_name_arg;
    alias= (alias_arg ? *alias_arg : *table_name_arg);
    lock_type= lock_type_arg;
    mdl_request.init(MDL_key::TABLE, db.str, table_name.str, mdl_type,
                     MDL_TRANSACTION);
  }

  TABLE_LIST(TABLE *table_arg, thr_lock_type lock_type)
  {
    DBUG_ASSERT(table_arg->s);
    init_one_table(&table_arg->s->db, &table_arg->s->table_name,
                   NULL, lock_type);
    table= table_arg;
  }

  inline void init_one_table_for_prelocking(const LEX_CSTRING *db_arg,
                                            const LEX_CSTRING *table_name_arg,
                                            const LEX_CSTRING *alias_arg,
                                            enum thr_lock_type lock_type_arg,
                                            prelocking_types prelocking_type,
                                            TABLE_LIST *belong_to_view_arg,
                                            uint8 trg_event_map_arg,
                                            TABLE_LIST ***last_ptr)

  {
    init_one_table(db_arg, table_name_arg, alias_arg, lock_type_arg);
    cacheable_table= 1;
    prelocking_placeholder= prelocking_type;
    open_type= (prelocking_type == PRELOCK_ROUTINE ?
                OT_TEMPORARY_OR_BASE :
                OT_BASE_ONLY);
    belong_to_view= belong_to_view_arg;
    trg_event_map= trg_event_map_arg;

    **last_ptr= this;
    prev_global= *last_ptr;
    *last_ptr= &next_global;
  }


  
  TABLE_LIST *next_local;
  
  TABLE_LIST *next_global, **prev_global;
  LEX_CSTRING   db;
  LEX_CSTRING   table_name;
  LEX_CSTRING   schema_table_name;
  LEX_CSTRING   alias;
  const char    *option;                
  Item		*on_expr;		

  Item          *sj_on_expr;
  
  table_map     sj_inner_tables;
  
  uint          sj_in_exprs;
  
  
  Item_in_subselect  *sj_subq_pred;

  table_map     original_subq_pred_used_tables;

  
  Item_in_subselect  *jtbm_subselect;
  
  uint jtbm_table_no;

  SJ_MATERIALIZATION_INFO *sj_mat_info;

  
  Item          *prep_on_expr;
  COND_EQUAL    *cond_equal;            
  
  TABLE_LIST *natural_join;
  
  bool is_natural_join;
  
  List<String> *join_using_fields;
  
  List<Natural_join_column> *join_columns;
  
  bool is_join_columns_complete;

  
  TABLE_LIST *next_name_resolution_table;
  
  List<Index_hint> *index_hints;
  TABLE        *table;                          
  ulonglong         table_id; 
  
  select_unit  *derived_result;
  
  table_map	map;                    
  table_map get_map()
  {
    return jtbm_subselect? table_map(1) << jtbm_table_no : table->map;
  }
  uint get_tablenr()
  {
    return jtbm_subselect? jtbm_table_no : table->tablenr;
  }
  void set_tablenr(uint new_tablenr)
  {
    if (jtbm_subselect)
    {
      jtbm_table_no= new_tablenr;
    }
    if (table)
    {
      table->tablenr= new_tablenr;
      table->map= table_map(1) << new_tablenr;
    }
  }
  
  TABLE_LIST	*correspondent_table;
  
  st_select_lex_unit *derived;		
  With_element *with;          
  
  table_map with_internal_reference_map;
  TABLE_LIST * next_with_rec_ref;
  bool is_derived_with_recursive_reference;
  bool block_handle_derived;
  ST_SCHEMA_TABLE *schema_table;        
  st_select_lex	*schema_select_lex;
  
  bool schema_table_reformed;
  TMP_TABLE_PARAM *schema_table_param;
  
  st_select_lex	*select_lex;
  LEX *view;                    
  Field_translator *field_translation;	
  
  Field_translator *field_translation_end;
  bool field_translation_updated;
  
  TABLE_LIST	*merge_underlying_list;
  
  List<TABLE_LIST> *view_tables;
  
  TABLE_LIST	*belong_to_view;
  
  TABLE_LIST    *belong_to_derived;
  
  TABLE_LIST	*referencing_view;

  table_map view_used_tables;
  table_map     map_exec;
  
  uint          tablenr_exec;
  uint          maybe_null_exec;

  
  TABLE_LIST    *parent_l;
  
  Security_context *security_ctx;
  
  Security_context *view_sctx;
  bool allowed_show;
  Item          *where;                 
  Item          *check_option;          
  LEX_STRING	select_stmt;		
  LEX_CSTRING	md5;			
  LEX_CSTRING	source;			
  LEX_CSTRING	view_db;		
  LEX_CSTRING	view_name;		
  LEX_STRING	timestamp;		
  LEX_USER      definer;                
  ulonglong	file_version;		
  ulonglong	mariadb_version;	
  ulonglong     updatable_view;         
  
  ulonglong	algorithm;
  ulonglong     view_suid;              
  ulonglong     with_check;             
  
  uint8         effective_with_check;
  
  uint8         derived_type;
  GRANT_INFO	grant;
  
  ulonglong     engine_data;
  
  qc_engine_callback callback_func;
  thr_lock_type lock_type;
  uint		outer_join;		
  uint		shared;			
  bool          updatable;		
  bool		straight;		
  bool          updating;               
  bool		force_index;		
  bool          ignore_leaves;          
  bool          crashed;                 
  table_map     dep_tables;             
  table_map     on_expr_dep_tables;     
  struct st_nested_join *nested_join;   
  TABLE_LIST *embedding;             
  List<TABLE_LIST> *join_list;
  bool          lifted;               
  bool		cacheable_table;	
  
  bool		table_in_first_from_clause;
  
  enum enum_open_type open_type;
  
  bool          contain_auto_increment;
  bool          compact_view_format;    
  
  bool          where_processed;
  
  bool          check_option_processed;
  
  Table_type    required_type;
  handlerton	*db_type;		
  char		timestamp_buffer[MAX_DATETIME_WIDTH + 1];
  
  prelocking_types prelocking_placeholder;
  
  enum enum_open_strategy
  {
    
    OPEN_NORMAL= 0,
    
    OPEN_IF_EXISTS,
    
    OPEN_STUB
  } open_strategy;
  
  bool          is_alias;
  
  bool          is_fqtn;

  
  bool          fill_me;
  
  
  bool          merged;
  bool          merged_for_insert;
  bool          sequence;  

  
  List<Item>    used_items;
  
  List<Item>    persistent_used_items;

  

  View_creation_ctx *view_creation_ctx;

  

  LEX_CSTRING view_client_cs_name;
  LEX_CSTRING view_connection_cl_name;

  

  LEX_CSTRING view_body_utf8;

   

  
  uint8 trg_event_map;
  
  bool optimized_away;

  
  bool materialized;
  
  uint i_s_requested_object;

  bool prohibit_cond_pushdown;

  
  uint table_open_method;
  
  enum enum_schema_table_state schema_table_state;

  
  IS_table_read_plan *is_table_read_plan;

  MDL_request mdl_request;

#ifdef WITH_PARTITION_STORAGE_ENGINE
  
  List<String> *partition_names;
#endif 

  void calc_md5(const char *buffer);
  int view_check_option(THD *thd, bool ignore_failure);
  bool create_field_translation(THD *thd);
  bool setup_underlying(THD *thd);
  void cleanup_items();
  bool placeholder()
  {
    return derived || view || schema_table || !table;
  }
  void print(THD *thd, table_map eliminated_tables, String *str, 
             enum_query_type query_type);
  bool check_single_table(TABLE_LIST **table, table_map map,
                          TABLE_LIST *view);
  bool set_insert_values(MEM_ROOT *mem_root);
  void hide_view_error(THD *thd);
  TABLE_LIST *find_underlying_table(TABLE *table);
  TABLE_LIST *first_leaf_for_name_resolution();
  TABLE_LIST *last_leaf_for_name_resolution();

  
  vers_select_conds_t vers_conditions;

  
  TABLE_LIST *find_table_for_update()
  {
    TABLE_LIST *tbl= this;
    while(!tbl->is_multitable() && tbl->single_table_updatable() &&
        tbl->merge_underlying_list)
    {
      tbl= tbl->merge_underlying_list;
    }
    return tbl;
  }
  TABLE *get_real_join_table();
  bool is_leaf_for_name_resolution();
  inline TABLE_LIST *top_table()
    { return belong_to_view ? belong_to_view : this; }
  inline bool prepare_check_option(THD *thd)
  {
    bool res= FALSE;
    if (effective_with_check)
      res= prep_check_option(thd, effective_with_check);
    return res;
  }
  inline bool prepare_where(THD *thd, Item **conds,
                            bool no_where_clause)
  {
    if (!view || is_merged_derived())
      return prep_where(thd, conds, no_where_clause);
    return FALSE;
  }

  void register_want_access(ulong want_access);
  bool prepare_security(THD *thd);
#ifndef NO_EMBEDDED_ACCESS_CHECKS
  Security_context *find_view_security_context(THD *thd);
  bool prepare_view_security_context(THD *thd);
#endif
  
  void reinit_before_use(THD *thd);
  Item_subselect *containing_subselect();

  
  bool process_index_hints(TABLE *table);

  
  inline bool is_table_ref_id_equal(TABLE_SHARE *s) const
  {
    return (m_table_ref_type == s->get_table_ref_type() &&
            m_table_ref_version == s->get_table_ref_version());
  }

  
  inline void set_table_ref_id(TABLE_SHARE *s)
  { set_table_ref_id(s->get_table_ref_type(), s->get_table_ref_version()); }

  inline void set_table_ref_id(enum_table_ref_type table_ref_type_arg,
                        ulong table_ref_version_arg)
  {
    m_table_ref_type= table_ref_type_arg;
    m_table_ref_version= table_ref_version_arg;
  }

  
  inline bool is_non_derived()
  {
    return (!derived_type);
  }
  inline bool is_view_or_derived()
  {
    return (derived_type);
  }
  inline bool is_view()
  {
    return (derived_type & DTYPE_VIEW);
  }
  inline bool is_derived()
  {
    return (derived_type & DTYPE_TABLE);
  }
  bool is_with_table();
  bool is_recursive_with_table();
  bool is_with_table_recursive_reference();
  void register_as_derived_with_rec_ref(With_element *rec_elem);
  bool is_nonrecursive_derived_with_rec_ref();
  bool fill_recursive(THD *thd);

  inline void set_view()
  {
    derived_type= DTYPE_VIEW;
  }
  inline void set_derived()
  {
    derived_type= DTYPE_TABLE;
  }
  inline bool is_merged_derived()
  {
    return (derived_type & DTYPE_MERGE);
  }
  inline void set_merged_derived()
  {
    DBUG_ENTER("set_merged_derived");
    DBUG_PRINT("enter", ("Alias: '%s'  Unit: %p",
                        (alias.str ? alias.str : "<NULL>"),
                         get_unit()));
    derived_type= ((derived_type & DTYPE_MASK) |
                   DTYPE_TABLE | DTYPE_MERGE);
    set_check_merged();
    DBUG_VOID_RETURN;
  }
  inline bool is_materialized_derived()
  {
    return (derived_type & DTYPE_MATERIALIZE);
  }
  void set_materialized_derived()
  {
    DBUG_ENTER("set_materialized_derived");
    DBUG_PRINT("enter", ("Alias: '%s'  Unit: %p",
                        (alias.str ? alias.str : "<NULL>"),
                         get_unit()));
    derived= get_unit();
    derived_type= ((derived_type & (derived ? DTYPE_MASK : DTYPE_VIEW)) |
                   DTYPE_TABLE | DTYPE_MATERIALIZE);
    set_check_materialized();
    DBUG_VOID_RETURN;
  }
  inline bool is_multitable()
  {
    return (derived_type & DTYPE_MULTITABLE);
  }
  inline void set_multitable()
  {
    derived_type|= DTYPE_MULTITABLE;
  }
  bool set_as_with_table(THD *thd, With_element *with_elem);
  void reset_const_table();
  bool handle_derived(LEX *lex, uint phases);

  
  bool is_anonymous_derived_table() const { return derived && !view; }

  
  const char *get_db_name() const { return view != NULL ? view_db.str : db.str; }

  
  const char *get_table_name() const { return view != NULL ? view_name.str : table_name.str; }
  bool is_active_sjm();
  bool is_jtbm() { return MY_TEST(jtbm_subselect != NULL); }
  st_select_lex_unit *get_unit();
  st_select_lex *get_single_select();
  void wrap_into_nested_join(List<TABLE_LIST> &join_list);
  bool init_derived(THD *thd, bool init_view);
  int fetch_number_of_rows();
  bool change_refs_to_fields();

  bool single_table_updatable();

  bool is_inner_table_of_outer_join()
  {
    for (TABLE_LIST *tbl= this; tbl; tbl= tbl->embedding)
    {
      if (tbl->outer_join)
        return true;
    }
    return false;
  } 
  void set_lock_type(THD* thd, enum thr_lock_type lock);
  void check_pushable_cond_for_table(Item *cond);
  Item *build_pushable_cond_for_table(THD *thd, Item *cond); 

  void remove_join_columns()
  {
    if (join_columns)
    {
      join_columns->empty();
      join_columns= NULL;
      is_join_columns_complete= FALSE;
    }
  }

private:
  bool prep_check_option(THD *thd, uint8 check_opt_type);
  bool prep_where(THD *thd, Item **conds, bool no_where_clause);
  void set_check_materialized();
#ifndef DBUG_OFF
  void set_check_merged();
#else
  inline void set_check_merged() {}
#endif
  
  enum enum_table_ref_type m_table_ref_type;
  
  ulong m_table_ref_version;
};

class Item;



class Field_iterator: public Sql_alloc
{
public:
  Field_iterator() {}                         
  virtual ~Field_iterator() {}
  virtual void set(TABLE_LIST *)= 0;
  virtual void next()= 0;
  virtual bool end_of_fields()= 0;              
  virtual LEX_CSTRING *name()= 0;
  virtual Item *create_item(THD *)= 0;
  virtual Field *field()= 0;
};




class Field_iterator_table: public Field_iterator
{
  Field **ptr;
public:
  Field_iterator_table() :ptr(0) {}
  void set(TABLE_LIST *table) { ptr= table->table->field; }
  void set_table(TABLE *table) { ptr= table->field; }
  void next() { ptr++; }
  bool end_of_fields() { return *ptr == 0; }
  LEX_CSTRING *name();
  Item *create_item(THD *thd);
  Field *field() { return *ptr; }
};




class Field_iterator_view: public Field_iterator
{
  Field_translator *ptr, *array_end;
  TABLE_LIST *view;
public:
  Field_iterator_view() :ptr(0), array_end(0) {}
  void set(TABLE_LIST *table);
  void next() { ptr++; }
  bool end_of_fields() { return ptr == array_end; }
  LEX_CSTRING *name();
  Item *create_item(THD *thd);
  Item **item_ptr() {return &ptr->item; }
  Field *field() { return 0; }
  inline Item *item() { return ptr->item; }
  Field_translator *field_translator() { return ptr; }
};




class Field_iterator_natural_join: public Field_iterator
{
  List_iterator_fast<Natural_join_column> column_ref_it;
  Natural_join_column *cur_column_ref;
public:
  Field_iterator_natural_join() :cur_column_ref(NULL) {}
  ~Field_iterator_natural_join() {}
  void set(TABLE_LIST *table);
  void next();
  bool end_of_fields() { return !cur_column_ref; }
  LEX_CSTRING *name() { return cur_column_ref->name(); }
  Item *create_item(THD *thd) { return cur_column_ref->create_item(thd); }
  Field *field() { return cur_column_ref->field(); }
  Natural_join_column *column_ref() { return cur_column_ref; }
};




class Field_iterator_table_ref: public Field_iterator
{
  TABLE_LIST *table_ref, *first_leaf, *last_leaf;
  Field_iterator_table        table_field_it;
  Field_iterator_view         view_field_it;
  Field_iterator_natural_join natural_join_it;
  Field_iterator *field_it;
  void set_field_iterator();
public:
  Field_iterator_table_ref() :field_it(NULL) {}
  void set(TABLE_LIST *table);
  void next();
  bool end_of_fields()
  { return (table_ref == last_leaf && field_it->end_of_fields()); }
  LEX_CSTRING *name() { return field_it->name(); }
  const char *get_table_name();
  const char *get_db_name();
  GRANT_INFO *grant();
  Item *create_item(THD *thd) { return field_it->create_item(thd); }
  Field *field() { return field_it->field(); }
  Natural_join_column *get_or_create_column_ref(THD *thd, TABLE_LIST *parent_table_ref);
  Natural_join_column *get_natural_column_ref();
};


typedef struct st_nested_join
{
  List<TABLE_LIST>  join_list;       
  
  table_map         used_tables;
  table_map         not_null_tables; 
  
  struct st_join_table *first_nested;
  
  uint              counter;
  
  uint              n_tables;
  nested_join_map   nj_map;          
  
  table_map         sj_depends_on;
  
  table_map         sj_corr_tables;
  List<Item_ptr>    sj_outer_expr_list;
  
  bool is_fully_covered() const { return n_tables == counter; }
} NESTED_JOIN;


typedef struct st_changed_table_list
{
  struct	st_changed_table_list *next;
  char		*key;
  size_t  key_length;
} CHANGED_TABLE_LIST;


typedef struct st_open_table_list{
  struct st_open_table_list *next;
  char	*db,*table;
  uint32 in_use,locked;
} OPEN_TABLE_LIST;


static inline my_bitmap_map *tmp_use_all_columns(TABLE *table,
                                                 MY_BITMAP *bitmap)
{
  my_bitmap_map *old= bitmap->bitmap;
  bitmap->bitmap= table->s->all_set.bitmap;
  return old;
}


static inline void tmp_restore_column_map(MY_BITMAP *bitmap,
                                          my_bitmap_map *old)
{
  bitmap->bitmap= old;
}



static inline my_bitmap_map *dbug_tmp_use_all_columns(TABLE *table,
                                                      MY_BITMAP *bitmap)
{
#ifdef DBUG_ASSERT_EXISTS
  return tmp_use_all_columns(table, bitmap);
#else
  return 0;
#endif
}

static inline void dbug_tmp_restore_column_map(MY_BITMAP *bitmap,
                                               my_bitmap_map *old)
{
#ifdef DBUG_ASSERT_EXISTS
  tmp_restore_column_map(bitmap, old);
#endif
}



static inline void dbug_tmp_use_all_columns(TABLE *table,
                                            my_bitmap_map **save,
                                            MY_BITMAP *read_set,
                                            MY_BITMAP *write_set)
{
#ifdef DBUG_ASSERT_EXISTS
  save[0]= read_set->bitmap;
  save[1]= write_set->bitmap;
  (void) tmp_use_all_columns(table, read_set);
  (void) tmp_use_all_columns(table, write_set);
#endif
}


static inline void dbug_tmp_restore_column_maps(MY_BITMAP *read_set,
                                                MY_BITMAP *write_set,
                                                my_bitmap_map **old)
{
#ifdef DBUG_ASSERT_EXISTS
  tmp_restore_column_map(read_set, old[0]);
  tmp_restore_column_map(write_set, old[1]);
#endif
}

bool ok_for_lower_case_names(const char *names);

enum get_table_share_flags {
  GTS_TABLE                = 1,
  GTS_VIEW                 = 2,
  GTS_NOLOCK               = 4,
  GTS_USE_DISCOVERY        = 8,
  GTS_FORCE_DISCOVERY      = 16
};

size_t max_row_length(TABLE *table, MY_BITMAP const *cols, const uchar *data);

void init_mdl_requests(TABLE_LIST *table_list);

enum open_frm_error open_table_from_share(THD *thd, TABLE_SHARE *share,
                       const LEX_CSTRING *alias, uint db_stat, uint prgflag,
                       uint ha_open_flags, TABLE *outparam,
                       bool is_create_table,
                       List<String> *partitions_to_open= NULL);
bool fix_session_vcol_expr(THD *thd, Virtual_column_info *vcol);
bool fix_session_vcol_expr_for_read(THD *thd, Field *field,
                                    Virtual_column_info *vcol);
bool parse_vcol_defs(THD *thd, MEM_ROOT *mem_root, TABLE *table,
                     bool *error_reported);
TABLE_SHARE *alloc_table_share(const char *db, const char *table_name,
                               const char *key, uint key_length);
void init_tmp_table_share(THD *thd, TABLE_SHARE *share, const char *key,
                          uint key_length,
                          const char *table_name, const char *path);
void free_table_share(TABLE_SHARE *share);
enum open_frm_error open_table_def(THD *thd, TABLE_SHARE *share,
                                   uint flags = GTS_TABLE);

void open_table_error(TABLE_SHARE *share, enum open_frm_error error,
                      int db_errno);
void update_create_info_from_table(HA_CREATE_INFO *info, TABLE *form);
bool check_db_name(LEX_STRING *db);
bool check_column_name(const char *name);
bool check_table_name(const char *name, size_t length, bool check_for_path_chars);
int rename_file_ext(const char * from,const char * to,const char * ext);
char *get_field(MEM_ROOT *mem, Field *field);
bool get_field(MEM_ROOT *mem, Field *field, class String *res);

bool validate_comment_length(THD *thd, LEX_CSTRING *comment, size_t max_len,
                             uint err_code, const char *name);

int closefrm(TABLE *table);
void free_blobs(TABLE *table);
void free_field_buffers_larger_than(TABLE *table, uint32 size);
ulong get_form_pos(File file, uchar *head, TYPELIB *save_names);
void append_unescaped(String *res, const char *pos, size_t length);
void prepare_frm_header(THD *thd, uint reclength, uchar *fileinfo,
                        HA_CREATE_INFO *create_info, uint keys, KEY *key_info);
const char *fn_frm_ext(const char *name);


static inline int set_zone(int nr,int min_zone,int max_zone)
{
  if (nr <= min_zone)
    return min_zone;
  if (nr >= max_zone)
    return max_zone;
  return nr;
}


extern LEX_CSTRING PERFORMANCE_SCHEMA_DB_NAME;

extern LEX_CSTRING GENERAL_LOG_NAME;
extern LEX_CSTRING SLOW_LOG_NAME;
extern LEX_CSTRING TRANSACTION_REG_NAME;


extern LEX_CSTRING INFORMATION_SCHEMA_NAME;
extern LEX_CSTRING MYSQL_SCHEMA_NAME;


extern LEX_CSTRING MYSQL_USER_NAME, MYSQL_DB_NAME, MYSQL_PROC_NAME;

inline bool is_infoschema_db(const LEX_CSTRING *name)
{
  return (INFORMATION_SCHEMA_NAME.length == name->length &&
          !my_strcasecmp(system_charset_info,
                         INFORMATION_SCHEMA_NAME.str, name->str));
}

inline void mark_as_null_row(TABLE *table)
{
  table->null_row=1;
  table->status|=STATUS_NULL_ROW;
  bfill(table->null_flags,table->s->null_bytes,255);
}

bool is_simple_order(ORDER *order);

class Open_tables_backup;



class TR_table: public TABLE_LIST
{
  THD *thd;
  Open_tables_backup *open_tables_backup;

public:
  enum field_id_t {
    FLD_TRX_ID= 0,
    FLD_COMMIT_ID,
    FLD_BEGIN_TS,
    FLD_COMMIT_TS,
    FLD_ISO_LEVEL,
    FIELD_COUNT
  };

  enum enabled {NO, MAYBE, YES};
  static enum enabled use_transaction_registry;

  
  TR_table(THD *_thd, bool rw= false);
  
  bool open();
  ~TR_table();
  
  THD *get_thd() const { return thd; }
  
  void store(uint field_id, ulonglong val);
  
  void store(uint field_id, timeval ts);
  
  bool update(ulonglong start_id, ulonglong end_id);
  
  bool query(ulonglong trx_id);
  
  bool query(MYSQL_TIME &commit_time, bool backwards);
  
  bool query_sees(bool &result, ulonglong trx_id1, ulonglong trx_id0,
                  ulonglong commit_id1= 0,
                  enum_tx_isolation iso_level1= ISO_READ_UNCOMMITTED,
                  ulonglong commit_id0= 0);

  
  enum_tx_isolation iso_level() const;
  
  void store_iso_level(enum_tx_isolation iso_level)
  {
    DBUG_ASSERT(iso_level <= ISO_SERIALIZABLE);
    store(FLD_ISO_LEVEL, iso_level + 1);
  }

  
  void warn_schema_incorrect(const char *reason);
  
  bool check(bool error);

  TABLE * operator-> () const
  {
    return table;
  }
  Field * operator[] (uint field_id) const
  {
    DBUG_ASSERT(field_id < FIELD_COUNT);
    return table->field[field_id];
  }
  operator bool () const
  {
    return table;
  }
  bool operator== (const TABLE_LIST &subj) const
  {
    return (!cmp(&db, &subj.db) && !cmp(&table_name, &subj.table_name));
  }
  bool operator!= (const TABLE_LIST &subj) const
  {
    return !(*this == subj);
  }
};

#endif 

#endif 
