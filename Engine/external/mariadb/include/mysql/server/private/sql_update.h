

#ifndef SQL_UPDATE_INCLUDED
#define SQL_UPDATE_INCLUDED

#include "sql_class.h"                          

class Item;
struct TABLE_LIST;
class THD;

typedef class st_select_lex SELECT_LEX;
typedef class st_select_lex_unit SELECT_LEX_UNIT;

bool mysql_prepare_update(THD *thd, TABLE_LIST *table_list,
                          Item **conds, uint order_num, ORDER *order);
bool check_unique_table(THD *thd, TABLE_LIST *table_list);
int mysql_update(THD *thd,TABLE_LIST *tables,List<Item> &fields,
		 List<Item> &values,COND *conds,
		 uint order_num, ORDER *order, ha_rows limit,
                 bool ignore, ha_rows *found_return, ha_rows *updated_return);
bool mysql_multi_update(THD *thd, TABLE_LIST *table_list,
                        List<Item> *fields, List<Item> *values,
                        COND *conds, ulonglong options,
                        enum enum_duplicates handle_duplicates, bool ignore,
                        SELECT_LEX_UNIT *unit, SELECT_LEX *select_lex,
                        multi_update **result);
bool records_are_comparable(const TABLE *table);
bool compare_record(const TABLE *table);

#endif 
