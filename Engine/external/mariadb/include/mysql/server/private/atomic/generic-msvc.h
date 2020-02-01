#ifndef ATOMIC_MSC_INCLUDED
#define ATOMIC_MSC_INCLUDED



#include <windows.h>

static inline int my_atomic_cas32(int32 volatile *a, int32 *cmp, int32 set)
{
  int32 initial_cmp= *cmp;
  int32 initial_a= InterlockedCompareExchange((volatile LONG*)a,
                                              set, initial_cmp);
  int ret= (initial_a == initial_cmp);
  if (!ret)
    *cmp= initial_a;
  return ret;
}

static inline int my_atomic_cas64(int64 volatile *a, int64 *cmp, int64 set)
{
  int64 initial_cmp= *cmp;
  int64 initial_a= InterlockedCompareExchange64((volatile LONGLONG*)a,
                                                (LONGLONG)set,
                                                (LONGLONG)initial_cmp);
  int ret= (initial_a == initial_cmp);
  if (!ret)
    *cmp= initial_a;
  return ret;
}

static inline int my_atomic_casptr(void * volatile *a, void **cmp, void *set)
{
  void *initial_cmp= *cmp;
  void *initial_a= InterlockedCompareExchangePointer(a, set, initial_cmp);
  int ret= (initial_a == initial_cmp);
  if (!ret)
    *cmp= initial_a;
  return ret;
}

static inline int32 my_atomic_add32(int32 volatile *a, int32 v)
{
  return (int32)InterlockedExchangeAdd((volatile LONG*)a, v);
}

static inline int64 my_atomic_add64(int64 volatile *a, int64 v)
{
  return (int64)InterlockedExchangeAdd64((volatile LONGLONG*)a, (LONGLONG)v);
}




static inline int32 my_atomic_load32(int32 volatile *a)
{
  int32 value= *a;
  MemoryBarrier();
  return value;
}

static inline int64 my_atomic_load64(int64 volatile *a)
{
#ifdef _M_X64
  int64 value= *a;
  MemoryBarrier();
  return value;
#else
  return (int64) InterlockedCompareExchange64((volatile LONGLONG *) a, 0, 0);
#endif
}

static inline void* my_atomic_loadptr(void * volatile *a)
{
  void *value= *a;
  MemoryBarrier();
  return value;
}

static inline int32 my_atomic_fas32(int32 volatile *a, int32 v)
{
  return (int32)InterlockedExchange((volatile LONG*)a, v);
}

static inline int64 my_atomic_fas64(int64 volatile *a, int64 v)
{
  return (int64)InterlockedExchange64((volatile LONGLONG*)a, v);
}

static inline void * my_atomic_fasptr(void * volatile *a, void * v)
{
  return InterlockedExchangePointer(a, v);
}

static inline void my_atomic_store32(int32 volatile *a, int32 v)
{
  MemoryBarrier();
  *a= v;
}

static inline void my_atomic_store64(int64 volatile *a, int64 v)
{
#ifdef _M_X64
  MemoryBarrier();
  *a= v;
#else
  (void) InterlockedExchange64((volatile LONGLONG *) a, v);
#endif
}

static inline void my_atomic_storeptr(void * volatile *a, void *v)
{
  MemoryBarrier();
  *a= v;
}

#endif 
