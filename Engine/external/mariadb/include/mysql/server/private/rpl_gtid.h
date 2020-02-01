

#ifndef RPL_GTID_H
#define RPL_GTID_H

#include "hash.h"
#include "queues.h"





extern const LEX_CSTRING rpl_gtid_slave_state_table_name;

class String;

struct rpl_gtid
{
  uint32 domain_id;
  uint32 server_id;
  uint64 seq_no;
};

inline bool operator==(const rpl_gtid& lhs, const rpl_gtid& rhs)
{
  return
    lhs.domain_id == rhs.domain_id &&
    lhs.server_id == rhs.server_id &&
    lhs.seq_no    == rhs.seq_no;
};

enum enum_gtid_skip_type {
  GTID_SKIP_NOT, GTID_SKIP_STANDALONE, GTID_SKIP_TRANSACTION
};



struct gtid_waiting {
  
  struct hash_element {
    QUEUE queue;
    uint32 domain_id;
  };
  
  struct queue_element {
    uint64 wait_seq_no;
    THD *thd;
    int queue_idx;
    
    bool do_small_wait;
    
    bool done;
  };

  mysql_mutex_t LOCK_gtid_waiting;
  HASH hash;

  void init();
  void destroy();
  hash_element *get_entry(uint32 domain_id);
  int wait_for_pos(THD *thd, String *gtid_str, longlong timeout_us);
  void promote_new_waiter(gtid_waiting::hash_element *he);
  int wait_for_gtid(THD *thd, rpl_gtid *wait_gtid, struct timespec *wait_until);
  void process_wait_hash(uint64 wakeup_seq_no, gtid_waiting::hash_element *he);
  int register_in_wait_queue(THD *thd, rpl_gtid *wait_gtid, hash_element *he,
                             queue_element *elem);
  void remove_from_wait_queue(hash_element *he, queue_element *elem);
};


class Relay_log_info;
struct rpl_group_info;
class Gtid_list_log_event;


struct rpl_slave_state
{
  
  struct list_element
  {
    struct list_element *next;
    uint64 sub_id;
    uint64 seq_no;
    uint32 server_id;
    
    void *hton;
  };

  
  struct element
  {
    struct list_element *list;
    uint32 domain_id;
    
    uint64 highest_seq_no;
    
    gtid_waiting::queue_element *gtid_waiter;
    
    uint64 min_wait_seq_no;
    mysql_cond_t COND_wait_gtid;

    
    const Relay_log_info *owner_rli;
    uint32 owner_count;
    mysql_cond_t COND_gtid_ignore_duplicates;

    list_element *grab_list() { list_element *l= list; list= NULL; return l; }
    void add(list_element *l)
    {
      l->next= list;
      list= l;
    }
  };

  
  enum gtid_pos_table_state {
    GTID_POS_AUTO_CREATE,
    GTID_POS_CREATE_REQUESTED,
    GTID_POS_CREATE_IN_PROGRESS,
    GTID_POS_AVAILABLE
  };
  struct gtid_pos_table {
    struct gtid_pos_table *next;
    
    void *table_hton;
    LEX_CSTRING table_name;
    uint8 state;
  };

  
  HASH hash;
  
  mysql_mutex_t LOCK_slave_state;
  
  DYNAMIC_ARRAY gtid_sort_array;

  uint64 last_sub_id;
  
  void * volatile gtid_pos_tables;
  
  void * volatile default_gtid_pos_table;
  bool loaded;

  rpl_slave_state();
  ~rpl_slave_state();

  void truncate_hash();
  ulong count() const { return hash.records; }
  int update(uint32 domain_id, uint32 server_id, uint64 sub_id,
             uint64 seq_no, void *hton, rpl_group_info *rgi);
  int truncate_state_table(THD *thd);
  void select_gtid_pos_table(THD *thd, LEX_CSTRING *out_tablename);
  int record_gtid(THD *thd, const rpl_gtid *gtid, uint64 sub_id,
                  rpl_group_info *rgi, bool in_statement, void **out_hton);
  uint64 next_sub_id(uint32 domain_id);
  int iterate(int (*cb)(rpl_gtid *, void *), void *data,
              rpl_gtid *extra_gtids, uint32 num_extra,
              bool sort);
  int tostring(String *dest, rpl_gtid *extra_gtids, uint32 num_extra);
  bool domain_to_gtid(uint32 domain_id, rpl_gtid *out_gtid);
  int load(THD *thd, const char *state_from_master, size_t len, bool reset,
           bool in_statement);
  bool is_empty();

  element *get_element(uint32 domain_id);
  int put_back_list(uint32 domain_id, list_element *list);

  void update_state_hash(uint64 sub_id, rpl_gtid *gtid, void *hton,
                         rpl_group_info *rgi);
  int record_and_update_gtid(THD *thd, struct rpl_group_info *rgi);
  int check_duplicate_gtid(rpl_gtid *gtid, rpl_group_info *rgi);
  void release_domain_owner(rpl_group_info *rgi);
  void set_gtid_pos_tables_list(gtid_pos_table *new_list,
                                gtid_pos_table *default_entry);
  void add_gtid_pos_table(gtid_pos_table *entry);
  struct gtid_pos_table *alloc_gtid_pos_table(LEX_CSTRING *table_name,
      void *hton, rpl_slave_state::gtid_pos_table_state state);
  void free_gtid_pos_tables(struct gtid_pos_table *list);
};



struct rpl_binlog_state
{
  struct element {
    uint32 domain_id;
    HASH hash;                
    
    rpl_gtid *last_gtid;
    
    uint64 seq_no_counter;

    int update_element(const rpl_gtid *gtid);
  };
  
  HASH hash;
  
  mysql_mutex_t LOCK_binlog_state;
  my_bool initialized;

  
  DYNAMIC_ARRAY gtid_sort_array;

   rpl_binlog_state() :initialized(0) {}
  ~rpl_binlog_state();

  void init();
  void reset_nolock();
  void reset();
  void free();
  bool load(struct rpl_gtid *list, uint32 count);
  bool load(rpl_slave_state *slave_pos);
  int update_nolock(const struct rpl_gtid *gtid, bool strict);
  int update(const struct rpl_gtid *gtid, bool strict);
  int update_with_next_gtid(uint32 domain_id, uint32 server_id,
                             rpl_gtid *gtid);
  int alloc_element_nolock(const rpl_gtid *gtid);
  bool check_strict_sequence(uint32 domain_id, uint32 server_id, uint64 seq_no);
  int bump_seq_no_if_needed(uint32 domain_id, uint64 seq_no);
  int write_to_iocache(IO_CACHE *dest);
  int read_from_iocache(IO_CACHE *src);
  uint32 count();
  int get_gtid_list(rpl_gtid *gtid_list, uint32 list_size);
  int get_most_recent_gtid_list(rpl_gtid **list, uint32 *size);
  bool append_pos(String *str);
  bool append_state(String *str);
  rpl_gtid *find_nolock(uint32 domain_id, uint32 server_id);
  rpl_gtid *find(uint32 domain_id, uint32 server_id);
  rpl_gtid *find_most_recent(uint32 domain_id);
  const char* drop_domain(DYNAMIC_ARRAY *ids, Gtid_list_log_event *glev, char*);
};



struct slave_connection_state
{
  struct entry {
    rpl_gtid gtid;
    uint32 flags;
  };
  
  enum start_flags
  {
    START_OWN_SLAVE_POS= 0x1,
    START_ON_EMPTY_DOMAIN= 0x2
  };

  
  HASH hash;

  
  DYNAMIC_ARRAY gtid_sort_array;

  slave_connection_state();
  ~slave_connection_state();

  void reset() { my_hash_reset(&hash); }
  int load(const char *slave_request, size_t len);
  int load(const rpl_gtid *gtid_list, uint32 count);
  int load(rpl_slave_state *state, rpl_gtid *extra_gtids, uint32 num_extra);
  rpl_gtid *find(uint32 domain_id);
  entry *find_entry(uint32 domain_id);
  int update(const rpl_gtid *in_gtid);
  void remove(const rpl_gtid *gtid);
  void remove_if_present(const rpl_gtid *in_gtid);
  ulong count() const { return hash.records; }
  int to_string(String *out_str);
  int append_to_string(String *out_str);
  int get_gtid_list(rpl_gtid *gtid_list, uint32 list_size);
  bool is_pos_reached();
};


extern bool rpl_slave_state_tostring_helper(String *dest, const rpl_gtid *gtid,
                                            bool *first);
extern int gtid_check_rpl_slave_state_table(TABLE *table);
extern rpl_gtid *gtid_parse_string_to_list(const char *p, size_t len,
                                           uint32 *out_len);

#endif  
