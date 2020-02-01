

#ifndef SQL_TABLESPACE_INCLUDED
#define SQL_TABLESPACE_INCLUDED

class THD;
class st_alter_tablespace;

int mysql_alter_tablespace(THD* thd, st_alter_tablespace *ts_info);

#endif 
