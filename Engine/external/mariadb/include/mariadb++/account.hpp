








#ifndef _MARIADB_ACCOUNT_HPP_
#define _MARIADB_ACCOUNT_HPP_

#include <string>
#include <map>
#include <mariadb++/types.hpp>
#include <mysql.h>

namespace mariadb {
class account;
typedef std::shared_ptr<account> account_ref;

class option_arg {
   public:
    virtual ~option_arg() = default;
    virtual const void *value() = 0;
};

#define MAKE_OPTION_ARG(name, type, return_value)                   \
class option_arg_##name : public option_arg {                       \
   public:                                                          \
    explicit option_arg_##name(const type &arg) : m_value(arg) { }  \
    const void *value() override { return return_value; }           \
   protected:                                                       \
    type m_value;                                                   \
}

MAKE_OPTION_ARG(bool, bool, &m_value);
MAKE_OPTION_ARG(int, int, &m_value);
MAKE_OPTION_ARG(string, std::string, m_value.c_str());


class account {
   public:
    typedef std::map<std::string, std::string> map_options_t;
    typedef std::map<mysql_option, std::unique_ptr<option_arg>> map_connect_options_t;

   public:
    
    virtual ~account() {}

    
    const std::string &host_name() const;

    
    const std::string &user_name() const;

    
    const std::string &password() const;

    
    const std::string &unix_socket() const;

    
    const std::string &ssl_key() const;

    
    const std::string &ssl_certificate() const;

    
    const std::string &ssl_ca() const;

    
    const std::string &ssl_ca_path() const;

    
    const std::string &ssl_cipher() const;

    
    u32 port() const;

    
    const std::string &schema() const;

    
    void set_schema(const std::string &schema);

    
    void set_ssl(const std::string &key, const std::string &certificate, const std::string &ca,
                 const std::string &ca_path, const std::string &cipher);

    
    bool auto_commit() const;

    
    void set_auto_commit(bool auto_commit);

    
    const std::string option(const std::string &name) const;

    
    const map_options_t &options() const;

    
    void set_option(const std::string &name, const std::string &value);

    
    void clear_options();

    
    const map_connect_options_t &connect_options() const;

    
    void set_connect_option(mysql_option option, bool arg);
    
    void set_connect_option(mysql_option option, int arg);
    
    void set_connect_option(mysql_option option, const std::string &arg);

    
    void clear_connect_options();

    
    static account_ref create(const std::string &host_name, const std::string &user_name,
                              const std::string &password, const std::string &schema = "",
                              u32 port = 3306, const std::string &unix_socket = "");

   private:
    
    account(const std::string &host_name, const std::string &user_name, const std::string &password,
            const std::string &schema, u32 port, const std::string &unix_sock);

    bool m_auto_commit = true;
    u32 m_port;
    std::string m_host_name;
    std::string m_user_name;
    std::string m_password;
    std::string m_schema;
    std::string m_unix_socket;
    std::string m_ssl_key;
    std::string m_ssl_certificate;
    std::string m_ssl_ca;
    std::string m_ssl_ca_path;
    std::string m_ssl_cipher;
    map_options_t m_options;
    map_connect_options_t m_connect_options;
};
}

#endif
