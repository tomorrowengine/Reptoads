



#ifndef MYSQL_ASYNC_H
#define MYSQL_ASYNC_H

extern int my_connect_async(struct mysql_async_context *b, my_socket fd,
                            const struct sockaddr *name, uint namelen,
                            int vio_timeout);
extern ssize_t my_recv_async(struct mysql_async_context *b, my_socket fd,
                             unsigned char *buf, size_t size, int timeout);
extern ssize_t my_send_async(struct mysql_async_context *b, my_socket fd,
                             const unsigned char *buf, size_t size,
                             int timeout);
extern my_bool my_io_wait_async(struct mysql_async_context *b,
                                enum enum_vio_io_event event, int timeout);
#ifdef HAVE_OPENSSL
extern int my_ssl_read_async(struct mysql_async_context *b, SSL *ssl,
                             void *buf, int size);
extern int my_ssl_write_async(struct mysql_async_context *b, SSL *ssl,
                              const void *buf, int size);
#endif

#endif  
