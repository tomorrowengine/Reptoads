#ifndef HANDLER_INCLUDED
#define HANDLER_INCLUDED




#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "sql_const.h"
#include "sql_basic_types.h"
#include "mysqld.h"                             
#include "sql_plugin.h"        
#include "thr_lock.h"          
#include "sql_cache.h"
#include "structs.h"                            
#include "sql_array.h"          
#include "mdl.h"
#include "vers_string.h"

#include "sql_analyze_stmt.h" 

#include <my_compare.h>
#include <ft_global.h>
#include <keycache.h>
#include <mysql/psi/mysql_table.h>
#include "sql_sequence.h"

class Alter_info;
class Virtual_column_info;
class sequence_definition;



#define HA_ADMIN_ALREADY_DONE	  1
#define HA_ADMIN_OK               0
#define HA_ADMIN_NOT_IMPLEMENTED -1
#define HA_ADMIN_FAILED		 -2
#define HA_ADMIN_CORRUPT         -3
#define HA_ADMIN_INTERNAL_ERROR  -4
#define HA_ADMIN_INVALID         -5
#define HA_ADMIN_REJECT          -6
#define HA_ADMIN_TRY_ALTER       -7
#define HA_ADMIN_WRONG_CHECKSUM  -8
#define HA_ADMIN_NOT_BASE_TABLE  -9
#define HA_ADMIN_NEEDS_UPGRADE  -10
#define HA_ADMIN_NEEDS_ALTER    -11
#define HA_ADMIN_NEEDS_CHECK    -12


enum enum_alter_inplace_result {
  HA_ALTER_ERROR,
  HA_ALTER_INPLACE_COPY_NO_LOCK,
  HA_ALTER_INPLACE_COPY_LOCK,
  HA_ALTER_INPLACE_NOCOPY_LOCK,
  HA_ALTER_INPLACE_NOCOPY_NO_LOCK,
  HA_ALTER_INPLACE_INSTANT,
  HA_ALTER_INPLACE_NOT_SUPPORTED,
  HA_ALTER_INPLACE_EXCLUSIVE_LOCK,
  HA_ALTER_INPLACE_SHARED_LOCK,
  HA_ALTER_INPLACE_NO_LOCK
};



#define HA_NO_TRANSACTIONS     (1ULL << 0) 
#define HA_PARTIAL_COLUMN_READ (1ULL << 1) 
#define HA_TABLE_SCAN_ON_INDEX (1ULL << 2) 

#define HA_REC_NOT_IN_SEQ      (1ULL << 3)
#define HA_CAN_GEOMETRY        (1ULL << 4)

#define HA_FAST_KEY_READ       (1ULL << 5)

#define HA_REQUIRES_KEY_COLUMNS_FOR_DELETE (1ULL << 6)
#define HA_NULL_IN_KEY         (1ULL << 7) 
#define HA_DUPLICATE_POS       (1ULL << 8)    
#define HA_NO_BLOBS            (1ULL << 9) 
#define HA_CAN_INDEX_BLOBS     (1ULL << 10)
#define HA_AUTO_PART_KEY       (1ULL << 11) 
#define HA_REQUIRE_PRIMARY_KEY (1ULL << 12) 
#define HA_STATS_RECORDS_IS_EXACT (1ULL << 13) 

#define HA_CAN_INSERT_DELAYED  (1ULL << 14)

#define HA_PRIMARY_KEY_IN_READ_INDEX (1ULL << 15)
 
#define HA_PRIMARY_KEY_REQUIRED_FOR_POSITION (1ULL << 16) 
#define HA_CAN_RTREEKEYS       (1ULL << 17)
#define HA_NOT_DELETE_WITH_CACHE (1ULL << 18) 

#define HA_PRIMARY_KEY_REQUIRED_FOR_DELETE (1ULL << 19)
#define HA_NO_PREFIX_CHAR_KEYS (1ULL << 20)
#define HA_CAN_FULLTEXT        (1ULL << 21)
#define HA_CAN_SQL_HANDLER     (1ULL << 22)
#define HA_NO_AUTO_INCREMENT   (1ULL << 23)

#define HA_HAS_OLD_CHECKSUM    (1ULL << 24)

#define HA_FILE_BASED	       (1ULL << 26)
#define HA_CAN_BIT_FIELD       (1ULL << 28) 
#define HA_NEED_READ_RANGE_BUFFER (1ULL << 29) 
#define HA_ANY_INDEX_MAY_BE_UNIQUE (1ULL << 30)
#define HA_NO_COPY_ON_ALTER    (1ULL << 31)
#define HA_HAS_RECORDS	       (1ULL << 32) 

#define HA_HAS_OWN_BINLOGGING  (1ULL << 33)

#define HA_BINLOG_ROW_CAPABLE  (1ULL << 34)
#define HA_BINLOG_STMT_CAPABLE (1ULL << 35)


#define HA_DUPLICATE_KEY_NOT_IN_ORDER    (1ULL << 36)


#define HA_CAN_REPAIR                    (1ULL << 37)


#define HA_HAS_NEW_CHECKSUM    (1ULL << 38)
#define HA_CAN_VIRTUAL_COLUMNS (1ULL << 39)
#define HA_MRR_CANT_SORT       (1ULL << 40)
#define HA_RECORD_MUST_BE_CLEAN_ON_WRITE (1ULL << 41) 


#define HA_CAN_TABLE_CONDITION_PUSHDOWN (1ULL << 42)

#define HA_MUST_USE_TABLE_CONDITION_PUSHDOWN HA_CAN_TABLE_CONDITION_PUSHDOWN


#define HA_READ_BEFORE_WRITE_REMOVAL  (1ULL << 43)


#define HA_CAN_FULLTEXT_EXT              (1ULL << 44)


#define HA_CAN_EXPORT                 (1ULL << 45)


#define HA_CONCURRENT_OPTIMIZE          (1ULL << 46)


#define HA_CAN_TABLES_WITHOUT_ROLLBACK  (1ULL << 47)



#define HA_PERSISTENT_TABLE              (1ULL << 48)


#define HA_BINLOG_FLAGS (HA_BINLOG_ROW_CAPABLE | HA_BINLOG_STMT_CAPABLE)


#define HA_CAN_FORCE_BULK_UPDATE (1ULL << 50)
#define HA_CAN_FORCE_BULK_DELETE (1ULL << 51)
#define HA_CAN_DIRECT_UPDATE_AND_DELETE (1ULL << 52)


#define HA_CAN_MULTISTEP_MERGE (1LL << 53)


#define HA_CMP_REF_IS_EXPENSIVE (1ULL << 54)


#define HA_WANTS_PRIMARY_KEY (1ULL << 55)


#define HA_READ_NEXT            1       
#define HA_READ_PREV            2       
#define HA_READ_ORDER           4       
#define HA_READ_RANGE           8       
#define HA_ONLY_WHOLE_INDEX	16	
#define HA_KEYREAD_ONLY         64	


#define HA_KEY_SCAN_NOT_ROR     128 
#define HA_DO_INDEX_COND_PUSHDOWN  256 

#define HA_CLUSTERED_INDEX      512



#define HA_INPLACE_ADD_INDEX_NO_READ_WRITE         (1UL << 0)
#define HA_INPLACE_DROP_INDEX_NO_READ_WRITE        (1UL << 1)
#define HA_INPLACE_ADD_UNIQUE_INDEX_NO_READ_WRITE  (1UL << 2)
#define HA_INPLACE_DROP_UNIQUE_INDEX_NO_READ_WRITE (1UL << 3)
#define HA_INPLACE_ADD_PK_INDEX_NO_READ_WRITE      (1UL << 4)
#define HA_INPLACE_DROP_PK_INDEX_NO_READ_WRITE     (1UL << 5)

#define HA_INPLACE_ADD_INDEX_NO_WRITE              (1UL << 6)
#define HA_INPLACE_DROP_INDEX_NO_WRITE             (1UL << 7)
#define HA_INPLACE_ADD_UNIQUE_INDEX_NO_WRITE       (1UL << 8)
#define HA_INPLACE_DROP_UNIQUE_INDEX_NO_WRITE      (1UL << 9)
#define HA_INPLACE_ADD_PK_INDEX_NO_WRITE           (1UL << 10)
#define HA_INPLACE_DROP_PK_INDEX_NO_WRITE          (1UL << 11)

#define HA_PARTITION_FUNCTION_SUPPORTED         (1UL << 12)
#define HA_FAST_CHANGE_PARTITION                (1UL << 13)
#define HA_PARTITION_ONE_PHASE                  (1UL << 14)


#define HA_KEY_SWITCH_NONUNIQ      0
#define HA_KEY_SWITCH_ALL          1
#define HA_KEY_SWITCH_NONUNIQ_SAVE 2
#define HA_KEY_SWITCH_ALL_SAVE     3


#define MAX_HA 64


#define HA_SLOT_UNDEF ((uint)-1)



#define HA_OPEN_KEYFILE		1U
#define HA_READ_ONLY		16U	

#define HA_TRY_READ_ONLY	32U

	
#define HA_KEY_NULL_LENGTH	1
#define HA_KEY_BLOB_LENGTH	2



#define MAX_KEY_LENGTH (MAX_DATA_LENGTH_FOR_KEY \
                         +(MAX_REF_PARTS \
                          *(HA_KEY_NULL_LENGTH + HA_KEY_BLOB_LENGTH)))

#define HA_LEX_CREATE_TMP_TABLE	1U
#define HA_CREATE_TMP_ALTER     8U
#define HA_LEX_CREATE_SEQUENCE  16U
#define HA_VERSIONED_TABLE      32U

#define HA_MAX_REC_LENGTH	65535


#define HA_CACHE_TBL_NONTRANSACT 0
#define HA_CACHE_TBL_NOCACHE     1U
#define HA_CACHE_TBL_ASKTRANSACT 2U
#define HA_CACHE_TBL_TRANSACT    4U




static const uint MYSQL_START_TRANS_OPT_WITH_CONS_SNAPSHOT = 1;

static const uint MYSQL_START_TRANS_OPT_READ_ONLY          = 2;

static const uint MYSQL_START_TRANS_OPT_READ_WRITE         = 4;


#define HA_CHECK_DUP_KEY 1U
#define HA_CHECK_DUP_UNIQUE 2U
#define HA_CHECK_FK_ERROR 4U
#define HA_CHECK_DUP (HA_CHECK_DUP_KEY + HA_CHECK_DUP_UNIQUE)
#define HA_CHECK_ALL (~0U)


#define INFO_KIND_UPDATE_FIELDS       101
#define INFO_KIND_UPDATE_VALUES       102
#define INFO_KIND_FORCE_LIMIT_BEGIN   103
#define INFO_KIND_FORCE_LIMIT_END     104

enum legacy_db_type
{
  
  DB_TYPE_UNKNOWN=0,
  DB_TYPE_HEAP=6,
  DB_TYPE_MYISAM=9,
  DB_TYPE_MRG_MYISAM=10,
  DB_TYPE_INNODB=12,
  DB_TYPE_EXAMPLE_DB=15,
  DB_TYPE_ARCHIVE_DB=16,
  DB_TYPE_CSV_DB=17,
  DB_TYPE_FEDERATED_DB=18,
  DB_TYPE_BLACKHOLE_DB=19,
  DB_TYPE_PARTITION_DB=20,
  DB_TYPE_BINLOG=21,
  DB_TYPE_PBXT=23,
  DB_TYPE_PERFORMANCE_SCHEMA=28,
  DB_TYPE_ARIA=42,
  DB_TYPE_TOKUDB=43,
  DB_TYPE_SEQUENCE=44,
  DB_TYPE_FIRST_DYNAMIC=45,
  DB_TYPE_DEFAULT=127 
};

#define DB_TYPE_AUTOASSIGN DB_TYPE_UNKNOWN

enum row_type { ROW_TYPE_NOT_USED=-1, ROW_TYPE_DEFAULT, ROW_TYPE_FIXED,
		ROW_TYPE_DYNAMIC, ROW_TYPE_COMPRESSED,
		ROW_TYPE_REDUNDANT, ROW_TYPE_COMPACT, ROW_TYPE_PAGE };


#define ROW_TYPE_MAX ((uint)ROW_TYPE_PAGE + 1)


enum column_format_type {
  COLUMN_FORMAT_TYPE_DEFAULT=   0, 
  COLUMN_FORMAT_TYPE_FIXED=     1, 
  COLUMN_FORMAT_TYPE_DYNAMIC=   2  
};

enum enum_binlog_func {
  BFN_RESET_LOGS=        1,
  BFN_RESET_SLAVE=       2,
  BFN_BINLOG_WAIT=       3,
  BFN_BINLOG_END=        4,
  BFN_BINLOG_PURGE_FILE= 5
};

enum enum_binlog_command {
  LOGCOM_CREATE_TABLE,
  LOGCOM_ALTER_TABLE,
  LOGCOM_RENAME_TABLE,
  LOGCOM_DROP_TABLE,
  LOGCOM_CREATE_DB,
  LOGCOM_ALTER_DB,
  LOGCOM_DROP_DB
};




#define HA_CREATE_USED_AUTO             (1UL << 0)
#define HA_CREATE_USED_RAID             (1UL << 1) 
#define HA_CREATE_USED_UNION            (1UL << 2)
#define HA_CREATE_USED_INSERT_METHOD    (1UL << 3)
#define HA_CREATE_USED_MIN_ROWS         (1UL << 4)
#define HA_CREATE_USED_MAX_ROWS         (1UL << 5)
#define HA_CREATE_USED_AVG_ROW_LENGTH   (1UL << 6)
#define HA_CREATE_USED_PACK_KEYS        (1UL << 7)
#define HA_CREATE_USED_CHARSET          (1UL << 8)
#define HA_CREATE_USED_DEFAULT_CHARSET  (1UL << 9)
#define HA_CREATE_USED_DATADIR          (1UL << 10)
#define HA_CREATE_USED_INDEXDIR         (1UL << 11)
#define HA_CREATE_USED_ENGINE           (1UL << 12)
#define HA_CREATE_USED_CHECKSUM         (1UL << 13)
#define HA_CREATE_USED_DELAY_KEY_WRITE  (1UL << 14)
#define HA_CREATE_USED_ROW_FORMAT       (1UL << 15)
#define HA_CREATE_USED_COMMENT          (1UL << 16)
#define HA_CREATE_USED_PASSWORD         (1UL << 17)
#define HA_CREATE_USED_CONNECTION       (1UL << 18)
#define HA_CREATE_USED_KEY_BLOCK_SIZE   (1UL << 19)

#define HA_CREATE_USED_TRANSACTIONAL    (1UL << 20)
#define HA_CREATE_USED_PAGE_CHECKSUM    (1UL << 21)

#define HA_CREATE_USED_STATS_PERSISTENT (1UL << 22)

#define HA_CREATE_USED_STATS_AUTO_RECALC (1UL << 23)

#define HA_CREATE_USED_STATS_SAMPLE_PAGES (1UL << 24)


#define HA_CREATE_USED_SEQUENCE           (1UL << 25)

typedef ulonglong alter_table_operations;




#define ALTER_PARSER_ADD_COLUMN     (1ULL <<  0)

#define ALTER_PARSER_DROP_COLUMN    (1ULL <<  1)

#define ALTER_CHANGE_COLUMN         (1ULL <<  2)


#define ALTER_ADD_INDEX             (1ULL <<  3)

#define ALTER_DROP_INDEX            (1ULL <<  4)

#define ALTER_RENAME                (1ULL <<  5)

#define ALTER_ORDER                 (1ULL <<  6)

#define ALTER_OPTIONS               (1ULL <<  7)

#define ALTER_CHANGE_COLUMN_DEFAULT (1ULL <<  8)

#define ALTER_KEYS_ONOFF            (1ULL <<  9)

#define ALTER_RECREATE              (1ULL << 10)

#define ALTER_ADD_FOREIGN_KEY       (1ULL << 21)

#define ALTER_DROP_FOREIGN_KEY      (1ULL << 22)

#define ALTER_COLUMN_ORDER          (1ULL << 25)
#define ALTER_ADD_CHECK_CONSTRAINT  (1ULL << 27)
#define ALTER_DROP_CHECK_CONSTRAINT (1ULL << 28)
#define ALTER_RENAME_COLUMN         (1ULL << 29)
#define ALTER_COLUMN_UNVERSIONED    (1ULL << 30)
#define ALTER_ADD_SYSTEM_VERSIONING (1ULL << 31)
#define ALTER_DROP_SYSTEM_VERSIONING (1ULL << 32)
#define ALTER_ADD_PERIOD             (1ULL << 33)
#define ALTER_DROP_PERIOD            (1ULL << 34)



#define ALTER_RECREATE_TABLE	     ALTER_RECREATE
#define ALTER_CHANGE_CREATE_OPTION   ALTER_OPTIONS
#define ALTER_ADD_COLUMN             (ALTER_ADD_VIRTUAL_COLUMN | \
                                      ALTER_ADD_STORED_BASE_COLUMN | \
                                      ALTER_ADD_STORED_GENERATED_COLUMN)
#define ALTER_DROP_COLUMN             (ALTER_DROP_VIRTUAL_COLUMN | \
                                       ALTER_DROP_STORED_COLUMN)
#define ALTER_COLUMN_DEFAULT          ALTER_CHANGE_COLUMN_DEFAULT


#define ALTER_ADD_NON_UNIQUE_NON_PRIM_INDEX  (1ULL << 35)


#define ALTER_DROP_NON_UNIQUE_NON_PRIM_INDEX (1ULL << 36)


#define ALTER_ADD_UNIQUE_INDEX               (1ULL << 37)


#define ALTER_DROP_UNIQUE_INDEX              (1ULL << 38)


#define ALTER_ADD_PK_INDEX                   (1ULL << 39)


#define ALTER_DROP_PK_INDEX                  (1ULL << 40)


#define ALTER_ADD_VIRTUAL_COLUMN             (1ULL << 41)

#define ALTER_ADD_STORED_BASE_COLUMN         (1ULL << 42)

#define ALTER_ADD_STORED_GENERATED_COLUMN    (1ULL << 43)


#define ALTER_DROP_VIRTUAL_COLUMN            (1ULL << 44)
#define ALTER_DROP_STORED_COLUMN             (1ULL << 45)


#define ALTER_COLUMN_NAME          	     (1ULL << 46)


#define ALTER_VIRTUAL_COLUMN_TYPE            (1ULL << 47)
#define ALTER_STORED_COLUMN_TYPE             (1ULL << 48)


#define ALTER_COLUMN_EQUAL_PACK_LENGTH       (1ULL << 49)


#define ALTER_STORED_COLUMN_ORDER            (1ULL << 50)


#define ALTER_VIRTUAL_COLUMN_ORDER           (1ULL << 51)


#define ALTER_COLUMN_NULLABLE                (1ULL << 52)


#define ALTER_COLUMN_NOT_NULLABLE            (1ULL << 53)


#define ALTER_VIRTUAL_GCOL_EXPR              (1ULL << 54)
#define ALTER_STORED_GCOL_EXPR               (1ULL << 55)


#define ALTER_COLUMN_OPTION                  (1ULL << 56)


#define ALTER_COLUMN_STORAGE_TYPE            ALTER_COLUMN_OPTION


#define ALTER_COLUMN_COLUMN_FORMAT           (1ULL << 57)


#define ALTER_COLUMN_VCOL                    (1ULL << 58)


#define ALTER_PARTITIONED                    (1ULL << 59)


#define ALTER_COLUMN_INDEX_LENGTH            (1ULL << 60)




#define ALTER_PARTITION_ADD         (1ULL << 1)

#define ALTER_PARTITION_DROP        (1ULL << 2)

#define ALTER_PARTITION_COALESCE    (1ULL << 3)

#define ALTER_PARTITION_REORGANIZE  (1ULL << 4)

#define ALTER_PARTITION_INFO        (1ULL << 5)


#define ALTER_PARTITION_ADMIN       (1ULL << 6)

#define ALTER_PARTITION_REBUILD     (1ULL << 7)

#define ALTER_PARTITION_ALL         (1ULL << 8)

#define ALTER_PARTITION_REMOVE      (1ULL << 9)

#define ALTER_PARTITION_EXCHANGE    (1ULL << 10)

#define ALTER_PARTITION_TRUNCATE    (1ULL << 11)

#define ALTER_PARTITION_TABLE_REORG           (1ULL << 12)


extern const char *mysqld_system_database;


struct st_system_tablename
{
  const char *db;
  const char *tablename;
};


typedef ulonglong my_xid; 
#define MYSQL_XID_PREFIX "MySQLXid"
#define MYSQL_XID_PREFIX_LEN 8 
#define MYSQL_XID_OFFSET (MYSQL_XID_PREFIX_LEN+sizeof(server_id))
#define MYSQL_XID_GTRID_LEN (MYSQL_XID_OFFSET+sizeof(my_xid))

#define XIDDATASIZE MYSQL_XIDDATASIZE
#define MAXGTRIDSIZE 64
#define MAXBQUALSIZE 64

#define COMPATIBLE_DATA_YES 0
#define COMPATIBLE_DATA_NO  1


struct xid_t {
  long formatID;
  long gtrid_length;
  long bqual_length;
  char data[XIDDATASIZE];  

  xid_t() {}                                
  bool eq(struct xid_t *xid)
  { return !xid->is_null() && eq(xid->gtrid_length, xid->bqual_length, xid->data); }
  bool eq(long g, long b, const char *d)
  { return !is_null() && g == gtrid_length && b == bqual_length && !memcmp(d, data, g+b); }
  void set(struct xid_t *xid)
  { memcpy(this, xid, xid->length()); }
  void set(long f, const char *g, long gl, const char *b, long bl)
  {
    formatID= f;
    memcpy(data, g, gtrid_length= gl);
    memcpy(data+gl, b, bqual_length= bl);
  }
  void set(ulonglong xid)
  {
    my_xid tmp;
    formatID= 1;
    set(MYSQL_XID_PREFIX_LEN, 0, MYSQL_XID_PREFIX);
    memcpy(data+MYSQL_XID_PREFIX_LEN, &server_id, sizeof(server_id));
    tmp= xid;
    memcpy(data+MYSQL_XID_OFFSET, &tmp, sizeof(tmp));
    gtrid_length=MYSQL_XID_GTRID_LEN;
  }
  void set(long g, long b, const char *d)
  {
    formatID= 1;
    gtrid_length= g;
    bqual_length= b;
    memcpy(data, d, g+b);
  }
  bool is_null() const { return formatID == -1; }
  void null() { formatID= -1; }
  my_xid quick_get_my_xid()
  {
    my_xid tmp;
    memcpy(&tmp, data+MYSQL_XID_OFFSET, sizeof(tmp));
    return tmp;
  }
  my_xid get_my_xid()
  {
    return gtrid_length == MYSQL_XID_GTRID_LEN && bqual_length == 0 &&
           !memcmp(data, MYSQL_XID_PREFIX, MYSQL_XID_PREFIX_LEN) ?
           quick_get_my_xid() : 0;
  }
  uint length()
  {
    return static_cast<uint>(sizeof(formatID)) + key_length();
  }
  uchar *key() const
  {
    return (uchar *)&gtrid_length;
  }
  uint key_length() const
  {
    return static_cast<uint>(sizeof(gtrid_length)+sizeof(bqual_length)+
                             gtrid_length+bqual_length);
  }
};
typedef struct xid_t XID;


#define SQL_XIDSIZE (XIDDATASIZE * 2 + 8 + MY_INT64_NUM_DECIMAL_DIGITS)

uint get_sql_xid(XID *xid, char *buf);


#define MIN_XID_LIST_SIZE  128
#define MAX_XID_LIST_SIZE  (1024*128)


#define UNDEF_NODEGROUP 65535
enum ts_command_type
{
  TS_CMD_NOT_DEFINED = -1,
  CREATE_TABLESPACE = 0,
  ALTER_TABLESPACE = 1,
  CREATE_LOGFILE_GROUP = 2,
  ALTER_LOGFILE_GROUP = 3,
  DROP_TABLESPACE = 4,
  DROP_LOGFILE_GROUP = 5,
  CHANGE_FILE_TABLESPACE = 6,
  ALTER_ACCESS_MODE_TABLESPACE = 7
};

enum ts_alter_tablespace_type
{
  TS_ALTER_TABLESPACE_TYPE_NOT_DEFINED = -1,
  ALTER_TABLESPACE_ADD_FILE = 1,
  ALTER_TABLESPACE_DROP_FILE = 2
};

enum tablespace_access_mode
{
  TS_NOT_DEFINED= -1,
  TS_READ_ONLY = 0,
  TS_READ_WRITE = 1,
  TS_NOT_ACCESSIBLE = 2
};

struct handlerton;
class st_alter_tablespace : public Sql_alloc
{
  public:
  const char *tablespace_name;
  const char *logfile_group_name;
  enum ts_command_type ts_cmd_type;
  enum ts_alter_tablespace_type ts_alter_tablespace_type;
  const char *data_file_name;
  const char *undo_file_name;
  const char *redo_file_name;
  ulonglong extent_size;
  ulonglong undo_buffer_size;
  ulonglong redo_buffer_size;
  ulonglong initial_size;
  ulonglong autoextend_size;
  ulonglong max_size;
  uint nodegroup_id;
  handlerton *storage_engine;
  bool wait_until_completed;
  const char *ts_comment;
  enum tablespace_access_mode ts_access_mode;
  st_alter_tablespace()
  {
    tablespace_name= NULL;
    logfile_group_name= "DEFAULT_LG"; 
    ts_cmd_type= TS_CMD_NOT_DEFINED;
    data_file_name= NULL;
    undo_file_name= NULL;
    redo_file_name= NULL;
    extent_size= 1024*1024;        
    undo_buffer_size= 8*1024*1024; 
    redo_buffer_size= 8*1024*1024; 
    initial_size= 128*1024*1024;   
    autoextend_size= 0;            
    max_size= 0;                   
    storage_engine= NULL;
    nodegroup_id= UNDEF_NODEGROUP;
    wait_until_completed= TRUE;
    ts_comment= NULL;
    ts_access_mode= TS_NOT_DEFINED;
  }
};



struct TABLE;


enum enum_schema_tables
{
  SCH_ALL_PLUGINS,
  SCH_APPLICABLE_ROLES,
  SCH_CHARSETS,
  SCH_CHECK_CONSTRAINTS,
  SCH_COLLATIONS,
  SCH_COLLATION_CHARACTER_SET_APPLICABILITY,
  SCH_COLUMNS,
  SCH_COLUMN_PRIVILEGES,
  SCH_ENABLED_ROLES,
  SCH_ENGINES,
  SCH_EVENTS,
  SCH_EXPLAIN,
  SCH_FILES,
  SCH_GLOBAL_STATUS,
  SCH_GLOBAL_VARIABLES,
  SCH_KEY_CACHES,
  SCH_KEY_COLUMN_USAGE,
  SCH_OPEN_TABLES,
  SCH_PARAMETERS,
  SCH_PARTITIONS,
  SCH_PLUGINS,
  SCH_PROCESSLIST,
  SCH_PROFILES,
  SCH_REFERENTIAL_CONSTRAINTS,
  SCH_PROCEDURES,
  SCH_SCHEMATA,
  SCH_SCHEMA_PRIVILEGES,
  SCH_SESSION_STATUS,
  SCH_SESSION_VARIABLES,
  SCH_STATISTICS,
  SCH_SYSTEM_VARIABLES,
  SCH_TABLES,
  SCH_TABLESPACES,
  SCH_TABLE_CONSTRAINTS,
  SCH_TABLE_NAMES,
  SCH_TABLE_PRIVILEGES,
  SCH_TRIGGERS,
  SCH_USER_PRIVILEGES,
  SCH_VIEWS,
#ifdef HAVE_SPATIAL
  SCH_GEOMETRY_COLUMNS,
  SCH_SPATIAL_REF_SYS,
#endif 
};

struct TABLE_SHARE;
struct HA_CREATE_INFO;
struct st_foreign_key_info;
typedef struct st_foreign_key_info FOREIGN_KEY_INFO;
typedef bool (stat_print_fn)(THD *thd, const char *type, size_t type_len,
                             const char *file, size_t file_len,
                             const char *status, size_t status_len);
enum ha_stat_type { HA_ENGINE_STATUS, HA_ENGINE_LOGS, HA_ENGINE_MUTEX };
extern st_plugin_int *hton2plugin[MAX_HA];


enum log_status
{
  HA_LOG_STATUS_FREE= 0,      
  HA_LOG_STATUS_INUSE= 1,     
  HA_LOG_STATUS_NOSUCHLOG= 2  
};

void signal_log_not_needed(struct handlerton, char *log_file);

struct handler_log_file_data {
  LEX_STRING filename;
  enum log_status status;
};



struct ha_table_option_struct;
struct ha_field_option_struct;
struct ha_index_option_struct;

enum ha_option_type { HA_OPTION_TYPE_ULL,    
                      HA_OPTION_TYPE_STRING, 
                      HA_OPTION_TYPE_ENUM,   
                      HA_OPTION_TYPE_BOOL,   
                      HA_OPTION_TYPE_SYSVAR};

#define HA_xOPTION_NUMBER(name, struc, field, def, min, max, blk_siz)   \
  { HA_OPTION_TYPE_ULL, name, sizeof(name)-1,                        \
    offsetof(struc, field), def, min, max, blk_siz, 0, 0 }
#define HA_xOPTION_STRING(name, struc, field)                        \
  { HA_OPTION_TYPE_STRING, name, sizeof(name)-1,                     \
    offsetof(struc, field), 0, 0, 0, 0, 0, 0}
#define HA_xOPTION_ENUM(name, struc, field, values, def)             \
  { HA_OPTION_TYPE_ENUM, name, sizeof(name)-1,                       \
    offsetof(struc, field), def, 0,                                  \
    sizeof(values)-1, 0, values, 0 }
#define HA_xOPTION_BOOL(name, struc, field, def)                     \
  { HA_OPTION_TYPE_BOOL, name, sizeof(name)-1,                       \
    offsetof(struc, field), def, 0, 1, 0, 0, 0 }
#define HA_xOPTION_SYSVAR(name, struc, field, sysvar)                \
  { HA_OPTION_TYPE_SYSVAR, name, sizeof(name)-1,                     \
    offsetof(struc, field), 0, 0, 0, 0, 0, MYSQL_SYSVAR(sysvar) }
#define HA_xOPTION_END { HA_OPTION_TYPE_ULL, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

#define HA_TOPTION_NUMBER(name, field, def, min, max, blk_siz)          \
  HA_xOPTION_NUMBER(name, ha_table_option_struct, field, def, min, max, blk_siz)
#define HA_TOPTION_STRING(name, field)                               \
  HA_xOPTION_STRING(name, ha_table_option_struct, field)
#define HA_TOPTION_ENUM(name, field, values, def)                    \
  HA_xOPTION_ENUM(name, ha_table_option_struct, field, values, def)
#define HA_TOPTION_BOOL(name, field, def)                            \
  HA_xOPTION_BOOL(name, ha_table_option_struct, field, def)
#define HA_TOPTION_SYSVAR(name, field, sysvar)                       \
  HA_xOPTION_SYSVAR(name, ha_table_option_struct, field, sysvar)
#define HA_TOPTION_END HA_xOPTION_END

#define HA_FOPTION_NUMBER(name, field, def, min, max, blk_siz)          \
  HA_xOPTION_NUMBER(name, ha_field_option_struct, field, def, min, max, blk_siz)
#define HA_FOPTION_STRING(name, field)                               \
  HA_xOPTION_STRING(name, ha_field_option_struct, field)
#define HA_FOPTION_ENUM(name, field, values, def)                    \
  HA_xOPTION_ENUM(name, ha_field_option_struct, field, values, def)
#define HA_FOPTION_BOOL(name, field, def)                            \
  HA_xOPTION_BOOL(name, ha_field_option_struct, field, def)
#define HA_FOPTION_SYSVAR(name, field, sysvar)                       \
  HA_xOPTION_SYSVAR(name, ha_field_option_struct, field, sysvar)
#define HA_FOPTION_END HA_xOPTION_END

#define HA_IOPTION_NUMBER(name, field, def, min, max, blk_siz)          \
  HA_xOPTION_NUMBER(name, ha_index_option_struct, field, def, min, max, blk_siz)
#define HA_IOPTION_STRING(name, field)                               \
  HA_xOPTION_STRING(name, ha_index_option_struct, field)
#define HA_IOPTION_ENUM(name, field, values, def)                    \
  HA_xOPTION_ENUM(name, ha_index_option_struct, field, values, def)
#define HA_IOPTION_BOOL(name, field, def)                            \
  HA_xOPTION_BOOL(name, ha_index_option_struct, field, def)
#define HA_IOPTION_SYSVAR(name, field, sysvar)                       \
  HA_xOPTION_SYSVAR(name, ha_index_option_struct, field, sysvar)
#define HA_IOPTION_END HA_xOPTION_END

typedef struct st_ha_create_table_option {
  enum ha_option_type type;
  const char *name;
  size_t name_length;
  ptrdiff_t offset;
  ulonglong def_value;
  ulonglong min_value, max_value, block_size;
  const char *values;
  struct st_mysql_sys_var *var;
} ha_create_table_option;

enum handler_iterator_type
{
  
  HA_TRANSACTLOG_ITERATOR= 1
};
enum handler_create_iterator_result
{
  HA_ITERATOR_OK,          
  HA_ITERATOR_UNSUPPORTED, 
  HA_ITERATOR_ERROR        
};


struct handler_iterator {
  
  int (*next)(struct handler_iterator *, void *iterator_object);
  
  void (*destroy)(struct handler_iterator *);
  
  void *buffer;
};

class handler;
class group_by_handler;
struct Query;
typedef class st_select_lex SELECT_LEX;
typedef struct st_order ORDER;


struct handlerton
{
  
  SHOW_COMP_OPTION state;

  
  enum legacy_db_type db_type;
  
   uint slot;
   
   uint savepoint_offset;
   
   int  (*close_connection)(handlerton *hton, THD *thd);
   
   void (*kill_query)(handlerton *hton, THD *thd, enum thd_kill_levels level);
   
   int  (*savepoint_set)(handlerton *hton, THD *thd, void *sv);
   
   int  (*savepoint_rollback)(handlerton *hton, THD *thd, void *sv);
   
   bool (*savepoint_rollback_can_release_mdl)(handlerton *hton, THD *thd);
   int  (*savepoint_release)(handlerton *hton, THD *thd, void *sv);
   
   int (*commit)(handlerton *hton, THD *thd, bool all);
   
   void (*commit_ordered)(handlerton *hton, THD *thd, bool all);
   int  (*rollback)(handlerton *hton, THD *thd, bool all);
   int  (*prepare)(handlerton *hton, THD *thd, bool all);
   
   void (*prepare_ordered)(handlerton *hton, THD *thd, bool all);
   int  (*recover)(handlerton *hton, XID *xid_list, uint len);
   int  (*commit_by_xid)(handlerton *hton, XID *xid);
   int  (*rollback_by_xid)(handlerton *hton, XID *xid);
   
   void (*commit_checkpoint_request)(handlerton *hton, void *cookie);
  
   int  (*checkpoint_state)(handlerton *hton, bool disabled);
   void *(*create_cursor_read_view)(handlerton *hton, THD *thd);
   void (*set_cursor_read_view)(handlerton *hton, THD *thd, void *read_view);
   void (*close_cursor_read_view)(handlerton *hton, THD *thd, void *read_view);
   handler *(*create)(handlerton *hton, TABLE_SHARE *table, MEM_ROOT *mem_root);
   void (*drop_database)(handlerton *hton, char* path);
   int (*panic)(handlerton *hton, enum ha_panic_function flag);
   int (*start_consistent_snapshot)(handlerton *hton, THD *thd);
   bool (*flush_logs)(handlerton *hton);
   bool (*show_status)(handlerton *hton, THD *thd, stat_print_fn *print, enum ha_stat_type stat);
   uint (*partition_flags)();
   alter_table_operations (*alter_table_flags)(alter_table_operations flags);
   int (*alter_tablespace)(handlerton *hton, THD *thd, st_alter_tablespace *ts_info);
   int (*fill_is_table)(handlerton *hton, THD *thd, TABLE_LIST *tables, 
                        class Item *cond, 
                        enum enum_schema_tables);
   uint32 flags;                                
   
   int (*binlog_func)(handlerton *hton, THD *thd, enum_binlog_func fn, void *arg);
   void (*binlog_log_query)(handlerton *hton, THD *thd, 
                            enum_binlog_command binlog_command,
                            const char *query, uint query_length,
                            const char *db, const char *table_name);

   
   enum log_status (*get_log_status)(handlerton *hton, char *log);

   
   enum handler_create_iterator_result
     (*create_iterator)(handlerton *hton, enum handler_iterator_type type,
                        struct handler_iterator *fill_this_in);
   int (*abort_transaction)(handlerton *hton, THD *bf_thd,
			    THD *victim_thd, my_bool signal);
   int (*set_checkpoint)(handlerton *hton, const XID* xid);
   int (*get_checkpoint)(handlerton *hton, XID* xid);
   void (*fake_trx_id)(handlerton *hton, THD *thd);
   
   ha_create_table_option *table_options; 
   ha_create_table_option *field_options; 
   ha_create_table_option *index_options; 

   
   const char **tablefile_extensions; 

  

  
  group_by_handler *(*create_group_by)(THD *thd, Query *query);

   

   
   int (*discover_table)(handlerton *hton, THD* thd, TABLE_SHARE *share);

   
   class discovered_list
   {
     public:
     virtual bool add_table(const char *tname, size_t tlen) = 0;
     virtual bool add_file(const char *fname) = 0;
     protected: virtual ~discovered_list() {}
   };

   
   int (*discover_table_names)(handlerton *hton, LEX_CSTRING *db, MY_DIR *dir,
                               discovered_list *result);

   
   int (*discover_table_existence)(handlerton *hton, const char *db,
                                   const char *table_name);

   
   int (*discover_table_structure)(handlerton *hton, THD* thd,
                                   TABLE_SHARE *share, HA_CREATE_INFO *info);

   
   
   ulonglong (*prepare_commit_versioned)(THD *thd, ulonglong *trx_id);
};


static inline LEX_CSTRING *hton_name(const handlerton *hton)
{
  return &(hton2plugin[hton->slot]->name);
}

static inline handlerton *plugin_hton(plugin_ref plugin)
{
  return plugin_data(plugin, handlerton *);
}

static inline sys_var *find_hton_sysvar(handlerton *hton, st_mysql_sys_var *var)
{
  return find_plugin_sysvar(hton2plugin[hton->slot], var);
}

handlerton *ha_default_handlerton(THD *thd);
handlerton *ha_default_tmp_handlerton(THD *thd);


#define HTON_NO_FLAGS                 0
#define HTON_CLOSE_CURSORS_AT_COMMIT (1 << 0)
#define HTON_ALTER_NOT_SUPPORTED     (1 << 1) 
#define HTON_CAN_RECREATE            (1 << 2) 
#define HTON_HIDDEN                  (1 << 3) 
#define HTON_NOT_USER_SELECTABLE     (1 << 5)
#define HTON_TEMPORARY_NOT_SUPPORTED (1 << 6) 
#define HTON_SUPPORT_LOG_TABLES      (1 << 7) 
#define HTON_NO_PARTITION            (1 << 8) 


#define HTON_NO_BINLOG_ROW_OPT       (1 << 9)
#define HTON_SUPPORTS_EXTENDED_KEYS  (1 <<10) 
#define HTON_NATIVE_SYS_VERSIONING (1 << 11) 


#define HTON_SUPPORTS_FOREIGN_KEYS   (1 << 0) 

#define HTON_CAN_MERGE               (1 <<11) 

#define HTON_CAN_READ_CONNECT_STRING_IN_PARTITION (1 <<12)

class Ha_trx_info;

struct THD_TRANS
{
  
  bool        no_2pc;
  
  Ha_trx_info *ha_list;
  
  bool modified_non_trans_table;

  void reset() {
    no_2pc= FALSE;
    modified_non_trans_table= FALSE;
    m_unsafe_rollback_flags= 0;
  }
  bool is_empty() const { return ha_list == NULL; }
  THD_TRANS() {}                        

  unsigned int m_unsafe_rollback_flags;
 
  enum unsafe_statement_types
  {
    CREATED_TEMP_TABLE= 2,
    DROPPED_TEMP_TABLE= 4,
    DID_WAIT= 8,
    DID_DDL= 0x10
  };

  void mark_created_temp_table()
  {
    DBUG_PRINT("debug", ("mark_created_temp_table"));
    m_unsafe_rollback_flags|= CREATED_TEMP_TABLE;
  }
  void mark_dropped_temp_table()
  {
    DBUG_PRINT("debug", ("mark_dropped_temp_table"));
    m_unsafe_rollback_flags|= DROPPED_TEMP_TABLE;
  }
  bool has_created_dropped_temp_table() const {
    return
      (m_unsafe_rollback_flags & (CREATED_TEMP_TABLE|DROPPED_TEMP_TABLE)) != 0;
  }
  void mark_trans_did_wait() { m_unsafe_rollback_flags|= DID_WAIT; }
  bool trans_did_wait() const {
    return (m_unsafe_rollback_flags & DID_WAIT) != 0;
  }
  bool is_trx_read_write() const;
  void mark_trans_did_ddl() { m_unsafe_rollback_flags|= DID_DDL; }
  bool trans_did_ddl() const {
    return (m_unsafe_rollback_flags & DID_DDL) != 0;
  }

};




class Ha_trx_info
{
public:
  
  void register_ha(THD_TRANS *trans, handlerton *ht_arg)
  {
    DBUG_ASSERT(m_flags == 0);
    DBUG_ASSERT(m_ht == NULL);
    DBUG_ASSERT(m_next == NULL);

    m_ht= ht_arg;
    m_flags= (int) TRX_READ_ONLY; 

    m_next= trans->ha_list;
    trans->ha_list= this;
  }

  
  void reset()
  {
    m_next= NULL;
    m_ht= NULL;
    m_flags= 0;
  }

  Ha_trx_info() { reset(); }

  void set_trx_read_write()
  {
    DBUG_ASSERT(is_started());
    m_flags|= (int) TRX_READ_WRITE;
  }
  bool is_trx_read_write() const
  {
    DBUG_ASSERT(is_started());
    return m_flags & (int) TRX_READ_WRITE;
  }
  bool is_started() const { return m_ht != NULL; }
  
  void coalesce_trx_with(const Ha_trx_info *stmt_trx)
  {
    
    DBUG_ASSERT(is_started());
    if (stmt_trx->is_trx_read_write())
      set_trx_read_write();
  }
  Ha_trx_info *next() const
  {
    DBUG_ASSERT(is_started());
    return m_next;
  }
  handlerton *ht() const
  {
    DBUG_ASSERT(is_started());
    return m_ht;
  }
private:
  enum { TRX_READ_ONLY= 0, TRX_READ_WRITE= 1 };
  
  Ha_trx_info *m_next;
  
  handlerton *m_ht;
  
  uchar       m_flags;
};


inline bool THD_TRANS::is_trx_read_write() const
{
  Ha_trx_info *ha_info;
  for (ha_info= ha_list; ha_info; ha_info= ha_info->next())
    if (ha_info->is_trx_read_write())
      return TRUE;
  return FALSE;
}


enum enum_tx_isolation { ISO_READ_UNCOMMITTED, ISO_READ_COMMITTED,
			 ISO_REPEATABLE_READ, ISO_SERIALIZABLE};


typedef struct {
  ulonglong data_file_length;
  ulonglong max_data_file_length;
  ulonglong index_file_length;
  ulonglong max_index_file_length;
  ulonglong delete_length;
  ha_rows records;
  ulong mean_rec_length;
  time_t create_time;
  time_t check_time;
  time_t update_time;
  ulonglong check_sum;
} PARTITION_STATS;

#define UNDEF_NODEGROUP 65535
class Item;
struct st_table_log_memory_entry;

class partition_info;

struct st_partition_iter;

enum ha_choice { HA_CHOICE_UNDEF, HA_CHOICE_NO, HA_CHOICE_YES, HA_CHOICE_MAX };

enum enum_stats_auto_recalc { HA_STATS_AUTO_RECALC_DEFAULT= 0,
                              HA_STATS_AUTO_RECALC_ON,
                              HA_STATS_AUTO_RECALC_OFF };


struct Schema_specification_st
{
  CHARSET_INFO *default_table_charset;
  void init()
  {
    bzero(this, sizeof(*this));
  }
};

class Create_field;

enum vers_sys_type_t
{
  VERS_UNDEFINED= 0,
  VERS_TIMESTAMP,
  VERS_TRX_ID
};

extern const LEX_CSTRING null_clex_str;

struct Vers_parse_info
{
  Vers_parse_info() :
    check_unit(VERS_UNDEFINED),
    versioned_fields(false),
    unversioned_fields(false)
  {}

  void init() 
  {
    system_time= start_end_t(null_clex_str, null_clex_str);
    as_row= start_end_t(null_clex_str, null_clex_str);
    check_unit= VERS_UNDEFINED;
    versioned_fields= false;
    unversioned_fields= false;
  }

  struct start_end_t
  {
    start_end_t()
    {}
    start_end_t(LEX_CSTRING _start, LEX_CSTRING _end) :
      start(_start),
      end(_end) {}
    Lex_ident start;
    Lex_ident end;
  };

  start_end_t system_time;
  start_end_t as_row;
  vers_sys_type_t check_unit;

  void set_system_time(Lex_ident start, Lex_ident end)
  {
    system_time.start= start;
    system_time.end= end;
  }

protected:
  friend struct Table_scope_and_contents_source_st;
  void set_start(const LEX_CSTRING field_name)
  {
    as_row.start= field_name;
    system_time.start= field_name;
  }
  void set_end(const LEX_CSTRING field_name)
  {
    as_row.end= field_name;
    system_time.end= field_name;
  }
  bool is_start(const char *name) const;
  bool is_end(const char *name) const;
  bool is_start(const Create_field &f) const;
  bool is_end(const Create_field &f) const;
  bool fix_implicit(THD *thd, Alter_info *alter_info);
  operator bool() const
  {
    return as_row.start || as_row.end || system_time.start || system_time.end;
  }
  bool need_check(const Alter_info *alter_info) const;
  bool check_conditions(const Lex_table_name &table_name,
                        const Lex_table_name &db) const;
public:
  static const Lex_ident default_start;
  static const Lex_ident default_end;

  bool fix_alter_info(THD *thd, Alter_info *alter_info,
                       HA_CREATE_INFO *create_info, TABLE *table);
  bool fix_create_like(Alter_info &alter_info, HA_CREATE_INFO &create_info,
                       TABLE_LIST &src_table, TABLE_LIST &table);
  bool check_sys_fields(const Lex_table_name &table_name,
                        const Lex_table_name &db,
                        Alter_info *alter_info);

  
  bool versioned_fields : 1;
  bool unversioned_fields : 1;
};



struct Table_scope_and_contents_source_pod_st 
{
  CHARSET_INFO *table_charset;
  LEX_CUSTRING tabledef_version;
  LEX_CSTRING connect_string;
  LEX_CSTRING comment;
  LEX_CSTRING alias;
  const char *password, *tablespace;
  const char *data_file_name, *index_file_name;
  ulonglong max_rows,min_rows;
  ulonglong auto_increment_value;
  ulong table_options;                  
  ulong avg_row_length;
  ulong used_fields;
  ulong key_block_size;
  ulong expression_length;
  ulong field_check_constraints;
  
  uint stats_sample_pages;
  uint null_bits;                       
  uint options;				
  uint merge_insert_method;
  uint extra_size;                      
  handlerton *db_type;
  
  enum row_type row_type;
  enum ha_choice transactional;
  enum ha_storage_media storage_media;  
  enum ha_choice page_checksum;         
  engine_option_value *option_list;     
  enum_stats_auto_recalc stats_auto_recalc;
  bool varchar;                         
  bool sequence;                        

  List<Virtual_column_info> *check_constraint_list;

  
  ha_table_option_struct *option_struct;           
  ha_field_option_struct **fields_option_struct;   
  ha_index_option_struct **indexes_option_struct;  

  
  TABLE *table;
  TABLE_LIST *pos_in_locked_tables;
  TABLE_LIST *merge_list;
  MDL_ticket *mdl_ticket;
  bool table_was_deleted;
  sequence_definition *seq_create_info;

  void init()
  {
    bzero(this, sizeof(*this));
  }
  bool tmp_table() const { return options & HA_LEX_CREATE_TMP_TABLE; }
  void use_default_db_type(THD *thd)
  {
    db_type= tmp_table() ? ha_default_tmp_handlerton(thd)
                         : ha_default_handlerton(thd);
  }

  bool versioned() const
  {
    return options & HA_VERSIONED_TABLE;
  }
};


struct Table_scope_and_contents_source_st:
         public Table_scope_and_contents_source_pod_st
{
  Vers_parse_info vers_info;

  void init()
  {
    Table_scope_and_contents_source_pod_st::init();
    vers_info.init();
  }

  bool vers_fix_system_fields(THD *thd, Alter_info *alter_info,
                         const TABLE_LIST &create_table,
                         bool create_select= false);

  bool vers_check_system_fields(THD *thd, Alter_info *alter_info,
                                const TABLE_LIST &create_table);

};



struct HA_CREATE_INFO: public Table_scope_and_contents_source_st,
                       public Schema_specification_st
{
  void init()
  {
    Table_scope_and_contents_source_st::init();
    Schema_specification_st::init();
  }
  bool check_conflicting_charset_declarations(CHARSET_INFO *cs);
  bool add_table_option_default_charset(CHARSET_INFO *cs)
  {
    
    if (check_conflicting_charset_declarations(cs))
      return true;
    default_table_charset= cs;
    used_fields|= HA_CREATE_USED_DEFAULT_CHARSET;
    return false;
  }
  bool add_alter_list_item_convert_to_charset(CHARSET_INFO *cs)
  {
    
    DBUG_ASSERT(cs);
    if (check_conflicting_charset_declarations(cs))
      return true;
    table_charset= default_table_charset= cs;
    used_fields|= (HA_CREATE_USED_CHARSET | HA_CREATE_USED_DEFAULT_CHARSET);  
    return false;
  }
  ulong table_options_with_row_type()
  {
    if (row_type == ROW_TYPE_DYNAMIC || row_type == ROW_TYPE_PAGE)
      return table_options | HA_OPTION_PACK_RECORD;
    else
      return table_options;
  }
};



struct Table_specification_st: public HA_CREATE_INFO,
                               public DDL_options_st
{
  
  void init()
  {
    HA_CREATE_INFO::init();
    DDL_options_st::init();
  }
  void init(DDL_options_st::Options options_arg)
  {
    HA_CREATE_INFO::init();
    DDL_options_st::init(options_arg);
  }
  
  void lex_start()
  {
    HA_CREATE_INFO::options= 0;
    DDL_options_st::init();
  }
};




class inplace_alter_handler_ctx : public Sql_alloc
{
public:
  inplace_alter_handler_ctx() {}

  virtual ~inplace_alter_handler_ctx() {}
};




class Alter_inplace_info
{
public:

  
  HA_CREATE_INFO *create_info;

  
  Alter_info *alter_info;

  
  KEY  *key_info_buffer;

  
  uint key_count;

  
  uint index_drop_count;

  
  KEY  **index_drop_buffer;

  
  uint index_add_count;

  
  uint *index_add_buffer;

  
  inplace_alter_handler_ctx *handler_ctx;

  
  inplace_alter_handler_ctx **group_commit_ctx;

  
  alter_table_operations handler_flags;

  
  ulong partition_flags;

  
  partition_info *modified_part_info;

  
  const bool ignore;

  
  bool online;

  
  const char *unsupported_reason;

  Alter_inplace_info(HA_CREATE_INFO *create_info_arg,
                     Alter_info *alter_info_arg,
                     KEY *key_info_arg, uint key_count_arg,
                     partition_info *modified_part_info_arg,
                     bool ignore_arg)
    : create_info(create_info_arg),
    alter_info(alter_info_arg),
    key_info_buffer(key_info_arg),
    key_count(key_count_arg),
    index_drop_count(0),
    index_drop_buffer(NULL),
    index_add_count(0),
    index_add_buffer(NULL),
    handler_ctx(NULL),
    group_commit_ctx(NULL),
    handler_flags(0),
    modified_part_info(modified_part_info_arg),
    ignore(ignore_arg),
    online(false),
    unsupported_reason(NULL)
  {}

  ~Alter_inplace_info()
  {
    delete handler_ctx;
  }

  
  void report_unsupported_error(const char *not_supported,
                                const char *try_instead) const;
};


typedef struct st_key_create_information
{
  enum ha_key_alg algorithm;
  ulong block_size;
  uint flags;                                   
  LEX_CSTRING parser_name;
  LEX_CSTRING comment;
  
  bool check_for_duplicate_indexes;
} KEY_CREATE_INFO;



class TABLEOP_HOOKS
{
public:
  TABLEOP_HOOKS() {}
  virtual ~TABLEOP_HOOKS() {}

  inline void prelock(TABLE **tables, uint count)
  {
    do_prelock(tables, count);
  }

  inline int postlock(TABLE **tables, uint count)
  {
    return do_postlock(tables, count);
  }
private:
  
  virtual void do_prelock(TABLE **tables, uint count)
  {
    
  }

  
  virtual int do_postlock(TABLE **tables, uint count)
  {
    return 0;                           
  }
};

typedef struct st_savepoint SAVEPOINT;
extern ulong savepoint_alloc_size;
extern KEY_CREATE_INFO default_key_create_info;


typedef class Item COND;

typedef struct st_ha_check_opt
{
  st_ha_check_opt() {}                        
  uint flags;       
  uint sql_flags;   
  time_t start_time;   
  KEY_CACHE *key_cache; 
  void init();
} HA_CHECK_OPT;




typedef void *range_seq_t;

typedef struct st_range_seq_if
{
  
  void (*get_key_info)(void *init_params, uint *length, key_part_map *map);

  
  range_seq_t (*init)(void *init_params, uint n_ranges, uint flags);


  
  bool (*next) (range_seq_t seq, KEY_MULTI_RANGE *range);

   
  bool (*skip_record) (range_seq_t seq, range_id_t range_info, uchar *rowid);

   
  bool (*skip_index_tuple) (range_seq_t seq, range_id_t range_info);
} RANGE_SEQ_IF;

typedef bool (*SKIP_INDEX_TUPLE_FUNC) (range_seq_t seq, range_id_t range_info);

class Cost_estimate
{ 
public:
  double io_count;     
  double avg_io_cost;  
  double cpu_cost;     
  double import_cost;  
  double mem_cost;      
  
  enum { IO_COEFF=1 };
  enum { CPU_COEFF=1 };
  enum { MEM_COEFF=1 };
  enum { IMPORT_COEFF=1 };

  Cost_estimate()
  {
    reset();
  }

  double total_cost() 
  {
    return IO_COEFF*io_count*avg_io_cost + CPU_COEFF * cpu_cost +
           MEM_COEFF*mem_cost + IMPORT_COEFF*import_cost;
  }

  
  bool is_zero() const
  {
    return io_count == 0.0 && cpu_cost == 0.0 &&
      import_cost == 0.0 && mem_cost == 0.0;
  }

  void reset()
  {
    avg_io_cost= 1.0;
    io_count= cpu_cost= mem_cost= import_cost= 0.0;
  }

  void multiply(double m)
  {
    io_count *= m;
    cpu_cost *= m;
    import_cost *= m;
    
  }

  void add(const Cost_estimate* cost)
  {
    double io_count_sum= io_count + cost->io_count;
    add_io(cost->io_count, cost->avg_io_cost);
    io_count= io_count_sum;
    cpu_cost += cost->cpu_cost;
  }

  void add_io(double add_io_cnt, double add_avg_cost)
  {
    
    if (add_io_cnt > 0)
    {
      double io_count_sum= io_count + add_io_cnt;
      avg_io_cost= (io_count * avg_io_cost + 
                    add_io_cnt * add_avg_cost) / io_count_sum;
      io_count= io_count_sum;
    }
  }

  
  void add_cpu(double add_cpu_cost) { cpu_cost+= add_cpu_cost; }

  
  void add_import(double add_import_cost) { import_cost+= add_import_cost; }

  
  void add_mem(double add_mem_cost) { mem_cost+= add_mem_cost; }

  
  void convert_from_cost(double cost)
  {
    reset();
    io_count= cost;
  }
};

void get_sweep_read_cost(TABLE *table, ha_rows nrows, bool interrupted, 
                         Cost_estimate *cost);


#define HA_MRR_SINGLE_POINT 1U
#define HA_MRR_FIXED_KEY  2U


#define HA_MRR_NO_ASSOCIATION 4U


#define HA_MRR_SORTED 8U


#define HA_MRR_INDEX_ONLY 16U


#define HA_MRR_LIMITS 32U



#define HA_MRR_USE_DEFAULT_IMPL 64U


#define HA_MRR_NO_NULL_ENDPOINTS 128U


#define HA_MRR_MATERIALIZED_KEYS 256U


#define HA_MRR_IMPLEMENTATION_FLAG1 512U
#define HA_MRR_IMPLEMENTATION_FLAG2 1024U
#define HA_MRR_IMPLEMENTATION_FLAG3 2048U
#define HA_MRR_IMPLEMENTATION_FLAG4 4096U
#define HA_MRR_IMPLEMENTATION_FLAG5 8192U
#define HA_MRR_IMPLEMENTATION_FLAG6 16384U

#define HA_MRR_IMPLEMENTATION_FLAGS \
  (512U | 1024U | 2048U | 4096U | 8192U | 16384U)



typedef struct st_handler_buffer
{
  uchar *buffer;         
  uchar *buffer_end;     
  uchar *end_of_used_area;     
} HANDLER_BUFFER;

typedef struct system_status_var SSV;

class ha_statistics
{
public:
  ulonglong data_file_length;		
  ulonglong max_data_file_length;	
  ulonglong index_file_length;
  ulonglong max_index_file_length;
  ulonglong delete_length;		
  ulonglong auto_increment_value;
  
  ha_rows records;
  ha_rows deleted;			
  ulong mean_rec_length;		
  time_t create_time;			
  time_t check_time;
  time_t update_time;
  uint block_size;			

  
  uint mrr_length_per_rec; 

  ha_statistics():
    data_file_length(0), max_data_file_length(0),
    index_file_length(0), max_index_file_length(0), delete_length(0),
    auto_increment_value(0), records(0), deleted(0), mean_rec_length(0),
    create_time(0), check_time(0), update_time(0), block_size(0),
    mrr_length_per_rec(0)
  {}
};

extern "C" enum icp_result handler_index_cond_check(void* h_arg);

uint calculate_key_len(TABLE *, uint, const uchar *, key_part_map);

#define make_keypart_map(N) (((key_part_map)2 << (N)) - 1)

#define make_prev_keypart_map(N) (((key_part_map)1 << (N)) - 1)



class Handler_share
{
public:
  Handler_share() {}
  virtual ~Handler_share() {}
};




class handler :public Sql_alloc
{
public:
  typedef ulonglong Table_flags;
protected:
  TABLE_SHARE *table_share;   
  TABLE *table;               
  Table_flags cached_table_flags;       

  ha_rows estimation_rows_to_insert;
public:
  handlerton *ht;                 
  uchar *ref;				
  uchar *dup_ref;			

  ha_statistics stats;

  
  range_seq_t mrr_iter;    
  RANGE_SEQ_IF mrr_funcs;  
  HANDLER_BUFFER *multi_range_buffer; 
  uint ranges_in_seq; 
  
  KEY_MULTI_RANGE mrr_cur_range;

  
  key_range save_end_range, *end_range;
  KEY_PART_INFO *range_key_part;
  int key_compare_result_on_equal;

  
  bool mrr_is_output_sorted;
  
  bool mrr_have_range;
  bool eq_range;
  bool internal_tmp_table;                 
  bool implicit_emptied;                   
  bool mark_trx_read_write_done;           
  bool check_table_binlog_row_based_done; 
  bool check_table_binlog_row_based_result; 
  
  bool row_already_logged;
  
  bool in_range_check_pushed_down;

  uint errkey;                             
  uint key_used_on_scan;
  uint active_index, keyread;

  
  uint ref_length;
  FT_INFO *ft_handler;
  enum init_stat { NONE=0, INDEX, RND };
  init_stat inited, pre_inited;

  const COND *pushed_cond;
  
  ulonglong next_insert_id;
  
  ulonglong insert_id_for_cur_row;
  
  
  ulonglong rows_read;
  ulonglong rows_tmp_read;
  ulonglong rows_changed;
  
  ulonglong index_rows_read[MAX_KEY+1];

private:
  
  Exec_time_tracker *tracker;
public:
  void set_time_tracker(Exec_time_tracker *tracker_arg) { tracker=tracker_arg;}

  Item *pushed_idx_cond;
  uint pushed_idx_cond_keyno;  

  Discrete_interval auto_inc_interval_for_cur_row;
  
  uint auto_inc_intervals_count;

  
  PSI_table *m_psi;

  virtual void unbind_psi();
  virtual void rebind_psi();

  bool set_top_table_fields;
  struct TABLE *top_table;
  Field **top_table_field;
  uint top_table_fields;

private:
  
  int m_lock_type;
  
  Handler_share **ha_share;

public:
  handler(handlerton *ht_arg, TABLE_SHARE *share_arg)
    :table_share(share_arg), table(0),
    estimation_rows_to_insert(0), ht(ht_arg),
    ref(0), end_range(NULL),
    implicit_emptied(0),
    mark_trx_read_write_done(0),
    check_table_binlog_row_based_done(0),
    check_table_binlog_row_based_result(0),
    row_already_logged(0),
    in_range_check_pushed_down(FALSE),
    key_used_on_scan(MAX_KEY),
    active_index(MAX_KEY), keyread(MAX_KEY),
    ref_length(sizeof(my_off_t)),
    ft_handler(0), inited(NONE), pre_inited(NONE),
    pushed_cond(0), next_insert_id(0), insert_id_for_cur_row(0),
    tracker(NULL),
    pushed_idx_cond(NULL),
    pushed_idx_cond_keyno(MAX_KEY),
    auto_inc_intervals_count(0),
    m_psi(NULL), set_top_table_fields(FALSE), top_table(0),
    top_table_field(0), top_table_fields(0),
    m_lock_type(F_UNLCK), ha_share(NULL)
  {
    DBUG_PRINT("info",
               ("handler created F_UNLCK %d F_RDLCK %d F_WRLCK %d",
                F_UNLCK, F_RDLCK, F_WRLCK));
    reset_statistics();
  }
  virtual ~handler(void)
  {
    DBUG_ASSERT(m_lock_type == F_UNLCK);
    DBUG_ASSERT(inited == NONE);
  }
  virtual handler *clone(const char *name, MEM_ROOT *mem_root);
  
  void init()
  {
    cached_table_flags= table_flags();
  }
  
  
  int ha_open(TABLE *table, const char *name, int mode, uint test_if_locked,
              MEM_ROOT *mem_root= 0, List<String> *partitions_to_open=NULL);
  int ha_index_init(uint idx, bool sorted)
  {
    DBUG_EXECUTE_IF("ha_index_init_fail", return HA_ERR_TABLE_DEF_CHANGED;);
    int result;
    DBUG_ENTER("ha_index_init");
    DBUG_ASSERT(inited==NONE);
    if (!(result= index_init(idx, sorted)))
    {
      inited=       INDEX;
      active_index= idx;
      end_range= NULL;
    }
    DBUG_RETURN(result);
  }
  int ha_index_end()
  {
    DBUG_ENTER("ha_index_end");
    DBUG_ASSERT(inited==INDEX);
    inited=       NONE;
    active_index= MAX_KEY;
    end_range=    NULL;
    DBUG_RETURN(index_end());
  }
  
  virtual int prepare_index_scan() { return 0; }
  virtual int prepare_index_key_scan_map(const uchar * key, key_part_map keypart_map)
  {
    uint key_len= calculate_key_len(table, active_index, key, keypart_map);
    return  prepare_index_key_scan(key, key_len);
  }
  virtual int prepare_index_key_scan( const uchar * key, uint key_len )
  { return 0; }
  virtual int prepare_range_scan(const key_range *start_key, const key_range *end_key)
  { return 0; }

  int ha_rnd_init(bool scan) __attribute__ ((warn_unused_result))
  {
    DBUG_EXECUTE_IF("ha_rnd_init_fail", return HA_ERR_TABLE_DEF_CHANGED;);
    int result;
    DBUG_ENTER("ha_rnd_init");
    DBUG_ASSERT(inited==NONE || (inited==RND && scan));
    inited= (result= rnd_init(scan)) ? NONE: RND;
    end_range= NULL;
    DBUG_RETURN(result);
  }
  int ha_rnd_end()
  {
    DBUG_ENTER("ha_rnd_end");
    DBUG_ASSERT(inited==RND);
    inited=NONE;
    end_range= NULL;
    DBUG_RETURN(rnd_end());
  }
  int ha_rnd_init_with_error(bool scan) __attribute__ ((warn_unused_result));
  int ha_reset();
  
  int ha_index_or_rnd_end()
  {
    return inited == INDEX ? ha_index_end() : inited == RND ? ha_rnd_end() : 0;
  }
  
  Table_flags ha_table_flags() const { return cached_table_flags; }
  
  int ha_external_lock(THD *thd, int lock_type);
  int ha_write_row(uchar * buf);
  int ha_update_row(const uchar * old_data, const uchar * new_data);
  int ha_delete_row(const uchar * buf);
  void ha_release_auto_increment();

  bool keyread_enabled() { return keyread < MAX_KEY; }
  int ha_start_keyread(uint idx)
  {
    int res= keyread_enabled() ? 0 : extra_opt(HA_EXTRA_KEYREAD, idx);
    keyread= idx;
    return res;
  }
  int ha_end_keyread()
  {
    if (!keyread_enabled())
      return 0;
    keyread= MAX_KEY;
    return extra(HA_EXTRA_NO_KEYREAD);
  }

  int check_collation_compatibility();
  int ha_check_for_upgrade(HA_CHECK_OPT *check_opt);
  
  int ha_check(THD *thd, HA_CHECK_OPT *check_opt);
  int ha_repair(THD* thd, HA_CHECK_OPT* check_opt);
  void ha_start_bulk_insert(ha_rows rows, uint flags= 0)
  {
    DBUG_ENTER("handler::ha_start_bulk_insert");
    estimation_rows_to_insert= rows;
    start_bulk_insert(rows, flags);
    DBUG_VOID_RETURN;
  }
  int ha_end_bulk_insert()
  {
    DBUG_ENTER("handler::ha_end_bulk_insert");
    estimation_rows_to_insert= 0;
    int ret= end_bulk_insert();
    DBUG_RETURN(ret);
  }
  int ha_bulk_update_row(const uchar *old_data, const uchar *new_data,
                         ha_rows *dup_key_found);
  int ha_delete_all_rows();
  int ha_truncate();
  int ha_reset_auto_increment(ulonglong value);
  int ha_optimize(THD* thd, HA_CHECK_OPT* check_opt);
  int ha_analyze(THD* thd, HA_CHECK_OPT* check_opt);
  bool ha_check_and_repair(THD *thd);
  int ha_disable_indexes(uint mode);
  int ha_enable_indexes(uint mode);
  int ha_discard_or_import_tablespace(my_bool discard);
  int ha_rename_table(const char *from, const char *to);
  int ha_delete_table(const char *name);
  void ha_drop_table(const char *name);

  int ha_create(const char *name, TABLE *form, HA_CREATE_INFO *info);

  int ha_create_partitioning_metadata(const char *name, const char *old_name,
                                      int action_flag);

  int ha_change_partitions(HA_CREATE_INFO *create_info,
                           const char *path,
                           ulonglong * const copied,
                           ulonglong * const deleted,
                           const uchar *pack_frm_data,
                           size_t pack_frm_len);
  int ha_drop_partitions(const char *path);
  int ha_rename_partitions(const char *path);

  void adjust_next_insert_id_after_explicit_value(ulonglong nr);
  int update_auto_increment();
  virtual void print_error(int error, myf errflag);
  virtual bool get_error_message(int error, String *buf);
  uint get_dup_key(int error);
  
  virtual bool get_foreign_dup_key(char *child_table_name,
                                   uint child_table_name_len,
                                   char *child_key_name,
                                   uint child_key_name_len)
  { DBUG_ASSERT(false); return(false); }
  void reset_statistics()
  {
    rows_read= rows_changed= rows_tmp_read= 0;
    bzero(index_rows_read, sizeof(index_rows_read));
  }
  virtual void change_table_ptr(TABLE *table_arg, TABLE_SHARE *share)
  {
    table= table_arg;
    table_share= share;
    reset_statistics();
  }
  virtual double scan_time()
  { return ulonglong2double(stats.data_file_length) / IO_SIZE + 2; }

  
  virtual double read_time(uint index, uint ranges, ha_rows rows)
  { return rows2double(ranges+rows); }

  
  virtual double keyread_time(uint index, uint ranges, ha_rows rows);

  virtual const key_map *keys_to_use_for_scanning() { return &key_map_empty; }

  
  bool has_transactions()
  {
    return ((ha_table_flags() & (HA_NO_TRANSACTIONS | HA_PERSISTENT_TABLE))
            == 0);
  }
  
  bool has_transaction_manager()
  {
    return ((ha_table_flags() & HA_NO_TRANSACTIONS) == 0);
  }

  
  virtual bool is_fatal_error(int error, uint flags)
  {
    if (!error ||
        ((flags & HA_CHECK_DUP_KEY) &&
         (error == HA_ERR_FOUND_DUPP_KEY ||
          error == HA_ERR_FOUND_DUPP_UNIQUE)) ||
        error == HA_ERR_AUTOINC_ERANGE ||
        ((flags & HA_CHECK_FK_ERROR) &&
         (error == HA_ERR_ROW_IS_REFERENCED ||
          error == HA_ERR_NO_REFERENCED_ROW)))
      return FALSE;
    return TRUE;
  }

  
  virtual int pre_records() { return 0; }
  virtual ha_rows records() { return stats.records; }
  
  virtual ha_rows estimate_rows_upper_bound()
  { return stats.records+EXTRA_RECORDS; }

  
  virtual enum row_type get_row_type() const { return ROW_TYPE_NOT_USED; }

  virtual const char *index_type(uint key_number) { DBUG_ASSERT(0); return "";}


  
  virtual void column_bitmaps_signal();
  
  uint get_index(void) const
  { return inited == INDEX ? active_index : MAX_KEY; }
  int ha_close(void);

  
  virtual bool start_bulk_update() { return 1; }
  
  virtual bool start_bulk_delete() { return 1; }
  
  virtual int exec_bulk_update(ha_rows *dup_key_found)
  {
    DBUG_ASSERT(FALSE);
    return HA_ERR_WRONG_COMMAND;
  }
  
  virtual int end_bulk_update() { return 0; }
  
  virtual int end_bulk_delete()
  {
    DBUG_ASSERT(FALSE);
    return HA_ERR_WRONG_COMMAND;
  }
  virtual int pre_index_read_map(const uchar *key,
                                 key_part_map keypart_map,
                                 enum ha_rkey_function find_flag,
                                 bool use_parallel)
   { return 0; }
  virtual int pre_index_first(bool use_parallel)
   { return 0; }
  virtual int pre_index_last(bool use_parallel)
   { return 0; }
  virtual int pre_index_read_last_map(const uchar *key,
                                      key_part_map keypart_map,
                                      bool use_parallel)
   { return 0; }

  virtual int pre_multi_range_read_next(bool use_parallel)
  { return 0; }
  virtual int pre_read_range_first(const key_range *start_key,
                                   const key_range *end_key,
                                   bool eq_range, bool sorted,
                                   bool use_parallel)
   { return 0; }
  virtual int pre_ft_read(bool use_parallel)
   { return 0; }
  virtual int pre_rnd_next(bool use_parallel)
   { return 0; }
  int ha_pre_rnd_init(bool scan)
  {
    int result;
    DBUG_ENTER("ha_pre_rnd_init");
    DBUG_ASSERT(pre_inited==NONE || (pre_inited==RND && scan));
    pre_inited= (result= pre_rnd_init(scan)) ? NONE: RND;
    DBUG_RETURN(result);
  }
  int ha_pre_rnd_end()
  {
    DBUG_ENTER("ha_pre_rnd_end");
    DBUG_ASSERT(pre_inited==RND);
    pre_inited=NONE;
    DBUG_RETURN(pre_rnd_end());
  }
  virtual int pre_rnd_init(bool scan) { return 0; }
  virtual int pre_rnd_end() { return 0; }
  virtual int pre_index_init(uint idx, bool sorted) { return 0; }
  virtual int pre_index_end() { return 0; }
  int ha_pre_index_init(uint idx, bool sorted)
  {
    int result;
    DBUG_ENTER("ha_pre_index_init");
    DBUG_ASSERT(pre_inited==NONE);
    if (!(result= pre_index_init(idx, sorted)))
      pre_inited=INDEX;
    DBUG_RETURN(result);
  }
  int ha_pre_index_end()
  {
    DBUG_ENTER("ha_pre_index_end");
    DBUG_ASSERT(pre_inited==INDEX);
    pre_inited=NONE;
    DBUG_RETURN(pre_index_end());
  }
  int ha_pre_index_or_rnd_end()
  {
    return (pre_inited == INDEX ?
            ha_pre_index_end() :
            pre_inited == RND ? ha_pre_rnd_end() : 0 );
  }
  virtual bool vers_can_native(THD *thd)
  {
    return ht->flags & HTON_NATIVE_SYS_VERSIONING;
  }

  
protected:
  virtual int index_read_map(uchar * buf, const uchar * key,
                             key_part_map keypart_map,
                             enum ha_rkey_function find_flag)
  {
    uint key_len= calculate_key_len(table, active_index, key, keypart_map);
    return index_read(buf, key, key_len, find_flag);
  }
  
  virtual int index_read_idx_map(uchar * buf, uint index, const uchar * key,
                                 key_part_map keypart_map,
                                 enum ha_rkey_function find_flag);
  virtual int index_next(uchar * buf)
   { return  HA_ERR_WRONG_COMMAND; }
  virtual int index_prev(uchar * buf)
   { return  HA_ERR_WRONG_COMMAND; }
  virtual int index_first(uchar * buf)
   { return  HA_ERR_WRONG_COMMAND; }
  virtual int index_last(uchar * buf)
   { return  HA_ERR_WRONG_COMMAND; }
  virtual int index_next_same(uchar *buf, const uchar *key, uint keylen);
  
  virtual int index_read_last_map(uchar * buf, const uchar * key,
                                  key_part_map keypart_map)
  {
    uint key_len= calculate_key_len(table, active_index, key, keypart_map);
    return index_read_last(buf, key, key_len);
  }
  virtual int close(void)=0;
  inline void update_rows_read()
  {
    if (likely(!internal_tmp_table))
      rows_read++;
    else
      rows_tmp_read++;
  }
  inline void update_index_statistics()
  {
    index_rows_read[active_index]++;
    update_rows_read();
  }
public:

  int ha_index_read_map(uchar * buf, const uchar * key,
                        key_part_map keypart_map,
                        enum ha_rkey_function find_flag);
  int ha_index_read_idx_map(uchar * buf, uint index, const uchar * key,
                            key_part_map keypart_map,
                            enum ha_rkey_function find_flag);
  int ha_index_next(uchar * buf);
  int ha_index_prev(uchar * buf);
  int ha_index_first(uchar * buf);
  int ha_index_last(uchar * buf);
  int ha_index_next_same(uchar *buf, const uchar *key, uint keylen);
  
  virtual ha_rows multi_range_read_info_const(uint keyno, RANGE_SEQ_IF *seq,
                                              void *seq_init_param, 
                                              uint n_ranges, uint *bufsz,
                                              uint *mrr_mode,
                                              Cost_estimate *cost);
  virtual ha_rows multi_range_read_info(uint keyno, uint n_ranges, uint keys,
                                        uint key_parts, uint *bufsz, 
                                        uint *mrr_mode, Cost_estimate *cost);
  virtual int multi_range_read_init(RANGE_SEQ_IF *seq, void *seq_init_param,
                                    uint n_ranges, uint mrr_mode, 
                                    HANDLER_BUFFER *buf);
  virtual int multi_range_read_next(range_id_t *range_info);
  
  virtual int multi_range_read_explain_info(uint mrr_mode, char *str, 
                                            size_t size)
  { return 0; }

  virtual int read_range_first(const key_range *start_key,
                               const key_range *end_key,
                               bool eq_range, bool sorted);
  virtual int read_range_next();
  void set_end_range(const key_range *end_key);
  int compare_key(key_range *range);
  int compare_key2(key_range *range) const;
  virtual int ft_init() { return HA_ERR_WRONG_COMMAND; }
  virtual int pre_ft_init() { return HA_ERR_WRONG_COMMAND; }
  virtual void ft_end() {}
  virtual int pre_ft_end() { return 0; }
  virtual FT_INFO *ft_init_ext(uint flags, uint inx,String *key)
    { return NULL; }
public:
  virtual int ft_read(uchar *buf) { return HA_ERR_WRONG_COMMAND; }
  virtual int rnd_next(uchar *buf)=0;
  virtual int rnd_pos(uchar * buf, uchar *pos)=0;
  
  virtual int rnd_pos_by_record(uchar *record)
  {
    int error;
    DBUG_ASSERT(table_flags() & HA_PRIMARY_KEY_REQUIRED_FOR_POSITION);

    error = ha_rnd_init(false);
    if (error != 0)
      return error;

    position(record);
    error = ha_rnd_pos(record, ref);
    ha_rnd_end();
    return error;
  }
  virtual int read_first_row(uchar *buf, uint primary_key);
public:

  
  inline int ha_ft_read(uchar *buf);
  inline void ha_ft_end() { ft_end(); ft_handler=NULL; }
  int ha_rnd_next(uchar *buf);
  int ha_rnd_pos(uchar *buf, uchar *pos);
  inline int ha_rnd_pos_by_record(uchar *buf);
  inline int ha_read_first_row(uchar *buf, uint primary_key);

  
  virtual int remember_rnd_pos()
    { return HA_ERR_WRONG_COMMAND; }
  virtual int restart_rnd_next(uchar *buf)
    { return HA_ERR_WRONG_COMMAND; }
  virtual int rnd_same(uchar *buf, uint inx)
    { return HA_ERR_WRONG_COMMAND; }

  virtual ha_rows records_in_range(uint inx, key_range *min_key,
                                   key_range *max_key)
    { return (ha_rows) 10; }
  
  virtual void position(const uchar *record)=0;
  virtual int info(uint)=0; 
  virtual void get_dynamic_partition_info(PARTITION_STATS *stat_info,
                                          uint part_id);
  virtual void set_partitions_to_open(List<String> *partition_names) {}
  virtual int change_partitions_to_open(List<String> *partition_names)
  { return 0; }
  virtual int extra(enum ha_extra_function operation)
  { return 0; }
  virtual int extra_opt(enum ha_extra_function operation, ulong arg)
  { return extra(operation); }

  
  bool ha_was_semi_consistent_read();
  virtual bool was_semi_consistent_read() { return 0; }
  
  virtual void try_semi_consistent_read(bool) {}
  virtual void unlock_row() {}
  virtual int start_stmt(THD *thd, thr_lock_type lock_type) {return 0;}
  virtual bool need_info_for_auto_inc() { return 0; }
  virtual bool can_use_for_auto_inc_init() { return 1; }
  virtual void get_auto_increment(ulonglong offset, ulonglong increment,
                                  ulonglong nb_desired_values,
                                  ulonglong *first_value,
                                  ulonglong *nb_reserved_values);
  void set_next_insert_id(ulonglong id)
  {
    DBUG_PRINT("info",("auto_increment: next value %lu", (ulong)id));
    next_insert_id= id;
  }
  void restore_auto_increment(ulonglong prev_insert_id)
  {
    
    next_insert_id= (prev_insert_id > 0) ? prev_insert_id :
      insert_id_for_cur_row;
  }

  virtual void update_create_info(HA_CREATE_INFO *create_info) {}
  int check_old_types();
  virtual int assign_to_keycache(THD* thd, HA_CHECK_OPT* check_opt)
  { return HA_ADMIN_NOT_IMPLEMENTED; }
  virtual int preload_keys(THD* thd, HA_CHECK_OPT* check_opt)
  { return HA_ADMIN_NOT_IMPLEMENTED; }
  

  virtual int indexes_are_disabled(void) {return 0;}
  virtual char *update_table_comment(const char * comment)
  { return (char*) comment;}
  virtual void append_create_info(String *packet) {}
  
  virtual bool is_fk_defined_on_table_or_index(uint index)
  { return FALSE; }
  virtual char* get_foreign_key_create_info()
  { return(NULL);}  
  
  virtual bool can_switch_engines() { return true; }
  virtual int can_continue_handler_scan() { return 0; }
  
  virtual int
  get_foreign_key_list(THD *thd, List<FOREIGN_KEY_INFO> *f_key_list)
  { return 0; }
  
  virtual int
  get_parent_foreign_key_list(THD *thd, List<FOREIGN_KEY_INFO> *f_key_list)
  { return 0; }
  virtual uint referenced_by_foreign_key() { return 0;}
  virtual void init_table_handle_for_HANDLER()
  { return; }       
  virtual void free_foreign_key_create_info(char* str) {}
  
  const char *table_type() const { return hton_name(ht)->str; }
  const char **bas_ext() const { return ht->tablefile_extensions; }

  virtual int get_default_no_partitions(HA_CREATE_INFO *create_info)
  { return 1;}
  virtual void set_auto_partitions(partition_info *part_info) { return; }
  virtual bool get_no_parts(const char *name,
                            uint *no_parts)
  {
    *no_parts= 0;
    return 0;
  }
  virtual void set_part_info(partition_info *part_info) {return;}
  virtual void return_record_by_parent() { return; }

  virtual ulong index_flags(uint idx, uint part, bool all_parts) const =0;

  uint max_record_length() const
  { return MY_MIN(HA_MAX_REC_LENGTH, max_supported_record_length()); }
  uint max_keys() const
  { return MY_MIN(MAX_KEY, max_supported_keys()); }
  uint max_key_parts() const
  { return MY_MIN(MAX_REF_PARTS, max_supported_key_parts()); }
  uint max_key_length() const
  { return MY_MIN(MAX_DATA_LENGTH_FOR_KEY, max_supported_key_length()); }
  uint max_key_part_length() const
  { return MY_MIN(MAX_DATA_LENGTH_FOR_KEY, max_supported_key_part_length()); }

  virtual uint max_supported_record_length() const { return HA_MAX_REC_LENGTH; }
  virtual uint max_supported_keys() const { return 0; }
  virtual uint max_supported_key_parts() const { return MAX_REF_PARTS; }
  virtual uint max_supported_key_length() const { return MAX_DATA_LENGTH_FOR_KEY; }
  virtual uint max_supported_key_part_length() const { return 255; }
  virtual uint min_record_length(uint options) const { return 1; }

  virtual uint checksum() const { return 0; }
  virtual bool is_crashed() const  { return 0; }
  virtual bool auto_repair(int error) const { return 0; }

  void update_global_table_stats();
  void update_global_index_stats();

#define CHF_CREATE_FLAG 0
#define CHF_DELETE_FLAG 1
#define CHF_RENAME_FLAG 2
#define CHF_INDEX_FLAG  3

  
  virtual uint lock_count(void) const { return 1; }
  
  virtual THR_LOCK_DATA **store_lock(THD *thd,
				     THR_LOCK_DATA **to,
				     enum thr_lock_type lock_type)=0;

  
  virtual uint8 table_cache_type() { return HA_CACHE_TBL_NONTRANSACT; }


  

  virtual my_bool register_query_cache_table(THD *thd, const char *table_key,
                                             uint key_length,
                                             qc_engine_callback
                                             *engine_callback,
                                             ulonglong *engine_data)
  {
    *engine_callback= 0;
    return TRUE;
  }

  

  virtual uint count_query_cache_dependant_tables(uint8 *tables_type
                                                  __attribute__((unused)))
  {
    return 0;
  }

  

  virtual my_bool
    register_query_cache_dependant_tables(THD *thd
                                          __attribute__((unused)),
                                          Query_cache *cache
                                          __attribute__((unused)),
                                          Query_cache_block_table **block
                                          __attribute__((unused)),
                                          uint *n __attribute__((unused)))
  {
    return FALSE;
  }

 
 virtual bool primary_key_is_clustered() { return FALSE; }
 virtual int cmp_ref(const uchar *ref1, const uchar *ref2)
 {
   return memcmp(ref1, ref2, ref_length);
 }

 

  
 virtual const COND *cond_push(const COND *cond) { return cond; };
 
 virtual void cond_pop() { return; };

 
 virtual int info_push(uint info_type, void *info) { return 0; };

 
 virtual int set_top_table_and_fields(TABLE *top_table,
                                      Field **top_table_field,
                                      uint top_table_fields)
 {
   if (!set_top_table_fields)
   {
     set_top_table_fields= TRUE;
     this->top_table= top_table;
     this->top_table_field= top_table_field;
     this->top_table_fields= top_table_fields;
   }
   return 0;
 }
 virtual void clear_top_table_fields()
 {
   if (set_top_table_fields)
   {
     set_top_table_fields= FALSE;
     top_table= NULL;
     top_table_field= NULL;
     top_table_fields= 0;
   }
 }

 
 virtual Item *idx_cond_push(uint keyno, Item* idx_cond) { return idx_cond; }

 
 virtual void cancel_pushed_idx_cond()
 {
   pushed_idx_cond= NULL;
   pushed_idx_cond_keyno= MAX_KEY;
   in_range_check_pushed_down= false;
 }

 
  virtual TABLE_LIST *get_next_global_for_child() { return NULL; }

 
 virtual bool check_if_incompatible_data(HA_CREATE_INFO *create_info,
					 uint table_changes)
 { return COMPATIBLE_DATA_NO; }

 

 

 
 virtual enum_alter_inplace_result
 check_if_supported_inplace_alter(TABLE *altered_table,
                                  Alter_inplace_info *ha_alter_info);


 
 bool ha_prepare_inplace_alter_table(TABLE *altered_table,
                                     Alter_inplace_info *ha_alter_info);


 
 bool ha_inplace_alter_table(TABLE *altered_table,
                             Alter_inplace_info *ha_alter_info)
 {
   return inplace_alter_table(altered_table, ha_alter_info);
 }


 
 bool ha_commit_inplace_alter_table(TABLE *altered_table,
                                    Alter_inplace_info *ha_alter_info,
                                    bool commit);


 
 void ha_notify_table_changed()
 {
   notify_table_changed();
 }


protected:
 
 virtual bool prepare_inplace_alter_table(TABLE *altered_table,
                                          Alter_inplace_info *ha_alter_info)
 { return false; }


 
 virtual bool inplace_alter_table(TABLE *altered_table,
                                  Alter_inplace_info *ha_alter_info)
 { return false; }


 
 virtual bool commit_inplace_alter_table(TABLE *altered_table,
                                         Alter_inplace_info *ha_alter_info,
                                         bool commit)
{
  
  ha_alter_info->group_commit_ctx= NULL;
  return false;
}


 
 virtual void notify_table_changed() { }

public:
 


  
  virtual void use_hidden_primary_key();
  virtual alter_table_operations alter_table_flags(alter_table_operations flags)
  {
    if (ht->alter_table_flags)
      return ht->alter_table_flags(flags);
    return 0;
  }

  virtual LEX_CSTRING *engine_name();
  
  TABLE* get_table() { return table; }
  TABLE_SHARE* get_table_share() { return table_share; }
protected:
  
  void **ha_data(THD *) const;
  THD *ha_thd(void) const;

  
  PSI_table_share *ha_table_share_psi() const;

  
  virtual int rename_table(const char *from, const char *to);
  
  virtual int delete_table(const char *name);

public:
  bool check_table_binlog_row_based(bool binlog_row);

  
  inline void mark_trx_read_write()
  {
    if (unlikely(!mark_trx_read_write_done))
    {
      mark_trx_read_write_done= 1;
      mark_trx_read_write_internal();
    }
  }

private:
  void mark_trx_read_write_internal();
  bool check_table_binlog_row_based_internal(bool binlog_row);

protected:
  
  inline void increment_statistics(ulong SSV::*offset) const;
  inline void decrement_statistics(ulong SSV::*offset) const;

private:
  

  virtual int open(const char *name, int mode, uint test_if_locked)=0;
  
  virtual int index_init(uint idx, bool sorted) { return 0; }
  virtual int index_end() { return 0; }
  
  virtual int rnd_init(bool scan)= 0;
  virtual int rnd_end() { return 0; }
  virtual int write_row(uchar *buf __attribute__((unused)))
  {
    return HA_ERR_WRONG_COMMAND;
  }

  
  virtual int update_row(const uchar *old_data __attribute__((unused)),
                         const uchar *new_data __attribute__((unused)))
  {
    return HA_ERR_WRONG_COMMAND;
  }

  
  virtual int update_first_row(uchar *new_data);

  virtual int delete_row(const uchar *buf __attribute__((unused)))
  {
    return HA_ERR_WRONG_COMMAND;
  }

  
public:
  int ha_direct_update_rows(ha_rows *update_rows);
  virtual int direct_update_rows_init(List<Item> *update_fields)
  {
    return HA_ERR_WRONG_COMMAND;
  }
private:
  virtual int pre_direct_update_rows_init(List<Item> *update_fields)
  {
    return HA_ERR_WRONG_COMMAND;
  }
  virtual int direct_update_rows(ha_rows *update_rows __attribute__((unused)))
  {
    return HA_ERR_WRONG_COMMAND;
  }
  virtual int pre_direct_update_rows()
  {
    return HA_ERR_WRONG_COMMAND;
  }

  
public:
  int ha_direct_delete_rows(ha_rows *delete_rows);
  virtual int direct_delete_rows_init()
  {
    return HA_ERR_WRONG_COMMAND;
  }
private:
  virtual int pre_direct_delete_rows_init()
  {
    return HA_ERR_WRONG_COMMAND;
  }
  virtual int direct_delete_rows(ha_rows *delete_rows __attribute__((unused)))
  {
    return HA_ERR_WRONG_COMMAND;
  }
  virtual int pre_direct_delete_rows()
  {
    return HA_ERR_WRONG_COMMAND;
  }

  
  virtual int reset() { return 0; }
  virtual Table_flags table_flags(void) const= 0;
  
  virtual int external_lock(THD *thd __attribute__((unused)),
                            int lock_type __attribute__((unused)))
  {
    return 0;
  }
  virtual void release_auto_increment() { return; };
  
  virtual int check_for_upgrade(HA_CHECK_OPT *check_opt)
  { return 0; }
  virtual int check(THD* thd, HA_CHECK_OPT* check_opt)
  { return HA_ADMIN_NOT_IMPLEMENTED; }

  
  virtual int repair(THD* thd, HA_CHECK_OPT* check_opt)
  {
    DBUG_ASSERT(!(ha_table_flags() & HA_CAN_REPAIR));
    return HA_ADMIN_NOT_IMPLEMENTED;
  }
  virtual void start_bulk_insert(ha_rows rows, uint flags) {}
  virtual int end_bulk_insert() { return 0; }
protected:
  virtual int index_read(uchar * buf, const uchar * key, uint key_len,
                         enum ha_rkey_function find_flag)
   { return  HA_ERR_WRONG_COMMAND; }
  virtual int index_read_last(uchar * buf, const uchar * key, uint key_len)
  {
    my_errno= HA_ERR_WRONG_COMMAND;
    return HA_ERR_WRONG_COMMAND;
  }
  friend class ha_partition;
  friend class ha_sequence;
public:
  
  virtual int bulk_update_row(const uchar *old_data, const uchar *new_data,
                              ha_rows *dup_key_found)
  {
    DBUG_ASSERT(FALSE);
    return HA_ERR_WRONG_COMMAND;
  }
  
  virtual int delete_all_rows()
  { return (my_errno=HA_ERR_WRONG_COMMAND); }
  
  virtual int truncate()
  {
    int error= delete_all_rows();
    return error ? error : reset_auto_increment(0);
  }
  
  virtual int reset_auto_increment(ulonglong value)
  { return 0; }
  virtual int optimize(THD* thd, HA_CHECK_OPT* check_opt)
  { return HA_ADMIN_NOT_IMPLEMENTED; }
  virtual int analyze(THD* thd, HA_CHECK_OPT* check_opt)
  { return HA_ADMIN_NOT_IMPLEMENTED; }
  virtual bool check_and_repair(THD *thd) { return TRUE; }
  virtual int disable_indexes(uint mode) { return HA_ERR_WRONG_COMMAND; }
  virtual int enable_indexes(uint mode) { return HA_ERR_WRONG_COMMAND; }
  virtual int discard_or_import_tablespace(my_bool discard)
  { return (my_errno=HA_ERR_WRONG_COMMAND); }
  virtual void drop_table(const char *name);
  virtual int create(const char *name, TABLE *form, HA_CREATE_INFO *info)=0;

  virtual int create_partitioning_metadata(const char *name, const char *old_name,
                                   int action_flag)
  { return FALSE; }

  virtual int change_partitions(HA_CREATE_INFO *create_info,
                                const char *path,
                                ulonglong * const copied,
                                ulonglong * const deleted,
                                const uchar *pack_frm_data,
                                size_t pack_frm_len)
  { return HA_ERR_WRONG_COMMAND; }
  virtual int drop_partitions(const char *path)
  { return HA_ERR_WRONG_COMMAND; }
  virtual int rename_partitions(const char *path)
  { return HA_ERR_WRONG_COMMAND; }
  virtual bool set_ha_share_ref(Handler_share **arg_ha_share)
  {
    DBUG_ASSERT(!ha_share);
    DBUG_ASSERT(arg_ha_share);
    if (ha_share || !arg_ha_share)
      return true;
    ha_share= arg_ha_share;
    return false;
  }
  int get_lock_type() const { return m_lock_type; }
public:
  
  virtual handlerton *partition_ht() const
  { return ht; }
  inline int ha_write_tmp_row(uchar *buf);
  inline int ha_delete_tmp_row(uchar *buf);
  inline int ha_update_tmp_row(const uchar * old_data, uchar * new_data);

  virtual void set_lock_type(enum thr_lock_type lock);

  friend enum icp_result handler_index_cond_check(void* h_arg);

  
  virtual int find_unique_row(uchar *record, uint unique_ref)
  { return -1; }

  bool native_versioned() const
  { DBUG_ASSERT(ht); return partition_ht()->flags & HTON_NATIVE_SYS_VERSIONING; }
  virtual void update_partition(uint	part_id)
  {}
protected:
  Handler_share *get_ha_share_ptr();
  void set_ha_share_ptr(Handler_share *arg_ha_share);
  void lock_shared_ha_data();
  void unlock_shared_ha_data();
};

#include "multi_range_read.h"
#include "group_by_handler.h"

bool key_uses_partial_cols(TABLE_SHARE *table, uint keyno);

	

extern const char *ha_row_type[];
extern MYSQL_PLUGIN_IMPORT const char *tx_isolation_names[];
extern MYSQL_PLUGIN_IMPORT const char *binlog_format_names[];
extern TYPELIB tx_isolation_typelib;
extern const char *myisam_stats_method_names[];
extern ulong total_ha, total_ha_2pc;


plugin_ref ha_resolve_by_name(THD *thd, const LEX_CSTRING *name, bool tmp_table);
plugin_ref ha_lock_engine(THD *thd, const handlerton *hton);
handlerton *ha_resolve_by_legacy_type(THD *thd, enum legacy_db_type db_type);
handler *get_new_handler(TABLE_SHARE *share, MEM_ROOT *alloc,
                         handlerton *db_type);
handlerton *ha_checktype(THD *thd, handlerton *hton, bool no_substitute);

static inline handlerton *ha_checktype(THD *thd, enum legacy_db_type type,
                                       bool no_substitute = 0)
{
  return ha_checktype(thd, ha_resolve_by_legacy_type(thd, type), no_substitute);
}

static inline enum legacy_db_type ha_legacy_type(const handlerton *db_type)
{
  return (db_type == NULL) ? DB_TYPE_UNKNOWN : db_type->db_type;
}

static inline const char *ha_resolve_storage_engine_name(const handlerton *db_type)
{
  return db_type == NULL ? "UNKNOWN" : hton_name(db_type)->str;
}

static inline bool ha_check_storage_engine_flag(const handlerton *db_type, uint32 flag)
{
  return db_type == NULL ? FALSE : MY_TEST(db_type->flags & flag);
}

static inline bool ha_storage_engine_is_enabled(const handlerton *db_type)
{
  return (db_type && db_type->create) ?
         (db_type->state == SHOW_OPTION_YES) : FALSE;
}

#define view_pseudo_hton ((handlerton *)1)


int ha_init_errors(void);
int ha_init(void);
int ha_end(void);
int ha_initialize_handlerton(st_plugin_int *plugin);
int ha_finalize_handlerton(st_plugin_int *plugin);

TYPELIB *ha_known_exts(void);
int ha_panic(enum ha_panic_function flag);
void ha_close_connection(THD* thd);
void ha_kill_query(THD* thd, enum thd_kill_levels level);
bool ha_flush_logs(handlerton *db_type);
void ha_drop_database(char* path);
void ha_checkpoint_state(bool disable);
void ha_commit_checkpoint_request(void *cookie, void (*pre_hook)(void *));
int ha_create_table(THD *thd, const char *path,
                    const char *db, const char *table_name,
                    HA_CREATE_INFO *create_info, LEX_CUSTRING *frm);
int ha_delete_table(THD *thd, handlerton *db_type, const char *path,
                    const LEX_CSTRING *db, const LEX_CSTRING *alias, bool generate_warning);


bool ha_show_status(THD *thd, handlerton *db_type, enum ha_stat_type stat);


#ifdef MYSQL_SERVER
class Discovered_table_list: public handlerton::discovered_list
{
  THD *thd;
  const char *wild, *wend;
  bool with_temps; 
public:
  Dynamic_array<LEX_CSTRING*> *tables;

  Discovered_table_list(THD *thd_arg, Dynamic_array<LEX_CSTRING*> *tables_arg,
                        const LEX_CSTRING *wild_arg);
  Discovered_table_list(THD *thd_arg, Dynamic_array<LEX_CSTRING*> *tables_arg)
    : thd(thd_arg), wild(NULL), with_temps(true), tables(tables_arg) {}
  ~Discovered_table_list() {}

  bool add_table(const char *tname, size_t tlen);
  bool add_file(const char *fname);

  void sort();
  void remove_duplicates(); 
#ifndef DBUG_OFF
  
  void sort_desc();
#endif
};

int ha_discover_table(THD *thd, TABLE_SHARE *share);
int ha_discover_table_names(THD *thd, LEX_CSTRING *db, MY_DIR *dirp,
                            Discovered_table_list *result, bool reusable);
bool ha_table_exists(THD *thd, const LEX_CSTRING *db, const LEX_CSTRING *table_name,
                     handlerton **hton= 0, bool *is_sequence= 0);
#endif


extern "C" int ha_init_key_cache(const char *name, KEY_CACHE *key_cache, void *);
int ha_resize_key_cache(KEY_CACHE *key_cache);
int ha_change_key_cache_param(KEY_CACHE *key_cache);
int ha_repartition_key_cache(KEY_CACHE *key_cache);
int ha_change_key_cache(KEY_CACHE *old_key_cache, KEY_CACHE *new_key_cache);


int ha_start_consistent_snapshot(THD *thd);
int ha_commit_or_rollback_by_xid(XID *xid, bool commit);
int ha_commit_one_phase(THD *thd, bool all);
int ha_commit_trans(THD *thd, bool all);
int ha_rollback_trans(THD *thd, bool all);
int ha_prepare(THD *thd);
int ha_recover(HASH *commit_list);


int ha_enable_transaction(THD *thd, bool on);


int ha_rollback_to_savepoint(THD *thd, SAVEPOINT *sv);
bool ha_rollback_to_savepoint_can_release_mdl(THD *thd);
int ha_savepoint(THD *thd, SAVEPOINT *sv);
int ha_release_savepoint(THD *thd, SAVEPOINT *sv);
#ifdef WITH_WSREP
int ha_abort_transaction(THD *bf_thd, THD *victim_thd, my_bool signal);
void ha_fake_trx_id(THD *thd);
#else
inline void ha_fake_trx_id(THD *thd) { }
#endif


void trans_register_ha(THD *thd, bool all, handlerton *ht);


#define trans_need_2pc(thd, all)                   ((total_ha_2pc > 1) && \
        !((all ? &thd->transaction.all : &thd->transaction.stmt)->no_2pc))

const char *get_canonical_filename(handler *file, const char *path,
                                   char *tmp_path);
bool mysql_xa_recover(THD *thd);
void commit_checkpoint_notify_ha(handlerton *hton, void *cookie);

inline const LEX_CSTRING *table_case_name(HA_CREATE_INFO *info, const LEX_CSTRING *name)
{
  return ((lower_case_table_names == 2 && info->alias.str) ? &info->alias : name);
}

typedef bool Log_func(THD*, TABLE*, bool, const uchar*, const uchar*);
int binlog_log_row(TABLE* table,
                   const uchar *before_record,
                   const uchar *after_record,
                   Log_func *log_func);

#define TABLE_IO_WAIT(TRACKER, PSI, OP, INDEX, FLAGS, PAYLOAD) \
  { \
    Exec_time_tracker *this_tracker; \
    if (unlikely((this_tracker= tracker))) \
      tracker->start_tracking(); \
    \
    MYSQL_TABLE_IO_WAIT(PSI, OP, INDEX, FLAGS, PAYLOAD); \
    \
    if (unlikely(this_tracker)) \
      tracker->stop_tracking(); \
  }

void print_keydup_error(TABLE *table, KEY *key, const char *msg, myf errflag);
void print_keydup_error(TABLE *table, KEY *key, myf errflag);

int del_global_index_stat(THD *thd, TABLE* table, KEY* key_info);
int del_global_table_stat(THD *thd, const  LEX_CSTRING *db, const LEX_CSTRING *table);
#endif 
