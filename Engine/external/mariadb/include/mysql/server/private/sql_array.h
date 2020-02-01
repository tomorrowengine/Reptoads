#ifndef SQL_ARRAY_INCLUDED
#define SQL_ARRAY_INCLUDED



#include <my_sys.h>


template <typename Element_type> class Bounds_checked_array
{
public:
  Bounds_checked_array() : m_array(NULL), m_size(0) {}

  Bounds_checked_array(Element_type *el, size_t size_arg)
    : m_array(el), m_size(size_arg)
  {}

  void reset() { m_array= NULL; m_size= 0; }
 
  void reset(Element_type *array_arg, size_t size_arg)
  {
    m_array= array_arg;
    m_size= size_arg;
  }

  
  void resize(size_t new_size)
  {
    DBUG_ASSERT(new_size <= m_size);
    m_size= new_size;
  }

  Element_type &operator[](size_t n)
  {
    DBUG_ASSERT(n < m_size);
    return m_array[n];
  }

  const Element_type &operator[](size_t n) const
  {
    DBUG_ASSERT(n < m_size);
    return m_array[n];
  }

  size_t element_size() const { return sizeof(Element_type); }
  size_t size() const         { return m_size; }

  bool is_null() const { return m_array == NULL; }

  void pop_front()
  {
    DBUG_ASSERT(m_size > 0);
    m_array+= 1;
    m_size-= 1;
  }

  Element_type *array() const { return m_array; }

  bool operator==(const Bounds_checked_array<Element_type>&rhs) const
  {
    return m_array == rhs.m_array && m_size == rhs.m_size;
  }
  bool operator!=(const Bounds_checked_array<Element_type>&rhs) const
  {
    return m_array != rhs.m_array || m_size != rhs.m_size;
  }

private:
  Element_type *m_array;
  size_t        m_size;
};



template <class Elem> class Dynamic_array
{
  DYNAMIC_ARRAY  array;
public:
  Dynamic_array(uint prealloc=16, uint increment=16)
  {
    init(prealloc, increment);
  }

  Dynamic_array(MEM_ROOT *root, uint prealloc=16, uint increment=16)
  {
    void *init_buffer= alloc_root(root, sizeof(Elem) * prealloc);
    my_init_dynamic_array2(&array, sizeof(Elem), init_buffer, 
                           prealloc, increment, MYF(0));
  }

  void init(uint prealloc=16, uint increment=16)
  {
    init_dynamic_array2(&array, sizeof(Elem), 0, prealloc, increment, MYF(0));
  }

  
  Elem& at(size_t idx)
  {
    DBUG_ASSERT(idx < array.elements);
    return *(((Elem*)array.buffer) + idx);
  }
  
  const Elem& at(size_t idx) const
  {
    return *(((Elem*)array.buffer) + idx);
  }

  
  Elem *front()
  {
    return (Elem*)array.buffer;
  }

  
  const Elem *front() const
  {
    return (const Elem*)array.buffer;
  }

  
  Elem *back()
  {
    return ((Elem*)array.buffer) + array.elements - 1;
  }

  
  const Elem *back() const
  {
    return ((const Elem*)array.buffer) + array.elements - 1;
  }

  
  bool append(const Elem &el)
  {
    return insert_dynamic(&array, &el);
  }

  bool append_val(Elem el)
  {
    return (insert_dynamic(&array, (uchar*)&el));
  }

  bool push(Elem &el)
  {
    return append(el);
  }

  
  Elem& pop()
  {
    return *((Elem*)pop_dynamic(&array));
  }

  void del(size_t idx)
  {
    DBUG_ASSERT(idx <= array.max_element);
    delete_dynamic_element(&array, (uint)idx);
  }

  size_t elements() const
  {
    return array.elements;
  }

  void elements(size_t num_elements)
  {
    DBUG_ASSERT(num_elements <= array.max_element);
    array.elements= (uint)num_elements;
  }

  void clear()
  {
    elements(0);
  }

  void set(uint idx, const Elem &el)
  {
    set_dynamic(&array, &el, idx);
  }

  void freeze()
  {
    freeze_size(&array);
  }

  bool resize(size_t new_size, Elem default_val)
  {
    size_t old_size= elements();
    if (unlikely(allocate_dynamic(&array, (uint)new_size)))
      return true;
    
    if (new_size > old_size)
    {
      set_dynamic(&array, (uchar*)&default_val, (uint)(new_size - 1));
      
    }
    return false;
  }

  ~Dynamic_array()
  {
    delete_dynamic(&array);
  }

  void free_memory()
  {
    delete_dynamic(&array);
  }

  typedef int (*CMP_FUNC)(const Elem *el1, const Elem *el2);

  void sort(CMP_FUNC cmp_func)
  {
    my_qsort(array.buffer, array.elements, sizeof(Elem), (qsort_cmp)cmp_func);
  }

  typedef int (*CMP_FUNC2)(void *, const Elem *el1, const Elem *el2);
  void sort(CMP_FUNC2 cmp_func, void *data)
  {
    my_qsort2(array.buffer, array.elements, sizeof(Elem), (qsort2_cmp)cmp_func, data);
  }
};

typedef Bounds_checked_array<Item*> Ref_ptr_array;

#endif 
