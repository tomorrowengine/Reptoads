








#ifndef _MARIADB_SAVE_POINT_HPP_
#define _MARIADB_SAVE_POINT_HPP_

#include <mariadb++/types.hpp>

namespace mariadb {
class connection;
class transaction;


class save_point {
    friend class connection;
    friend class transaction;

   public:
    
    virtual ~save_point();

    
    void commit();

   private:
    
    save_point(transaction* trans);

   private:
    
    transaction* m_transaction;
    
    std::string m_name;
};

typedef std::shared_ptr<save_point> save_point_ref;
}

#endif
