#ifndef MYSQL_PLUGIN_PASSWORD_VALIDATION_INCLUDED




#define MYSQL_PLUGIN_PASSWORD_VALIDATION_INCLUDED

#include <mysql/plugin.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MariaDB_PASSWORD_VALIDATION_INTERFACE_VERSION 0x0100


struct st_mariadb_password_validation
{
  int interface_version;                        
  
  int (*validate_password)(MYSQL_CONST_LEX_STRING *username,
                           MYSQL_CONST_LEX_STRING *password);
};

#ifdef __cplusplus
}
#endif

#endif

