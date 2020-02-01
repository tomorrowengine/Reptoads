#ifndef MY_ATOMIC_INCLUDED
#define MY_ATOMIC_INCLUDED





#if defined(_MSC_VER)
#include "atomic/generic-msvc.h"
#elif defined(HAVE_SOLARIS_ATOMIC)
#include "atomic/solaris.h"
#elif defined(HAVE_GCC_C11_ATOMICS)
#include "atomic/gcc_builtins.h"
#elif defined(HAVE_GCC_ATOMIC_BUILTINS)
#include "atomic/gcc_sync.h"
#endif

#if SIZEOF_LONG == 4
#define my_atomic_addlong(A,B) my_atomic_add32((int32*) (A), (B))
#define my_atomic_loadlong(A) my_atomic_load32((int32*) (A))
#define my_atomic_loadlong_explicit(A,O) my_atomic_load32_explicit((int32*) (A), (O))
#define my_atomic_storelong(A,B) my_atomic_store32((int32*) (A), (B))
#define my_atomic_faslong(A,B) my_atomic_fas32((int32*) (A), (B))
#define my_atomic_caslong(A,B,C) my_atomic_cas32((int32*) (A), (int32*) (B), (C))
#else
#define my_atomic_addlong(A,B) my_atomic_add64((int64*) (A), (B))
#define my_atomic_loadlong(A) my_atomic_load64((int64*) (A))
#define my_atomic_loadlong_explicit(A,O) my_atomic_load64_explicit((int64*) (A), (O))
#define my_atomic_storelong(A,B) my_atomic_store64((int64*) (A), (B))
#define my_atomic_faslong(A,B) my_atomic_fas64((int64*) (A), (B))
#define my_atomic_caslong(A,B,C) my_atomic_cas64((int64*) (A), (int64*) (B), (C))
#endif

#ifndef MY_MEMORY_ORDER_SEQ_CST
#define MY_MEMORY_ORDER_RELAXED
#define MY_MEMORY_ORDER_CONSUME
#define MY_MEMORY_ORDER_ACQUIRE
#define MY_MEMORY_ORDER_RELEASE
#define MY_MEMORY_ORDER_ACQ_REL
#define MY_MEMORY_ORDER_SEQ_CST

#define my_atomic_store32_explicit(P, D, O) my_atomic_store32((P), (D))
#define my_atomic_store64_explicit(P, D, O) my_atomic_store64((P), (D))
#define my_atomic_storeptr_explicit(P, D, O) my_atomic_storeptr((P), (D))

#define my_atomic_load32_explicit(P, O) my_atomic_load32((P))
#define my_atomic_load64_explicit(P, O) my_atomic_load64((P))
#define my_atomic_loadptr_explicit(P, O) my_atomic_loadptr((P))

#define my_atomic_fas32_explicit(P, D, O) my_atomic_fas32((P), (D))
#define my_atomic_fas64_explicit(P, D, O) my_atomic_fas64((P), (D))
#define my_atomic_fasptr_explicit(P, D, O) my_atomic_fasptr((P), (D))

#define my_atomic_add32_explicit(P, A, O) my_atomic_add32((P), (A))
#define my_atomic_add64_explicit(P, A, O) my_atomic_add64((P), (A))
#define my_atomic_addptr_explicit(P, A, O) my_atomic_addptr((P), (A))

#define my_atomic_cas32_weak_explicit(P, E, D, S, F) \
  my_atomic_cas32((P), (E), (D))
#define my_atomic_cas64_weak_explicit(P, E, D, S, F) \
  my_atomic_cas64((P), (E), (D))
#define my_atomic_casptr_weak_explicit(P, E, D, S, F) \
  my_atomic_casptr((P), (E), (D))

#define my_atomic_cas32_strong_explicit(P, E, D, S, F) \
  my_atomic_cas32((P), (E), (D))
#define my_atomic_cas64_strong_explicit(P, E, D, S, F) \
  my_atomic_cas64((P), (E), (D))
#define my_atomic_casptr_strong_explicit(P, E, D, S, F) \
  my_atomic_casptr((P), (E), (D))
#endif

#endif 
