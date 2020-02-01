

#include <my_config.h>

#ifndef WSREP_SST_H
#define WSREP_SST_H

#ifdef WITH_WSREP

#include <mysql.h>                    

#define WSREP_SST_OPT_ROLE     "--role"
#define WSREP_SST_OPT_ADDR     "--address"
#define WSREP_SST_OPT_AUTH     "--auth"
#define WSREP_SST_OPT_DATA     "--datadir"
#define WSREP_SST_OPT_CONF     "--defaults-file"
#define WSREP_SST_OPT_CONF_SUFFIX "--defaults-group-suffix"
#define WSREP_SST_OPT_CONF_EXTRA  "--defaults-extra-file"
#define WSREP_SST_OPT_PARENT   "--parent"
#define WSREP_SST_OPT_BINLOG   "--binlog"
#define WSREP_SST_OPT_BINLOG_INDEX "--binlog-index"


#define WSREP_SST_OPT_USER     "--user"
#define WSREP_SST_OPT_PSWD     "--password"
#define WSREP_SST_OPT_HOST     "--host"
#define WSREP_SST_OPT_PORT     "--port"
#define WSREP_SST_OPT_LPORT    "--local-port"


#define WSREP_SST_OPT_SOCKET   "--socket"
#define WSREP_SST_OPT_GTID     "--gtid"
#define WSREP_SST_OPT_BYPASS   "--bypass"
#define WSREP_SST_OPT_GTID_DOMAIN_ID "--gtid-domain-id"

#define WSREP_SST_MYSQLDUMP    "mysqldump"
#define WSREP_SST_RSYNC        "rsync"
#define WSREP_SST_SKIP         "skip"
#define WSREP_SST_MARIABACKUP  "mariabackup"
#define WSREP_SST_XTRABACKUP   "xtrabackup"
#define WSREP_SST_XTRABACKUPV2 "xtrabackupv2"
#define WSREP_SST_DEFAULT      WSREP_SST_RSYNC
#define WSREP_SST_ADDRESS_AUTO "AUTO"
#define WSREP_SST_AUTH_MASK    "********"


extern const char* wsrep_sst_method;
extern const char* wsrep_sst_receive_address;
extern const char* wsrep_sst_donor;
extern const char* wsrep_sst_auth;
extern my_bool wsrep_sst_donor_rejects_queries;


extern void wsrep_sst_grab();

extern bool wsrep_sst_wait();

extern bool wsrep_sst_continue();
extern void wsrep_sst_auth_init();
extern void wsrep_sst_auth_free();

extern void wsrep_SE_init_grab();   
extern void wsrep_SE_init_wait();   
extern void wsrep_SE_init_done();   
extern void wsrep_SE_initialized(); 

#else
#define wsrep_SE_initialized() do { } while(0)
#define wsrep_SE_init_grab() do { } while(0)
#define wsrep_SE_init_done() do { } while(0)
#define wsrep_sst_continue() (0)

#endif 
#endif 
