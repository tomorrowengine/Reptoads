#ifndef _EVENT_SCHEDULER_H_
#define _EVENT_SCHEDULER_H_






class Event_queue;
class Event_job_data;
class Event_db_repository;
class Event_queue_element_for_exec;
class Events;
class THD;

void
pre_init_event_thread(THD* thd);

bool
post_init_event_thread(THD* thd);

void
deinit_event_thread(THD *thd);


class Event_worker_thread
{
public:
  static void
  init(Event_db_repository *db_repository_arg)
  {
    db_repository= db_repository_arg;
  }

  void
  run(THD *thd, Event_queue_element_for_exec *event);

private:
  void
  print_warnings(THD *thd, Event_job_data *et);

  static Event_db_repository *db_repository;
};


class Event_scheduler
{
public:
  Event_scheduler(Event_queue *event_queue_arg);
  ~Event_scheduler();


  

  bool
  start(int *err_no);

  bool
  stop();

  
  bool
  run(THD *thd);


  
  bool
  is_running();

  void
  dump_internal_status();

private:
  uint
  workers_count();

  
  bool
  execute_top(Event_queue_element_for_exec *event_name);

  
  void
  lock_data(const char *func, uint line);

  void
  unlock_data(const char *func, uint line);

  void
  cond_wait(THD *thd, struct timespec *abstime, const PSI_stage_info *stage,
            const char *src_func, const char *src_file, uint src_line);

  mysql_mutex_t LOCK_scheduler_state;

  enum enum_state
  {
    INITIALIZED = 0,
    RUNNING,
    STOPPING
  };

  
  enum enum_state state;

  THD *scheduler_thd;

  mysql_cond_t COND_state;

  Event_queue *queue;

  uint mutex_last_locked_at_line;
  uint mutex_last_unlocked_at_line;
  const char* mutex_last_locked_in_func;
  const char* mutex_last_unlocked_in_func;
  bool mutex_scheduler_data_locked;
  bool waiting_on_cond;

  ulonglong started_events;

private:
  
  Event_scheduler(const Event_scheduler &);
  void operator=(Event_scheduler &);
};



#endif 
