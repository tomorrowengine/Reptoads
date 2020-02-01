#ifndef PARTITION_ELEMENT_INCLUDED
#define PARTITION_ELEMENT_INCLUDED



#include "my_base.h"                            
#include "handler.h"                            


enum partition_type {
  NOT_A_PARTITION= 0,
  RANGE_PARTITION,
  HASH_PARTITION,
  LIST_PARTITION,
  VERSIONING_PARTITION
};

enum partition_state {
  PART_NORMAL= 0,
  PART_IS_DROPPED= 1,
  PART_TO_BE_DROPPED= 2,
  PART_TO_BE_ADDED= 3,
  PART_TO_BE_REORGED= 4,
  PART_REORGED_DROPPED= 5,
  PART_CHANGED= 6,
  PART_IS_CHANGED= 7,
  PART_IS_ADDED= 8,
  PART_ADMIN= 9
};



typedef struct p_column_list_val
{
  void* column_value;
  Item* item_expression;
  partition_info *part_info;
  uint partition_id;
  bool max_value; 
  bool null_value;
  char fixed;
} part_column_list_val;




typedef struct p_elem_val
{
  longlong value;
  uint added_items;
  bool null_value;
  bool unsigned_flag;
  part_column_list_val *col_val_array;
} part_elem_value;

struct st_ddl_log_memory_entry;

enum stat_trx_field
{
  STAT_TRX_END= 0
};

class partition_element :public Sql_alloc
{
public:
  enum elem_type
  {
    CONVENTIONAL= 0,
    CURRENT,
    HISTORY
  };

  List<partition_element> subpartitions;
  List<part_elem_value> list_val_list;
  ha_rows part_max_rows;
  ha_rows part_min_rows;
  longlong range_value;
  const char *partition_name;
  const char *tablespace_name;
  struct st_ddl_log_memory_entry *log_entry;
  const char* part_comment;
  const char* data_file_name;
  const char* index_file_name;
  handlerton *engine_type;
  LEX_CSTRING connect_string;
  enum partition_state part_state;
  uint16 nodegroup_id;
  bool has_null_value;
  bool signed_flag;                          
  bool max_value;                            
  uint32 id;
  bool empty;

  
  
  elem_type type()
  {
    return (elem_type)(int(signed_flag) << 1 | int(max_value));
  }

  void type(elem_type val)
  {
    max_value= (bool)(val & 1);
    signed_flag= (bool)(val & 2);
  }

  partition_element()
  : part_max_rows(0), part_min_rows(0), range_value(0),
    partition_name(NULL), tablespace_name(NULL),
    log_entry(NULL), part_comment(NULL),
    data_file_name(NULL), index_file_name(NULL),
    engine_type(NULL), connect_string(null_clex_str), part_state(PART_NORMAL),
    nodegroup_id(UNDEF_NODEGROUP), has_null_value(FALSE),
    signed_flag(FALSE), max_value(FALSE),
    id(UINT_MAX32),
    empty(true)
  {}
  partition_element(partition_element *part_elem)
  : part_max_rows(part_elem->part_max_rows),
    part_min_rows(part_elem->part_min_rows),
    range_value(0), partition_name(NULL),
    tablespace_name(part_elem->tablespace_name),
    part_comment(part_elem->part_comment),
    data_file_name(part_elem->data_file_name),
    index_file_name(part_elem->index_file_name),
    engine_type(part_elem->engine_type),
    connect_string(null_clex_str),
    part_state(part_elem->part_state),
    nodegroup_id(part_elem->nodegroup_id),
    has_null_value(FALSE),
    id(part_elem->id),
    empty(part_elem->empty)
  {}
  ~partition_element() {}

  part_column_list_val& get_col_val(uint idx)
  {
    DBUG_ASSERT(type() == CONVENTIONAL || list_val_list.elements == 1);
    part_elem_value *ev= list_val_list.head();
    DBUG_ASSERT(ev);
    DBUG_ASSERT(ev->col_val_array);
    return ev->col_val_array[idx];
  }
};

#endif 
