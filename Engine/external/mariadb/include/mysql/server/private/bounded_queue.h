

#ifndef BOUNDED_QUEUE_INCLUDED
#define BOUNDED_QUEUE_INCLUDED

#include "my_base.h"
#include <my_sys.h>
#include "queues.h"
#include <string.h>

class Sort_param;


template<typename Element_type, typename Key_type>
class Bounded_queue
{
public:
  Bounded_queue()
  {
    memset(&m_queue, 0, sizeof(m_queue));
  }

  ~Bounded_queue()
  {
    delete_queue(&m_queue);
  }

  
  typedef void (*keymaker_function)(Sort_param *param,
                                    Key_type *to,
                                    Element_type *from);

  
  typedef int (*compare_function)(size_t *n, Key_type **a, Key_type **b);

  
  int init(ha_rows max_elements, bool max_at_top,
           compare_function compare, size_t compare_length,
           keymaker_function keymaker, Sort_param *sort_param,
           Key_type **sort_keys);

  
  void push(Element_type *element);

  
  Key_type **pop()
  {
    
    if (queue_is_full((&m_queue)))
      queue_remove(&m_queue, 0);
    DBUG_ASSERT(m_queue.elements > 0);
    if (m_queue.elements == 0)
      return NULL;
    return reinterpret_cast<Key_type**>(queue_remove(&m_queue, 0));
  }

  
  uint num_elements() const { return m_queue.elements; }

  
  bool is_initialized() const { return m_queue.max_elements > 0; }

private:
  Key_type         **m_sort_keys;
  size_t             m_compare_length;
  keymaker_function  m_keymaker;
  Sort_param        *m_sort_param;
  st_queue           m_queue;
};


template<typename Element_type, typename Key_type>
int Bounded_queue<Element_type, Key_type>::init(ha_rows max_elements,
                                                bool max_at_top,
                                                compare_function compare,
                                                size_t compare_length,
                                                keymaker_function keymaker,
                                                Sort_param *sort_param,
                                                Key_type **sort_keys)
{
  DBUG_ASSERT(sort_keys != NULL);

  m_sort_keys=      sort_keys;
  m_compare_length= compare_length;
  m_keymaker=       keymaker;
  m_sort_param=     sort_param;
  
  if (max_elements >= (UINT_MAX - 1))
    return 1;
  if (compare == NULL)
    compare=
      reinterpret_cast<compare_function>(get_ptr_compare(compare_length));
  
  return init_queue(&m_queue, (uint) max_elements + 1,
                    0, max_at_top,
                    reinterpret_cast<queue_compare>(compare),
                    &m_compare_length, 0, 0);
}


template<typename Element_type, typename Key_type>
void Bounded_queue<Element_type, Key_type>::push(Element_type *element)
{
  DBUG_ASSERT(is_initialized());
  if (queue_is_full((&m_queue)))
  {
    
    Key_type **pq_top= reinterpret_cast<Key_type **>(queue_top(&m_queue));
    (*m_keymaker)(m_sort_param, *pq_top, element);
    queue_replace_top(&m_queue);
  } else {
    
    (*m_keymaker)(m_sort_param, m_sort_keys[m_queue.elements], element);
    queue_insert(&m_queue,
                 reinterpret_cast<uchar*>(&m_sort_keys[m_queue.elements]));
  }
}

#endif  
