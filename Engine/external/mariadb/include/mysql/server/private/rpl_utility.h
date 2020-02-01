

#ifndef RPL_UTILITY_H
#define RPL_UTILITY_H

#ifndef __cplusplus
#error "Don't include this C++ header file from a non-C++ file!"
#endif

#include "sql_priv.h"
#include "m_string.h"                           
#ifdef MYSQL_SERVER
#include "table.h"                              
#endif
#include "mysql_com.h"

class Relay_log_info;
class Log_event;
struct rpl_group_info;



class table_def
{
public:
  
  table_def(unsigned char *types, ulong size, uchar *field_metadata,
            int metadata_size, uchar *null_bitmap, uint16 flags);

  ~table_def();

  
  ulong size() const { return m_size; }


  
  enum_field_types binlog_type(ulong index) const
  {
    return static_cast<enum_field_types>(m_type[index]);
  }
  
  enum_field_types type(ulong index) const
  {
    DBUG_ASSERT(index < m_size);
    
    enum_field_types source_type= binlog_type(index);
    uint16 source_metadata= m_field_metadata[index];
    switch (source_type)
    {
    case MYSQL_TYPE_STRING:
    {
      int real_type= source_metadata >> 8;
      if (real_type == MYSQL_TYPE_ENUM || real_type == MYSQL_TYPE_SET)
        source_type= static_cast<enum_field_types>(real_type);
      break;
    }

    
    case MYSQL_TYPE_DATE:
      source_type= MYSQL_TYPE_NEWDATE;
      break;

    default:
      
      break;
    }

    return source_type;
  }


  
  uint16 field_metadata(uint index) const
  {
    DBUG_ASSERT(index < m_size);
    if (m_field_metadata_size)
      return m_field_metadata[index];
    else
      return 0;
  }

  
  my_bool maybe_null(uint index) const
  {
    DBUG_ASSERT(index < m_size);
    return ((m_null_bits[(index / 8)] & 
            (1 << (index % 8))) == (1 << (index %8)));
  }

  
  uint32 calc_field_size(uint col, uchar *master_data) const;

  
#ifndef MYSQL_CLIENT
  bool compatible_with(THD *thd, rpl_group_info *rgi, TABLE *table,
                      TABLE **conv_table_var) const;

  
  TABLE *create_conversion_table(THD *thd, rpl_group_info *rgi,
                                 TABLE *target_table) const;
#endif


private:
  ulong m_size;           
  unsigned char *m_type;  
  uint m_field_metadata_size;
  uint16 *m_field_metadata;
  uchar *m_null_bits;
  uint16 m_flags;         
  uchar *m_memory;
};


#ifndef MYSQL_CLIENT

struct RPL_TABLE_LIST
  : public TABLE_LIST
{
  bool m_tabledef_valid;
  table_def m_tabledef;
  TABLE *m_conv_table;
  bool master_had_triggers;
};



CPP_UNNAMED_NS_START

  
  template <class Obj>
  class auto_afree_ptr
  {
    Obj* m_ptr;
  public:
    auto_afree_ptr(Obj* ptr) : m_ptr(ptr) { }
    ~auto_afree_ptr() { if (m_ptr) my_afree(m_ptr); }
    void assign(Obj* ptr) {
      
      DBUG_ASSERT(m_ptr == NULL);
      m_ptr= ptr;
    }
    Obj* get() { return m_ptr; }
  };

CPP_UNNAMED_NS_END

class Deferred_log_events
{
private:
  DYNAMIC_ARRAY array;
  Log_event *last_added;

public:
  Deferred_log_events(Relay_log_info *rli);
  ~Deferred_log_events();
  
  int add(Log_event *ev);
  bool is_empty();
  bool execute(struct rpl_group_info *rgi);
  void rewind();
  bool is_last(Log_event *ev) { return ev == last_added; };
};

#endif


#define DBUG_PRINT_BITSET(N,FRM,BS)                \
  do {                                             \
    char buf[256];                                 \
    uint i;                                        \
    for (i = 0 ; i < MY_MIN(sizeof(buf) - 1, (BS)->n_bits) ; i++) \
      buf[i] = bitmap_is_set((BS), i) ? '1' : '0'; \
    buf[i] = '\0';                                 \
    DBUG_PRINT((N), ((FRM), buf));                 \
  } while (0)

#endif 
