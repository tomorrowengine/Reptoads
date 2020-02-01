

#ifndef REPLICATION_H
#define REPLICATION_H



#include <mysql.h>

typedef struct st_mysql MYSQL;

#ifdef __cplusplus
extern "C" {
#endif


enum Trans_flags {
  
  TRANS_IS_REAL_TRANS = 1
};


typedef struct Trans_param {
  uint32 server_id;
  uint32 flags;

  
  const char *log_file;
  my_off_t log_pos;
} Trans_param;


typedef struct Trans_observer {
  uint32 len;

  
  int (*after_commit)(Trans_param *param);

  
  int (*after_rollback)(Trans_param *param);
} Trans_observer;


enum Binlog_storage_flags {
  
  BINLOG_STORAGE_IS_SYNCED = 1,

  
  BINLOG_GROUP_COMMIT_LEADER = 2,

  
  BINLOG_GROUP_COMMIT_TRAILER = 4
};


typedef struct Binlog_storage_param {
  uint32 server_id;
} Binlog_storage_param;


typedef struct Binlog_storage_observer {
  uint32 len;

  
  int (*after_flush)(Binlog_storage_param *param,
                     const char *log_file, my_off_t log_pos,
                     uint32 flags);

  
  int (*after_sync)(Binlog_storage_param *param,
                    const char *log_file, my_off_t log_pos,
                    uint32 flags);
} Binlog_storage_observer;


typedef struct Binlog_transmit_param {
  uint32 server_id;
  uint32 flags;
} Binlog_transmit_param;


typedef struct Binlog_transmit_observer {
  uint32 len;
  
  
  int (*transmit_start)(Binlog_transmit_param *param,
                        const char *log_file, my_off_t log_pos);

  
  int (*transmit_stop)(Binlog_transmit_param *param);

  
  int (*reserve_header)(Binlog_transmit_param *param,
                        unsigned char *header,
                        unsigned long size,
                        unsigned long *len);

  
  int (*before_send_event)(Binlog_transmit_param *param,
                           unsigned char *packet, unsigned long len,
                           const char *log_file, my_off_t log_pos );

  
  int (*after_send_event)(Binlog_transmit_param *param,
                          const char *event_buf, unsigned long len);

  
  int (*after_reset_master)(Binlog_transmit_param *param);
} Binlog_transmit_observer;


enum Binlog_relay_IO_flags {
  
  BINLOG_RELAY_IS_SYNCED = 1
};



typedef struct Binlog_relay_IO_param {
  uint32 server_id;

  
  char *host;
  char *user;
  unsigned int port;

  char *master_log_name;
  my_off_t master_log_pos;

  MYSQL *mysql;                        
} Binlog_relay_IO_param;


typedef struct Binlog_relay_IO_observer {
  uint32 len;

  
  int (*thread_start)(Binlog_relay_IO_param *param);

  
  int (*thread_stop)(Binlog_relay_IO_param *param);

  
  int (*before_request_transmit)(Binlog_relay_IO_param *param, uint32 flags);

  
  int (*after_read_event)(Binlog_relay_IO_param *param,
                          const char *packet, unsigned long len,
                          const char **event_buf, unsigned long *event_len);

  
  int (*after_queue_event)(Binlog_relay_IO_param *param,
                           const char *event_buf, unsigned long event_len,
                           uint32 flags);

  
  int (*after_reset_slave)(Binlog_relay_IO_param *param);
} Binlog_relay_IO_observer;



int register_trans_observer(Trans_observer *observer, void *p);


int unregister_trans_observer(Trans_observer *observer, void *p);


int register_binlog_storage_observer(Binlog_storage_observer *observer, void *p);


int unregister_binlog_storage_observer(Binlog_storage_observer *observer, void *p);


int register_binlog_transmit_observer(Binlog_transmit_observer *observer, void *p);


int unregister_binlog_transmit_observer(Binlog_transmit_observer *observer, void *p);


int register_binlog_relay_io_observer(Binlog_relay_IO_observer *observer, void *p);


int unregister_binlog_relay_io_observer(Binlog_relay_IO_observer *observer, void *p);


MYSQL *rpl_connect_master(MYSQL *mysql);


int get_user_var_int(const char *name,
                     long long int *value, int *null_value);


int get_user_var_real(const char *name,
                      double *value, int *null_value);


int get_user_var_str(const char *name,
                     char *value, unsigned long len,
                     unsigned int precision, int *null_value);

  

#ifdef __cplusplus
}
#endif
#endif 
