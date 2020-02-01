#ifndef SQL_SERVERS_INCLUDED
#define SQL_SERVERS_INCLUDED



#include "slave.h" 

class THD;
typedef struct st_lex_server_options LEX_SERVER_OPTIONS;
typedef struct st_mem_root MEM_ROOT;


typedef struct st_federated_server
{
  const char *server_name;
  long port;
  size_t server_name_length;
  const char *db, *scheme, *username, *password, *socket, *owner, *host, *sport;
} FOREIGN_SERVER;


bool servers_init(bool dont_read_server_table);
bool servers_reload(THD *thd);
void servers_free(bool end=0);


int create_server(THD *thd, LEX_SERVER_OPTIONS *server_options);

 
int drop_server(THD *thd, LEX_SERVER_OPTIONS *server_options);


int alter_server(THD *thd, LEX_SERVER_OPTIONS *server_options);


FOREIGN_SERVER *get_server_by_name(MEM_ROOT *mem, const char *server_name,
                                   FOREIGN_SERVER *server_buffer);

#endif 
