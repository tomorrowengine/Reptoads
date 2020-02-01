#ifndef SQL_TRUNCATE_INCLUDED
#define SQL_TRUNCATE_INCLUDED


class THD;
struct TABLE_LIST;


class Sql_cmd_truncate_table : public Sql_cmd
{
private:
  
  MDL_ticket *m_ticket_downgrade;

public:
  
  Sql_cmd_truncate_table()
  {}

  virtual ~Sql_cmd_truncate_table()
  {}

  
  bool execute(THD *thd);

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_TRUNCATE;
  }

protected:
  enum truncate_result{
    TRUNCATE_OK=0,
    TRUNCATE_FAILED_BUT_BINLOG,
    TRUNCATE_FAILED_SKIP_BINLOG
  };

  
  bool lock_table(THD *, TABLE_LIST *, bool *);

  
  enum truncate_result handler_truncate(THD *, TABLE_LIST *, bool);

  
  bool truncate_table(THD *, TABLE_LIST *);
};

#endif
