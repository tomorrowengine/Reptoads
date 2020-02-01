#ifndef SQL_ALLOC_INCLUDED
#define SQL_ALLOC_INCLUDED


#include <my_sys.h>                    

THD *thd_get_current_thd();



class Sql_alloc
{
public:
  static void *operator new(size_t size) throw ()
  {
    return thd_alloc(thd_get_current_thd(), size);
  }
  static void *operator new[](size_t size) throw ()
  {
    return thd_alloc(thd_get_current_thd(), size);
  }
  static void *operator new[](size_t size, MEM_ROOT *mem_root) throw ()
  { return alloc_root(mem_root, size); }
  static void *operator new(size_t size, MEM_ROOT *mem_root) throw()
  { return alloc_root(mem_root, size); }
  static void operator delete(void *ptr, size_t size) { TRASH_FREE(ptr, size); }
  static void operator delete(void *, MEM_ROOT *){}
  static void operator delete[](void *, MEM_ROOT *)
  {  }
  static void operator delete[](void *ptr, size_t size) { TRASH_FREE(ptr, size); }
#ifdef HAVE_valgrind
  bool dummy_for_valgrind;
  inline Sql_alloc() :dummy_for_valgrind(0) {}
#else
  inline Sql_alloc() {}
#endif
  inline ~Sql_alloc() {}
};
#endif 
