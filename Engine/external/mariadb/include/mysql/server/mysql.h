



#ifndef _mysql_h
#define _mysql_h

#ifdef _AIX           
#include <standards.h>
#endif

#ifdef __CYGWIN__     
#undef WIN
#undef _WIN
#undef _WIN32
#undef _WIN64
#undef __WIN__
#endif

#ifdef	__cplusplus
extern "C" {
#endif

#ifndef MY_GLOBAL_INCLUDED				
#ifndef MYSQL_ABI_CHECK
#include <sys/types.h>
#endif

#ifndef MYSQL_PLUGIN_INCLUDED
typedef char my_bool;
#endif

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__WIN__)
#define __WIN__
#endif
#if !defined(__WIN__)
#define STDCALL
#else
#define STDCALL __stdcall
#endif

#ifndef my_socket_defined
#if defined (_WIN64)
#define my_socket unsigned long long
#elif defined (_WIN32)
#define my_socket unsigned int
#else
typedef int my_socket;
#endif 
#endif 
#endif 

#include "mysql_version.h"
#include "mysql_com.h"
#include "mysql_time.h"

#include "my_list.h" 

extern unsigned int mariadb_deinitialize_ssl;
extern unsigned int mysql_port;
extern char *mysql_unix_port;

#define CLIENT_NET_READ_TIMEOUT		(365*24*3600)	
#define CLIENT_NET_WRITE_TIMEOUT	(365*24*3600)	

#define IS_PRI_KEY(n)	((n) & PRI_KEY_FLAG)
#define IS_NOT_NULL(n)	((n) & NOT_NULL_FLAG)
#define IS_BLOB(n)	((n) & BLOB_FLAG)

#define IS_NUM(t)	(((t) <= MYSQL_TYPE_INT24 && (t) != MYSQL_TYPE_TIMESTAMP) || (t) == MYSQL_TYPE_YEAR || (t) == MYSQL_TYPE_NEWDECIMAL)
#define IS_LONGDATA(t) ((t) >= MYSQL_TYPE_TINY_BLOB && (t) <= MYSQL_TYPE_STRING)


typedef struct st_mysql_field {
  char *name;                 
  char *org_name;             
  char *table;                
  char *org_table;            
  char *db;                   
  char *catalog;	      
  char *def;                  
  unsigned long length;       
  unsigned long max_length;   
  unsigned int name_length;
  unsigned int org_name_length;
  unsigned int table_length;
  unsigned int org_table_length;
  unsigned int db_length;
  unsigned int catalog_length;
  unsigned int def_length;
  unsigned int flags;         
  unsigned int decimals;      
  unsigned int charsetnr;     
  enum enum_field_types type; 
  void *extension;
} MYSQL_FIELD;

typedef char **MYSQL_ROW;		
typedef unsigned int MYSQL_FIELD_OFFSET; 

#ifndef MY_GLOBAL_INCLUDED
#if defined(NO_CLIENT_LONG_LONG)
typedef unsigned long my_ulonglong;
#elif defined (__WIN__)
typedef unsigned __int64 my_ulonglong;
#else
typedef unsigned long long my_ulonglong;
#endif
#endif

#include "typelib.h"

#define MYSQL_COUNT_ERROR (~(my_ulonglong) 0)


#define ER_WARN_DATA_TRUNCATED WARN_DATA_TRUNCATED
#define WARN_PLUGIN_DELETE_BUILTIN ER_PLUGIN_DELETE_BUILTIN
#define ER_FK_DUP_NAME ER_DUP_CONSTRAINT_NAME
#define ER_VIRTUAL_COLUMN_FUNCTION_IS_NOT_ALLOWED ER_GENERATED_COLUMN_FUNCTION_IS_NOT_ALLOWED
#define ER_PRIMARY_KEY_BASED_ON_VIRTUAL_COLUMN ER_PRIMARY_KEY_BASED_ON_GENERATED_COLUMN
#define ER_WRONG_FK_OPTION_FOR_VIRTUAL_COLUMN ER_WRONG_FK_OPTION_FOR_GENERATED_COLUMN
#define ER_UNSUPPORTED_ACTION_ON_VIRTUAL_COLUMN ER_UNSUPPORTED_ACTION_ON_GENERATED_COLUMN
#define ER_UNSUPPORTED_ENGINE_FOR_VIRTUAL_COLUMNS ER_UNSUPPORTED_ENGINE_FOR_GENERATED_COLUMNS

typedef struct st_mysql_rows {
  struct st_mysql_rows *next;		
  MYSQL_ROW data;
  unsigned long length;
} MYSQL_ROWS;

typedef MYSQL_ROWS *MYSQL_ROW_OFFSET;	

#include "my_alloc.h"

typedef struct embedded_query_result EMBEDDED_QUERY_RESULT;
typedef struct st_mysql_data {
  MYSQL_ROWS *data;
  struct embedded_query_result *embedded_info;
  MEM_ROOT alloc;
  my_ulonglong rows;
  unsigned int fields;
  
  void *extension;
} MYSQL_DATA;

enum mysql_option 
{
  MYSQL_OPT_CONNECT_TIMEOUT, MYSQL_OPT_COMPRESS, MYSQL_OPT_NAMED_PIPE,
  MYSQL_INIT_COMMAND, MYSQL_READ_DEFAULT_FILE, MYSQL_READ_DEFAULT_GROUP,
  MYSQL_SET_CHARSET_DIR, MYSQL_SET_CHARSET_NAME, MYSQL_OPT_LOCAL_INFILE,
  MYSQL_OPT_PROTOCOL, MYSQL_SHARED_MEMORY_BASE_NAME, MYSQL_OPT_READ_TIMEOUT,
  MYSQL_OPT_WRITE_TIMEOUT, MYSQL_OPT_USE_RESULT,
  MYSQL_OPT_USE_REMOTE_CONNECTION, MYSQL_OPT_USE_EMBEDDED_CONNECTION,
  MYSQL_OPT_GUESS_CONNECTION, MYSQL_SET_CLIENT_IP, MYSQL_SECURE_AUTH,
  MYSQL_REPORT_DATA_TRUNCATION, MYSQL_OPT_RECONNECT,
  MYSQL_OPT_SSL_VERIFY_SERVER_CERT, MYSQL_PLUGIN_DIR, MYSQL_DEFAULT_AUTH,
  MYSQL_OPT_BIND,
  MYSQL_OPT_SSL_KEY, MYSQL_OPT_SSL_CERT, 
  MYSQL_OPT_SSL_CA, MYSQL_OPT_SSL_CAPATH, MYSQL_OPT_SSL_CIPHER,
  MYSQL_OPT_SSL_CRL, MYSQL_OPT_SSL_CRLPATH,
  MYSQL_OPT_CONNECT_ATTR_RESET, MYSQL_OPT_CONNECT_ATTR_ADD,
  MYSQL_OPT_CONNECT_ATTR_DELETE,
  MYSQL_SERVER_PUBLIC_KEY,
  MYSQL_ENABLE_CLEARTEXT_PLUGIN,
  MYSQL_OPT_CAN_HANDLE_EXPIRED_PASSWORDS,

  
  MYSQL_PROGRESS_CALLBACK=5999,
  MYSQL_OPT_NONBLOCK,
  MYSQL_OPT_USE_THREAD_SPECIFIC_MEMORY
};


struct st_mysql_options_extention; 

struct st_mysql_options {
  unsigned int connect_timeout, read_timeout, write_timeout;
  unsigned int port, protocol;
  unsigned long client_flag;
  char *host,*user,*password,*unix_socket,*db;
  struct st_dynamic_array *init_commands;
  char *my_cnf_file,*my_cnf_group, *charset_dir, *charset_name;
  char *ssl_key;				
  char *ssl_cert;				
  char *ssl_ca;					
  char *ssl_capath;				
  char *ssl_cipher;				
  char *shared_memory_base_name;
  unsigned long max_allowed_packet;
  my_bool use_ssl;				
  my_bool compress,named_pipe;
  my_bool use_thread_specific_memory;
  my_bool unused2;
  my_bool unused3;
  my_bool unused4;
  enum mysql_option methods_to_use;
  char *client_ip;
  
  my_bool secure_auth;
  
  my_bool report_data_truncation;

  
  int (*local_infile_init)(void **, const char *, void *);
  int (*local_infile_read)(void *, char *, unsigned int);
  void (*local_infile_end)(void *);
  int (*local_infile_error)(void *, char *, unsigned int);
  void *local_infile_userdata;
  struct st_mysql_options_extention *extension;
};

enum mysql_status 
{
  MYSQL_STATUS_READY, MYSQL_STATUS_GET_RESULT, MYSQL_STATUS_USE_RESULT,
  MYSQL_STATUS_STATEMENT_GET_RESULT
};

enum mysql_protocol_type 
{
  MYSQL_PROTOCOL_DEFAULT, MYSQL_PROTOCOL_TCP, MYSQL_PROTOCOL_SOCKET,
  MYSQL_PROTOCOL_PIPE, MYSQL_PROTOCOL_MEMORY
};

typedef struct character_set
{
  unsigned int      number;     
  unsigned int      state;      
  const char        *csname;    
  const char        *name;      
  const char        *comment;   
  const char        *dir;       
  unsigned int      mbminlen;   
  unsigned int      mbmaxlen;   
} MY_CHARSET_INFO;

struct st_mysql_methods;
struct st_mysql_stmt;

typedef struct st_mysql
{
  NET		net;			
  unsigned char	*connector_fd;		
  char		*host,*user,*passwd,*unix_socket,*server_version,*host_info;
  char          *info, *db;
  const struct charset_info_st *charset;
  MYSQL_FIELD	*fields;
  MEM_ROOT	field_alloc;
  my_ulonglong affected_rows;
  my_ulonglong insert_id;		
  my_ulonglong extra_info;		
  unsigned long thread_id;		
  unsigned long packet_length;
  unsigned int	port;
  unsigned long client_flag,server_capabilities;
  unsigned int	protocol_version;
  unsigned int	field_count;
  unsigned int 	server_status;
  unsigned int  server_language;
  unsigned int	warning_count;
  struct st_mysql_options options;
  enum mysql_status status;
  my_bool	free_me;		
  my_bool	reconnect;		

  
  char	        scramble[SCRAMBLE_LENGTH+1];
  my_bool       auto_local_infile;
  void *unused2, *unused3, *unused4, *unused5;

  LIST  *stmts;                     
  const struct st_mysql_methods *methods;
  void *thd;
  
  my_bool *unbuffered_fetch_owner;
  
  char *info_buffer;
  void *extension;
} MYSQL;


typedef struct st_mysql_res {
  my_ulonglong  row_count;
  MYSQL_FIELD	*fields;
  MYSQL_DATA	*data;
  MYSQL_ROWS	*data_cursor;
  unsigned long *lengths;		
  MYSQL		*handle;		
  const struct st_mysql_methods *methods;
  MYSQL_ROW	row;			
  MYSQL_ROW	current_row;		
  MEM_ROOT	field_alloc;
  unsigned int	field_count, current_field;
  my_bool	eof;			
  
  my_bool       unbuffered_fetch_cancelled;  
  void *extension;
} MYSQL_RES;


#if !defined(MYSQL_SERVER) && !defined(MYSQL_CLIENT)
#define MYSQL_CLIENT
#endif


typedef struct st_mysql_parameters
{
  unsigned long *p_max_allowed_packet;
  unsigned long *p_net_buffer_length;
  void *extension;
} MYSQL_PARAMETERS;




#define MYSQL_WAIT_READ 1

#define MYSQL_WAIT_WRITE  2

#define MYSQL_WAIT_EXCEPT 4

#define MYSQL_WAIT_TIMEOUT 8

#if !defined(MYSQL_SERVER) && !defined(EMBEDDED_LIBRARY)
#define max_allowed_packet (*mysql_get_parameters()->p_max_allowed_packet)
#define net_buffer_length (*mysql_get_parameters()->p_net_buffer_length)
#endif


int STDCALL mysql_server_init(int argc, char **argv, char **groups);
void STDCALL mysql_server_end(void);


#define mysql_library_init mysql_server_init
#define mysql_library_end mysql_server_end

MYSQL_PARAMETERS *STDCALL mysql_get_parameters(void);


my_bool STDCALL mysql_thread_init(void);
void STDCALL mysql_thread_end(void);



my_ulonglong STDCALL mysql_num_rows(MYSQL_RES *res);
unsigned int STDCALL mysql_num_fields(MYSQL_RES *res);
my_bool STDCALL mysql_eof(MYSQL_RES *res);
MYSQL_FIELD *STDCALL mysql_fetch_field_direct(MYSQL_RES *res,
					      unsigned int fieldnr);
MYSQL_FIELD * STDCALL mysql_fetch_fields(MYSQL_RES *res);
MYSQL_ROW_OFFSET STDCALL mysql_row_tell(MYSQL_RES *res);
MYSQL_FIELD_OFFSET STDCALL mysql_field_tell(MYSQL_RES *res);

unsigned int STDCALL mysql_field_count(MYSQL *mysql);
my_ulonglong STDCALL mysql_affected_rows(MYSQL *mysql);
my_ulonglong STDCALL mysql_insert_id(MYSQL *mysql);
unsigned int STDCALL mysql_errno(MYSQL *mysql);
const char * STDCALL mysql_error(MYSQL *mysql);
const char *STDCALL mysql_sqlstate(MYSQL *mysql);
unsigned int STDCALL mysql_warning_count(MYSQL *mysql);
const char * STDCALL mysql_info(MYSQL *mysql);
unsigned long STDCALL mysql_thread_id(MYSQL *mysql);
const char * STDCALL mysql_character_set_name(MYSQL *mysql);
int          STDCALL mysql_set_character_set(MYSQL *mysql, const char *csname);
int          STDCALL mysql_set_character_set_start(int *ret, MYSQL *mysql,
                                                   const char *csname);
int          STDCALL mysql_set_character_set_cont(int *ret, MYSQL *mysql,
                                                  int status);

MYSQL *		STDCALL mysql_init(MYSQL *mysql);
my_bool		STDCALL mysql_ssl_set(MYSQL *mysql, const char *key,
				      const char *cert, const char *ca,
				      const char *capath, const char *cipher);
const char *    STDCALL mysql_get_ssl_cipher(MYSQL *mysql);
my_bool		STDCALL mysql_change_user(MYSQL *mysql, const char *user, 
					  const char *passwd, const char *db);
int             STDCALL mysql_change_user_start(my_bool *ret, MYSQL *mysql,
                                                const char *user,
                                                const char *passwd,
                                                const char *db);
int             STDCALL mysql_change_user_cont(my_bool *ret, MYSQL *mysql,
                                               int status);
MYSQL *		STDCALL mysql_real_connect(MYSQL *mysql, const char *host,
					   const char *user,
					   const char *passwd,
					   const char *db,
					   unsigned int port,
					   const char *unix_socket,
					   unsigned long clientflag);
int             STDCALL mysql_real_connect_start(MYSQL **ret, MYSQL *mysql,
                                                 const char *host,
                                                 const char *user,
                                                 const char *passwd,
                                                 const char *db,
                                                 unsigned int port,
                                                 const char *unix_socket,
                                                 unsigned long clientflag);
int             STDCALL mysql_real_connect_cont(MYSQL **ret, MYSQL *mysql,
                                                int status);
int		STDCALL mysql_select_db(MYSQL *mysql, const char *db);
int             STDCALL mysql_select_db_start(int *ret, MYSQL *mysql,
                                              const char *db);
int             STDCALL mysql_select_db_cont(int *ret, MYSQL *mysql,
                                             int status);
int		STDCALL mysql_query(MYSQL *mysql, const char *q);
int             STDCALL mysql_query_start(int *ret, MYSQL *mysql,
                                          const char *q);
int             STDCALL mysql_query_cont(int *ret, MYSQL *mysql,
                                         int status);
int		STDCALL mysql_send_query(MYSQL *mysql, const char *q,
					 unsigned long length);
int             STDCALL mysql_send_query_start(int *ret, MYSQL *mysql,
                                               const char *q,
                                               unsigned long length);
int             STDCALL mysql_send_query_cont(int *ret, MYSQL *mysql,
                                              int status);
int		STDCALL mysql_real_query(MYSQL *mysql, const char *q,
					unsigned long length);
int             STDCALL mysql_real_query_start(int *ret, MYSQL *mysql,
                                               const char *q,
                                               unsigned long length);
int             STDCALL mysql_real_query_cont(int *ret, MYSQL *mysql,
                                              int status);
MYSQL_RES *     STDCALL mysql_store_result(MYSQL *mysql);
int             STDCALL mysql_store_result_start(MYSQL_RES **ret, MYSQL *mysql);
int             STDCALL mysql_store_result_cont(MYSQL_RES **ret, MYSQL *mysql,
                                                int status);
MYSQL_RES *     STDCALL mysql_use_result(MYSQL *mysql);

void        STDCALL mysql_get_character_set_info(MYSQL *mysql,
                           MY_CHARSET_INFO *charset);



#define LOCAL_INFILE_ERROR_LEN 512

void
mysql_set_local_infile_handler(MYSQL *mysql,
                               int (*local_infile_init)(void **, const char *,
                            void *),
                               int (*local_infile_read)(void *, char *,
							unsigned int),
                               void (*local_infile_end)(void *),
                               int (*local_infile_error)(void *, char*,
							 unsigned int),
                               void *);

void
mysql_set_local_infile_default(MYSQL *mysql);

int		STDCALL mysql_shutdown(MYSQL *mysql,
                                       enum mysql_enum_shutdown_level
                                       shutdown_level);
int             STDCALL mysql_shutdown_start(int *ret, MYSQL *mysql,
                                             enum mysql_enum_shutdown_level
                                             shutdown_level);
int             STDCALL mysql_shutdown_cont(int *ret, MYSQL *mysql,
                                            int status);
int		STDCALL mysql_dump_debug_info(MYSQL *mysql);
int             STDCALL mysql_dump_debug_info_start(int *ret, MYSQL *mysql);
int             STDCALL mysql_dump_debug_info_cont(int *ret, MYSQL *mysql,
                                                   int status);
int		STDCALL mysql_refresh(MYSQL *mysql,
				     unsigned int refresh_options);
int             STDCALL mysql_refresh_start(int *ret, MYSQL *mysql,
                                            unsigned int refresh_options);
int             STDCALL mysql_refresh_cont(int *ret, MYSQL *mysql, int status);
int		STDCALL mysql_kill(MYSQL *mysql,unsigned long pid);
int             STDCALL mysql_kill_start(int *ret, MYSQL *mysql,
                                         unsigned long pid);
int             STDCALL mysql_kill_cont(int *ret, MYSQL *mysql, int status);
int		STDCALL mysql_set_server_option(MYSQL *mysql,
						enum enum_mysql_set_option
						option);
int             STDCALL mysql_set_server_option_start(int *ret, MYSQL *mysql,
                                                      enum enum_mysql_set_option
                                                      option);
int             STDCALL mysql_set_server_option_cont(int *ret, MYSQL *mysql,
                                                     int status);
int		STDCALL mysql_ping(MYSQL *mysql);
int             STDCALL mysql_ping_start(int *ret, MYSQL *mysql);
int             STDCALL mysql_ping_cont(int *ret, MYSQL *mysql, int status);
const char *	STDCALL mysql_stat(MYSQL *mysql);
int             STDCALL mysql_stat_start(const char **ret, MYSQL *mysql);
int             STDCALL mysql_stat_cont(const char **ret, MYSQL *mysql,
                                        int status);
const char *	STDCALL mysql_get_server_info(MYSQL *mysql);
const char *	STDCALL mysql_get_server_name(MYSQL *mysql);
const char *	STDCALL mysql_get_client_info(void);
unsigned long	STDCALL mysql_get_client_version(void);
const char *	STDCALL mysql_get_host_info(MYSQL *mysql);
unsigned long	STDCALL mysql_get_server_version(MYSQL *mysql);
unsigned int	STDCALL mysql_get_proto_info(MYSQL *mysql);
MYSQL_RES *	STDCALL mysql_list_dbs(MYSQL *mysql,const char *wild);
int             STDCALL mysql_list_dbs_start(MYSQL_RES **ret, MYSQL *mysql,
                                             const char *wild);
int             STDCALL mysql_list_dbs_cont(MYSQL_RES **ret, MYSQL *mysql,
                                            int status);
MYSQL_RES *	STDCALL mysql_list_tables(MYSQL *mysql,const char *wild);
int             STDCALL mysql_list_tables_start(MYSQL_RES **ret, MYSQL *mysql,
                                                const char *wild);
int             STDCALL mysql_list_tables_cont(MYSQL_RES **ret, MYSQL *mysql,
                                               int status);
MYSQL_RES *	STDCALL mysql_list_processes(MYSQL *mysql);
int             STDCALL mysql_list_processes_start(MYSQL_RES **ret,
                                                   MYSQL *mysql);
int             STDCALL mysql_list_processes_cont(MYSQL_RES **ret, MYSQL *mysql,
                                                  int status);
int		STDCALL mysql_options(MYSQL *mysql,enum mysql_option option,
				      const void *arg);
int             STDCALL mysql_options4(MYSQL *mysql,enum mysql_option option,
                                       const void *arg1, const void *arg2);
void		STDCALL mysql_free_result(MYSQL_RES *result);
int             STDCALL mysql_free_result_start(MYSQL_RES *result);
int             STDCALL mysql_free_result_cont(MYSQL_RES *result, int status);
void		STDCALL mysql_data_seek(MYSQL_RES *result,
					my_ulonglong offset);
MYSQL_ROW_OFFSET STDCALL mysql_row_seek(MYSQL_RES *result,
						MYSQL_ROW_OFFSET offset);
MYSQL_FIELD_OFFSET STDCALL mysql_field_seek(MYSQL_RES *result,
					   MYSQL_FIELD_OFFSET offset);
MYSQL_ROW	STDCALL mysql_fetch_row(MYSQL_RES *result);
int             STDCALL mysql_fetch_row_start(MYSQL_ROW *ret,
                                              MYSQL_RES *result);
int             STDCALL mysql_fetch_row_cont(MYSQL_ROW *ret, MYSQL_RES *result,
                                             int status);
unsigned long * STDCALL mysql_fetch_lengths(MYSQL_RES *result);
MYSQL_FIELD *	STDCALL mysql_fetch_field(MYSQL_RES *result);
MYSQL_RES *     STDCALL mysql_list_fields(MYSQL *mysql, const char *table,
					  const char *wild);
int             STDCALL mysql_list_fields_start(MYSQL_RES **ret, MYSQL *mysql,
                                                const char *table,
                                                const char *wild);
int             STDCALL mysql_list_fields_cont(MYSQL_RES **ret, MYSQL *mysql,
                                               int status);
unsigned long	STDCALL mysql_escape_string(char *to,const char *from,
					    unsigned long from_length);
unsigned long	STDCALL mysql_hex_string(char *to,const char *from,
                                         unsigned long from_length);
unsigned long STDCALL mysql_real_escape_string(MYSQL *mysql,
					       char *to,const char *from,
					       unsigned long length);
void		STDCALL mysql_debug(const char *debug);
void 		STDCALL myodbc_remove_escape(MYSQL *mysql,char *name);
unsigned int	STDCALL mysql_thread_safe(void);
my_bool		STDCALL mysql_embedded(void);
my_bool		STDCALL mariadb_connection(MYSQL *mysql);
my_bool         STDCALL mysql_read_query_result(MYSQL *mysql);
int             STDCALL mysql_read_query_result_start(my_bool *ret,
                                                      MYSQL *mysql);
int             STDCALL mysql_read_query_result_cont(my_bool *ret,
                                                     MYSQL *mysql, int status);





enum enum_mysql_stmt_state
{
  MYSQL_STMT_INIT_DONE= 1, MYSQL_STMT_PREPARE_DONE, MYSQL_STMT_EXECUTE_DONE,
  MYSQL_STMT_FETCH_DONE
};




typedef struct st_mysql_bind
{
  unsigned long	*length;          
  my_bool       *is_null;	  
  void		*buffer;	  
  
  my_bool       *error;
  unsigned char *row_ptr;         
  void (*store_param_func)(NET *net, struct st_mysql_bind *param);
  void (*fetch_result)(struct st_mysql_bind *, MYSQL_FIELD *,
                       unsigned char **row);
  void (*skip_result)(struct st_mysql_bind *, MYSQL_FIELD *,
		      unsigned char **row);
  
  unsigned long buffer_length;
  unsigned long offset;           
  unsigned long	length_value;     
  unsigned int	param_number;	  
  unsigned int  pack_length;	  
  enum enum_field_types buffer_type;	
  my_bool       error_value;      
  my_bool       is_unsigned;      
  my_bool	long_data_used;	  
  my_bool	is_null_value;    
  void *extension;
} MYSQL_BIND;


struct st_mysql_stmt_extension;


typedef struct st_mysql_stmt
{
  MEM_ROOT       mem_root;             
  LIST           list;                 
  MYSQL          *mysql;               
  MYSQL_BIND     *params;              
  MYSQL_BIND     *bind;                
  MYSQL_FIELD    *fields;              
  MYSQL_DATA     result;               
  MYSQL_ROWS     *data_cursor;         
  
  int            (*read_row_func)(struct st_mysql_stmt *stmt, 
                                  unsigned char **row);
  
  my_ulonglong   affected_rows;
  my_ulonglong   insert_id;            
  unsigned long	 stmt_id;	       
  unsigned long  flags;                
  unsigned long  prefetch_rows;        
  
  unsigned int   server_status;
  unsigned int	 last_errno;	       
  unsigned int   param_count;          
  unsigned int   field_count;          
  enum enum_mysql_stmt_state state;    
  char		 last_error[MYSQL_ERRMSG_SIZE]; 
  char		 sqlstate[SQLSTATE_LENGTH+1];
  
  my_bool        send_types_to_server;
  my_bool        bind_param_done;      
  unsigned char  bind_result_done;     
  
  my_bool       unbuffered_fetch_cancelled;  
  
  my_bool       update_max_length;     
  struct st_mysql_stmt_extension *extension;
} MYSQL_STMT;

enum enum_stmt_attr_type
{
  
  STMT_ATTR_UPDATE_MAX_LENGTH,
  
  STMT_ATTR_CURSOR_TYPE,
  
  STMT_ATTR_PREFETCH_ROWS
};

MYSQL_STMT * STDCALL mysql_stmt_init(MYSQL *mysql);
int STDCALL mysql_stmt_prepare(MYSQL_STMT *stmt, const char *query,
                               unsigned long length);
int STDCALL mysql_stmt_prepare_start(int *ret, MYSQL_STMT *stmt,
                                     const char *query, unsigned long length);
int STDCALL mysql_stmt_prepare_cont(int *ret, MYSQL_STMT *stmt, int status);
int STDCALL mysql_stmt_execute(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_execute_start(int *ret, MYSQL_STMT *stmt);
int STDCALL mysql_stmt_execute_cont(int *ret, MYSQL_STMT *stmt, int status);
int STDCALL mysql_stmt_fetch(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_fetch_start(int *ret, MYSQL_STMT *stmt);
int STDCALL mysql_stmt_fetch_cont(int *ret, MYSQL_STMT *stmt, int status);
int STDCALL mysql_stmt_fetch_column(MYSQL_STMT *stmt, MYSQL_BIND *bind_arg, 
                                    unsigned int column,
                                    unsigned long offset);
int STDCALL mysql_stmt_store_result(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_store_result_start(int *ret, MYSQL_STMT *stmt);
int STDCALL mysql_stmt_store_result_cont(int *ret, MYSQL_STMT *stmt,
                                         int status);
unsigned long STDCALL mysql_stmt_param_count(MYSQL_STMT * stmt);
my_bool STDCALL mysql_stmt_attr_set(MYSQL_STMT *stmt,
                                    enum enum_stmt_attr_type attr_type,
                                    const void *attr);
my_bool STDCALL mysql_stmt_attr_get(MYSQL_STMT *stmt,
                                    enum enum_stmt_attr_type attr_type,
                                    void *attr);
my_bool STDCALL mysql_stmt_bind_param(MYSQL_STMT * stmt, MYSQL_BIND * bnd);
my_bool STDCALL mysql_stmt_bind_result(MYSQL_STMT * stmt, MYSQL_BIND * bnd);
my_bool STDCALL mysql_stmt_close(MYSQL_STMT * stmt);
int STDCALL mysql_stmt_close_start(my_bool *ret, MYSQL_STMT *stmt);
int STDCALL mysql_stmt_close_cont(my_bool *ret, MYSQL_STMT * stmt, int status);
my_bool STDCALL mysql_stmt_reset(MYSQL_STMT * stmt);
int STDCALL mysql_stmt_reset_start(my_bool *ret, MYSQL_STMT * stmt);
int STDCALL mysql_stmt_reset_cont(my_bool *ret, MYSQL_STMT *stmt, int status);
my_bool STDCALL mysql_stmt_free_result(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_free_result_start(my_bool *ret, MYSQL_STMT *stmt);
int STDCALL mysql_stmt_free_result_cont(my_bool *ret, MYSQL_STMT *stmt,
                                        int status);
my_bool STDCALL mysql_stmt_send_long_data(MYSQL_STMT *stmt, 
                                          unsigned int param_number,
                                          const char *data, 
                                          unsigned long length);
int STDCALL mysql_stmt_send_long_data_start(my_bool *ret, MYSQL_STMT *stmt,
                                            unsigned int param_number,
                                            const char *data,
                                            unsigned long len);
int STDCALL mysql_stmt_send_long_data_cont(my_bool *ret, MYSQL_STMT *stmt,
                                           int status);
MYSQL_RES *STDCALL mysql_stmt_result_metadata(MYSQL_STMT *stmt);
MYSQL_RES *STDCALL mysql_stmt_param_metadata(MYSQL_STMT *stmt);
unsigned int STDCALL mysql_stmt_errno(MYSQL_STMT * stmt);
const char *STDCALL mysql_stmt_error(MYSQL_STMT * stmt);
const char *STDCALL mysql_stmt_sqlstate(MYSQL_STMT * stmt);
MYSQL_ROW_OFFSET STDCALL mysql_stmt_row_seek(MYSQL_STMT *stmt, 
                                             MYSQL_ROW_OFFSET offset);
MYSQL_ROW_OFFSET STDCALL mysql_stmt_row_tell(MYSQL_STMT *stmt);
void STDCALL mysql_stmt_data_seek(MYSQL_STMT *stmt, my_ulonglong offset);
my_ulonglong STDCALL mysql_stmt_num_rows(MYSQL_STMT *stmt);
my_ulonglong STDCALL mysql_stmt_affected_rows(MYSQL_STMT *stmt);
my_ulonglong STDCALL mysql_stmt_insert_id(MYSQL_STMT *stmt);
unsigned int STDCALL mysql_stmt_field_count(MYSQL_STMT *stmt);

my_bool STDCALL mysql_commit(MYSQL * mysql);
int STDCALL mysql_commit_start(my_bool *ret, MYSQL * mysql);
int STDCALL mysql_commit_cont(my_bool *ret, MYSQL * mysql, int status);
my_bool STDCALL mysql_rollback(MYSQL * mysql);
int STDCALL mysql_rollback_start(my_bool *ret, MYSQL * mysql);
int STDCALL mysql_rollback_cont(my_bool *ret, MYSQL * mysql, int status);
my_bool STDCALL mysql_autocommit(MYSQL * mysql, my_bool auto_mode);
int STDCALL mysql_autocommit_start(my_bool *ret, MYSQL * mysql,
                                   my_bool auto_mode);
int STDCALL mysql_autocommit_cont(my_bool *ret, MYSQL * mysql, int status);
my_bool STDCALL mysql_more_results(MYSQL *mysql);
int STDCALL mysql_next_result(MYSQL *mysql);
int STDCALL mysql_next_result_start(int *ret, MYSQL *mysql);
int STDCALL mysql_next_result_cont(int *ret, MYSQL *mysql, int status);
int STDCALL mysql_stmt_next_result(MYSQL_STMT *stmt);
int STDCALL mysql_stmt_next_result_start(int *ret, MYSQL_STMT *stmt);
int STDCALL mysql_stmt_next_result_cont(int *ret, MYSQL_STMT *stmt, int status);
void STDCALL mysql_close_slow_part(MYSQL *mysql);
void STDCALL mysql_close(MYSQL *sock);
int STDCALL mysql_close_start(MYSQL *sock);
int STDCALL mysql_close_cont(MYSQL *sock, int status);
my_socket STDCALL mysql_get_socket(const MYSQL *mysql);
unsigned int STDCALL mysql_get_timeout_value(const MYSQL *mysql);
unsigned int STDCALL mysql_get_timeout_value_ms(const MYSQL *mysql);


unsigned long STDCALL mysql_net_read_packet(MYSQL *mysql);
unsigned long STDCALL mysql_net_field_length(unsigned char **packet);


#define MYSQL_NO_DATA        100
#define MYSQL_DATA_TRUNCATED 101

#define mysql_reload(mysql) mysql_refresh((mysql),REFRESH_GRANT)

#ifdef USE_OLD_FUNCTIONS
MYSQL *		STDCALL mysql_connect(MYSQL *mysql, const char *host,
				      const char *user, const char *passwd);
int		STDCALL mysql_create_db(MYSQL *mysql, const char *DB);
int		STDCALL mysql_drop_db(MYSQL *mysql, const char *DB);
#endif
#define HAVE_MYSQL_REAL_CONNECT

#ifdef	__cplusplus
}
#endif

#endif 
