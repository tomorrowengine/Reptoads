


#ifndef MYSQL_CLIENT_PLUGIN_INCLUDED
#define MYSQL_CLIENT_PLUGIN_INCLUDED

#ifndef MYSQL_ABI_CHECK
#include <stdarg.h>
#include <stdlib.h>
#endif


#ifndef PLUGINDIR
#define PLUGINDIR "lib/plugin"
#endif

#define plugin_declarations_sym "_mysql_client_plugin_declaration_"


#define MYSQL_CLIENT_PLUGIN_RESERVED         0 
#define MYSQL_CLIENT_PLUGIN_RESERVED2        1
#define MYSQL_CLIENT_AUTHENTICATION_PLUGIN   2 

#define MYSQL_CLIENT_AUTHENTICATION_PLUGIN_INTERFACE_VERSION  0x0100
#define MYSQL_CLIENT_MAX_PLUGINS             3


#define MARIADB_CLIENT_REMOTEIO_PLUGIN       100 
#define MARIADB_CLIENT_PVIO_PLUGIN           101
#define MARIADB_CLIENT_TRACE_PLUGIN          102
#define MARIADB_CLIENT_CONNECTION_PLUGIN     103

#define MARIADB_CLIENT_REMOTEIO_PLUGIN_INTERFACE_VERSION 0x0100
#define MARIADB_CLIENT_PVIO_PLUGIN_INTERFACE_VERSION 0x0100
#define MARIADB_CLIENT_TRACE_PLUGIN_INTERFACE_VERSION 0x0100
#define MARIADB_CLIENT_CONNECTION_PLUGIN_INTERFACE_VERSION 0x0100

#define MARIADB_CLIENT_MAX_PLUGINS             4

#define mysql_declare_client_plugin(X)          \
     struct st_mysql_client_plugin_ ## X        \
        _mysql_client_plugin_declaration_ = {   \
          MYSQL_CLIENT_ ## X ## _PLUGIN,        \
          MYSQL_CLIENT_ ## X ## _PLUGIN_INTERFACE_VERSION,
#define mysql_end_client_plugin             }


#ifndef MYSQL_CLIENT_PLUGIN_HEADER
#define MYSQL_CLIENT_PLUGIN_HEADER                      \
  int type;                                             \
  unsigned int interface_version;                       \
  const char *name;                                     \
  const char *author;                                   \
  const char *desc;                                     \
  unsigned int version[3];                              \
  const char *license;                                  \
  void *mysql_api;                                      \
  int (*init)(char *, size_t, int, va_list);            \
  int (*deinit)();                                      \
  int (*options)(const char *option, const void *);
struct st_mysql_client_plugin
{
  MYSQL_CLIENT_PLUGIN_HEADER
};
#endif

struct st_mysql;



typedef struct st_ma_connection_plugin
{
  MYSQL_CLIENT_PLUGIN_HEADER
  
  MYSQL *(*connect)(MYSQL *mysql, const char *host,
                    const char *user, const char *passwd,
		                const char *db, unsigned int port,
                    const char *unix_socket, unsigned long clientflag);
  void (*close)(MYSQL *mysql);
  int (*set_optionsv)(MYSQL *mysql, unsigned int option, ...);
  int (*set_connection)(MYSQL *mysql,enum enum_server_command command,
                        const char *arg,
                        size_t length, my_bool skipp_check, void *opt_arg);
  my_bool (*reconnect)(MYSQL *mysql);
  int (*reset)(MYSQL *mysql);
} MARIADB_CONNECTION_PLUGIN;

#define MARIADB_DB_DRIVER(a) ((a)->ext_db)


#include <ma_pvio.h>

typedef struct st_mariadb_client_plugin_PVIO
{
  MYSQL_CLIENT_PLUGIN_HEADER
  struct st_ma_pvio_methods *methods;
} MARIADB_PVIO_PLUGIN;


#include <mysql/plugin_auth_common.h>

struct st_mysql_client_plugin_AUTHENTICATION
{
  MYSQL_CLIENT_PLUGIN_HEADER
  int (*authenticate_user)(MYSQL_PLUGIN_VIO *vio, struct st_mysql *mysql);
};


struct st_mysql_client_plugin_TRACE
{
  MYSQL_CLIENT_PLUGIN_HEADER
};


typedef char *(*mysql_authentication_dialog_ask_t)(struct st_mysql *mysql,
                      int type, const char *prompt, char *buf, int buf_len);


#ifdef HAVE_REMOTEIO
#include <mariadb/ma_io.h>


typedef struct st_mysql_client_plugin_REMOTEIO
{
  MYSQL_CLIENT_PLUGIN_HEADER
  struct st_rio_methods *methods;
} MARIADB_REMOTEIO_PLUGIN;
#endif




struct st_mysql_client_plugin *
mysql_load_plugin(struct st_mysql *mysql, const char *name, int type,
                  int argc, ...);


struct st_mysql_client_plugin * STDCALL
mysql_load_plugin_v(struct st_mysql *mysql, const char *name, int type,
                    int argc, va_list args);


struct st_mysql_client_plugin * STDCALL
mysql_client_find_plugin(struct st_mysql *mysql, const char *name, int type);


struct st_mysql_client_plugin * STDCALL
mysql_client_register_plugin(struct st_mysql *mysql,
                             struct st_mysql_client_plugin *plugin);

extern struct st_mysql_client_plugin *mysql_client_builtins[];

#endif


