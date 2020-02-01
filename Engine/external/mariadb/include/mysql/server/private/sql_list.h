#ifndef INCLUDES_MYSQL_SQL_LIST_H
#define INCLUDES_MYSQL_SQL_LIST_H


#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "sql_alloc.h"



template <typename T>
class SQL_I_List :public Sql_alloc
{
public:
  uint elements;
  
  T *first;
  
  T **next;

  SQL_I_List() { empty(); }

  SQL_I_List(const SQL_I_List &tmp) : Sql_alloc()
  {
    elements= tmp.elements;
    first= tmp.first;
    next= elements ? tmp.next : &first;
  }

  inline void empty()
  {
    elements= 0;
    first= NULL;
    next= &first;
  }

  inline void link_in_list(T *element, T **next_ptr)
  {
    elements++;
    (*next)= element;
    next= next_ptr;
    *next= NULL;
  }

  inline void save_and_clear(SQL_I_List<T> *save)
  {
    *save= *this;
    empty();
  }

  inline void push_front(SQL_I_List<T> *save)
  {
    
    *save->next= first;
    first= save->first;
    elements+= save->elements;
  }

  inline void push_back(SQL_I_List<T> *save)
  {
    if (save->first)
    {
      *next= save->first;
      next= save->next;
      elements+= save->elements;
    }
  }
};







struct list_node :public Sql_alloc
{
  list_node *next;
  void *info;
  list_node(void *info_par,list_node *next_par)
    :next(next_par),info(info_par)
  {}
  list_node()					
  {
    info= 0;
    next= this;
  }
};

typedef bool List_eq(void *a, void *b);

extern MYSQL_PLUGIN_IMPORT list_node end_of_list;

class base_list :public Sql_alloc
{
protected:
  list_node *first,**last;

public:
  uint elements;

  bool operator==(const base_list &rhs) const
  {
    return
      elements == rhs.elements &&
      first == rhs.first &&
      last == rhs.last;
  }
  base_list& operator=(const base_list &rhs)
  {
    elements= rhs.elements;
    first= rhs.first;
    last= elements ? rhs.last : &first;
    return *this;
  }

  inline void empty() { elements=0; first= &end_of_list; last=&first;}
  inline base_list() { empty(); }
  
  inline base_list(const base_list &tmp) :Sql_alloc()
  {
    *this= tmp;
  }
  
  bool copy(const base_list *rhs, MEM_ROOT *mem_root);
  base_list(const base_list &rhs, MEM_ROOT *mem_root) { copy(&rhs, mem_root); }
  inline base_list(bool) {}
  inline bool push_back(void *info)
  {
    if (((*last)=new list_node(info, &end_of_list)))
    {
      last= &(*last)->next;
      elements++;
      return 0;
    }
    return 1;
  }
  inline bool push_back(void *info, MEM_ROOT *mem_root)
  {
    if (((*last)=new (mem_root) list_node(info, &end_of_list)))
    {
      last= &(*last)->next;
      elements++;
      return 0;
    }
    return 1;
  }
  bool push_front_impl(list_node *node)
  {
    if (node)
    {
      if (last == &first)
        last= &node->next;
      first=node;
      elements++;
      return 0;
    }
    return 1;
  }
  inline bool push_front(void *info)
  { return push_front_impl(new list_node(info, first)); }
  inline bool push_front(void *info, MEM_ROOT *mem_root)
  { return push_front_impl(new (mem_root) list_node(info,first)); }
  void remove(list_node **prev)
  {
    list_node *node=(*prev)->next;
    if (!--elements)
      last= &first;
    else if (last == &(*prev)->next)
      last= prev;
    delete *prev;
    *prev=node;
  }
  inline void append(base_list *list)
  {
    if (!list->is_empty())
    {
      if (is_empty())
      {
        *this= *list;
        return;
      }
      *last= list->first;
      last= list->last;
      elements+= list->elements;
    }
  }
  inline void *pop(void)
  {
    if (first == &end_of_list) return 0;
    list_node *tmp=first;
    first=first->next;
    if (!--elements)
      last= &first;
    return tmp->info;
  }

  
  inline void disjoin(const base_list *list)
  {
    list_node **prev= &first;
    list_node *node= first;
    list_node *list_first= list->first;
    elements=0;
    while (node != &end_of_list && node != list_first)
    {
      prev= &node->next;
      node= node->next;
      elements++;
      if (node == &end_of_list)
        return;
    }
    *prev= &end_of_list;
    last= prev;
  }
  inline void prepend(base_list *list)
  {
    if (!list->is_empty())
    {
      if (is_empty())
        last= list->last;
      *list->last= first;
      first= list->first;
      elements+= list->elements;
    }
  }
  
  inline void swap(base_list &rhs)
  {
    list_node **rhs_last=rhs.last;
    swap_variables(list_node *, first, rhs.first);
    swap_variables(uint, elements, rhs.elements);
    rhs.last= last == &first ? &rhs.first : last;
    last = rhs_last == &rhs.first ? &first : rhs_last;
  }

  inline list_node* last_node() { return *last; }
  inline list_node* first_node() { return first;}
  inline void *head() { return first->info; }
  inline void **head_ref() { return first != &end_of_list ? &first->info : 0; }
  inline bool is_empty() { return first == &end_of_list ; }
  inline list_node *last_ref() { return &end_of_list; }
  inline bool add_unique(void *info, List_eq *eq)
  {
    list_node *node= first;
    for (;
         node != &end_of_list && (!(*eq)(node->info, info));
         node= node->next) ;
    if (node == &end_of_list)
      return push_back(info);
    return 1;
  }
  friend class base_list_iterator;
  friend class error_list;
  friend class error_list_iterator;

  
  void *elem(uint n)
  {
    list_node *node= first;
    void *data= NULL;
    for (uint i= 0; i <= n; i++)
    {
      if (node == &end_of_list)
      {
        data= NULL;
        break;
      }
      data= node->info;
      node= node->next;
    }
    return data;
  }

#ifdef LIST_EXTRA_DEBUG
  

  bool check_list(const char *name)
  {
    base_list *list= this;
    list_node *node= first;
    uint cnt= 0;

    while (node->next != &end_of_list)
    {
      if (!node->info)
      {
        DBUG_PRINT("list_invariants",("%s: error: NULL element in the list", 
                                      name));
        return FALSE;
      }
      node= node->next;
      cnt++;
    }
    if (last != &(node->next))
    {
      DBUG_PRINT("list_invariants", ("%s: error: wrong last pointer", name));
      return FALSE;
    }
    if (cnt+1 != elements)
    {
      DBUG_PRINT("list_invariants", ("%s: error: wrong element count", name));
      return FALSE;
    }
    DBUG_PRINT("list_invariants", ("%s: list is ok", name));
    return TRUE;
  }
#endif 

protected:
  void after(void *info,list_node *node)
  {
    list_node *new_node=new list_node(info,node->next);
    node->next=new_node;
    elements++;
    if (last == &(node->next))
      last= &new_node->next;
  }
};


class base_list_iterator
{
protected:
  base_list *list;
  list_node **el,**prev,*current;
  void sublist(base_list &ls, uint elm)
  {
    ls.first= *el;
    ls.last= list->last;
    ls.elements= elm;
  }
public:
  base_list_iterator() 
    :list(0), el(0), prev(0), current(0)
  {}

  base_list_iterator(base_list &list_par) 
  { init(list_par); }

  inline void init(base_list &list_par)
  {
    list= &list_par;
    el= &list_par.first;
    prev= 0;
    current= 0;
  }

  inline void *next(void)
  {
    prev=el;
    current= *el;
    el= &current->next;
    return current->info;
  }
  
  inline void *peek()
  {
    return (*el)->info;
  }
  inline void *next_fast(void)
  {
    list_node *tmp;
    tmp= *el;
    el= &tmp->next;
    return tmp->info;
  }
  inline void rewind(void)
  {
    el= &list->first;
  }
  inline void *replace(void *element)
  {						
    void *tmp=current->info;
    DBUG_ASSERT(current->info != 0);
    current->info=element;
    return tmp;
  }
  void *replace(base_list &new_list)
  {
    void *ret_value=current->info;
    if (!new_list.is_empty())
    {
      *new_list.last=current->next;
      current->info=new_list.first->info;
      current->next=new_list.first->next;
      if ((list->last == &current->next) && (new_list.elements > 1))
	list->last= new_list.last;
      list->elements+=new_list.elements-1;
    }
    return ret_value;				
  }
  inline void remove(void)			
  {
    list->remove(prev);
    el=prev;
    current=0;					
  }
  void after(void *element)			
  {
    list->after(element,current);
    current=current->next;
    el= &current->next;
  }
  inline void **ref(void)			
  {
    return &current->info;
  }
  inline bool is_last(void)
  {
    return el == &list->last_ref()->next;
  }
  inline bool at_end()
  {
    return current == &end_of_list;
  }
  friend class error_list_iterator;
};

template <class T> class List :public base_list
{
public:
  inline List() :base_list() {}
  inline List(const List<T> &tmp) :base_list(tmp) {}
  inline List(const List<T> &tmp, MEM_ROOT *mem_root) :
    base_list(tmp, mem_root) {}
  inline bool push_back(T *a) { return base_list::push_back(a); }
  inline bool push_back(T *a, MEM_ROOT *mem_root)
  { return base_list::push_back((void*) a, mem_root); }
  inline bool push_front(T *a) { return base_list::push_front(a); }
  inline bool push_front(T *a, MEM_ROOT *mem_root)
  { return base_list::push_front((void*) a, mem_root); }
  inline T* head() {return (T*) base_list::head(); }
  inline T** head_ref() {return (T**) base_list::head_ref(); }
  inline T* pop()  {return (T*) base_list::pop(); }
  inline void append(List<T> *list) { base_list::append(list); }
  inline void prepend(List<T> *list) { base_list::prepend(list); }
  inline void disjoin(List<T> *list) { base_list::disjoin(list); }
  inline bool add_unique(T *a, bool (*eq)(T *a, T *b))
  { return base_list::add_unique(a, (List_eq *)eq); }
  inline bool copy(const List<T> *list, MEM_ROOT *root)
  { return base_list::copy(list, root); }
  void delete_elements(void)
  {
    list_node *element,*next;
    for (element=first; element != &end_of_list; element=next)
    {
      next=element->next;
      delete (T*) element->info;
    }
    empty();
  }
  T *elem(uint n) { return (T*) base_list::elem(n); }
};


template <class T> class List_iterator :public base_list_iterator
{
public:
  List_iterator(List<T> &a) : base_list_iterator(a) {}
  List_iterator() : base_list_iterator() {}
  inline void init(List<T> &a) { base_list_iterator::init(a); }
  inline T* operator++(int) { return (T*) base_list_iterator::next(); }
  inline T* peek() { return (T*) base_list_iterator::peek(); }
  inline T *replace(T *a)   { return (T*) base_list_iterator::replace(a); }
  inline T *replace(List<T> &a) { return (T*) base_list_iterator::replace(a); }
  inline void rewind(void)  { base_list_iterator::rewind(); }
  inline void remove()      { base_list_iterator::remove(); }
  inline void after(T *a)   { base_list_iterator::after(a); }
  inline T** ref(void)	    { return (T**) base_list_iterator::ref(); }
};


template <class T> class List_iterator_fast :public base_list_iterator
{
protected:
  inline T *replace(T *)   { return (T*) 0; }
  inline T *replace(List<T> &) { return (T*) 0; }
  inline void remove(void)  {}
  inline void after(T *)    {}
  inline T** ref(void)	    { return (T**) 0; }

public:
  inline List_iterator_fast(List<T> &a) : base_list_iterator(a) {}
  inline List_iterator_fast() : base_list_iterator() {}
  inline void init(List<T> &a) { base_list_iterator::init(a); }
  inline T* operator++(int) { return (T*) base_list_iterator::next_fast(); }
  inline void rewind(void)  { base_list_iterator::rewind(); }
  void sublist(List<T> &list_arg, uint el_arg)
  {
    base_list_iterator::sublist(list_arg, el_arg);
  }
};




template <class T> 
inline void bubble_sort(List<T> *list_to_sort,
                        int (*sort_func)(T *a, T *b, void *arg), void *arg)
{
  bool swap;
  T **ref1= 0;
  T **ref2= 0;
  List_iterator<T> it(*list_to_sort);
  do
  {
    T **last_ref= ref1;
    T *item1= it++;
    ref1= it.ref();
    T *item2;

    swap= FALSE;
    while ((item2= it++) && (ref2= it.ref()) != last_ref)
    {
      if (sort_func(item1, item2, arg) > 0)
      {
        *ref1= item2;
        *ref2= item1;
        swap= TRUE;
      }
      else
        item1= item2;
      ref1= ref2;
    }
    it.rewind();
  } while (swap);
}




struct ilink
{
  struct ilink **prev,*next;
  static void *operator new(size_t size) throw ()
  {
    return (void*)my_malloc((uint)size, MYF(MY_WME | MY_FAE | ME_FATALERROR));
  }
  static void operator delete(void* ptr_arg, size_t)
  {
     my_free(ptr_arg);
  }

  inline ilink()
  {
    prev=0; next=0;
  }
  inline void unlink()
  {
    
    if (prev) *prev= next;
    if (next) next->prev=prev;
    prev=0 ; next=0;
  }
  inline void assert_linked()
  {
    DBUG_ASSERT(prev != 0 && next != 0);
  }
  inline void assert_not_linked()
  {
    DBUG_ASSERT(prev == 0 && next == 0);
  }
  virtual ~ilink() { unlink(); }		
};




class i_string: public ilink
{
public:
  const char* ptr;
  i_string():ptr(0) { }
  i_string(const char* s) : ptr(s) {}
};


class i_string_pair: public ilink
{
public:
  const char* key;
  const char* val;
  i_string_pair():key(0),val(0) { }
  i_string_pair(const char* key_arg, const char* val_arg) : 
    key(key_arg),val(val_arg) {}
};


template <class T> class I_List_iterator;


class base_ilist
{
  struct ilink *first;
  struct ilink last;
public:
  inline void empty() { first= &last; last.prev= &first; }
  base_ilist() { empty(); }
  inline bool is_empty() {  return first == &last; }
  
  
  inline bool is_last(ilink *p) { return p->next == NULL || p->next == &last; }
  inline void append(ilink *a)
  {
    first->prev= &a->next;
    a->next=first; a->prev= &first; first=a;
  }
  inline void push_back(ilink *a)
  {
    *last.prev= a;
    a->next= &last;
    a->prev= last.prev;
    last.prev= &a->next;
  }
  inline struct ilink *get()
  {
    struct ilink *first_link=first;
    if (first_link == &last)
      return 0;
    first_link->unlink();			
    return first_link;
  }
  inline struct ilink *head()
  {
    return (first != &last) ? first : 0;
  }

  

  void move_elements_to(base_ilist *new_owner)
  {
    DBUG_ASSERT(new_owner->is_empty());
    new_owner->first= first;
    new_owner->last= last;
    empty();
  }

  friend class base_ilist_iterator;
 private:
  
  base_ilist(const base_ilist&);
  void operator=(const base_ilist&);
};


class base_ilist_iterator
{
  base_ilist *list;
  struct ilink **el,*current;
public:
  base_ilist_iterator(base_ilist &list_par) :list(&list_par),
    el(&list_par.first),current(0) {}
  void *next(void)
  {
    
    current= *el;
    if (current == &list->last) return 0;
    el= &current->next;
    return current;
  }
};


template <class T>
class I_List :private base_ilist
{
public:
  I_List() :base_ilist()	{}
  inline bool is_last(T *p)     { return base_ilist::is_last(p); }
  inline void empty()		{ base_ilist::empty(); }
  inline bool is_empty()        { return base_ilist::is_empty(); } 
  inline void append(T* a)	{ base_ilist::append(a); }
  inline void push_back(T* a)	{ base_ilist::push_back(a); }
  inline T* get()		{ return (T*) base_ilist::get(); }
  inline T* head()		{ return (T*) base_ilist::head(); }
  inline void move_elements_to(I_List<T>* new_owner) {
    base_ilist::move_elements_to(new_owner);
  }
#ifndef _lint
  friend class I_List_iterator<T>;
#endif
};


template <class T> class I_List_iterator :public base_ilist_iterator
{
public:
  I_List_iterator(I_List<T> &a) : base_ilist_iterator(a) {}
  inline T* operator++(int) { return (T*) base_ilist_iterator::next(); }
};



template <typename T>
inline
void
list_copy_and_replace_each_value(List<T> &list, MEM_ROOT *mem_root)
{
  
  List_iterator<T> it(list);
  T *el;
  while ((el= it++))
    it.replace(el->clone(mem_root));
}

void free_list(I_List <i_string_pair> *list);
void free_list(I_List <i_string> *list);

#endif 
