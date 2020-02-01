

#ifndef SQL_TABLE_MAINTENANCE_H
#define SQL_TABLE_MAINTENANCE_H


#define SQL_ADMIN_MSG_TEXT_SIZE (128 * 1024)

bool mysql_assign_to_keycache(THD* thd, TABLE_LIST* table_list,
                              const LEX_CSTRING *key_cache_name);
bool mysql_preload_keys(THD* thd, TABLE_LIST* table_list);
int reassign_keycache_tables(THD* thd, KEY_CACHE *src_cache,
                             KEY_CACHE *dst_cache);


class Sql_cmd_analyze_table : public Sql_cmd
{
public:
  
  Sql_cmd_analyze_table()
  {}

  ~Sql_cmd_analyze_table()
  {}

  bool execute(THD *thd);

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ANALYZE;
  }
};




class Sql_cmd_check_table : public Sql_cmd
{
public:
  
  Sql_cmd_check_table()
  {}

  ~Sql_cmd_check_table()
  {}

  bool execute(THD *thd);

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_CHECK;
  }
};



class Sql_cmd_optimize_table : public Sql_cmd
{
public:
  
  Sql_cmd_optimize_table()
  {}

  ~Sql_cmd_optimize_table()
  {}

  bool execute(THD *thd);

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_OPTIMIZE;
  }
};




class Sql_cmd_repair_table : public Sql_cmd
{
public:
  
  Sql_cmd_repair_table()
  {}

  ~Sql_cmd_repair_table()
  {}

  bool execute(THD *thd);

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_REPAIR;
  }
};

#endif
