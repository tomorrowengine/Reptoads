#ifndef SQL_PREPARE_H
#define SQL_PREPARE_H


#include "sql_error.h"


#define LAST_STMT_ID 0xFFFFFFFF
#define STMT_ID_MASK 0x7FFFFFFF

class THD;
struct LEX;



class Reprepare_observer
{
public:
  
  bool report_error(THD *thd);
  bool is_invalidated() const { return m_invalidated; }
  void reset_reprepare_observer() { m_invalidated= FALSE; }
private:
  bool m_invalidated;
};


void mysqld_stmt_prepare(THD *thd, const char *packet, uint packet_length);
void mysqld_stmt_execute(THD *thd, char *packet, uint packet_length);
void mysqld_stmt_execute_bulk(THD *thd, char *packet, uint packet_length);
void mysqld_stmt_bulk_execute(THD *thd, char *packet, uint packet_length);
void mysqld_stmt_close(THD *thd, char *packet);
void mysql_sql_stmt_prepare(THD *thd);
void mysql_sql_stmt_execute(THD *thd);
void mysql_sql_stmt_execute_immediate(THD *thd);
void mysql_sql_stmt_close(THD *thd);
void mysqld_stmt_fetch(THD *thd, char *packet, uint packet_length);
void mysqld_stmt_reset(THD *thd, char *packet);
void mysql_stmt_get_longdata(THD *thd, char *pos, ulong packet_length);
void reinit_stmt_before_use(THD *thd, LEX *lex);

my_bool bulk_parameters_iterations(THD *thd);
my_bool bulk_parameters_set(THD *thd);


class Server_runnable
{
public:
  virtual bool execute_server_code(THD *thd)= 0;
  virtual ~Server_runnable();
};




class Ed_row;



class Ed_result_set: public Sql_alloc
{
public:
  operator List<Ed_row>&() { return *m_rows; }
  unsigned int size() const { return m_rows->elements; }

  Ed_result_set(List<Ed_row> *rows_arg, size_t column_count,
                MEM_ROOT *mem_root_arg);

  
  ~Ed_result_set() {}

  size_t get_field_count() const { return m_column_count; }

  static void operator delete(void *ptr, size_t size) throw ();
  static void operator delete(void *, MEM_ROOT *){}
private:
  Ed_result_set(const Ed_result_set &);        
  Ed_result_set &operator=(Ed_result_set &);   
private:
  MEM_ROOT m_mem_root;
  size_t m_column_count;
  List<Ed_row> *m_rows;
  Ed_result_set *m_next_rset;
  friend class Ed_connection;
};


class Ed_connection
{
public:
  
  Ed_connection(THD *thd);

  
  bool execute_direct(LEX_STRING sql_text);

  
  bool execute_direct(Server_runnable *server_runnable);

  
  ulonglong get_affected_rows() const
  {
    return m_diagnostics_area.affected_rows();
  }

  
  ulonglong get_last_insert_id() const
  {
    return m_diagnostics_area.last_insert_id();
  }

  
  ulong get_warn_count() const
  {
    return m_diagnostics_area.warn_count();
  }

  
  const char *get_last_error() const { return m_diagnostics_area.message(); }
  unsigned int get_last_errno() const { return m_diagnostics_area.sql_errno(); }
  const char *get_last_sqlstate() const { return m_diagnostics_area.get_sqlstate(); }

  
  Ed_result_set *use_result_set() { return m_current_rset; }
  
  Ed_result_set *store_result_set();

  
  bool has_next_result() const { return MY_TEST(m_current_rset->m_next_rset); }
  
  bool move_to_next_result()
  {
    m_current_rset= m_current_rset->m_next_rset;
    return MY_TEST(m_current_rset);
  }

  ~Ed_connection() { free_old_result(); }
private:
  Diagnostics_area m_diagnostics_area;
  
  THD *m_thd;
  Ed_result_set *m_rsets;
  Ed_result_set *m_current_rset;
  friend class Protocol_local;
private:
  void free_old_result();
  void add_result_set(Ed_result_set *ed_result_set);
private:
  Ed_connection(const Ed_connection &);        
  Ed_connection &operator=(Ed_connection &);   
};




struct Ed_column: public LEX_STRING
{
  
};




class Ed_row: public Sql_alloc
{
public:
  const Ed_column &operator[](const unsigned int column_index) const
  {
    return *get_column(column_index);
  }
  const Ed_column *get_column(const unsigned int column_index) const
  {
    DBUG_ASSERT(column_index < size());
    return m_column_array + column_index;
  }
  size_t size() const { return m_column_count; }

  Ed_row(Ed_column *column_array_arg, size_t column_count_arg)
    :m_column_array(column_array_arg),
    m_column_count(column_count_arg)
  {}
private:
  Ed_column *m_column_array;
  size_t m_column_count; 
};

#endif 
