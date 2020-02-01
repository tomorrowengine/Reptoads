







#include "TruncationTest.h"

TEST_F(TruncationTest, testUInt) {
    uint32_t max = std::numeric_limits<uint32_t>::max();

    
    m_con->execute("INSERT INTO " + m_table_name + " VALUES (" + std::to_string(max) + ");");

    
    result_set_ref res = m_con->query("SELECT * FROM " + m_table_name + ";");

    ASSERT_TRUE(!!res);
    ASSERT_TRUE(res->next());
    ASSERT_EQ(max, res->get_unsigned32(0));

    
    statement_ref stmt = m_con->create_statement("SELECT * FROM " + m_table_name + ";");
    result_set_ref stmt_res = stmt->query();

    ASSERT_TRUE(!!stmt_res);
    ASSERT_TRUE(stmt_res->next());
    ASSERT_EQ(max, stmt_res->get_unsigned32(0));
}
