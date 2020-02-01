









#include <algorithm>
#include <mariadb++/connection.hpp>

using namespace mariadb;

namespace {
handle g_save_point_id = 0;

const char* g_save_point_create = "SAVEPOINT ";
const char* g_save_point_rollback = "ROLLBACK TO SAVEPOINT ";
const char* g_save_point_release = "RELEASE SAVEPOINT ";
}




save_point::save_point(transaction* trans) : m_transaction(trans) {
    m_name = "SP" + std::to_string(++g_save_point_id);
    m_transaction->m_connection->execute(g_save_point_create + m_name);
}




save_point::~save_point() {
    if (!m_transaction) return;

    m_transaction->remove_save_point(this);
    m_transaction->m_connection->execute(g_save_point_rollback + m_name);
}




void save_point::commit() {
    if (!m_transaction) return;

    m_transaction->remove_save_point(this);
    m_transaction->m_connection->execute(g_save_point_release + m_name);
    m_transaction = NULL;
}
