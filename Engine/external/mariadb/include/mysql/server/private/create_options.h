



#ifndef SQL_CREATE_OPTIONS_INCLUDED
#define SQL_CREATE_OPTIONS_INCLUDED

#include "sql_class.h"

enum { ENGINE_OPTION_MAX_LENGTH=32767 };

class engine_option_value: public Sql_alloc
{
 public:
  LEX_CSTRING name;
  LEX_CSTRING value;
  engine_option_value *next;    
  bool parsed;                  
  bool quoted_value;            

  engine_option_value(engine_option_value *src,
                      engine_option_value **start, engine_option_value **end) :
    name(src->name), value(src->value),
    next(NULL), parsed(src->parsed), quoted_value(src->quoted_value)
  {
    link(start, end);
  }
  engine_option_value(LEX_CSTRING &name_arg, LEX_CSTRING &value_arg,
                      bool quoted,
                      engine_option_value **start, engine_option_value **end) :
    name(name_arg), value(value_arg),
    next(NULL), parsed(false), quoted_value(quoted)
  {
    link(start, end);
  }
  engine_option_value(LEX_CSTRING &name_arg,
                      engine_option_value **start, engine_option_value **end) :
    name(name_arg), value(null_clex_str),
    next(NULL), parsed(false), quoted_value(false)
  {
    link(start, end);
  }
  engine_option_value(LEX_CSTRING &name_arg, ulonglong value_arg,
                      engine_option_value **start, engine_option_value **end,
                      MEM_ROOT *root) :
    name(name_arg), next(NULL), parsed(false), quoted_value(false)
  {
    char *str;
    if (likely((value.str= str= (char *)alloc_root(root, 22))))
    {
      value.length= longlong10_to_str(value_arg, str, 10) - str;
      link(start, end);
    }
  }
  static uchar *frm_read(const uchar *buff, const uchar *buff_end,
                         engine_option_value **start,
                         engine_option_value **end, MEM_ROOT *root);
  void link(engine_option_value **start, engine_option_value **end);
  uint frm_length();
  uchar *frm_image(uchar *buff);
};

typedef struct st_key KEY;
class Create_field;

bool resolve_sysvar_table_options(handlerton *hton);
void free_sysvar_table_options(handlerton *hton);
bool parse_engine_table_options(THD *thd, handlerton *ht, TABLE_SHARE *share);
bool parse_option_list(THD* thd, handlerton *hton, void *option_struct,
                       engine_option_value **option_list,
                       ha_create_table_option *rules,
                       bool suppress_warning, MEM_ROOT *root);
bool engine_table_options_frm_read(const uchar *buff, size_t length,
                                   TABLE_SHARE *share);
engine_option_value *merge_engine_table_options(engine_option_value *source,
                                                engine_option_value *changes,
                                                MEM_ROOT *root);

uint engine_table_options_frm_length(engine_option_value *table_option_list,
                                     List<Create_field> &create_fields,
                                     uint keys, KEY *key_info);
uchar *engine_table_options_frm_image(uchar *buff,
                                      engine_option_value *table_option_list,
                                      List<Create_field> &create_fields,
                                      uint keys, KEY *key_info);

bool engine_options_differ(void *old_struct, void *new_struct,
                           ha_create_table_option *rules);
bool is_engine_option_known(engine_option_value *opt,
                            ha_create_table_option *rules);
#endif
