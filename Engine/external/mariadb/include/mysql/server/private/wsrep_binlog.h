

#ifndef WSREP_BINLOG_H
#define WSREP_BINLOG_H

#include "sql_class.h" 

#define HEAP_PAGE_SIZE 65536 
#define WSREP_MAX_WS_SIZE 2147483647 


int wsrep_write_cache_buf(IO_CACHE *cache, uchar **buf, size_t *buf_len);


int wsrep_write_cache (wsrep_t*  const wsrep,
                       THD*      const thd,
                       IO_CACHE* const cache,
                       size_t*   const len);


void wsrep_dump_rbr_buf(THD *thd, const void* rbr_buf, size_t buf_len);


void wsrep_dump_rbr_direct(THD* thd, IO_CACHE* cache);


void wsrep_dump_rbr_buf_with_header(THD *thd, const void *rbr_buf,
                                    size_t buf_len);

int wsrep_binlog_close_connection(THD* thd);
int wsrep_binlog_savepoint_set(THD *thd,  void *sv);
int wsrep_binlog_savepoint_rollback(THD *thd, void *sv);

#endif 
