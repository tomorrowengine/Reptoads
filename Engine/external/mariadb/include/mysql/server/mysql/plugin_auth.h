#ifndef MYSQL_PLUGIN_AUTH_INCLUDED




#define MYSQL_PLUGIN_AUTH_INCLUDED

#include <mysql/plugin.h>

#define MYSQL_AUTHENTICATION_INTERFACE_VERSION 0x0201

#include <mysql/plugin_auth_common.h>

#ifdef __cplusplus
extern "C" {
#endif



#define PASSWORD_USED_NO         0
#define PASSWORD_USED_YES        1
#define PASSWORD_USED_NO_MENTION 2



typedef struct st_mysql_server_auth_info
{
  
  const char *user_name;

  
  unsigned int user_name_length;

  
  const char *auth_string;

  
  unsigned long auth_string_length;

  
  char authenticated_as[MYSQL_USERNAME_LENGTH+1]; 


    
  char external_user[MYSQL_USERNAME_LENGTH+1];

  
  int  password_used;

  
  const char *host_or_ip;

  
  unsigned int host_or_ip_length;

  
  MYSQL_THD thd;

} MYSQL_SERVER_AUTH_INFO;


struct st_mysql_auth
{
  int interface_version;                        
  
  const char *client_auth_plugin;
  
  int (*authenticate_user)(MYSQL_PLUGIN_VIO *vio, MYSQL_SERVER_AUTH_INFO *info);
};

#ifdef __cplusplus
}
#endif

#endif

