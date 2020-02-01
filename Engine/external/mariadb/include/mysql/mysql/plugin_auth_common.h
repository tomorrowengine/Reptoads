


#ifndef MYSQL_PLUGIN_AUTH_COMMON_INCLUDED

#define MYSQL_PLUGIN_AUTH_COMMON_INCLUDED


#define MYSQL_USERNAME_LENGTH 512




#define CR_ERROR 0

#define CR_OK -1

#define CR_OK_HANDSHAKE_COMPLETE -2

typedef struct st_plugin_vio_info
{
  enum { MYSQL_VIO_INVALID, MYSQL_VIO_TCP, MYSQL_VIO_SOCKET,
         MYSQL_VIO_PIPE, MYSQL_VIO_MEMORY } protocol;
#ifndef _WIN32
  int socket;     
#else
  SOCKET socket;     
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

