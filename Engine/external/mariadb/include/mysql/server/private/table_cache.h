#ifndef TABLE_CACHE_H_INCLUDED
#define TABLE_CACHE_H_INCLUDED



struct Share_free_tables
{
  typedef I_P_List <TABLE, TABLE_share> List;
  List list;
  
  char pad[CPU_LEVEL1_DCACHE_LINESIZE];
};

typedef int64 tdc_version_t;
#define TDC_VERSION_MAX INT_MAX64

struct TDC_element
{
  uchar m_key[NAME_LEN + 1 + NAME_LEN + 1];
  uint m_key_length;
  tdc_version_t version;
  bool flushed;
  TABLE_SHARE *share;

  
  mysql_mutex_t LOCK_table_share;
  mysql_cond_t COND_release;
  TDC_element *next, **prev;            
  uint ref_count;                       
  uint all_tables_refs;                 
  
  Wait_for_flush_list m_flush_tickets;
  
  All_share_tables_list all_tables;
  
  char pad[CPU_LEVEL1_DCACHE_LINESIZE];
  Share_free_tables free_tables[1];
};


enum enum_tdc_remove_table_type
{
  TDC_RT_REMOVE_ALL,
  TDC_RT_REMOVE_NOT_OWN,
  TDC_RT_REMOVE_UNUSED,
  TDC_RT_REMOVE_NOT_OWN_KEEP_SHARE
};

extern ulong tdc_size;
extern ulong tc_size;
extern uint32 tc_instances;
extern uint32 tc_active_instances;

extern bool tdc_init(void);
extern void tdc_start_shutdown(void);
extern void tdc_deinit(void);
extern ulong tdc_records(void);
extern void tdc_purge(bool all);
extern TDC_element *tdc_lock_share(THD *thd, const char *db,
                                   const char *table_name);
extern void tdc_unlock_share(TDC_element *element);
extern TABLE_SHARE *tdc_acquire_share(THD *thd, TABLE_LIST *tl, uint flags,
                                      TABLE **out_table= 0);
extern void tdc_release_share(TABLE_SHARE *share);
extern bool tdc_remove_table(THD *thd, enum_tdc_remove_table_type remove_type,
                             const char *db, const char *table_name,
                             bool kill_delayed_threads);


extern int tdc_wait_for_old_version(THD *thd, const char *db,
                                    const char *table_name,
                                    ulong wait_timeout, uint deadlock_weight,
                                    tdc_version_t refresh_version= TDC_VERSION_MAX);
extern tdc_version_t tdc_refresh_version(void);
extern tdc_version_t tdc_increment_refresh_version(void);
extern int tdc_iterate(THD *thd, my_hash_walk_action action, void *argument,
                       bool no_dups= false);

extern uint tc_records(void);
extern void tc_purge(bool mark_flushed= false);
extern void tc_add_table(THD *thd, TABLE *table);
extern void tc_release_table(TABLE *table);



inline uint tdc_create_key(char *key, const char *db, const char *table_name)
{
  
  return (uint) (strmake(strmake(key, db, NAME_LEN) + 1, table_name,
                         NAME_LEN) - key + 1);
}
#endif 
