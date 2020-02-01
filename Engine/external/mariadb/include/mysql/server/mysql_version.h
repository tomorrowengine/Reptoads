



#ifndef _mysql_version_h
#define _mysql_version_h
#ifdef _CUSTOMCONFIG_
#include <custom_conf.h>
#else
#define PROTOCOL_VERSION		10
#define MYSQL_SERVER_VERSION		"10.3.14-MariaDB"
#define MYSQL_BASE_VERSION		"mysqld-10.3"
#define MARIADB_BASE_VERSION		"mariadb-10.3"
#define MYSQL_SERVER_SUFFIX_DEF		""
#define FRM_VER				6
#define MYSQL_VERSION_ID		100314
#define MYSQL_PORT			3306
#define MYSQL_PORT_DEFAULT		0
#define MYSQL_UNIX_ADDR			"/tmp/mysql.sock"
#define MYSQL_CONFIG_NAME		"my"
#define MYSQL_COMPILATION_COMMENT	"mariadb.org binary distribution"
#define SERVER_MATURITY_LEVEL           MariaDB_PLUGIN_MATURITY_STABLE

#ifdef WITH_WSREP
#define WSREP_PATCH_VERSION             "24"
#endif


#endif 

#ifndef LICENSE
#define LICENSE				GPL
#endif 

#endif 
