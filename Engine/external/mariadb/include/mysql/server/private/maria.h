



#ifndef _maria_h
#define _maria_h
#include <my_base.h>
#include <m_ctype.h>
#include "my_compare.h"
#include "ft_global.h"
#include <myisamchk.h>

#ifdef	__cplusplus
extern "C" {
#endif

#define MARIA_CANNOT_ROLLBACK



#if MAX_INDEXES > HA_MAX_POSSIBLE_KEY
#define MARIA_MAX_KEY    HA_MAX_POSSIBLE_KEY    
#else
#define MARIA_MAX_KEY    MAX_INDEXES            
#endif

#define MARIA_NAME_IEXT	".MAI"
#define MARIA_NAME_DEXT	".MAD"

#define MARIA_MAX_TEMP_LENGTH	(2*1024L*1024L*1024L)

#define MARIA_KEY_BLOCK_LENGTH	8192		
#define MARIA_MIN_KEY_BLOCK_LENGTH	1024	
#define MARIA_MAX_KEY_BLOCK_LENGTH	32768

#define MARIA_MIN_PAGE_CACHE_SIZE	(8192L*16L)


#define MARIA_KEYMAP_BITS      (8 * SIZEOF_LONG_LONG)
#define MARIA_KEYMAP_HIGH_MASK (1ULL << (MARIA_KEYMAP_BITS - 1))
#define maria_get_mask_all_keys_active(_keys_) \
                            (((_keys_) < MARIA_KEYMAP_BITS) ? \
                             ((1ULL << (_keys_)) - 1ULL) : \
                             (~ 0ULL))
#if MARIA_MAX_KEY > MARIA_KEYMAP_BITS
#define maria_is_key_active(_keymap_,_keyno_) \
                            (((_keyno_) < MARIA_KEYMAP_BITS) ? \
                             MY_TEST((_keymap_) & (1ULL << (_keyno_))) : \
                             MY_TEST((_keymap_) & MARIA_KEYMAP_HIGH_MASK))
#define maria_set_key_active(_keymap_,_keyno_) \
                            (_keymap_)|= (((_keyno_) < MARIA_KEYMAP_BITS) ? \
                                          (1ULL << (_keyno_)) : \
                                          MARIA_KEYMAP_HIGH_MASK)
#define maria_clear_key_active(_keymap_,_keyno_) \
                            (_keymap_)&= (((_keyno_) < MARIA_KEYMAP_BITS) ? \
                                          (~ (1ULL << (_keyno_))) : \
                                          (~ (0ULL))  )
#else
#define maria_is_key_active(_keymap_,_keyno_) \
                            MY_TEST((_keymap_) & (1ULL << (_keyno_)))
#define maria_set_key_active(_keymap_,_keyno_) \
                            (_keymap_)|= (1ULL << (_keyno_))
#define maria_clear_key_active(_keymap_,_keyno_) \
                            (_keymap_)&= (~ (1ULL << (_keyno_)))
#endif
#define maria_is_any_key_active(_keymap_) \
                            MY_TEST((_keymap_))
#define maria_is_all_keys_active(_keymap_,_keys_) \
                            ((_keymap_) == maria_get_mask_all_keys_active(_keys_))
#define maria_set_all_keys_active(_keymap_,_keys_) \
                            (_keymap_)= maria_get_mask_all_keys_active(_keys_)
#define maria_clear_all_keys_active(_keymap_) \
                            (_keymap_)= 0
#define maria_intersect_keys_active(_to_,_from_) \
                            (_to_)&= (_from_)
#define maria_is_any_intersect_keys_active(_keymap1_,_keys_,_keymap2_) \
                            ((_keymap1_) & (_keymap2_) & \
                             maria_get_mask_all_keys_active(_keys_))
#define maria_copy_keys_active(_to_,_maxkeys_,_from_) \
                            (_to_)= (maria_get_mask_all_keys_active(_maxkeys_) & \
                                     (_from_))

	

typedef ulonglong MARIA_RECORD_POS;

typedef struct st_maria_info
{
  ha_rows records;			
  ha_rows deleted;			
  MARIA_RECORD_POS recpos;		
  MARIA_RECORD_POS newrecpos;		
  MARIA_RECORD_POS dup_key_pos;		
  my_off_t data_file_length;            
  my_off_t max_data_file_length, index_file_length;
  my_off_t max_index_file_length, delete_length;
  ulonglong auto_increment;
  ulonglong key_map;                    
  time_t create_time;                   
  time_t check_time;
  time_t update_time;
  ulong record_offset;
  double *rec_per_key;                   
  ulong reclength;                      
  ulong mean_reclength;                 
  char *data_file_name, *index_file_name;
  enum data_file_type data_file_type;
  uint keys;                            
  uint options;                         
  uint reflength;
  int errkey,                           
    sortkey;                            
  File filenr;                          
} MARIA_INFO;


typedef struct st_maria_create_info
{
  const char *index_file_name, *data_file_name;	
  ha_rows max_rows;
  ha_rows reloc_rows;
  ulonglong auto_increment;
  ulonglong data_file_length;
  ulonglong key_file_length;
  
  uint null_bytes;
  uint old_options;
  enum data_file_type org_data_file_type;
  uint16 language;
  my_bool with_auto_increment, transactional;
} MARIA_CREATE_INFO;

struct st_maria_share;
struct st_maria_handler;			
typedef struct st_maria_handler MARIA_HA;
struct st_maria_s_param;
struct st_maria_keydef;
struct st_maria_page;

typedef struct st_maria_key                 
{
  uchar *data;                              
  struct st_maria_keydef *keyinfo;	    
  uint data_length;                         
  uint ref_length;                          
  uint32 flag;                               
} MARIA_KEY;


typedef struct st_maria_keydef          
{
  struct st_maria_share *share;         
  mysql_rwlock_t root_lock;                  
  uint16 keysegs;                       
  uint16 flag;                          

  uint8 key_alg;                        
  uint8 key_nr;				
  uint16 block_length;                  
  uint16 underflow_block_length;        
  uint16 keylength;                     
  uint16 minlength;                     
  uint16 maxlength;                     
  uint32 write_comp_flag;		
  uint32 version;                       
  uint32 ftkey_nr;                      

  HA_KEYSEG *seg, *end;
  struct st_mysql_ftparser *parser;     
  int (*bin_search)(const MARIA_KEY *key, const struct st_maria_page *page,
                    uint32 comp_flag, uchar **ret_pos, uchar *buff,
                    my_bool *was_last_key);
  uint (*get_key)(MARIA_KEY *key, uint page_flag, uint nod_flag,
                  uchar **page);
  uchar *(*skip_key)(MARIA_KEY *key, uint page_flag, uint nod_flag,
                     uchar *page);
  int (*pack_key)(const MARIA_KEY *key, uint nod_flag,
		  uchar *next_key, uchar *org_key, uchar *prev_key,
		  struct st_maria_s_param *s_temp);
  void (*store_key)(struct st_maria_keydef *keyinfo, uchar *key_pos,
		    struct st_maria_s_param *s_temp);
  my_bool (*ck_insert)(MARIA_HA *inf, MARIA_KEY *key);
  my_bool (*ck_delete)(MARIA_HA *inf, MARIA_KEY *klen);
  MARIA_KEY *(*make_key)(MARIA_HA *info, MARIA_KEY *int_key, uint keynr,
                         uchar *key, const uchar *record,
                         MARIA_RECORD_POS filepos, ulonglong trid);
} MARIA_KEYDEF;


#define MARIA_UNIQUE_HASH_LENGTH	4

typedef struct st_maria_unique_def	
{
  uint16 keysegs;                       
  uint8 key;                            
  uint8 null_are_equal;
  HA_KEYSEG *seg, *end;
} MARIA_UNIQUEDEF;

typedef struct st_maria_decode_tree     
{
  uint16 *table;
  uint quick_table_bits;
  uchar *intervalls;
} MARIA_DECODE_TREE;


struct st_maria_bit_buff;



typedef struct st_maria_columndef		
{
  enum en_fieldtype type;
  uint32 offset;				
  uint16 length;				
  uint16 column_nr;
  
  uint16 fill_length;
  uint16 null_pos;				
  uint16 empty_pos;                             
  uint8 null_bit;				
  
  uint8 empty_bit;

#ifndef NOT_PACKED_DATABASES
  void(*unpack)(struct st_maria_columndef *rec,
                struct st_maria_bit_buff *buff,
                uchar *start, uchar *end);
  enum en_fieldtype base_type;
  uint space_length_bits, pack_type;
  MARIA_DECODE_TREE *huff_tree;
#endif
} MARIA_COLUMNDEF;


extern ulong maria_block_size, maria_checkpoint_frequency;
extern ulong maria_concurrent_insert;
extern my_bool maria_flush, maria_single_user, maria_page_checksums;
extern my_bool maria_delay_key_write;
extern my_off_t maria_max_temp_length;
extern ulong maria_bulk_insert_tree_size, maria_data_pointer_size;
extern MY_TMPDIR *maria_tmpdir;
extern my_bool maria_encrypt_tables;


extern int (*maria_test_invalid_symlink)(const char *filename);

	

extern int maria_init(void);
extern void maria_end(void);
extern my_bool maria_upgrade(void);
extern int maria_close(MARIA_HA *file);
extern int maria_delete(MARIA_HA *file, const uchar *buff);
extern MARIA_HA *maria_open(const char *name, int mode,
					uint wait_if_locked);
extern MARIA_HA *maria_clone(struct st_maria_share *share, int mode);
extern int maria_panic(enum ha_panic_function function);
extern int maria_rfirst(MARIA_HA *file, uchar *buf, int inx);
extern int maria_rkey(MARIA_HA *file, uchar *buf, int inx,
		      const uchar *key, key_part_map keypart_map,
                      enum ha_rkey_function search_flag);
extern int maria_rlast(MARIA_HA *file, uchar *buf, int inx);
extern int maria_rnext(MARIA_HA *file, uchar *buf, int inx);
extern int maria_rnext_same(MARIA_HA *info, uchar *buf);
extern int maria_rprev(MARIA_HA *file, uchar *buf, int inx);
extern int maria_rrnd(MARIA_HA *file, uchar *buf,
                      MARIA_RECORD_POS pos);
extern int maria_scan_init(MARIA_HA *file);
extern int maria_scan(MARIA_HA *file, uchar *buf);
extern void maria_scan_end(MARIA_HA *file);
extern int maria_rsame(MARIA_HA *file, uchar *record, int inx);
extern int maria_rsame_with_pos(MARIA_HA *file, uchar *record,
				int inx, MARIA_RECORD_POS pos);
extern int maria_update(MARIA_HA *file, const uchar *old,
			const uchar *new_record);
extern int maria_write(MARIA_HA *file, uchar *buff);
extern MARIA_RECORD_POS maria_position(MARIA_HA *file);
extern int maria_status(MARIA_HA *info, MARIA_INFO *x, uint flag);
extern int maria_lock_database(MARIA_HA *file, int lock_type);
extern int maria_create(const char *name, enum data_file_type record_type,
                        uint keys, MARIA_KEYDEF *keydef,
			uint columns, MARIA_COLUMNDEF *columndef,
			uint uniques, MARIA_UNIQUEDEF *uniquedef,
			MARIA_CREATE_INFO *create_info, uint flags);
extern int maria_delete_table(const char *name);
extern int maria_rename(const char *from, const char *to);
extern int maria_extra(MARIA_HA *file,
		       enum ha_extra_function function, void *extra_arg);
extern int maria_reset(MARIA_HA *file);
extern ha_rows maria_records_in_range(MARIA_HA *info, int inx,
				      key_range *min_key, key_range *max_key);
extern int maria_is_changed(MARIA_HA *info);
extern int maria_delete_all_rows(MARIA_HA *info);
extern uint maria_get_pointer_length(ulonglong file_length, uint def);
extern int maria_commit(MARIA_HA *info);
extern int maria_begin(MARIA_HA *info);
extern void maria_disable_logging(MARIA_HA *info);
extern void maria_enable_logging(MARIA_HA *info);

#define HA_RECOVER_NONE         0       
#define HA_RECOVER_DEFAULT      1       
#define HA_RECOVER_BACKUP       2       
#define HA_RECOVER_FORCE        4       
#define HA_RECOVER_QUICK        8       

#define HA_RECOVER_ANY (HA_RECOVER_DEFAULT | HA_RECOVER_BACKUP | HA_RECOVER_FORCE | HA_RECOVER_QUICK)



#define MARIA_CHK_REPAIR 1              
#define MARIA_CHK_VERIFY 2              

typedef uint maria_bit_type;

typedef struct st_maria_bit_buff
{					
  maria_bit_type current_byte;
  uint bits;
  uchar *pos, *end, *blob_pos, *blob_end;
  uint error;
} MARIA_BIT_BUFF;


void maria_chk_init(HA_CHECK *param);
void maria_chk_init_for_check(HA_CHECK *param, MARIA_HA *info);
int maria_chk_status(HA_CHECK *param, MARIA_HA *info);
int maria_chk_del(HA_CHECK *param, MARIA_HA *info, ulonglong test_flag);
int maria_chk_size(HA_CHECK *param, MARIA_HA *info);
int maria_chk_key(HA_CHECK *param, MARIA_HA *info);
int maria_chk_data_link(HA_CHECK *param, MARIA_HA *info, my_bool extend);
int maria_repair(HA_CHECK *param, MARIA_HA *info, char * name, my_bool);
int maria_sort_index(HA_CHECK *param, MARIA_HA *info, char * name);
int maria_zerofill(HA_CHECK *param, MARIA_HA *info, const char *name);
int maria_repair_by_sort(HA_CHECK *param, MARIA_HA *info,
			 const char *name, my_bool rep_quick);
int maria_repair_parallel(HA_CHECK *param, MARIA_HA *info,
			  const char *name, my_bool rep_quick);
int maria_change_to_newfile(const char *filename, const char *old_ext,
                            const char *new_ext, time_t backup_time,
                            myf myflags);
void maria_lock_memory(HA_CHECK *param);
int maria_update_state_info(HA_CHECK *param, MARIA_HA *info, uint update);
void maria_update_key_parts(MARIA_KEYDEF *keyinfo, double *rec_per_key_part,
                            ulonglong *unique, ulonglong *notnull,
                            ulonglong records);
int maria_filecopy(HA_CHECK *param, File to, File from, my_off_t start,
                   my_off_t length, const char *type);
int maria_movepoint(MARIA_HA *info, uchar *record, my_off_t oldpos,
                    my_off_t newpos, uint prot_key);
int maria_test_if_almost_full(MARIA_HA *info);
int maria_recreate_table(HA_CHECK *param, MARIA_HA **org_info, char *filename);
int maria_disable_indexes(MARIA_HA *info);
int maria_enable_indexes(MARIA_HA *info);
int maria_indexes_are_disabled(MARIA_HA *info);
void maria_disable_indexes_for_rebuild(MARIA_HA *info, ha_rows rows,
                                       my_bool all_keys);
my_bool maria_test_if_sort_rep(MARIA_HA *info, ha_rows rows, ulonglong key_map,
			       my_bool force);

int maria_init_bulk_insert(MARIA_HA *info, size_t cache_size, ha_rows rows);
void maria_flush_bulk_insert(MARIA_HA *info, uint inx);
int maria_end_bulk_insert(MARIA_HA *info, my_bool abort);
int maria_preload(MARIA_HA *info, ulonglong key_map, my_bool ignore_leaves);
void maria_versioning(MARIA_HA *info, my_bool versioning);
void maria_ignore_trids(MARIA_HA *info);
uint maria_max_key_length(void);
#define maria_max_key_segments() HA_MAX_KEY_SEG


FT_INFO *maria_ft_init_search(uint,void *, uint, uchar *, size_t,
                              CHARSET_INFO *, uchar *);



void _ma_update_auto_increment_key(HA_CHECK *param, MARIA_HA *info,
                                  my_bool repair);


#ifdef	__cplusplus
}
#endif
#endif
