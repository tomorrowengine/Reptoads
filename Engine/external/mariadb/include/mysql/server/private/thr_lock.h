



#ifndef _thr_lock_h
#define _thr_lock_h
#ifdef	__cplusplus
extern "C" {
#endif
#include <my_pthread.h>
#include <my_list.h>

struct st_thr_lock;
extern ulong locks_immediate,locks_waited ;


enum thr_lock_type { TL_IGNORE=-1,
		     TL_UNLOCK,			
                     
                     TL_READ_DEFAULT,
		     TL_READ,			
		     TL_READ_WITH_SHARED_LOCKS,
		     
		     TL_READ_HIGH_PRIORITY,
		     
		     TL_READ_NO_INSERT,
		     
		     TL_WRITE_ALLOW_WRITE,
		     
		     TL_WRITE_CONCURRENT_INSERT,
		     
		     TL_WRITE_DELAYED,
                     
                     TL_WRITE_DEFAULT,
		     
		     TL_WRITE_LOW_PRIORITY,
		     
		     TL_WRITE,
		     
		     TL_WRITE_ONLY};

enum enum_thr_lock_result { THR_LOCK_SUCCESS= 0, THR_LOCK_ABORTED= 1,
                            THR_LOCK_WAIT_TIMEOUT= 2, THR_LOCK_DEADLOCK= 3 };



#define THR_LOCK_LATE_PRIV  1U	
#define THR_LOCK_MERGE_PRIV 2U	

#define THR_UNLOCK_UPDATE_STATUS 1U

extern ulong max_write_lock_count;
extern my_bool thr_lock_inited;
extern enum thr_lock_type thr_upgraded_concurrent_insert_lock;



typedef struct st_thr_lock_info
{
  pthread_t thread;
  my_thread_id thread_id;
  void *mysql_thd;        
} THR_LOCK_INFO;


typedef struct st_thr_lock_data {
  THR_LOCK_INFO *owner;
  struct st_thr_lock_data *next,**prev;
  struct st_thr_lock *lock;
  mysql_cond_t *cond;
  void *status_param;			
  void *debug_print_param;              
  struct PSI_table *m_psi;
  enum thr_lock_type type;
  enum thr_lock_type org_type;		
  uint priority;
} THR_LOCK_DATA;

struct st_lock_list {
  THR_LOCK_DATA *data,**last;
};

typedef struct st_thr_lock {
  LIST list;
  mysql_mutex_t mutex;
  struct st_lock_list read_wait;
  struct st_lock_list read;
  struct st_lock_list write_wait;
  struct st_lock_list write;
  
  ulong write_lock_count;
  uint read_no_write_count;
  void (*get_status)(void*, my_bool);	
  void (*copy_status)(void*,void*);
  void (*update_status)(void*);		
  void (*restore_status)(void*);        
  my_bool (*start_trans)(void*);	
  my_bool (*check_status)(void *);
  void   (*fix_status)(void *, void *);
  const char *name;                     
  my_bool allow_multiple_concurrent_insert;
} THR_LOCK;


extern LIST *thr_lock_thread_list;
extern mysql_mutex_t THR_LOCK_lock;
struct st_my_thread_var;

my_bool init_thr_lock(void);		
void thr_lock_info_init(THR_LOCK_INFO *info, struct st_my_thread_var *tmp);
void thr_lock_init(THR_LOCK *lock);
void thr_lock_delete(THR_LOCK *lock);
void thr_lock_data_init(THR_LOCK *lock,THR_LOCK_DATA *data,
			void *status_param);
void thr_unlock(THR_LOCK_DATA *data, uint unlock_flags);
enum enum_thr_lock_result thr_multi_lock(THR_LOCK_DATA **data,
                                         uint count, THR_LOCK_INFO *owner,
                                         ulong lock_wait_timeout);
void thr_multi_unlock(THR_LOCK_DATA **data,uint count, uint unlock_flags);
void thr_merge_locks(THR_LOCK_DATA **data, uint org_count, uint new_count);
void thr_abort_locks(THR_LOCK *lock, my_bool upgrade_lock);
my_bool thr_abort_locks_for_thread(THR_LOCK *lock, my_thread_id thread);
void thr_print_locks(void);		
my_bool thr_upgrade_write_delay_lock(THR_LOCK_DATA *data,
                                     enum thr_lock_type new_lock_type,
                                     ulong lock_wait_timeout);
void    thr_downgrade_write_lock(THR_LOCK_DATA *data,
                                 enum thr_lock_type new_lock_type);
my_bool thr_reschedule_write_lock(THR_LOCK_DATA *data,
                                  ulong lock_wait_timeout);
void thr_set_lock_wait_callback(void (*before_wait)(void),
                                void (*after_wait)(void));

#ifdef WITH_WSREP
  typedef my_bool (* wsrep_thd_is_brute_force_fun)(void *, my_bool);
  typedef int (* wsrep_abort_thd_fun)(void *, void *, my_bool);
  typedef int (* wsrep_on_fun)(void *);
  void wsrep_thr_lock_init(
    wsrep_thd_is_brute_force_fun bf_fun, wsrep_abort_thd_fun abort_fun,
    my_bool debug, my_bool convert_LOCK_to_trx, wsrep_on_fun on_fun);
#endif

#ifdef	__cplusplus
}
#endif
#endif 
