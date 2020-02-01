#ifndef MY_CPU_INCLUDED
#define MY_CPU_INCLUDED






#ifdef _ARCH_PWR8
#include <sys/platform/ppc.h>

#define HMT_very_low() __ppc_set_ppr_very_low()

#define HMT_low() __ppc_set_ppr_low()

#define HMT_medium_low() __ppc_set_ppr_med_low()

#define HMT_medium() __ppc_set_ppr_med()

#define HMT_medium_high() __ppc_set_ppr_med_high()

#define HMT_high() asm volatile("or 3,3,3")
#else
#define HMT_very_low()
#define HMT_low()
#define HMT_medium_low()
#define HMT_medium()
#define HMT_medium_high()
#define HMT_high()
#endif


static inline void MY_RELAX_CPU(void)
{
#ifdef HAVE_PAUSE_INSTRUCTION
  
#ifdef __SUNPRO_CC
  asm ("pause" );
#else
  __asm__ __volatile__ ("pause");
#endif

#elif defined(HAVE_FAKE_PAUSE_INSTRUCTION)
  __asm__ __volatile__ ("rep; nop");
#elif defined _WIN32
  
  YieldProcessor();
#elif defined(_ARCH_PWR8)
  __ppc_get_timebase();
#else
  int32 var, oldval = 0;
  my_atomic_cas32_strong_explicit(&var, &oldval, 1, MY_MEMORY_ORDER_RELAXED,
                                  MY_MEMORY_ORDER_RELAXED);
#endif
}




static inline int LF_BACKOFF(void)
{
  int i;
  for (i= 0; i < 200; i++)
    MY_RELAX_CPU();
  return 1;
}
#endif
