#ifndef MY_LIBWRAP_INCLUDED
#define MY_LIBWRAP_INCLUDED



#ifdef HAVE_LIBWRAP
#include <tcpd.h>
#include <syslog.h>
#ifdef NEED_SYS_SYSLOG_H
#include <sys/syslog.h>
#endif 

extern void my_fromhost(struct request_info *req);
extern int my_hosts_access(struct request_info *req);
extern char *my_eval_client(struct request_info *req);

#endif 
#endif 
