


#ifndef CLIENT_SETTINGS_INCLUDED
#define CLIENT_SETTINGS_INCLUDED
#else
#error You have already included an client_settings.h and it should not be included twice
#endif 

#include <thr_alarm.h>
#include <sql_common.h>


#define CLIENT_CAPABILITIES (CLIENT_MYSQL | \
                             CLIENT_LONG_FLAG |     \
                             CLIENT_TRANSACTIONS |  \
                             CLIENT_PROTOCOL_41 |   \
                             CLIENT_SECURE_CONNECTION | \
                             CLIENT_PLUGIN_AUTH | \
                             CLIENT_PLUGIN_AUTH_LENENC_CLIENT_DATA | \
                             CLIENT_CONNECT_ATTRS)

#define read_user_name(A) A[0]= 0
#undef _CUSTOMCONFIG_

#define mysql_server_init(a,b,c) mysql_client_plugin_init()
#define mysql_server_end()       mysql_client_plugin_deinit()

#ifdef HAVE_REPLICATION
C_MODE_START
void slave_io_thread_detach_vio();
C_MODE_END
#else
#define slave_io_thread_detach_vio()
#endif

