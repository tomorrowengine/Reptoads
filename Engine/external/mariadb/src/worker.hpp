









#ifndef _MARIADB_WORKER_HPP_
#define _MARIADB_WORKER_HPP_

#include <mariadb++/connection.hpp>
#include <mariadb++/concurrency.hpp>

namespace mariadb {
using namespace concurrency;

namespace command {
enum type { execute, insert, query };
}




class worker {
   public:
    
    
    
    worker();
    worker(account_ref& account, handle hnd, bool keep_handle, command::type command,
           const std::string& query);
    worker(account_ref& account, handle hnd, bool keep_handle, command::type command,
           statement_ref& statement);

    
    
    
    status::type status() const;
    handle get_handle() const;
    bool keep_handle() const;

    
    
    
    u64 result() const;
    result_set_ref result_set() const;

    
    
    
    void execute();

   private:
    bool m_keep_handle;
    handle m_handle;
    status::type m_status;
    command::type m_command;
    u64 m_result;
    std::string m_query;
    account_ref m_account;
    result_set_ref m_result_set;
    statement_ref m_statement;
};
}

#endif
