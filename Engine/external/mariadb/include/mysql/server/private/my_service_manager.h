


#ifndef MY_SERVICE_MANAGER_INCLUDED
#define MY_SERVICE_MANAGER_INCLUDED

#if defined(HAVE_SYSTEMD) && !defined(EMBEDDED_LIBRARY)

#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif  
#include <systemd/sd-daemon.h>

#define service_manager_extend_timeout(INTERVAL, FMTSTR, ...) \
  sd_notifyf(0, "STATUS=" FMTSTR "\nEXTEND_TIMEOUT_USEC=%u\n", ##__VA_ARGS__, INTERVAL * 1000000)

#else
#define sd_notify(X, Y)
#define sd_notifyf(E, F, ...)
#define service_manager_extend_timeout(I, FMTSTR, ...)
#endif

#endif 
