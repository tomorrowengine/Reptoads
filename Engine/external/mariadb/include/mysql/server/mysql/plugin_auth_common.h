#ifndef MYSQL_PLUGIN_AUTH_COMMON_INCLUDED


#ifdef _WIN32
#include <windows.h>
#endif


#define MYSQL_PLUGIN_AUTH_COMMON_INCLUDED


#define MYSQL_USERNAME_LENGTH 512



 
#define CR_AUTH_PLUGIN_ERROR 3

#define CR_AUTH_HANDSHAKE 2

#define CR_AUTH_USER_CREDENTIALS 1

#define CR_ERROR 0

#define CR_OK -1

#define CR_OK_HANDSHAKE_COMPLETE -2

typedef struct st_plugin_vio_info
{
  enum { MYSQL_VIO_INVALID, MYSQL_VIO_TCP, MYSQL_VIO_SOCKET,
         MYSQL_VIO_PIPE, MYSQL_VIO_MEMORY } protocol;
  int socket;     
#ifdef _WIN32
  HANDLE handle;  
#endif
} MYSQL_PLUGIN_VIO_INFO;


typedef struct st_plugin_vio
{
  
  int (*read_packet)(struct st_plugin_vio *vio, 
                     unsigned char **buf);
  
  
  int (*write_packet)(struct st_plugin_vio *vio, 
                      const unsigned char *packet, 
                      int packet_len);

  
  void (*info)(struct st_plugin_vio *vio, struct st_plugin_vio_info *info);

} MYSQL_PLUGIN_VIO;

#endif

