#ifndef _EVENT_DATA_OBJECTS_H_
#define _EVENT_DATA_OBJECTS_H_




#include "event_parse_data.h"
#include "thr_lock.h"                           

class Field;
class THD;
class Time_zone;
struct TABLE;

class Event_queue_element_for_exec
{
public:
  Event_queue_element_for_exec(){};
  ~Event_queue_element_for_exec();

  bool
  init(const LEX_CSTRING *dbname, const LEX_CSTRING *name);

  LEX_CSTRING dbname;
  LEX_CSTRING name;
  bool dropped;
  THD *thd;

private:
  
  Event_queue_element_for_exec(const Event_queue_element_for_exec &);
  void operator=(Event_queue_element_for_exec &);
};


class Event_basic
{
protected:
  MEM_ROOT mem_root;

public:

  LEX_CSTRING dbname;
  LEX_CSTRING name;
  LEX_CSTRING definer;

  Time_zone *time_zone;

  Event_basic();
  virtual ~Event_basic();

  virtual bool
  load_from_row(THD *thd, TABLE *table) = 0;

protected:
  bool
  load_string_fields(Field **fields, ...);

  bool
  load_time_zone(THD *thd, const LEX_CSTRING *tz_name);
};



class Event_queue_element : public Event_basic
{
public:
  int on_completion;
  int status;
  uint32 originator;

  my_time_t last_executed;
  my_time_t execute_at;
  my_time_t starts;
  my_time_t ends;
  bool starts_null;
  bool ends_null;
  bool execute_at_null;

  longlong expression;
  interval_type interval;

  bool dropped;

  uint execution_count;

  Event_queue_element();
  virtual ~Event_queue_element();

  virtual bool
  load_from_row(THD *thd, TABLE *table);

  bool
  compute_next_execution_time();

  void
  mark_last_executed(THD *thd);
};


class Event_timed : public Event_queue_element
{
  Event_timed(const Event_timed &);	
  void operator=(Event_timed &);

public:
  LEX_CSTRING body;

  LEX_CSTRING definer_user;
  LEX_CSTRING definer_host;

  LEX_CSTRING comment;

  ulonglong created;
  ulonglong modified;

  sql_mode_t sql_mode;

  class Stored_program_creation_ctx *creation_ctx;
  LEX_CSTRING body_utf8;

  Event_timed();
  virtual ~Event_timed();

  void
  init();

  virtual bool
  load_from_row(THD *thd, TABLE *table);

  int
  get_create_event(THD *thd, String *buf);
};


class Event_job_data : public Event_basic
{
public:
  LEX_CSTRING body;
  LEX_CSTRING definer_user;
  LEX_CSTRING definer_host;

  sql_mode_t sql_mode;

  class Stored_program_creation_ctx *creation_ctx;

  Event_job_data();

  virtual bool
  load_from_row(THD *thd, TABLE *table);

  bool
  execute(THD *thd, bool drop);
private:
  bool
  construct_sp_sql(THD *thd, String *sp_sql);
  bool
  construct_drop_event_sql(THD *thd, String *sp_sql);

  Event_job_data(const Event_job_data &);       
  void operator=(Event_job_data &);
};



bool
event_basic_db_equal(const LEX_CSTRING *db, Event_basic *et);


bool
event_basic_identifier_equal(const LEX_CSTRING *db, const LEX_CSTRING *name,
                             Event_basic *b);



#endif 
