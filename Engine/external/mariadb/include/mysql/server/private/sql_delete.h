

#ifndef SQL_DELETE_INCLUDED
#define SQL_DELETE_INCLUDED

#include "my_base.h"                            

class THD;
struct TABLE_LIST;
class Item;
class select_result;

typedef class Item COND;
template <typename T> class SQL_I_List;

int mysql_prepare_delete(THD *thd, TABLE_LIST *table_list,
                         uint wild_num, List<Item> &field_list, Item **conds,
                         bool *delete_while_scanning);
bool mysql_delete(THD *thd, TABLE_LIST *table_list, COND *conds,
                  SQL_I_List<ORDER> *order, ha_rows rows, 
                  ulonglong options, select_result *result);

#endif 
