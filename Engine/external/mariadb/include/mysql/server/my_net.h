



#ifndef _my_net_h
#define _my_net_h

C_MODE_START

#include <errno.h>
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#if defined(HAVE_POLL_H)
#include <poll.h>
#elif defined(HAVE_SYS_POLL_H)
#include <sys/poll.h>
#endif 
#ifdef HAVE_SYS_IOCTL_H
#include <sys/ioctl.h>
#endif

#if !defined(__WIN__)
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#if !defined(alpha_linux_port)
#include <netinet/tcp.h>
#endif
#endif

#if defined(__WIN__)
#define O_NONBLOCK 1    


#define SHUT_RDWR 0x02
#else
#include <netdb.h>     
#endif


#ifndef HAVE_IN_ADDR_T
#define in_addr_t uint32
#endif


C_MODE_END
#endif
