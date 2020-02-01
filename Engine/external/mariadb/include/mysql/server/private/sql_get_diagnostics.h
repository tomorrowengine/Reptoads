

#ifndef SQL_GET_DIAGNOSTICS_H
#define SQL_GET_DIAGNOSTICS_H


class Diagnostics_information;



class Sql_cmd_get_diagnostics : public Sql_cmd
{
public:
  
  Sql_cmd_get_diagnostics(Diagnostics_information *info)
    : m_info(info)
  {}

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_GET_DIAGNOSTICS;
  }

  virtual bool execute(THD *thd);

private:
  
  Diagnostics_information *m_info;
};



class Diagnostics_information : public Sql_alloc
{
public:
  
  enum Which_area
  {
    
    CURRENT_AREA
  };

  
  void set_which_da(Which_area area)
  { m_area= area; }

  
  Which_area get_which_da(void) const
  { return m_area; }

  
  virtual bool aggregate(THD *thd, const Diagnostics_area *da) = 0;

protected:
  
  virtual ~Diagnostics_information()
  {
    DBUG_ASSERT(false);
  }

  
  template <typename Diag_item, typename Context>
  bool evaluate(THD *thd, Diag_item *diag_item, Context ctx)
  {
    Item *value;

    
    if (! (value= diag_item->get_value(thd, ctx)))
      return true;

    
    return diag_item->set_value(thd, &value);
  }

private:
  
  Which_area m_area;
};



class Diagnostics_information_item : public Sql_alloc
{
public:
  
  bool set_value(THD *thd, Item **value);

protected:
  
  Diagnostics_information_item(Item *target)
    : m_target(target)
  {}

  
  virtual ~Diagnostics_information_item()
  {
    DBUG_ASSERT(false);
  }

private:
  
  Item *m_target;
};



class Statement_information_item : public Diagnostics_information_item
{
public:
  
  enum Name
  {
    NUMBER,
    ROW_COUNT
  };

  
  Statement_information_item(Name name, Item *target)
    : Diagnostics_information_item(target), m_name(name)
  {}

  
  Item *get_value(THD *thd, const Diagnostics_area *da);

private:
  
  Name m_name;
};



class Statement_information : public Diagnostics_information
{
public:
  
  Statement_information(List<Statement_information_item> *items)
    : m_items(items)
  {}

  
  bool aggregate(THD *thd, const Diagnostics_area *da);

private:
  
  List<Statement_information_item> *m_items;
};



class Condition_information_item : public Diagnostics_information_item
{
public:
  
  enum Name
  {
    CLASS_ORIGIN,
    SUBCLASS_ORIGIN,
    CONSTRAINT_CATALOG,
    CONSTRAINT_SCHEMA,
    CONSTRAINT_NAME,
    CATALOG_NAME,
    SCHEMA_NAME,
    TABLE_NAME,
    COLUMN_NAME,
    CURSOR_NAME,
    MESSAGE_TEXT,
    MYSQL_ERRNO,
    RETURNED_SQLSTATE
  };

  
  Condition_information_item(Name name, Item *target)
    : Diagnostics_information_item(target), m_name(name)
  {}

  
  Item *get_value(THD *thd, const Sql_condition *cond);

private:
  
  Name m_name;

  
  Item *make_utf8_string_item(THD *thd, const String *str);
};



class Condition_information : public Diagnostics_information
{
public:
  
  Condition_information(Item *cond_number_expr,
                        List<Condition_information_item> *items)
    : m_cond_number_expr(cond_number_expr), m_items(items)
  {}

  
  bool aggregate(THD *thd, const Diagnostics_area *da);

private:
  
  Item *m_cond_number_expr;

  
  List<Condition_information_item> *m_items;
};

#endif

