

#ifndef TABLE_MAPPING_H
#define TABLE_MAPPING_H


#ifndef MYSQL_CLIENT
struct TABLE;
#else
class Table_map_log_event;
typedef Table_map_log_event TABLE;
void free_table_map_log_event(TABLE *table);
#endif






#include "hash.h"                               

class table_mapping {

private:
  MEM_ROOT m_mem_root;

public:

  enum enum_error {
      ERR_NO_ERROR = 0,
      ERR_LIMIT_EXCEEDED,
      ERR_MEMORY_ALLOCATION
  };

  table_mapping();
  ~table_mapping();

  TABLE* get_table(ulonglong table_id);

  int       set_table(ulonglong table_id, TABLE* table);
  int       remove_table(ulonglong table_id);
  void      clear_tables();
  ulong     count() const { return m_table_ids.records; }

private:
  
  struct entry { 
    ulonglong table_id;
    union {
      TABLE *table;
      entry *next;
    };
  };

  entry *find_entry(ulonglong table_id)
  {
    return (entry *) my_hash_search(&m_table_ids,
                                    (uchar*)&table_id,
                                    sizeof(table_id));
  }
  int expand();

  
  entry *m_free;

  
  HASH m_table_ids;
};

#endif
