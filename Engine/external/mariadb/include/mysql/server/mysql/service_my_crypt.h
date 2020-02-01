#ifndef MYSQL_SERVICE_MY_CRYPT_INCLUDED
#define MYSQL_SERVICE_MY_CRYPT_INCLUDED






#ifdef __cplusplus
extern "C" {
#endif


#define MY_AES_OK               0
#define MY_AES_BAD_DATA         -100
#define MY_AES_OPENSSL_ERROR    -101
#define MY_AES_BAD_KEYSIZE      -102


#define MY_AES_BLOCK_SIZE 16


#define MY_AES_MAX_KEY_LENGTH 32

#define MY_AES_CTX_SIZE 512

enum my_aes_mode {
    MY_AES_ECB, MY_AES_CBC
#ifdef HAVE_EncryptAes128Ctr
  , MY_AES_CTR
#endif
#ifdef HAVE_EncryptAes128Gcm
  , MY_AES_GCM
#endif
};

extern struct my_crypt_service_st {
  int (*my_aes_crypt_init)(void *ctx, enum my_aes_mode mode, int flags,
                      const unsigned char* key, unsigned int klen,
                      const unsigned char* iv, unsigned int ivlen);
  int (*my_aes_crypt_update)(void *ctx, const unsigned char *src, unsigned int slen,
                        unsigned char *dst, unsigned int *dlen);
  int (*my_aes_crypt_finish)(void *ctx, unsigned char *dst, unsigned int *dlen);
  int (*my_aes_crypt)(enum my_aes_mode mode, int flags,
                 const unsigned char *src, unsigned int slen, unsigned char *dst, unsigned int *dlen,
                 const unsigned char *key, unsigned int klen, const unsigned char *iv, unsigned int ivlen);
  unsigned int (*my_aes_get_size)(enum my_aes_mode mode, unsigned int source_length);
  unsigned int (*my_aes_ctx_size)(enum my_aes_mode mode);
  int (*my_random_bytes)(unsigned char* buf, int num);
} *my_crypt_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define my_aes_crypt_init(A,B,C,D,E,F,G) \
   my_crypt_service->my_aes_crypt_init(A,B,C,D,E,F,G)

#define my_aes_crypt_update(A,B,C,D,E) \
   my_crypt_service->my_aes_crypt_update(A,B,C,D,E)

#define my_aes_crypt_finish(A,B,C) \
  my_crypt_service->my_aes_crypt_finish(A,B,C)

#define my_aes_crypt(A,B,C,D,E,F,G,H,I,J) \
  my_crypt_service->my_aes_crypt(A,B,C,D,E,F,G,H,I,J)

#define my_aes_get_size(A,B)\
  my_crypt_service->my_aes_get_size(A,B)

#define my_aes_ctx_size(A)\
  my_crypt_service->my_aes_ctx_size(A)

#define my_random_bytes(A,B)\
  my_crypt_service->my_random_bytes(A,B)

#else

int my_aes_crypt_init(void *ctx, enum my_aes_mode mode, int flags,
                      const unsigned char* key, unsigned int klen,
                      const unsigned char* iv, unsigned int ivlen);
int my_aes_crypt_update(void *ctx, const unsigned char *src, unsigned int slen,
                        unsigned char *dst, unsigned int *dlen);
int my_aes_crypt_finish(void *ctx, unsigned char *dst, unsigned int *dlen);
int my_aes_crypt(enum my_aes_mode mode, int flags,
                 const unsigned char *src, unsigned int slen, unsigned char *dst, unsigned int *dlen,
                 const unsigned char *key, unsigned int klen, const unsigned char *iv, unsigned int ivlen);

int my_random_bytes(unsigned char* buf, int num);
unsigned int my_aes_get_size(enum my_aes_mode mode, unsigned int source_length);
unsigned int my_aes_ctx_size(enum my_aes_mode mode);
#endif


#ifdef __cplusplus
}
#endif

#endif 
