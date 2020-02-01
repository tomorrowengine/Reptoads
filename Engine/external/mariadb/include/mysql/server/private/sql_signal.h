

#ifndef SQL_SIGNAL_H
#define SQL_SIGNAL_H


class Sql_cmd_common_signal : public Sql_cmd
{
protected:
  
  Sql_cmd_common_signal(const sp_condition_value *cond,
                        const Set_signal_information& set)
    : Sql_cmd(),
      m_cond(cond),
      m_set_signal_information(set)
  {}

  virtual ~Sql_cmd_common_signal()
  {}

  
  int eval_signal_informations(THD *thd, Sql_condition *cond);

  
  bool raise_condition(THD *thd, Sql_condition *cond);

  
  const sp_condition_value *m_cond;

  
  Set_signal_information m_set_signal_information;
};


class Sql_cmd_signal : public Sql_cmd_common_signal
{
public:
  
  Sql_cmd_signal(const sp_condition_value *cond,
                 const Set_signal_information& set)
    : Sql_cmd_common_signal(cond, set)
  {}

  virtual ~Sql_cmd_signal()
  {}

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_SIGNAL;
  }

  virtual bool execute(THD *thd);
};


class Sql_cmd_resignal : public Sql_cmd_common_signal
{
public:
  
  Sql_cmd_resignal(const sp_condition_value *cond,
                   const Set_signal_information& set)
    : Sql_cmd_common_signal(cond, set)
  {}

  virtual ~Sql_cmd_resignal()
  {}

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_RESIGNAL;
  }

  virtual bool execute(THD *thd);
};

#endif

