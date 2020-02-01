


#ifndef SEMISYNC_H
#define SEMISYNC_H

#include "mysqld.h"
#include "log_event.h"
#include "replication.h"


class Trace {
public:
  static const unsigned long k_trace_function;
  static const unsigned long k_trace_general;
  static const unsigned long k_trace_detail;
  static const unsigned long k_trace_net_wait;

  unsigned long           m_trace_level;                      

  Trace()
    :m_trace_level(0L)
  {}
  Trace(unsigned long trace_level)
    :m_trace_level(trace_level)
  {}
};


class Repl_semi_sync_base
  :public Trace {
public:
  static const unsigned char  k_sync_header[2];     

  
  static const unsigned char k_packet_magic_num;
  static const unsigned char k_packet_flag_sync;
};


#define REPLY_MAGIC_NUM_LEN 1
#define REPLY_BINLOG_POS_LEN 8
#define REPLY_BINLOG_NAME_LEN (FN_REFLEN + 1)
#define REPLY_MAGIC_NUM_OFFSET 0
#define REPLY_BINLOG_POS_OFFSET (REPLY_MAGIC_NUM_OFFSET + REPLY_MAGIC_NUM_LEN)
#define REPLY_BINLOG_NAME_OFFSET (REPLY_BINLOG_POS_OFFSET + REPLY_BINLOG_POS_LEN)
#define REPLY_MESSAGE_MAX_LENGTH \
    (REPLY_MAGIC_NUM_LEN + REPLY_BINLOG_POS_LEN + REPLY_BINLOG_NAME_LEN)

#endif 
