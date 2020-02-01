







#include "GeneralTest.h"
#include "mariadb++/concurrency.hpp"

TEST_F(GeneralTest, testCreateFail) {
    
    ASSERT_ANY_THROW(m_con->execute("CREATE TAVBEL testtest ();"));
    ASSERT_ANY_THROW(m_con->execute("CREATE TABLE testtest (\");"));
    ASSERT_ANY_THROW(m_con->execute("CREATE TABLE testtest¸ ()"));
    ASSERT_ANY_THROW(m_con->execute("CREATE TABLE ()"));
}

TEST_F(GeneralTest, testMissingConnection) {
    
    account_ref no_acc = account::create("0.0.0.0", "", "");
    connection_ref no_conn = connection::create(no_acc);

    ASSERT_FALSE(no_conn->connected());
    EXPECT_ANY_THROW(no_conn->execute("CREATE TABLE asdf;"));
    EXPECT_ANY_THROW(no_conn->query("SELECT * FROM asdf;"));
    EXPECT_ANY_THROW(no_conn->insert("INSERT INTO asdf (a) VALUES ('a');"));

    EXPECT_ANY_THROW(no_conn->create_statement("SELECT * FROM asdf;"));
}

TEST_F(GeneralTest, testDuplicateTable) {
    EXPECT_ANY_THROW(m_con->execute("CREATE TABLE " + m_table_name +
                                    " (id INT AUTO_INCREMENT, PRIMARY KEY(id));"));
}

TEST_F(GeneralTest, testConcurrentInsert) {
    constexpr int num_results = 100;

    std::vector<handle> handles;
    std::set<u64> results;

    concurrency::set_account(m_account_setup);

    
    for (int i = 0; i < num_results; i++) {
        auto hndl = concurrency::insert("INSERT INTO " + m_table_name + "(str) VALUES('teest');", true);
        handles.push_back(hndl);
    }

    
    for (auto h : handles) EXPECT_TRUE(concurrency::wait_handle(h));

    
    for (auto h : handles) {
        u64 res = concurrency::get_execute_result(h);
        auto set_result = results.insert(res);

        
        EXPECT_TRUE(set_result.second);
    }

    
    for (auto h : handles) concurrency::release_handle(h);

    EXPECT_EQ(num_results, results.size());
}
