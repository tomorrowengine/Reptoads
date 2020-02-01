#ifndef _EVENT_DB_REPOSITORY_H_
#define _EVENT_DB_REPOSITORY_H_




enum enum_events_table_field
{
  ET_FIELD_DB = 0, 
  ET_FIELD_NAME,
  ET_FIELD_BODY,
  ET_FIELD_DEFINER,
  ET_FIELD_EXECUTE_AT,
  ET_FIELD_INTERVAL_EXPR,
  ET_FIELD_TRANSIENT_INTERVAL,
  ET_FIELD_CREATED,
  ET_FIELD_MODIFIED,
  ET_FIELD_LAST_EXECUTED,
  ET_FIELD_STARTS,
  ET_FIELD_ENDS,
  ET_FIELD_STATUS,
  ET_FIELD_ON_COMPLETION,
  ET_FIELD_SQL_MODE,
  ET_FIELD_COMMENT,
  ET_FIELD_ORIGINATOR,
  ET_FIELD_TIME_ZONE,
  ET_FIELD_CHARACTER_SET_CLIENT,
  ET_FIELD_COLLATION_CONNECTION,
  ET_FIELD_DB_COLLATION,
  ET_FIELD_BODY_UTF8,
  ET_FIELD_COUNT 
};


int
events_table_index_read_for_db(THD *thd, TABLE *schema_table,
                               TABLE *event_table);

int
events_table_scan_all(THD *thd, TABLE *schema_table, TABLE *event_table);


class Event_basic;
class Event_parse_data;

class Event_db_repository
{
public:
  Event_db_repository(){}

  bool
  create_event(THD *thd, Event_parse_data *parse_data,
               bool *event_already_exists);
  bool
  update_event(THD *thd, Event_parse_data *parse_data, LEX_CSTRING *new_dbname,
               LEX_CSTRING *new_name);

  bool
  drop_event(THD *thd, const LEX_CSTRING *db, const LEX_CSTRING *name,
             bool drop_if_exists);

  void
  drop_schema_events(THD *thd, const LEX_CSTRING *schema);

  bool
  find_named_event(const LEX_CSTRING *db, const LEX_CSTRING *name,
                   TABLE *table);

  bool
  load_named_event(THD *thd, const LEX_CSTRING *dbname,
                   const LEX_CSTRING *name,
                   Event_basic *et);

  static bool
  open_event_table(THD *thd, enum thr_lock_type lock_type, TABLE **table);

  bool
  fill_schema_events(THD *thd, TABLE_LIST *tables, const char *db);

  bool
  update_timing_fields_for_event(THD *thd,
                                 const LEX_CSTRING *event_db_name,
                                 const LEX_CSTRING *event_name,
                                 my_time_t last_executed,
                                 ulonglong status);
public:
  static bool
  check_system_tables(THD *thd);
private:
  bool
  index_read_for_db_for_i_s(THD *thd, TABLE *schema_table, TABLE *event_table,
                            const char *db);

  bool
  table_scan_all_for_i_s(THD *thd, TABLE *schema_table, TABLE *event_table);

private:
  
  Event_db_repository(const Event_db_repository &);
  void operator=(Event_db_repository &);
};


#endif 
