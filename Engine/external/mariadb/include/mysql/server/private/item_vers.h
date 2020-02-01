#ifndef ITEM_VERS_INCLUDED
#define ITEM_VERS_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

class Item_func_trt_ts: public Item_datetimefunc
{
  TR_table::field_id_t trt_field;
public:
  Item_func_trt_ts(THD *thd, Item* a, TR_table::field_id_t _trt_field);
  const char *func_name() const
  {
    if (trt_field == TR_table::FLD_BEGIN_TS)
    {
      return "trt_begin_ts";
    }
    return "trt_commit_ts";
  }
  bool get_date(MYSQL_TIME *res, ulonglong fuzzy_date);
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_trt_ts>(thd, this); }
  bool fix_length_and_dec()
  { fix_attributes_datetime(decimals); return FALSE; }
};

class Item_func_trt_id : public Item_longlong_func
{
  TR_table::field_id_t trt_field;
  bool backwards;

  longlong get_by_trx_id(ulonglong trx_id);
  longlong get_by_commit_ts(MYSQL_TIME &commit_ts, bool backwards);

public:
  Item_func_trt_id(THD *thd, Item* a, TR_table::field_id_t _trt_field, bool _backwards= false);
  Item_func_trt_id(THD *thd, Item* a, Item* b, TR_table::field_id_t _trt_field);

  const char *func_name() const
  {
    switch (trt_field)
    {
    case TR_table::FLD_TRX_ID:
      return "trt_trx_id";
    case TR_table::FLD_COMMIT_ID:
        return "trt_commit_id";
    case TR_table::FLD_ISO_LEVEL:
      return "trt_iso_level";
    default:
      DBUG_ASSERT(0);
    }
    return NULL;
  }

  bool fix_length_and_dec()
  {
    bool res= Item_int_func::fix_length_and_dec();
    max_length= 20;
    return res;
  }

  longlong val_int();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_trt_id>(thd, this); }
};

class Item_func_trt_trx_sees : public Item_bool_func
{
protected:
  bool accept_eq;

public:
  Item_func_trt_trx_sees(THD *thd, Item* a, Item* b);
  const char *func_name() const
  {
    return "trt_trx_sees";
  }
  longlong val_int();
  Item *get_copy(THD *thd)
  { return get_item_copy<Item_func_trt_trx_sees>(thd, this); }
};

class Item_func_trt_trx_sees_eq :
  public Item_func_trt_trx_sees
{
public:
  Item_func_trt_trx_sees_eq(THD *thd, Item* a, Item* b) :
    Item_func_trt_trx_sees(thd, a, b)
  {
    accept_eq= true;
  }
  const char *func_name() const
  {
    return "trt_trx_sees_eq";
  }
};

#endif 
