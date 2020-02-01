#ifndef RPL_PARALLEL_H
#define RPL_PARALLEL_H

#include "log_event.h"


struct rpl_parallel;
struct rpl_parallel_entry;
struct rpl_parallel_thread_pool;

class Relay_log_info;
struct inuse_relaylog;



struct group_commit_orderer {
  
  mysql_cond_t COND_group_commit_orderer;
  uint64 wait_count;
  group_commit_orderer *prev_gco;
  group_commit_orderer *next_gco;
  
  uint64 prior_sub_id;
  
  uint64 last_sub_id;
  
  bool installed;

  enum force_switch_bits
  {
    
    MULTI_BATCH= 1,
    
    FORCE_SWITCH= 2
  };
  uint8 flags;
};


struct rpl_parallel_thread {
  bool delay_start;
  bool running;
  bool stop;
  bool pause_for_ftwrl;
  mysql_mutex_t LOCK_rpl_thread;
  mysql_cond_t COND_rpl_thread;
  mysql_cond_t COND_rpl_thread_queue;
  mysql_cond_t COND_rpl_thread_stop;
  struct rpl_parallel_thread *next;             
  struct rpl_parallel_thread_pool *pool;
  THD *thd;
  
  struct rpl_parallel_thread **current_owner;
  
  rpl_parallel_entry *current_entry;
  struct queued_event {
    queued_event *next;
    
    enum queued_event_t {
      QUEUED_EVENT,
      QUEUED_POS_UPDATE,
      QUEUED_MASTER_RESTART
    } typ;
    union {
      Log_event *ev;                            
      rpl_parallel_entry *entry_for_queued;     
    };
    rpl_group_info *rgi;
    inuse_relaylog *ir;
    ulonglong future_event_relay_log_pos;
    char event_relay_log_name[FN_REFLEN];
    char future_event_master_log_name[FN_REFLEN];
    ulonglong event_relay_log_pos;
    my_off_t future_event_master_log_pos;
    size_t event_size;
  } *event_queue, *last_in_queue;
  uint64 queued_size;
  
  queued_event *qev_free_list;
  rpl_group_info *rgi_free_list;
  group_commit_orderer *gco_free_list;
  
  queued_event *loc_qev_list, **loc_qev_last_ptr_ptr;
  size_t loc_qev_size;
  uint64 qev_free_pending;
  rpl_group_info *loc_rgi_list, **loc_rgi_last_ptr_ptr;
  group_commit_orderer *loc_gco_list, **loc_gco_last_ptr_ptr;
  
  inuse_relaylog *accumulated_ir_last;
  uint64 accumulated_ir_count;

  void enqueue(queued_event *qev)
  {
    if (last_in_queue)
      last_in_queue->next= qev;
    else
      event_queue= qev;
    last_in_queue= qev;
    queued_size+= qev->event_size;
  }

  void dequeue1(queued_event *list)
  {
    DBUG_ASSERT(list == event_queue);
    event_queue= last_in_queue= NULL;
  }

  void dequeue2(size_t dequeue_size)
  {
    queued_size-= dequeue_size;
  }

  queued_event *get_qev_common(Log_event *ev, ulonglong event_size);
  queued_event *get_qev(Log_event *ev, ulonglong event_size,
                        Relay_log_info *rli);
  queued_event *retry_get_qev(Log_event *ev, queued_event *orig_qev,
                              const char *relay_log_name,
                              ulonglong event_pos, ulonglong event_size);
  
  void loc_free_qev(queued_event *qev);
  
  void free_qev(queued_event *qev);
  rpl_group_info *get_rgi(Relay_log_info *rli, Gtid_log_event *gtid_ev,
                          rpl_parallel_entry *e, ulonglong event_size);
  
  void loc_free_rgi(rpl_group_info *rgi);
  
  void free_rgi(rpl_group_info *rgi);
  group_commit_orderer *get_gco(uint64 wait_count, group_commit_orderer *prev,
                                uint64 first_sub_id);
  
  void loc_free_gco(group_commit_orderer *gco);
  
  void batch_free();
  
  void inuse_relaylog_refcount_update();
};


struct rpl_parallel_thread_pool {
  struct rpl_parallel_thread **threads;
  struct rpl_parallel_thread *free_list;
  mysql_mutex_t LOCK_rpl_thread_pool;
  mysql_cond_t COND_rpl_thread_pool;
  uint32 count;
  bool inited;
  
  bool busy;

  rpl_parallel_thread_pool();
  int init(uint32 size);
  void destroy();
  struct rpl_parallel_thread *get_thread(rpl_parallel_thread **owner,
                                         rpl_parallel_entry *entry);
  void release_thread(rpl_parallel_thread *rpt);
};


struct rpl_parallel_entry {
  mysql_mutex_t LOCK_parallel_entry;
  mysql_cond_t COND_parallel_entry;
  uint32 domain_id;
  
  uint32 need_sub_id_signal;
  uint64 last_commit_id;
  bool active;
  
  bool force_abort;
  
  uint64 stop_count;

  
  rpl_parallel_thread **rpl_threads;
  uint32 rpl_thread_max;
  uint32 rpl_thread_idx;
  
  uint64 last_committed_sub_id;
  
  uint64 current_sub_id;
  
  uint64 largest_started_sub_id;
  rpl_group_info *current_group_info;
  
  uint64 stop_on_error_sub_id;
  
  uint64 pause_sub_id;
  
  uint64 count_queued_event_groups;
  
  uint64 count_committing_event_groups;
  
  group_commit_orderer *current_gco;

  rpl_parallel_thread * choose_thread(rpl_group_info *rgi, bool *did_enter_cond,
                                      PSI_stage_info *old_stage, bool reuse);
  int queue_master_restart(rpl_group_info *rgi,
                           Format_description_log_event *fdev);
};
struct rpl_parallel {
  HASH domain_hash;
  rpl_parallel_entry *current;
  bool sql_thread_stopping;

  rpl_parallel();
  ~rpl_parallel();
  void reset();
  rpl_parallel_entry *find(uint32 domain_id);
  void wait_for_done(THD *thd, Relay_log_info *rli);
  void stop_during_until();
  bool workers_idle();
  int wait_for_workers_idle(THD *thd);
  int do_event(rpl_group_info *serial_rgi, Log_event *ev, ulonglong event_size);
};


extern struct rpl_parallel_thread_pool global_rpl_thread_pool;


extern int rpl_parallel_resize_pool_if_no_slaves(void);
extern int rpl_parallel_activate_pool(rpl_parallel_thread_pool *pool);
extern int rpl_parallel_inactivate_pool(rpl_parallel_thread_pool *pool);
extern bool process_gtid_for_restart_pos(Relay_log_info *rli, rpl_gtid *gtid);
extern int rpl_pause_for_ftwrl(THD *thd);
extern void rpl_unpause_after_ftwrl(THD *thd);

#endif  
