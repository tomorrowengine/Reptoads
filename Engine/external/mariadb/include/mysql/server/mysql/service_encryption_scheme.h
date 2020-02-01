#ifndef MYSQL_SERVICE_ENCRYPTION_SCHEME_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

#define ENCRYPTION_SCHEME_KEY_INVALID    -1
#define ENCRYPTION_SCHEME_BLOCK_LENGTH   16

struct st_encryption_scheme_key {
  unsigned int version;
  unsigned char key[ENCRYPTION_SCHEME_BLOCK_LENGTH];
};

struct st_encryption_scheme {
  unsigned char iv[ENCRYPTION_SCHEME_BLOCK_LENGTH];
  struct st_encryption_scheme_key key[3];
  unsigned int keyserver_requests;
  unsigned int key_id;
  unsigned int type;

  void (*locker)(struct st_encryption_scheme *self, int release);
};

extern struct encryption_scheme_service_st {
  int (*encryption_scheme_encrypt_func)
                               (const unsigned char* src, unsigned int slen,
                                unsigned char* dst, unsigned int* dlen,
                                struct st_encryption_scheme *scheme,
                                unsigned int key_version, unsigned int i32_1,
                                unsigned int i32_2, unsigned long long i64);
  int (*encryption_scheme_decrypt_func)
                               (const unsigned char* src, unsigned int slen,
                                unsigned char* dst, unsigned int* dlen,
                                struct st_encryption_scheme *scheme,
                                unsigned int key_version, unsigned int i32_1,
                                unsigned int i32_2, unsigned long long i64);
} *encryption_scheme_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define encryption_scheme_encrypt(S,SL,D,DL,SCH,KV,I32,J32,I64) encryption_scheme_service->encryption_scheme_encrypt_func(S,SL,D,DL,SCH,KV,I32,J32,I64)
#define encryption_scheme_decrypt(S,SL,D,DL,SCH,KV,I32,J32,I64) encryption_scheme_service->encryption_scheme_decrypt_func(S,SL,D,DL,SCH,KV,I32,J32,I64)

#else

int encryption_scheme_encrypt(const unsigned char* src, unsigned int slen,
                              unsigned char* dst, unsigned int* dlen,
                              struct st_encryption_scheme *scheme,
                              unsigned int key_version, unsigned int i32_1,
                              unsigned int i32_2, unsigned long long i64);
int encryption_scheme_decrypt(const unsigned char* src, unsigned int slen,
                              unsigned char* dst, unsigned int* dlen,
                              struct st_encryption_scheme *scheme,
                              unsigned int key_version, unsigned int i32_1,
                              unsigned int i32_2, unsigned long long i64);

#endif


#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_ENCRYPTION_SCHEME_INCLUDED
#endif
