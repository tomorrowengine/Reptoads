#ifndef SYS_VARS_SHARED_INCLUDED
#define SYS_VARS_SHARED_INCLUDED





#include <sql_priv.h>
#include "set_var.h"

extern bool throw_bounds_warning(THD *thd, const char *name,const char *v);
extern bool throw_bounds_warning(THD *thd, const char *name,
                                 bool fixed, bool is_unsigned, longlong v);
extern bool throw_bounds_warning(THD *thd, const char *name, bool fixed,
                                 double v);
extern sys_var *intern_find_sys_var(const char *str, size_t length);

extern sys_var_chain all_sys_vars;


class PolyLock
{
public:
  virtual void rdlock()= 0;
  virtual void wrlock()= 0;
  virtual void unlock()= 0;
  virtual ~PolyLock() {}
};

class PolyLock_mutex: public PolyLock
{
  mysql_mutex_t *mutex;
public:
  PolyLock_mutex(mysql_mutex_t *arg): mutex(arg) {}
  void rdlock() { mysql_mutex_lock(mutex); }
  void wrlock() { mysql_mutex_lock(mutex); }
  void unlock() { mysql_mutex_unlock(mutex); }
};

class PolyLock_rwlock: public PolyLock
{
  mysql_rwlock_t *rwlock;
public:
  PolyLock_rwlock(mysql_rwlock_t *arg): rwlock(arg) {}
  void rdlock() { mysql_rwlock_rdlock(rwlock); }
  void wrlock() { mysql_rwlock_wrlock(rwlock); }
  void unlock() { mysql_rwlock_unlock(rwlock); }
};

class AutoWLock
{
  PolyLock *lock;
public:
  AutoWLock(PolyLock *l) : lock(l) { if (lock) lock->wrlock(); }
  ~AutoWLock() { if (lock) lock->unlock(); }
};

class AutoRLock
{
  PolyLock *lock;
public:
  AutoRLock(PolyLock *l) : lock(l) { if (lock) lock->rdlock(); }
  ~AutoRLock() { if (lock) lock->unlock(); }
};


#endif 
