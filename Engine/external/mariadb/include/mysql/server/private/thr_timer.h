



#ifndef THR_TIMER_INCLUDED
#define THR_TIMER_INCLUDED
#ifdef	__cplusplus
extern "C" {
#endif

typedef struct st_timer {
  struct timespec expire_time;
  my_bool expired;
  uint index_in_queue;
  void (*func)(void*);
  void *func_arg;
} thr_timer_t;


my_bool init_thr_timer(uint init_size_for_timer_queue);
void end_thr_timer();


void thr_timer_init(thr_timer_t *timer_data, void(*function)(void*),
                    void *arg);
my_bool thr_timer_settime(thr_timer_t *timer_data, ulonglong microseconds);
void    thr_timer_end(thr_timer_t *timer_data);

#ifdef	__cplusplus
}
#endif 
#endif 
