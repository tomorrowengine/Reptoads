



#ifndef ITEM_CREATE_H
#define ITEM_CREATE_H

#include "item_func.h" 

typedef struct st_udf_func udf_func;



class Create_func
{
public:
  
  virtual Item *create_func(THD *thd, LEX_CSTRING *name, List<Item> *item_list) = 0;

protected:
  
  Create_func() {}
  
  virtual ~Create_func() {}
};




class Create_native_func : public Create_func
{
public:
  virtual Item *create_func(THD *thd, LEX_CSTRING *name,
                            List<Item> *item_list);

  
  virtual Item *create_native(THD *thd, LEX_CSTRING *name,
                              List<Item> *item_list) = 0;

protected:
  
  Create_native_func() {}
  
  virtual ~Create_native_func() {}
};




class Create_qfunc : public Create_func
{
public:
  
  virtual Item *create_func(THD *thd, LEX_CSTRING *name,
                            List<Item> *item_list);

  
  virtual Item *create_with_db(THD *thd, LEX_CSTRING *db, LEX_CSTRING *name,
                               bool use_explicit_name,
                               List<Item> *item_list) = 0;

protected:
  
  Create_qfunc() {}
  
  virtual ~Create_qfunc() {}
};



extern Create_func *find_native_function_builder(THD *thd,
                                                 const LEX_CSTRING *name);



extern Create_qfunc * find_qualified_function_builder(THD *thd);


#ifdef HAVE_DLOPEN


class Create_udf_func : public Create_func
{
public:
  virtual Item *create_func(THD *thd, LEX_CSTRING *name,
                            List<Item> *item_list);

  
  Item *create(THD *thd, udf_func *fct, List<Item> *item_list);

  
  static Create_udf_func s_singleton;

protected:
  
  Create_udf_func() {}
  
  virtual ~Create_udf_func() {}
};
#endif


Item *create_temporal_literal(THD *thd,
                              const char *str, size_t length,
                              CHARSET_INFO *cs,
                              enum_field_types type,
                              bool send_error);
inline
Item *create_temporal_literal(THD *thd, const String *str,
                              enum_field_types type,
                              bool send_error)
{
  return create_temporal_literal(thd,
                                 str->ptr(), str->length(), str->charset(),
                                 type, send_error);
}

struct Native_func_registry
{
  LEX_CSTRING name;
  Create_func *builder;
};

int item_create_init();
int item_create_append(Native_func_registry array[]);
void item_create_cleanup();

Item *create_func_dyncol_create(THD *thd, List<DYNCALL_CREATE_DEF> &list);
Item *create_func_dyncol_add(THD *thd, Item *str,
                             List<DYNCALL_CREATE_DEF> &list);
Item *create_func_dyncol_delete(THD *thd, Item *str, List<Item> &nums);
Item *create_func_dyncol_get(THD *thd, Item *num, Item *str,
                             const Type_handler *handler,
                             const char *c_len, const char *c_dec,
                             CHARSET_INFO *cs);
Item *create_func_dyncol_json(THD *thd, Item *str);
#endif

