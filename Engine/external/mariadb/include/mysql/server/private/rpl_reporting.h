

#ifndef RPL_REPORTING_H
#define RPL_REPORTING_H

#include <my_sys.h>                             


#define MAX_SLAVE_ERRMSG      1024


class Slave_reporting_capability
{
public:
  
  mutable mysql_mutex_t err_lock;
  
  Slave_reporting_capability(char const *thread_name);

  
  void report(loglevel level, int err_code, const char *extra_info,
              const char *msg, ...) const
    ATTRIBUTE_FORMAT(printf, 5, 6);

  
  void clear_error() {
    mysql_mutex_lock(&err_lock);
    m_last_error.clear();
    mysql_mutex_unlock(&err_lock);
  }

  
  class Error {
    friend class Slave_reporting_capability;
  public:
    Error()
    {
      clear();
    }

    void clear()
    {
      number= 0;
      message[0]= '\0';
    }

    
    uint32 number;
    
    char message[MAX_SLAVE_ERRMSG];
  };

  Error const& last_error() const { return m_last_error; }

  virtual ~Slave_reporting_capability()= 0;
private:
  
  mutable Error m_last_error;

  char const *const m_thread_name;

  
  Slave_reporting_capability(const Slave_reporting_capability& rhs);
  Slave_reporting_capability& operator=(const Slave_reporting_capability& rhs);
};

#endif 

