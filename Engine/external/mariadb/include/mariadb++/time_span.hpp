








#ifndef _MARIADB_TIME_SPAN_HPP_
#define _MARIADB_TIME_SPAN_HPP_

#include <sstream>
#include <mariadb++/types.hpp>

namespace mariadb {
class time_span {
   public:
    
    time_span(u32 days = 0, u8 hours = 0, u8 minutes = 0, u8 seconds = 0, u16 milliseconds = 0,
              bool negative = false);

    
    time_span(const time_span& dur);

    
    int compare(const time_span& rhs) const;

    time_span& operator=(const time_span& rhs);
    bool operator==(const time_span& rhs) const;
    bool operator!=(const time_span& rhs) const;
    bool operator<(const time_span& rhs) const;
    bool operator<=(const time_span& rhs) const;
    bool operator>(const time_span& rhs) const;
    bool operator>=(const time_span& rhs) const;

    
    void set(u32 days = 0, u8 hours = 0, u8 minutes = 0, u8 seconds = 0, u16 milliseconds = 0,
             bool negative = false);

    
    bool zero() const;

    
    bool negative() const;

    
    bool negative(bool negative);

    
    u32 days() const;

    
    u32 days(u32 day);

    
    u8 hours() const;

    
    u8 hours(u8 hour);

    
    u8 minutes() const;

    
    u8 minutes(u8 minute);

    
    u8 seconds() const;

    
    u8 seconds(u8 second);

    
    u16 milliseconds() const;

    
    u16 milliseconds(u16 millisecond);

    
    u64 total_hours() const;

    
    u64 total_minutes() const;

    
    u64 total_seconds() const;

    
    u64 total_milliseconds() const;

   private:
    bool m_negative = false;
    u32 m_days = 0;
    u8 m_hours = 0;
    u8 m_minutes = 0;
    u8 m_seconds = 0;
    u16 m_milliseconds = 0;
};

std::ostream& operator<<(std::ostream& os, const time_span& ts);
}

#endif
