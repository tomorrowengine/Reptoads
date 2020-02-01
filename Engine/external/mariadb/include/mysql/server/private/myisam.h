



#ifndef _myisam_h
#define _myisam_h
#ifdef	__cplusplus
extern "C" {
#endif

#include <my_base.h>
#include <m_ctype.h>
#include "keycache.h"
#include "my_compare.h"
#include <myisamchk.h>
#include <mysql/plugin.h>
#include <my_check_opt.h>


#if MAX_INDEXES > HA_MAX_POSSIBLE_KEY
#define MI_MAX_KEY                  HA_MAX_POSSIBLE_KEY 
#else
#define MI_MAX_KEY                  MAX_INDEXES         
#endif

#define MI_MAX_POSSIBLE_KEY_BUFF    HA_MAX_POSSIBLE_KEY_BUFF

#define MI_MAX_KEY_LENGTH           1000            
#define MI_MAX_KEY_SEG              16              

#define MI_NAME_IEXT	".MYI"
#define MI_NAME_DEXT	".MYD"


#define MI_KEY_BLOCK_LENGTH	1024	
#define MI_MIN_KEY_BLOCK_LENGTH	1024	
#define MI_MAX_KEY_BLOCK_LENGTH	16384


#define MI_KEYMAP_BITS      (8 * SIZEOF_LONG_LONG)
#define MI_KEYMAP_HIGH_MASK (1ULL << (MI_KEYMAP_BITS - 1))
#define mi_get_mask_all_keys_active(_keys_) \
                            (((_keys_) < MI_KEYMAP_BITS) ? \
                             ((1ULL << (_keys_)) - 1ULL) : \
                             (~ 0ULL))

#if MI_MAX_KEY > MI_KEYMAP_BITS

#define mi_is_key_active(_keymap_,_keyno_) \
                            (((_keyno_) < MI_KEYMAP_BITS) ? \
                             MY_TEST((_keymap_) & (1ULL << (_keyno_))) : \
                             MY_TEST((_keymap_) & MI_KEYMAP_HIGH_MASK))
#define mi_set_key_active(_keymap_,_keyno_) \
                            (_keymap_)|= (((_keyno_) < MI_KEYMAP_BITS) ? \
                                          (1ULL << (_keyno_)) : \
                                          MI_KEYMAP_HIGH_MASK)
#define mi_clear_key_active(_keymap_,_keyno_) \
                            (_keymap_)&= (((_keyno_) < MI_KEYMAP_BITS) ? \
                                          (~ (1ULL << (_keyno_))) : \
                                          (~ (0ULL))  )

#else

#define mi_is_key_active(_keymap_,_keyno_) \
                            MY_TEST((_keymap_) & (1ULL << (_keyno_)))
#define mi_set_key_active(_keymap_,_keyno_) \
                            (_keymap_)|= (1ULL << (_keyno_))
#define mi_clear_key_active(_keymap_,_keyno_) \
                            (_keymap_)&= (~ (1ULL << (_keyno_)))

#endif

#define mi_is_any_key_active(_keymap_) \
                            MY_TEST((_keymap_))
#define mi_is_all_keys_active(_keymap_,_keys_) \
                            ((_keymap_) == mi_get_mask_all_keys_active(_keys_))
#define mi_set_all_keys_active(_keymap_,_keys_) \
                            (_keymap_)= mi_get_mask_all_keys_active(_keys_)
#define mi_clear_all_keys_active(_keymap_) \
                            (_keymap_)= 0
#define mi_intersect_keys_active(_to_,_from_) \
                            (_to_)&= (_from_)
#define mi_is_any_intersect_keys_active(_keymap1_,_keys_,_keymap2_) \
                            ((_keymap1_) & (_keymap2_) & \
                             mi_get_mask_all_keys_active(_keys_))
#define mi_copy_keys_active(_to_,_maxkeys_,_from_) \
                            (_to_)= (mi_get_mask_all_keys_active(_maxkeys_) & \
                                     (_from_))

	

typedef struct st_mi_isaminfo		
{
  ha_rows records;			
  ha_rows deleted;			
  my_off_t recpos;			
  my_off_t newrecpos;			
  my_off_t dupp_key_pos;		
  my_off_t data_file_length,		
           max_data_file_length,
           index_file_length,
           max_index_file_length,
           delete_length;
  ulong reclength;			
  ulong mean_reclength;			
  ulonglong auto_increment;
  ulonglong key_map;			
  char  *data_file_name, *index_file_name;
  uint  keys;				
  uint	options;			
  int	errkey,				
	sortkey;			
  File	filenr;				
  time_t create_time;			
  time_t check_time;
  time_t update_time;
  uint  reflength;
  ulong record_offset;
  ulong *rec_per_key;			
} MI_ISAMINFO;


typedef struct st_mi_create_info
{
  const char *index_file_name, *data_file_name;	
  ha_rows max_rows;
  ha_rows reloc_rows;
  ulonglong auto_increment;
  ulonglong data_file_length;
  ulonglong key_file_length;
  uint old_options;
  uint16 language;
  my_bool with_auto_increment;
} MI_CREATE_INFO;

struct st_myisam_info;			
struct st_mi_isam_share;
typedef struct st_myisam_info MI_INFO;
struct st_mi_s_param;

typedef struct st_mi_keydef		
{
  struct st_mi_isam_share *share;       
  uint16 keysegs;			
  uint16 flag;				

  uint8  key_alg;			
  uint16 block_length;			
  uint16 underflow_block_length;	
  uint16 keylength;			
  uint16 minlength;			
  uint16 maxlength;			
  uint16 block_size_index;		
  uint32 version;			
  uint32 ftkey_nr;                      

  HA_KEYSEG *seg,*end;
  struct st_mysql_ftparser *parser;     
  int (*bin_search)(struct st_myisam_info *info,struct st_mi_keydef *keyinfo,
		    uchar *page,uchar *key,
		    uint key_len,uint comp_flag,uchar * *ret_pos,
		    uchar *buff, my_bool *was_last_key);
  uint (*get_key)(struct st_mi_keydef *keyinfo,uint nod_flag,uchar * *page,
		  uchar *key);
  int (*pack_key)(struct st_mi_keydef *keyinfo,uint nod_flag,uchar *next_key,
		  uchar *org_key, uchar *prev_key, uchar *key,
		  struct st_mi_s_param *s_temp);
  void (*store_key)(struct st_mi_keydef *keyinfo, uchar *key_pos,
		    struct st_mi_s_param *s_temp);
  int (*ck_insert)(struct st_myisam_info *inf, uint k_nr, uchar *k, uint klen);
  int (*ck_delete)(struct st_myisam_info *inf, uint k_nr, uchar *k, uint klen);
} MI_KEYDEF;


#define MI_UNIQUE_HASH_LENGTH	4

typedef struct st_unique_def		
{
  uint16 keysegs;			
  uchar key;				
  uint8 null_are_equal;
  HA_KEYSEG *seg,*end;
} MI_UNIQUEDEF;

typedef struct st_mi_decode_tree	
{
  uint16 *table;
  uint	 quick_table_bits;
  uchar	 *intervalls;
} MI_DECODE_TREE;


struct st_mi_bit_buff;



typedef struct st_columndef		
{
  enum en_fieldtype type;
  uint16 length;			
  uint32 offset;			
  uint8  null_bit;			
  uint16 null_pos;			

#ifndef NOT_PACKED_DATABASES
  void (*unpack)(struct st_columndef *rec,struct st_mi_bit_buff *buff,
		 uchar *start,uchar *end);
  enum en_fieldtype base_type;
  uint space_length_bits,pack_type;
  MI_DECODE_TREE *huff_tree;
#endif
} MI_COLUMNDEF;

extern char * myisam_log_filename;		
extern ulong myisam_block_size;
extern ulong myisam_concurrent_insert;
extern my_bool myisam_flush,myisam_delay_key_write,myisam_single_user;
extern my_off_t myisam_max_temp_length;
extern ulong myisam_data_pointer_size;



extern int (*myisam_test_invalid_symlink)(const char *filename);
extern ulonglong myisam_mmap_size, myisam_mmap_used;
extern mysql_mutex_t THR_LOCK_myisam_mmap;

	

extern int mi_close(struct st_myisam_info *file);
extern int mi_delete(struct st_myisam_info *file,const uchar *buff);
extern struct st_myisam_info *mi_open(const char *name,int mode,
				      uint wait_if_locked);
extern int mi_panic(enum ha_panic_function function);
extern int mi_rfirst(struct st_myisam_info *file,uchar *buf,int inx);
extern int mi_rkey(MI_INFO *info, uchar *buf, int inx, const uchar *key,
                   key_part_map keypart_map, enum ha_rkey_function search_flag);
extern int mi_rlast(struct st_myisam_info *file,uchar *buf,int inx);
extern int mi_rnext(struct st_myisam_info *file,uchar *buf,int inx);
extern int mi_rnext_same(struct st_myisam_info *info, uchar *buf);
extern int mi_rprev(struct st_myisam_info *file,uchar *buf,int inx);
extern int mi_rrnd(struct st_myisam_info *file,uchar *buf, my_off_t pos);
extern int mi_scan_init(struct st_myisam_info *file);
extern int mi_scan(struct st_myisam_info *file,uchar *buf);
extern int mi_rsame(struct st_myisam_info *file,uchar *record,int inx);
extern int mi_rsame_with_pos(struct st_myisam_info *file,uchar *record,
			     int inx, my_off_t pos);
extern int mi_update(struct st_myisam_info *file,const uchar *old,
		     const uchar *new_record);
extern int mi_write(struct st_myisam_info *file,uchar *buff);
extern my_off_t mi_position(struct st_myisam_info *file);
extern int mi_status(struct st_myisam_info *info, MI_ISAMINFO *x, uint flag);
extern int mi_lock_database(struct st_myisam_info *file,int lock_type);
extern int mi_create(const char *name,uint keys,MI_KEYDEF *keydef,
		     uint columns, MI_COLUMNDEF *columndef,
		     uint uniques, MI_UNIQUEDEF *uniquedef,
		     MI_CREATE_INFO *create_info, uint flags);
extern int mi_delete_table(const char *name);
extern int mi_rename(const char *from, const char *to);
extern int mi_extra(struct st_myisam_info *file,
		    enum ha_extra_function function,
		    void *extra_arg);
extern int mi_reset(struct st_myisam_info *file);
extern ha_rows mi_records_in_range(MI_INFO *info,int inx,
                                   key_range *min_key, key_range *max_key);
extern int mi_log(int activate_log);
extern int mi_is_changed(struct st_myisam_info *info);
extern int mi_delete_all_rows(struct st_myisam_info *info);
extern ulong _mi_calc_blob_length(uint length , const uchar *pos);
extern uint mi_get_pointer_length(ulonglong file_length, uint def);
extern int mi_make_backup_of_index(struct st_myisam_info *info,
                                   time_t backup_time, myf flags);
#define myisam_max_key_length() HA_MAX_KEY_LENGTH
#define myisam_max_key_segments() HA_MAX_KEY_SEG

#define MEMMAP_EXTRA_MARGIN     7       


#define   MYISAMCHK_REPAIR 1  
#define   MYISAMCHK_VERIFY 2  

typedef uint mi_bit_type;

typedef struct st_mi_bit_buff
{                                       
  mi_bit_type current_byte;
  uint bits;
  uchar *pos, *end, *blob_pos, *blob_end;
  uint error;
} MI_BIT_BUFF;

typedef struct st_sort_info
{
  
  mysql_mutex_t mutex;
  mysql_cond_t  cond;
  MI_INFO *info;
  HA_CHECK *param;
  uchar *buff;
  SORT_KEY_BLOCKS *key_block, *key_block_end;
  SORT_FT_BUF *ft_buf;
  my_off_t filelength, dupp, buff_length;
  ha_rows max_records;
  uint current_key, total_keys;
  volatile uint got_error;
  uint threads_running;
  myf myf_rw;
  enum data_file_type new_data_file_type;
} MI_SORT_INFO;

typedef struct st_mi_sort_param
{
  pthread_t thr;
  IO_CACHE read_cache, tempfile, tempfile_for_exceptions;
  DYNAMIC_ARRAY buffpek;
  MI_BIT_BUFF   bit_buff;               

  MI_KEYDEF *keyinfo;
  MI_SORT_INFO *sort_info;
  HA_KEYSEG *seg;
  uchar **sort_keys;
  uchar *rec_buff;
  void *wordlist, *wordptr;
  MEM_ROOT wordroot;
  uchar *record;
  MY_TMPDIR *tmpdir;

  
  ulonglong unique[HA_MAX_KEY_SEG+1];
  ulonglong notnull[HA_MAX_KEY_SEG+1];

  my_off_t pos,max_pos,filepos,start_recpos;
  uint key, key_length,real_key_length;
  uint maxbuffers, find_length;
  ulonglong sortbuff_size;
  ha_rows keys;
  my_bool fix_datafile, master;
  my_bool calc_checksum;                

  int (*key_cmp)(struct st_mi_sort_param *, const void *, const void *);
  int (*key_read)(struct st_mi_sort_param *,void *);
  int (*key_write)(struct st_mi_sort_param *, const void *);
  void (*lock_in_memory)(HA_CHECK *);
  int (*write_keys)(struct st_mi_sort_param *, uchar **,
                    ulonglong , struct st_buffpek *, IO_CACHE *);
  my_off_t (*read_to_buffer)(IO_CACHE *,struct st_buffpek *, uint);
  int (*write_key)(struct st_mi_sort_param *, IO_CACHE *,uchar *,
                   uint, ulonglong);
} MI_SORT_PARAM;


void myisamchk_init(HA_CHECK *param);
int chk_status(HA_CHECK *param, MI_INFO *info);
int chk_del(HA_CHECK *param, MI_INFO *info, ulonglong test_flag);
int chk_size(HA_CHECK *param, MI_INFO *info);
int chk_key(HA_CHECK *param, MI_INFO *info);
int chk_data_link(HA_CHECK *param, MI_INFO *info, my_bool extend);
int mi_repair(HA_CHECK *param, MI_INFO *info, char * name, int rep_quick);
int mi_sort_index(HA_CHECK *param, MI_INFO *info, char * name);
int mi_repair_by_sort(HA_CHECK *param, MI_INFO *info,
		      const char * name, int rep_quick);
int mi_repair_parallel(HA_CHECK *param, MI_INFO *info,
		      const char * name, int rep_quick);
int change_to_newfile(const char * filename, const char * old_ext,
                      const char * new_ext, time_t backup_time, myf myflags);
int lock_file(HA_CHECK *param, File file, my_off_t start, int lock_type,
	      const char *filetype, const char *filename);
void lock_memory(HA_CHECK *param);
void update_auto_increment_key(HA_CHECK *param, MI_INFO *info,
			       my_bool repair);
int update_state_info(HA_CHECK *param, MI_INFO *info,uint update);
void update_key_parts(MI_KEYDEF *keyinfo, ulong *rec_per_key_part,
                      ulonglong *unique, ulonglong *notnull,
                      ulonglong records);
int filecopy(HA_CHECK *param, File to,File from,my_off_t start,
	     my_off_t length, const char *type);
int movepoint(MI_INFO *info,uchar *record,my_off_t oldpos,
	      my_off_t newpos, uint prot_key);
int write_data_suffix(MI_SORT_INFO *sort_info, my_bool fix_datafile);
int test_if_almost_full(MI_INFO *info);
int recreate_table(HA_CHECK *param, MI_INFO **org_info, char *filename);
void mi_disable_non_unique_index(MI_INFO *info, ha_rows rows);
my_bool mi_test_if_sort_rep(MI_INFO *info, ha_rows rows, ulonglong key_map,
			    my_bool force);

int mi_init_bulk_insert(MI_INFO *info, size_t cache_size, ha_rows rows);
void mi_flush_bulk_insert(MI_INFO *info, uint inx);
int mi_end_bulk_insert(MI_INFO *info, my_bool abort);
int mi_assign_to_key_cache(MI_INFO *info, ulonglong key_map,
			   KEY_CACHE *key_cache);
void mi_change_key_cache(KEY_CACHE *old_key_cache,
			 KEY_CACHE *new_key_cache);
int mi_preload(MI_INFO *info, ulonglong key_map, my_bool ignore_leaves);

int write_data_suffix(MI_SORT_INFO *sort_info, my_bool fix_datafile);
int flush_pending_blocks(MI_SORT_PARAM *param);
int sort_ft_buf_flush(MI_SORT_PARAM *sort_param);
int thr_write_keys(MI_SORT_PARAM *sort_param);
int sort_write_record(MI_SORT_PARAM *sort_param);
int _create_index_by_sort(MI_SORT_PARAM *info,my_bool no_messages, ulonglong);

#ifdef	__cplusplus
}
#endif
#endif
