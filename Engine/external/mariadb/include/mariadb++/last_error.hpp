








#ifndef _MARIADB_LAST_ERROR_HPP_
#define _MARIADB_LAST_ERROR_HPP_

#include <string>
#include "types.hpp"

namespace mariadb {
class last_error {
   public:
    
    
    
    last_error();

    
    
    
    u32 error_no() const;
    const std::string& error() const;

   protected:
    u32 m_last_error_no;
    std::string m_last_error;
};
}

#endif
