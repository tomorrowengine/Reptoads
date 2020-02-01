




#ifndef _heap_h
#define _heap_h
#ifdef	__cplusplus
extern "C" {
#endif

#ifndef _my_base_h
#include <my_base.h>
#endif

#include <my_pthread.h>
#include <thr_lock.h>

#include "my_compare.h"
#include "my_tree.h"

	

#define HP_MAX_LEVELS	4		
#define HP_PTRS_IN_NOD	128

	

typedef struct st_heapinfo		
{
  ulong records;			
  ulong deleted;			
  ulong max_records;
  ulonglong data_length;
  ulonglong index_length;
  uint reclength;			
  int errkey;
  ulonglong auto_increment;
  time_t create_time;
} HEAPINFO;


	

typedef struct st_heap_ptrs
{
  uchar *blocks[HP_PTRS_IN_NOD];		
} HP_PTRS;

struct st_level_info
{
  
  uint free_ptrs_in_block;
  
  
  ulong records_under_level;

  
  HP_PTRS *last_blocks;			
};




typedef struct st_heap_block
{
  HP_PTRS *root;                         
  struct st_level_info level_info[HP_MAX_LEVELS+1];
  uint levels;                          
  uint recbuffer;			
  ulong records_in_block;		
  ulong last_allocated; 
} HP_BLOCK;

struct st_heap_info;			

typedef struct st_hp_keydef		
{
  uint flag;				
  uint keysegs;				
  uint length;				
  uint8 algorithm;			
  HA_KEYSEG *seg;
  HP_BLOCK block;			
  
  ha_rows hash_buckets; 
  TREE rb_tree;
  int (*write_key)(struct st_heap_info *info, struct st_hp_keydef *keyinfo,
		   const uchar *record, uchar *recpos);
  int (*delete_key)(struct st_heap_info *info, struct st_hp_keydef *keyinfo,
		   const uchar *record, uchar *recpos, int flag);
  uint (*get_key_length)(struct st_hp_keydef *keydef, const uchar *key);
} HP_KEYDEF;

typedef struct st_heap_share
{
  HP_BLOCK block;
  HP_KEYDEF  *keydef;
  ulonglong data_length,index_length,max_table_size;
  ulonglong auto_increment;
  ulong min_records,max_records;	
  ulong records;			
  ulong blength;			
  ulong deleted;			
  uint key_stat_version;                
  uint key_version;                     
  uint file_version;                    
  uint reclength;			
  uint visible;                         
  uint changed;
  uint keys,max_key_length;
  uint currently_disabled_keys;    
  uint open_count;
  uchar *del_link;			
  char * name;			
  time_t create_time;
  THR_LOCK lock;
  mysql_mutex_t intern_lock;            
  my_bool delete_on_close;
  my_bool internal;                     
  LIST open_list;
  uint auto_key;
  uint auto_key_type;			
} HP_SHARE;

struct st_hp_hash_info;

typedef struct st_heap_info
{
  HP_SHARE *s;
  uchar *current_ptr;
  struct st_hp_hash_info *current_hash_ptr;
  ulong current_record,next_block;
  int lastinx,errkey;
  int  mode;				
  uint opt_flag,update;
  uchar *lastkey;			
  uchar *recbuf;                         
  enum ha_rkey_function last_find_flag;
  TREE_ELEMENT *parents[MAX_TREE_HEIGHT+1];
  TREE_ELEMENT **last_pos;
  uint key_version;                     
  uint file_version;                    
  uint lastkey_len;
  my_bool implicit_emptied;
  THR_LOCK_DATA lock;
  LIST open_list;
} HP_INFO;


typedef struct st_heap_create_info
{
  HP_KEYDEF *keydef;
  uint auto_key;                        
  uint auto_key_type;
  uint keys;
  uint reclength;
  ulong max_records;
  ulong min_records;
  ulonglong max_table_size;
  ulonglong auto_increment;
  my_bool with_auto_increment;
  my_bool internal_table;
  
  my_bool pin_share;
} HP_CREATE_INFO;

	

extern HP_INFO *heap_open(const char *name, int mode);
extern HP_INFO *heap_open_from_share(HP_SHARE *share, int mode);
extern HP_INFO *heap_open_from_share_and_register(HP_SHARE *share, int mode);
extern void heap_release_share(HP_SHARE *share, my_bool internal_table);
extern int heap_close(HP_INFO *info);
extern int heap_write(HP_INFO *info,const uchar *buff);
extern int heap_update(HP_INFO *info,const uchar *old,const uchar *newdata);
extern int heap_rrnd(HP_INFO *info,uchar *buf,uchar *pos);
extern int heap_scan_init(HP_INFO *info);
extern int heap_scan(HP_INFO *info, uchar *record);
extern int heap_delete(HP_INFO *info,const uchar *buff);
extern int heap_info(HP_INFO *info,HEAPINFO *x,int flag);
extern int heap_create(const char *name,
                       HP_CREATE_INFO *create_info, HP_SHARE **share,
                       my_bool *created_new_share);
extern int heap_delete_table(const char *name);
extern void heap_drop_table(HP_INFO *info);
extern int heap_extra(HP_INFO *info,enum ha_extra_function function);
extern int heap_reset(HP_INFO *info);
extern int heap_rename(const char *old_name,const char *new_name);
extern int heap_panic(enum ha_panic_function flag);
extern int heap_rsame(HP_INFO *info,uchar *record,int inx);
extern int heap_rnext(HP_INFO *info,uchar *record);
extern int heap_rprev(HP_INFO *info,uchar *record);
extern int heap_rfirst(HP_INFO *info,uchar *record,int inx);
extern int heap_rlast(HP_INFO *info,uchar *record,int inx);
extern void heap_clear(HP_INFO *info);
extern void heap_clear_keys(HP_INFO *info);
extern int heap_disable_indexes(HP_INFO *info);
extern int heap_enable_indexes(HP_INFO *info);
extern int heap_indexes_are_disabled(HP_INFO *info);
extern void heap_update_auto_increment(HP_INFO *info, const uchar *record);
ha_rows hp_rb_records_in_range(HP_INFO *info, int inx, key_range *min_key,
                               key_range *max_key);
int hp_panic(enum ha_panic_function flag);
int heap_rkey(HP_INFO *info, uchar *record, int inx, const uchar *key,
              key_part_map keypart_map, enum ha_rkey_function find_flag);
extern uchar * heap_find(HP_INFO *info,int inx,const uchar *key);
extern int heap_check_heap(HP_INFO *info, my_bool print_status);
extern uchar *heap_position(HP_INFO *info);



#if defined(WANT_OLD_HEAP_VERSION) || defined(OLD_HEAP_VERSION)
extern int heap_rrnd_old(HP_INFO *info,uchar *buf,ulong pos);
extern ulong heap_position_old(HP_INFO *info);
#endif
#ifdef OLD_HEAP_VERSION
typedef ulong HEAP_PTR;
#define heap_position(A) heap_position_old(A)
#define heap_rrnd(A,B,C) heap_rrnd_old(A,B,C)
#else
typedef uchar *HEAP_PTR;
#endif

#ifdef	__cplusplus
}
#endif
#endif
