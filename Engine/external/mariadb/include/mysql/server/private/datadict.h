#ifndef DATADICT_INCLUDED
#define DATADICT_INCLUDED


#include "handler.h"



enum Table_type
{
  TABLE_TYPE_UNKNOWN,
  TABLE_TYPE_NORMAL,                            
  TABLE_TYPE_SEQUENCE,
  TABLE_TYPE_VIEW
};



enum Table_type dd_frm_type(THD *thd, char *path, LEX_CSTRING *engine_name,
                            bool *is_sequence);

static inline bool dd_frm_is_view(THD *thd, char *path)
{
  bool not_used2;
  return dd_frm_type(thd, path, NULL, &not_used2) == TABLE_TYPE_VIEW;
}

bool dd_recreate_table(THD *thd, const char *db, const char *table_name,
                       const char *path = NULL);

#endif 
