#ifndef SQL_MY_APC_INCLUDED
#define SQL_MY_APC_INCLUDED




class THD;


class Apc_target
{
  mysql_mutex_t *LOCK_thd_kill_ptr;
public:
  Apc_target() : enabled(0), apc_calls(NULL) {} 
  ~Apc_target() { DBUG_ASSERT(!enabled && !apc_calls);}

  void init(mysql_mutex_t *target_mutex);

  
  void destroy() { DBUG_ASSERT(!enabled); }

  
  void enable() { enabled++; }

  
  void disable()
  {
    DBUG_ASSERT(enabled);
    mysql_mutex_lock(LOCK_thd_kill_ptr);
    bool process= !--enabled && have_apc_requests();
    mysql_mutex_unlock(LOCK_thd_kill_ptr);
    if (unlikely(process))
      process_apc_requests();
  }

  void process_apc_requests();
  
  inline bool have_apc_requests()
  {
    return MY_TEST(apc_calls);
  }

  inline bool is_enabled() { return enabled; }
  
  
  class Apc_call
  {
  public:
    
    virtual void call_in_target_thread()= 0;
    virtual ~Apc_call() {}
  };
  
  
  bool make_apc_call(THD *caller_thd, Apc_call *call, int timeout_sec, bool *timed_out);

#ifndef DBUG_OFF
  int n_calls_processed; 
#endif
private:
  class Call_request;

  
  int enabled;

  
  Call_request *apc_calls;
 
  class Call_request
  {
  public:
    Apc_call *call; 

    
    bool processed;

    
    mysql_cond_t COND_request;
    
    
    Call_request *next;
    Call_request *prev;
    
    const char *what; 
  };

  void enqueue_request(Call_request *qe);
  void dequeue_request(Call_request *qe);

  
  Call_request *get_first_in_queue()
  {
    return apc_calls;
  }
};

#ifdef HAVE_PSI_INTERFACE
void init_show_explain_psi_keys(void);
#else
#define init_show_explain_psi_keys() 
#endif

#endif 

