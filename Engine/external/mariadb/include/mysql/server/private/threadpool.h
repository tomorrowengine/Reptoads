

#define MAX_THREAD_GROUPS 100000


extern uint threadpool_min_threads;  
extern uint threadpool_idle_timeout; 
extern uint threadpool_size; 
extern uint threadpool_max_size;
extern uint threadpool_stall_limit;  
extern uint threadpool_max_threads;  
extern uint threadpool_oversubscribe;  
extern uint threadpool_prio_kickup_timer;  
#ifdef _WIN32
extern uint threadpool_mode; 
#define TP_MODE_WINDOWS 0
#define TP_MODE_GENERIC 1
#endif


struct TP_connection;
extern void tp_callback(TP_connection *c);
extern void tp_timeout_handler(TP_connection *c);




struct TP_STATISTICS
{
  
  volatile int32 num_worker_threads;
};

extern TP_STATISTICS tp_stats;



extern void tp_set_min_threads(uint val);
extern void tp_set_max_threads(uint val);
extern void tp_set_threadpool_size(uint val);
extern void tp_set_threadpool_stall_limit(uint val);
extern int tp_get_idle_thread_count();
extern int tp_get_thread_count();


extern void tp_scheduler(void);

extern int show_threadpool_idle_threads(THD *thd, SHOW_VAR *var, char *buff,
                                        enum enum_var_type scope);

enum  TP_PRIORITY {
  TP_PRIORITY_HIGH,
  TP_PRIORITY_LOW,
  TP_PRIORITY_AUTO
};


enum TP_STATE
{
  TP_STATE_IDLE,
  TP_STATE_RUNNING,
};



struct TP_connection
{
  THD*        thd;
  CONNECT*    connect;
  TP_STATE    state;
  TP_PRIORITY priority;
  TP_connection(CONNECT *c) :
    thd(0),
    connect(c),
    state(TP_STATE_IDLE),
    priority(TP_PRIORITY_HIGH)
  {}

  virtual ~TP_connection()
  {};

  
  virtual int init() = 0;

  virtual void set_io_timeout(int sec) = 0;

  
  virtual int start_io() = 0;

  virtual void wait_begin(int type)= 0;
  virtual void wait_end() = 0;

};


struct TP_pool
{
  virtual ~TP_pool(){};
  virtual int init()= 0;
  virtual TP_connection *new_connection(CONNECT *)= 0;
  virtual void add(TP_connection *c)= 0;
  virtual int set_max_threads(uint){ return 0; }
  virtual int set_min_threads(uint){ return 0; }
  virtual int set_pool_size(uint){ return 0; }
  virtual int set_idle_timeout(uint){ return 0; }
  virtual int set_oversubscribe(uint){ return 0; }
  virtual int set_stall_limit(uint){ return 0; }
  virtual int get_thread_count() { return tp_stats.num_worker_threads; }
  virtual int get_idle_thread_count(){ return 0; }
};

#ifdef _WIN32
struct TP_pool_win:TP_pool
{
  TP_pool_win(); 
  virtual int init();
  virtual ~TP_pool_win();
  virtual TP_connection *new_connection(CONNECT *c);
  virtual void add(TP_connection *);
  virtual int set_max_threads(uint);
  virtual int set_min_threads(uint);
};
#endif

struct TP_pool_generic :TP_pool
{
  TP_pool_generic();
  ~TP_pool_generic();
  virtual int init();
  virtual TP_connection *new_connection(CONNECT *c);
  virtual void add(TP_connection *);
  virtual int set_pool_size(uint);
  virtual int set_stall_limit(uint);
  virtual int get_idle_thread_count();
};
