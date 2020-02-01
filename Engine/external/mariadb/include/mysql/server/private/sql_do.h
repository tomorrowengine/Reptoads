

#ifndef SQL_DO_INCLUDED
#define SQL_DO_INCLUDED

#include "sql_list.h"                           

class THD;
class Item;

bool mysql_do(THD *thd, List<Item> &values);

#endif 
