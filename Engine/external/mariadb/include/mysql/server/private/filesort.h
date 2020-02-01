

#ifndef FILESORT_INCLUDED
#define FILESORT_INCLUDED

#include "my_base.h"                            
#include "sql_alloc.h"
#include "filesort_utils.h"

class SQL_SELECT;
class THD;
struct TABLE;
class Filesort_tracker;
struct SORT_FIELD;
typedef struct st_order ORDER;
class JOIN;
 


class Filesort: public Sql_alloc
{
public:
  
  ORDER *order;
  
  ha_rows limit;
  
  SORT_FIELD *sortorder;
  
  SQL_SELECT *select;
  
  bool own_select;
  
  bool using_pq;
  
  
  bool sort_positions;

  Filesort_tracker *tracker;

  Filesort(ORDER *order_arg, ha_rows limit_arg, bool sort_positions_arg,
           SQL_SELECT *select_arg):
    order(order_arg),
    limit(limit_arg),
    sortorder(NULL),
    select(select_arg),
    own_select(false), 
    using_pq(false),
    sort_positions(sort_positions_arg)
  {
    DBUG_ASSERT(order);
  };

  ~Filesort() { cleanup(); }
  
  uint make_sortorder(THD *thd, JOIN *join, table_map first_table_bit);

private:
  void cleanup();
};


class SORT_INFO
{
  
  Filesort_buffer filesort_buffer;

public:
  SORT_INFO()
    :addon_field(0), record_pointers(0)
  {
    buffpek.str= 0;
    my_b_clear(&io_cache);
  }

  ~SORT_INFO();

  void free_data()
  {
    close_cached_file(&io_cache);
    my_free(record_pointers);
    my_free(buffpek.str);
    my_free(addon_field);
  }

  void reset()
  {
    free_data();
    record_pointers= 0;
    buffpek.str= 0;
    addon_field= 0;
  }


  IO_CACHE  io_cache;           
  LEX_STRING buffpek;           
  LEX_STRING addon_buf;         
  struct st_sort_addon_field *addon_field;     
  
  void    (*unpack)(struct st_sort_addon_field *, uchar *, uchar *);
  uchar     *record_pointers;    
  
  ha_rows   return_rows;
  ha_rows   examined_rows;	
  ha_rows   found_rows;         

  
  void sort_buffer(Sort_param *param, uint count)
  { filesort_buffer.sort_buffer(param, count); }

  
  uchar *get_record_buffer(uint idx)
  { return filesort_buffer.get_record_buffer(idx); }

  uchar **get_sort_keys()
  { return filesort_buffer.get_sort_keys(); }

  uchar **alloc_sort_buffer(uint num_records, uint record_length)
  { return filesort_buffer.alloc_sort_buffer(num_records, record_length); }

  void free_sort_buffer()
  { filesort_buffer.free_sort_buffer(); }

  void init_record_pointers()
  { filesort_buffer.init_record_pointers(); }

  size_t sort_buffer_size() const
  { return filesort_buffer.sort_buffer_size(); }

  friend SORT_INFO *filesort(THD *thd, TABLE *table, Filesort *filesort,
                             Filesort_tracker* tracker, JOIN *join,
                             table_map first_table_bit);
};

SORT_INFO *filesort(THD *thd, TABLE *table, Filesort *filesort,
                    Filesort_tracker* tracker, JOIN *join=NULL,
                    table_map first_table_bit=0);

void change_double_for_sort(double nr,uchar *to);

#endif 
