



#ifndef _myisamchk_h
#define _myisamchk_h



#define TT_USEFRM               1U
#define TT_FOR_UPGRADE          2U
#define TT_FROM_MYSQL           4U


#define O_NEW_DATA	2U
#define O_DATA_LOST	4U

typedef struct st_sort_key_blocks		
{
  uchar *buff, *end_pos;
  uchar lastkey[HA_MAX_POSSIBLE_KEY_BUFF];
  uint last_length;
  int inited;
} SORT_KEY_BLOCKS;




typedef enum
{
  
  MI_STATS_METHOD_NULLS_NOT_EQUAL,
  
  MI_STATS_METHOD_NULLS_EQUAL,
  
  MI_STATS_METHOD_IGNORE_NULLS
} enum_handler_stats_method;

struct st_myisam_info;

typedef struct st_handler_check_param
{
  char *isam_file_name;
  MY_TMPDIR *tmpdir;
  void *thd;
  const char *db_name, *table_name, *op_name;
  ulonglong auto_increment_value;
  ulonglong max_data_file_length;
  ulonglong keys_in_use;
  ulonglong max_record_length;
  
  ulonglong unique_count[HA_MAX_KEY_SEG + 1];
  ulonglong notnull_count[HA_MAX_KEY_SEG + 1];

  my_off_t search_after_block;
  my_off_t new_file_pos, key_file_blocks;
  my_off_t keydata, totaldata, key_blocks, start_check_pos;
  my_off_t used, empty, splits, del_length, link_used, lost;
  ha_rows total_records, total_deleted, records,del_blocks;
  ha_rows full_page_count, tail_count;
  ha_checksum record_checksum, glob_crc;
  ha_checksum key_crc[HA_MAX_POSSIBLE_KEY];
  ha_checksum tmp_key_crc[HA_MAX_POSSIBLE_KEY];
  ha_checksum tmp_record_checksum;
  ulonglong   org_key_map;
  ulonglong   testflag;

  
  ulonglong max_trid, max_found_trid;
  ulonglong not_visible_rows_found;
  ulonglong sort_buffer_length;
  ulonglong use_buffers;                        
  size_t read_buffer_length, write_buffer_length, sort_key_blocks;
  time_t backup_time;                           
  ulong rec_per_key_part[HA_MAX_KEY_SEG * HA_MAX_POSSIBLE_KEY];
  double new_rec_per_key_part[HA_MAX_KEY_SEG * HA_MAX_POSSIBLE_KEY];
  uint out_flag, error_printed, verbose;
  uint opt_sort_key, total_files, max_level;
  uint key_cache_block_size, pagecache_block_size;
  int tmpfile_createflag, err_count;
  myf myf_rw;
  uint16 language;
  my_bool warning_printed, note_printed, wrong_trd_printed;
  my_bool using_global_keycache, opt_lock_memory, opt_follow_links;
  my_bool retry_repair, force_sort, calc_checksum, static_row_size;
  char temp_filename[FN_REFLEN];
  IO_CACHE read_cache;
  enum_handler_stats_method stats_method;
  
  uint stage, max_stage;
  uint progress_counter;             
  ulonglong progress, max_progress;

  int (*fix_record)(struct st_myisam_info *info, uchar *record, int keynum);

  mysql_mutex_t print_msg_mutex;
  my_bool need_print_msg_lock;
  myf malloc_flags;
} HA_CHECK;


typedef struct st_sort_ftbuf
{
  uchar *buf, *end;
  int count;
  uchar lastkey[HA_MAX_KEY_BUFF];
} SORT_FT_BUF;


typedef struct st_buffpek {
  my_off_t file_pos;                    
  uchar *base, *key;                    
  ha_rows count;                        
  ha_rows mem_count;                    
  ha_rows max_keys;                     
} BUFFPEK;

#endif 
