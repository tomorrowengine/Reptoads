









#include <sstream>
#include <mariadb++/exceptions.hpp>

using namespace mariadb;




exception::date_time::date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second,
                                u16 millisecond) throw()
    : base() {
    std::ostringstream oss;
    oss << "Invalid date time: year - " << year << ", month - " << month << ", day - " << day
        << ", hour - " << hour << ", minute - " << minute << ", second - " << second
        << ", millisecond - " << millisecond;

    m_error = oss.str();
}

exception::time::time(u8 hour, u8 minute, u8 second, u16 millisecond) throw() : base() {
    std::ostringstream oss;
    oss << "Invalid time: hour - " << hour << ", minute - " << minute << ", second - " << second
        << ", millisecond - " << millisecond;

    m_error = oss.str();
}
