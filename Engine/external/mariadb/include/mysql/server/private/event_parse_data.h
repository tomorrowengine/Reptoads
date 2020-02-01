

#ifndef _EVENT_PARSE_DATA_H_
#define _EVENT_PARSE_DATA_H_

#include "sql_alloc.h"

class Item;
class THD;
class sp_name;

#define EVEX_GET_FIELD_FAILED   -2
#define EVEX_BAD_PARAMS         -5
#define EVEX_MICROSECOND_UNSUP  -6
#define EVEX_MAX_INTERVAL_VALUE 1000000000L

class Event_parse_data : public Sql_alloc
{
public:
  
  enum enum_status
  {
    ENABLED = 1,
    DISABLED,
    SLAVESIDE_DISABLED  
  };

  enum enum_on_completion
  {
    
    ON_COMPLETION_DEFAULT = 0,
    ON_COMPLETION_DROP,
    ON_COMPLETION_PRESERVE
  };

  int on_completion;
  int status;
  bool status_changed;
  uint32 originator;
  
  bool do_not_create;

  bool body_changed;

  LEX_CSTRING dbname;
  LEX_CSTRING name;
  LEX_CSTRING definer;
  LEX_CSTRING comment;

  Item* item_starts;
  Item* item_ends;
  Item* item_execute_at;

  my_time_t starts;
  my_time_t ends;
  my_time_t execute_at;
  bool starts_null;
  bool ends_null;
  bool execute_at_null;

  sp_name *identifier;
  Item* item_expression;
  longlong expression;
  interval_type interval;

  static Event_parse_data *
  new_instance(THD *thd);

  bool
  check_parse_data(THD *thd);

  bool
  check_dates(THD *thd, int previous_on_completion);

private:

  void
  init_definer(THD *thd);

  void
  init_name(THD *thd, sp_name *spn);

  int
  init_execute_at(THD *thd);

  int
  init_interval(THD *thd);

  int
  init_starts(THD *thd);

  int
  init_ends(THD *thd);

  Event_parse_data();
  ~Event_parse_data();

  void
  report_bad_value(const char *item_name, Item *bad_item);

  void
  check_if_in_the_past(THD *thd, my_time_t ltime_utc);

  Event_parse_data(const Event_parse_data &);	
  void check_originator_id(THD *thd);
  void operator=(Event_parse_data &);
};
#endif
