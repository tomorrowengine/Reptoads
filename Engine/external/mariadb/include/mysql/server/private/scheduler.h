#ifndef SCHEDULER_INCLUDED
#define SCHEDULER_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface
#endif

class THD;



struct scheduler_functions
{
  uint max_threads, *connection_count;
  ulong *max_connections;
  bool (*init)(void);
  bool (*init_new_connection_thread)(void);
  void (*add_connection)(CONNECT *connect);
  void (*thd_wait_begin)(THD *thd, int wait_type);
  void (*thd_wait_end)(THD *thd);
  void (*post_kill_notification)(THD *thd);
  bool (*end_thread)(THD *thd, bool cache_thread);
  void (*end)(void);
};



enum scheduler_types
{
  
  SCHEDULER_ONE_THREAD_PER_CONNECTION=0,
  SCHEDULER_NO_THREADS,
  SCHEDULER_TYPES_COUNT
};

void one_thread_per_connection_scheduler(scheduler_functions *func,
    ulong *arg_max_connections, uint *arg_connection_count);
void one_thread_scheduler(scheduler_functions *func);

extern void scheduler_init();
extern void post_kill_notification(THD *);

struct thd_scheduler
{
public:
  
  PSI_thread *m_psi;
  void *data;                  
};

#ifdef HAVE_POOL_OF_THREADS
void pool_of_threads_scheduler(scheduler_functions* func,
   ulong *arg_max_connections,
   uint *arg_connection_count);
#else
#define pool_of_threads_scheduler(A,B,C) \
  one_thread_per_connection_scheduler(A, B, C)
#endif 

#endif 
