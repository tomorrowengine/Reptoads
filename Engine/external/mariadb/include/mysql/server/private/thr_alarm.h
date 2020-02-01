



#ifndef _thr_alarm_h
#define _thr_alarm_h
#ifdef	__cplusplus
extern "C" {
#endif

#ifndef USE_ALARM_THREAD
#define USE_ONE_SIGNAL_HAND		
#endif
#ifdef HAVE_rts_threads
#undef USE_ONE_SIGNAL_HAND
#define USE_ALARM_THREAD
#define THR_SERVER_ALARM SIGUSR1
#else
#define THR_SERVER_ALARM SIGALRM
#endif

typedef struct st_alarm_info
{
  time_t next_alarm_time;
  uint active_alarms;
  uint max_used_alarms;
} ALARM_INFO;

void thr_alarm_info(ALARM_INFO *info);
extern my_bool my_disable_thr_alarm;

#ifdef _WIN32
#define DONT_USE_THR_ALARM
#endif
#if defined(DONT_USE_THR_ALARM)

#define USE_ALARM_THREAD
#undef USE_ONE_SIGNAL_HAND

typedef my_bool thr_alarm_t;
typedef my_bool ALARM;

#define thr_alarm_init(A) (*(A))=0
#define thr_alarm_in_use(A) (*(A) != 0)
#define thr_end_alarm(A)
#define thr_alarm(A,B,C) ((*(A)=1)-1)

#define thr_got_alarm(A) 0
#define init_thr_alarm(A)
#define thr_alarm_kill(A)
#define resize_thr_alarm(N)
#define end_thr_alarm(A)

#else
#if defined(__WIN__)
typedef struct st_thr_alarm_entry
{
  UINT_PTR crono;
} thr_alarm_entry;

#else 

typedef int thr_alarm_entry;

#define thr_got_alarm(thr_alarm) (**(thr_alarm))

#endif 

typedef thr_alarm_entry* thr_alarm_t;

typedef struct st_alarm {
  time_t expire_time;
  thr_alarm_entry alarmed;		
  pthread_t thread;
  my_thread_id thread_id;
  uint index_in_queue;
  my_bool malloced;
} ALARM;

extern uint thr_client_alarm;
extern pthread_t alarm_thread;

#define thr_alarm_init(A) (*(A))=0
#define thr_alarm_in_use(A) (*(A)!= 0)
void init_thr_alarm(uint max_alarm);
void resize_thr_alarm(uint max_alarms);
my_bool thr_alarm(thr_alarm_t *alarmed, uint sec, ALARM *buff);
void thr_alarm_kill(my_thread_id thread_id);
void thr_end_alarm(thr_alarm_t *alarmed);
void end_thr_alarm(my_bool free_structures);
sig_handler process_alarm(int);
#ifndef thr_got_alarm
my_bool thr_got_alarm(thr_alarm_t *alrm);
#endif


#endif 

#ifdef	__cplusplus
}
#endif 
#endif 
