


#ifndef MEM_ROOT_ARRAY_INCLUDED
#define MEM_ROOT_ARRAY_INCLUDED

#include <my_alloc.h>


template<typename Element_type, bool has_trivial_destructor>
class Mem_root_array
{
public:
  
  typedef Element_type value_type;

  Mem_root_array(MEM_ROOT *root)
    : m_root(root), m_array(NULL), m_size(0), m_capacity(0)
  {
    DBUG_ASSERT(m_root != NULL);
  }

  Mem_root_array(MEM_ROOT *root, size_t n, const value_type &val= value_type())
    : m_root(root), m_array(NULL), m_size(0), m_capacity(0)
  {
    resize(n, val);
  }

  ~Mem_root_array()
  {
    clear();
  }

  Element_type &at(size_t n)
  {
    DBUG_ASSERT(n < size());
    return m_array[n];
  }

  const Element_type &at(size_t n) const
  {
    DBUG_ASSERT(n < size());
    return m_array[n];
  }

  Element_type &operator[](size_t n) { return at(n); }
  const Element_type &operator[](size_t n) const { return at(n); }

  Element_type &back() { return at(size() - 1); }
  const Element_type &back() const { return at(size() - 1); }

  
  Element_type *begin() { return &m_array[0]; }

  
  Element_type *end() { return &m_array[size()]; }

  
  void clear()
  {
    if (!empty())
      chop(0);
  }

  
  void chop(const size_t pos)
  {
    DBUG_ASSERT(pos < m_size);
    if (!has_trivial_destructor)
    {
      for (size_t ix= pos; ix < m_size; ++ix)
      {
        Element_type *p= &m_array[ix];
        p->~Element_type();              
      }
    }
    m_size= pos;
  }

  
  bool reserve(size_t n)
  {
    if (n <= m_capacity)
      return false;

    void *mem= alloc_root(m_root, n * element_size());
    if (!mem)
      return true;
    Element_type *array= static_cast<Element_type*>(mem);

    
    for (size_t ix= 0; ix < m_size; ++ix)
    {
      Element_type *new_p= &array[ix];
      Element_type *old_p= &m_array[ix];
      new (new_p) Element_type(*old_p);         
      if (!has_trivial_destructor)
        old_p->~Element_type();                 
    }

    
    m_array= array;
    m_capacity= n;
    return false;
  }

  
  bool push_back(const Element_type &element)
  {
    const size_t min_capacity= 20;
    const size_t expansion_factor= 2;
    if (0 == m_capacity && reserve(min_capacity))
      return true;
    if (m_size == m_capacity && reserve(m_capacity * expansion_factor))
      return true;
    Element_type *p= &m_array[m_size++];
    new (p) Element_type(element);
    return false;
  }

  
  void pop_back()
  {
    DBUG_ASSERT(!empty());
    if (!has_trivial_destructor)
      back().~Element_type();
    m_size-= 1;
  }

  
  void resize(size_t n, const value_type &val= value_type())
  {
    if (n == m_size)
      return;
    if (n > m_size)
    {
      if (!reserve(n))
      {
        while (n != m_size)
          push_back(val);
      }
      return;
    }
    if (!has_trivial_destructor)
    {
      while (n != m_size)
        pop_back();
    }
    m_size= n;
  }

  size_t capacity()     const { return m_capacity; }
  size_t element_size() const { return sizeof(Element_type); }
  bool   empty()        const { return size() == 0; }
  size_t size()         const { return m_size; }

private:
  MEM_ROOT *const m_root;
  Element_type   *m_array;
  size_t          m_size;
  size_t          m_capacity;

  
  Mem_root_array(const Mem_root_array&);
  Mem_root_array &operator=(const Mem_root_array&);
};


#endif  
