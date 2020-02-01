

#ifndef SQL_ALTER_TABLE_H
#define SQL_ALTER_TABLE_H

class Alter_drop;
class Alter_column;
class Key;



class Alter_info
{
public:

  enum enum_enable_or_disable { LEAVE_AS_IS, ENABLE, DISABLE };

  bool vers_prohibited(THD *thd) const;

  
  enum enum_alter_table_algorithm
  {

    ALTER_TABLE_ALGORITHM_DEFAULT,

    
    ALTER_TABLE_ALGORITHM_COPY,

    
    ALTER_TABLE_ALGORITHM_INPLACE,

    
    ALTER_TABLE_ALGORITHM_NOCOPY,

    
    ALTER_TABLE_ALGORITHM_INSTANT
  };


  
  enum enum_alter_table_lock
  {
    
    ALTER_TABLE_LOCK_DEFAULT,

    
    ALTER_TABLE_LOCK_NONE,

    
    ALTER_TABLE_LOCK_SHARED,

    
    ALTER_TABLE_LOCK_EXCLUSIVE
  };


  
  List<Alter_drop>              drop_list;
  
  List<Alter_column>            alter_list;
  
  List<Key>                     key_list;
  
  List<Create_field>            create_list;

  enum flags_bits
  {
    CHECK_CONSTRAINT_IF_NOT_EXISTS= 1
  };
  List<Virtual_column_info>     check_constraint_list;
  
  alter_table_operations        flags;
  ulong                         partition_flags;
  
  enum_enable_or_disable        keys_onoff;
  
  List<const char>              partition_names;
  
  uint                          num_parts;
  
  enum_alter_table_algorithm    requested_algorithm;
  
  enum_alter_table_lock         requested_lock;


  Alter_info() :
  flags(0), partition_flags(0),
    keys_onoff(LEAVE_AS_IS),
    num_parts(0),
    requested_algorithm(ALTER_TABLE_ALGORITHM_DEFAULT),
    requested_lock(ALTER_TABLE_LOCK_DEFAULT)
  {}

  void reset()
  {
    drop_list.empty();
    alter_list.empty();
    key_list.empty();
    create_list.empty();
    check_constraint_list.empty();
    flags= 0;
    partition_flags= 0;
    keys_onoff= LEAVE_AS_IS;
    num_parts= 0;
    partition_names.empty();
    requested_algorithm= ALTER_TABLE_ALGORITHM_DEFAULT;
    requested_lock= ALTER_TABLE_LOCK_DEFAULT;
  }


  
  Alter_info(const Alter_info &rhs, MEM_ROOT *mem_root);


  
  bool set_requested_algorithm(const LEX_CSTRING *str);


  

  bool set_requested_lock(const LEX_CSTRING *str);

  
  const char* algorithm() const;

  
  const char* lock() const;

  
  bool supports_algorithm(THD *thd, enum_alter_inplace_result result,
                          const Alter_inplace_info *ha_alter_info);

  
  bool supports_lock(THD *thd, enum_alter_inplace_result result,
                     const Alter_inplace_info *ha_alter_info);

private:
  Alter_info &operator=(const Alter_info &rhs); 
  Alter_info(const Alter_info &rhs);            
};



class Alter_table_ctx
{
public:
  Alter_table_ctx();

  Alter_table_ctx(THD *thd, TABLE_LIST *table_list, uint tables_opened_arg,
                  const LEX_CSTRING *new_db_arg, const LEX_CSTRING *new_name_arg);

  
  bool is_database_changed() const
  { return (new_db.str != db.str); };

  
  bool is_table_renamed() const
  { return (is_database_changed() || new_name.str != table_name.str); };

  
  const char *get_new_filename() const
  {
    DBUG_ASSERT(!tmp_table);
    return new_filename;
  }

  
  const char *get_path() const
  {
    DBUG_ASSERT(!tmp_table);
    return path;
  }

  
  const char *get_new_path() const
  {
    DBUG_ASSERT(!tmp_table);
    return new_path;
  }

  
  const char *get_tmp_path() const
  { return tmp_path; }

  
  void set_fk_error_if_delete_row(FOREIGN_KEY_INFO *fk)
  {
    fk_error_if_delete_row= true;
    fk_error_id= fk->foreign_id->str;
    fk_error_table= fk->foreign_table->str;
  }

public:
  Create_field *datetime_field;
  bool         error_if_not_empty;
  uint         tables_opened;
  LEX_CSTRING  db;
  LEX_CSTRING  table_name;
  LEX_CSTRING  alias;
  LEX_CSTRING  new_db;
  LEX_CSTRING  new_name;
  LEX_CSTRING  new_alias;
  LEX_CSTRING  tmp_name;
  char         tmp_buff[80];
  
  bool         fk_error_if_delete_row;
  
  const char   *fk_error_id;
  
  const char   *fk_error_table;

private:
  char new_filename[FN_REFLEN + 1];
  char new_alias_buff[NAME_LEN + 1];
  char tmp_name_buff[NAME_LEN + 1];
  char path[FN_REFLEN + 1];
  char new_path[FN_REFLEN + 1];
  char tmp_path[FN_REFLEN + 1];

#ifdef DBUG_ASSERT_EXISTS
  
  bool tmp_table;
#endif

  Alter_table_ctx &operator=(const Alter_table_ctx &rhs); 
  Alter_table_ctx(const Alter_table_ctx &rhs);            
};



class Sql_cmd_common_alter_table : public Sql_cmd
{
protected:
  
  Sql_cmd_common_alter_table()
  {}

  virtual ~Sql_cmd_common_alter_table()
  {}

  virtual enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_TABLE;
  }
};


class Sql_cmd_alter_table : public Sql_cmd_common_alter_table
{
public:
  
  Sql_cmd_alter_table()
  {}

  ~Sql_cmd_alter_table()
  {}

  bool execute(THD *thd);
};



class Sql_cmd_alter_sequence : public Sql_cmd,
                               public DDL_options
{
public:
  
  Sql_cmd_alter_sequence(const DDL_options &options)
   :DDL_options(options)
  {}

  ~Sql_cmd_alter_sequence()
  {}

  enum_sql_command sql_command_code() const
  {
    return SQLCOM_ALTER_SEQUENCE;
  }
  bool execute(THD *thd);
};



class Sql_cmd_discard_import_tablespace : public Sql_cmd_common_alter_table
{
public:
  enum enum_tablespace_op_type
  {
    DISCARD_TABLESPACE, IMPORT_TABLESPACE
  };

  Sql_cmd_discard_import_tablespace(enum_tablespace_op_type tablespace_op_arg)
    : m_tablespace_op(tablespace_op_arg)
  {}

  bool execute(THD *thd);

private:
  const enum_tablespace_op_type m_tablespace_op;
};

#endif
