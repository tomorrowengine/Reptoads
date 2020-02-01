#ifndef _ma_tls_h_
#define _ma_tls_h_

enum enum_pvio_tls_type {
  SSL_TYPE_DEFAULT=0,
#ifdef _WIN32
  SSL_TYPE_SCHANNEL,
#endif
  SSL_TYPE_OPENSSL,
  SSL_TYPE_GNUTLS
};

#define PROTOCOL_SSLV3    0
#define PROTOCOL_TLS_1_0  1
#define PROTOCOL_TLS_1_1  2
#define PROTOCOL_TLS_1_2  3
#define PROTOCOL_TLS_1_3  4
#define PROTOCOL_UNKNOWN  5
#define PROTOCOL_MAX PROTOCOL_TLS_1_3

#define TLS_VERSION_LENGTH 64
extern char tls_library_version[TLS_VERSION_LENGTH];

typedef struct st_ma_pvio_tls {
  void *data;
  MARIADB_PVIO *pvio;
  void *ssl;
} MARIADB_TLS;




int ma_tls_start(char *errmsg, size_t errmsg_len);


void ma_tls_end(void);


void * ma_tls_init(MYSQL *mysql);


my_bool ma_tls_connect(MARIADB_TLS *ctls);


ssize_t ma_tls_read(MARIADB_TLS *ctls, const uchar* buffer, size_t length);


ssize_t ma_tls_write(MARIADB_TLS *ctls, const uchar* buffer, size_t length);


my_bool ma_tls_close(MARIADB_TLS *ctls);


int ma_tls_verify_server_cert(MARIADB_TLS *ctls);


const char *ma_tls_get_cipher(MARIADB_TLS *ssl);


unsigned int ma_tls_get_finger_print(MARIADB_TLS *ctls, char *fp, unsigned int fp_len);


int ma_tls_get_protocol_version(MARIADB_TLS *ctls);
const char *ma_pvio_tls_get_protocol_version(MARIADB_TLS *ctls);
int ma_pvio_tls_get_protocol_version_id(MARIADB_TLS *ctls);


MARIADB_TLS *ma_pvio_tls_init(MYSQL *mysql);
my_bool ma_pvio_tls_connect(MARIADB_TLS *ctls);
ssize_t ma_pvio_tls_read(MARIADB_TLS *ctls, const uchar *buffer, size_t length);
ssize_t ma_pvio_tls_write(MARIADB_TLS *ctls, const uchar *buffer, size_t length);
my_bool ma_pvio_tls_close(MARIADB_TLS *ctls);
int ma_pvio_tls_verify_server_cert(MARIADB_TLS *ctls);
const char *ma_pvio_tls_cipher(MARIADB_TLS *ctls);
my_bool ma_pvio_tls_check_fp(MARIADB_TLS *ctls, const char *fp, const char *fp_list);
my_bool ma_pvio_start_ssl(MARIADB_PVIO *pvio);
void ma_pvio_tls_end();

#endif 
