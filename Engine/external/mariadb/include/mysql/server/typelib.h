


#ifndef _typelib_h
#define _typelib_h

#include "my_alloc.h"

typedef struct st_typelib {	
  unsigned int count;		
  const char *name;		
  const char **type_names;
  unsigned int *type_lengths;
} TYPELIB;

extern my_ulonglong find_typeset(char *x, TYPELIB *typelib,int *error_position);
extern int find_type_with_warning(const char *x, TYPELIB *typelib,
                                  const char *option);
#define FIND_TYPE_BASIC           0

#define FIND_TYPE_NO_PREFIX      (1U << 0)

#define FIND_TYPE_NO_OVERWRITE   0

#define FIND_TYPE_ALLOW_NUMBER   0

#define FIND_TYPE_COMMA_TERM     (1U << 3)

extern int find_type(const char *x, const TYPELIB *typelib, unsigned int flags);
extern void make_type(char *to,unsigned int nr,TYPELIB *typelib);
extern const char *get_type(TYPELIB *typelib,unsigned int nr);
extern TYPELIB *copy_typelib(MEM_ROOT *root, TYPELIB *from);

extern TYPELIB sql_protocol_typelib;

my_ulonglong find_set_from_flags(const TYPELIB *lib, unsigned int default_name,
                              my_ulonglong cur_set, my_ulonglong default_set,
                              const char *str, unsigned int length,
                              char **err_pos, unsigned int *err_len);

#endif 
