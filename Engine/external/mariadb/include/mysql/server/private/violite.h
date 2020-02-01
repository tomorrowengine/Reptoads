



#ifndef vio_violite_h_
#define	vio_violite_h_

#include "my_net.h"   
#include <mysql/psi/mysql_socket.h>



#ifdef	__cplusplus
extern "C" {
#endif 

#ifdef __cplusplus
typedef struct st_vio Vio;
#endif 

enum enum_vio_type
{
  VIO_CLOSED, VIO_TYPE_TCPIP, VIO_TYPE_SOCKET, VIO_TYPE_NAMEDPIPE,
  VIO_TYPE_SSL, VIO_TYPE_SHARED_MEMORY
};


enum enum_vio_io_event
{
  VIO_IO_EVENT_READ,
  VIO_IO_EVENT_WRITE,
  VIO_IO_EVENT_CONNECT
};

struct vio_keepalive_opts
{
  int interval;
  int idle;
  int probes;
};


#define VIO_LOCALHOST 1U                        
#define VIO_BUFFERED_READ 2U                    
#define VIO_READ_BUFFER_SIZE 16384U             
#define VIO_DESCRIPTION_SIZE 30                 

Vio* vio_new(my_socket sd, enum enum_vio_type type, uint flags);
Vio*  mysql_socket_vio_new(MYSQL_SOCKET mysql_socket, enum enum_vio_type type, uint flags);
#ifdef __WIN__
Vio* vio_new_win32pipe(HANDLE hPipe);
Vio* vio_new_win32shared_memory(HANDLE handle_file_map,
                                HANDLE handle_map,
                                HANDLE event_server_wrote,
                                HANDLE event_server_read,
                                HANDLE event_client_wrote,
                                HANDLE event_client_read,
                                HANDLE event_conn_closed);
#else
#define HANDLE void *
#endif 

void	vio_delete(Vio* vio);
int	vio_close(Vio* vio);
my_bool vio_reset(Vio* vio, enum enum_vio_type type,
                  my_socket sd, void *ssl, uint flags);
size_t	vio_read(Vio *vio, uchar *	buf, size_t size);
size_t  vio_read_buff(Vio *vio, uchar * buf, size_t size);
size_t	vio_write(Vio *vio, const uchar * buf, size_t size);
int	vio_blocking(Vio *vio, my_bool onoff, my_bool *old_mode);
my_bool	vio_is_blocking(Vio *vio);

int	vio_fastsend(Vio *vio);

int	vio_keepalive(Vio *vio, my_bool	onoff);
int	vio_set_keepalive_options(Vio * vio, const struct vio_keepalive_opts *opts);

my_bool	vio_should_retry(Vio *vio);

my_bool vio_was_timeout(Vio *vio);

const char* vio_description(Vio *vio);

enum enum_vio_type vio_type(Vio* vio);

int	vio_errno(Vio*vio);

my_socket vio_fd(Vio*vio);

my_bool vio_peer_addr(Vio *vio, char *buf, uint16 *port, size_t buflen);

int vio_io_wait(Vio *vio, enum enum_vio_io_event event, int timeout);
my_bool vio_is_connected(Vio *vio);
#ifndef DBUG_OFF
ssize_t vio_pending(Vio *vio);
#endif

extern int vio_timeout(Vio *vio, uint which, int timeout_sec);
extern void vio_set_wait_callback(void (*before_wait)(void),
                                void (*after_wait)(void));

my_bool vio_socket_connect(Vio *vio, struct sockaddr *addr, socklen_t len,
                           int timeout);

void vio_get_normalized_ip(const struct sockaddr *src, size_t src_length, struct sockaddr *dst);

my_bool vio_get_normalized_ip_string(const struct sockaddr *addr, size_t addr_length,
                                     char *ip_string, size_t ip_string_size);

my_bool vio_is_no_name_error(int err_code);

int vio_getnameinfo(const struct sockaddr *sa,
                    char *hostname, size_t hostname_size,
                    char *port, size_t port_size,
                    int flags);

#ifdef HAVE_OPENSSL

#ifdef __APPLE__
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#define HEADER_DES_LOCL_H dummy_something
#define YASSL_MYSQL_COMPATIBLE
#ifndef YASSL_PREFIX
#define YASSL_PREFIX
#endif

typedef my_socket YASSL_SOCKET_T;
#define YASSL_SOCKET_T_DEFINED
#include <openssl/ssl.h>
#include <openssl/err.h>

enum enum_ssl_init_error
{
  SSL_INITERR_NOERROR= 0, SSL_INITERR_CERT, SSL_INITERR_KEY,
  SSL_INITERR_NOMATCH, SSL_INITERR_BAD_PATHS, SSL_INITERR_CIPHERS,
  SSL_INITERR_MEMFAIL, SSL_INITERR_DH, SSL_INITERR_LASTERR
};
const char* sslGetErrString(enum enum_ssl_init_error err);

struct st_VioSSLFd
{
  SSL_CTX *ssl_context;
};

int sslaccept(struct st_VioSSLFd*, Vio *, long timeout, unsigned long *errptr);
int sslconnect(struct st_VioSSLFd*, Vio *, long timeout, unsigned long *errptr);

struct st_VioSSLFd
*new_VioSSLConnectorFd(const char *key_file, const char *cert_file,
		       const char *ca_file,  const char *ca_path,
		       const char *cipher, enum enum_ssl_init_error *error,
                       const char *crl_file, const char *crl_path);
struct st_VioSSLFd
*new_VioSSLAcceptorFd(const char *key_file, const char *cert_file,
		      const char *ca_file,const char *ca_path,
		      const char *cipher, enum enum_ssl_init_error *error,
                      const char *crl_file, const char *crl_path);
void free_vio_ssl_acceptor_fd(struct st_VioSSLFd *fd);
#endif 

void vio_end(void);

#ifdef	__cplusplus
}
#endif

#if !defined(DONT_MAP_VIO)
#define vio_delete(vio) 			(vio)->viodelete(vio)
#define vio_errno(vio)	 			(vio)->vioerrno(vio)
#define vio_read(vio, buf, size)                ((vio)->read)(vio,buf,size)
#define vio_write(vio, buf, size)               ((vio)->write)(vio, buf, size)
#define vio_blocking(vio, set_blocking_mode, old_mode)\
 	(vio)->vioblocking(vio, set_blocking_mode, old_mode)
#define vio_is_blocking(vio) 			(vio)->is_blocking(vio)
#define vio_fastsend(vio)			(vio)->fastsend(vio)
#define vio_keepalive(vio, set_keep_alive)	(vio)->viokeepalive(vio, set_keep_alive)
#define vio_should_retry(vio) 			(vio)->should_retry(vio)
#define vio_was_timeout(vio)                    (vio)->was_timeout(vio)
#define vio_close(vio)				((vio)->vioclose)(vio)
#define vio_shutdown(vio,how)			((vio)->shutdown)(vio,how)
#define vio_peer_addr(vio, buf, prt, buflen)	(vio)->peer_addr(vio, buf, prt, buflen)
#define vio_io_wait(vio, event, timeout)        (vio)->io_wait(vio, event, timeout)
#define vio_is_connected(vio)                   (vio)->is_connected(vio)
#endif 

#ifdef _WIN32


#ifndef SHUT_RD
#define SHUT_RD SD_RECEIVE
#endif

#endif


enum SSL_type
{
  SSL_TYPE_NOT_SPECIFIED= -1,
  SSL_TYPE_NONE,
  SSL_TYPE_ANY,
  SSL_TYPE_X509,
  SSL_TYPE_SPECIFIED
};



struct st_vio
{
  MYSQL_SOCKET  mysql_socket;     
  my_bool		localhost;	
  int			fcntl_mode;	
  struct sockaddr_storage local;	
  struct sockaddr_storage remote;	
  enum enum_vio_type	type;		
  const char		*desc;		
  char                  *read_buffer;   
  char                  *read_pos;      
  char                  *read_end;      
  struct mysql_async_context *async_context; 
  int                   read_timeout;   
  int                   write_timeout;  
  
  void    (*viodelete)(Vio*);
  int     (*vioerrno)(Vio*);
  size_t  (*read)(Vio*, uchar *, size_t);
  size_t  (*write)(Vio*, const uchar *, size_t);
  int     (*timeout)(Vio*, uint, my_bool);
  int     (*vioblocking)(Vio*, my_bool, my_bool *);
  my_bool (*is_blocking)(Vio*);
  int     (*viokeepalive)(Vio*, my_bool);
  int     (*fastsend)(Vio*);
  my_bool (*peer_addr)(Vio*, char *, uint16*, size_t);
  void    (*in_addr)(Vio*, struct sockaddr_storage*);
  my_bool (*should_retry)(Vio*);
  my_bool (*was_timeout)(Vio*);
  int     (*vioclose)(Vio*);
  my_bool (*is_connected)(Vio*);
  int (*shutdown)(Vio *, int);
  my_bool (*has_data) (Vio*);
  int (*io_wait)(Vio*, enum enum_vio_io_event, int);
  my_bool (*connect)(Vio*, struct sockaddr *, socklen_t, int);
#ifdef HAVE_OPENSSL
  void	  *ssl_arg;
#endif
#ifdef HAVE_SMEM
  HANDLE  handle_file_map;
  char    *handle_map;
  HANDLE  event_server_wrote;
  HANDLE  event_server_read;
  HANDLE  event_client_wrote;
  HANDLE  event_client_read;
  HANDLE  event_conn_closed;
  size_t  shared_memory_remain;
  char    *shared_memory_pos;
#endif 
#ifdef _WIN32
  HANDLE hPipe;
  OVERLAPPED overlapped;
  DWORD read_timeout_ms;
  DWORD write_timeout_ms;
#endif
};
#endif 
