



#ifndef _mysql_com_h
#define _mysql_com_h

#include "my_decimal_limits.h"

#define HOSTNAME_LENGTH 60
#define SYSTEM_CHARSET_MBMAXLEN 3
#define NAME_CHAR_LEN	64U             
#define USERNAME_CHAR_LENGTH 128U
#define NAME_LEN                (NAME_CHAR_LEN*SYSTEM_CHARSET_MBMAXLEN)
#define USERNAME_LENGTH         (USERNAME_CHAR_LENGTH*SYSTEM_CHARSET_MBMAXLEN)
#define DEFINER_CHAR_LENGTH     (USERNAME_CHAR_LENGTH + HOSTNAME_LENGTH + 1)
#define DEFINER_LENGTH          (USERNAME_LENGTH + HOSTNAME_LENGTH + 1)

#define MYSQL_AUTODETECT_CHARSET_NAME "auto"

#define MYSQL50_TABLE_NAME_PREFIX         "#mysql50#"
#define MYSQL50_TABLE_NAME_PREFIX_LENGTH  (sizeof(MYSQL50_TABLE_NAME_PREFIX)-1)
#define SAFE_NAME_LEN (NAME_LEN + MYSQL50_TABLE_NAME_PREFIX_LENGTH)



#define RPL_VERSION_HACK "5.5.5-"

#define SERVER_VERSION_LENGTH 60
#define SQLSTATE_LENGTH 5
#define LIST_PROCESS_HOST_LEN 64


#define TABLE_COMMENT_INLINE_MAXLEN 180 
#define TABLE_COMMENT_MAXLEN 2048
#define COLUMN_COMMENT_MAXLEN 1024
#define INDEX_COMMENT_MAXLEN 1024
#define TABLE_PARTITION_COMMENT_MAXLEN 1024


#define MAX_PACKET_LENGTH (256L*256L*256L-1)


#define USER_HOST_BUFF_SIZE HOSTNAME_LENGTH + USERNAME_LENGTH + 2

#define LOCAL_HOST	"localhost"
#define LOCAL_HOST_NAMEDPIPE "."


#if defined(__WIN__) && !defined( _CUSTOMCONFIG_)
#define MYSQL_NAMEDPIPE "MySQL"
#define MYSQL_SERVICENAME "MySQL"
#endif 



enum enum_server_command
{
  COM_SLEEP, COM_QUIT, COM_INIT_DB, COM_QUERY, COM_FIELD_LIST,
  COM_CREATE_DB, COM_DROP_DB, COM_REFRESH, COM_SHUTDOWN, COM_STATISTICS,
  COM_PROCESS_INFO, COM_CONNECT, COM_PROCESS_KILL, COM_DEBUG, COM_PING,
  COM_TIME, COM_DELAYED_INSERT, COM_CHANGE_USER, COM_BINLOG_DUMP,
  COM_TABLE_DUMP, COM_CONNECT_OUT, COM_REGISTER_SLAVE,
  COM_STMT_PREPARE, COM_STMT_EXECUTE, COM_STMT_SEND_LONG_DATA, COM_STMT_CLOSE,
  COM_STMT_RESET, COM_SET_OPTION, COM_STMT_FETCH, COM_DAEMON,
  COM_UNIMPLEMENTED, 
  COM_RESET_CONNECTION,
  
  COM_MDB_GAP_BEG,
  COM_MDB_GAP_END=249,
  COM_STMT_BULK_EXECUTE=250,
  COM_SLAVE_WORKER=251,
  COM_SLAVE_IO=252,
  COM_SLAVE_SQL=253,
  COM_MULTI=254,
  
  COM_END=255
};



enum enum_indicator_type
{
  STMT_INDICATOR_NONE= 0,
  STMT_INDICATOR_NULL,
  STMT_INDICATOR_DEFAULT,
  STMT_INDICATOR_IGNORE
};


#define STMT_BULK_FLAG_CLIENT_SEND_TYPES 128
#define STMT_BULK_FLAG_INSERT_ID_REQUEST 64



#define MYSQL_TYPE_VIRTUAL 245

#define SCRAMBLE_LENGTH 20
#define SCRAMBLE_LENGTH_323 8

#define SCRAMBLED_PASSWORD_CHAR_LENGTH (SCRAMBLE_LENGTH*2+1)
#define SCRAMBLED_PASSWORD_CHAR_LENGTH_323 (SCRAMBLE_LENGTH_323*2)


#define NOT_NULL_FLAG	1U		
#define PRI_KEY_FLAG	2U		
#define UNIQUE_KEY_FLAG 4U		
#define MULTIPLE_KEY_FLAG 8U		
#define BLOB_FLAG	16U		
#define UNSIGNED_FLAG	32U		
#define ZEROFILL_FLAG	64U		
#define BINARY_FLAG	128U		


#define ENUM_FLAG	256U		
#define AUTO_INCREMENT_FLAG 512U	
#define TIMESTAMP_FLAG	1024U		
#define SET_FLAG	2048U		
#define NO_DEFAULT_VALUE_FLAG 4096U	
#define ON_UPDATE_NOW_FLAG 8192U	
#define NUM_FLAG	32768U		
#define PART_KEY_FLAG	16384U		
#define GROUP_FLAG	32768U		
#define BINCMP_FLAG	131072U		
#define GET_FIXED_FIELDS_FLAG (1U << 18) 
#define FIELD_IN_PART_FUNC_FLAG (1U << 19)
#define PART_INDIRECT_KEY_FLAG (1U << 20)


#define FIELD_IN_ADD_INDEX (1U << 20)
#define FIELD_IS_RENAMED (1U << 21)     
#define FIELD_FLAGS_STORAGE_MEDIA 22    
#define FIELD_FLAGS_STORAGE_MEDIA_MASK (3U << FIELD_FLAGS_STORAGE_MEDIA)
#define FIELD_FLAGS_COLUMN_FORMAT 24    
#define FIELD_FLAGS_COLUMN_FORMAT_MASK (3U << FIELD_FLAGS_COLUMN_FORMAT)
#define FIELD_IS_DROPPED (1U << 26)     

#define VERS_SYS_START_FLAG (1 << 27)   
#define VERS_SYS_END_FLAG (1 << 28)     
#define VERS_SYSTEM_FIELD (VERS_SYS_START_FLAG | VERS_SYS_END_FLAG)
#define VERS_UPDATE_UNVERSIONED_FLAG (1 << 29) 

#define REFRESH_GRANT           (1ULL << 0)  
#define REFRESH_LOG             (1ULL << 1)  
#define REFRESH_TABLES          (1ULL << 2)  
#define REFRESH_HOSTS           (1ULL << 3)  
#define REFRESH_STATUS          (1ULL << 4)  
#define REFRESH_THREADS         (1ULL << 5)  
#define REFRESH_SLAVE           (1ULL << 6)  
#define REFRESH_MASTER          (1ULL << 7)  


#define REFRESH_ERROR_LOG       (1ULL << 8)  
#define REFRESH_ENGINE_LOG      (1ULL << 9)  
#define REFRESH_BINARY_LOG      (1ULL << 10) 
#define REFRESH_RELAY_LOG       (1ULL << 11) 
#define REFRESH_GENERAL_LOG     (1ULL << 12) 
#define REFRESH_SLOW_LOG        (1ULL << 13) 

#define REFRESH_READ_LOCK       (1ULL << 14) 
#define REFRESH_CHECKPOINT      (1ULL << 15) 

#define REFRESH_QUERY_CACHE     (1ULL << 16) 
#define REFRESH_QUERY_CACHE_FREE (1ULL << 17) 
#define REFRESH_DES_KEY_FILE    (1ULL << 18)
#define REFRESH_USER_RESOURCES  (1ULL << 19)
#define REFRESH_FOR_EXPORT      (1ULL << 20) 

#define REFRESH_GENERIC         (1ULL << 30)
#define REFRESH_FAST            (1ULL << 31) 

#define CLIENT_LONG_PASSWORD	0	
#define CLIENT_MYSQL            1ULL       
#define CLIENT_FOUND_ROWS	2ULL	
#define CLIENT_LONG_FLAG	4ULL	
#define CLIENT_CONNECT_WITH_DB	8ULL	
#define CLIENT_NO_SCHEMA	16ULL	
#define CLIENT_COMPRESS		32ULL	
#define CLIENT_ODBC		64ULL	
#define CLIENT_LOCAL_FILES	128ULL	
#define CLIENT_IGNORE_SPACE	256ULL	
#define CLIENT_PROTOCOL_41	512ULL	
#define CLIENT_INTERACTIVE	1024ULL	
#define CLIENT_SSL              2048ULL	
#define CLIENT_IGNORE_SIGPIPE   4096ULL    
#define CLIENT_TRANSACTIONS	8192ULL	
#define CLIENT_RESERVED         16384ULL   
#define CLIENT_SECURE_CONNECTION 32768ULL  
#define CLIENT_MULTI_STATEMENTS (1ULL << 16) 
#define CLIENT_MULTI_RESULTS    (1ULL << 17) 
#define CLIENT_PS_MULTI_RESULTS (1ULL << 18) 

#define CLIENT_PLUGIN_AUTH  (1ULL << 19) 
#define CLIENT_CONNECT_ATTRS (1ULL << 20) 

#define CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA (1ULL << 21)

#define CLIENT_CAN_HANDLE_EXPIRED_PASSWORDS (1ULL << 22)


#define CLIENT_SESSION_TRACK (1ULL << 23)

#define CLIENT_DEPRECATE_EOF (1ULL << 24)

#define CLIENT_PROGRESS_OBSOLETE  (1ULL << 29)
#define CLIENT_SSL_VERIFY_SERVER_CERT (1ULL << 30)

#define CLIENT_REMEMBER_OPTIONS (1ULL << 31)


#define MARIADB_CLIENT_FLAGS_MASK 0xffffffff00000000ULL

#define MARIADB_CLIENT_PROGRESS (1ULL << 32)

#define MARIADB_CLIENT_COM_MULTI (1ULL << 33)

#define MARIADB_CLIENT_STMT_BULK_OPERATIONS (1ULL << 34)

#ifdef HAVE_COMPRESS
#define CAN_CLIENT_COMPRESS CLIENT_COMPRESS
#else
#define CAN_CLIENT_COMPRESS 0
#endif


#define CLIENT_ALL_FLAGS  (\
                           CLIENT_FOUND_ROWS | \
                           CLIENT_LONG_FLAG | \
                           CLIENT_CONNECT_WITH_DB | \
                           CLIENT_NO_SCHEMA | \
                           CLIENT_COMPRESS | \
                           CLIENT_ODBC | \
                           CLIENT_LOCAL_FILES | \
                           CLIENT_IGNORE_SPACE | \
                           CLIENT_PROTOCOL_41 | \
                           CLIENT_INTERACTIVE | \
                           CLIENT_SSL | \
                           CLIENT_IGNORE_SIGPIPE | \
                           CLIENT_TRANSACTIONS | \
                           CLIENT_RESERVED | \
                           CLIENT_SECURE_CONNECTION | \
                           CLIENT_MULTI_STATEMENTS | \
                           CLIENT_MULTI_RESULTS | \
                           CLIENT_PS_MULTI_RESULTS | \
                           CLIENT_SSL_VERIFY_SERVER_CERT | \
                           CLIENT_REMEMBER_OPTIONS | \
                           MARIADB_CLIENT_PROGRESS | \
                           CLIENT_PLUGIN_AUTH | \
                           CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA | \
                           CLIENT_SESSION_TRACK |\
                           CLIENT_DEPRECATE_EOF |\
                           CLIENT_CONNECT_ATTRS |\
                           MARIADB_CLIENT_COM_MULTI |\
                           MARIADB_CLIENT_STMT_BULK_OPERATIONS)




#define CLIENT_BASIC_FLAGS (((CLIENT_ALL_FLAGS & ~CLIENT_SSL) \
                                               & ~CLIENT_COMPRESS) \
                                               & ~CLIENT_SSL_VERIFY_SERVER_CERT)


#define SERVER_STATUS_IN_TRANS     1U
#define SERVER_STATUS_AUTOCOMMIT   2U	
#define SERVER_MORE_RESULTS_EXISTS 8U   
#define SERVER_QUERY_NO_GOOD_INDEX_USED 16U
#define SERVER_QUERY_NO_INDEX_USED      32U

#define SERVER_STATUS_CURSOR_EXISTS 64U

#define SERVER_STATUS_LAST_ROW_SENT 128U
#define SERVER_STATUS_DB_DROPPED        256U 
#define SERVER_STATUS_NO_BACKSLASH_ESCAPES 512U

#define SERVER_STATUS_METADATA_CHANGED 1024U
#define SERVER_QUERY_WAS_SLOW          2048U


#define SERVER_PS_OUT_PARAMS            4096U


#define SERVER_STATUS_IN_TRANS_READONLY 8192U


#define SERVER_SESSION_STATE_CHANGED    16384U

#define SERVER_STATUS_ANSI_QUOTES       32768U


#define SERVER_STATUS_CLEAR_SET (SERVER_QUERY_NO_GOOD_INDEX_USED| \
                                 SERVER_QUERY_NO_INDEX_USED|\
                                 SERVER_MORE_RESULTS_EXISTS|\
                                 SERVER_STATUS_METADATA_CHANGED |\
                                 SERVER_QUERY_WAS_SLOW |\
                                 SERVER_STATUS_DB_DROPPED |\
                                 SERVER_STATUS_CURSOR_EXISTS|\
                                 SERVER_STATUS_LAST_ROW_SENT|\
                                 SERVER_SESSION_STATE_CHANGED)

#define MYSQL_ERRMSG_SIZE	512
#define NET_READ_TIMEOUT	30		
#define NET_WRITE_TIMEOUT	60		
#define NET_WAIT_TIMEOUT	8*60*60		

struct st_vio;					
typedef struct st_vio Vio;

#define MAX_TINYINT_WIDTH       3       
#define MAX_SMALLINT_WIDTH      5       
#define MAX_MEDIUMINT_WIDTH     8       
#define MAX_INT_WIDTH           10      
#define MAX_BIGINT_WIDTH        20      
#define MAX_CHAR_WIDTH		255	
#define MAX_BLOB_WIDTH		16777216	

typedef struct st_net {
#if !defined(CHECK_EMBEDDED_DIFFERENCES) || !defined(EMBEDDED_LIBRARY)
  Vio *vio;
  unsigned char *buff,*buff_end,*write_pos,*read_pos;
  my_socket fd;					
  
  unsigned long remain_in_buf,length, buf_length, where_b;
  unsigned long max_packet,max_packet_size;
  unsigned int pkt_nr,compress_pkt_nr;
  unsigned int write_timeout, read_timeout, retry_count;
  int fcntl;
  unsigned int *return_status;
  unsigned char reading_or_writing;
  char save_char;
  char net_skip_rest_factor;
  my_bool thread_specific_malloc;
  unsigned char compress;
  my_bool unused3; 
  
#endif
  void *thd; 	   
  unsigned int last_errno;
  unsigned char error; 
  my_bool unused4; 
  my_bool unused5; 
  
  char last_error[MYSQL_ERRMSG_SIZE];
  
  char sqlstate[SQLSTATE_LENGTH+1];
  void *extension;
} NET;


#define packet_error ~0UL

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
                        
                        MYSQL_TYPE_BLOB_COMPRESSED= 140,
                        MYSQL_TYPE_VARCHAR_COMPRESSED= 141,

                        MYSQL_TYPE_NEWDECIMAL=246,
			MYSQL_TYPE_ENUM=247,
			MYSQL_TYPE_SET=248,
			MYSQL_TYPE_TINY_BLOB=249,
			MYSQL_TYPE_MEDIUM_BLOB=250,
			MYSQL_TYPE_LONG_BLOB=251,
			MYSQL_TYPE_BLOB=252,
			MYSQL_TYPE_VAR_STRING=253,
			MYSQL_TYPE_STRING=254,
			MYSQL_TYPE_GEOMETRY=255

};


#define CLIENT_MULTI_QUERIES    CLIENT_MULTI_STATEMENTS    
#define FIELD_TYPE_DECIMAL     MYSQL_TYPE_DECIMAL
#define FIELD_TYPE_NEWDECIMAL  MYSQL_TYPE_NEWDECIMAL
#define FIELD_TYPE_TINY        MYSQL_TYPE_TINY
#define FIELD_TYPE_SHORT       MYSQL_TYPE_SHORT
#define FIELD_TYPE_LONG        MYSQL_TYPE_LONG
#define FIELD_TYPE_FLOAT       MYSQL_TYPE_FLOAT
#define FIELD_TYPE_DOUBLE      MYSQL_TYPE_DOUBLE
#define FIELD_TYPE_NULL        MYSQL_TYPE_NULL
#define FIELD_TYPE_TIMESTAMP   MYSQL_TYPE_TIMESTAMP
#define FIELD_TYPE_LONGLONG    MYSQL_TYPE_LONGLONG
#define FIELD_TYPE_INT24       MYSQL_TYPE_INT24
#define FIELD_TYPE_DATE        MYSQL_TYPE_DATE
#define FIELD_TYPE_TIME        MYSQL_TYPE_TIME
#define FIELD_TYPE_DATETIME    MYSQL_TYPE_DATETIME
#define FIELD_TYPE_YEAR        MYSQL_TYPE_YEAR
#define FIELD_TYPE_NEWDATE     MYSQL_TYPE_NEWDATE
#define FIELD_TYPE_ENUM        MYSQL_TYPE_ENUM
#define FIELD_TYPE_SET         MYSQL_TYPE_SET
#define FIELD_TYPE_TINY_BLOB   MYSQL_TYPE_TINY_BLOB
#define FIELD_TYPE_MEDIUM_BLOB MYSQL_TYPE_MEDIUM_BLOB
#define FIELD_TYPE_LONG_BLOB   MYSQL_TYPE_LONG_BLOB
#define FIELD_TYPE_BLOB        MYSQL_TYPE_BLOB
#define FIELD_TYPE_VAR_STRING  MYSQL_TYPE_VAR_STRING
#define FIELD_TYPE_STRING      MYSQL_TYPE_STRING
#define FIELD_TYPE_CHAR        MYSQL_TYPE_TINY
#define FIELD_TYPE_INTERVAL    MYSQL_TYPE_ENUM
#define FIELD_TYPE_GEOMETRY    MYSQL_TYPE_GEOMETRY
#define FIELD_TYPE_BIT         MYSQL_TYPE_BIT


 


#define MYSQL_SHUTDOWN_KILLABLE_CONNECT    (unsigned char)(1 << 0)
#define MYSQL_SHUTDOWN_KILLABLE_TRANS      (unsigned char)(1 << 1)
#define MYSQL_SHUTDOWN_KILLABLE_LOCK_TABLE (unsigned char)(1 << 2)
#define MYSQL_SHUTDOWN_KILLABLE_UPDATE     (unsigned char)(1 << 3)

enum mysql_enum_shutdown_level {
  
  SHUTDOWN_DEFAULT = 0,
  
  SHUTDOWN_WAIT_CONNECTIONS= MYSQL_SHUTDOWN_KILLABLE_CONNECT,
  
  SHUTDOWN_WAIT_TRANSACTIONS= MYSQL_SHUTDOWN_KILLABLE_TRANS,
  
  SHUTDOWN_WAIT_UPDATES= MYSQL_SHUTDOWN_KILLABLE_UPDATE,
  
  SHUTDOWN_WAIT_ALL_BUFFERS= (MYSQL_SHUTDOWN_KILLABLE_UPDATE << 1),
  
  SHUTDOWN_WAIT_CRITICAL_BUFFERS= (MYSQL_SHUTDOWN_KILLABLE_UPDATE << 1) + 1
};

enum enum_cursor_type
{
  CURSOR_TYPE_NO_CURSOR= 0,
  CURSOR_TYPE_READ_ONLY= 1,
  CURSOR_TYPE_FOR_UPDATE= 2,
  CURSOR_TYPE_SCROLLABLE= 4
};



enum enum_mysql_set_option
{
  MYSQL_OPTION_MULTI_STATEMENTS_ON,
  MYSQL_OPTION_MULTI_STATEMENTS_OFF
};


enum enum_session_state_type
{
  SESSION_TRACK_SYSTEM_VARIABLES,             
  SESSION_TRACK_SCHEMA,                       
  SESSION_TRACK_STATE_CHANGE,                 
  SESSION_TRACK_GTIDS,
  SESSION_TRACK_TRANSACTION_CHARACTERISTICS,  
  SESSION_TRACK_TRANSACTION_STATE,            
  SESSION_TRACK_always_at_the_end             
};

#define SESSION_TRACK_BEGIN SESSION_TRACK_SYSTEM_VARIABLES

#define IS_SESSION_STATE_TYPE(T) \
  (((int)(T) >= SESSION_TRACK_BEGIN) && ((T) < SESSION_TRACK_always_at_the_end))

#define net_new_transaction(net) ((net)->pkt_nr=0)

#ifdef __cplusplus
extern "C" {
#endif

my_bool	my_net_init(NET *net, Vio* vio, void *thd, unsigned int my_flags);
void	my_net_local_init(NET *net);
void	net_end(NET *net);
void	net_clear(NET *net, my_bool clear_buffer);
my_bool net_realloc(NET *net, size_t length);
my_bool	net_flush(NET *net);
my_bool	my_net_write(NET *net,const unsigned char *packet, size_t len);
my_bool	net_write_command(NET *net,unsigned char command,
			  const unsigned char *header, size_t head_len,
			  const unsigned char *packet, size_t len);
int	net_real_write(NET *net,const unsigned char *packet, size_t len);
unsigned long my_net_read_packet(NET *net, my_bool read_from_server);
unsigned long my_net_read_packet_reallen(NET *net, my_bool read_from_server,
                                         unsigned long* reallen);
#define my_net_read(A) my_net_read_packet((A), 0)

#ifdef MY_GLOBAL_INCLUDED
void my_net_set_write_timeout(NET *net, uint timeout);
void my_net_set_read_timeout(NET *net, uint timeout);
#endif

struct sockaddr;
int my_connect(my_socket s, const struct sockaddr *name, unsigned int namelen,
	       unsigned int timeout);
struct my_rnd_struct;

#ifdef __cplusplus
}
#endif

  

enum Item_result
{
  STRING_RESULT=0, REAL_RESULT, INT_RESULT, ROW_RESULT, DECIMAL_RESULT,
  TIME_RESULT
};

typedef struct st_udf_args
{
  unsigned int arg_count;		
  enum Item_result *arg_type;		
  char **args;				
  unsigned long *lengths;		
  char *maybe_null;			
  const char **attributes;              
  unsigned long *attribute_lengths;     
  void *extension;
} UDF_ARGS;

  

typedef struct st_udf_init
{
  my_bool maybe_null;          
  unsigned int decimals;       
  unsigned long max_length;    
  char *ptr;                   
  my_bool const_item;          
  void *extension;
} UDF_INIT;


  
#define NET_HEADER_SIZE 4		
#define COMP_HEADER_SIZE 3		

  

#ifdef __cplusplus
extern "C" {
#endif



void create_random_string(char *to, unsigned int length,
                          struct my_rnd_struct *rand_st);

void hash_password(unsigned long *to, const char *password, unsigned int password_len);
void make_scrambled_password_323(char *to, const char *password);
void scramble_323(char *to, const char *message, const char *password);
my_bool check_scramble_323(const unsigned char *reply, const char *message,
                           unsigned long *salt);
void get_salt_from_password_323(unsigned long *res, const char *password);
void make_scrambled_password(char *to, const char *password);
void scramble(char *to, const char *message, const char *password);
my_bool check_scramble(const unsigned char *reply, const char *message,
                       const unsigned char *hash_stage2);
void get_salt_from_password(unsigned char *res, const char *password);
char *octet2hex(char *to, const char *str, size_t len);



char *get_tty_password(const char *opt_message);
void get_tty_password_buff(const char *opt_message, char *to, size_t length);
const char *mysql_errno_to_sqlstate(unsigned int mysql_errno);



my_bool my_thread_init(void);
void my_thread_end(void);

#ifdef MY_GLOBAL_INCLUDED
#include "pack.h"
#endif

#ifdef __cplusplus
}
#endif

#define NULL_LENGTH ~0UL 
#define MYSQL_STMT_HEADER       4U
#define MYSQL_LONG_DATA_HEADER  6U




#endif
