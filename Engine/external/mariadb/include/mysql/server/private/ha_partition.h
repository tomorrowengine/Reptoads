#ifndef HA_PARTITION_INCLUDED
#define HA_PARTITION_INCLUDED



#include "sql_partition.h"      
#include "queues.h"             

#define PARTITION_BYTES_IN_POS 2



typedef struct st_part_name_def
{
  uchar *partition_name;
  uint length;
  uint32 part_id;
  my_bool is_subpart;
} PART_NAME_DEF;


class Parts_share_refs
{
public:
  uint num_parts;                              
  Handler_share **ha_shares;                   
  Parts_share_refs()
  {
    num_parts= 0;
    ha_shares= NULL;
  }
  ~Parts_share_refs()
  {
    uint i;
    for (i= 0; i < num_parts; i++)
      delete ha_shares[i];
    delete[] ha_shares;
  }
  bool init(uint arg_num_parts)
  {
    DBUG_ASSERT(!num_parts && !ha_shares);
    num_parts= arg_num_parts;
    
    ha_shares= new Handler_share *[num_parts];
    if (!ha_shares)
    {
      num_parts= 0;
      return true;
    }
    memset(ha_shares, 0, sizeof(Handler_share*) * num_parts);
    return false;
  }
};

class ha_partition;


struct st_partition_ft_info
{
  struct _ft_vft        *please;
  st_partition_ft_info  *next;
  ha_partition          *file;
  FT_INFO               **part_ft_info;
};


#ifdef HAVE_PSI_MUTEX_INTERFACE
extern PSI_mutex_key key_partition_auto_inc_mutex;
#endif


class Partition_share : public Handler_share
{
public:
  bool auto_inc_initialized;
  mysql_mutex_t auto_inc_mutex;                
  ulonglong next_auto_inc_val;                 
  
  bool partition_name_hash_initialized;
  HASH partition_name_hash;
  
  Parts_share_refs partitions_share_refs;
  Partition_share()
    : auto_inc_initialized(false),
    next_auto_inc_val(0),
    partition_name_hash_initialized(false),
    partition_names(NULL)
  {
    mysql_mutex_init(key_partition_auto_inc_mutex,
                    &auto_inc_mutex,
                    MY_MUTEX_INIT_FAST);
  }

  ~Partition_share()
  {
    mysql_mutex_destroy(&auto_inc_mutex);
    if (partition_names)
    {
      my_free(partition_names);
    }
    if (partition_name_hash_initialized)
    {
      my_hash_free(&partition_name_hash);
    }
  }
  
  bool init(uint num_parts);

  
  void release_auto_inc_if_possible(THD *thd, TABLE_SHARE *table_share,
                                    const ulonglong next_insert_id,
                                    const ulonglong max_reserved);

  
  inline void lock_auto_inc()
  {
    mysql_mutex_lock(&auto_inc_mutex);
  }
  
  inline void unlock_auto_inc()
  {
    mysql_mutex_unlock(&auto_inc_mutex);
  }
  
  bool populate_partition_name_hash(partition_info *part_info);
  
  const char *get_partition_name(size_t part_id) const;
private:
  const uchar **partition_names;
  
  bool insert_partition_name_in_hash(const char *name,
                                     uint part_id,
                                     bool is_subpart);
};

typedef struct st_partition_key_multi_range
{
  uint id;
  uchar *key[2];
  uint length[2];
  KEY_MULTI_RANGE key_multi_range;
  range_id_t ptr;
  st_partition_key_multi_range *next;
} PARTITION_KEY_MULTI_RANGE;


typedef struct st_partition_part_key_multi_range
{
  PARTITION_KEY_MULTI_RANGE *partition_key_multi_range;
  st_partition_part_key_multi_range *next;
} PARTITION_PART_KEY_MULTI_RANGE;


class ha_partition;
typedef struct st_partition_part_key_multi_range_hld
{
  ha_partition *partition;
  uint32 part_id;
  PARTITION_PART_KEY_MULTI_RANGE *partition_part_key_multi_range;
} PARTITION_PART_KEY_MULTI_RANGE_HLD;


extern "C" int cmp_key_part_id(void *key_p, uchar *ref1, uchar *ref2);
extern "C" int cmp_key_rowid_part_id(void *ptr, uchar *ref1, uchar *ref2);

class ha_partition :public handler
{
private:
  enum partition_index_scan_type
  {
    partition_index_read= 0,
    partition_index_first= 1,
    partition_index_last= 3,
    partition_index_read_last= 4,
    partition_read_range = 5,
    partition_no_index_scan= 6,
    partition_read_multi_range = 7,
    partition_ft_read= 8
  };
  
  int  m_mode;                          
  uint m_open_test_lock;                
  uchar *m_file_buffer;                 
  char *m_name_buffer_ptr;		
  MEM_ROOT m_mem_root;
  plugin_ref *m_engine_array;           
  handler **m_file;                     
  uint m_file_tot_parts;                
  handler **m_new_file;                 
  handler **m_reorged_file;             
  handler **m_added_file;               
  LEX_CSTRING *m_connect_string;
  partition_info *m_part_info;          
  Field **m_part_field_array;           
  uchar *m_ordered_rec_buffer;          
  st_partition_ft_info *ft_first;
  st_partition_ft_info *ft_current;
  
  KEY *m_curr_key_info[3];              
  uchar *m_rec0;                        
  const uchar *m_err_rec;               
  QUEUE m_queue;                        

  
  size_t m_priority_queue_rec_len;

  
  bool m_using_extended_keys;

  
  enum enum_handler_status
  {
    handler_not_initialized= 0,
    handler_initialized,
    handler_opened,
    handler_closed
  };
  enum_handler_status m_handler_status;

  uint m_reorged_parts;                  
  uint m_tot_parts;                      
  uint m_num_locks;                       
  uint m_last_part;                      
  part_id_range m_part_spec;             
  uint m_scan_value;                     
                                         
  uint m_ref_length;                     
                                         
  key_range m_start_key;                 
  enum partition_index_scan_type m_index_scan_type;
                                                   
  uint m_top_entry;                      
                                         
  uint m_rec_length;                     

  bool m_ordered;                        
  bool m_pkey_is_clustered;              
  bool m_create_handler;                 
  bool m_is_sub_partitioned;             
  bool m_ordered_scan_ongoing;
  bool m_rnd_init_and_first;
  bool m_ft_init_and_first;

  
  ha_partition *m_is_clone_of;
  MEM_ROOT *m_clone_mem_root;

  
  bool m_myisam;                         
                                         
  
  bool m_innodb;                        
                                        
  
  bool m_extra_cache;
  uint m_extra_cache_size;
  
  bool m_extra_prepare_for_update;
  
  uint m_extra_cache_part_id;

  void init_handler_variables();
  

  bool auto_increment_lock;             
  
  bool auto_increment_safe_stmt_log_lock;
  
  MY_BITMAP m_bulk_insert_started;
  ha_rows   m_bulk_inserted_rows;
  
  enum_monotonicity_info m_part_func_monotonicity_info;
  part_id_range m_direct_update_part_spec;
  bool                m_pre_calling;
  bool                m_pre_call_use_parallel;
  
  bool                bulk_access_executing;

  
  MY_BITMAP m_locked_partitions;
  
  Partition_share *part_share;
  
  List<Parts_share_refs> m_new_partitions_share_refs;
  
  uint32 *m_part_ids_sorted_by_num_of_records;
  
  static int compare_number_of_records(ha_partition *me,
                                       const uint32 *a,
                                       const uint32 *b);
  
  MY_BITMAP m_partitions_to_reset;
  
  MY_BITMAP m_key_not_found_partitions;
  bool m_key_not_found;
  List<String> *m_partitions_to_open;
  MY_BITMAP m_opened_partitions;
  
  
  handler *m_file_sample;
public:
  handler **get_child_handlers()
  {
    return m_file;
  }
  virtual part_id_range *get_part_spec()
  {
    return &m_part_spec;
  }
  virtual uint get_no_current_part_id()
  {
    return NO_CURRENT_PART_ID;
  }
  Partition_share *get_part_share() { return part_share; }
  handler *clone(const char *name, MEM_ROOT *mem_root);
  virtual void set_part_info(partition_info *part_info)
  {
     m_part_info= part_info;
     m_is_sub_partitioned= part_info->is_sub_partitioned();
  }

  virtual void return_record_by_parent();

  virtual bool vers_can_native(THD *thd)
  {
    if (thd->lex->part_info)
    {
      
      return thd->lex->part_info->part_type != VERSIONING_PARTITION;
    }
    else
    {
      bool can= true;
      for (uint i= 0; i < m_tot_parts && can; i++)
        can= can && m_file[i]->vers_can_native(thd);
      return can;
    }
  }

  
    ha_partition(handlerton *hton, TABLE_SHARE * table);
    ha_partition(handlerton *hton, partition_info * part_info);
    ha_partition(handlerton *hton, TABLE_SHARE *share,
                 partition_info *part_info_arg,
                 ha_partition *clone_arg,
                 MEM_ROOT *clone_mem_root_arg);
   ~ha_partition();
   void ha_partition_init();
  
   bool initialize_partition(MEM_ROOT *mem_root);

  
  virtual int delete_table(const char *from);
  virtual int rename_table(const char *from, const char *to);
  virtual int create(const char *name, TABLE *form,
		     HA_CREATE_INFO *create_info);
  virtual int create_partitioning_metadata(const char *name,
                                   const char *old_name, int action_flag);
  virtual void update_create_info(HA_CREATE_INFO *create_info);
  virtual char *update_table_comment(const char *comment);
  virtual int change_partitions(HA_CREATE_INFO *create_info,
                                const char *path,
                                ulonglong * const copied,
                                ulonglong * const deleted,
                                const uchar *pack_frm_data,
                                size_t pack_frm_len);
  virtual int drop_partitions(const char *path);
  virtual int rename_partitions(const char *path);
  bool get_no_parts(const char *name, uint *num_parts)
  {
    DBUG_ENTER("ha_partition::get_no_parts");
    *num_parts= m_tot_parts;
    DBUG_RETURN(0);
  }
  virtual void change_table_ptr(TABLE *table_arg, TABLE_SHARE *share);
  virtual bool check_if_incompatible_data(HA_CREATE_INFO *create_info,
                                          uint table_changes);
  void update_part_create_info(HA_CREATE_INFO *create_info, uint part_id)
  {
    m_file[part_id]->update_create_info(create_info);
  }
private:
  int copy_partitions(ulonglong * const copied, ulonglong * const deleted);
  void cleanup_new_partition(uint part_count);
  int prepare_new_partition(TABLE *table, HA_CREATE_INFO *create_info,
                            handler *file, const char *part_name,
                            partition_element *p_elem,
                            uint disable_non_uniq_indexes);
  
  uint del_ren_table(const char *from, const char *to);
  
  bool create_handler_file(const char *name);
  bool setup_engine_array(MEM_ROOT *mem_root);
  bool read_par_file(const char *name);
  bool get_from_handler_file(const char *name, MEM_ROOT *mem_root,
                             bool is_clone);
  bool new_handlers_from_part_info(MEM_ROOT *mem_root);
  bool create_handlers(MEM_ROOT *mem_root);
  void clear_handler_file();
  int set_up_table_before_create(TABLE *table_arg,
                                 const char *partition_name_with_path,
                                 HA_CREATE_INFO *info,
                                 partition_element *p_elem);
  partition_element *find_partition_element(uint part_id);
  bool insert_partition_name_in_hash(const char *name, uint part_id,
                                     bool is_subpart);
  bool populate_partition_name_hash();
  Partition_share *get_share();
  bool set_ha_share_ref(Handler_share **ha_share);
  void fix_data_dir(char* path);
  bool init_partition_bitmaps();
  void free_partition_bitmaps();

public:

  
  virtual int open(const char *name, int mode, uint test_if_locked);
  virtual int close(void);

  
  virtual THR_LOCK_DATA **store_lock(THD * thd, THR_LOCK_DATA ** to,
				     enum thr_lock_type lock_type);
  virtual int external_lock(THD * thd, int lock_type);
  LEX_CSTRING *engine_name() { return hton_name(partition_ht()); }
  
  virtual int start_stmt(THD * thd, thr_lock_type lock_type);
  
  virtual uint lock_count(void) const;
  
  virtual void unlock_row();
  
  virtual bool was_semi_consistent_read();
  
  virtual void try_semi_consistent_read(bool);

  
#ifdef HAVE_M_PSI_PER_PARTITION
  
  virtual void unbind_psi();
  virtual void rebind_psi();
#endif
  
  virtual int write_row(uchar * buf);
  virtual bool start_bulk_update();
  virtual int exec_bulk_update(ha_rows *dup_key_found);
  virtual int end_bulk_update();
  virtual int bulk_update_row(const uchar *old_data, const uchar *new_data,
                              ha_rows *dup_key_found);
  virtual int update_row(const uchar * old_data, const uchar * new_data);
  virtual int direct_update_rows_init(List<Item> *update_fields);
  virtual int pre_direct_update_rows_init(List<Item> *update_fields);
  virtual int direct_update_rows(ha_rows *update_rows);
  virtual int pre_direct_update_rows();
  virtual bool start_bulk_delete();
  virtual int end_bulk_delete();
  virtual int delete_row(const uchar * buf);
  virtual int direct_delete_rows_init();
  virtual int pre_direct_delete_rows_init();
  virtual int direct_delete_rows(ha_rows *delete_rows);
  virtual int pre_direct_delete_rows();
  virtual int delete_all_rows(void);
  virtual int truncate();
  virtual void start_bulk_insert(ha_rows rows, uint flags);
  virtual int end_bulk_insert();
private:
  ha_rows guess_bulk_insert_rows();
  void start_part_bulk_insert(THD *thd, uint part_id);
  long estimate_read_buffer_size(long original_size);
public:

  
  int truncate_partition(Alter_info *, bool *binlog_stmt);

  virtual bool is_fatal_error(int error, uint flags)
  {
    if (!handler::is_fatal_error(error, flags) ||
        error == HA_ERR_NO_PARTITION_FOUND ||
        error == HA_ERR_NOT_IN_LOCK_PARTITIONS)
      return FALSE;
    return TRUE;
  }


  
  
  virtual int rnd_init(bool scan);
  virtual int rnd_end();
  virtual int rnd_next(uchar * buf);
  virtual int rnd_pos(uchar * buf, uchar * pos);
  virtual int rnd_pos_by_record(uchar *record);
  virtual void position(const uchar * record);

  
  virtual int index_read_map(uchar * buf, const uchar * key,
                             key_part_map keypart_map,
                             enum ha_rkey_function find_flag);
  virtual int index_init(uint idx, bool sorted);
  virtual int index_end();

  
  virtual int index_read_idx_map(uchar *buf, uint index, const uchar *key,
                                 key_part_map keypart_map,
                                 enum ha_rkey_function find_flag);
  
  virtual int index_next(uchar * buf);
  virtual int index_prev(uchar * buf);
  virtual int index_first(uchar * buf);
  virtual int index_last(uchar * buf);
  virtual int index_next_same(uchar * buf, const uchar * key, uint keylen);

  int index_read_last_map(uchar *buf,
                          const uchar *key,
                          key_part_map keypart_map);

  


  virtual int read_range_first(const key_range * start_key,
			       const key_range * end_key,
			       bool eq_range, bool sorted);
  virtual int read_range_next();


  HANDLER_BUFFER *m_mrr_buffer;
  uint *m_mrr_buffer_size;
  uchar *m_mrr_full_buffer;
  uint m_mrr_full_buffer_size;
  uint m_mrr_new_full_buffer_size;
  MY_BITMAP m_mrr_used_partitions;
  uint *m_stock_range_seq;
  uint m_current_range_seq;
  uint m_mrr_mode;
  uint m_mrr_n_ranges;
  range_id_t *m_range_info;
  bool m_multi_range_read_first;
  uint m_mrr_range_init_flags;
  uint m_mrr_range_length;
  PARTITION_KEY_MULTI_RANGE *m_mrr_range_first;
  PARTITION_KEY_MULTI_RANGE *m_mrr_range_current;
  uint *m_part_mrr_range_length;
  PARTITION_PART_KEY_MULTI_RANGE **m_part_mrr_range_first;
  PARTITION_PART_KEY_MULTI_RANGE **m_part_mrr_range_current;
  PARTITION_PART_KEY_MULTI_RANGE_HLD *m_partition_part_key_multi_range_hld;
  range_seq_t m_seq;
  RANGE_SEQ_IF *m_seq_if;
  RANGE_SEQ_IF m_part_seq_if;

  virtual int multi_range_key_create_key(
    RANGE_SEQ_IF *seq,
    range_seq_t seq_it
  );
  virtual ha_rows multi_range_read_info_const(uint keyno, RANGE_SEQ_IF *seq,
                                              void *seq_init_param,
                                              uint n_ranges, uint *bufsz,
                                              uint *mrr_mode,
                                              Cost_estimate *cost);
  virtual ha_rows multi_range_read_info(uint keyno, uint n_ranges, uint keys,
                                        uint key_parts, uint *bufsz,
                                        uint *mrr_mode, Cost_estimate *cost);
  virtual int multi_range_read_init(RANGE_SEQ_IF *seq, void *seq_init_param,
                                    uint n_ranges, uint mrr_mode,
                                    HANDLER_BUFFER *buf);
  virtual int multi_range_read_next(range_id_t *range_info);
  virtual int multi_range_read_explain_info(uint mrr_mode, char *str,
                                            size_t size);
  uint last_part() { return m_last_part; }

private:
  bool init_record_priority_queue();
  void destroy_record_priority_queue();
  int common_index_read(uchar * buf, bool have_start_key);
  int common_first_last(uchar * buf);
  int partition_scan_set_up(uchar * buf, bool idx_read_flag);
  bool check_parallel_search();
  int handle_pre_scan(bool reverse_order, bool use_parallel);
  int handle_unordered_next(uchar * buf, bool next_same);
  int handle_unordered_scan_next_partition(uchar * buf);
  int handle_ordered_index_scan(uchar * buf, bool reverse_order);
  int handle_ordered_index_scan_key_not_found();
  int handle_ordered_next(uchar * buf, bool next_same);
  int handle_ordered_prev(uchar * buf);
  void return_top_record(uchar * buf);
public:
  
  virtual int info(uint);
  void get_dynamic_partition_info(PARTITION_STATS *stat_info,
                                  uint part_id);
  void set_partitions_to_open(List<String> *partition_names);
  int change_partitions_to_open(List<String> *partition_names);
  int open_read_partitions(char *name_buff, size_t name_buff_size);
  virtual int extra(enum ha_extra_function operation);
  virtual int extra_opt(enum ha_extra_function operation, ulong arg);
  virtual int reset(void);
  virtual uint count_query_cache_dependant_tables(uint8 *tables_type);
  virtual my_bool
    register_query_cache_dependant_tables(THD *thd,
                                          Query_cache *cache,
                                          Query_cache_block_table **block,
                                          uint *n);

private:
  typedef int handler_callback(handler *, void *);

  my_bool reg_query_cache_dependant_table(THD *thd,
                                          char *engine_key,
                                          uint engine_key_len,
                                          char *query_key, uint query_key_len,
                                          uint8 type,
                                          Query_cache *cache,
                                          Query_cache_block_table
                                          **block_table,
                                          handler *file, uint *n);
  static const uint NO_CURRENT_PART_ID= NOT_A_PARTITION_ID;
  int loop_partitions(handler_callback callback, void *param);
  int loop_extra_alter(enum ha_extra_function operations);
  void late_extra_cache(uint partition_id);
  void late_extra_no_cache(uint partition_id);
  void prepare_extra_cache(uint cachesize);
  handler *get_open_file_sample() const { return m_file_sample; }
public:

  

  

private:
  
  ha_rows min_rows_for_estimate();
  uint get_biggest_used_partition(uint *part_index);
public:

  
  virtual const key_map *keys_to_use_for_scanning();

  
  virtual double scan_time();

  
  virtual double read_time(uint index, uint ranges, ha_rows rows);
  
  virtual ha_rows records_in_range(uint inx, key_range * min_key,
				   key_range * max_key);

  
  virtual ha_rows estimate_rows_upper_bound();

  
  virtual uint8 table_cache_type();
  virtual ha_rows records();

  
  static uint32 calculate_key_hash_value(Field **field_array);

  
  
  virtual const char *index_type(uint inx);

  
  virtual const char *table_type() const;

  
  virtual enum row_type get_row_type() const;

  
  virtual void print_error(int error, myf errflag);
  virtual bool get_error_message(int error, String * buf);
  
  virtual Table_flags table_flags() const;

  
  virtual ulong index_flags(uint inx, uint part, bool all_parts) const
  {
    
    return m_file[0]->index_flags(inx, part, all_parts);
  }

  
  virtual alter_table_operations alter_table_flags(alter_table_operations flags);
  
  uint min_of_the_max_uint(uint (handler::*operator_func)(void) const) const;
  virtual uint max_supported_record_length() const;
  virtual uint max_supported_keys() const;
  virtual uint max_supported_key_parts() const;
  virtual uint max_supported_key_length() const;
  virtual uint max_supported_key_part_length() const;
  virtual uint min_record_length(uint options) const;

  
  virtual bool primary_key_is_clustered()
  { return m_pkey_is_clustered; }

  
  virtual int cmp_ref(const uchar * ref1, const uchar * ref2);
  
  virtual bool need_info_for_auto_inc();
  virtual bool can_use_for_auto_inc_init();
  virtual void get_auto_increment(ulonglong offset, ulonglong increment,
                                  ulonglong nb_desired_values,
                                  ulonglong *first_value,
                                  ulonglong *nb_reserved_values);
  virtual void release_auto_increment();
private:
  virtual int reset_auto_increment(ulonglong value);
  void update_next_auto_inc_val();
  virtual void lock_auto_increment()
  {
    
    if (auto_increment_safe_stmt_log_lock)
      return;
    if (table_share->tmp_table == NO_TMP_TABLE)
    {
      part_share->lock_auto_inc();
      DBUG_ASSERT(!auto_increment_lock);
      auto_increment_lock= TRUE;
    }
  }
  virtual void unlock_auto_increment()
  {
    
    if (auto_increment_lock && !auto_increment_safe_stmt_log_lock)
    {
      auto_increment_lock= FALSE;
      part_share->unlock_auto_inc();
    }
  }
  virtual void set_auto_increment_if_higher(Field *field)
  {
    ulonglong nr= (((Field_num*) field)->unsigned_flag ||
                   field->val_int() > 0) ? field->val_int() : 0;
    lock_auto_increment();
    DBUG_ASSERT(part_share->auto_inc_initialized ||
                !can_use_for_auto_inc_init());
    
    if (nr >= part_share->next_auto_inc_val)
      part_share->next_auto_inc_val= nr + 1;
    unlock_auto_increment();
  }

public:

  
  virtual void init_table_handle_for_HANDLER();

  

  
    virtual bool can_switch_engines();
  
    void ft_close_search(FT_INFO *handler);
    virtual int ft_init();
    virtual int pre_ft_init();
    virtual void ft_end();
    virtual int pre_ft_end();
    virtual FT_INFO *ft_init_ext(uint flags, uint inx, String *key);
    virtual int ft_read(uchar *buf);
    virtual int pre_ft_read(bool use_parallel);

  

  
    virtual enum_alter_inplace_result
      check_if_supported_inplace_alter(TABLE *altered_table,
                                       Alter_inplace_info *ha_alter_info);
    virtual bool prepare_inplace_alter_table(TABLE *altered_table,
                                             Alter_inplace_info *ha_alter_info);
    virtual bool inplace_alter_table(TABLE *altered_table,
                                     Alter_inplace_info *ha_alter_info);
    virtual bool commit_inplace_alter_table(TABLE *altered_table,
                                            Alter_inplace_info *ha_alter_info,
                                            bool commit);
    virtual void notify_table_changed();

  

  
    virtual int optimize(THD* thd, HA_CHECK_OPT *check_opt);
    virtual int analyze(THD* thd, HA_CHECK_OPT *check_opt);
    virtual int check(THD* thd, HA_CHECK_OPT *check_opt);
    virtual int repair(THD* thd, HA_CHECK_OPT *check_opt);
    virtual bool check_and_repair(THD *thd);
    virtual bool auto_repair(int error) const;
    virtual bool is_crashed() const;
    virtual int check_for_upgrade(HA_CHECK_OPT *check_opt);

  
    virtual const COND *cond_push(const COND *cond);
    virtual void cond_pop();
    virtual void clear_top_table_fields();
    virtual int info_push(uint info_type, void *info);

    private:
    int handle_opt_partitions(THD *thd, HA_CHECK_OPT *check_opt, uint flags);
    int handle_opt_part(THD *thd, HA_CHECK_OPT *check_opt, uint part_id,
                        uint flag);
    
    int check_misplaced_rows(uint read_part_id, bool repair);
    void append_row_to_str(String &str);
    public:

  
    virtual uint checksum() const;
  
    virtual int assign_to_keycache(THD* thd, HA_CHECK_OPT *check_opt);
    virtual int preload_keys(THD* thd, HA_CHECK_OPT* check_opt);
    virtual TABLE_LIST *get_next_global_for_child();

  
    virtual int disable_indexes(uint mode);
    virtual int enable_indexes(uint mode);
    virtual int indexes_are_disabled(void);

  

  
  virtual handlerton *partition_ht() const
  {
    handlerton *h= m_file[0]->ht;
    for (uint i=1; i < m_tot_parts; i++)
      DBUG_ASSERT(h == m_file[i]->ht);
    return h;
  }

  ha_rows part_records(void *_part_elem)
  {
    partition_element *part_elem= reinterpret_cast<partition_element *>(_part_elem);
    DBUG_ASSERT(m_part_info);
    uint32 sub_factor= m_part_info->num_subparts ? m_part_info->num_subparts : 1;
    uint32 part_id= part_elem->id * sub_factor;
    uint32 part_id_end= part_id + sub_factor;
    DBUG_ASSERT(part_id_end <= m_tot_parts);
    ha_rows part_recs= 0;
    for (; part_id < part_id_end; ++part_id)
    {
      handler *file= m_file[part_id];
      DBUG_ASSERT(bitmap_is_set(&(m_part_info->read_partitions), part_id));
      file->info(HA_STATUS_VARIABLE | HA_STATUS_NO_LOCK | HA_STATUS_OPEN);
      part_recs+= file->stats.records;
    }
    return part_recs;
  }

  friend int cmp_key_rowid_part_id(void *ptr, uchar *ref1, uchar *ref2);
  friend int cmp_key_part_id(void *key_p, uchar *ref1, uchar *ref2);
};
#endif 
