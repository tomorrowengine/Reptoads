

#ifndef _my_sys_h
#define _my_sys_h

#include <m_string.h>

C_MODE_START

#ifdef HAVE_AIOWAIT
#include <sys/asynch.h>			
typedef struct my_aio_result {
  aio_result_t result;
  int	       pending;
} my_aio_result;
#endif

#include <my_valgrind.h>

#include <my_pthread.h>

#include <m_ctype.h>                    
#include <stdarg.h>
#include <typelib.h>
#ifdef _WIN32
#include <malloc.h> 
#endif
#include <mysql/plugin.h>
#include <mysql/service_my_print_error.h>

#define MY_INIT(name)   { my_progname= name; my_init(); }


#define MYSYS_ERRMSG_SIZE   (512)
#define MYSYS_STRERROR_SIZE (128)

#define MY_FILE_ERROR	((size_t) -1)

	
#define MY_FFNF		1U	
#define MY_FNABP	2U	
#define MY_NABP		4U	
#define MY_FAE		8U	
#define MY_WME		16U	
#define MY_WAIT_IF_FULL 32U	
#define MY_IGNORE_BADFD 32U     
#define MY_ENCRYPT      64U     
#define MY_TEMPORARY    64U     
#define MY_NOSYMLINKS  512U     
#define MY_FULL_IO     512U     
#define MY_DONT_CHECK_FILESIZE 128U 
#define MY_LINK_WARNING 32U	
#define MY_COPYTIME	64U	
#define MY_DELETE_OLD	256U	
#define MY_RESOLVE_LINK 128U	
#define MY_HOLD_ORIGINAL_MODES 128U  
#define MY_REDEL_MAKE_BACKUP 256U
#define MY_SEEK_NOT_DONE 32U	
#define MY_SHORT_WAIT	64U	
#define MY_FORCE_LOCK   128U    
#define MY_NO_WAIT      256U	

#define MY_INIT_BUFFER_USED 256U
#define MY_ZEROFILL	32U	
#define MY_ALLOW_ZERO_PTR 64U	
#define MY_FREE_ON_ERROR 128U	
#define MY_HOLD_ON_ERROR 256U	
#define MY_DONT_OVERWRITE_FILE 2048U 
#define MY_THREADSAFE 2048U     
#define MY_SYNC       4096U     
#define MY_SYNC_DIR   32768U    
#define MY_SYNC_FILESIZE 65536U 
#define MY_THREAD_SPECIFIC 0x10000U 
#define MY_THREAD_MOVE     0x20000U 

#define MY_TREE_WITH_DELETE 0x40000U

#define MY_CHECK_ERROR	1U	
#define MY_GIVE_INFO	2U	
#define MY_DONT_FREE_DBUG 4U    

#define ME_BELL         4U      
#define ME_WAITTANG     0       
#define ME_NOREFRESH    64U     
#define ME_NOINPUT      0       
#define ME_JUST_INFO    1024U   
#define ME_JUST_WARNING 2048U   
#define ME_FATALERROR   4096U   

	
#define MY_REPLACE_DIR		1U	
#define MY_REPLACE_EXT		2U	
#define MY_UNPACK_FILENAME	4U	
#define MY_PACK_FILENAME	8U	
#define MY_RESOLVE_SYMLINKS	16U	
#define MY_RETURN_REAL_PATH	32U	
#define MY_SAFE_PATH		64U	
#define MY_RELATIVE_PATH	128U	
#define MY_APPEND_EXT           256U    


	
#define MY_SEEK_SET	0
#define MY_SEEK_CUR	1
#define MY_SEEK_END	2

	
#define MY_WAIT_FOR_USER_TO_FIX_PANIC	60	
#define MY_WAIT_GIVE_USER_A_MESSAGE	10	
#define MIN_COMPRESS_LENGTH		50	
#define DFLT_INIT_HITS  3

	
#define MY_KEEP_PREALLOC	1U
#define MY_MARK_BLOCKS_FREE     2U 

	
#define MY_ERRNO_EDOM		33
#define MY_ERRNO_ERANGE		34

	
#define GETDATE_DATE_TIME	1U
#define GETDATE_SHORT_DATE	2U
#define GETDATE_HHMMSSTIME	4U
#define GETDATE_GMT		8U
#define GETDATE_FIXEDLENGTH	16U


#define MY_BACKUP_NAME_EXTRA_LENGTH 17

char *guess_malloc_library();


#if defined(SAFEMALLOC)
void sf_report_leaked_memory(my_thread_id id);
int sf_sanity();
extern my_thread_id (*sf_malloc_dbug_id)(void);
#define SAFEMALLOC_REPORT_MEMORY(X) sf_report_leaked_memory(X)
#else
#define SAFEMALLOC_REPORT_MEMORY(X) do {} while(0)
#endif

typedef void (*MALLOC_SIZE_CB) (long long size, my_bool is_thread_specific); 
extern void set_malloc_size_cb(MALLOC_SIZE_CB func);

	
extern void *my_malloc(size_t Size,myf MyFlags);
extern void *my_multi_malloc(myf MyFlags, ...);
extern void *my_multi_malloc_large(myf MyFlags, ...);
extern void *my_realloc(void *oldpoint, size_t Size, myf MyFlags);
extern void my_free(void *ptr);
extern void *my_memdup(const void *from,size_t length,myf MyFlags);
extern char *my_strdup(const char *from,myf MyFlags);
extern char *my_strndup(const char *from, size_t length, myf MyFlags);

#ifdef HAVE_LINUX_LARGE_PAGES
extern uint my_get_large_page_size(void);
extern uchar * my_large_malloc(size_t size, myf my_flags);
extern void my_large_free(uchar *ptr);
#else
#define my_get_large_page_size() (0)
#define my_large_malloc(A,B) my_malloc_lock((A),(B))
#define my_large_free(A) my_free_lock((A))
#endif 

void my_init_atomic_write(void);
#ifdef __linux__
my_bool my_test_if_atomic_write(File handle, int pagesize);
#else
#define my_test_if_atomic_write(A, B) 0
#endif 
extern my_bool my_may_have_atomic_write;

#if defined(HAVE_ALLOCA) && !defined(HAVE_valgrind)
#if defined(_AIX) && !defined(__GNUC__) && !defined(_AIX43)
#pragma alloca
#endif 
#if defined(__MWERKS__)
#undef alloca
#define alloca _alloca
#endif 
#if defined(__GNUC__) && !defined(HAVE_ALLOCA_H) && ! defined(alloca)
#define alloca __builtin_alloca
#endif 
#define my_alloca(SZ) alloca((size_t) (SZ))
#define my_afree(PTR) ((void)0)
#define MAX_ALLOCA_SZ 4096
#define my_safe_alloca(size) (((size) <= MAX_ALLOCA_SZ) ? \
                               my_alloca(size) : \
                               my_malloc((size), MYF(MY_THREAD_SPECIFIC|MY_WME)))
#define my_safe_afree(ptr, size) \
                  do { if ((size) > MAX_ALLOCA_SZ) my_free(ptr); } while(0)
#else
#define my_alloca(SZ) my_malloc(SZ,MYF(MY_FAE))
#define my_afree(PTR) my_free(PTR)
#define my_safe_alloca(size) my_alloca(size)
#define my_safe_afree(ptr, size) my_afree(ptr)
#endif 

#ifndef errno				
#ifdef HAVE_ERRNO_AS_DEFINE
#include <errno.h>			
#else
extern int errno;			
#endif
#endif					
extern char *home_dir;			
extern MYSQL_PLUGIN_IMPORT char  *mysql_data_home;
extern const char *my_progname;		
extern const char *my_progname_short;	
extern char curr_dir[];		
extern void (*error_handler_hook)(uint my_err, const char *str,myf MyFlags);
extern void (*fatal_error_handler_hook)(uint my_err, const char *str,
				       myf MyFlags);
extern uint my_file_limit;
extern ulonglong my_thread_stack_size;
extern int sf_leaking_memory; 

extern void (*proc_info_hook)(void *, const PSI_stage_info *, PSI_stage_info *,
                              const char *, const char *, const unsigned int);

#ifdef HAVE_LINUX_LARGE_PAGES
extern my_bool my_use_large_pages;
extern uint    my_large_page_size;
#endif


#define MY_ALL_CHARSETS_SIZE 2048
extern MYSQL_PLUGIN_IMPORT CHARSET_INFO *default_charset_info;
extern MYSQL_PLUGIN_IMPORT CHARSET_INFO *all_charsets[MY_ALL_CHARSETS_SIZE];
extern struct charset_info_st compiled_charsets[];


extern my_bool my_collation_is_known_id(uint id);
extern ulonglong my_collation_statistics_get_use_count(uint id);
extern const char *my_collation_get_tailoring(uint id);


extern ulong	my_file_opened,my_stream_opened, my_tmp_file_created;
extern ulong    my_file_total_opened;
extern ulong    my_sync_count;
extern uint	mysys_usage_id;
extern my_bool	my_init_done, my_thr_key_mysys_exists;
extern my_bool  my_assert_on_error;
extern myf      my_global_flags;        
					
extern void (*my_sigtstp_cleanup)(void),
					
	    (*my_sigtstp_restart)(void);
					
extern MYSQL_PLUGIN_IMPORT int my_umask;		
extern int my_umask_dir,
	   my_recived_signals,	
	   my_safe_to_handle_signal, 
	   my_dont_interrupt;	
extern my_bool my_use_symdir;

extern ulong	my_default_record_cache_size;
extern my_bool  my_disable_locking, my_disable_async_io,
                my_disable_flush_key_blocks, my_disable_symlinks;
extern my_bool my_disable_sync, my_disable_copystat_in_redel;
extern char	wild_many,wild_one,wild_prefix;
extern const char *charsets_dir;
extern my_bool timed_mutexes;

enum cache_type
{
  TYPE_NOT_SET= 0, READ_CACHE, WRITE_CACHE,
  SEQ_READ_APPEND		,
  READ_FIFO, READ_NET};

enum flush_type
{
  FLUSH_KEEP,           
  FLUSH_RELEASE,        
  FLUSH_IGNORE_CHANGED, 
  
  FLUSH_FORCE_WRITE,
  
  FLUSH_KEEP_LAZY
};

typedef struct st_record_cache	
{
  File file;
  int	rc_seek,error,inited;
  uint	rc_length,read_length,reclength;
  my_off_t rc_record_pos,end_of_file;
  uchar *rc_buff,*rc_buff2,*rc_pos,*rc_end,*rc_request_pos;
#ifdef HAVE_AIOWAIT
  int	use_async_io;
  my_aio_result aio_result;
#endif
  enum cache_type type;
} RECORD_CACHE;

enum file_type
{
  UNOPEN = 0, FILE_BY_OPEN, FILE_BY_CREATE, STREAM_BY_FOPEN, STREAM_BY_FDOPEN,
  FILE_BY_MKSTEMP, FILE_BY_DUP
};

struct st_my_file_info
{
  char  *name;
#ifdef _WIN32
  HANDLE fhandle;   
  int    oflag;     
#endif
  enum   file_type	type;
};

extern struct st_my_file_info *my_file_info;


typedef void (*FREE_FUNC)(void *);

typedef struct st_dynamic_array
{
  uchar *buffer;
  uint elements,max_element;
  uint alloc_increment;
  uint size_of_element;
  myf malloc_flags;
} DYNAMIC_ARRAY;

typedef struct st_my_tmpdir
{
  DYNAMIC_ARRAY full_list;
  char **list;
  uint cur, max;
  mysql_mutex_t mutex;
} MY_TMPDIR;

typedef struct st_dynamic_string
{
  char *str;
  size_t length,max_length,alloc_increment;
} DYNAMIC_STRING;

struct st_io_cache;

typedef struct st_io_cache_share
{
  mysql_mutex_t       mutex;           
  mysql_cond_t        cond;            
  mysql_cond_t        cond_writer;     
  
  my_off_t              pos_in_file;
  
  struct st_io_cache    *source_cache;
  uchar                 *buffer;         
  uchar                 *read_end;       
  int                   running_threads; 
  int                   total_threads;   
  int                   error;           
#ifdef NOT_YET_IMPLEMENTED
  
  my_bool alloced;
#endif
} IO_CACHE_SHARE;

typedef struct st_io_cache		
{
  
  my_off_t pos_in_file;
  
  my_off_t end_of_file;
  
  uchar	*read_pos;
  
  uchar  *read_end;
  uchar  *buffer;				
  
  uchar  *request_pos;

  
  uchar  *write_buffer;
  
  uchar *append_read_pos;
  
  uchar *write_pos;
  
  uchar *write_end;

  
  mysql_mutex_t append_buffer_lock;
  
  IO_CACHE_SHARE *share;

  
  int (*read_function)(struct st_io_cache *,uchar *,size_t);
  
  int (*write_function)(struct st_io_cache *,const uchar *,size_t);
  
  enum cache_type type;
  
  ulong disk_writes;
  char *file_name;			
  const char *dir;
  char prefix[3];
  File file; 

  struct st_io_cache *next_file_user;
  
  int	seek_not_done,error;
  
  size_t	buffer_length;
  
  size_t  read_length;
  myf	myflags;			
  
  my_bool alloced_buffer;
#ifdef HAVE_AIOWAIT
  
  uint inited;
  my_off_t aio_read_pos;
  my_aio_result aio_result;
#endif
} IO_CACHE;

typedef int (*qsort2_cmp)(const void *, const void *, const void *);

typedef void (*my_error_reporter)(enum loglevel level, const char *format, ...)
  ATTRIBUTE_FORMAT_FPTR(printf, 2, 3);

extern my_error_reporter my_charset_error_reporter;



extern int my_b_flush_io_cache(IO_CACHE *info, int need_append_buffer_lock);
extern int _my_b_get(IO_CACHE *info);
extern int _my_b_read(IO_CACHE *info,uchar *Buffer,size_t Count);
extern int _my_b_write(IO_CACHE *info,const uchar *Buffer,size_t Count);


static inline void my_b_clear(IO_CACHE *info) { info->buffer= 0; }
static inline int my_b_inited(IO_CACHE *info) { return MY_TEST(info->buffer); }
#define my_b_EOF INT_MIN

static inline int my_b_read(IO_CACHE *info, uchar *Buffer, size_t Count)
{
  if (info->read_pos + Count <= info->read_end)
  {
    memcpy(Buffer, info->read_pos, Count);
    info->read_pos+= Count;
    return 0;
  }
  return _my_b_read(info, Buffer, Count);
}

static inline int my_b_write(IO_CACHE *info, const uchar *Buffer, size_t Count)
{
  if (info->write_pos + Count <= info->write_end)
  {
    memcpy(info->write_pos, Buffer, Count);
    info->write_pos+= Count;
    return 0;
  }
  return _my_b_write(info, Buffer, Count);
}

static inline int my_b_get(IO_CACHE *info)
{
  if (info->read_pos != info->read_end)
  {
    info->read_pos++;
    return info->read_pos[-1];
  }
  return _my_b_get(info);
}

static inline my_bool my_b_write_byte(IO_CACHE *info, uchar chr)
{
  if (info->write_pos >= info->write_end)
    if (my_b_flush_io_cache(info, 1))
      return 1;
  *info->write_pos++= chr;
  return 0;
}


static inline size_t my_b_fill(IO_CACHE *info)
{
  info->read_pos= info->read_end;
  return _my_b_read(info,0,0) ? 0 : (size_t) (info->read_end - info->read_pos);
}

static inline my_off_t my_b_tell(const IO_CACHE *info)
{
  if (info->type == WRITE_CACHE) {
    return info->pos_in_file + (my_off_t)(info->write_pos - info->request_pos);

  }
  return info->pos_in_file + (my_off_t) (info->read_pos - info->request_pos);
}

static inline my_off_t my_b_write_tell(const IO_CACHE *info)
{
  return info->pos_in_file + (my_off_t) (info->write_pos - info->write_buffer);
}

static inline uchar* my_b_get_buffer_start(const IO_CACHE *info)
{
  return info->request_pos;
}

static inline size_t my_b_get_bytes_in_buffer(const IO_CACHE *info)
{
  return (size_t) (info->read_end - info->request_pos);
}

static inline my_off_t my_b_get_pos_in_file(const IO_CACHE *info)
{
  return info->pos_in_file;
}

static inline size_t my_b_bytes_in_cache(const IO_CACHE *info)
{
  if (info->type == WRITE_CACHE) {
    return (size_t) (info->write_end - info->write_pos);
  }
  return (size_t) (info->read_end - info->read_pos);
}

int my_b_copy_to_file    (IO_CACHE *cache, FILE *file, size_t count);
int my_b_copy_all_to_file(IO_CACHE *cache, FILE *file);

my_off_t my_b_append_tell(IO_CACHE* info);
my_off_t my_b_safe_tell(IO_CACHE* info); 
int my_b_pread(IO_CACHE *info, uchar *Buffer, size_t Count, my_off_t pos);

typedef uint32 ha_checksum;

extern int (*mysys_test_invalid_symlink)(const char *filename);
#include <my_alloc.h>

	

extern int my_copy(const char *from,const char *to,myf MyFlags);
extern int my_delete(const char *name,myf MyFlags);
extern int my_rmtree(const char *name, myf Myflags);
extern int my_getwd(char * buf,size_t size,myf MyFlags);
extern int my_setwd(const char *dir,myf MyFlags);
extern int my_lock(File fd,int op,my_off_t start, my_off_t length,myf MyFlags);
extern void *my_once_alloc(size_t Size,myf MyFlags);
extern void my_once_free(void);
extern char *my_once_strdup(const char *src,myf myflags);
extern void *my_once_memdup(const void *src, size_t len, myf myflags);
extern File my_open(const char *FileName,int Flags,myf MyFlags);
extern File my_register_filename(File fd, const char *FileName,
				 enum file_type type_of_file,
				 uint error_message_number, myf MyFlags);
extern File my_create(const char *FileName,int CreateFlags,
		      int AccessFlags, myf MyFlags);
extern int my_close(File Filedes,myf MyFlags);
extern int my_mkdir(const char *dir, int Flags, myf MyFlags);
extern int my_readlink(char *to, const char *filename, myf MyFlags);
extern int my_is_symlink(const char *filename);
extern int my_realpath(char *to, const char *filename, myf MyFlags);
extern File my_create_with_symlink(const char *linkname, const char *filename,
				   int createflags, int access_flags,
				   myf MyFlags);
extern int my_rename_with_symlink(const char *from,const char *to,myf MyFlags);
extern int my_symlink(const char *content, const char *linkname, myf MyFlags);
extern int my_handler_delete_with_symlink(const char *filename, myf sync_dir);

extern size_t my_read(File Filedes,uchar *Buffer,size_t Count,myf MyFlags);
extern size_t my_pread(File Filedes,uchar *Buffer,size_t Count,my_off_t offset,
		     myf MyFlags);
extern int my_rename(const char *from,const char *to,myf MyFlags);
extern my_off_t my_seek(File fd,my_off_t pos,int whence,myf MyFlags);
extern my_off_t my_tell(File fd,myf MyFlags);
extern size_t my_write(File Filedes,const uchar *Buffer,size_t Count,
		     myf MyFlags);
extern size_t my_pwrite(File Filedes,const uchar *Buffer,size_t Count,
		      my_off_t offset,myf MyFlags);
extern size_t my_fread(FILE *stream,uchar *Buffer,size_t Count,myf MyFlags);
extern size_t my_fwrite(FILE *stream,const uchar *Buffer,size_t Count,
		      myf MyFlags);
extern my_off_t my_fseek(FILE *stream,my_off_t pos,int whence,myf MyFlags);
extern my_off_t my_ftell(FILE *stream,myf MyFlags);


extern void *my_memmem(const void *haystack, size_t haystacklen,
                       const void *needle, size_t needlelen);


#ifdef _WIN32
extern int      my_access(const char *path, int amode);
#define my_check_user(A,B) (NULL)
#define my_set_user(A,B,C) (0)
#else
#define my_access access
struct passwd *my_check_user(const char *user, myf MyFlags);
int my_set_user(const char *user, struct passwd *user_info, myf MyFlags);
#endif

extern int check_if_legal_filename(const char *path);
extern int check_if_legal_tablename(const char *path);

#ifdef __WIN__
extern my_bool is_filename_allowed(const char *name, size_t length,
                   my_bool allow_current_dir);
#else 
# define is_filename_allowed(name, length, allow_cwd) (TRUE)
#endif  

#ifdef _WIN32

extern HANDLE   my_get_osfhandle(File fd);
extern void     my_osmaperr(unsigned long last_error);
#endif

extern void init_glob_errs(void);
extern const char** get_global_errmsgs(int nr);
extern void wait_for_free_space(const char *filename, int errors);
extern FILE *my_fopen(const char *FileName,int Flags,myf MyFlags);
extern FILE *my_fdopen(File Filedes,const char *name, int Flags,myf MyFlags);
extern FILE *my_freopen(const char *path, const char *mode, FILE *stream);
extern int my_fclose(FILE *fd,myf MyFlags);
extern int my_vfprintf(FILE *stream, const char* format, va_list args);
extern const char* my_strerror(char *buf, size_t len, int nr);
extern int my_fprintf(FILE *stream, const char* format, ...);
extern File my_fileno(FILE *fd);
extern int my_chsize(File fd,my_off_t newlength, int filler, myf MyFlags);
extern int my_chmod(const char *name, mode_t mode, myf my_flags);
extern const char *my_basename(const char *filename);
extern void thr_set_sync_wait_callback(void (*before_sync)(void),
                                       void (*after_sync)(void));
extern int my_sync(File fd, myf my_flags);
extern int my_sync_dir(const char *dir_name, myf my_flags);
extern int my_sync_dir_by_file(const char *file_name, myf my_flags);
extern const char *my_get_err_msg(uint nr);
extern int my_error_register(const char** (*get_errmsgs) (int nr),
                             uint first, uint last);
extern my_bool my_error_unregister(uint first, uint last);
extern void my_message(uint my_err, const char *str,myf MyFlags);
extern void my_message_stderr(uint my_err, const char *str, myf MyFlags);
extern my_bool my_init(void);
extern void my_end(int infoflag);
extern int my_redel(const char *from, const char *to, time_t backup_time_stamp,
                    myf MyFlags);
void my_create_backup_name(char *to, const char *from,
                           time_t backup_time_stamp);
extern int my_copystat(const char *from, const char *to, int MyFlags);
extern char * my_filename(File fd);

extern my_bool init_tmpdir(MY_TMPDIR *tmpdir, const char *pathlist);
extern char *my_tmpdir(MY_TMPDIR *tmpdir);
extern void free_tmpdir(MY_TMPDIR *tmpdir);

extern void my_remember_signal(int signal_number,sig_handler (*func)(int));
extern size_t dirname_part(char * to,const char *name, size_t *to_res_length);
extern size_t dirname_length(const char *name);
#define base_name(A) (A+dirname_length(A))
extern int test_if_hard_path(const char *dir_name);
extern my_bool has_path(const char *name);
extern char *convert_dirname(char *to, const char *from, const char *from_end);
extern void to_unix_path(char * name);
extern char * fn_ext(const char *name);
extern char * fn_ext2(const char *name);
extern char * fn_same(char * toname,const char *name,int flag);
extern char * fn_format(char * to,const char *name,const char *dir,
			   const char *form, uint flag);
extern size_t strlength(const char *str);
extern void pack_dirname(char * to,const char *from);
extern size_t normalize_dirname(char * to, const char *from);
extern size_t unpack_dirname(char * to,const char *from);
extern size_t cleanup_dirname(char * to,const char *from);
extern size_t system_filename(char * to,const char *from);
extern size_t unpack_filename(char * to,const char *from);
extern char * intern_filename(char * to,const char *from);
extern int pack_filename(char * to, const char *name, size_t max_length);
extern char * my_path(char * to,const char *progname,
			 const char *own_pathname_part);
extern char * my_load_path(char * to, const char *path,
			      const char *own_path_prefix);
extern int wild_compare(const char *str,const char *wildstr,
                        pbool str_is_pattern);
extern my_bool array_append_string_unique(const char *str,
                                          const char **array, size_t size);
extern void get_date(char * to,int timeflag,time_t use_time);
extern void soundex(CHARSET_INFO *, char * out_pntr, char * in_pntr,
                    pbool remove_garbage);
extern int init_record_cache(RECORD_CACHE *info,size_t cachesize,File file,
			     size_t reclength,enum cache_type type,
			     pbool use_async_io);
extern int read_cache_record(RECORD_CACHE *info,uchar *to);
extern int end_record_cache(RECORD_CACHE *info);
extern int write_cache_record(RECORD_CACHE *info,my_off_t filepos,
			      const uchar *record,size_t length);
extern int flush_write_cache(RECORD_CACHE *info);
extern void handle_recived_signals(void);

extern sig_handler my_set_alarm_variable(int signo);
extern my_bool radixsort_is_appliccable(uint n_items, size_t size_of_element);
extern void my_string_ptr_sort(uchar *base,uint items,size_t size);
extern void radixsort_for_str_ptr(uchar* base[], uint number_of_elements,
				  size_t size_of_element,uchar *buffer[]);
extern qsort_t my_qsort(void *base_ptr, size_t total_elems, size_t size,
                        qsort_cmp cmp);
extern qsort_t my_qsort2(void *base_ptr, size_t total_elems, size_t size,
                         qsort2_cmp cmp, void *cmp_argument);
extern qsort2_cmp get_ptr_compare(size_t);
void my_store_ptr(uchar *buff, size_t pack_length, my_off_t pos);
my_off_t my_get_ptr(uchar *ptr, size_t pack_length);
extern int init_io_cache(IO_CACHE *info,File file,size_t cachesize,
			 enum cache_type type,my_off_t seek_offset,
			 my_bool use_async_io, myf cache_myflags);
extern my_bool reinit_io_cache(IO_CACHE *info,enum cache_type type,
			       my_off_t seek_offset, my_bool use_async_io,
			       my_bool clear_cache);
extern void init_io_cache_share(IO_CACHE *read_cache, IO_CACHE_SHARE *cshare,
                                IO_CACHE *write_cache, uint num_threads);

extern int init_slave_io_cache(IO_CACHE *master, IO_CACHE *slave);
void end_slave_io_cache(IO_CACHE *cache);
void seek_io_cache(IO_CACHE *cache, my_off_t needed_offset);

extern void remove_io_thread(IO_CACHE *info);
extern int _my_b_async_read(IO_CACHE *info,uchar *Buffer,size_t Count);
extern int my_b_append(IO_CACHE *info,const uchar *Buffer,size_t Count);
extern int my_b_safe_write(IO_CACHE *info,const uchar *Buffer,size_t Count);

extern int my_block_write(IO_CACHE *info, const uchar *Buffer,
			  size_t Count, my_off_t pos);

#define flush_io_cache(info) my_b_flush_io_cache((info),1)

extern int end_io_cache(IO_CACHE *info);
extern void my_b_seek(IO_CACHE *info,my_off_t pos);
extern size_t my_b_gets(IO_CACHE *info, char *to, size_t max_length);
extern my_off_t my_b_filelength(IO_CACHE *info);
extern my_bool my_b_write_backtick_quote(IO_CACHE *info, const char *str,
                                         size_t len);
extern my_bool my_b_printf(IO_CACHE *info, const char* fmt, ...);
extern size_t my_b_vprintf(IO_CACHE *info, const char* fmt, va_list ap);
extern my_bool open_cached_file(IO_CACHE *cache,const char *dir,
				 const char *prefix, size_t cache_size,
				 myf cache_myflags);
extern my_bool real_open_cached_file(IO_CACHE *cache);
extern void close_cached_file(IO_CACHE *cache);
File create_temp_file(char *to, const char *dir, const char *pfx,
		      int mode, myf MyFlags);
#define my_init_dynamic_array(A,B,C,D,E) init_dynamic_array2(A,B,NULL,C,D,E)
#define my_init_dynamic_array2(A,B,C,D,E,F) init_dynamic_array2(A,B,C,D,E,F)
extern my_bool init_dynamic_array2(DYNAMIC_ARRAY *array, uint element_size,
                                   void *init_buffer, uint init_alloc,
                                   uint alloc_increment, myf my_flags);
extern my_bool insert_dynamic(DYNAMIC_ARRAY *array, const void* element);
extern void *alloc_dynamic(DYNAMIC_ARRAY *array);
extern void *pop_dynamic(DYNAMIC_ARRAY*);
extern my_bool set_dynamic(DYNAMIC_ARRAY *array, const void *element,
                           uint array_index);
extern my_bool allocate_dynamic(DYNAMIC_ARRAY *array, uint max_elements);
extern void get_dynamic(DYNAMIC_ARRAY *array, void *element, uint array_index);
extern void delete_dynamic(DYNAMIC_ARRAY *array);
extern void delete_dynamic_element(DYNAMIC_ARRAY *array, uint array_index);
extern void delete_dynamic_with_callback(DYNAMIC_ARRAY *array, FREE_FUNC f);
extern void freeze_size(DYNAMIC_ARRAY *array);
extern int  get_index_dynamic(DYNAMIC_ARRAY *array, void *element);
#define dynamic_array_ptr(array,array_index) ((array)->buffer+(array_index)*(array)->size_of_element)
#define dynamic_element(array,array_index,type) ((type)((array)->buffer) +(array_index))
#define push_dynamic(A,B) insert_dynamic((A),(B))
#define reset_dynamic(array) ((array)->elements= 0)
#define sort_dynamic(A,cmp) my_qsort((A)->buffer, (A)->elements, (A)->size_of_element, (cmp))

extern my_bool init_dynamic_string(DYNAMIC_STRING *str, const char *init_str,
				   size_t init_alloc,size_t alloc_increment);
extern my_bool dynstr_append(DYNAMIC_STRING *str, const char *append);
my_bool dynstr_append_mem(DYNAMIC_STRING *str, const char *append,
			  size_t length);
extern my_bool dynstr_append_os_quoted(DYNAMIC_STRING *str, const char *append,
                                       ...);
extern my_bool dynstr_append_quoted(DYNAMIC_STRING *str,
                                    const char *append, size_t len,
                                    char quote);
extern my_bool dynstr_set(DYNAMIC_STRING *str, const char *init_str);
extern my_bool dynstr_realloc(DYNAMIC_STRING *str, size_t additional_size);
extern my_bool dynstr_trunc(DYNAMIC_STRING *str, size_t n);
extern void dynstr_free(DYNAMIC_STRING *str);
extern uint32 copy_and_convert_extended(char *to, uint32 to_length,
                                        CHARSET_INFO *to_cs,
                                        const char *from, uint32 from_length,
                                        CHARSET_INFO *from_cs, uint *errors);
extern void dynstr_reassociate(DYNAMIC_STRING *str, char **res, size_t *length,
                               size_t *alloc_length);
extern uint32 copy_and_convert_extended(char *to, uint32 to_length,
                                        CHARSET_INFO *to_cs,
                                        const char *from, uint32 from_length,
                                        CHARSET_INFO *from_cs, uint *errors);
#ifdef HAVE_MLOCK
extern void *my_malloc_lock(size_t length,myf flags);
extern void my_free_lock(void *ptr);
#else
#define my_malloc_lock(A,B) my_malloc((A),(B))
#define my_free_lock(A) my_free((A))
#endif
#define alloc_root_inited(A) ((A)->min_malloc != 0)
#define ALLOC_ROOT_MIN_BLOCK_SIZE (MALLOC_OVERHEAD + sizeof(USED_MEM) + 8)
#define clear_alloc_root(A) do { (A)->free= (A)->used= (A)->pre_alloc= 0; (A)->min_malloc=0;} while(0)
extern void init_alloc_root(MEM_ROOT *mem_root, const char *name,
                            size_t block_size, size_t pre_alloc_size,
                            myf my_flags);
extern void *alloc_root(MEM_ROOT *mem_root, size_t Size);
extern void *multi_alloc_root(MEM_ROOT *mem_root, ...);
extern void free_root(MEM_ROOT *root, myf MyFLAGS);
extern void set_prealloc_root(MEM_ROOT *root, char *ptr);
extern void reset_root_defaults(MEM_ROOT *mem_root, size_t block_size,
                                size_t prealloc_size);
extern char *strdup_root(MEM_ROOT *root,const char *str);
static inline char *safe_strdup_root(MEM_ROOT *root, const char *str)
{
  return str ? strdup_root(root, str) : 0;
}
extern char *strmake_root(MEM_ROOT *root,const char *str,size_t len);
extern void *memdup_root(MEM_ROOT *root,const void *str, size_t len);
extern my_bool my_compress(uchar *, size_t *, size_t *);
extern my_bool my_uncompress(uchar *, size_t , size_t *);
extern uchar *my_compress_alloc(const uchar *packet, size_t *len,
                                size_t *complen);
extern void *my_az_allocator(void *dummy, unsigned int items, unsigned int size);
extern void my_az_free(void *dummy, void *address);
extern int my_compress_buffer(uchar *dest, size_t *destLen,
                              const uchar *source, size_t sourceLen);
extern int packfrm(const uchar *, size_t, uchar **, size_t *);
extern int unpackfrm(uchar **, size_t *, const uchar *);

extern ha_checksum my_checksum(ha_checksum crc, const uchar *mem,
                               size_t count);
#ifdef DBUG_ASSERT_EXISTS
extern void my_debug_put_break_here(void);
#else
#define my_debug_put_break_here() do {} while(0)
#endif

extern void my_sleep(ulong m_seconds);
extern ulong crc32(ulong crc, const uchar *buf, uint len);
extern uint my_set_max_open_files(uint files);
void my_free_open_file_info(void);

extern my_bool my_gethwaddr(uchar *to);
extern int my_getncpus(void);

#define HRTIME_RESOLUTION               1000000ULL  
typedef struct {ulonglong val;} my_hrtime_t;
void my_time_init(void);
extern my_hrtime_t my_hrtime(void);

#ifdef _WIN32
extern my_hrtime_t my_hrtime_coarse();
#else
#define my_hrtime_coarse() my_hrtime()
#endif

extern ulonglong my_interval_timer(void);
extern ulonglong my_getcputime(void);

#define microsecond_interval_timer()    (my_interval_timer()/1000)
#define hrtime_to_time(X)               ((X).val/HRTIME_RESOLUTION)
#define hrtime_from_time(X)             ((ulonglong)((X)*HRTIME_RESOLUTION))
#define hrtime_to_double(X)             ((X).val/(double)HRTIME_RESOLUTION)
#define hrtime_sec_part(X)              ((ulong)((X).val % HRTIME_RESOLUTION))
#define my_time(X)                      hrtime_to_time(my_hrtime_coarse())

#if STACK_DIRECTION < 0
#define available_stack_size(CUR,END) (long) ((char*)(CUR) - (char*)(END))
#else
#define available_stack_size(CUR,END) (long) ((char*)(END) - (char*)(CUR))
#endif

#ifdef HAVE_SYS_MMAN_H
#include <sys/mman.h>

#ifndef MAP_NOSYNC
#define MAP_NOSYNC      0
#endif
#ifndef MAP_NORESERVE
#define MAP_NORESERVE 0         
#endif

#ifdef HAVE_MMAP64
#define my_mmap(a,b,c,d,e,f)    mmap64(a,b,c,d,e,f)
#else
#define my_mmap(a,b,c,d,e,f)    mmap(a,b,c,d,e,f)
#endif
#define my_munmap(a,b)          munmap((a),(b))

#else

#define PROT_READ        1
#define PROT_WRITE       2
#define MAP_NORESERVE    0
#define MAP_SHARED       0x0001
#define MAP_PRIVATE      0x0002
#define MAP_NOSYNC       0x0800
#define MAP_FAILED       ((void *)-1)
#define MS_SYNC          0x0000

#define HAVE_MMAP
void *my_mmap(void *, size_t, int, int, int, my_off_t);
int my_munmap(void *, size_t);
#endif


#ifdef HAVE_GETPAGESIZE
#define my_getpagesize()        getpagesize()
#else
int my_getpagesize(void);
#endif

int my_msync(int, void *, size_t, int);

#define MY_UUID_SIZE 16
#define MY_UUID_STRING_LENGTH (8+1+4+1+4+1+4+1+12)

void my_uuid_init(ulong seed1, ulong seed2);
void my_uuid(uchar *guid);
void my_uuid2str(const uchar *guid, char *s);
void my_uuid_end(void);

const char *my_dlerror(const char *dlpath);


extern void my_charset_loader_init_mysys(MY_CHARSET_LOADER *loader);
extern uint get_charset_number(const char *cs_name, uint cs_flags);
extern uint get_collation_number(const char *name);
extern const char *get_charset_name(uint cs_number);

extern CHARSET_INFO *get_charset(uint cs_number, myf flags);
extern CHARSET_INFO *get_charset_by_name(const char *cs_name, myf flags);
extern CHARSET_INFO *my_collation_get_by_name(MY_CHARSET_LOADER *loader,
                                              const char *name, myf flags);
extern CHARSET_INFO *get_charset_by_csname(const char *cs_name,
					   uint cs_flags, myf my_flags);
extern CHARSET_INFO *my_charset_get_by_name(MY_CHARSET_LOADER *loader,
                                            const char *name,
                                            uint cs_flags, myf my_flags);
extern my_bool resolve_charset(const char *cs_name,
                               CHARSET_INFO *default_cs,
                               CHARSET_INFO **cs);
extern my_bool resolve_collation(const char *cl_name,
                                 CHARSET_INFO *default_cl,
                                 CHARSET_INFO **cl);
extern void free_charsets(void);
extern char *get_charsets_dir(char *buf);
extern my_bool my_charset_same(CHARSET_INFO *cs1, CHARSET_INFO *cs2);
extern my_bool init_compiled_charsets(myf flags);
extern void add_compiled_collation(struct charset_info_st *cs);
extern size_t escape_string_for_mysql(CHARSET_INFO *charset_info,
                                      char *to, size_t to_length,
                                      const char *from, size_t length);
extern char *get_tty_password(const char *opt_message);
#ifdef __WIN__
#define BACKSLASH_MBTAIL

extern CHARSET_INFO *fs_character_set(void);
#endif
extern size_t escape_quotes_for_mysql(CHARSET_INFO *charset_info,
                                      char *to, size_t to_length,
                                      const char *from, size_t length);

extern void thd_increment_bytes_sent(void *thd, size_t length);
extern void thd_increment_bytes_received(void *thd, size_t length);
extern void thd_increment_net_big_packet_count(void *thd, size_t length);

#ifdef __WIN__
extern my_bool have_tcpip;		



int my_security_attr_create(SECURITY_ATTRIBUTES **psa, const char **perror,
                            DWORD owner_rights, DWORD everybody_rights);

void my_security_attr_free(SECURITY_ATTRIBUTES *sa);


char* my_cgets(char *string, size_t clen, size_t* plen);

#endif

#include <mysql/psi/psi.h>

#ifdef HAVE_PSI_INTERFACE
extern MYSQL_PLUGIN_IMPORT struct PSI_bootstrap *PSI_hook;
extern void set_psi_server(PSI *psi);
void my_init_mysys_psi_keys(void);
#endif

struct st_mysql_file;
extern struct st_mysql_file *mysql_stdin;
C_MODE_END
#endif 
