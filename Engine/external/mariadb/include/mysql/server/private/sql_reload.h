#ifndef SQL_RELOAD_INCLUDED
#define SQL_RELOAD_INCLUDED


class THD;
struct TABLE_LIST;

bool reload_acl_and_cache(THD *thd, unsigned long long options,
                          TABLE_LIST *tables, int *write_to_binlog);

bool flush_tables_with_read_lock(THD *thd, TABLE_LIST *all_tables);

#endif
