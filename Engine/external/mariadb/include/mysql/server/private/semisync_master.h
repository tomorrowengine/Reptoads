


#ifndef SEMISYNC_MASTER_H
#define SEMISYNC_MASTER_H

#include "semisync.h"
#include "semisync_master_ack_receiver.h"

#ifdef HAVE_PSI_INTERFACE
extern PSI_mutex_key key_LOCK_binlog;
extern PSI_cond_key key_COND_binlog_send;
#endif

struct Tranx_node {
  char              log_name[FN_REFLEN];
  my_off_t          log_pos;
  struct Tranx_node *next;            
  struct Tranx_node *hash_next;    
};


#define BLOCK_TRANX_NODES 16
class Tranx_node_allocator
{
public:
  
  Tranx_node_allocator(uint reserved_nodes) :
    reserved_blocks(reserved_nodes/BLOCK_TRANX_NODES +
                  (reserved_nodes%BLOCK_TRANX_NODES > 1 ? 2 : 1)),
    first_block(NULL), last_block(NULL),
    current_block(NULL), last_node(-1), block_num(0) {}

  ~Tranx_node_allocator()
  {
    Block *block= first_block;
    while (block != NULL)
    {
      Block *next= block->next;
      free_block(block);
      block= next;
    }
  }

  
  Tranx_node *allocate_node()
  {
    Tranx_node *trx_node;
    Block *block= current_block;

    if (last_node == BLOCK_TRANX_NODES-1)
    {
      current_block= current_block->next;
      last_node= -1;
    }

    if (current_block == NULL && allocate_block())
    {
      current_block= block;
      if (current_block)
        last_node= BLOCK_TRANX_NODES-1;
      return NULL;
    }

    trx_node= &(current_block->nodes[++last_node]);
    trx_node->log_name[0] = '\0';
    trx_node->log_pos= 0;
    trx_node->next= 0;
    trx_node->hash_next= 0;
    return trx_node;
  }

  
  int free_all_nodes()
  {
    current_block= first_block;
    last_node= -1;
    free_blocks();
    return 0;
  }

  
  int free_nodes_before(Tranx_node* node)
  {
    Block *block;
    Block *prev_block= NULL;

    block= first_block;
    while (block != current_block->next)
    {
      
      if (&(block->nodes[0]) <= node && &(block->nodes[BLOCK_TRANX_NODES]) >= node)
      {
        
        if (first_block != block)
        {
          last_block->next= first_block;
          first_block= block;
          last_block= prev_block;
          last_block->next= NULL;
          free_blocks();
        }
        return 0;
      }
      prev_block= block;
      block= block->next;
    }

    
    DBUG_ASSERT(0);
    return 1;
  }

private:
  uint reserved_blocks;

 
  struct Block {
    Block *next;
    Tranx_node nodes[BLOCK_TRANX_NODES];
  };

  
  Block *first_block;
  
  Block *last_block;

  
  Block *current_block;

  
  int last_node;

  
  uint block_num;

  
  int allocate_block()
  {
    Block *block= (Block *)my_malloc(sizeof(Block), MYF(0));
    if (block)
    {
      block->next= NULL;

      if (first_block == NULL)
        first_block= block;
      else
        last_block->next= block;

      
      last_block= block;
      
      current_block= block;
      ++block_num;
      return 0;
    }
    return 1;
  }

  
  void free_block(Block *block)
  {
    my_free(block);
    --block_num;
  }


  
  void free_blocks()
  {
    if (current_block == NULL || current_block->next == NULL)
      return;

    
    Block *block= current_block->next->next;
    while (block_num > reserved_blocks && block != NULL)
    {
      Block *next= block->next;
      free_block(block);
      block= next;
    }
    current_block->next->next= block;
    if (block == NULL)
      last_block= current_block->next;
  }
};


class Active_tranx
  :public Trace {
private:

  Tranx_node_allocator m_allocator;
  
  Tranx_node       *m_trx_front, *m_trx_rear;

  Tranx_node      **m_trx_htb;        

  int              m_num_entries;              
  mysql_mutex_t *m_lock;                                     

  inline void assert_lock_owner();

  inline unsigned int calc_hash(const unsigned char *key, size_t length);
  unsigned int get_hash_value(const char *log_file_name, my_off_t log_file_pos);

  int compare(const char *log_file_name1, my_off_t log_file_pos1,
              const Tranx_node *node2) {
    return compare(log_file_name1, log_file_pos1,
                   node2->log_name, node2->log_pos);
  }
  int compare(const Tranx_node *node1,
              const char *log_file_name2, my_off_t log_file_pos2) {
    return compare(node1->log_name, node1->log_pos,
                   log_file_name2, log_file_pos2);
  }
  int compare(const Tranx_node *node1, const Tranx_node *node2) {
    return compare(node1->log_name, node1->log_pos,
                   node2->log_name, node2->log_pos);
  }

public:
  Active_tranx(mysql_mutex_t *lock, unsigned long trace_level);
  ~Active_tranx();

  
  int insert_tranx_node(const char *log_file_name, my_off_t log_file_pos);

  
  int clear_active_tranx_nodes(const char *log_file_name,
                               my_off_t    log_file_pos);

  
  bool is_tranx_end_pos(const char *log_file_name, my_off_t log_file_pos);

  
  static int compare(const char *log_file_name1, my_off_t log_file_pos1,
                     const char *log_file_name2, my_off_t log_file_pos2);

};


class Repl_semi_sync_master
  :public Repl_semi_sync_base {
 private:
  Active_tranx    *m_active_tranxs;  

  
  bool m_init_done;

  
  mysql_cond_t  COND_binlog_send;

  
  mysql_mutex_t LOCK_binlog;

  
  bool            m_reply_file_name_inited;

  
  char            m_reply_file_name[FN_REFLEN];

  
  my_off_t        m_reply_file_pos;

  
  bool            m_wait_file_name_inited;

  
  char            m_wait_file_name[FN_REFLEN];

  
  my_off_t        m_wait_file_pos;

  
  bool            m_commit_file_name_inited;

  
  char            m_commit_file_name[FN_REFLEN];

  
  my_off_t        m_commit_file_pos;

  
  volatile bool            m_master_enabled;      
  unsigned long           m_wait_timeout;      

  bool            m_state;                    

  
  ulong m_wait_point;

  void lock();
  void unlock();
  void cond_broadcast();
  int  cond_timewait(struct timespec *wait_time);

  
  bool is_on() {
    return (m_state);
  }

  void set_master_enabled(bool enabled) {
    m_master_enabled = enabled;
  }

  
  int switch_off();

  
  int try_switch_on(int server_id,
                    const char *log_file_name, my_off_t log_file_pos);

 public:
  Repl_semi_sync_master();
  ~Repl_semi_sync_master() {}

  void cleanup();

  bool get_master_enabled() {
    return m_master_enabled;
  }
  void set_trace_level(unsigned long trace_level) {
    m_trace_level = trace_level;
    if (m_active_tranxs)
      m_active_tranxs->m_trace_level = trace_level;
  }

  
  void set_wait_timeout(unsigned long wait_timeout) {
    m_wait_timeout = wait_timeout;
  }

  
  void set_wait_point(unsigned long ack_point)
  {
    m_wait_point = ack_point;
  }

  ulong wait_point() 
  {
    return m_wait_point; 
  }

  
  int init_object();

  
  int enable_master();

  
  int disable_master();

  
  void add_slave();
    
  
  void remove_slave();

  
  int report_reply_packet(uint32 server_id, const uchar *packet,
                        ulong packet_len);

  
  int report_reply_binlog(uint32 server_id,
                          const char* log_file_name,
                          my_off_t end_offset);

  
  int commit_trx(const char* trx_wait_binlog_name,
                 my_off_t trx_wait_binlog_pos);

  
  int wait_after_sync(const char* log_file, my_off_t log_pos);

  
  int wait_after_commit(THD* thd, bool all);

  
  int wait_after_rollback(THD *thd, bool all);
  
  int report_binlog_update(THD *thd, const char *log_file,my_off_t log_pos);

  int dump_start(THD* thd,
                  const char *log_file,
                  my_off_t log_pos);

  void dump_end(THD* thd);

  
  int reserve_sync_header(String* packet);

  
  int update_sync_header(THD* thd, unsigned char *packet,
                         const char *log_file_name,
                         my_off_t log_file_pos,
                         bool* need_sync);

  
  int write_tranx_in_binlog(const char* log_file_name, my_off_t log_file_pos);

  
  int flush_net(THD* thd, const char *event_buf);

  
  void set_export_stats();

  
  int after_reset_master();

  
  int before_reset_master();

  void check_and_switch();
};

enum rpl_semi_sync_master_wait_point_t {
  SEMI_SYNC_MASTER_WAIT_POINT_AFTER_BINLOG_SYNC,
  SEMI_SYNC_MASTER_WAIT_POINT_AFTER_STORAGE_COMMIT,
};

extern Repl_semi_sync_master repl_semisync_master;
extern Ack_receiver ack_receiver;


extern my_bool rpl_semi_sync_master_enabled;
extern my_bool rpl_semi_sync_master_status;
extern ulong rpl_semi_sync_master_wait_point;
extern ulong rpl_semi_sync_master_clients;
extern ulong rpl_semi_sync_master_timeout;
extern ulong rpl_semi_sync_master_trace_level;
extern ulong rpl_semi_sync_master_yes_transactions;
extern ulong rpl_semi_sync_master_no_transactions;
extern ulong rpl_semi_sync_master_off_times;
extern ulong rpl_semi_sync_master_wait_timeouts;
extern ulong rpl_semi_sync_master_timefunc_fails;
extern ulong rpl_semi_sync_master_num_timeouts;
extern ulong rpl_semi_sync_master_wait_sessions;
extern ulong rpl_semi_sync_master_wait_pos_backtraverse;
extern ulong rpl_semi_sync_master_avg_trx_wait_time;
extern ulong rpl_semi_sync_master_avg_net_wait_time;
extern ulonglong rpl_semi_sync_master_net_wait_num;
extern ulonglong rpl_semi_sync_master_trx_wait_num;
extern ulonglong rpl_semi_sync_master_net_wait_time;
extern ulonglong rpl_semi_sync_master_trx_wait_time;
extern unsigned long long rpl_semi_sync_master_request_ack;
extern unsigned long long rpl_semi_sync_master_get_ack;


extern char rpl_semi_sync_master_wait_no_slave;
extern Repl_semi_sync_master repl_semisync_master;

extern PSI_stage_info stage_waiting_for_semi_sync_ack_from_slave;
extern PSI_stage_info stage_reading_semi_sync_ack;
extern PSI_stage_info stage_waiting_for_semi_sync_slave;

void semi_sync_master_deinit();

#endif 
