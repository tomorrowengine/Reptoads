

#ifndef __MYSQL_CUSTOM_BUILD_CONFIG__
#define __MYSQL_CUSTOM_BUILD_CONFIG__

#define MYSQL_PORT		5002
#ifdef __WIN__
#define MYSQL_NAMEDPIPE		"SwSqlServer"
#define MYSQL_SERVICENAME	"SwSqlServer"
#define KEY_SERVICE_PARAMETERS
"SYSTEM\\CurrentControlSet\\Services\\SwSqlServer\\Parameters"
#endif

#endif 
