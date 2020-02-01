









#ifndef _MARIADB_TIME_HPP_
#define _MARIADB_TIME_HPP_

#include <iostream>
#include <mariadb++/time_span.hpp>

#include <mysql.h>

namespace mariadb {

class time {
   public:
    
    time(u8 hour = 0, u8 minute = 0, u8 second = 0, u16 millisecond = 0);

    
    time(const time& t);

    
    time(const tm& time_struct);

    
    time(const time_t& time);

    
    time(const MYSQL_TIME& time);

    
    time(const std::string& t);

    
    virtual ~time() = default;

    
    int compare(const time& t) const;

    
    time& operator=(const time& t);

    
    bool operator==(const time& t) const;

    
    bool operator!=(const time& t) const;

    
    bool operator<(const time& t) const;

    
    bool operator<=(const time& t) const;

    
    bool operator>(const time& t) const;

    
    bool operator>=(const time& t) const;

    
    u8 hour() const;

    
    u8 hour(u8 hour);

    
    u8 minute() const;

    
    u8 minute(u8 minute);

    
    u8 second() const;

    
    u8 second(u8 second);

    
    u16 millisecond() const;

    
    u16 millisecond(u16 millisecond);

    
    virtual bool set(const std::string& t);

    
    bool set(u8 hour, u8 minute, u8 second, u16 millisecond);

    
    time add_hours(s32 hours) const;

    
    time add_minutes(s32 minutes) const;

    
    time add_seconds(s32 seconds) const;

    
    time add_milliseconds(s32 milliseconds) const;

    
    time subtract(const time_span& dur) const;

    
    time add(const time_span& dur) const;

    
    time_span time_between(const time& t) const;

    
    time_t mktime() const;

    
    double diff_time(const time& t) const;

    
    MYSQL_TIME mysql_time() const;

    
    const std::string str_time(bool with_millisecond = false) const;

    
    static time now();

    
    static time now_utc();

   protected:
    u8 m_hour;
    u8 m_minute;
    u8 m_second;
    u16 m_millisecond;
};

std::ostream& operator<<(std::ostream& os, const time& t);
}

#endif
