


#ifndef SEMISYNC_SLAVE_H
#define SEMISYNC_SLAVE_H

#include "semisync.h"
#include "my_global.h"
#include "sql_priv.h"
#include "rpl_mi.h"
#include "mysql.h"

class Master_info;


class Repl_semi_sync_slave
  :public Repl_semi_sync_base {
public:
 Repl_semi_sync_slave() :m_slave_enabled(false) {}
  ~Repl_semi_sync_slave() {}

  void set_trace_level(unsigned long trace_level) {
    m_trace_level = trace_level;
  }

  
  int init_object();

  bool get_slave_enabled() {
    return m_slave_enabled;
  }

  void set_slave_enabled(bool enabled) {
    m_slave_enabled = enabled;
  }

  bool is_delay_master(){
    return m_delay_master;
  }

  void set_delay_master(bool enabled) {
    m_delay_master = enabled;
  }

  void set_kill_conn_timeout(unsigned int timeout) {
    m_kill_conn_timeout = timeout;
  }

  
  int slave_read_sync_header(const char *header, unsigned long total_len,
                             int *semi_flags,
                             const char **payload, unsigned long *payload_len);

  
  int slave_reply(Master_info* mi);
  int slave_start(Master_info *mi);
  int slave_stop(Master_info *mi);
  int request_transmit(Master_info*);
  void kill_connection(MYSQL *mysql);
  int reset_slave(Master_info *mi);

private:
  
  bool m_init_done;
  bool m_slave_enabled;        
  bool m_delay_master;
  unsigned int m_kill_conn_timeout;
};



extern my_bool rpl_semi_sync_slave_enabled;
extern my_bool rpl_semi_sync_slave_status;
extern ulong rpl_semi_sync_slave_trace_level;
extern Repl_semi_sync_slave repl_semisync_slave;

extern char rpl_semi_sync_slave_delay_master;
extern unsigned int rpl_semi_sync_slave_kill_conn_timeout;
extern unsigned long long rpl_semi_sync_slave_send_ack;

#endif 
