

#ifndef SQL_PARTITION_ADMIN_H
#define SQL_PARTITION_ADMIN_H

#ifndef WITH_PARTITION_STORAGE_ENGINE


class Sql_cmd_partition_unsupported : public Sql_cmd
{
public:
  Sql_cmd_partition_unsupported()
  {}

  ~Sql_cmd_partition_unsupported()
  {}

  
  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }

  bool execute(THD *thd);
};


class Sql_cmd_alter_table_exchange_partition :
  public Sql_cmd_partition_unsupported
{
public:
  Sql_cmd_alter_table_exchange_partition()
  {}

  ~Sql_cmd_alter_table_exchange_partition()
  {}
};


class  Sql_cmd_alter_table_analyze_partition :
  public Sql_cmd_partition_unsupported
{
public:
  Sql_cmd_alter_table_analyze_partition()
  {}

  ~Sql_cmd_alter_table_analyze_partition()
  {}
};


class Sql_cmd_alter_table_check_partition :
  public Sql_cmd_partition_unsupported
{
public:
  Sql_cmd_alter_table_check_partition()
  {}

  ~Sql_cmd_alter_table_check_partition()
  {}
};


class Sql_cmd_alter_table_optimize_partition :
  public Sql_cmd_partition_unsupported
{
public:
  Sql_cmd_alter_table_optimize_partition()
  {}

  ~Sql_cmd_alter_table_optimize_partition()
  {}
};


class Sql_cmd_alter_table_repair_partition :
  public Sql_cmd_partition_unsupported
{
public:
  Sql_cmd_alter_table_repair_partition()
  {}

  ~Sql_cmd_alter_table_repair_partition()
  {}
};


class Sql_cmd_alter_table_truncate_partition :
  public Sql_cmd_partition_unsupported
{
public:
  Sql_cmd_alter_table_truncate_partition()
  {}

  ~Sql_cmd_alter_table_truncate_partition()
  {}
};

#else


class Sql_cmd_alter_table_exchange_partition : public Sql_cmd_common_alter_table
{
public:
  
  Sql_cmd_alter_table_exchange_partition()
    : Sql_cmd_common_alter_table()
  {}

  ~Sql_cmd_alter_table_exchange_partition()
  {}

  bool execute(THD *thd);

private:
  bool exchange_partition(THD *thd, TABLE_LIST *, Alter_info *);
};



class Sql_cmd_alter_table_analyze_partition : public Sql_cmd_analyze_table
{
public:
  
  Sql_cmd_alter_table_analyze_partition()
    : Sql_cmd_analyze_table()
  {}

  ~Sql_cmd_alter_table_analyze_partition()
  {}

  bool execute(THD *thd);

  
  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }
};



class Sql_cmd_alter_table_check_partition : public Sql_cmd_check_table
{
public:
  
  Sql_cmd_alter_table_check_partition()
    : Sql_cmd_check_table()
  {}

  ~Sql_cmd_alter_table_check_partition()
  {}

  bool execute(THD *thd);

  
  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }
};



class Sql_cmd_alter_table_optimize_partition : public Sql_cmd_optimize_table
{
public:
  
  Sql_cmd_alter_table_optimize_partition()
    : Sql_cmd_optimize_table()
  {}

  ~Sql_cmd_alter_table_optimize_partition()
  {}

  bool execute(THD *thd);

  
  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }
};



class Sql_cmd_alter_table_repair_partition : public Sql_cmd_repair_table
{
public:
  
  Sql_cmd_alter_table_repair_partition()
    : Sql_cmd_repair_table()
  {}

  ~Sql_cmd_alter_table_repair_partition()
  {}

  bool execute(THD *thd);

  
  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }
};



class Sql_cmd_alter_table_truncate_partition : public Sql_cmd_truncate_table
{
public:
  
  Sql_cmd_alter_table_truncate_partition()
  {}

  virtual ~Sql_cmd_alter_table_truncate_partition()
  {}

  bool execute(THD *thd);

  
  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }
};

#endif 
#endif 
