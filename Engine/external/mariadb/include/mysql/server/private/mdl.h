#ifndef MDL_H
#define MDL_H


#include "sql_plist.h"
#include <my_sys.h>
#include <m_string.h>
#include <mysql_com.h>
#include <lf.h>

class THD;

class MDL_context;
class MDL_lock;
class MDL_ticket;
bool  ok_for_lower_case_names(const char *name);


#define ENTER_COND(C, M, S, O) enter_cond(C, M, S, O, __func__, __FILE__, __LINE__)


#define EXIT_COND(S) exit_cond(S, __func__, __FILE__, __LINE__)



class MDL_context_owner
{
public:
  virtual ~MDL_context_owner() {}

  
  virtual void enter_cond(mysql_cond_t *cond, mysql_mutex_t *mutex,
                          const PSI_stage_info *stage, PSI_stage_info *old_stage,
                          const char *src_function, const char *src_file,
                          int src_line) = 0;

  
  virtual void exit_cond(const PSI_stage_info *stage,
                         const char *src_function, const char *src_file,
                         int src_line) = 0;
  
  virtual int  is_killed() = 0;

  
  virtual THD* get_thd() = 0;

  
  virtual bool notify_shared_lock(MDL_context_owner *in_use,
                                  bool needs_thr_lock_abort) = 0;
};



enum enum_mdl_type {
  
  MDL_INTENTION_EXCLUSIVE= 0,
  
  MDL_SHARED,
  
  MDL_SHARED_HIGH_PRIO,
  
  MDL_SHARED_READ,
  
  MDL_SHARED_WRITE,
  
  MDL_SHARED_UPGRADABLE,
  
  MDL_SHARED_READ_ONLY,
  
  MDL_SHARED_NO_WRITE,
  
  MDL_SHARED_NO_READ_WRITE,
  
  MDL_EXCLUSIVE,
  
  MDL_TYPE_END};




enum enum_mdl_duration {
  
  MDL_STATEMENT= 0,
  
  MDL_TRANSACTION,
  
  MDL_EXPLICIT,
  
  MDL_DURATION_END };



#define MAX_MDLKEY_LENGTH (1 + NAME_LEN + 1 + NAME_LEN + 1)




class MDL_key
{
public:
#ifdef HAVE_PSI_INTERFACE
  static void init_psi_keys();
#endif

  
  enum enum_mdl_namespace { GLOBAL=0,
                            SCHEMA,
                            TABLE,
                            FUNCTION,
                            PROCEDURE,
                            PACKAGE_BODY,
                            TRIGGER,
                            EVENT,
                            COMMIT,
                            USER_LOCK,           
                            
                            NAMESPACE_END };

  const uchar *ptr() const { return (uchar*) m_ptr; }
  uint length() const { return m_length; }

  const char *db_name() const { return m_ptr + 1; }
  uint db_name_length() const { return m_db_name_length; }

  const char *name() const { return m_ptr + m_db_name_length + 2; }
  uint name_length() const { return m_length - m_db_name_length - 3; }

  enum_mdl_namespace mdl_namespace() const
  { return (enum_mdl_namespace)(m_ptr[0]); }

  
  void mdl_key_init(enum_mdl_namespace mdl_namespace_arg,
                    const char *db, const char *name_arg)
  {
    m_ptr[0]= (char) mdl_namespace_arg;
    
    DBUG_ASSERT(strlen(db) <= NAME_LEN);
    DBUG_ASSERT(strlen(name_arg) <= NAME_LEN);
    m_db_name_length= static_cast<uint16>(strmake(m_ptr + 1, db, NAME_LEN) -
                                          m_ptr - 1);
    m_length= static_cast<uint16>(strmake(m_ptr + m_db_name_length + 2,
                                          name_arg,
                                          NAME_LEN) - m_ptr + 1);
    m_hash_value= my_hash_sort(&my_charset_bin, (uchar*) m_ptr + 1,
                               m_length - 1);
    DBUG_SLOW_ASSERT(mdl_namespace_arg == USER_LOCK || ok_for_lower_case_names(db));
  }
  void mdl_key_init(const MDL_key *rhs)
  {
    memcpy(m_ptr, rhs->m_ptr, rhs->m_length);
    m_length= rhs->m_length;
    m_db_name_length= rhs->m_db_name_length;
    m_hash_value= rhs->m_hash_value;
  }
  bool is_equal(const MDL_key *rhs) const
  {
    return (m_length == rhs->m_length &&
            memcmp(m_ptr, rhs->m_ptr, m_length) == 0);
  }
  
  int cmp(const MDL_key *rhs) const
  {
    
    return memcmp(m_ptr, rhs->m_ptr, MY_MIN(m_length, rhs->m_length));
  }

  MDL_key(const MDL_key *rhs)
  {
    mdl_key_init(rhs);
  }
  MDL_key(enum_mdl_namespace namespace_arg,
          const char *db_arg, const char *name_arg)
  {
    mdl_key_init(namespace_arg, db_arg, name_arg);
  }
  MDL_key() {} 

  
  const PSI_stage_info * get_wait_state_name() const
  {
    return & m_namespace_to_wait_state_name[(int)mdl_namespace()];
  }
  my_hash_value_type hash_value() const
  {
    return m_hash_value + mdl_namespace();
  }
  my_hash_value_type tc_hash_value() const
  {
    return m_hash_value;
  }

private:
  uint16 m_length;
  uint16 m_db_name_length;
  my_hash_value_type m_hash_value;
  char m_ptr[MAX_MDLKEY_LENGTH];
  static PSI_stage_info m_namespace_to_wait_state_name[NAMESPACE_END];
private:
  MDL_key(const MDL_key &);                     
  MDL_key &operator=(const MDL_key &);          
  friend my_hash_value_type mdl_hash_function(CHARSET_INFO *,
                                              const uchar *, size_t);
};




class MDL_request
{
public:
  
  enum          enum_mdl_type type;
  
  enum enum_mdl_duration duration;

  
  MDL_request *next_in_list;
  MDL_request **prev_in_list;
  
  MDL_ticket *ticket;

  
  MDL_key key;

public:

  static void *operator new(size_t size, MEM_ROOT *mem_root) throw ()
  { return alloc_root(mem_root, size); }
  static void operator delete(void *, MEM_ROOT *) {}

  void init(MDL_key::enum_mdl_namespace namespace_arg,
            const char *db_arg, const char *name_arg,
            enum_mdl_type mdl_type_arg,
            enum_mdl_duration mdl_duration_arg);
  void init(const MDL_key *key_arg, enum_mdl_type mdl_type_arg,
            enum_mdl_duration mdl_duration_arg);
  
  inline void set_type(enum_mdl_type type_arg)
  {
    DBUG_ASSERT(ticket == NULL);
    type= type_arg;
  }
  void move_from(MDL_request &from)
  {
    type= from.type;
    duration= from.duration;
    ticket= from.ticket;
    next_in_list= from.next_in_list;
    prev_in_list= from.prev_in_list;
    key.mdl_key_init(&from.key);
    from.ticket=  NULL; 
  }

  
  bool is_write_lock_request() const
  {
    return (type >= MDL_SHARED_WRITE &&
            type != MDL_SHARED_READ_ONLY);
  }

  
  MDL_request& operator=(const MDL_request &)
  {
    ticket= NULL;
    
    return *this;
  }
  
  MDL_request() {}

  MDL_request(const MDL_request *rhs)
    :type(rhs->type),
    duration(rhs->duration),
    ticket(NULL),
    key(&rhs->key)
  {}
};


typedef void (*mdl_cached_object_release_hook)(void *);




class MDL_wait_for_graph_visitor
{
public:
  virtual bool enter_node(MDL_context *node) = 0;
  virtual void leave_node(MDL_context *node) = 0;

  virtual bool inspect_edge(MDL_context *dest) = 0;
  virtual ~MDL_wait_for_graph_visitor();
  MDL_wait_for_graph_visitor() {}
};



class MDL_wait_for_subgraph
{
public:
  virtual ~MDL_wait_for_subgraph();

  
  virtual bool accept_visitor(MDL_wait_for_graph_visitor *gvisitor) = 0;

  enum enum_deadlock_weight
  {
    DEADLOCK_WEIGHT_DML= 0,
    DEADLOCK_WEIGHT_DDL= 100
  };
  
  virtual uint get_deadlock_weight() const = 0;
};




class MDL_ticket : public MDL_wait_for_subgraph
{
public:
  
  MDL_ticket *next_in_context;
  MDL_ticket **prev_in_context;
  
  MDL_ticket *next_in_lock;
  MDL_ticket **prev_in_lock;
public:
#ifdef WITH_WSREP
  void wsrep_report(bool debug);
#endif 
  bool has_pending_conflicting_lock() const;

  MDL_context *get_ctx() const { return m_ctx; }
  bool is_upgradable_or_exclusive() const
  {
    return m_type == MDL_SHARED_UPGRADABLE ||
           m_type == MDL_SHARED_NO_WRITE ||
           m_type == MDL_SHARED_NO_READ_WRITE ||
           m_type == MDL_EXCLUSIVE;
  }
  enum_mdl_type get_type() const { return m_type; }
  MDL_lock *get_lock() const { return m_lock; }
  MDL_key *get_key() const;
  void downgrade_lock(enum_mdl_type type);

  bool has_stronger_or_equal_type(enum_mdl_type type) const;

  bool is_incompatible_when_granted(enum_mdl_type type) const;
  bool is_incompatible_when_waiting(enum_mdl_type type) const;

  
  virtual bool accept_visitor(MDL_wait_for_graph_visitor *dvisitor);
  virtual uint get_deadlock_weight() const;
private:
  friend class MDL_context;

  MDL_ticket(MDL_context *ctx_arg, enum_mdl_type type_arg
#ifndef DBUG_OFF
             , enum_mdl_duration duration_arg
#endif
            )
   : m_type(type_arg),
#ifndef DBUG_OFF
     m_duration(duration_arg),
#endif
     m_ctx(ctx_arg),
     m_lock(NULL)
  {}

  static MDL_ticket *create(MDL_context *ctx_arg, enum_mdl_type type_arg
#ifndef DBUG_OFF
                            , enum_mdl_duration duration_arg
#endif
                            );
  static void destroy(MDL_ticket *ticket);
private:
  
  enum enum_mdl_type m_type;
#ifndef DBUG_OFF
  
  enum_mdl_duration m_duration;
#endif
  
  MDL_context *m_ctx;

  
  MDL_lock *m_lock;

private:
  MDL_ticket(const MDL_ticket &);               
  MDL_ticket &operator=(const MDL_ticket &);    
};




class MDL_savepoint
{
public:
  MDL_savepoint() {};

private:
  MDL_savepoint(MDL_ticket *stmt_ticket, MDL_ticket *trans_ticket)
    : m_stmt_ticket(stmt_ticket), m_trans_ticket(trans_ticket)
  {}

  friend class MDL_context;

private:
  
  MDL_ticket *m_stmt_ticket;
  
  MDL_ticket *m_trans_ticket;
};




class MDL_wait
{
public:
  MDL_wait();
  ~MDL_wait();

  enum enum_wait_status { EMPTY = 0, GRANTED, VICTIM, TIMEOUT, KILLED };

  bool set_status(enum_wait_status result_arg);
  enum_wait_status get_status();
  void reset_status();
  enum_wait_status timed_wait(MDL_context_owner *owner,
                              struct timespec *abs_timeout,
                              bool signal_timeout,
                              const PSI_stage_info *wait_state_name);
private:
  
  mysql_mutex_t m_LOCK_wait_status;
  mysql_cond_t m_COND_wait_status;
  enum_wait_status m_wait_status;
};


typedef I_P_List<MDL_request, I_P_List_adapter<MDL_request,
                 &MDL_request::next_in_list,
                 &MDL_request::prev_in_list>,
                 I_P_List_counter>
        MDL_request_list;



class MDL_context
{
public:
  typedef I_P_List<MDL_ticket,
                   I_P_List_adapter<MDL_ticket,
                                    &MDL_ticket::next_in_context,
                                    &MDL_ticket::prev_in_context> >
          Ticket_list;

  typedef Ticket_list::Iterator Ticket_iterator;

  MDL_context();
  void destroy();

  bool try_acquire_lock(MDL_request *mdl_request);
  bool acquire_lock(MDL_request *mdl_request, double lock_wait_timeout);
  bool acquire_locks(MDL_request_list *requests, double lock_wait_timeout);
  bool upgrade_shared_lock(MDL_ticket *mdl_ticket,
                           enum_mdl_type new_type,
                           double lock_wait_timeout);

  bool clone_ticket(MDL_request *mdl_request);

  void release_all_locks_for_name(MDL_ticket *ticket);
  void release_lock(MDL_ticket *ticket);

  bool is_lock_owner(MDL_key::enum_mdl_namespace mdl_namespace,
                     const char *db, const char *name,
                     enum_mdl_type mdl_type);
  unsigned long get_lock_owner(MDL_key *mdl_key);

  bool has_lock(const MDL_savepoint &mdl_savepoint, MDL_ticket *mdl_ticket);

  inline bool has_locks() const
  {
    return !(m_tickets[MDL_STATEMENT].is_empty() &&
             m_tickets[MDL_TRANSACTION].is_empty() &&
             m_tickets[MDL_EXPLICIT].is_empty());
  }
  inline bool has_transactional_locks() const
  {
    return !m_tickets[MDL_TRANSACTION].is_empty();
  }

  MDL_savepoint mdl_savepoint()
  {
    return MDL_savepoint(m_tickets[MDL_STATEMENT].front(),
                         m_tickets[MDL_TRANSACTION].front());
  }

  void set_explicit_duration_for_all_locks();
  void set_transaction_duration_for_all_locks();
  void set_lock_duration(MDL_ticket *mdl_ticket, enum_mdl_duration duration);

  void release_statement_locks();
  void release_transactional_locks();
  void release_explicit_locks();
  void rollback_to_savepoint(const MDL_savepoint &mdl_savepoint);

  MDL_context_owner *get_owner() { return m_owner; }

  
  inline uint get_deadlock_weight() const
  { return m_waiting_for->get_deadlock_weight(); }
  
  void init(MDL_context_owner *arg) { m_owner= arg; }

  void set_needs_thr_lock_abort(bool needs_thr_lock_abort)
  {
    
    m_needs_thr_lock_abort= needs_thr_lock_abort;
  }
  bool get_needs_thr_lock_abort() const
  {
    return m_needs_thr_lock_abort;
  }
public:
  
  MDL_wait m_wait;
private:
  
  Ticket_list m_tickets[MDL_DURATION_END];
  MDL_context_owner *m_owner;
  
  bool m_needs_thr_lock_abort;

  
  mysql_prlock_t m_LOCK_waiting_for;
  
  MDL_wait_for_subgraph *m_waiting_for;
  LF_PINS *m_pins;
private:
  MDL_ticket *find_ticket(MDL_request *mdl_req,
                          enum_mdl_duration *duration);
  void release_locks_stored_before(enum_mdl_duration duration, MDL_ticket *sentinel);
  void release_lock(enum_mdl_duration duration, MDL_ticket *ticket);
  bool try_acquire_lock_impl(MDL_request *mdl_request,
                             MDL_ticket **out_ticket);
  bool fix_pins();

public:
  THD *get_thd() const { return m_owner->get_thd(); }
  bool has_explicit_locks();
  void find_deadlock();

  ulong get_thread_id() const { return thd_get_thread_id(get_thd()); }

  bool visit_subgraph(MDL_wait_for_graph_visitor *dvisitor);

  
  void will_wait_for(MDL_wait_for_subgraph *waiting_for_arg)
  {
    mysql_prlock_wrlock(&m_LOCK_waiting_for);
    m_waiting_for=  waiting_for_arg;
    mysql_prlock_unlock(&m_LOCK_waiting_for);
  }

  
  void done_waiting_for()
  {
    mysql_prlock_wrlock(&m_LOCK_waiting_for);
    m_waiting_for= NULL;
    mysql_prlock_unlock(&m_LOCK_waiting_for);
  }
  void lock_deadlock_victim()
  {
    mysql_prlock_rdlock(&m_LOCK_waiting_for);
  }
  void unlock_deadlock_victim()
  {
    mysql_prlock_unlock(&m_LOCK_waiting_for);
  }
private:
  MDL_context(const MDL_context &rhs);          
  MDL_context &operator=(MDL_context &rhs);     

  
  friend int i_s_metadata_lock_info_fill_row(MDL_ticket*, void*);
};


void mdl_init();
void mdl_destroy();

extern "C" unsigned long thd_get_thread_id(const MYSQL_THD thd);


extern "C" int thd_is_connected(MYSQL_THD thd);



extern "C" ulong max_write_lock_count;

extern MYSQL_PLUGIN_IMPORT
int mdl_iterate(int (*callback)(MDL_ticket *ticket, void *arg), void *arg);
#endif
