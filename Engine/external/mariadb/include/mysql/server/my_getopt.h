

#ifndef _my_getopt_h
#define _my_getopt_h

#include "my_sys.h"             

#include <my_default.h>

C_MODE_START

#define GET_NO_ARG     1
#define GET_BOOL       2
#define GET_INT        3
#define GET_UINT       4
#define GET_LONG       5
#define GET_ULONG      6
#define GET_LL         7
#define GET_ULL        8
#define GET_STR        9
#define GET_STR_ALLOC 10
#define GET_DISABLED  11
#define GET_ENUM      12
#define GET_SET       13
#define GET_DOUBLE    14
#define GET_FLAGSET   15
#define GET_BIT       16

#define GET_ASK_ADDR     128
#define GET_AUTO          64
#define GET_TYPE_MASK     63


enum get_opt_arg_type { NO_ARG, OPT_ARG, REQUIRED_ARG };

struct st_typelib;

struct my_option
{
  const char *name;                     
  int        id;                        
  const char *comment;                  
  void       *value;                    
  void       *u_max_value;              
  struct st_typelib *typelib;           
  ulong     var_type;                   
  enum get_opt_arg_type arg_type;       
  longlong   def_value;                 
  longlong   min_value;                 
  ulonglong  max_value;                 
  longlong   sub_size;                  
  long       block_size;                
  void       *app_type;                 
};

typedef my_bool (*my_get_one_option)(int, const struct my_option *, char *);


typedef void *(*my_getopt_value)(const char *, uint, const struct my_option *,
                                 int *);


extern char *disabled_my_option;
extern char *autoset_my_option;
extern my_bool my_getopt_print_errors;
extern my_bool my_getopt_skip_unknown;
extern my_bool my_getopt_prefix_matching;
extern my_error_reporter my_getopt_error_reporter;

extern int handle_options (int *argc, char ***argv, 
			   const struct my_option *longopts, my_get_one_option);
extern void my_cleanup_options(const struct my_option *options);
extern void my_print_help(const struct my_option *options);
extern void my_print_variables(const struct my_option *options);
extern void my_getopt_register_get_addr(my_getopt_value);

ulonglong getopt_ull_limit_value(ulonglong num, const struct my_option *optp,
                                 my_bool *fix);
longlong getopt_ll_limit_value(longlong, const struct my_option *,
                               my_bool *fix);
double getopt_double_limit_value(double num, const struct my_option *optp,
                                 my_bool *fix);
my_bool getopt_compare_strings(const char *s, const char *t, uint length);

ulonglong getopt_double2ulonglong(double);
double getopt_ulonglong2double(ulonglong);

C_MODE_END

#endif 

