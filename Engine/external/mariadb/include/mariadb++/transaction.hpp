








#ifndef _MARIADB_TRANSACTION_HPP_
#define _MARIADB_TRANSACTION_HPP_

#include <vector>
#include <algorithm>
#include <mariadb++/save_point.hpp>
#include <mariadb++/types.hpp>

namespace mariadb {
class connection;
class save_point;


class transaction {
    friend class connection;
    friend class save_point;

   public:
    
    virtual ~transaction();

    
    void commit();

    
    save_point_ref create_save_point();

   private:
    
    transaction(connection* conn, isolation::level level, bool consistent_snapshot);

    
    void remove_save_point(save_point* sv_point);

    
    void cleanup();

   private:
    
    connection* m_connection;
    
    std::vector<save_point*> m_save_points;
};

typedef std::shared_ptr<transaction> transaction_ref;
}

#endif
