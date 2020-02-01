#ifndef _EVENT_H_
#define _EVENT_H_




#ifdef HAVE_PSI_INTERFACE
extern PSI_mutex_key key_event_scheduler_LOCK_scheduler_state;
extern PSI_cond_key key_event_scheduler_COND_state;
extern PSI_thread_key key_thread_event_scheduler, key_thread_event_worker;
#endif 


extern PSI_stage_info stage_waiting_on_empty_queue;
extern PSI_stage_info stage_waiting_for_next_activation;
extern PSI_stage_info stage_waiting_for_scheduler_to_stop;

#include "sql_string.h"                         
#include "my_time.h"                            

class Event_db_repository;
class Event_parse_data;
class Event_queue;
class Event_scheduler;
struct TABLE_LIST;
class THD;
typedef class Item COND;

int
sortcmp_lex_string(const LEX_CSTRING *s, const LEX_CSTRING *t,
                   const CHARSET_INFO *cs);



class Events
{
public:
  
  enum enum_opt_event_scheduler { EVENTS_OFF, EVENTS_ON, EVENTS_DISABLED,
                                  EVENTS_ORIGINAL };
  
  static ulong opt_event_scheduler, startup_state;
  static ulong inited;
  static bool check_if_system_tables_error();
  static bool start(int *err_no);
  static bool stop();

public:
  
  static Event_db_repository *
  get_db_repository() { return db_repository; }

  static bool init(THD *thd, bool opt_noacl);

  static void
  deinit();

  static void
  init_mutexes();

  static void
  destroy_mutexes();

  static bool
  create_event(THD *thd, Event_parse_data *parse_data);

  static bool
  update_event(THD *thd, Event_parse_data *parse_data,
               LEX_CSTRING *new_dbname, LEX_CSTRING *new_name);

  static bool
  drop_event(THD *thd, const LEX_CSTRING *dbname, const LEX_CSTRING *name,
             bool if_exists);

  static void
  drop_schema_events(THD *thd, const char *db);

  static bool
  show_create_event(THD *thd, const LEX_CSTRING *dbname,
                    const LEX_CSTRING *name);

  
  static int
  reconstruct_interval_expression(String *buf, interval_type interval,
                                  longlong expression);

  static int
  fill_schema_events(THD *thd, TABLE_LIST *tables, COND * );

  static void
  dump_internal_status();

  static void set_original_state(ulong startup_state_org)
  {
    startup_state= startup_state_org;
  }

private:

  static bool
  load_events_from_db(THD *thd);

private:
  static Event_queue         *event_queue;
  static Event_scheduler     *scheduler;
  static Event_db_repository *db_repository;

private:
  
  Events(const Events &);
  void operator=(Events &);
};



#endif 
