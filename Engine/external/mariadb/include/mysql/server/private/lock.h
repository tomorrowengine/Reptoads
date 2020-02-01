

#ifndef LOCK_INCLUDED
#define LOCK_INCLUDED

#include "thr_lock.h"                           
#include "mdl.h"


struct TABLE;
struct TABLE_LIST;
class THD;
typedef struct st_mysql_lock MYSQL_LOCK;


MYSQL_LOCK *mysql_lock_tables(THD *thd, TABLE **table, uint count, uint flags);
bool mysql_lock_tables(THD *thd, MYSQL_LOCK *sql_lock, uint flags);
void mysql_unlock_tables(THD *thd, MYSQL_LOCK *sql_lock, bool free_lock);
void mysql_unlock_tables(THD *thd, MYSQL_LOCK *sql_lock);
void mysql_unlock_read_tables(THD *thd, MYSQL_LOCK *sql_lock);
void mysql_unlock_some_tables(THD *thd, TABLE **table,uint count, uint flag);
void mysql_lock_remove(THD *thd, MYSQL_LOCK *locked,TABLE *table);
bool mysql_lock_abort_for_thread(THD *thd, TABLE *table);
MYSQL_LOCK *mysql_lock_merge(MYSQL_LOCK *a,MYSQL_LOCK *b);

bool lock_schema_name(THD *thd, const char *db);

bool lock_object_name(THD *thd, MDL_key::enum_mdl_namespace mdl_type,
                      const char *db, const char *name);


#define GET_LOCK_UNLOCK         0
#define GET_LOCK_STORE_LOCKS    1
#define GET_LOCK_ACTION_MASK    1
#define GET_LOCK_ON_THD         (1 << 1)
#define GET_LOCK_SKIP_SEQUENCES (1 << 2)

MYSQL_LOCK *get_lock_data(THD *thd, TABLE **table_ptr, uint count, uint flags);
void reset_lock_data(MYSQL_LOCK *sql_lock, bool unlock);

#endif 
