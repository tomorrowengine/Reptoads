#ifndef SQL_HANDLER_INCLUDED
#define SQL_HANDLER_INCLUDED


#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "sql_class.h"                 
#include "my_base.h"                   
#include "sql_list.h"                  



class SQL_HANDLER {
public:
  TABLE *table;
  List<Item> fields;                            
  THD *thd;
  LEX_CSTRING handler_name;
  LEX_CSTRING db;
  LEX_CSTRING table_name;
  MEM_ROOT mem_root;
  MYSQL_LOCK *lock;
  MDL_request mdl_request;

  key_part_map keypart_map;
  int keyno;                                    
  uint key_len;
  enum enum_ha_read_modes mode;

  
  SQL_HANDLER *next;

  Query_arena arena;
  char *base_data;
  SQL_HANDLER(THD *thd_arg) :
    thd(thd_arg), arena(&mem_root, Query_arena::STMT_INITIALIZED)
  { init(); clear_alloc_root(&mem_root); base_data= 0; }
  void init()
  {
    keyno= -1;
    table= 0;
    lock= 0;
    mdl_request.ticket= 0;
  }
  void reset();

  ~SQL_HANDLER();
};

class THD;
struct TABLE_LIST;

bool mysql_ha_open(THD *thd, TABLE_LIST *tables, SQL_HANDLER *reopen);
bool mysql_ha_close(THD *thd, TABLE_LIST *tables);
bool mysql_ha_read(THD *, TABLE_LIST *,enum enum_ha_read_modes, const char *,
                   List<Item> *,enum ha_rkey_function,Item *,ha_rows,ha_rows);
void mysql_ha_flush(THD *thd);
void mysql_ha_flush_tables(THD *thd, TABLE_LIST *all_tables);
void mysql_ha_rm_tables(THD *thd, TABLE_LIST *tables);
void mysql_ha_cleanup(THD *thd);
void mysql_ha_set_explicit_lock_duration(THD *thd);
void mysql_ha_rm_temporary_tables(THD *thd);

SQL_HANDLER *mysql_ha_read_prepare(THD *thd, TABLE_LIST *tables,
                                   enum enum_ha_read_modes mode,
                                   const char *keyname,
                                   List<Item> *key_expr, enum ha_rkey_function ha_rkey_mode,
                                   Item *cond);
#endif
