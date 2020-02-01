#ifndef TZTIME_INCLUDED
#define TZTIME_INCLUDED




#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "my_time.h"                            
#include "mysql_time.h"                         
#include "sql_list.h"                           
#include "sql_string.h"                         

class THD;

#if !defined(TESTTIME) && !defined(TZINFO2SQL)

class THD;


class Time_zone: public Sql_alloc 
{
public:
  Time_zone() {}                              
  
  virtual my_time_t TIME_to_gmt_sec(const MYSQL_TIME *t, 
                                    uint *error_code) const = 0;
  
  virtual void   gmt_sec_to_TIME(MYSQL_TIME *tmp, my_time_t t) const = 0;
  
  virtual const String * get_name() const = 0;

  
  virtual ~Time_zone() {};

protected:
  static inline void adjust_leap_second(MYSQL_TIME *t);
};

extern Time_zone * my_tz_UTC;
extern Time_zone * my_tz_SYSTEM;
extern Time_zone * my_tz_OFFSET0;
extern Time_zone * my_tz_find(THD *thd, const String *name);
extern my_bool     my_tz_init(THD *org_thd, const char *default_tzname, my_bool bootstrap);
extern void        my_tz_free();
extern my_time_t   sec_since_epoch_TIME(MYSQL_TIME *t);



static const int MY_TZ_TABLES_COUNT= 4;

#endif 
#endif 
