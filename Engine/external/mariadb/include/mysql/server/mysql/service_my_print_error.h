

#ifndef MYSQL_SERVICE_MY_PRINT_ERROR_INCLUDED
#define MYSQL_SERVICE_MY_PRINT_ERROR_INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#ifndef MYSQL_ABI_CHECK
#include <stdarg.h>
#include <stdlib.h>
#endif

#define ME_ERROR_LOG    64      
#define ME_NOTE         1024    
#define ME_WARNING      2048    
#define ME_FATAL        4096    

extern struct my_print_error_service_st {
  void (*my_error_func)(unsigned int nr, unsigned long MyFlags, ...);
  void (*my_printf_error_func)(unsigned int nr, const char *fmt, unsigned long MyFlags,...);
  void (*my_printv_error_func)(unsigned int error, const char *format, unsigned long MyFlags, va_list ap);
} *my_print_error_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define my_error my_print_error_service->my_error_func
#define my_printf_error my_print_error_service->my_printf_error_func
#define my_printv_error(A,B,C,D) my_print_error_service->my_printv_error_func(A,B,C,D)

#else
#ifndef MY_ERROR_DEFINED
extern void my_error(unsigned int nr, unsigned long MyFlags, ...);
extern void my_printf_error(unsigned int my_err, const char *format, unsigned long MyFlags, ...);
extern void my_printv_error(unsigned int error, const char *format, unsigned long MyFlags,va_list ap);
#endif 
#endif 

#ifdef __cplusplus
}
#endif

#endif

