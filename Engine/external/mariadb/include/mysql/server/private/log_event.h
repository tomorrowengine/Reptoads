




#ifndef _log_event_h
#define _log_event_h

#if defined(USE_PRAGMA_INTERFACE) && defined(MYSQL_SERVER)
#pragma interface			
#endif

#include <my_bitmap.h>
#include "rpl_constants.h"

#ifdef MYSQL_CLIENT
#include "sql_const.h"
#include "rpl_utility.h"
#include "hash.h"
#include "rpl_tblmap.h"
#include "sql_string.h"
#endif

#ifdef MYSQL_SERVER
#include "rpl_record.h"
#include "rpl_reporting.h"
#include "sql_class.h"                          
#endif

#include "rpl_gtid.h"


#ifndef MYSQL_CLIENT
class String;
#endif

#define PREFIX_SQL_LOAD "SQL_LOAD-"
#define LONG_FIND_ROW_THRESHOLD 60 


#ifdef DBUG_OFF
#define ASSERT_OR_RETURN_ERROR(COND, ERRNO) \
  do { if (!(COND)) return ERRNO; } while (0)
#else
#define ASSERT_OR_RETURN_ERROR(COND, ERRNO) \
  DBUG_ASSERT(COND)
#endif

#define LOG_READ_EOF    -1
#define LOG_READ_BOGUS  -2
#define LOG_READ_IO     -3
#define LOG_READ_MEM    -5
#define LOG_READ_TRUNC  -6
#define LOG_READ_TOO_LARGE -7
#define LOG_READ_CHECKSUM_FAILURE -8
#define LOG_READ_DECRYPT -9

#define LOG_EVENT_OFFSET 4


#define BINLOG_VERSION    4


#define ST_SERVER_VER_LEN 50




#define DUMPFILE_FLAG		0x1
#define OPT_ENCLOSED_FLAG	0x2
#define REPLACE_FLAG		0x4
#define IGNORE_FLAG		0x8

#define FIELD_TERM_EMPTY	0x1
#define ENCLOSED_EMPTY		0x2
#define LINE_TERM_EMPTY		0x4
#define LINE_START_EMPTY	0x8
#define ESCAPED_EMPTY		0x10

#define NUM_LOAD_DELIM_STRS 5



#define LOG_EVENT_HEADER_LEN 19     
#define OLD_HEADER_LEN       13     

#define LOG_EVENT_MINIMAL_HEADER_LEN 19



#define QUERY_HEADER_MINIMAL_LEN     (4 + 4 + 1 + 2)

#define QUERY_HEADER_LEN     (QUERY_HEADER_MINIMAL_LEN + 2)
#define STOP_HEADER_LEN      0
#define LOAD_HEADER_LEN      (4 + 4 + 4 + 1 +1 + 4)
#define SLAVE_HEADER_LEN     0
#define START_V3_HEADER_LEN     (2 + ST_SERVER_VER_LEN + 4)
#define ROTATE_HEADER_LEN    8 
#define INTVAR_HEADER_LEN      0
#define CREATE_FILE_HEADER_LEN 4
#define APPEND_BLOCK_HEADER_LEN 4
#define EXEC_LOAD_HEADER_LEN   4
#define DELETE_FILE_HEADER_LEN 4
#define NEW_LOAD_HEADER_LEN    LOAD_HEADER_LEN
#define RAND_HEADER_LEN        0
#define USER_VAR_HEADER_LEN    0
#define FORMAT_DESCRIPTION_HEADER_LEN (START_V3_HEADER_LEN+1+LOG_EVENT_TYPES)
#define XID_HEADER_LEN         0
#define BEGIN_LOAD_QUERY_HEADER_LEN APPEND_BLOCK_HEADER_LEN
#define ROWS_HEADER_LEN_V1     8
#define TABLE_MAP_HEADER_LEN   8
#define EXECUTE_LOAD_QUERY_EXTRA_HEADER_LEN (4 + 4 + 4 + 1)
#define EXECUTE_LOAD_QUERY_HEADER_LEN  (QUERY_HEADER_LEN + EXECUTE_LOAD_QUERY_EXTRA_HEADER_LEN)
#define INCIDENT_HEADER_LEN    2
#define HEARTBEAT_HEADER_LEN   0
#define IGNORABLE_HEADER_LEN   0
#define ROWS_HEADER_LEN_V2    10
#define ANNOTATE_ROWS_HEADER_LEN  0
#define BINLOG_CHECKPOINT_HEADER_LEN 4
#define GTID_HEADER_LEN       19
#define GTID_LIST_HEADER_LEN   4
#define START_ENCRYPTION_HEADER_LEN 0


#define MAX_SIZE_LOG_EVENT_STATUS (1 + 4             + \
                                   1 + 8           + \
                                   1 + 1 + 255     + \
                                   1 + 4           + \
                                   1 + 6           + \
                                   1 + 1 + 255     + \
                                   1 + 2           + \
                                   1 + 2           + \
                                   1 + 8           + \
                                   1 + 4           + \
                                   1 + 3           + \
                                   1 + 16 + 1 + 60)
#define MAX_LOG_EVENT_HEADER   (  \
  LOG_EVENT_HEADER_LEN +  \
  QUERY_HEADER_LEN     +    \
  EXECUTE_LOAD_QUERY_EXTRA_HEADER_LEN +  \
  MAX_SIZE_LOG_EVENT_STATUS +  \
  NAME_LEN + 1)


#define MAX_MAX_ALLOWED_PACKET (1024*1024*1024)



#define EVENT_TYPE_OFFSET    4
#define SERVER_ID_OFFSET     5
#define EVENT_LEN_OFFSET     9
#define LOG_POS_OFFSET       13
#define FLAGS_OFFSET         17



#define ST_BINLOG_VER_OFFSET  0
#define ST_SERVER_VER_OFFSET  2
#define ST_CREATED_OFFSET     (ST_SERVER_VER_OFFSET + ST_SERVER_VER_LEN)
#define ST_COMMON_HEADER_LEN_OFFSET (ST_CREATED_OFFSET + 4)



#define SL_MASTER_PORT_OFFSET   8
#define SL_MASTER_POS_OFFSET    0
#define SL_MASTER_HOST_OFFSET   10



#define Q_THREAD_ID_OFFSET	0
#define Q_EXEC_TIME_OFFSET	4
#define Q_DB_LEN_OFFSET		8
#define Q_ERR_CODE_OFFSET	9
#define Q_STATUS_VARS_LEN_OFFSET 11
#define Q_DATA_OFFSET		QUERY_HEADER_LEN

#define Q_FLAGS2_CODE           0
#define Q_SQL_MODE_CODE         1

#define Q_CATALOG_CODE          2
#define Q_AUTO_INCREMENT	3
#define Q_CHARSET_CODE          4
#define Q_TIME_ZONE_CODE        5

#define Q_CATALOG_NZ_CODE       6

#define Q_LC_TIME_NAMES_CODE    7

#define Q_CHARSET_DATABASE_CODE 8

#define Q_TABLE_MAP_FOR_UPDATE_CODE 9

#define Q_MASTER_DATA_WRITTEN_CODE 10

#define Q_INVOKER 11

#define Q_HRNOW 128




#define I_TYPE_OFFSET        0
#define I_VAL_OFFSET         1


#define RAND_SEED1_OFFSET 0
#define RAND_SEED2_OFFSET 8


#define UV_VAL_LEN_SIZE        4
#define UV_VAL_IS_NULL         1
#define UV_VAL_TYPE_SIZE       1
#define UV_NAME_LEN_SIZE       4
#define UV_CHARSET_NUMBER_SIZE 4


#define L_THREAD_ID_OFFSET   0
#define L_EXEC_TIME_OFFSET   4
#define L_SKIP_LINES_OFFSET  8
#define L_TBL_LEN_OFFSET     12
#define L_DB_LEN_OFFSET      13
#define L_NUM_FIELDS_OFFSET  14
#define L_SQL_EX_OFFSET      18
#define L_DATA_OFFSET        LOAD_HEADER_LEN


#define R_POS_OFFSET       0
#define R_IDENT_OFFSET     8




#define CF_FILE_ID_OFFSET  0
#define CF_DATA_OFFSET     CREATE_FILE_HEADER_LEN


#define AB_FILE_ID_OFFSET  0
#define AB_DATA_OFFSET     APPEND_BLOCK_HEADER_LEN


#define EL_FILE_ID_OFFSET  0


#define DF_FILE_ID_OFFSET  0


#define TM_MAPID_OFFSET    0
#define TM_FLAGS_OFFSET    6


#define RW_MAPID_OFFSET    0
#define RW_FLAGS_OFFSET    6
#define RW_VHLEN_OFFSET    8
#define RW_V_TAG_LEN       1
#define RW_V_EXTRAINFO_TAG 0


#define ELQ_FILE_ID_OFFSET QUERY_HEADER_LEN
#define ELQ_FN_POS_START_OFFSET ELQ_FILE_ID_OFFSET + 4
#define ELQ_FN_POS_END_OFFSET ELQ_FILE_ID_OFFSET + 8
#define ELQ_DUP_HANDLING_OFFSET ELQ_FILE_ID_OFFSET + 12


#define BINLOG_MAGIC        (const uchar*) "\xfe\x62\x69\x6e"


#ifdef TO_BE_REMOVED
#define LOG_EVENT_TIME_F            0x1
#define LOG_EVENT_FORCED_ROTATE_F   0x2
#endif



#define LOG_EVENT_BINLOG_IN_USE_F       0x1


#define LOG_EVENT_THREAD_SPECIFIC_F 0x4


#define LOG_EVENT_SUPPRESS_USE_F    0x8




#define LOG_EVENT_ARTIFICIAL_F 0x20


#define LOG_EVENT_RELAY_LOG_F 0x40


#define LOG_EVENT_IGNORABLE_F 0x80


#define LOG_EVENT_SKIP_REPLICATION_F 0x8000



#define OPTIONS_WRITTEN_TO_BIN_LOG \
  (OPTION_AUTO_IS_NULL | OPTION_NO_FOREIGN_KEY_CHECKS |  \
   OPTION_RELAXED_UNIQUE_CHECKS | OPTION_NOT_AUTOCOMMIT)


#define EXPECTED_OPTIONS \
  ((1ULL << 14) | (1ULL << 26) | (1ULL << 27) | (1ULL << 19))

#if OPTIONS_WRITTEN_TO_BIN_LOG != EXPECTED_OPTIONS
#error OPTIONS_WRITTEN_TO_BIN_LOG must NOT change their values!
#endif
#undef EXPECTED_OPTIONS         

#define CHECKSUM_CRC32_SIGNATURE_LEN 4

#define BINLOG_CHECKSUM_LEN CHECKSUM_CRC32_SIGNATURE_LEN
#define BINLOG_CHECKSUM_ALG_DESC_LEN 1  




#define MARIA_SLAVE_CAPABILITY_UNKNOWN 0

#define MARIA_SLAVE_CAPABILITY_ANNOTATE 1

#define MARIA_SLAVE_CAPABILITY_TOLERATE_HOLES 2

#define MARIA_SLAVE_CAPABILITY_BINLOG_CHECKPOINT 3

#define MARIA_SLAVE_CAPABILITY_GTID 4


#define MARIA_SLAVE_CAPABILITY_MINE MARIA_SLAVE_CAPABILITY_GTID



enum Log_event_type
{
  
  UNKNOWN_EVENT= 0,
  START_EVENT_V3= 1,
  QUERY_EVENT= 2,
  STOP_EVENT= 3,
  ROTATE_EVENT= 4,
  INTVAR_EVENT= 5,
  LOAD_EVENT= 6,
  SLAVE_EVENT= 7,
  CREATE_FILE_EVENT= 8,
  APPEND_BLOCK_EVENT= 9,
  EXEC_LOAD_EVENT= 10,
  DELETE_FILE_EVENT= 11,
  
  NEW_LOAD_EVENT= 12,
  RAND_EVENT= 13,
  USER_VAR_EVENT= 14,
  FORMAT_DESCRIPTION_EVENT= 15,
  XID_EVENT= 16,
  BEGIN_LOAD_QUERY_EVENT= 17,
  EXECUTE_LOAD_QUERY_EVENT= 18,

  TABLE_MAP_EVENT = 19,

  
  PRE_GA_WRITE_ROWS_EVENT = 20,
  PRE_GA_UPDATE_ROWS_EVENT = 21,
  PRE_GA_DELETE_ROWS_EVENT = 22,

  
  WRITE_ROWS_EVENT_V1 = 23,
  UPDATE_ROWS_EVENT_V1 = 24,
  DELETE_ROWS_EVENT_V1 = 25,

  
  INCIDENT_EVENT= 26,

  
  HEARTBEAT_LOG_EVENT= 27,
  
  
  IGNORABLE_LOG_EVENT= 28,
  ROWS_QUERY_LOG_EVENT= 29,
 
  
  WRITE_ROWS_EVENT = 30,
  UPDATE_ROWS_EVENT = 31,
  DELETE_ROWS_EVENT = 32,
 
  
  GTID_LOG_EVENT= 33,
  ANONYMOUS_GTID_LOG_EVENT= 34,
  PREVIOUS_GTIDS_LOG_EVENT= 35,

  
  TRANSACTION_CONTEXT_EVENT= 36,
  VIEW_CHANGE_EVENT= 37,
  XA_PREPARE_LOG_EVENT= 38,

  

  
  MYSQL_EVENTS_END,

  MARIA_EVENTS_BEGIN= 160,
  
  ANNOTATE_ROWS_EVENT= 160,
  
  BINLOG_CHECKPOINT_EVENT= 161,
  
  GTID_EVENT= 162,
  
  GTID_LIST_EVENT= 163,

  START_ENCRYPTION_EVENT= 164,

  
  QUERY_COMPRESSED_EVENT = 165,
  WRITE_ROWS_COMPRESSED_EVENT_V1 = 166,
  UPDATE_ROWS_COMPRESSED_EVENT_V1 = 167,
  DELETE_ROWS_COMPRESSED_EVENT_V1 = 168,
  WRITE_ROWS_COMPRESSED_EVENT = 169,
  UPDATE_ROWS_COMPRESSED_EVENT = 170,
  DELETE_ROWS_COMPRESSED_EVENT = 171,

  

  ENUM_END_EVENT 
};




enum enum_logged_status
{
  LOGGED_TABLE_MAP= 1,
  LOGGED_ROW_EVENT= 2,
  LOGGED_NO_DATA=   4,
  LOGGED_CRITICAL=  8
};

static inline bool LOG_EVENT_IS_QUERY(enum Log_event_type type)
{
  return type == QUERY_EVENT || type == QUERY_COMPRESSED_EVENT;
}


static inline bool LOG_EVENT_IS_WRITE_ROW(enum Log_event_type type)
{
  return type == WRITE_ROWS_EVENT || type == WRITE_ROWS_EVENT_V1 ||
    type == WRITE_ROWS_COMPRESSED_EVENT ||
    type == WRITE_ROWS_COMPRESSED_EVENT_V1;
}


static inline bool LOG_EVENT_IS_UPDATE_ROW(enum Log_event_type type)
{
  return type == UPDATE_ROWS_EVENT || type == UPDATE_ROWS_EVENT_V1 ||
    type == UPDATE_ROWS_COMPRESSED_EVENT ||
    type == UPDATE_ROWS_COMPRESSED_EVENT_V1;
}


static inline bool LOG_EVENT_IS_DELETE_ROW(enum Log_event_type type)
{
  return type == DELETE_ROWS_EVENT || type == DELETE_ROWS_EVENT_V1 ||
    type == DELETE_ROWS_COMPRESSED_EVENT ||
    type == DELETE_ROWS_COMPRESSED_EVENT_V1;
}


static inline bool LOG_EVENT_IS_ROW_COMPRESSED(enum Log_event_type type)
{
  return type == WRITE_ROWS_COMPRESSED_EVENT ||
    type == WRITE_ROWS_COMPRESSED_EVENT_V1 ||
    type == UPDATE_ROWS_COMPRESSED_EVENT ||
    type == UPDATE_ROWS_COMPRESSED_EVENT_V1 ||
    type == DELETE_ROWS_COMPRESSED_EVENT ||
    type == DELETE_ROWS_COMPRESSED_EVENT_V1;
}


static inline bool LOG_EVENT_IS_ROW_V2(enum Log_event_type type)
{
  return (type >= WRITE_ROWS_EVENT && type <= DELETE_ROWS_EVENT) ||
    (type >= WRITE_ROWS_COMPRESSED_EVENT && type <= DELETE_ROWS_COMPRESSED_EVENT);
}



#define LOG_EVENT_TYPES (ENUM_END_EVENT-1)

enum Int_event_type
{
  INVALID_INT_EVENT = 0, LAST_INSERT_ID_EVENT = 1, INSERT_ID_EVENT = 2
};


#ifdef MYSQL_SERVER
class String;
class MYSQL_BIN_LOG;
class THD;
#endif

class Format_description_log_event;
class Relay_log_info;
class binlog_cache_data;

bool copy_event_cache_to_file_and_reinit(IO_CACHE *cache, FILE *file);

#ifdef MYSQL_CLIENT
enum enum_base64_output_mode {
  BASE64_OUTPUT_NEVER= 0,
  BASE64_OUTPUT_AUTO= 1,
  BASE64_OUTPUT_ALWAYS= 2,
  BASE64_OUTPUT_UNSPEC= 3,
  BASE64_OUTPUT_DECODE_ROWS= 4,
  
  BASE64_OUTPUT_MODE_COUNT
};

bool copy_event_cache_to_string_and_reinit(IO_CACHE *cache, LEX_STRING *to);


typedef struct st_print_event_info
{
  
  char db[FN_REFLEN+1]; 
  char charset[6]; 
  char time_zone_str[MAX_TIME_ZONE_NAME_LENGTH];
  char delimiter[16];
  sql_mode_t sql_mode;		
  my_thread_id thread_id;
  ulonglong row_events;
  ulong auto_increment_increment, auto_increment_offset;
  uint lc_time_names_number;
  uint charset_database_number;
  uint verbose;
  uint32 flags2;
  uint32 server_id;
  uint32 domain_id;
  uint8 common_header_len;
  enum_base64_output_mode base64_output_mode;
  my_off_t hexdump_from;

  table_mapping m_table_map;
  table_mapping m_table_map_ignored;
  bool flags2_inited;
  bool sql_mode_inited;
  bool charset_inited;
  bool thread_id_printed;
  bool server_id_printed;
  bool domain_id_printed;
  bool allow_parallel;
  bool allow_parallel_printed;
  bool found_row_event;
  bool print_row_count;
  static const uint max_delimiter_size= 16;
  
  bool short_form;
  
  bool printed_fd_event;
  
  bool skip_replication;

  
  IO_CACHE head_cache;
  IO_CACHE body_cache;
#ifdef WHEN_FLASHBACK_REVIEW_READY
  
  IO_CACHE review_sql_cache;
#endif
  FILE *file;
  st_print_event_info();

  ~st_print_event_info() {
    close_cached_file(&head_cache);
    close_cached_file(&body_cache);
#ifdef WHEN_FLASHBACK_REVIEW_READY
    close_cached_file(&review_sql_cache);
#endif
  }
  bool init_ok() 
    { return my_b_inited(&head_cache) && my_b_inited(&body_cache)
#ifdef WHEN_FLASHBACK_REVIEW_READY
      && my_b_inited(&review_sql_cache)
#endif
    ; }
  void flush_for_error()
  {
    if (!copy_event_cache_to_file_and_reinit(&head_cache, file))
      copy_event_cache_to_file_and_reinit(&body_cache, file);
    fflush(file);
  }
} PRINT_EVENT_INFO;
#endif



class Log_event_writer
{
public:
  ulonglong bytes_written;
  void *ctx;         
  uint checksum_len;
  int write(Log_event *ev);
  int write_header(uchar *pos, size_t len);
  int write_data(const uchar *pos, size_t len);
  int write_footer();
  my_off_t pos() { return my_b_safe_tell(file); }
  void add_status(enum_logged_status status);

  Log_event_writer(IO_CACHE *file_arg, binlog_cache_data *cache_data_arg,
                   Binlog_crypt_data *cr= 0)
  : bytes_written(0), ctx(0),
    file(file_arg), cache_data(cache_data_arg), crypto(cr) { }

private:
  IO_CACHE *file;
  binlog_cache_data *cache_data;
  
  ha_checksum crc;
  
  Binlog_crypt_data *crypto;
  
  uint event_len;
  int write_internal(const uchar *pos, size_t len);
  int encrypt_and_write(const uchar *pos, size_t len);
  int maybe_write_event_len(uchar *pos, size_t len);
};


typedef struct event_coordinates
{
  char * file_name; 
  my_off_t  pos;       
} LOG_POS_COORD;


class Log_event
{
public:
  
  enum enum_skip_reason {
    
    EVENT_SKIP_NOT,

    
    EVENT_SKIP_IGNORE,

    
    EVENT_SKIP_COUNT
  };

  enum enum_event_cache_type 
  {
    EVENT_INVALID_CACHE,
    
    EVENT_STMT_CACHE,
    
    EVENT_TRANSACTIONAL_CACHE,
    
    EVENT_NO_CACHE,
    
    EVENT_CACHE_COUNT
  };

  
  typedef unsigned char Byte;

  
  my_off_t log_pos;
  
  char *temp_buf;
  
  
  bool event_owns_temp_buf;

  
  my_time_t when;
  ulong     when_sec_part;
  
  ulong exec_time;
  
  size_t data_written;

  
  uint32 server_id;

  
  uint16 flags;

  uint16 cache_type;

  
  ulong slave_exec_mode;

  Log_event_writer *writer;

#ifdef MYSQL_SERVER
  THD* thd;

  Log_event();
  Log_event(THD* thd_arg, uint16 flags_arg, bool is_transactional);

  
  static void init_show_field_list(THD *thd, List<Item>* field_list);
#ifdef HAVE_REPLICATION
  int net_send(Protocol *protocol, const char* log_name, my_off_t pos);

  

  virtual void pack_info(Protocol *protocol);

#endif 
  virtual const char* get_db()
  {
    return thd ? thd->db.str : 0;
  }
#else
  Log_event() : temp_buf(0), when(0), flags(0) {}
  ha_checksum crc;
  
  virtual bool print(FILE* file, PRINT_EVENT_INFO* print_event_info) = 0;
  bool print_timestamp(IO_CACHE* file, time_t *ts = 0);
  bool print_header(IO_CACHE* file, PRINT_EVENT_INFO* print_event_info,
                    bool is_more);
  bool print_base64(IO_CACHE* file, PRINT_EVENT_INFO* print_event_info,
                    bool do_print_encoded);
#endif 

  
#ifdef MYSQL_CLIENT
  my_bool is_flashback;
  my_bool need_flashback_review;
  String  output_buf; 
#ifdef WHEN_FLASHBACK_REVIEW_READY
  String  m_review_dbname;
  String  m_review_tablename;

  void set_review_dbname(const char *name)
  {
    if (name)
    {
      m_review_dbname.free();
      m_review_dbname.append(name);
    }
  }
  void set_review_tablename(const char *name)
  {
    if (name)
    {
      m_review_tablename.free();
      m_review_tablename.append(name);
    }
  }
  const char *get_review_dbname() const { return m_review_dbname.ptr(); }
  const char *get_review_tablename() const { return m_review_tablename.ptr(); }
#endif
#endif

  
  static Log_event* read_log_event(IO_CACHE* file,
                                   const Format_description_log_event
                                   *description_event,
                                   my_bool crc_check);

  
  static int read_log_event(IO_CACHE* file, String* packet,
                            const Format_description_log_event *fdle,
                            enum enum_binlog_checksum_alg checksum_alg_arg);
  
  enum enum_binlog_checksum_alg checksum_alg;

  static void *operator new(size_t size)
  {
    return (void*) my_malloc((uint)size, MYF(MY_WME|MY_FAE));
  }

  static void operator delete(void *ptr, size_t)
  {
    my_free(ptr);
  }

  
  static void *operator new(size_t, void* ptr) { return ptr; }
  static void operator delete(void*, void*) { }

#ifdef MYSQL_SERVER
  bool write_header(size_t event_data_length);
  bool write_data(const uchar *buf, size_t data_length)
  { return writer->write_data(buf, data_length); }
  bool write_data(const char *buf, size_t data_length)
  { return write_data((uchar*)buf, data_length); }
  bool write_footer()
  { return writer->write_footer(); }

  my_bool need_checksum();

  virtual bool write()
  {
    return write_header(get_data_size()) || write_data_header() ||
	   write_data_body() || write_footer();
  }
  virtual bool write_data_header()
  { return 0; }
  virtual bool write_data_body()
  { return 0; }

  
  inline my_time_t get_time()
  {
    THD *tmp_thd;
    if (when)
      return when;
    if (thd)
    {
      when= thd->start_time;
      when_sec_part= thd->start_time_sec_part;
      return when;
    }
    
    if ((tmp_thd= current_thd))
    {
      when= tmp_thd->start_time;
      when_sec_part= tmp_thd->start_time_sec_part;
      return when;
    }
    my_hrtime_t hrtime= my_hrtime();
    when= hrtime_to_my_time(hrtime);
    when_sec_part= hrtime_sec_part(hrtime);
    return when;
  }
#endif
  virtual Log_event_type get_type_code() = 0;
  virtual enum_logged_status logged_status() { return LOGGED_CRITICAL; }
  virtual bool is_valid() const = 0;
  virtual my_off_t get_header_len(my_off_t len) { return len; }
  void set_artificial_event() { flags |= LOG_EVENT_ARTIFICIAL_F; }
  void set_relay_log_event() { flags |= LOG_EVENT_RELAY_LOG_F; }
  bool is_artificial_event() const { return flags & LOG_EVENT_ARTIFICIAL_F; }
  bool is_relay_log_event() const { return flags & LOG_EVENT_RELAY_LOG_F; }
  inline bool use_trans_cache() const
  { 
    return (cache_type == Log_event::EVENT_TRANSACTIONAL_CACHE);
  }
  inline void set_direct_logging()
  {
    cache_type = Log_event::EVENT_NO_CACHE;
  }
  inline bool use_direct_logging()
  {
    return (cache_type == Log_event::EVENT_NO_CACHE);
  }
  Log_event(const char* buf, const Format_description_log_event
            *description_event);
  virtual ~Log_event() { free_temp_buf();}
  void register_temp_buf(char* buf, bool must_free) 
  { 
    temp_buf= buf; 
    event_owns_temp_buf= must_free;
  }
  void free_temp_buf()
  {
    if (temp_buf)
    {
      if (event_owns_temp_buf)
        my_free(temp_buf);
      temp_buf = 0;
    }
  }
  
  virtual int get_data_size() { return 0;}
  static Log_event* read_log_event(const char* buf, uint event_len,
				   const char **error,
                                   const Format_description_log_event
                                   *description_event, my_bool crc_check);
  
  static const char* get_type_str(Log_event_type type);
  
  const char* get_type_str();

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)

  
  int apply_event(rpl_group_info *rgi)
  {
    int res;
    THD_STAGE_INFO(thd, stage_apply_event);
    res= do_apply_event(rgi);
    THD_STAGE_INFO(thd, stage_after_apply_event);
    return res;
  }


  
  int update_pos(rpl_group_info *rgi)
  {
    return do_update_pos(rgi);
  }

  
  enum_skip_reason shall_skip(rpl_group_info *rgi)
  {
    return do_shall_skip(rgi);
  }


  
  static bool is_part_of_group(enum Log_event_type ev_type)
  {
    switch (ev_type)
    {
    case GTID_EVENT:
    case INTVAR_EVENT:
    case RAND_EVENT:
    case USER_VAR_EVENT:
    case TABLE_MAP_EVENT:
    case ANNOTATE_ROWS_EVENT:
      return true;
    case DELETE_ROWS_EVENT:
    case UPDATE_ROWS_EVENT:
    case WRITE_ROWS_EVENT:
    
    default:
      return false;
    }
  }
  
  virtual bool is_part_of_group() { return 0; }

  static bool is_group_event(enum Log_event_type ev_type)
  {
    switch (ev_type)
    {
    case START_EVENT_V3:
    case STOP_EVENT:
    case ROTATE_EVENT:
    case SLAVE_EVENT:
    case FORMAT_DESCRIPTION_EVENT:
    case INCIDENT_EVENT:
    case HEARTBEAT_LOG_EVENT:
    case BINLOG_CHECKPOINT_EVENT:
    case GTID_LIST_EVENT:
    case START_ENCRYPTION_EVENT:
      return false;

    default:
      return true;
    }
  }
  
protected:

  
  enum_skip_reason continue_group(rpl_group_info *rgi);

  
  virtual int do_apply_event(rpl_group_info *rgi)
  {
    return 0;                
  }


  
  virtual int do_update_pos(rpl_group_info *rgi);


  
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};





class Query_log_event: public Log_event
{
  LEX_CSTRING user;
  LEX_CSTRING host;
protected:
  Log_event::Byte* data_buf;
public:
  const char* query;
  const char* catalog;
  const char* db;
  
  uint32 q_len;
  uint32 db_len;
  uint16 error_code;
  my_thread_id thread_id;
  
  ulong slave_proxy_id;

  

  uint catalog_len;			

  
  uint16 status_vars_len;

  
  bool flags2_inited;
  bool sql_mode_inited;
  bool charset_inited;

  uint32 flags2;
  sql_mode_t sql_mode;
  ulong auto_increment_increment, auto_increment_offset;
  char charset[6];
  uint time_zone_len; 
  const char *time_zone_str;
  uint lc_time_names_number; 
  uint charset_database_number;
  
  ulonglong table_map_for_update;
  
  uint32 master_data_written;

#ifdef MYSQL_SERVER

  Query_log_event(THD* thd_arg, const char* query_arg, size_t query_length,
                  bool using_trans, bool direct, bool suppress_use, int error);
  const char* get_db() { return db; }
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print_query_header(IO_CACHE* file, PRINT_EVENT_INFO* print_event_info);
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Query_log_event();
  Query_log_event(const char* buf, uint event_len,
                  const Format_description_log_event *description_event,
                  Log_event_type event_type);
  ~Query_log_event()
  {
    if (data_buf)
      my_free(data_buf);
  }
  Log_event_type get_type_code() { return QUERY_EVENT; }
  static int dummy_event(String *packet, ulong ev_offset, enum enum_binlog_checksum_alg checksum_alg);
  static int begin_event(String *packet, ulong ev_offset, enum enum_binlog_checksum_alg checksum_alg);
#ifdef MYSQL_SERVER
  bool write();
  virtual bool write_post_header_for_derived() { return FALSE; }
#endif
  bool is_valid() const { return query != 0; }

  
  virtual ulong get_post_header_size_for_derived() { return 0; }
  

public:        
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
  virtual int do_apply_event(rpl_group_info *rgi);

  int do_apply_event(rpl_group_info *rgi,
                       const char *query_arg,
                       uint32 q_len_arg);
  static bool peek_is_commit_rollback(const char *event_start,
                                      size_t event_len, enum enum_binlog_checksum_alg checksum_alg);
#endif 
  
  bool is_trans_keyword()
  {
    
    return !strncmp(query, "BEGIN", q_len) ||
      !strncmp(query, "COMMIT", q_len) ||
      !strncasecmp(query, "SAVEPOINT", 9) ||
      !strncasecmp(query, "ROLLBACK", 8);
  }
  virtual bool is_begin()    { return !strcmp(query, "BEGIN"); }
  virtual bool is_commit()   { return !strcmp(query, "COMMIT"); }
  virtual bool is_rollback() { return !strcmp(query, "ROLLBACK"); }
};

class Query_compressed_log_event:public Query_log_event{
protected:
  Log_event::Byte* query_buf;  
public:
  Query_compressed_log_event(const char* buf, uint event_len,
    const Format_description_log_event *description_event,
    Log_event_type event_type);
  ~Query_compressed_log_event()
  {
    if (query_buf)
      my_free(query_buf);
  }
  Log_event_type get_type_code() { return QUERY_COMPRESSED_EVENT; }

  
  virtual bool is_begin()    { return false; }
  virtual bool is_commit()   { return false; }
  virtual bool is_rollback() { return false; }
#ifdef MYSQL_SERVER
  Query_compressed_log_event(THD* thd_arg, const char* query_arg, ulong query_length,
    bool using_trans, bool direct, bool suppress_use, int error);
  virtual bool write();
#endif
};



struct sql_ex_info
{
  sql_ex_info() {}                            
  const char* field_term;
  const char* enclosed;
  const char* line_term;
  const char* line_start;
  const char* escaped;
  int cached_new_format;
  uint8 field_term_len,enclosed_len,line_term_len,line_start_len, escaped_len;
  char opt_flags;
  char empty_flags;

  
  void force_new_format() { cached_new_format = 1;}
  int data_size()
  {
    return (new_format() ?
	    field_term_len + enclosed_len + line_term_len +
	    line_start_len + escaped_len + 6 : 7);
  }
  bool write_data(Log_event_writer *writer);
  const char* init(const char* buf, const char* buf_end, bool use_new_format);
  bool new_format()
  {
    return ((cached_new_format != -1) ? cached_new_format :
	    (cached_new_format=(field_term_len > 1 ||
				enclosed_len > 1 ||
				line_term_len > 1 || line_start_len > 1 ||
				escaped_len > 1)));
  }
};


class Load_log_event: public Log_event
{
private:
protected:
  int copy_log_event(const char *buf, ulong event_len,
                     int body_offset,
                     const Format_description_log_event* description_event);

public:
  bool print_query(THD *thd, bool need_db, const char *cs, String *buf,
                   my_off_t *fn_start, my_off_t *fn_end,
                   const char *qualify_db);
  my_thread_id thread_id;
  ulong slave_proxy_id;
  uint32 table_name_len;
  
  uint32 db_len;
  uint32 fname_len;
  uint32 num_fields;
  const char* fields;
  const uchar* field_lens;
  uint32 field_block_len;

  const char* table_name;
  const char* db;
  const char* fname;
  uint32 skip_lines;
  sql_ex_info sql_ex;
  bool local_fname;
  
  bool is_concurrent;

  
  void set_fname_outside_temp_buf(const char *afname, size_t alen)
  {
    fname= afname;
    fname_len= (uint)alen;
    local_fname= TRUE;
  }
  
  int  check_fname_outside_temp_buf()
  {
    return local_fname;
  }

#ifdef MYSQL_SERVER
  String field_lens_buf;
  String fields_buf;

  Load_log_event(THD* thd, const sql_exchange* ex, const char* db_arg,
		 const char* table_name_arg,
		 List<Item>& fields_arg,
                 bool is_concurrent_arg,
                 enum enum_duplicates handle_dup, bool ignore,
		 bool using_trans);
  void set_fields(const char* db, List<Item> &fields_arg,
                  Name_resolution_context *context);
  const char* get_db() { return db; }
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info, bool commented);
#endif

  
  Load_log_event(const char* buf, uint event_len,
                 const Format_description_log_event* description_event);
  ~Load_log_event()
  {}
  Log_event_type get_type_code()
  {
    return sql_ex.new_format() ? NEW_LOAD_EVENT: LOAD_EVENT;
  }
#ifdef MYSQL_SERVER
  bool write_data_header();
  bool write_data_body();
#endif
  bool is_valid() const { return table_name != 0; }
  int get_data_size()
  {
    return (table_name_len + db_len + 2 + fname_len
	    + LOAD_HEADER_LEN
	    + sql_ex.data_size() + field_block_len + num_fields);
  }

public:        
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi)
  {
    return do_apply_event(thd->slave_net,rgi,0);
  }

  int do_apply_event(NET *net, rpl_group_info *rgi,
                     bool use_rli_only_for_errors);
#endif
};


class Start_log_event_v3: public Log_event
{
public:
  
  time_t created;
  uint16 binlog_version;
  char server_version[ST_SERVER_VER_LEN];
  
  bool dont_set_created;

#ifdef MYSQL_SERVER
  Start_log_event_v3();
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  Start_log_event_v3() {}
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Start_log_event_v3(const char* buf, uint event_len,
                     const Format_description_log_event* description_event);
  ~Start_log_event_v3() {}
  Log_event_type get_type_code() { return START_EVENT_V3;}
  my_off_t get_header_len(my_off_t l __attribute__((unused)))
  { return LOG_EVENT_MINIMAL_HEADER_LEN; }
#ifdef MYSQL_SERVER
  bool write();
#endif
  bool is_valid() const { return server_version[0] != 0; }
  int get_data_size()
  {
    return START_V3_HEADER_LEN; 
  }

protected:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info*)
  {
    
    if (this->server_id == global_system_variables.server_id)
      return Log_event::EVENT_SKIP_IGNORE;
    else
      return Log_event::EVENT_SKIP_NOT;
  }
#endif
};


class Start_encryption_log_event : public Log_event
{
public:
#ifdef MYSQL_SERVER
  Start_encryption_log_event(uint crypto_scheme_arg, uint key_version_arg,
                             const uchar* nonce_arg)
  : crypto_scheme(crypto_scheme_arg), key_version(key_version_arg)
  {
    cache_type = EVENT_NO_CACHE;
    DBUG_ASSERT(crypto_scheme == 1);
    memcpy(nonce, nonce_arg, BINLOG_NONCE_LENGTH);
  }

  bool write_data_body()
  {
    uchar scheme_buf= crypto_scheme;
    uchar key_version_buf[BINLOG_KEY_VERSION_LENGTH];
    int4store(key_version_buf, key_version);
    return write_data(&scheme_buf, sizeof(scheme_buf)) ||
           write_data(key_version_buf, sizeof(key_version_buf)) ||
           write_data(nonce, BINLOG_NONCE_LENGTH);
  }
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Start_encryption_log_event(
     const char* buf, uint event_len,
     const Format_description_log_event* description_event);

  bool is_valid() const { return crypto_scheme == 1; }

  Log_event_type get_type_code() { return START_ENCRYPTION_EVENT; }

  int get_data_size()
  {
    return BINLOG_CRYPTO_SCHEME_LENGTH + BINLOG_KEY_VERSION_LENGTH +
           BINLOG_NONCE_LENGTH;
  }

  uint crypto_scheme;
  uint key_version;
  uchar nonce[BINLOG_NONCE_LENGTH];

protected:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info* rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info* rgi)
  {
     return Log_event::EVENT_SKIP_NOT;
  }
#endif

};




class Format_description_log_event: public Start_log_event_v3
{
public:
  
  uint8 common_header_len;
  uint8 number_of_event_types;
  
  uint8 *post_header_len;
  struct master_version_split {
    enum {KIND_MYSQL, KIND_MARIADB};
    int kind;
    uchar ver[3];
  };
  master_version_split server_version_split;
  const uint8 *event_type_permutation;

  Format_description_log_event(uint8 binlog_ver, const char* server_ver=0);
  Format_description_log_event(const char* buf, uint event_len,
                               const Format_description_log_event
                               *description_event);
  ~Format_description_log_event()
  {
    my_free(post_header_len);
  }
  Log_event_type get_type_code() { return FORMAT_DESCRIPTION_EVENT;}
#ifdef MYSQL_SERVER
  bool write();
#endif
  bool header_is_valid() const
  {
    return ((common_header_len >= ((binlog_version==1) ? OLD_HEADER_LEN :
                                   LOG_EVENT_MINIMAL_HEADER_LEN)) &&
            (post_header_len != NULL));
  }

  bool version_is_valid() const
  {
    
    return !(server_version_split.ver[0] == 0 &&
             server_version_split.ver[1] == 0 &&
             server_version_split.ver[2] == 0);
  }

  bool is_valid() const
  {
    return header_is_valid() && version_is_valid();
  }

  int get_data_size()
  {
    
    return FORMAT_DESCRIPTION_HEADER_LEN;
  }

  Binlog_crypt_data crypto_data;
  bool start_decryption(Start_encryption_log_event* sele);
  void copy_crypto_data(const Format_description_log_event* o)
  {
    crypto_data= o->crypto_data;
  }
  void reset_crypto()
  {
    crypto_data.scheme= 0;
  }

  void calc_server_version_split();
  static bool is_version_before_checksum(const master_version_split *version_split);
protected:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};



class Intvar_log_event: public Log_event
{
public:
  ulonglong val;
  uchar type;

#ifdef MYSQL_SERVER
Intvar_log_event(THD* thd_arg,uchar type_arg, ulonglong val_arg,
                 bool using_trans, bool direct)
    :Log_event(thd_arg,0,using_trans),val(val_arg),type(type_arg)
  {
    if (direct)
      cache_type= Log_event::EVENT_NO_CACHE;
  }
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Intvar_log_event(const char* buf,
                   const Format_description_log_event *description_event);
  ~Intvar_log_event() {}
  Log_event_type get_type_code() { return INTVAR_EVENT;}
  const char* get_var_type_name();
  int get_data_size() { return  9; ;}
#ifdef MYSQL_SERVER
  bool write();
#endif
  bool is_valid() const { return 1; }
  bool is_part_of_group() { return 1; }

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};




class Rand_log_event: public Log_event
{
 public:
  ulonglong seed1;
  ulonglong seed2;

#ifdef MYSQL_SERVER
  Rand_log_event(THD* thd_arg, ulonglong seed1_arg, ulonglong seed2_arg,
                 bool using_trans, bool direct)
    :Log_event(thd_arg,0,using_trans),seed1(seed1_arg),seed2(seed2_arg)
  {
    if (direct)
      cache_type= Log_event::EVENT_NO_CACHE;
  }
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Rand_log_event(const char* buf,
                 const Format_description_log_event *description_event);
  ~Rand_log_event() {}
  Log_event_type get_type_code() { return RAND_EVENT;}
  int get_data_size() { return 16;  }
#ifdef MYSQL_SERVER
  bool write();
#endif
  bool is_valid() const { return 1; }
  bool is_part_of_group() { return 1; }

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};


#ifdef MYSQL_CLIENT
typedef ulonglong my_xid; 
#endif

class Xid_log_event: public Log_event
{
 public:
   my_xid xid;

#ifdef MYSQL_SERVER
  Xid_log_event(THD* thd_arg, my_xid x, bool direct):
   Log_event(thd_arg, 0, TRUE), xid(x)
   {
     if (direct)
       cache_type= Log_event::EVENT_NO_CACHE;
   }
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Xid_log_event(const char* buf,
                const Format_description_log_event *description_event);
  ~Xid_log_event() {}
  Log_event_type get_type_code() { return XID_EVENT;}
  int get_data_size() { return sizeof(xid); }
#ifdef MYSQL_SERVER
  bool write();
#endif
  bool is_valid() const { return 1; }

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};



class User_var_log_event: public Log_event
{
public:
  enum {
    UNDEF_F= 0,
    UNSIGNED_F= 1
  };
  const char *name;
  size_t name_len;
  const char *val;
  size_t val_len;
  Item_result type;
  uint charset_number;
  bool is_null;
  uchar flags;
#ifdef MYSQL_SERVER
  bool deferred;
  query_id_t query_id;
  User_var_log_event(THD* thd_arg, const char *name_arg, size_t name_len_arg,
                     const char *val_arg, size_t val_len_arg, Item_result type_arg,
		     uint charset_number_arg, uchar flags_arg,
                     bool using_trans, bool direct)
    :Log_event(thd_arg, 0, using_trans),
    name(name_arg), name_len(name_len_arg), val(val_arg),
    val_len(val_len_arg), type(type_arg), charset_number(charset_number_arg),
    flags(flags_arg), deferred(false)
    {
      is_null= !val;
      if (direct)
        cache_type= Log_event::EVENT_NO_CACHE;
    }
  void pack_info(Protocol* protocol);
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  User_var_log_event(const char* buf, uint event_len,
                     const Format_description_log_event *description_event);
  ~User_var_log_event() {}
  Log_event_type get_type_code() { return USER_VAR_EVENT;}
#ifdef MYSQL_SERVER
  bool write();
  
  bool is_deferred() { return deferred; }
  
  void set_deferred(query_id_t qid) { deferred= true; query_id= qid; }
#endif
  bool is_valid() const { return name != 0; }
  bool is_part_of_group() { return 1; }

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};



class Stop_log_event: public Log_event
{
public:
#ifdef MYSQL_SERVER
  Stop_log_event() :Log_event()
  {}
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Stop_log_event(const char* buf,
                 const Format_description_log_event *description_event):
    Log_event(buf, description_event)
  {}
  ~Stop_log_event() {}
  Log_event_type get_type_code() { return STOP_EVENT;}
  bool is_valid() const { return 1; }

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi)
  {
    
    if (this->server_id == global_system_variables.server_id)
      return Log_event::EVENT_SKIP_IGNORE;
    else
      return Log_event::EVENT_SKIP_NOT;
  }
#endif
};



class Rotate_log_event: public Log_event
{
public:
  enum {
    DUP_NAME= 2, 
    RELAY_LOG=4  
  };
  const char* new_log_ident;
  ulonglong pos;
  uint ident_len;
  uint flags;
#ifdef MYSQL_SERVER
  Rotate_log_event(const char* new_log_ident_arg,
		   uint ident_len_arg,
		   ulonglong pos_arg, uint flags);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Rotate_log_event(const char* buf, uint event_len,
                   const Format_description_log_event* description_event);
  ~Rotate_log_event()
  {
    if (flags & DUP_NAME)
      my_free((void*) new_log_ident);
  }
  Log_event_type get_type_code() { return ROTATE_EVENT;}
  my_off_t get_header_len(my_off_t l __attribute__((unused)))
  { return LOG_EVENT_MINIMAL_HEADER_LEN; }
  int get_data_size() { return  ident_len + ROTATE_HEADER_LEN;}
  bool is_valid() const { return new_log_ident != 0; }
#ifdef MYSQL_SERVER
  bool write();
#endif

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};


class Binlog_checkpoint_log_event: public Log_event
{
public:
  char *binlog_file_name;
  uint binlog_file_len;

#ifdef MYSQL_SERVER
  Binlog_checkpoint_log_event(const char *binlog_file_name_arg,
                              uint binlog_file_len_arg);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol *protocol);
#endif
#else
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif
  Binlog_checkpoint_log_event(const char *buf, uint event_len,
             const Format_description_log_event *description_event);
  ~Binlog_checkpoint_log_event() { my_free(binlog_file_name); }
  Log_event_type get_type_code() { return BINLOG_CHECKPOINT_EVENT;}
  int get_data_size() { return  binlog_file_len + BINLOG_CHECKPOINT_HEADER_LEN;}
  bool is_valid() const { return binlog_file_name != 0; }
#ifdef MYSQL_SERVER
  bool write();
  enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};




class Gtid_log_event: public Log_event
{
public:
  uint64 seq_no;
  uint64 commit_id;
  uint32 domain_id;
  uchar flags2;

  

  
  static const uchar FL_STANDALONE= 1;
  
  static const uchar FL_GROUP_COMMIT_ID= 2;
  
  static const uchar FL_TRANSACTIONAL= 4;
  
  static const uchar FL_ALLOW_PARALLEL= 8;
  
  static const uchar FL_WAITED= 16;
  
  static const uchar FL_DDL= 32;

#ifdef MYSQL_SERVER
  Gtid_log_event(THD *thd_arg, uint64 seq_no, uint32 domain_id, bool standalone,
                 uint16 flags, bool is_transactional, uint64 commit_id);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol *protocol);
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
#else
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif
  Gtid_log_event(const char *buf, uint event_len,
                 const Format_description_log_event *description_event);
  ~Gtid_log_event() { }
  Log_event_type get_type_code() { return GTID_EVENT; }
  enum_logged_status logged_status() { return LOGGED_NO_DATA; }
  int get_data_size()
  {
    return GTID_HEADER_LEN + ((flags2 & FL_GROUP_COMMIT_ID) ? 2 : 0);
  }
  bool is_valid() const { return seq_no != 0; }
#ifdef MYSQL_SERVER
  bool write();
  static int make_compatible_event(String *packet, bool *need_dummy_event,
                                    ulong ev_offset, enum enum_binlog_checksum_alg checksum_alg);
  static bool peek(const char *event_start, size_t event_len,
                   enum enum_binlog_checksum_alg checksum_alg,
                   uint32 *domain_id, uint32 *server_id, uint64 *seq_no,
                   uchar *flags2, const Format_description_log_event *fdev);
#endif
};




class Gtid_list_log_event: public Log_event
{
public:
  uint32 count;
  uint32 gl_flags;
  struct rpl_gtid *list;
  uint64 *sub_id_list;

  static const uint element_size= 4+4+8;
  
  enum gtid_flags
  {
    FLAG_UNTIL_REACHED= (1<<28),
    FLAG_IGN_GTIDS= (1<<29),
  };
#ifdef MYSQL_SERVER
  Gtid_list_log_event(rpl_binlog_state *gtid_set, uint32 gl_flags);
  Gtid_list_log_event(slave_connection_state *gtid_set, uint32 gl_flags);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol *protocol);
#endif
#else
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif
  Gtid_list_log_event(const char *buf, uint event_len,
                      const Format_description_log_event *description_event);
  ~Gtid_list_log_event() { my_free(list); my_free(sub_id_list); }
  Log_event_type get_type_code() { return GTID_LIST_EVENT; }
  int get_data_size() {
    
    return (count==0 ?
            GTID_LIST_HEADER_LEN+2 : GTID_LIST_HEADER_LEN+count*element_size);
  }
  bool is_valid() const { return list != NULL; }
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  bool to_packet(String *packet);
  bool write();
  virtual int do_apply_event(rpl_group_info *rgi);
  enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
  static bool peek(const char *event_start, size_t event_len,
                   enum enum_binlog_checksum_alg checksum_alg,
                   rpl_gtid **out_gtid_list, uint32 *out_list_len,
                   const Format_description_log_event *fdev);
};






class Create_file_log_event: public Load_log_event
{
protected:
  
  bool fake_base;
public:
  uchar* block;
  const char *event_buf;
  uint block_len;
  uint file_id;
  bool inited_from_old;

#ifdef MYSQL_SERVER
  Create_file_log_event(THD* thd, sql_exchange* ex, const char* db_arg,
			const char* table_name_arg,
			List<Item>& fields_arg,
                        bool is_concurrent_arg,
			enum enum_duplicates handle_dup, bool ignore,
			uchar* block_arg, uint block_len_arg,
			bool using_trans);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info,
             bool enable_local);
#endif

  Create_file_log_event(const char* buf, uint event_len,
                        const Format_description_log_event* description_event);
  ~Create_file_log_event()
  {
    my_free((void*) event_buf);
  }

  Log_event_type get_type_code()
  {
    return fake_base ? Load_log_event::get_type_code() : CREATE_FILE_EVENT;
  }
  int get_data_size()
  {
    return (fake_base ? Load_log_event::get_data_size() :
	    Load_log_event::get_data_size() +
	    4 + 1 + block_len);
  }
  bool is_valid() const { return inited_from_old || block != 0; }
#ifdef MYSQL_SERVER
  bool write_data_header();
  bool write_data_body();
  
  bool write_base();
#endif

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
#endif
};




class Append_block_log_event: public Log_event
{
public:
  uchar* block;
  uint block_len;
  uint file_id;
  
  const char* db;

#ifdef MYSQL_SERVER
  Append_block_log_event(THD* thd, const char* db_arg, uchar* block_arg,
			 uint block_len_arg, bool using_trans);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
  virtual int get_create_or_append() const;
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Append_block_log_event(const char* buf, uint event_len,
                         const Format_description_log_event
                         *description_event);
  ~Append_block_log_event() {}
  Log_event_type get_type_code() { return APPEND_BLOCK_EVENT;}
  int get_data_size() { return  block_len + APPEND_BLOCK_HEADER_LEN ;}
  bool is_valid() const { return block != 0; }
#ifdef MYSQL_SERVER
  bool write();
  const char* get_db() { return db; }
#endif

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
#endif
};




class Delete_file_log_event: public Log_event
{
public:
  uint file_id;
  const char* db; 

#ifdef MYSQL_SERVER
  Delete_file_log_event(THD* thd, const char* db_arg, bool using_trans);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info,
             bool enable_local);
#endif

  Delete_file_log_event(const char* buf, uint event_len,
                        const Format_description_log_event* description_event);
  ~Delete_file_log_event() {}
  Log_event_type get_type_code() { return DELETE_FILE_EVENT;}
  int get_data_size() { return DELETE_FILE_HEADER_LEN ;}
  bool is_valid() const { return file_id != 0; }
#ifdef MYSQL_SERVER
  bool write();
  const char* get_db() { return db; }
#endif

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
#endif
};




class Execute_load_log_event: public Log_event
{
public:
  uint file_id;
  const char* db; 

#ifdef MYSQL_SERVER
  Execute_load_log_event(THD* thd, const char* db_arg, bool using_trans);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
#endif

  Execute_load_log_event(const char* buf, uint event_len,
                         const Format_description_log_event
                         *description_event);
  ~Execute_load_log_event() {}
  Log_event_type get_type_code() { return EXEC_LOAD_EVENT;}
  int get_data_size() { return  EXEC_LOAD_HEADER_LEN ;}
  bool is_valid() const { return file_id != 0; }
#ifdef MYSQL_SERVER
  bool write();
  const char* get_db() { return db; }
#endif

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
#endif
};



class Begin_load_query_log_event: public Append_block_log_event
{
public:
#ifdef MYSQL_SERVER
  Begin_load_query_log_event(THD* thd_arg, const char *db_arg,
                             uchar* block_arg, uint block_len_arg,
                             bool using_trans);
#ifdef HAVE_REPLICATION
  Begin_load_query_log_event(THD* thd);
  int get_create_or_append() const;
#endif 
#endif
  Begin_load_query_log_event(const char* buf, uint event_len,
                             const Format_description_log_event
                             *description_event);
  ~Begin_load_query_log_event() {}
  Log_event_type get_type_code() { return BEGIN_LOAD_QUERY_EVENT; }
private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif
};



enum enum_load_dup_handling { LOAD_DUP_ERROR= 0, LOAD_DUP_IGNORE,
                              LOAD_DUP_REPLACE };


class Execute_load_query_log_event: public Query_log_event
{
public:
  uint file_id;       
  uint fn_pos_start;  
                      
  uint fn_pos_end;    
  
  enum_load_dup_handling dup_handling;

#ifdef MYSQL_SERVER
  Execute_load_query_log_event(THD* thd, const char* query_arg,
                               ulong query_length, uint fn_pos_start_arg,
                               uint fn_pos_end_arg,
                               enum_load_dup_handling dup_handling_arg,
                               bool using_trans, bool direct,
                               bool suppress_use, int errcode);
#ifdef HAVE_REPLICATION
  void pack_info(Protocol* protocol);
#endif 
#else
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
  
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info,
	     const char *local_fname);
#endif
  Execute_load_query_log_event(const char* buf, uint event_len,
                               const Format_description_log_event
                               *description_event);
  ~Execute_load_query_log_event() {}

  Log_event_type get_type_code() { return EXECUTE_LOAD_QUERY_EVENT; }
  bool is_valid() const { return Query_log_event::is_valid() && file_id != 0; }

  ulong get_post_header_size_for_derived();
#ifdef MYSQL_SERVER
  bool write_post_header_for_derived();
#endif

private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
#endif
};


#ifdef MYSQL_CLIENT

class Unknown_log_event: public Log_event
{
public:
  enum { UNKNOWN, ENCRYPTED } what;
  
  Unknown_log_event(const char* buf,
                    const Format_description_log_event *description_event):
    Log_event(buf, description_event), what(UNKNOWN)
  {}
  
  Unknown_log_event(): Log_event(), what(ENCRYPTED) {}
  ~Unknown_log_event() {}
  bool print(FILE* file, PRINT_EVENT_INFO* print_event_info);
  Log_event_type get_type_code() { return UNKNOWN_EVENT;}
  bool is_valid() const { return 1; }
};
#endif
char *str_to_hex(char *to, const char *from, size_t len);


class Annotate_rows_log_event: public Log_event
{
public:
#ifndef MYSQL_CLIENT
  Annotate_rows_log_event(THD*, bool using_trans, bool direct);
#endif
  Annotate_rows_log_event(const char *buf, uint event_len,
                          const Format_description_log_event*);
  ~Annotate_rows_log_event();

  virtual int get_data_size();
  virtual Log_event_type get_type_code();
  enum_logged_status logged_status() { return LOGGED_NO_DATA; }
  virtual bool is_valid() const;
  virtual bool is_part_of_group() { return 1; }

#ifndef MYSQL_CLIENT
  virtual bool write_data_header();
  virtual bool write_data_body();
#endif

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
  virtual void pack_info(Protocol*);
#endif

#ifdef MYSQL_CLIENT
  virtual bool print(FILE*, PRINT_EVENT_INFO*);
#endif

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
private:
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info*);
#endif

private:
  char *m_query_txt;
  uint  m_query_len;
  char *m_save_thd_query_txt;
  uint  m_save_thd_query_len;
  bool  m_saved_thd_query;
  bool  m_used_query_txt;
};


class Table_map_log_event : public Log_event
{
public:
  
  enum
  {
    TYPE_CODE = TABLE_MAP_EVENT
  };

  
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
    
    ENUM_FLAG_COUNT
  };

  typedef uint16 flag_set;

  
  enum 
  {
    TM_NO_FLAGS = 0U,
    TM_BIT_LEN_EXACT_F = (1U << 0),
    
    TM_BIT_HAS_TRIGGERS_F= (1U << 14)
  };

  flag_set get_flags(flag_set flag) const { return m_flags & flag; }

#ifdef MYSQL_SERVER
  Table_map_log_event(THD *thd, TABLE *tbl, ulong tid, bool is_transactional);
#endif
#ifdef HAVE_REPLICATION
  Table_map_log_event(const char *buf, uint event_len, 
                      const Format_description_log_event *description_event);
#endif

  ~Table_map_log_event();

#ifdef MYSQL_CLIENT
  table_def *create_table_def()
  {
    return new table_def(m_coltype, m_colcnt, m_field_metadata,
                         m_field_metadata_size, m_null_bits, m_flags);
  }
  int rewrite_db(const char* new_name, size_t new_name_len,
                 const Format_description_log_event*);
#endif
  ulonglong get_table_id() const        { return m_table_id; }
  const char *get_table_name() const { return m_tblnam; }
  const char *get_db_name() const    { return m_dbnam; }

  virtual Log_event_type get_type_code() { return TABLE_MAP_EVENT; }
  virtual enum_logged_status logged_status() { return LOGGED_TABLE_MAP; }
  virtual bool is_valid() const { return m_memory != NULL;  }
  virtual bool is_part_of_group() { return 1; }

  virtual int get_data_size() { return (uint) m_data_size; } 
#ifdef MYSQL_SERVER
  virtual int save_field_metadata();
  virtual bool write_data_header();
  virtual bool write_data_body();
  virtual const char *get_db() { return m_dbnam; }
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual void pack_info(Protocol *protocol);
#endif

#ifdef MYSQL_CLIENT
  virtual bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif


private:
#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);
#endif

#ifdef MYSQL_SERVER
  TABLE         *m_table;
#endif
  char const    *m_dbnam;
  size_t         m_dblen;
  char const    *m_tblnam;
  size_t         m_tbllen;
  ulong          m_colcnt;
  uchar         *m_coltype;

  uchar         *m_memory;
  ulonglong      m_table_id;
  flag_set       m_flags;

  size_t         m_data_size;

  uchar          *m_field_metadata;        
  
  ulong          m_field_metadata_size;   
  uchar         *m_null_bits;
  uchar         *m_meta_memory;
};





class Rows_log_event : public Log_event
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

    
    COMPLETE_ROWS_F = (1U << 3),

    
    NO_CHECK_CONSTRAINT_CHECKS_F = (1U << 7)
  };

  typedef uint16 flag_set;

  
  enum 
  {
      RLE_NO_FLAGS = 0U
  };

  virtual ~Rows_log_event();

  void set_flags(flag_set flags_arg) { m_flags |= flags_arg; }
  void clear_flags(flag_set flags_arg) { m_flags &= ~flags_arg; }
  flag_set get_flags(flag_set flags_arg) const { return m_flags & flags_arg; }
  void update_flags() { int2store(temp_buf + m_flags_pos, m_flags); }

  Log_event_type get_type_code() { return m_type; } 
  enum_logged_status logged_status() { return LOGGED_ROW_EVENT; }
  virtual Log_event_type get_general_type_code() = 0; 

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual void pack_info(Protocol *protocol);
#endif

#ifdef MYSQL_CLIENT
  
  virtual bool print(FILE *file, PRINT_EVENT_INFO *print_event_info)= 0;
  void change_to_flashback_event(PRINT_EVENT_INFO *print_event_info, uchar *rows_buff, Log_event_type ev_type);
  bool print_verbose(IO_CACHE *file,
                     PRINT_EVENT_INFO *print_event_info);
  size_t print_verbose_one_row(IO_CACHE *file, table_def *td,
                               PRINT_EVENT_INFO *print_event_info,
                               MY_BITMAP *cols_bitmap,
                               const uchar *ptr, const uchar *prefix,
                               const my_bool no_fill_output= 0); 
  size_t calc_row_event_length(table_def *td,
                               PRINT_EVENT_INFO *print_event_info,
                               MY_BITMAP *cols_bitmap,
                               const uchar *value);
  void count_row_events(PRINT_EVENT_INFO *print_event_info);

#endif

#ifdef MYSQL_SERVER
  int add_row_data(uchar *data, size_t length)
  {
    return do_add_row_data(data,length); 
  }
#endif

  
  virtual int get_data_size();

  MY_BITMAP const *get_cols() const { return &m_cols; }
  MY_BITMAP const *get_cols_ai() const { return &m_cols_ai; }
  size_t get_width() const          { return m_width; }
  ulonglong get_table_id() const        { return m_table_id; }

#if defined(MYSQL_SERVER)
  
  bool read_write_bitmaps_cmp(TABLE *table)
  {
    bool res= FALSE;

    switch (get_general_type_code())
    {
      case DELETE_ROWS_EVENT:
        res= bitmap_cmp(get_cols(), table->read_set);
        break;
      case UPDATE_ROWS_EVENT:
        res= (bitmap_cmp(get_cols(), table->read_set) &&
              bitmap_cmp(get_cols_ai(), table->rpl_write_set));
        break;
      case WRITE_ROWS_EVENT:
        res= bitmap_cmp(get_cols(), table->rpl_write_set);
        break;
      default:
        
        DBUG_ASSERT(0);
    }
    return res;
  }
#endif

#ifdef MYSQL_SERVER
  virtual bool write_data_header();
  virtual bool write_data_body();
  virtual bool write_compressed();
  virtual const char *get_db() { return m_table->s->db.str; }
#endif
  
  virtual bool is_valid() const
  {
    return m_rows_buf && m_cols.bitmap;
  }
  bool is_part_of_group() { return get_flags(STMT_END_F) != 0; }

  uint     m_row_count;         

  const uchar* get_extra_row_data() const   { return m_extra_row_data; }

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual uint8 get_trg_event_map()= 0;
#endif

protected:
  
#ifdef MYSQL_SERVER
  Rows_log_event(THD*, TABLE*, ulong table_id, 
		 MY_BITMAP const *cols, bool is_transactional,
		 Log_event_type event_type);
#endif
  Rows_log_event(const char *row_data, uint event_len, 
		 const Format_description_log_event *description_event);
  void uncompress_buf();

#ifdef MYSQL_CLIENT
  bool print_helper(FILE *, PRINT_EVENT_INFO *, char const *const name);
#endif

#ifdef MYSQL_SERVER
  virtual int do_add_row_data(uchar *data, size_t length);
#endif

#ifdef MYSQL_SERVER
  TABLE *m_table;		
#endif
  ulonglong       m_table_id;	
  MY_BITMAP   m_cols;		
  ulong       m_width;          
  
  MY_BITMAP   m_cols_ai;

  ulong       m_master_reclength; 

  
  uint32    m_bitbuf[128/(sizeof(uint32)*8)];
  uint32    m_bitbuf_ai[128/(sizeof(uint32)*8)];

  uchar    *m_rows_buf;		
  uchar    *m_rows_cur;		
  uchar    *m_rows_end;		

  size_t   m_rows_before_size;  
  size_t   m_flags_pos; 

  flag_set m_flags;		

  Log_event_type m_type;        

  uchar    *m_extra_row_data;   
                                

  bool m_vers_from_plain;


  

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  const uchar *m_curr_row;     
  const uchar *m_curr_row_end; 
  uchar    *m_key;      
  KEY      *m_key_info; 
  uint      m_key_nr;   
  bool master_had_triggers;     

  int find_key(); 
  int find_row(rpl_group_info *);
  int write_row(rpl_group_info *, const bool);
  int update_sequence();

  
  
  int unpack_current_row(rpl_group_info *rgi, MY_BITMAP const *cols)
  {
    DBUG_ASSERT(m_table);

    ASSERT_OR_RETURN_ERROR(m_curr_row <= m_rows_end, HA_ERR_CORRUPT_EVENT);
    return ::unpack_row(rgi, m_table, m_width, m_curr_row, cols,
                                   &m_curr_row_end, &m_master_reclength, m_rows_end);
  }

  
  int unpack_current_row(rpl_group_info *rgi)
  {
    DBUG_ASSERT(m_table);

    ASSERT_OR_RETURN_ERROR(m_curr_row <= m_rows_end, HA_ERR_CORRUPT_EVENT);
    return ::unpack_row(rgi, m_table, m_width, m_curr_row, &m_cols,
                                   &m_curr_row_end, &m_master_reclength, m_rows_end);
  }
  bool process_triggers(trg_event_type event,
                        trg_action_time_type time_type,
                        bool old_row_is_record1);

  
  inline bool is_auto_inc_in_extra_columns()
  {
    DBUG_ASSERT(m_table);
    return (m_table->next_number_field &&
            m_table->next_number_field->field_index >= m_width);
  }
#endif

private:

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
  virtual int do_update_pos(rpl_group_info *rgi);
  virtual enum_skip_reason do_shall_skip(rpl_group_info *rgi);

  
  virtual 
  int do_before_row_operations(const Slave_reporting_capability *const log) = 0;

  
  virtual 
  int do_after_row_operations(const Slave_reporting_capability *const log,
                              int error) = 0;

  
  virtual int do_exec_row(rpl_group_info *rli) = 0;
#endif 

  friend class Old_rows_log_event;
};


class Write_rows_log_event : public Rows_log_event
{
public:
  enum 
  {
    
    TYPE_CODE = WRITE_ROWS_EVENT
  };

#if defined(MYSQL_SERVER)
  Write_rows_log_event(THD*, TABLE*, ulong table_id,
                       bool is_transactional);
#endif
#ifdef HAVE_REPLICATION
  Write_rows_log_event(const char *buf, uint event_len, 
                       const Format_description_log_event *description_event);
#endif
#if defined(MYSQL_SERVER) 
  static bool binlog_row_logging_function(THD *thd, TABLE *table,
                                          bool is_transactional,
                                          const uchar *before_record
                                          __attribute__((unused)),
                                          const uchar *after_record)
  {
    DBUG_ASSERT(!table->versioned(VERS_TRX_ID));
    return thd->binlog_write_row(table, is_transactional, after_record);
  }
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  uint8 get_trg_event_map();
#endif

private:
  virtual Log_event_type get_general_type_code() { return (Log_event_type)TYPE_CODE; }

#ifdef MYSQL_CLIENT
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_before_row_operations(const Slave_reporting_capability *const);
  virtual int do_after_row_operations(const Slave_reporting_capability *const,int);
  virtual int do_exec_row(rpl_group_info *);
#endif
};

class Write_rows_compressed_log_event : public Write_rows_log_event
{
public:
#if defined(MYSQL_SERVER)
  Write_rows_compressed_log_event(THD*, TABLE*, ulong table_id,
                       bool is_transactional);
  virtual bool write();
#endif
#ifdef HAVE_REPLICATION
  Write_rows_compressed_log_event(const char *buf, uint event_len, 
                       const Format_description_log_event *description_event);
#endif
private:
#if defined(MYSQL_CLIENT)
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif
};


class Update_rows_log_event : public Rows_log_event
{
public:
  enum 
  {
    
    TYPE_CODE = UPDATE_ROWS_EVENT
  };

#ifdef MYSQL_SERVER
  Update_rows_log_event(THD*, TABLE*, ulong table_id,
                        bool is_transactional);

  void init(MY_BITMAP const *cols);
#endif

  virtual ~Update_rows_log_event();

#ifdef HAVE_REPLICATION
  Update_rows_log_event(const char *buf, uint event_len, 
			const Format_description_log_event *description_event);
#endif

#ifdef MYSQL_SERVER
  static bool binlog_row_logging_function(THD *thd, TABLE *table,
                                          bool is_transactional,
                                          const uchar *before_record,
                                          const uchar *after_record)
  {
    DBUG_ASSERT(!table->versioned(VERS_TRX_ID));
    return thd->binlog_update_row(table, is_transactional,
                                  before_record, after_record);
  }
#endif

  virtual bool is_valid() const
  {
    return Rows_log_event::is_valid() && m_cols_ai.bitmap;
  }

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  uint8 get_trg_event_map();
#endif

protected:
  virtual Log_event_type get_general_type_code() { return (Log_event_type)TYPE_CODE; }

#ifdef MYSQL_CLIENT
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_before_row_operations(const Slave_reporting_capability *const);
  virtual int do_after_row_operations(const Slave_reporting_capability *const,int);
  virtual int do_exec_row(rpl_group_info *);
#endif 
};

class Update_rows_compressed_log_event : public Update_rows_log_event
{
public:
#if defined(MYSQL_SERVER)
  Update_rows_compressed_log_event(THD*, TABLE*, ulong table_id,
                        bool is_transactional);
  virtual bool write();
#endif
#ifdef HAVE_REPLICATION
  Update_rows_compressed_log_event(const char *buf, uint event_len, 
                       const Format_description_log_event *description_event);
#endif
private:
#if defined(MYSQL_CLIENT)
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif
};


class Delete_rows_log_event : public Rows_log_event
{
public:
  enum 
  {
    
    TYPE_CODE = DELETE_ROWS_EVENT
  };

#ifdef MYSQL_SERVER
  Delete_rows_log_event(THD*, TABLE*, ulong, bool is_transactional);
#endif
#ifdef HAVE_REPLICATION
  Delete_rows_log_event(const char *buf, uint event_len, 
			const Format_description_log_event *description_event);
#endif
#ifdef MYSQL_SERVER
  static bool binlog_row_logging_function(THD *thd, TABLE *table,
                                          bool is_transactional,
                                          const uchar *before_record,
                                          const uchar *after_record
                                          __attribute__((unused)))
  {
    DBUG_ASSERT(!table->versioned(VERS_TRX_ID));
    return thd->binlog_delete_row(table, is_transactional,
                                  before_record);
  }
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  uint8 get_trg_event_map();
#endif

protected:
  virtual Log_event_type get_general_type_code() { return (Log_event_type)TYPE_CODE; }

#ifdef MYSQL_CLIENT
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_before_row_operations(const Slave_reporting_capability *const);
  virtual int do_after_row_operations(const Slave_reporting_capability *const,int);
  virtual int do_exec_row(rpl_group_info *);
#endif
};

class Delete_rows_compressed_log_event : public Delete_rows_log_event
{
public:
#if defined(MYSQL_SERVER)
  Delete_rows_compressed_log_event(THD*, TABLE*, ulong, bool is_transactional);
  virtual bool write();
#endif
#ifdef HAVE_REPLICATION
  Delete_rows_compressed_log_event(const char *buf, uint event_len, 
                       const Format_description_log_event *description_event);
#endif
private:
#if defined(MYSQL_CLIENT)
  bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif
};


#include "log_event_old.h"


class Incident_log_event : public Log_event {
public:
#ifdef MYSQL_SERVER
  Incident_log_event(THD *thd_arg, Incident incident)
    : Log_event(thd_arg, 0, FALSE), m_incident(incident)
  {
    DBUG_ENTER("Incident_log_event::Incident_log_event");
    DBUG_PRINT("enter", ("m_incident: %d", m_incident));
    m_message.str= NULL;                    
    m_message.length= 0;
    set_direct_logging();
    
    flags&= ~LOG_EVENT_SKIP_REPLICATION_F;
    DBUG_VOID_RETURN;
  }

  Incident_log_event(THD *thd_arg, Incident incident, const LEX_CSTRING *msg)
    : Log_event(thd_arg, 0, FALSE), m_incident(incident)
  {
    DBUG_ENTER("Incident_log_event::Incident_log_event");
    DBUG_PRINT("enter", ("m_incident: %d", m_incident));
    m_message.length= 0;
    if (unlikely(!(m_message.str= (char*) my_malloc(msg->length+1,
                                                    MYF(MY_WME)))))
    {
      
      m_incident= INCIDENT_NONE;
      DBUG_VOID_RETURN;
    }
    strmake(m_message.str, msg->str, msg->length);
    m_message.length= msg->length;
    set_direct_logging();
    
    flags&= ~LOG_EVENT_SKIP_REPLICATION_F;
    DBUG_VOID_RETURN;
  }
#endif

#ifdef MYSQL_SERVER
  void pack_info(Protocol*);

  virtual bool write_data_header();
  virtual bool write_data_body();
#endif

  Incident_log_event(const char *buf, uint event_len,
                     const Format_description_log_event *descr_event);

  virtual ~Incident_log_event();

#ifdef MYSQL_CLIENT
  virtual bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

#if defined(MYSQL_SERVER) && defined(HAVE_REPLICATION)
  virtual int do_apply_event(rpl_group_info *rgi);
#endif

  virtual Log_event_type get_type_code() { return INCIDENT_EVENT; }

  virtual bool is_valid() const
  {
    return m_incident > INCIDENT_NONE && m_incident < INCIDENT_COUNT;
  }
  virtual int get_data_size() {
    return INCIDENT_HEADER_LEN + 1 + (uint) m_message.length;
  }

private:
  const char *description() const;

  Incident m_incident;
  LEX_STRING m_message;
};



class Ignorable_log_event : public Log_event {
public:
  int number;
  const char *description;

#ifndef MYSQL_CLIENT
  Ignorable_log_event(THD *thd_arg)
    :Log_event(thd_arg, LOG_EVENT_IGNORABLE_F, FALSE),
    number(0), description("internal")
  {
    DBUG_ENTER("Ignorable_log_event::Ignorable_log_event");
    DBUG_VOID_RETURN;
  }
#endif

  Ignorable_log_event(const char *buf,
                      const Format_description_log_event *descr_event,
                      const char *event_name);
  virtual ~Ignorable_log_event();

#ifndef MYSQL_CLIENT
  void pack_info(Protocol*);
#endif

#ifdef MYSQL_CLIENT
  virtual bool print(FILE *file, PRINT_EVENT_INFO *print_event_info);
#endif

  virtual Log_event_type get_type_code() { return IGNORABLE_LOG_EVENT; }

  virtual bool is_valid() const { return 1; }

  virtual int get_data_size() { return IGNORABLE_HEADER_LEN; }
};

#ifdef MYSQL_CLIENT
bool copy_cache_to_string_wrapped(IO_CACHE *body,
                                  LEX_STRING *to,
                                  bool do_wrap,
                                  const char *delimiter,
                                  bool is_verbose);
bool copy_cache_to_file_wrapped(IO_CACHE *body,
                                FILE *file,
                                bool do_wrap,
                                const char *delimiter,
                                bool is_verbose);
#endif

#ifdef MYSQL_SERVER

class Heartbeat_log_event: public Log_event
{
public:
  Heartbeat_log_event(const char* buf, uint event_len,
                      const Format_description_log_event* description_event);
  Log_event_type get_type_code() { return HEARTBEAT_LOG_EVENT; }
  bool is_valid() const
    {
      return (log_ident != NULL &&
              log_pos >= BIN_LOG_HEADER_SIZE);
    }
  const char * get_log_ident() { return log_ident; }
  uint get_ident_len() { return ident_len; }
  
private:
  const char* log_ident;
  uint ident_len;
};

inline int Log_event_writer::write(Log_event *ev)
{
  ev->writer= this;
  int res= ev->write();
  IF_DBUG(ev->writer= 0,); 
  add_status(ev->logged_status());
  return res;
}


bool slave_execute_deferred_events(THD *thd);
#endif

bool event_that_should_be_ignored(const char *buf);
bool event_checksum_test(uchar *buf, ulong event_len, enum_binlog_checksum_alg alg);
enum enum_binlog_checksum_alg get_checksum_alg(const char* buf, ulong len);
extern TYPELIB binlog_checksum_typelib;




int binlog_buf_compress(const char *src, char *dst, uint32 len, uint32 *comlen);
int binlog_buf_uncompress(const char *src, char *dst, uint32 len, uint32 *newlen);
uint32 binlog_get_compress_len(uint32 len);
uint32 binlog_get_uncompress_len(const char *buf);

int query_event_uncompress(const Format_description_log_event *description_event, bool contain_checksum,
                           const char *src, ulong src_len, char* buf, ulong buf_size, bool* is_malloc,
                           char **dst, ulong *newlen);

int row_log_event_uncompress(const Format_description_log_event *description_event, bool contain_checksum,
                             const char *src, ulong src_len, char* buf, ulong buf_size, bool* is_malloc,
                             char **dst, ulong *newlen);


#endif 
