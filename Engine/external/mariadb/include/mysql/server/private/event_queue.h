#ifndef _EVENT_QUEUE_H_
#define _EVENT_QUEUE_H_




#ifdef HAVE_PSI_INTERFACE
extern PSI_mutex_key key_LOCK_event_queue;
extern PSI_cond_key key_COND_queue_state;
#endif 

#include "queues.h"                             
#include "sql_string.h"                         
#include "my_time.h"                    

class Event_basic;
class Event_queue_element;
class Event_queue_element_for_exec;

class THD;



class Event_queue
{
public:
  Event_queue();
  ~Event_queue();

  bool
  init_queue(THD *thd);

  

  bool
  create_event(THD *thd, Event_queue_element *new_element,
               bool *created);

  void
  update_event(THD *thd, const LEX_CSTRING *dbname, const LEX_CSTRING *name,
               Event_queue_element *new_element);

  void
  drop_event(THD *thd, const LEX_CSTRING *dbname, const LEX_CSTRING *name);

  void
  drop_schema_events(THD *thd, const LEX_CSTRING *schema);

  void
  recalculate_activation_times(THD *thd);

  bool
  get_top_for_execution_if_time(THD *thd,
                                Event_queue_element_for_exec **event_name);


  void
  dump_internal_status();

private:
  void
  empty_queue();

  void
  deinit_queue();
  
  void
  lock_data(const char *func, uint line);

  void
  unlock_data(const char *func, uint line);

  void
  cond_wait(THD *thd, struct timespec *abstime, const PSI_stage_info *stage,
            const char *src_func, const char *src_file, uint src_line);

  void
  find_n_remove_event(const LEX_CSTRING *db, const LEX_CSTRING *name);


  void
  drop_matching_events(THD *thd, const LEX_CSTRING *pattern,
                       bool (*)(const LEX_CSTRING*, Event_basic *));


  void
  dbug_dump_queue(my_time_t now);

  
  mysql_mutex_t LOCK_event_queue;
  mysql_cond_t COND_queue_state;

  
  QUEUE queue;

  my_time_t next_activation_at;

  uint mutex_last_locked_at_line;
  uint mutex_last_unlocked_at_line;
  uint mutex_last_attempted_lock_at_line;
  const char* mutex_last_locked_in_func;
  const char* mutex_last_unlocked_in_func;
  const char* mutex_last_attempted_lock_in_func;
  bool mutex_queue_data_locked;
  bool mutex_queue_data_attempting_lock;
  bool waiting_on_cond;
};


#endif 
