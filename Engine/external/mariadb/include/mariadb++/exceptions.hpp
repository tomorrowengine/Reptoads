









#ifndef _MARIADB_EXCEPTIONS_HPP_
#define _MARIADB_EXCEPTIONS_HPP_

#include <string>
#include <mariadb++/types.hpp>

namespace mariadb {
namespace exception {
class base : public std::exception {
   public:
    
    
    
    base() throw() : std::exception(), m_error_id(0), m_error("Exception not defined") {}

    base(u32 error_id, const std::string& error) throw()
        : std::exception(), m_error_id(error_id), m_error(error) {}

    base(const std::string& error) throw() : std::exception(), m_error_id(0), m_error(error) {}

    
    
    
    virtual ~base() throw() {}

    
    
    
    virtual const char* what() const throw() { return m_error.c_str(); }

    u32 error_id() const throw() { return m_error_id; }

   protected:
    u32 m_error_id;
    std::string m_error;
};

class date_time : public base {
   public:
    
    
    
    date_time(u16 year, u8 month, u8 day, u8 hour, u8 minute, u8 second, u16 millisecond) throw();
};

class time : public base {
   public:
    
    
    
    time(u8 hour, u8 minute, u8 second, u16 millisecond) throw();
};

class connection : public base {
   public:
    
    
    
    connection(u32 error_id, const std::string& error) throw() : base(error_id, error) {}
};
}
}

#endif
