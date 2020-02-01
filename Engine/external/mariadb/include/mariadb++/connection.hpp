









#ifndef _MARIADB_CONNECTION_HPP_
#define _MARIADB_CONNECTION_HPP_

#include <string>
#include <mariadb++/account.hpp>
#include <mariadb++/statement.hpp>
#include <mariadb++/transaction.hpp>
#include <mariadb++/save_point.hpp>
#include <mariadb++/result_set.hpp>

#include <mysql.h>

namespace mariadb {

class connection : public last_error {
    friend class result_set;
    friend class statement;
    friend class transaction;
    friend class save_point;

   public:
    
    virtual ~connection();

    
    bool connect();

    
    void disconnect();

    
    bool connected() const;

    
    account_ref account() const;

    
    const std::string& schema() const;

    
    bool set_schema(const std::string& schema);

    
    const std::string& charset() const;

    
    bool set_charset(const std::string& value);

    
    u64 execute(const std::string& query);

    
    u64 insert(const std::string& query);

    
    result_set_ref query(const std::string& query);

    
    bool auto_commit() const;

    
    bool set_auto_commit(bool auto_commit);

    
    statement_ref create_statement(const std::string& query);

    
    transaction_ref create_transaction(isolation::level level = isolation::repeatable_read,
                                       bool consistent_snapshot = true);

    
    static connection_ref create(const account_ref& account);

   private:
    
    connection(const account_ref& account);

   private:
    
    MYSQL* m_mysql;

    
    bool m_auto_commit;
    
    std::string m_schema;
    
    std::string m_charset;
    
    account_ref m_account;
};
}

#endif
