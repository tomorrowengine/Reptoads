

#ifndef HOSTNAME_INCLUDED
#define HOSTNAME_INCLUDED

#include "my_net.h"
#include "hash_filo.h"

struct Host_errors
{
public:
  Host_errors();
  ~Host_errors();

  void reset();
  void aggregate(const Host_errors *errors);

  
  ulong m_connect;

  
  ulong m_host_blocked;
  
  ulong m_nameinfo_transient;
  
  ulong m_nameinfo_permanent;
  
  ulong m_format;
  
  ulong m_addrinfo_transient;
  
  ulong m_addrinfo_permanent;
  
  ulong m_FCrDNS;
  
  ulong m_host_acl;
  
  ulong m_no_auth_plugin;
  
  ulong m_auth_plugin;
  
  ulong m_handshake;
  
  ulong m_proxy_user;
  
  ulong m_proxy_user_acl;
  
  ulong m_authentication;
  
  ulong m_ssl;
  
  ulong m_max_user_connection;
  
  ulong m_max_user_connection_per_hour;
  
  ulong m_default_database;
  
  ulong m_init_connect;
  
  ulong m_local;

  bool has_error() const
  {
    return ((m_host_blocked != 0)
      || (m_nameinfo_transient != 0)
      || (m_nameinfo_permanent != 0)
      || (m_format != 0)
      || (m_addrinfo_transient != 0)
      || (m_addrinfo_permanent != 0)
      || (m_FCrDNS != 0)
      || (m_host_acl != 0)
      || (m_no_auth_plugin != 0)
      || (m_auth_plugin != 0)
      || (m_handshake != 0)
      || (m_proxy_user != 0)
      || (m_proxy_user_acl != 0)
      || (m_authentication != 0)
      || (m_ssl != 0)
      || (m_max_user_connection != 0)
      || (m_max_user_connection_per_hour != 0)
      || (m_default_database != 0)
      || (m_init_connect != 0)
      || (m_local != 0));
  }

  void sum_connect_errors()
  {
    
    m_connect= m_handshake;
  }

  void clear_connect_errors()
  {
    m_connect= 0;
  }
};


#define HOST_ENTRY_KEY_SIZE INET6_ADDRSTRLEN


class Host_entry : public hash_filo_element
{
public:
  Host_entry *next()
  { return (Host_entry*) hash_filo_element::next(); }

  
  char ip_key[HOST_ENTRY_KEY_SIZE];

  
  char m_hostname[HOSTNAME_LENGTH + 1];
  
  uint m_hostname_length;
  
  bool m_host_validated;
  ulonglong m_first_seen;
  ulonglong m_last_seen;
  ulonglong m_first_error_seen;
  ulonglong m_last_error_seen;
  
  Host_errors m_errors;

  void set_error_timestamps(ulonglong now)
  {
    if (m_first_error_seen == 0)
      m_first_error_seen= now;
    m_last_error_seen= now;
  }
};


extern ulong host_cache_size;

#define RC_OK 0
#define RC_BLOCKED_HOST 1
int ip_to_hostname(struct sockaddr_storage *ip_storage,
                   const char *ip_string,
                   const char **hostname, uint *connect_errors);

void inc_host_errors(const char *ip_string, Host_errors *errors);
void reset_host_connect_errors(const char *ip_string);
bool hostname_cache_init();
void hostname_cache_free();
void hostname_cache_refresh(void);
uint hostname_cache_size();
void hostname_cache_resize(uint size);
void hostname_cache_lock();
void hostname_cache_unlock();
Host_entry *hostname_cache_first();

#endif 
