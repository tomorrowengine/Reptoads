



#ifdef __WIN__
#define MY_CONTEXT_USE_WIN32_FIBERS 1
#elif defined(__GNUC__) && __GNUC__ >= 3 && defined(__x86_64__) && !defined(__ILP32__)
#define MY_CONTEXT_USE_X86_64_GCC_ASM
#elif defined(__GNUC__) && __GNUC__ >= 3 && defined(__i386__)
#define MY_CONTEXT_USE_I386_GCC_ASM
#elif defined(HAVE_UCONTEXT_H)
#define MY_CONTEXT_USE_UCONTEXT
#else
#define MY_CONTEXT_DISABLE
#endif

#ifdef MY_CONTEXT_USE_WIN32_FIBERS
struct my_context {
  void (*user_func)(void *);
  void *user_arg;
  void *app_fiber;
  void *lib_fiber;
  int return_value;
#ifndef DBUG_OFF
  void *dbug_state;
#endif
};
#endif


#ifdef MY_CONTEXT_USE_UCONTEXT
#include <ucontext.h>

struct my_context {
  void (*user_func)(void *);
  void *user_data;
  void *stack;
  size_t stack_size;
  ucontext_t base_context;
  ucontext_t spawned_context;
  int active;
#ifdef HAVE_VALGRIND_MEMCHECK_H
  unsigned int valgrind_stack_id;
#endif
#ifndef DBUG_OFF
  void *dbug_state;
#endif
};
#endif


#ifdef MY_CONTEXT_USE_X86_64_GCC_ASM
#include <stdint.h>

struct my_context {
  uint64_t save[9];
  void *stack_top;
  void *stack_bot;
#ifdef HAVE_VALGRIND_MEMCHECK_H
  unsigned int valgrind_stack_id;
#endif
#ifndef DBUG_OFF
  void *dbug_state;
#endif
};
#endif


#ifdef MY_CONTEXT_USE_I386_GCC_ASM
#include <stdint.h>

struct my_context {
  uint64_t save[7];
  void *stack_top;
  void *stack_bot;
#ifdef HAVE_VALGRIND_MEMCHECK_H
  unsigned int valgrind_stack_id;
#endif
#ifndef DBUG_OFF
  void *dbug_state;
#endif
};
#endif


#ifdef MY_CONTEXT_DISABLE
struct my_context {
  int dummy;
};
#endif



extern int my_context_init(struct my_context *c, size_t stack_size);


extern void my_context_destroy(struct my_context *c);


extern int my_context_spawn(struct my_context *c, void (*f)(void *), void *d);


extern int my_context_yield(struct my_context *c);


extern int my_context_continue(struct my_context *c);


struct mysql_async_context {
  
  unsigned int events_to_wait_for;
  
  unsigned int events_occurred;
  
  union {
    void *r_ptr;
    const void *r_const_ptr;
    int r_int;
    my_bool r_my_bool;
  } ret_result;
  
  unsigned int timeout_value;
  
  my_bool active;
  
  my_bool suspended;
  
  void (*suspend_resume_hook)(my_bool suspend, void *user_data);
  void *suspend_resume_hook_user_data;
  
  struct my_context async_context;
};
