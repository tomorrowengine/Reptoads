


#ifndef _my_alarm_h
#define _my_alarm_h
#ifdef	__cplusplus
extern "C" {
#endif

extern int volatile my_have_got_alarm;
extern ulong my_time_to_wait_for_lock;

#if defined(HAVE_ALARM) && !defined(NO_ALARM_LOOP)
#include <signal.h>
#define ALARM_VARIABLES uint alarm_old=0; \
			sig_return alarm_signal=0
#define ALARM_INIT	my_have_got_alarm=0 ; \
			alarm_old=(uint) alarm(MY_HOW_OFTEN_TO_ALARM); \
			alarm_signal=signal(SIGALRM,my_set_alarm_variable);
#define ALARM_END	(void) signal(SIGALRM,alarm_signal); \
			(void) alarm(alarm_old);
#define ALARM_TEST	my_have_got_alarm
#ifdef SIGNAL_HANDLER_RESET_ON_DELIVERY
#define ALARM_REINIT	(void) alarm(MY_HOW_OFTEN_TO_ALARM); \
			(void) signal(SIGALRM,my_set_alarm_variable);\
			my_have_got_alarm=0;
#else
#define ALARM_REINIT	(void) alarm((uint) MY_HOW_OFTEN_TO_ALARM); \
			my_have_got_alarm=0;
#endif 
#else
#define ALARM_VARIABLES long alarm_pos=0,alarm_end_pos=MY_HOW_OFTEN_TO_WRITE-1
#define ALARM_INIT
#define ALARM_END
#define ALARM_TEST (alarm_pos++ >= alarm_end_pos)
#define ALARM_REINIT (alarm_end_pos+=MY_HOW_OFTEN_TO_WRITE)
#endif 

#ifdef	__cplusplus
}
#endif
#endif
