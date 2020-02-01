

#ifndef FILESORT_UTILS_INCLUDED
#define FILESORT_UTILS_INCLUDED

#include "my_base.h"
#include "sql_array.h"

class Sort_param;


double get_merge_many_buffs_cost_fast(ha_rows num_rows,
                                      ha_rows num_keys_per_buffer,
                                      uint    elem_size);



class Filesort_buffer
{
public:
  Filesort_buffer()
    : m_idx_array(), m_start_of_data(NULL), allocated_size(0)
  {}
  
  ~Filesort_buffer()
  {
    my_free(m_idx_array.array());
  }

  bool is_allocated()
  {
    return m_idx_array.array() != 0;
  }
  void reset()
  {
    m_idx_array.reset();
  }

  
  void sort_buffer(const Sort_param *param, uint count);

  
  uchar *get_record_buffer(uint idx)
  {
    m_idx_array[idx]= m_start_of_data + (idx * m_record_length);
    return m_idx_array[idx];
  }

  
  void init_record_pointers()
  {
    for (uint ix= 0; ix < m_idx_array.size(); ++ix)
      (void) get_record_buffer(ix);
  }

  
  size_t sort_buffer_size() const
  {
    return allocated_size;
  }

  
  uchar **alloc_sort_buffer(uint num_records, uint record_length);

  
  void free_sort_buffer();

  
  uchar **get_sort_keys() { return m_idx_array.array(); }

  
  Filesort_buffer &operator=(const Filesort_buffer &rhs)
  {
    m_idx_array= rhs.m_idx_array;
    m_record_length= rhs.m_record_length;
    m_start_of_data= rhs.m_start_of_data;
    allocated_size=  rhs.allocated_size;
    return *this;
  }

private:
  typedef Bounds_checked_array<uchar*> Idx_array;

  Idx_array  m_idx_array;                       
  uint       m_record_length;
  uchar     *m_start_of_data;                   
  size_t    allocated_size;
};

#endif  
