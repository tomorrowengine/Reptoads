

#ifndef WSREP_UTILS_H
#define WSREP_UTILS_H

#include "wsrep_priv.h"
#include "wsrep_mysqld.h"

unsigned int wsrep_check_ip (const char* const addr, bool *is_ipv6);
size_t wsrep_guess_ip (char* buf, size_t buf_len);


size_t wsrep_host_len(const char* addr, size_t addr_len);

namespace wsp {

class Address {
public:
  Address()
    : m_address_len(0), m_family(UNSPEC), m_port(0), m_valid(false)
  {
    memset(m_address, 0, sizeof(m_address));
  }
  Address(const char *addr_in)
    : m_address_len(0), m_family(UNSPEC), m_port(0), m_valid(false)
  {
    memset(m_address, 0, sizeof(m_address));
    parse_addr(addr_in);
  }
  bool is_valid() { return m_valid; }
  bool is_ipv6() { return (m_family == INET6); }

  const char* get_address() { return m_address; }
  size_t get_address_len() { return m_address_len; }
  int get_port() { return m_port; }

private:
  enum family {
    UNSPEC= 0,
    INET,                                       
    INET6,                                      
  };

  char   m_address[256];
  size_t m_address_len;
  family m_family;
  int    m_port;
  bool   m_valid;

  void parse_addr(const char *addr_in) {
    const char *start;
    const char *end;
    const char *port;
    const char* open_bracket= strchr(const_cast<char *>(addr_in), '[');
    const char* close_bracket= strchr(const_cast<char *>(addr_in), ']');
    const char* colon= strchr(const_cast<char *>(addr_in), ':');
    const char* dot= strchr(const_cast<char *>(addr_in), '.');

    int cc= colon_count(addr_in);

    if (open_bracket != NULL ||
        dot == NULL ||
        (colon != NULL && (dot == NULL || colon < dot)))
    {
      
      if (open_bracket != NULL) {
        
        if (close_bracket == NULL &&
            open_bracket < close_bracket)       
        {
          m_valid= false;
          return;
        }

        start= open_bracket + 1;
        end= close_bracket;

        
        port= strchr(close_bracket, ':');
        if ((port != NULL) && parse_port(port + 1))
        {
          return;                               
        }
        m_family= INET6;
      }
      else
      {
        switch (cc) {
        case 0:
          
          start= addr_in;
          end= addr_in + strlen(addr_in);
          break;
        case 1:
          
          start= addr_in;
          end= colon;
          if (parse_port(colon + 1))
            return;                             
          break;
        default:
          
          start= addr_in;
          end= addr_in + strlen(addr_in);
          m_family= INET6;
          break;
        }
      }
    } else {                                    
      start= addr_in;
      if (colon != NULL) {                      
        end= colon;
        if (parse_port(colon + 1))
          return;                               
      } else {
        end= addr_in + strlen(addr_in);
      }
    }

    size_t len= end - start;

    
    if (len >= sizeof(m_address))
    {
      
      m_valid= false;
      return;
    }

    memcpy(m_address, start, len);
    m_address[len]= '\0';
    m_address_len= ++ len;
    m_valid= true;
    return;
  }

  int colon_count(const char *addr) {
    int count= 0, i= 0;

    while(addr[i] != '\0')
    {
      if (addr[i] == ':') ++count;
      ++ i;
    }
    return count;
  }

  bool parse_port(const char *port) {
    errno= 0;                                   
    m_port= strtol(port, NULL, 10);
    if (errno == EINVAL || errno == ERANGE)
    {
      m_port= 0;                                
      m_valid= false;
      return true;
    }
    return false;
  }
};

class Config_state
{
public:
  Config_state() : view_(), status_(WSREP_MEMBER_UNDEFINED)
  {}

  void set(wsrep_member_status_t status, const wsrep_view_info_t* view)
  {
    wsrep_notify_status(status, view);

    lock();

    status_= status;
    view_= *view;
    member_info_.clear();

    wsrep_member_info_t memb;
    for(int i= 0; i < view->memb_num; i ++)
    {
      memb= view->members[i];
      member_info_.append_val(memb);
    }

    unlock();
  }

  void set(wsrep_member_status_t status)
  {
    wsrep_notify_status(status, 0);
    lock();
    status_= status;
    unlock();
  }

  wsrep_view_info_t get_view_info() const
  {
    return view_;
  }

  wsrep_member_status_t get_status() const
  {
    return status_;
  }

  Dynamic_array<wsrep_member_info_t> * get_member_info()
  {
    return &member_info_;
  }

  int lock()
  {
    return mysql_mutex_lock(&LOCK_wsrep_config_state);
  }

  int unlock()
  {
    return mysql_mutex_unlock(&LOCK_wsrep_config_state);
  }

private:
  wsrep_view_info_t                  view_;
  wsrep_member_status_t              status_;
  Dynamic_array<wsrep_member_info_t> member_info_;
};

} 

extern wsp::Config_state *wsrep_config_state;

namespace wsp {

class env
{
private:
    size_t len_;
    char** env_;
    int    errno_;
    bool ctor_common(char** e);
    void dtor();
    env& operator =(env);
public:
    explicit env(char** env);
    explicit env(const env&);
    ~env();
    int append(const char* var); 
    int error() const { return errno_; }
    char** operator()() { return env_; }
};


class process
{
private:
    const char* const str_;
    FILE*       io_;
    int         err_;
    pid_t       pid_;

public:

    process  (const char* cmd, const char* type, char** env);
    ~process ();

    FILE* pipe () { return io_;  }
    int   error() { return err_; }
    int   wait ();
    const char* cmd() { return str_; }
};

class thd
{
  class thd_init
  {
  public:
    thd_init()  { my_thread_init(); }
    ~thd_init() { my_thread_end();  }
  }
  init;

  thd (const thd&);
  thd& operator= (const thd&);

public:

  thd(my_bool wsrep_on);
  ~thd();
  THD* const ptr;
};

class string
{
public:
    string() : string_(0) {}
    explicit string(size_t s) : string_(static_cast<char*>(malloc(s))) {}
    char* operator()() { return string_; }
    void set(char* str) { if (string_) free (string_); string_ = str; }
    ~string() { set (0); }
private:
    char* string_;
};

#ifdef REMOVED
class lock
{
  pthread_mutex_t* const mtx_;

public:

  lock (pthread_mutex_t* mtx) : mtx_(mtx)
  {
    int err = pthread_mutex_lock (mtx_);

    if (err)
    {
      WSREP_ERROR("Mutex lock failed: %s", strerror(err));
      abort();
    }
  }

  virtual ~lock ()
  {
    int err = pthread_mutex_unlock (mtx_);

    if (err)
    {
      WSREP_ERROR("Mutex unlock failed: %s", strerror(err));
      abort();
    }
  }

  inline void wait (pthread_cond_t* cond)
  {
    pthread_cond_wait (cond, mtx_);
  }

private:

  lock (const lock&);
  lock& operator=(const lock&);

};

class monitor
{
  int             mutable refcnt;
  pthread_mutex_t mutable mtx;
  pthread_cond_t  mutable cond;

public:

  monitor() : refcnt(0)
  {
    pthread_mutex_init (&mtx, NULL);
    pthread_cond_init  (&cond, NULL);
  }

  ~monitor()
  {
    pthread_mutex_destroy (&mtx);
    pthread_cond_destroy  (&cond);
  }

  void enter() const
  {
    lock l(&mtx);

    while (refcnt)
    {
      l.wait(&cond);
    }
    refcnt++;
  }

  void leave() const
  {
    lock l(&mtx);

    refcnt--;
    if (refcnt == 0)
    {
      pthread_cond_signal (&cond);
    }
  }

private:

  monitor (const monitor&);
  monitor& operator= (const monitor&);
};

class critical
{
  const monitor& mon;

public:

  critical(const monitor& m) : mon(m) { mon.enter(); }

  ~critical() { mon.leave(); }

private:

  critical (const critical&);
  critical& operator= (const critical&);
};
#endif

} 

#endif 
