









#ifndef _MARIADB_CONCURRENCY_HPP_
#define _MARIADB_CONCURRENCY_HPP_

#include <vector>
#include <mariadb++/account.hpp>
#include <mariadb++/statement.hpp>

namespace mariadb {
namespace concurrency {
    namespace status {
        enum type { waiting, executing, succeed, failed, removed };
    }

    
    
    
    extern void set_account(account_ref& account);

    
    
    
    extern status::type worker_status(handle h);

    
    
    
    
    
    extern handle execute(const std::string& query, bool keep_handle);
    inline void execute(const std::string& squery) { execute(squery, false); }

    extern handle insert(const std::string& query, bool keep_handle);
    inline void insert(const std::string& squery) { insert(squery, false); }

    extern handle query(const std::string& query, bool keep_handle);
    inline void query(const std::string& squery) { query(squery, false); }

    
    
    
    
    
    extern statement_ref create_statement(const std::string& query);
    extern handle execute(statement_ref& statement, bool keep_handle);
    inline void execute(statement_ref& statement) { execute(statement, false); }

    extern handle insert(statement_ref& statement, bool keep_handle);
    inline void insert(statement_ref& statement) { insert(statement, false); }

    extern handle query(statement_ref& statement, bool keep_handle);
    inline void query(statement_ref& statement) { query(statement, false); }

    
    
    
    extern u64 get_execute_result(handle h);
    extern u64 get_insert_result(handle h);
    extern result_set_ref get_query_result(handle h);

    
    
    
    
    
    extern void release_handle(handle h);

    
    
    
    extern bool wait_handle(handle h, u64 wait_time_ms = 100);
}
}

#endif
