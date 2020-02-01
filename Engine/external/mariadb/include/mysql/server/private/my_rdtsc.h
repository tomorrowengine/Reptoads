



#ifndef MY_RDTSC_H
#define MY_RDTSC_H


struct my_timer_unit_info
{
  
  ulonglong routine;
  
  ulonglong overhead;
  
  ulonglong frequency;
  
  ulonglong resolution;
};


struct my_timer_info
{
  
  struct my_timer_unit_info cycles;
  
  struct my_timer_unit_info nanoseconds;
  
  struct my_timer_unit_info microseconds;
  
  struct my_timer_unit_info milliseconds;
  
  struct my_timer_unit_info ticks;
};

typedef struct my_timer_info MY_TIMER_INFO;

C_MODE_START


ulonglong my_timer_cycles(void);


ulonglong my_timer_nanoseconds(void);


ulonglong my_timer_microseconds(void);


ulonglong my_timer_milliseconds(void);


ulonglong my_timer_ticks(void);


void my_timer_init(MY_TIMER_INFO *mti);

C_MODE_END

#define MY_TIMER_ROUTINE_ASM_X86                  1
#define MY_TIMER_ROUTINE_ASM_X86_64               2
#define MY_TIMER_ROUTINE_RDTSCLL                  3
#define MY_TIMER_ROUTINE_ASM_X86_WIN              4
#define MY_TIMER_ROUTINE_RDTSC                    5
#define MY_TIMER_ROUTINE_ASM_IA64                 6
#define MY_TIMER_ROUTINE_ASM_PPC                  7
#define MY_TIMER_ROUTINE_SGI_CYCLE                8
#define MY_TIMER_ROUTINE_GETHRTIME                9
#define MY_TIMER_ROUTINE_READ_REAL_TIME          10
#define MY_TIMER_ROUTINE_CLOCK_GETTIME           11
#define MY_TIMER_ROUTINE_NXGETTIME               12
#define MY_TIMER_ROUTINE_GETTIMEOFDAY            13
#define MY_TIMER_ROUTINE_QUERYPERFORMANCECOUNTER 14
#define MY_TIMER_ROUTINE_GETTICKCOUNT            15
#define MY_TIMER_ROUTINE_TIME                    16
#define MY_TIMER_ROUTINE_TIMES                   17
#define MY_TIMER_ROUTINE_FTIME                   18
#define MY_TIMER_ROUTINE_ASM_PPC64               19
#define MY_TIMER_ROUTINE_ASM_SUNPRO_SPARC64      20
#define MY_TIMER_ROUTINE_ASM_SUNPRO_SPARC32      21
#define MY_TIMER_ROUTINE_ASM_SUNPRO_I386         22
#define MY_TIMER_ROUTINE_ASM_GCC_SPARC64         23
#define MY_TIMER_ROUTINE_ASM_GCC_SPARC32         24
#define MY_TIMER_ROUTINE_MACH_ABSOLUTE_TIME      25
#define MY_TIMER_ROUTINE_GETSYSTEMTIMEASFILETIME 26
#define MY_TIMER_ROUTINE_ASM_SUNPRO_X86_64       27
#define MY_TIMER_ROUTINE_ASM_S390                28

#endif

