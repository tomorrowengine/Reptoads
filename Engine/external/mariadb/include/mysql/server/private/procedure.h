#ifndef PROCEDURE_INCLUDED
#define PROCEDURE_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface				
#endif


#include "sql_class.h"                          
#include "set_var.h"                            

#define PROC_NO_SORT 1				
#define PROC_GROUP   2				



class Item_proc :public Item
{
public:
  Item_proc(THD *thd, const char *name_par): Item(thd)
  {
     this->name.str=    name_par;
     this->name.length= strlen(name_par);
  }
  enum Type type() const { return Item::PROC_ITEM; }
  virtual void set(double nr)=0;
  virtual void set(const char *str,uint length,CHARSET_INFO *cs)=0;
  virtual void set(longlong nr)=0;
  const Type_handler *type_handler() const=0;
  void set(const char *str) { set(str,(uint) strlen(str), default_charset()); }
  void make_send_field(THD *thd, Send_field *tmp_field)
  {
    init_make_send_field(tmp_field,field_type());
  }
  unsigned int size_of() { return sizeof(*this);}
  bool check_vcol_func_processor(void *arg)
  {
    DBUG_ASSERT(0); 
    return mark_unsupported_function("proc", arg, VCOL_IMPOSSIBLE);
  }
  bool get_date(MYSQL_TIME *ltime, ulonglong fuzzydate)
  {
    return type_handler()->Item_get_date(this, ltime, fuzzydate);
  }
  Item* get_copy(THD *thd) { return 0; }
};

class Item_proc_real :public Item_proc
{
  double value;
public:
  Item_proc_real(THD *thd, const char *name_par, uint dec):
    Item_proc(thd, name_par)
  {
     decimals=dec; max_length=float_length(dec);
  }
  const Type_handler *type_handler() const { return &type_handler_double; }
  void set(double nr) { value=nr; }
  void set(longlong nr) { value=(double) nr; }
  void set(const char *str,uint length,CHARSET_INFO *cs)
  {
    int err_not_used;
    char *end_not_used;
    value= my_strntod(cs,(char*) str,length, &end_not_used, &err_not_used);
  }
  double val_real() { return value; }
  longlong val_int() { return (longlong) value; }
  String *val_str(String *s)
  {
    s->set_real(value,decimals,default_charset());
    return s;
  }
  my_decimal *val_decimal(my_decimal *);
  unsigned int size_of() { return sizeof(*this);}
};

class Item_proc_int :public Item_proc
{
  longlong value;
public:
  Item_proc_int(THD *thd, const char *name_par): Item_proc(thd, name_par)
  { max_length=11; }
  const Type_handler *type_handler() const { return &type_handler_longlong; }
  void set(double nr) { value=(longlong) nr; }
  void set(longlong nr) { value=nr; }
  void set(const char *str,uint length, CHARSET_INFO *cs)
  { int err; value=my_strntoll(cs,str,length,10,NULL,&err); }
  double val_real() { return (double) value; }
  longlong val_int() { return value; }
  String *val_str(String *s) { s->set(value, default_charset()); return s; }
  my_decimal *val_decimal(my_decimal *);
  unsigned int size_of() { return sizeof(*this);}
};


class Item_proc_string :public Item_proc
{
public:
  Item_proc_string(THD *thd, const char *name_par, uint length):
    Item_proc(thd, name_par) { this->max_length=length; }
  const Type_handler *type_handler() const { return &type_handler_varchar; }
  void set(double nr) { str_value.set_real(nr, 2, default_charset()); }
  void set(longlong nr) { str_value.set(nr, default_charset()); }
  void set(const char *str, uint length, CHARSET_INFO *cs)
  { str_value.copy(str,length,cs); }
  double val_real()
  {
    int err_not_used;
    char *end_not_used;
    CHARSET_INFO *cs= str_value.charset();
    return my_strntod(cs, (char*) str_value.ptr(), str_value.length(),
		      &end_not_used, &err_not_used);
  }
  longlong val_int()
  { 
    int err;
    CHARSET_INFO *cs=str_value.charset();
    return my_strntoll(cs,str_value.ptr(),str_value.length(),10,NULL,&err);
  }
  String *val_str(String*)
  {
    return null_value ? (String*) 0 : (String*) &str_value;
  }
  my_decimal *val_decimal(my_decimal *);
  unsigned int size_of() { return sizeof(*this);}  
};



class Procedure {
protected:
  List<Item> *fields;
  select_result *result;
public:
  const uint flags;
  ORDER *group,*param_fields;
  Procedure(select_result *res,uint flags_par) :result(res),flags(flags_par),
    group(0),param_fields(0) {}
  virtual ~Procedure() {group=param_fields=0; fields=0; }
  virtual void add(void)=0;
  virtual void end_group(void)=0;
  virtual int send_row(List<Item> &fields)=0;
  virtual bool change_columns(THD *thd, List<Item> &fields)= 0;
  virtual void update_refs(void) {}
  virtual int end_of_records() { return 0; }
};

Procedure *setup_procedure(THD *thd,ORDER *proc_param,select_result *result,
			   List<Item> &field_list,int *error);

#endif 
