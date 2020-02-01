

#ifndef SQL_INSERT_INCLUDED
#define SQL_INSERT_INCLUDED

#include "sql_class.h"                          
#include "sql_list.h"


typedef List<Item> List_item;
typedef struct st_copy_info COPY_INFO;

bool mysql_prepare_insert(THD *thd, TABLE_LIST *table_list, TABLE *table,
                          List<Item> &fields, List_item *values,
                          List<Item> &update_fields,
                          List<Item> &update_values, enum_duplicates duplic,
                          COND **where, bool select_insert);
bool mysql_insert(THD *thd,TABLE_LIST *table,List<Item> &fields,
                  List<List_item> &values, List<Item> &update_fields,
                  List<Item> &update_values, enum_duplicates flag,
                  bool ignore);
void upgrade_lock_type_for_insert(THD *thd, thr_lock_type *lock_type,
                                  enum_duplicates duplic,
                                  bool is_multi_insert);
int check_that_all_fields_are_given_values(THD *thd, TABLE *entry,
                                           TABLE_LIST *table_list);
int vers_insert_history_row(TABLE *table);
int write_record(THD *thd, TABLE *table, COPY_INFO *info);
void kill_delayed_threads(void);

#ifdef EMBEDDED_LIBRARY
inline void kill_delayed_threads(void) {}
#endif

#endif 
