#ifndef MYSQL_CLIENT_PLUGIN_INCLUDED



#define MYSQL_CLIENT_PLUGIN_INCLUDED


#undef MYSQL_PLUGIN_EXPORT
#if defined(_MSC_VER)
  #define MYSQL_PLUGIN_EXPORT_C __declspec(dllexport)
#else 
  #define MYSQL_PLUGIN_EXPORT_C
#endif
#ifdef __cplusplus
#define MYSQL_PLUGIN_EXPORT extern "C" MYSQL_PLUGIN_EXPORT_C
#define C_MODE_START    extern "C" {
#define C_MODE_END }
#else
#define MYSQL_PLUGIN_EXPORT MYSQL_PLUGIN_EXPORT_C
#define C_MODE_START
#define C_MODE_END
#endif

#ifndef MYSQL_ABI_CHECK
#include <stdarg.h>
#include <stdlib.h>
#endif


#define MYSQL_CLIENT_reserved1               0
#define MYSQL_CLIENT_reserved2               1
#define MYSQL_CLIENT_AUTHENTICATION_PLUGIN   2

#define MYSQL_CLIENT_AUTHENTICATION_PLUGIN_INTERFACE_VERSION  0x0100

#define MYSQL_CLIENT_MAX_PLUGINS             3

#define mysql_declare_client_plugin(X)          \
     C_MODE_START MYSQL_PLUGIN_EXPORT_C         \
        struct st_mysql_client_plugin_ ## X        \
        _mysql_client_plugin_declaration_ = {   \
          MYSQL_CLIENT_ ## X ## _PLUGIN,        \
          MYSQL_CLIENT_ ## X ## _PLUGIN_INTERFACE_VERSION,
#define mysql_end_client_plugin             }; C_MODE_END


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

struct st_mysql;


#include <mysql/plugin_auth_common.h>

struct st_mysql_client_plugin_AUTHENTICATION
{
  MYSQL_CLIENT_PLUGIN_HEADER
  int (*authenticate_user)(MYSQL_PLUGIN_VIO *vio, struct st_mysql *mysql);
};

#include <mysql/auth_dialog_client.h>




struct st_mysql_client_plugin *
mysql_load_plugin(struct st_mysql *mysql, const char *name, int type,
                  int argc, ...);


struct st_mysql_client_plugin *
mysql_load_plugin_v(struct st_mysql *mysql, const char *name, int type,
                    int argc, va_list args);


struct st_mysql_client_plugin *
mysql_client_find_plugin(struct st_mysql *mysql, const char *name, int type);


struct st_mysql_client_plugin *
mysql_client_register_plugin(struct st_mysql *mysql,
                             struct st_mysql_client_plugin *plugin);


int mysql_plugin_options(struct st_mysql_client_plugin *plugin,
                         const char *option, const void *value);
#endif

