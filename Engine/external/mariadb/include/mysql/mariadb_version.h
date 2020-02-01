



#ifndef _mariadb_version_h_
#define _mariadb_version_h_

#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION		10
#define MARIADB_CLIENT_VERSION_STR	"10.3.14"
#define MARIADB_BASE_VERSION		"mariadb-10.3"
#define MARIADB_VERSION_ID		100314
#define MARIADB_PORT	        	3306
#define MARIADB_UNIX_ADDR               "/tmp/mysql.sock"

#define MYSQL_CONFIG_NAME               "my"
#define MYSQL_VERSION_ID                100314
#define MYSQL_SERVER_VERSION            "10.3.14-MariaDB"

#define MARIADB_PACKAGE_VERSION "3.0.9"
#define MARIADB_PACKAGE_VERSION_ID 30009
#define MARIADB_SYSTEM_TYPE "Windows"
#define MARIADB_MACHINE_TYPE "AMD64"
#define MARIADB_PLUGINDIR "C:/Program Files/MariaDB 10.3/lib/plugin"


#ifndef MYSQL_CHARSET
#define MYSQL_CHARSET			""
#endif
#endif


#define CC_SOURCE_REVISION ""

#endif 
