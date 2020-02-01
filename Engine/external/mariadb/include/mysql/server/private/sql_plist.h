#ifndef SQL_PLIST_H
#define SQL_PLIST_H



template <typename T, typename L>
class I_P_List_iterator;
class I_P_List_null_counter;
template <typename T> class I_P_List_no_push_back;




template <typename T, typename B,
          typename C = I_P_List_null_counter,
          typename I = I_P_List_no_push_back<T> >
class I_P_List : public C, public I
{
  T *m_first;

  
public:
  I_P_List() : I(&m_first), m_first(NULL) {};
  

  inline void empty()      { m_first= NULL; C::reset(); I::set_last(&m_first); }
  inline bool is_empty() const { return (m_first == NULL); }
  inline void push_front(T* a)
  {
    *B::next_ptr(a)= m_first;
    if (m_first)
      *B::prev_ptr(m_first)= B::next_ptr(a);
    else
      I::set_last(B::next_ptr(a));
    m_first= a;
    *B::prev_ptr(a)= &m_first;
    C::inc();
  }
  inline void push_back(T *a)
  {
    T **last= I::get_last();
    *B::next_ptr(a)= *last;
    *last= a;
    *B::prev_ptr(a)= last;
    I::set_last(B::next_ptr(a));
    C::inc();
  }
  inline void insert_after(T *pos, T *a)
  {
    if (pos == NULL)
      push_front(a);
    else
    {
      *B::next_ptr(a)= *B::next_ptr(pos);
      *B::prev_ptr(a)= B::next_ptr(pos);
      *B::next_ptr(pos)= a;
      if (*B::next_ptr(a))
      {
        T *old_next= *B::next_ptr(a);
        *B::prev_ptr(old_next)= B::next_ptr(a);
      }
      else
        I::set_last(B::next_ptr(a));
      C::inc();
    }
  }
  inline void remove(T *a)
  {
    T *next= *B::next_ptr(a);
    if (next)
      *B::prev_ptr(next)= *B::prev_ptr(a);
    else
      I::set_last(*B::prev_ptr(a));
    **B::prev_ptr(a)= next;
    C::dec();
  }
  inline T* front() { return m_first; }
  inline const T *front() const { return m_first; }
  inline T* pop_front()
  {
    T *result= front();

    if (result)
      remove(result);

    return result;
  }
  void swap(I_P_List<T, B, C> &rhs)
  {
    swap_variables(T *, m_first, rhs.m_first);
    I::swap(rhs);
    if (m_first)
      *B::prev_ptr(m_first)= &m_first;
    else
      I::set_last(&m_first);
    if (rhs.m_first)
      *B::prev_ptr(rhs.m_first)= &rhs.m_first;
    else
      I::set_last(&rhs.m_first);
    C::swap(rhs);
  }
  typedef B Adapter;
  typedef I_P_List<T, B, C, I> Base;
  typedef I_P_List_iterator<T, Base> Iterator;
  typedef I_P_List_iterator<const T, Base> Const_Iterator;
#ifndef _lint
  friend class I_P_List_iterator<T, Base>;
  friend class I_P_List_iterator<const T, Base>;
#endif
};




template <typename T, typename L>
class I_P_List_iterator
{
  const L *list;
  T *current;
public:
  I_P_List_iterator(const L &a)
    : list(&a), current(a.m_first) {}
  I_P_List_iterator(const L &a, T* current_arg)
    : list(&a), current(current_arg) {}
  inline void init(const L &a)
  {
    list= &a;
    current= a.m_first;
  }
  
  inline T* operator++(int)
  {
    T *result= current;
    if (result)
      current= *L::Adapter::next_ptr(current);
    return result;
  }
  
  inline T* operator++()
  {
    current= *L::Adapter::next_ptr(current);
    return current;
  }
  inline void rewind()
  {
    current= list->m_first;
  }
};




template <typename T, T* T::*next, T** T::*prev>
struct I_P_List_adapter
{
  static inline T **next_ptr(T *el) { return &(el->*next); }
  static inline const T* const* next_ptr(const T *el) { return &(el->*next); }
  static inline T ***prev_ptr(T *el) { return &(el->*prev); }
};




class I_P_List_null_counter
{
protected:
  void reset() {}
  void inc() {}
  void dec() {}
  void swap(I_P_List_null_counter &) {}
};




class I_P_List_counter
{
  uint m_counter;
protected:
  I_P_List_counter() : m_counter (0) {}
  void reset() {m_counter= 0;}
  void inc() {m_counter++;}
  void dec() {m_counter--;}
  void swap(I_P_List_counter &rhs)
  { swap_variables(uint, m_counter, rhs.m_counter); }
public:
  uint elements() const { return m_counter; }
};




template <typename T> class I_P_List_no_push_back
{
protected:
  I_P_List_no_push_back(T **) {}
  void set_last(T **) {}
  
  void swap(I_P_List_no_push_back<T> &) {}
};




template <typename T> class I_P_List_fast_push_back
{
  T **m_last;
protected:
  I_P_List_fast_push_back(T **a) : m_last(a) { };
  void set_last(T **a) { m_last= a; }
  T** get_last() const { return m_last; }
  void swap(I_P_List_fast_push_back<T> &rhs)
  { swap_variables(T**, m_last, rhs.m_last); }
};

#endif
