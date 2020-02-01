







#include "SelectTest.h"
#include <cmath>

TEST_F(SelectTest, SelectEmptyTable) {
    m_con->execute("CREATE TABLE " + m_table_name +
                   " (id INT AUTO_INCREMENT, PRIMARY KEY (`id`));");
    result_set_ref res = m_con->query("SELECT * FROM " + m_table_name);

    ASSERT_TRUE(!!res);
    ASSERT_FALSE(res->next());
}

TEST_F(SelectTest, IntegerLimits) {
    m_con->execute("CREATE TABLE `" + m_table_name +
                   "` (\n"
                   "\t`seq` INT(11) NULL DEFAULT NULL,\n"
                   "\t`signed_tiny` TINYINT(4) NULL DEFAULT NULL,\n"
                   "\t`signed_small` SMALLINT(6) NULL DEFAULT NULL,\n"
                   "\t`signed_medium` MEDIUMINT(9) NULL DEFAULT NULL,\n"
                   "\t`signed_int` INT(11) NULL DEFAULT NULL,\n"
                   "\t`signed_big` BIGINT(20) NULL DEFAULT NULL,\n"
                   "\t`unsigned_tiny` TINYINT(3) UNSIGNED NULL DEFAULT NULL,\n"
                   "\t`unsigned_small` SMALLINT(5) UNSIGNED NULL DEFAULT NULL,\n"
                   "\t`unsigned_medium` MEDIUMINT(8) UNSIGNED NULL DEFAULT NULL,\n"
                   "\t`unsigned_int` INT(10) UNSIGNED NULL DEFAULT NULL,\n"
                   "\t`unsigned_big` BIGINT(20) UNSIGNED NULL DEFAULT NULL\n"
                   ");");
    
    m_con->query(
        "INSERT INTO " + m_table_name +
        " VALUES (0, -128, -32768, -8388608, -2147483648, -9223372036854775808, 0, 0, 0, 0, 0);");
    
    m_con->query("INSERT INTO " + m_table_name +
                 " VALUES (1, 127, 32767, 8388607, 2147483647, 9223372036854775807, 255, 65535, "
                 "16777215, 4294967295, 18446744073709551615);");

    
    result_set_ref res = m_con->query("SELECT * FROM " + m_table_name + " ORDER BY seq ASC;");
    ASSERT_TRUE(!!res);
    ASSERT_TRUE(res->next());

    
    EXPECT_EQ(-128, res->get_signed8(1));
    EXPECT_EQ(-32768, res->get_signed16(2));
    EXPECT_EQ(-8388608, res->get_signed32(3));
    EXPECT_EQ(-2147483648, res->get_signed32(4));
    EXPECT_EQ(-9223372036854775807 - 1,
              res->get_signed64(5));  
                                      
    EXPECT_EQ(0, res->get_unsigned8(6));
    EXPECT_EQ(0, res->get_unsigned16(7));
    EXPECT_EQ(0, res->get_unsigned32(8));
    EXPECT_EQ(0, res->get_unsigned32(9));
    EXPECT_EQ(0, res->get_unsigned64(10));

    
    ASSERT_TRUE(res->next());

    
    EXPECT_EQ(127, res->get_signed8(1));
    EXPECT_EQ(32767, res->get_signed16(2));
    EXPECT_EQ(8388607, res->get_signed32(3));
    EXPECT_EQ(2147483647, res->get_signed32(4));
    EXPECT_EQ(9223372036854775807, res->get_signed64(5));
    EXPECT_EQ(255, res->get_unsigned8(6));
    EXPECT_EQ(65535, res->get_unsigned16(7));
    EXPECT_EQ(16777215, res->get_unsigned32(8));
    EXPECT_EQ(4294967295, res->get_unsigned32(9));
    EXPECT_EQ(18446744073709551615ULL, res->get_unsigned64(10));
}

TEST_F(SelectTest, RealLimits) {
    m_con->execute("CREATE TABLE `" + m_table_name +
                   "` (\n"
                   "\t`seq` INT(11) NULL DEFAULT NULL,\n"
                   "\t`negative_float` FLOAT NULL DEFAULT NULL,\n"
                   "\t`negative_double` DOUBLE NULL DEFAULT NULL,\n"
                   "\t`positive_float` FLOAT UNSIGNED NULL DEFAULT NULL,\n"
                   "\t`positive_double` DOUBLE UNSIGNED NULL DEFAULT NULL\n"
                   ");");

    m_con->query("INSERT INTO " + m_table_name +
                 " VALUES (0, -3.402823466e+38, -1.7976931348623157e+308, 1.175494351e-38, "
                 "2.2250738585072014e-308);");
    m_con->query("INSERT INTO " + m_table_name +
                 " VALUES (1, -1.175494351e-38, -2.2250738585072014e-308, 3.402823466e+38, "
                 "1.7976931348623157e+308);");
    m_con->query("INSERT INTO " + m_table_name + " VALUES (2, 0, 0, 0, 0);");

    
    result_set_ref res = m_con->query("SELECT * FROM " + m_table_name + " ORDER BY seq ASC;");
    ASSERT_TRUE(!!res);
    ASSERT_TRUE(res->next());

    
    EXPECT_FLOAT_EQ(-3.40282e+38,
                    res->get_float(1));  
    EXPECT_FLOAT_EQ(-1.7976931348623157e+308, res->get_double(2));
    EXPECT_TRUE(std::isnan(res->get_float(3)));
    EXPECT_FLOAT_EQ(2.2250738585072014e-308, res->get_double(4));

    
    ASSERT_TRUE(res->next());
    EXPECT_TRUE(std::isnan(res->get_float(1)));
    EXPECT_FLOAT_EQ(-2.2250738585072014e-308, res->get_double(2));
    EXPECT_FLOAT_EQ(3.40282e+38,
                    res->get_float(3));  
    EXPECT_FLOAT_EQ(1.7976931348623157e+308, res->get_double(4));

    
    ASSERT_TRUE(res->next());
    EXPECT_FLOAT_EQ(0, res->get_float(1));
    EXPECT_FLOAT_EQ(0, res->get_double(2));
    EXPECT_FLOAT_EQ(0, res->get_float(3));
    EXPECT_FLOAT_EQ(0, res->get_double(4));
}
