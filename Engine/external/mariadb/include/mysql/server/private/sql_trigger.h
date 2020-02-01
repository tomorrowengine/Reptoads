#ifndef SQL_TRIGGER_INCLUDED
#define SQL_TRIGGER_INCLUDED



#include <mysqld_error.h>



class Item_trigger_field;
class sp_head;
class sp_name;
class Query_tables_list;
struct TABLE_LIST;
class Query_tables_list;


enum trg_event_type
{
  TRG_EVENT_INSERT= 0,
  TRG_EVENT_UPDATE= 1,
  TRG_EVENT_DELETE= 2,
  TRG_EVENT_MAX
};

static inline uint8 trg2bit(enum trg_event_type trg)
{ return static_cast<uint8>(1 << static_cast<int>(trg)); }

#include "table.h"                              


enum trg_action_time_type
{
  TRG_ACTION_BEFORE= 0, TRG_ACTION_AFTER= 1, TRG_ACTION_MAX
};

enum trigger_order_type
{
  TRG_ORDER_NONE= 0,
  TRG_ORDER_FOLLOWS= 1,
  TRG_ORDER_PRECEDES= 2
};


struct st_trg_execution_order
{
  
  enum trigger_order_type ordering_clause;

  
  LEX_CSTRING anchor_trigger_name;
};


class Table_triggers_list;



class Trigger :public Sql_alloc
{
public:
    Trigger(Table_triggers_list *base_arg, sp_head *code):
    base(base_arg), body(code), next(0), trigger_fields(0), action_order(0)
  {
    bzero((char *)&subject_table_grants, sizeof(subject_table_grants));
  }
  ~Trigger();
  Table_triggers_list *base;
  sp_head *body;
  Trigger *next;                                

  
  Item_trigger_field *trigger_fields;
  LEX_CSTRING name;
  LEX_CSTRING on_table_name;                     
  LEX_CSTRING definition;
  LEX_CSTRING definer;

  
  LEX_CSTRING client_cs_name;
  LEX_CSTRING connection_cl_name;
  LEX_CSTRING db_cl_name;

  GRANT_INFO subject_table_grants;
  sql_mode_t sql_mode;
  
  ulonglong create_time;
  trg_event_type event;
  trg_action_time_type action_time;
  uint action_order;

  bool is_fields_updated_in_trigger(MY_BITMAP *used_fields);
  void get_trigger_info(LEX_CSTRING *stmt, LEX_CSTRING *body,
                        LEX_STRING *definer);
  
  bool change_on_table_name(void* param_arg);
  bool change_table_name(void* param_arg);
  bool add_to_file_list(void* param_arg);
};

typedef bool (Trigger::*Triggers_processor)(void *arg);



class Table_triggers_list: public Sql_alloc
{
  friend class Trigger;

  
  Trigger *triggers[TRG_EVENT_MAX][TRG_ACTION_MAX];
  
  Field             **record0_field;
  uchar              *extra_null_bitmap;
  
  Field             **record1_field;
  
  Field             **new_field;
  Field             **old_field;

  
  TABLE *trigger_table;

  
  bool m_has_unparseable_trigger;

  
  char m_parse_error_message[MYSQL_ERRMSG_SIZE];
  uint count;                                   

public:
  
  List<LEX_CSTRING>  definitions_list;
  
  List<ulonglong> definition_modes_list;
  
  List<ulonglong> create_times;

  List<LEX_CSTRING>  definers_list;

  

  List<LEX_CSTRING> client_cs_names;
  List<LEX_CSTRING> connection_cl_names;
  List<LEX_CSTRING> db_cl_names;

  

  Table_triggers_list(TABLE *table_arg)
    :record0_field(0), extra_null_bitmap(0), record1_field(0),
    trigger_table(table_arg),
    m_has_unparseable_trigger(false), count(0)
  {
    bzero((char *) triggers, sizeof(triggers));
  }
  ~Table_triggers_list();

  bool create_trigger(THD *thd, TABLE_LIST *table, String *stmt_query);
  bool drop_trigger(THD *thd, TABLE_LIST *table, String *stmt_query);
  bool process_triggers(THD *thd, trg_event_type event,
                        trg_action_time_type time_type,
                        bool old_row_is_record1);
  void empty_lists();
  bool create_lists_needed_for_files(MEM_ROOT *root);
  bool save_trigger_file(THD *thd, const LEX_CSTRING *db, const LEX_CSTRING *table_name);

  static bool check_n_load(THD *thd, const LEX_CSTRING *db, const LEX_CSTRING *table_name,
                           TABLE *table, bool names_only);
  static bool drop_all_triggers(THD *thd, const LEX_CSTRING *db,
                                const LEX_CSTRING *table_name);
  static bool change_table_name(THD *thd, const LEX_CSTRING *db,
                                const LEX_CSTRING *old_alias,
                                const LEX_CSTRING *old_table,
                                const LEX_CSTRING *new_db,
                                const LEX_CSTRING *new_table);
  void add_trigger(trg_event_type event_type, 
                   trg_action_time_type action_time,
                   trigger_order_type ordering_clause,
                   LEX_CSTRING *anchor_trigger_name,
                   Trigger *trigger);
  Trigger *get_trigger(trg_event_type event_type, 
                       trg_action_time_type action_time)
  {
    return triggers[event_type][action_time];
  }
  
  Trigger *get_trigger(uint event_type, uint action_time)
  {
    return get_trigger((trg_event_type) event_type,
                       (trg_action_time_type) action_time);
  }

  bool has_triggers(trg_event_type event_type, 
                    trg_action_time_type action_time)
  {
    return get_trigger(event_type,action_time) != 0;
  }
  bool has_delete_triggers()
  {
    return (has_triggers(TRG_EVENT_DELETE,TRG_ACTION_BEFORE) ||
            has_triggers(TRG_EVENT_DELETE,TRG_ACTION_AFTER));
  }

  void mark_fields_used(trg_event_type event);

  void set_parse_error_message(char *error_message);

  friend class Item_trigger_field;

  bool add_tables_and_routines_for_triggers(THD *thd,
                                            Query_tables_list *prelocking_ctx,
                                            TABLE_LIST *table_list);

  Field **nullable_fields() { return record0_field; }
  void reset_extra_null_bitmap()
  {
    size_t null_bytes= (trigger_table->s->stored_fields -
                        trigger_table->s->null_fields + 7)/8;
    bzero(extra_null_bitmap, null_bytes);
  }

  Trigger *find_trigger(const LEX_CSTRING *name, bool remove_from_list);

  Trigger* for_all_triggers(Triggers_processor func, void *arg);

private:
  bool prepare_record_accessors(TABLE *table);
  Trigger *change_table_name_in_trignames(const LEX_CSTRING *old_db_name,
                                          const LEX_CSTRING *new_db_name,
                                          const LEX_CSTRING *new_table_name,
                                          Trigger *trigger);
  bool change_table_name_in_triggers(THD *thd,
                                     const LEX_CSTRING *old_db_name,
                                     const LEX_CSTRING *new_db_name,
                                     const LEX_CSTRING *old_table_name,
                                     const LEX_CSTRING *new_table_name);

  bool check_for_broken_triggers() 
  {
    if (m_has_unparseable_trigger)
    {
      my_message(ER_PARSE_ERROR, m_parse_error_message, MYF(0));
      return true;
    }
    return false;
  }
};

bool add_table_for_trigger(THD *thd,
                           const sp_name *trg_name,
                           bool continue_if_not_exist,
                           TABLE_LIST **table);

void build_trn_path(THD *thd, const sp_name *trg_name, LEX_STRING *trn_path);

bool check_trn_exists(const LEX_CSTRING *trn_path);

bool load_table_name_for_trigger(THD *thd,
                                 const sp_name *trg_name,
                                 const LEX_CSTRING *trn_path,
                                 LEX_CSTRING *tbl_name);
bool mysql_create_or_drop_trigger(THD *thd, TABLE_LIST *tables, bool create);

extern const char * const TRG_EXT;
extern const char * const TRN_EXT;

#endif 
