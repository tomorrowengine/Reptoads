

#ifndef LOG_EVENT_OLD_H
#define LOG_EVENT_OLD_H








#undef ROWS_HEADER_LEN
#define ROWS_HEADER_LEN ROWS_HEADER_LEN_V1


class Old_rows_log_event : public Log_event
{
  
public:
  
  enum enum_error
  {
    ERR_OPEN_FAILURE = -1,               
    ERR_OK = 0,                                 
    ERR_TABLE_LIMIT_EXCEEDED = 1,      
    ERR_OUT_OF_MEM = 2,                         
    ERR_BAD_TABLE_DEF = 3,     
    ERR_RBR_TO_SBR = 4  
  };

  
  enum enum_flag
  {
    
    STMT_END_F = (1U << 0),

    
    NO_FOREIGN_KEY_CHECKS_F = (1U << 1),

    
    RELAXED_UNIQUE_CHECKS_F = (1U << 2),

    
    COMPLETE_ROWS_F = (1U << 3)
  };

  typedef uint16 flag_set;

  
  enum 
  {
      RLE_NO_FLAGS = 0U
  };

  virtual ~Old_rows_log_event();

  void set_flags(flag_set flags_arg) { m_flags |= flags_arg; }
  void clear_flags(flag_set flags_arg) { m_flags &= ~flags_arg; }
  flag_set get_flags(flag_set flags_arg) const { return m_flags & flags_arg; }

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  virtual void pack_info(Protocol *protocol);
#endif

#ifdef MYSQL_CLIENT
  
  virtual bool print(FILE *file, PRINT_EVENT_INFO *print_event_info)= 0;
#endif

#ifndef MYSQL_CLIENT
  int add_row_data(uchar *data, size_t length)
  {
    return do_add_row_data(data,length); 
  }
#endif

  
  virtual int get_data_size();

  MY_BITMAP const *get_cols() const { return &m_cols; }
  size_t get_width() const          { return m_width; }
  ulong get_table_id() const        { return m_table_id; }

#ifndef MYSQL_CLIENT
  virtual bool write_data_header();
  virtual bool write_data_body();
  virtual const char *get_db() { return m_table->s->db.str; }
#endif
  
  virtual bool is_valid() const
  {
    return m_rows_buf && m_cols.bitmap;
  }
  bool is_part_of_group() { return 1; }

  uint     m_row_count;         

protected:
  
#ifndef MYSQL_CLIENT
  Old_rows_log_event(THD*, TABLE*, ulong table_id,
                     MY_BITMAP const *cols, bool is_transactional);
#endif
  Old_rows_log_event(const char *row_data, uint event_len,
                     Log_event_type event_type,
                     const Format_description_log_event *description_event);

#ifdef MYSQL_CLIENT
  bool print_helper(FILE *, PRINT_EVENT_INFO *, char const *const name);
#endif

#ifndef MYSQL_CLIENT
  virtual int do_add_row_data(uchar *data, size_t length);
#endif

#ifndef MYSQL_CLIENT
  TABLE *m_table;		
#endif
  ulong       m_table_id;	
  MY_BITMAP   m_cols;		
  ulong       m_width;          

  ulong       m_master_reclength; 

  
  uint32    m_bitbuf[128/(sizeof(uint32)*8)];
  uint32    m_bitbuf_ai[128/(sizeof(uint32)*8)];

  uchar    *m_rows_buf;		
  uchar    *m_rows_cur;		
  uchar    *m_rows_end;		

  flag_set m_flags;		

  

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  const uchar *m_curr_row;     
  const uchar *m_curr_row_end; 
  uchar    *m_key;      

  int find_row(rpl_group_info *);
  int write_row(rpl_group_info *, const bool);

  
  int unpack_current_row(rpl_group_info *rgi)
  { 
    DBUG_ASSERT(m_table);
    ASSERT_OR_RETURN_ERROR(m_curr_row < m_rows_end, HA_ERR_CORRUPT_EVENT);
    return ::unpack_row(rgi, m_table, m_width, m_curr_row, &m_cols,
                                   &m_curr_row_end, &m_master_reclength, m_rows_end);
  }
#endif

private:

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);

  
  virtual 
  int do_before_row_operations(const Slave_reporting_capability *const log) = 0;

  
  virtual 
  int do_after_row_operations(const Slave_reporting_capability *const log,
                              int error) = 0;

  
  virtual int do_exec_row(rpl_group_info *rgi) = 0;
#endif 

  
 protected:
  
#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)

  int do_apply_event(Old_rows_log_event*, rpl_group_info *rgi);

  
  virtual int do_before_row_operations(TABLE *table) = 0;

  
  virtual int do_after_row_operations(TABLE *table, int error) = 0;

  
  virtual int do_prepare_row(THD*, rpl_group_info*, TABLE*,
                             uchar const *row_start,
                             uchar const **row_end) = 0;

  
  virtual int do_exec_row(TABLE *table) = 0;

#endif 
};



class Write_rows_log_event_old : public Old_rows_log_event
{
  
public:
#if !defined(MYSQL_CLIENT)
  Write_rows_log_event_old(THD*, TABLE*, ulong table_id,
                           MY_BITMAP const *cols, bool is_transactional);
#endif
#ifdef HAVE_REPLICATION
  Write_rows_log_event_old(const char *buf, uint event_len,
                           const Format_description_log_event *description_event);
#endif
#if !defined(MYSQL_CLIENT) 
  static bool binlog_row_logging_function(THD *thd, TABLE *table,
                                          bool is_transactional,
                                          const uchar *before_record
                                          __attribute__((unused)),
                                          const uchar *after_record)
  {
    return thd->binlog_write_row(table, is_transactional, after_record);
  }
#endif

private:
#ifdef MYSQL_CLIENT
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  virtual int do_before_row_operations(const Slave_reporting_capability *const);
  virtual int do_after_row_operations(const Slave_reporting_capability *const,int);
  virtual int do_exec_row(rpl_group_info *);
#endif
  

public:
  enum
  {
    
    TYPE_CODE = PRE_GA_WRITE_ROWS_EVENT
  };

private:
  virtual Log_event_type get_type_code() { return (Log_event_type)TYPE_CODE; }

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  
  virtual int do_apply_event(rpl_group_info *rgi)
  { return Old_rows_log_event::do_apply_event(this, rgi); }

  
  virtual int do_before_row_operations(TABLE *table);
  virtual int do_after_row_operations(TABLE *table, int error);
  virtual int do_prepare_row(THD*, rpl_group_info*, TABLE*,
                             uchar const *row_start, uchar const **row_end);
  virtual int do_exec_row(TABLE *table);

#endif
};



class Update_rows_log_event_old : public Old_rows_log_event
{
  
public:
#ifndef MYSQL_CLIENT
  Update_rows_log_event_old(THD*, TABLE*, ulong table_id,
                            MY_BITMAP const *cols,
                            bool is_transactional);
#endif

#ifdef HAVE_REPLICATION
  Update_rows_log_event_old(const char *buf, uint event_len,
                            const Format_description_log_event *description_event);
#endif

#if !defined(MYSQL_CLIENT) 
  static bool binlog_row_logging_function(THD *thd, TABLE *table,
                                          bool is_transactional,
                                          MY_BITMAP *cols,
                                          uint fields,
                                          const uchar *before_record,
                                          const uchar *after_record)
  {
    return thd->binlog_update_row(table, is_transactional,
                                  before_record, after_record);
  }
#endif

protected:
#ifdef MYSQL_CLIENT
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  virtual int do_before_row_operations(const Slave_reporting_capability *const);
  virtual int do_after_row_operations(const Slave_reporting_capability *const,int);
  virtual int do_exec_row(rpl_group_info *);
#endif 
  

  uchar *m_after_image, *m_memory;
  
public:
  enum 
  {
    
    TYPE_CODE = PRE_GA_UPDATE_ROWS_EVENT
  };

private:
  virtual Log_event_type get_type_code() { return (Log_event_type)TYPE_CODE; }

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  
  virtual int do_apply_event(rpl_group_info *rgi)
  { return Old_rows_log_event::do_apply_event(this, rgi); }

  
  virtual int do_before_row_operations(TABLE *table);
  virtual int do_after_row_operations(TABLE *table, int error);
  virtual int do_prepare_row(THD*, rpl_group_info*, TABLE*,
                             uchar const *row_start, uchar const **row_end);
  virtual int do_exec_row(TABLE *table);
#endif 
};



class Delete_rows_log_event_old : public Old_rows_log_event
{
  
public:
#ifndef MYSQL_CLIENT
  Delete_rows_log_event_old(THD*, TABLE*, ulong,
                            MY_BITMAP const *cols, bool is_transactional);
#endif
#ifdef HAVE_REPLICATION
  Delete_rows_log_event_old(const char *buf, uint event_len,
                            const Format_description_log_event *description_event);
#endif
#if !defined(MYSQL_CLIENT) 
  static bool binlog_row_logging_function(THD *thd, TABLE *table,
                                          bool is_transactional,
                                          MY_BITMAP *cols,
                                          uint fields,
                                          const uchar *before_record,
                                          const uchar *after_record
                                          __attribute__((unused)))
  {
    return thd->binlog_delete_row(table, is_transactional, before_record);
  }
#endif
  
protected:
#ifdef MYSQL_CLIENT
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  virtual int do_before_row_operations(const Slave_reporting_capability *const);
  virtual int do_after_row_operations(const Slave_reporting_capability *const,int);
  virtual int do_exec_row(rpl_group_info *);
#endif
  

  uchar *m_after_image, *m_memory;
 
public:
  enum 
  {
    
    TYPE_CODE = PRE_GA_DELETE_ROWS_EVENT
  };

private:
  virtual Log_event_type get_type_code() { return (Log_event_type)TYPE_CODE; }

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  
  virtual int do_apply_event(rpl_group_info *rgi)
  { return Old_rows_log_event::do_apply_event(this, rgi); }

  
  virtual int do_before_row_operations(TABLE *table);
  virtual int do_after_row_operations(TABLE *table, int error);
  virtual int do_prepare_row(THD*, rpl_group_info*, TABLE*,
                             uchar const *row_start, uchar const **row_end);
  virtual int do_exec_row(TABLE *table);
#endif
};


#endif
