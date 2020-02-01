



#ifndef _keycache_h
#define _keycache_h

#include "my_sys.h"                             

C_MODE_START



#define DEFAULT_KEY_CACHE_PARTITIONS    0



#define MAX_KEY_CACHE_PARTITIONS    64



typedef struct st_key_cache_statistics
{
  ulonglong mem_size;       
  ulonglong block_size;     
  ulonglong blocks_used;     
  ulonglong blocks_unused;  
  ulonglong blocks_changed; 
  ulonglong blocks_warm;    
  ulonglong read_requests;  
  ulonglong reads;        
  ulonglong write_requests; 
  ulonglong writes;       
} KEY_CACHE_STATISTICS;

#define NUM_LONG_KEY_CACHE_STAT_VARIABLES 3


typedef enum key_cache_type
{
  SIMPLE_KEY_CACHE,         
  PARTITIONED_KEY_CACHE
} KEY_CACHE_TYPE;


typedef
  int    (*INIT_KEY_CACHE)  
           (void *, uint key_cache_block_size,
            size_t use_mem, uint division_limit, uint age_threshold,
            uint changed_blocks_hash_size);
typedef
  int    (*RESIZE_KEY_CACHE)
           (void *, uint key_cache_block_size,
            size_t use_mem, uint division_limit, uint age_threshold,
            uint changed_blocks_hash_size);
typedef
  void   (*CHANGE_KEY_CACHE_PARAM)
           (void *keycache_cb,
            uint division_limit, uint age_threshold);
typedef
  uchar* (*KEY_CACHE_READ)
           (void *keycache_cb,
            File file, my_off_t filepos, int level,
            uchar *buff, uint length,
            uint block_length, int return_buffer);
typedef
  int    (*KEY_CACHE_INSERT)
           (void *keycache_cb,
            File file, my_off_t filepos, int level,
            uchar *buff, uint length);
typedef
  int    (*KEY_CACHE_WRITE)
           (void *keycache_cb,
            File file, void *file_extra,
            my_off_t filepos, int level,
            uchar *buff, uint length, 
            uint block_length, int force_write);
typedef
  int    (*FLUSH_KEY_BLOCKS)
           (void *keycache_cb,
            int file, void *file_extra,
            enum flush_type type); 
typedef
  int    (*RESET_KEY_CACHE_COUNTERS)
           (const char *name, void *keycache_cb); 
typedef
  void   (*END_KEY_CACHE)
           (void *keycache_cb, my_bool cleanup);
typedef
  void   (*GET_KEY_CACHE_STATISTICS)
           (void *keycache_cb, uint partition_no, 
            KEY_CACHE_STATISTICS *key_cache_stats); 

   

typedef struct st_key_cache_funcs 
{
  INIT_KEY_CACHE init;
  RESIZE_KEY_CACHE         resize;
  CHANGE_KEY_CACHE_PARAM   change_param;     
  KEY_CACHE_READ           read;
  KEY_CACHE_INSERT         insert;
  KEY_CACHE_WRITE          write;
  FLUSH_KEY_BLOCKS         flush;
  RESET_KEY_CACHE_COUNTERS reset_counters; 
  END_KEY_CACHE            end;
  GET_KEY_CACHE_STATISTICS get_stats; 
} KEY_CACHE_FUNCS;


typedef struct st_key_cache
{
  KEY_CACHE_TYPE key_cache_type; 
  void *keycache_cb;             
  KEY_CACHE_FUNCS *interface_funcs; 
  ulonglong param_buff_size;     
  ulonglong param_block_size;    
  ulonglong param_division_limit;
  ulonglong param_age_threshold; 
  ulonglong param_partitions;    
  ulonglong changed_blocks_hash_size; 
  my_bool key_cache_inited;      
  my_bool can_be_used;           
  my_bool in_init;               
  uint partitions;               
  size_t key_cache_mem_size;     
  pthread_mutex_t op_lock;       
} KEY_CACHE;



extern KEY_CACHE dflt_key_cache_var, *dflt_key_cache;

extern int init_key_cache(KEY_CACHE *keycache, uint key_cache_block_size,
			  size_t use_mem, uint division_limit,
			  uint age_threshold, uint changed_blocks_hash_size,
                          uint partitions);
extern int resize_key_cache(KEY_CACHE *keycache, uint key_cache_block_size,
			    size_t use_mem, uint division_limit,
			    uint age_threshold, uint changed_blocks_hash_size);
extern void change_key_cache_param(KEY_CACHE *keycache, uint division_limit,
				   uint age_threshold);
extern uchar *key_cache_read(KEY_CACHE *keycache,
                            File file, my_off_t filepos, int level,
                            uchar *buff, uint length,
			    uint block_length,int return_buffer);
extern int key_cache_insert(KEY_CACHE *keycache,
                            File file, my_off_t filepos, int level,
                            uchar *buff, uint length);
extern int key_cache_write(KEY_CACHE *keycache,
                           File file, void *file_extra,
                           my_off_t filepos, int level,
                           uchar *buff, uint length,
			   uint block_length, int force_write);
extern int flush_key_blocks(KEY_CACHE *keycache,
                            int file, void *file_extra,
                            enum flush_type type);
extern void end_key_cache(KEY_CACHE *keycache, my_bool cleanup);
extern void get_key_cache_statistics(KEY_CACHE *keycache,
                                     uint partition_no, 
                                     KEY_CACHE_STATISTICS *key_cache_stats);


extern my_bool multi_keycache_init(void);
extern void multi_keycache_free(void);
extern KEY_CACHE *multi_key_cache_search(uchar *key, uint length,
                                         KEY_CACHE *def);
extern my_bool multi_key_cache_set(const uchar *key, uint length,
				   KEY_CACHE *key_cache);
extern void multi_key_cache_change(KEY_CACHE *old_data,
				   KEY_CACHE *new_data);
extern int reset_key_cache_counters(const char *name,
                                    KEY_CACHE *key_cache, void *);
extern int repartition_key_cache(KEY_CACHE *keycache,
                                 uint key_cache_block_size,
			         size_t use_mem, 
                                 uint division_limit,
			         uint age_threshold,
                                 uint changed_blocks_hash_size,
                                 uint partitions);
C_MODE_END
#endif 
