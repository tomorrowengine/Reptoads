








#include <mariadb++/last_error.hpp>

using namespace mariadb;




last_error::last_error() : m_last_error_no(0) {}




u32 last_error::error_no() const { return m_last_error_no; }

const std::string& last_error::error() const { return m_last_error; }
