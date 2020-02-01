



#ifndef _mysql_com_h
#define _mysql_com_h


#define NAME_CHAR_LEN   64
#define NAME_LEN	256		
#define HOSTNAME_LENGTH 60
#define SYSTEM_MB_MAX_CHAR_LENGTH 4
#define USERNAME_CHAR_LENGTH 128
#define USERNAME_LENGTH USERNAME_CHAR_LENGTH * SYSTEM_MB_MAX_CHAR_LENGTH
#define SERVER_VERSION_LENGTH 60
#define SQLSTATE_LENGTH 5
#define SCRAMBLE_LENGTH 20
#define SCRAMBLE_LENGTH_323 8

#define LOCAL_HOST	"localhost"
#define LOCAL_HOST_NAMEDPIPE "."

#if defined(_WIN32) && !defined( _CUSTOMCONFIG_)
#define MARIADB_NAMEDPIPE "MySQL"
#define MYSQL_SERVICENAME "MySql"
#endif 


#define MYSQL_AUTODETECT_CHARSET_NAME "auto"
#define BINCMP_FLAG       131072

enum mysql_enum_shutdown_level
{
  SHUTDOWN_DEFAULT = 0,
  KILL_QUERY= 254,
  KILL_CONNECTION= 255
};

enum enum_server_command
{
  COM_SLEEP = 0,
  COM_QUIT,
  COM_INIT_DB,
  COM_QUERY,
  COM_FIELD_LIST,
  COM_CREATE_DB,
  COM_DROP_DB,
  COM_REFRESH,
  COM_SHUTDOWN,
  COM_STATISTICS,
  COM_PROCESS_INFO,
  COM_CONNECT,
  COM_PROCESS_KILL,
  COM_DEBUG,
  COM_PING,
  COM_TIME = 15,
  COM_DELAYED_INSERT,
  COM_CHANGE_USER,
  COM_BINLOG_DUMP,
  COM_TABLE_DUMP,
  COM_CONNECT_OUT = 20,
  COM_REGISTER_SLAVE,
  COM_STMT_PREPARE = 22,
  COM_STMT_EXECUTE = 23,
  COM_STMT_SEND_LONG_DATA = 24,
  COM_STMT_CLOSE = 25,
  COM_STMT_RESET = 26,
  COM_SET_OPTION = 27,
  COM_STMT_FETCH = 28,
  COM_DAEMON= 29,
  COM_UNSUPPORTED= 30,
  COM_RESET_CONNECTION = 31,
  COM_STMT_BULK_EXECUTE = 250,
  COM_MULTI = 254,
  COM_END
};


#define NOT_NULL_FLAG	1		
#define PRI_KEY_FLAG	2		
#define UNIQUE_KEY_FLAG 4		
#define MULTIPLE_KEY_FLAG 8		
#define BLOB_FLAG	16		
#define UNSIGNED_FLAG	32		
#define ZEROFILL_FLAG	64		
#define BINARY_FLAG	128

#define ENUM_FLAG	256		
#define AUTO_INCREMENT_FLAG 512		
#define TIMESTAMP_FLAG	1024		
#define SET_FLAG	2048		

#define NO_DEFAULT_VALUE_FLAG 4096	
#define ON_UPDATE_NOW_FLAG 8192         

#define NUM_FLAG	32768		
#define PART_KEY_FLAG	16384		
#define GROUP_FLAG	32768		
#define UNIQUE_FLAG	65536		

#define REFRESH_GRANT		1	
#define REFRESH_LOG		2	
#define REFRESH_TABLES		4	
#define REFRESH_HOSTS		8	
#define REFRESH_STATUS		16	
#define REFRESH_THREADS		32	
#define REFRESH_SLAVE           64      
#define REFRESH_MASTER          128     


#define REFRESH_READ_LOCK	16384	
#define REFRESH_FAST		32768	

#define CLIENT_MYSQL          1
#define CLIENT_FOUND_ROWS	    2	
#define CLIENT_LONG_FLAG	    4	
#define CLIENT_CONNECT_WITH_DB	    8	
#define CLIENT_NO_SCHEMA	   16	
#define CLIENT_COMPRESS		   32	
#define CLIENT_ODBC		   64	
#define CLIENT_LOCAL_FILES	  128	
#define CLIENT_IGNORE_SPACE	  256	
#define CLIENT_INTERACTIVE	  1024	
#define CLIENT_SSL                2048     
#define CLIENT_IGNORE_SIGPIPE     4096     
#define CLIENT_TRANSACTIONS	  8192	

#define CLIENT_PROTOCOL_41         512
#define CLIENT_RESERVED          16384
#define CLIENT_SECURE_CONNECTION 32768  
#define CLIENT_MULTI_STATEMENTS  (1UL << 16)
#define CLIENT_MULTI_RESULTS     (1UL << 17)
#define CLIENT_PS_MULTI_RESULTS  (1UL << 18)
#define CLIENT_PLUGIN_AUTH       (1UL << 19)
#define CLIENT_CONNECT_ATTRS     (1UL << 20)
#define CLIENT_CAN_HANDLE_EXPIRED_PASSWORDS (1UL << 22)
#define CLIENT_SESSION_TRACKING  (1UL << 23)
#define CLIENT_PROGRESS          (1UL << 29) 
#define CLIENT_PROGRESS_OBSOLETE  CLIENT_PROGRESS 
#define CLIENT_SSL_VERIFY_SERVER_CERT (1UL << 30)
#define CLIENT_REMEMBER_OPTIONS  (1UL << 31)


#define MARIADB_CLIENT_FLAGS 0xFFFFFFFF00000000ULL
#define MARIADB_CLIENT_PROGRESS (1ULL << 32)
#define MARIADB_CLIENT_COM_MULTI (1ULL << 33)
#define MARIADB_CLIENT_STMT_BULK_OPERATIONS (1ULL << 34)

#define IS_MARIADB_EXTENDED_SERVER(mysql)\
        !(mysql->server_capabilities & CLIENT_MYSQL)

#define MARIADB_CLIENT_SUPPORTED_FLAGS (MARIADB_CLIENT_PROGRESS |\
                                       MARIADB_CLIENT_COM_MULTI |\
                                       MARIADB_CLIENT_STMT_BULK_OPERATIONS)

#define CLIENT_SUPPORTED_FLAGS  (CLIENT_MYSQL |\
                                 CLIENT_FOUND_ROWS |\
                                 CLIENT_LONG_FLAG |\
                                 CLIENT_CONNECT_WITH_DB |\
                                 CLIENT_NO_SCHEMA |\
                                 CLIENT_COMPRESS |\
                                 CLIENT_ODBC |\
                                 CLIENT_LOCAL_FILES |\
                                 CLIENT_IGNORE_SPACE |\
                                 CLIENT_INTERACTIVE |\
                                 CLIENT_SSL |\
                                 CLIENT_IGNORE_SIGPIPE |\
                                 CLIENT_TRANSACTIONS |\
                                 CLIENT_PROTOCOL_41 |\
                                 CLIENT_RESERVED |\
                                 CLIENT_SECURE_CONNECTION |\
                                 CLIENT_MULTI_STATEMENTS |\
                                 CLIENT_MULTI_RESULTS |\
                                 CLIENT_PROGRESS |\
		                 CLIENT_SSL_VERIFY_SERVER_CERT |\
                                 CLIENT_REMEMBER_OPTIONS |\
                                 CLIENT_PLUGIN_AUTH |\
                                 CLIENT_SESSION_TRACKING |\
                                 CLIENT_CONNECT_ATTRS)

#define CLIENT_CAPABILITIES	(CLIENT_MYSQL | \
                                 CLIENT_LONG_FLAG |\
                                 CLIENT_TRANSACTIONS |\
                                 CLIENT_SECURE_CONNECTION |\
                                 CLIENT_MULTI_RESULTS | \
                                 CLIENT_PS_MULTI_RESULTS |\
                                 CLIENT_PROTOCOL_41 |\
                                 CLIENT_PLUGIN_AUTH |\
                                 CLIENT_SESSION_TRACKING |\
                                 CLIENT_CONNECT_ATTRS)

#define CLIENT_DEFAULT_FLAGS ((CLIENT_SUPPORTED_FLAGS & ~CLIENT_COMPRESS)\
                                                      & ~CLIENT_SSL)

#define SERVER_STATUS_IN_TRANS               1	
#define SERVER_STATUS_AUTOCOMMIT             2	
#define SERVER_MORE_RESULTS_EXIST            8
#define SERVER_QUERY_NO_GOOD_INDEX_USED     16
#define SERVER_QUERY_NO_INDEX_USED          32
#define SERVER_STATUS_CURSOR_EXISTS         64
#define SERVER_STATUS_LAST_ROW_SENT        128
#define SERVER_STATUS_DB_DROPPED           256 
#define SERVER_STATUS_NO_BACKSLASH_ESCAPES 512
#define SERVER_STATUS_METADATA_CHANGED    1024
#define SERVER_QUERY_WAS_SLOW             2048
#define SERVER_PS_OUT_PARAMS              4096
#define SERVER_STATUS_IN_TRANS_READONLY   8192
#define SERVER_SESSION_STATE_CHANGED     16384
#define SERVER_STATUS_ANSI_QUOTES        32768

#define MYSQL_ERRMSG_SIZE	512
#define NET_READ_TIMEOUT	30		
#define NET_WRITE_TIMEOUT	60		
#define NET_WAIT_TIMEOUT	8*60*60		


#define LIST_PROCESS_HOST_LEN 64
#define MYSQL50_TABLE_NAME_PREFIX         "#mysql50#"
#define MYSQL50_TABLE_NAME_PREFIX_LENGTH  (sizeof(MYSQL50_TABLE_NAME_PREFIX)-1)
#define SAFE_NAME_LEN (NAME_LEN + MYSQL50_TABLE_NAME_PREFIX_LENGTH)

struct st_ma_pvio;
typedef struct st_ma_pvio MARIADB_PVIO;

#define MAX_CHAR_WIDTH		255	
#define MAX_BLOB_WIDTH		8192	


#define MAX_TINYINT_WIDTH     3
#define MAX_SMALLINT_WIDTH    5
#define MAX_MEDIUMINT_WIDTH   8
#define MAX_INT_WIDTH        10
#define MAX_BIGINT_WIDTH     20

struct st_ma_connection_plugin;


typedef struct st_net {
  MARIADB_PVIO *pvio;
  unsigned char *buff;
  unsigned char *buff_end,*write_pos,*read_pos;
  my_socket fd;					
  unsigned long remain_in_buf,length;
  unsigned long buf_length, where_b;
  unsigned long max_packet, max_packet_size;
  unsigned int pkt_nr, compress_pkt_nr;
  unsigned int write_timeout, read_timeout, retry_count;
  int fcntl;
  unsigned int *return_status;
  unsigned char reading_or_writing;
  char save_char;
  char unused_1;
  my_bool unused_2;
  my_bool compress;
  my_bool unused_3;
  void *unused_4;
  unsigned int last_errno;
  unsigned char error;
  my_bool unused_5;
  my_bool unused_6;
  char last_error[MYSQL_ERRMSG_SIZE];
  char sqlstate[SQLSTATE_LENGTH+1];
  struct st_mariadb_net_extension *extension;
} NET;

#define packet_error ((unsigned int) -1)


enum enum_mysql_set_option
{
  MYSQL_OPTION_MULTI_STATEMENTS_ON,
  MYSQL_OPTION_MULTI_STATEMENTS_OFF
};

enum enum_session_state_type
{
  SESSION_TRACK_SYSTEM_VARIABLES= 0,
  SESSION_TRACK_SCHEMA,
  SESSION_TRACK_STATE_CHANGE,
  
  SESSION_TRACK_GTIDS,
  SESSION_TRACK_TRANSACTION_CHARACTERISTICS,
  SESSION_TRACK_TRANSACTION_TYPE 
};

#define SESSION_TRACK_BEGIN 0
#define SESSION_TRACK_END SESSION_TRACK_TRANSACTION_TYPE
#define SESSION_TRACK_TYPES SESSION_TRACK_END + 1

enum enum_field_types { MYSQL_TYPE_DECIMAL, MYSQL_TYPE_TINY,
                        MYSQL_TYPE_SHORT,  MYSQL_TYPE_LONG,
                        MYSQL_TYPE_FLOAT,  MYSQL_TYPE_DOUBLE,
                        MYSQL_TYPE_NULL,   MYSQL_TYPE_TIMESTAMP,
                        MYSQL_TYPE_LONGLONG,MYSQL_TYPE_INT24,
                        MYSQL_TYPE_DATE,   MYSQL_TYPE_TIME,
                        MYSQL_TYPE_DATETIME, MYSQL_TYPE_YEAR,
                        MYSQL_TYPE_NEWDATE, MYSQL_TYPE_VARCHAR,
                        MYSQL_TYPE_BIT,
                        
                        MYSQL_TYPE_TIMESTAMP2,
                        MYSQL_TYPE_DATETIME2,
                        MYSQL_TYPE_TIME2,
                        
                        MYSQL_TYPE_JSON=245,
                        MYSQL_TYPE_NEWDECIMAL=246,
                        MYSQL_TYPE_ENUM=247,
                        MYSQL_TYPE_SET=248,
                        MYSQL_TYPE_TINY_BLOB=249,
                        MYSQL_TYPE_MEDIUM_BLOB=250,
                        MYSQL_TYPE_LONG_BLOB=251,
                        MYSQL_TYPE_BLOB=252,
                        MYSQL_TYPE_VAR_STRING=253,
                        MYSQL_TYPE_STRING=254,
                        MYSQL_TYPE_GEOMETRY=255,
                        MAX_NO_FIELD_TYPES };

#define FIELD_TYPE_CHAR FIELD_TYPE_TINY		
#define FIELD_TYPE_INTERVAL FIELD_TYPE_ENUM	
#define FIELD_TYPE_DECIMAL MYSQL_TYPE_DECIMAL
#define FIELD_TYPE_NEWDECIMAL MYSQL_TYPE_NEWDECIMAL
#define FIELD_TYPE_TINY MYSQL_TYPE_TINY
#define FIELD_TYPE_SHORT MYSQL_TYPE_SHORT
#define FIELD_TYPE_LONG MYSQL_TYPE_LONG
#define FIELD_TYPE_FLOAT MYSQL_TYPE_FLOAT
#define FIELD_TYPE_DOUBLE MYSQL_TYPE_DOUBLE
#define FIELD_TYPE_NULL MYSQL_TYPE_NULL
#define FIELD_TYPE_TIMESTAMP MYSQL_TYPE_TIMESTAMP
#define FIELD_TYPE_LONGLONG MYSQL_TYPE_LONGLONG
#define FIELD_TYPE_INT24 MYSQL_TYPE_INT24
#define FIELD_TYPE_DATE MYSQL_TYPE_DATE
#define FIELD_TYPE_TIME MYSQL_TYPE_TIME
#define FIELD_TYPE_DATETIME MYSQL_TYPE_DATETIME
#define FIELD_TYPE_YEAR MYSQL_TYPE_YEAR
#define FIELD_TYPE_NEWDATE MYSQL_TYPE_NEWDATE
#define FIELD_TYPE_ENUM MYSQL_TYPE_ENUM
#define FIELD_TYPE_SET MYSQL_TYPE_SET
#define FIELD_TYPE_TINY_BLOB MYSQL_TYPE_TINY_BLOB
#define FIELD_TYPE_MEDIUM_BLOB MYSQL_TYPE_MEDIUM_BLOB
#define FIELD_TYPE_LONG_BLOB MYSQL_TYPE_LONG_BLOB
#define FIELD_TYPE_BLOB MYSQL_TYPE_BLOB
#define FIELD_TYPE_VAR_STRING MYSQL_TYPE_VAR_STRING
#define FIELD_TYPE_STRING MYSQL_TYPE_STRING
#define FIELD_TYPE_GEOMETRY MYSQL_TYPE_GEOMETRY
#define FIELD_TYPE_BIT MYSQL_TYPE_BIT

extern unsigned long max_allowed_packet;
extern unsigned long net_buffer_length;

#define net_new_transaction(net) ((net)->pkt_nr=0)

int	ma_net_init(NET *net, MARIADB_PVIO *pvio);
void	ma_net_end(NET *net);
void	ma_net_clear(NET *net);
int	ma_net_flush(NET *net);
int	ma_net_write(NET *net,const unsigned char *packet, size_t len);
int	ma_net_write_command(NET *net,unsigned char command,const char *packet,
			  size_t len, my_bool disable_flush);
int	ma_net_real_write(NET *net,const char *packet, size_t len);
extern unsigned long ma_net_read(NET *net);

struct rand_struct {
  unsigned long seed1,seed2,max_value;
  double max_value_dbl;
};

  

enum Item_result {STRING_RESULT,REAL_RESULT,INT_RESULT,ROW_RESULT,DECIMAL_RESULT};

typedef struct st_udf_args
{
  unsigned int arg_count;		
  enum Item_result *arg_type;		
  char **args;				
  unsigned long *lengths;		
  char *maybe_null;			
} UDF_ARGS;

  

typedef struct st_udf_init
{
  my_bool maybe_null;			
  unsigned int decimals;		
  unsigned int max_length;		
  char	  *ptr;				
  my_bool const_item;			
} UDF_INIT;


#define MARIADB_CONNECTION_UNIXSOCKET   0
#define MARIADB_CONNECTION_TCP          1
#define MARIADB_CONNECTION_NAMEDPIPE    2
#define MARIADB_CONNECTION_SHAREDMEM    3

  
#define NET_HEADER_SIZE 4		
#define COMP_HEADER_SIZE 3		

  
#define native_password_plugin_name "mysql_native_password"
#define old_password_plugin_name    "mysql_old_password"

#ifdef __cplusplus
extern "C" {
#endif
  
char *ma_scramble_323(char *to,const char *message,const char *password);
void ma_scramble_41(const unsigned char *buffer, const char *scramble, const char *password);
void ma_hash_password(unsigned long *result, const char *password, size_t len);
void ma_make_scrambled_password(char *to,const char *password);



void mariadb_load_defaults(const char *conf_file, const char **groups,
		   int *argc, char ***argv);
my_bool ma_thread_init(void);
void ma_thread_end(void);

#ifdef __cplusplus
}
#endif

#define NULL_LENGTH ((unsigned long) ~0) 

#endif
