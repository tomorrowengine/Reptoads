#ifndef ITEM_INETFUNC_INCLUDED
#define ITEM_INETFUNC_INCLUDED




#include "item.h"



class Item_func_inet_aton : public Item_longlong_func
{
  bool check_arguments() const
  { return check_argument_types_can_return_text(0, arg_count); }
public:
  Item_func_inet_aton(THD *thd, Item *a): Item_longlong_func(thd, a) {}
  longlong val_int();
  const char *func_name() const { return "inet_aton"; }
  bool fix_length_and_dec()
  {
    decimals= 0;
    max_length= 21;
    maybe_null= 1;
    unsigned_flag= 1;
    return FALSE;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_inet_aton>(thd, this); }
};




class Item_func_inet_ntoa : public Item_str_func
{
public:
  Item_func_inet_ntoa(THD *thd, Item *a): Item_str_func(thd, a)
  { }
  String* val_str(String* str);
  const char *func_name() const { return "inet_ntoa"; }
  bool fix_length_and_dec()
  {
    decimals= 0;
    fix_length_and_charset(3 * 8 + 7, default_charset());
    maybe_null= 1;
    return FALSE;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_inet_ntoa>(thd, this); }
};




class Item_func_inet_bool_base : public Item_bool_func
{
public:
  inline Item_func_inet_bool_base(THD *thd, Item *ip_addr):
    Item_bool_func(thd, ip_addr)
  {
    null_value= false;
  }

public:
  virtual longlong val_int();
  bool need_parentheses_in_default() { return false; }

protected:
  virtual bool calc_value(const String *arg) = 0;
};




class Item_func_inet_str_base : public Item_str_ascii_func
{
public:
  inline Item_func_inet_str_base(THD *thd, Item *arg):
    Item_str_ascii_func(thd, arg)
  { }

public:
  virtual String *val_str_ascii(String *buffer);

protected:
  virtual bool calc_value(const String *arg, String *buffer) = 0;
};




class Item_func_inet6_aton : public Item_func_inet_str_base
{
public:
  inline Item_func_inet6_aton(THD *thd, Item *ip_addr):
    Item_func_inet_str_base(thd, ip_addr)
  { }

public:
  virtual const char *func_name() const
  { return "inet6_aton"; }

  virtual bool fix_length_and_dec()
  {
    decimals= 0;
    fix_length_and_charset(16, &my_charset_bin);
    maybe_null= 1;
    return FALSE;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_inet6_aton>(thd, this); }

protected:
  virtual bool calc_value(const String *arg, String *buffer);
};




class Item_func_inet6_ntoa : public Item_func_inet_str_base
{
public:
  inline Item_func_inet6_ntoa(THD *thd, Item *ip_addr):
    Item_func_inet_str_base(thd, ip_addr)
  { }

public:
  virtual const char *func_name() const
  { return "inet6_ntoa"; }

  virtual bool fix_length_and_dec()
  {
    decimals= 0;

    
    
    
    fix_length_and_charset(8 * 4 + 7, default_charset());

    maybe_null= 1;
    return FALSE;
  }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_inet6_ntoa>(thd, this); }

protected:
  virtual bool calc_value(const String *arg, String *buffer);
};




class Item_func_is_ipv4 : public Item_func_inet_bool_base
{
public:
  inline Item_func_is_ipv4(THD *thd, Item *ip_addr):
    Item_func_inet_bool_base(thd, ip_addr)
  { }

public:
  virtual const char *func_name() const
  { return "is_ipv4"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_is_ipv4>(thd, this); }

protected:
  virtual bool calc_value(const String *arg);
};




class Item_func_is_ipv6 : public Item_func_inet_bool_base
{
public:
  inline Item_func_is_ipv6(THD *thd, Item *ip_addr):
    Item_func_inet_bool_base(thd, ip_addr)
  { }

public:
  virtual const char *func_name() const
  { return "is_ipv6"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_is_ipv6>(thd, this); }

protected:
  virtual bool calc_value(const String *arg);
};




class Item_func_is_ipv4_compat : public Item_func_inet_bool_base
{
public:
  inline Item_func_is_ipv4_compat(THD *thd, Item *ip_addr):
    Item_func_inet_bool_base(thd, ip_addr)
  { }

public:
  virtual const char *func_name() const
  { return "is_ipv4_compat"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_is_ipv4_compat>(thd, this); }

protected:
  virtual bool calc_value(const String *arg);
};




class Item_func_is_ipv4_mapped : public Item_func_inet_bool_base
{
public:
  inline Item_func_is_ipv4_mapped(THD *thd, Item *ip_addr):
    Item_func_inet_bool_base(thd, ip_addr)
  { }

public:
  virtual const char *func_name() const
  { return "is_ipv4_mapped"; }
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_is_ipv4_mapped>(thd, this); }

protected:
  virtual bool calc_value(const String *arg);
};

#endif 
