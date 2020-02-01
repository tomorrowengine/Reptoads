

#ifndef SQL_LOAD_INCLUDED
#define SQL_LOAD_INCLUDED

#include "sql_list.h"                           

class Item;

#include "sql_class.h"                          

class sql_exchange;

int mysql_load(THD *thd, const sql_exchange *ex, TABLE_LIST *table_list,
	        List<Item> &fields_vars, List<Item> &set_fields,
                List<Item> &set_values_list,
                enum enum_duplicates handle_duplicates, bool ignore,
                bool local_file);


#endif 
