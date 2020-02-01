

#ifndef SQL_RENAME_INCLUDED
#define SQL_RENAME_INCLUDED

class THD;
struct TABLE_LIST;

bool mysql_rename_tables(THD *thd, TABLE_LIST *table_list, bool silent);

#endif 
