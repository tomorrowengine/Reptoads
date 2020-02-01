









#ifndef _MARIADB_RESULT_SET_HPP_
#define _MARIADB_RESULT_SET_HPP_

#include <map>
#include <vector>
#include <mariadb++/bind.hpp>
#include <mariadb++/data.hpp>
#include <mariadb++/date_time.hpp>
#include <mariadb++/decimal.hpp>
#include <mariadb++/last_error.hpp>

#define MAKE_GETTER_SIG_STR(nm, rtype, fq) rtype fq get_##nm(const std::string& name) const
#define MAKE_GETTER_SIG_INT(nm, rtype, fq) rtype fq get_##nm(u32 index) const

#define MAKE_GETTER_DECL(nm, rtype)   \
    MAKE_GETTER_SIG_STR(nm, rtype, ); \
    MAKE_GETTER_SIG_INT(nm, rtype, )

#define MAKE_GETTER(nm, rtype, vtype)                                                     \
    MAKE_GETTER_SIG_STR(nm, rtype, result_set::) { return get_##nm(column_index(name)); } \
    MAKE_GETTER_SIG_INT(nm, rtype, result_set::) {                                        \
        check_row_fetched();                                                              \
        check_type(index, vtype);                                                         \
                                                                                          \
        if (index >= m_field_count) throw std::out_of_range("Column index out of range");

namespace mariadb {
class connection;
class statement;


struct statement_data {
    explicit statement_data(MYSQL_STMT* stmt) : m_statement(stmt) {}

    ~statement_data();

    
    unsigned long m_bind_count = 0;
    
    MYSQL_STMT* m_statement;
    
    MYSQL_BIND* m_raw_binds = nullptr;
    
    std::vector<bind_ref> m_binds;
};

typedef std::shared_ptr<statement_data> statement_data_ref;


class result_set : public last_error {
    friend class connection;
    friend class statement;

    typedef std::map<std::string, u32> map_indexes_t;

   public:
    
    virtual ~result_set();

    
    u64 column_count() const;

    
    u32 column_index(const std::string& name) const;

    
    value::type column_type(u32 index) const;

    
    const std::string column_name(u32 index);

    
    unsigned long column_size(u32 index) const;

    
    u64 row_index() const;

    
    u64 row_count() const;

    
    bool next();

    
    bool set_row_index(u64 index);

    
    MAKE_GETTER_DECL(blob, stream_ref);
    MAKE_GETTER_DECL(data, data_ref);
    MAKE_GETTER_DECL(date, date_time);
    MAKE_GETTER_DECL(date_time, date_time);
    MAKE_GETTER_DECL(time, time);
    MAKE_GETTER_DECL(decimal, decimal);
    MAKE_GETTER_DECL(string, std::string);
    MAKE_GETTER_DECL(boolean, bool);
    MAKE_GETTER_DECL(unsigned8, u8);
    MAKE_GETTER_DECL(signed8, s8);
    MAKE_GETTER_DECL(unsigned16, u16);
    MAKE_GETTER_DECL(signed16, s16);
    MAKE_GETTER_DECL(unsigned32, u32);
    MAKE_GETTER_DECL(signed32, s32);
    MAKE_GETTER_DECL(unsigned64, u64);
    MAKE_GETTER_DECL(signed64, s64);
    MAKE_GETTER_DECL(float, f32);
    MAKE_GETTER_DECL(double, f64);
    MAKE_GETTER_DECL(is_null, bool);

   private:
    
    explicit result_set(connection* conn);

    
    explicit result_set(const statement_data_ref& stmt);

    
    void check_row_fetched() const;

    
    void check_type(u32 index, value::type requested) const;

    
    MYSQL_RES* m_result_set;
    
    MYSQL_FIELD* m_fields;
    
    MYSQL_ROW m_row;
    
    MYSQL_BIND* m_raw_binds;

    
    std::vector<bind_ref> m_binds;
    
    statement_data_ref m_stmt_data;
    
    map_indexes_t m_indexes;
    
    long unsigned int* m_lengths;

    
    u32 m_field_count;
    
    bool m_was_fetched;
};

typedef std::shared_ptr<result_set> result_set_ref;
}

#endif
