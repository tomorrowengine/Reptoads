

#ifndef _sql_cursor_h_
#define _sql_cursor_h_

#ifdef USE_PRAGMA_INTERFACE
#pragma interface                              
#endif

#include "sql_class.h"                          

class JOIN;





class Server_side_cursor: protected Query_arena, public Sql_alloc
{
protected:
  
  select_result *result;
public:
  Server_side_cursor(MEM_ROOT *mem_root_arg, select_result *result_arg)
    :Query_arena(mem_root_arg, STMT_INITIALIZED), result(result_arg)
  {}

  virtual bool is_open() const= 0;

  virtual int open(JOIN *top_level_join)= 0;
  virtual void fetch(ulong num_rows)= 0;
  virtual void close()= 0;
  virtual bool export_structure(THD *thd, Row_definition_list *defs)
  {
    DBUG_ASSERT(0);
    return true;
  }
  virtual ~Server_side_cursor();

  static void operator delete(void *ptr, size_t size);
  static void operator delete(void *, MEM_ROOT *){}
};


int mysql_open_cursor(THD *thd, select_result *result,
                      Server_side_cursor **res);

#endif 
