#ifndef MYSQL_PLUGIN_ENCRYPTION_INCLUDED




#define MYSQL_PLUGIN_ENCRYPTION_INCLUDED

#include <mysql/plugin.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MariaDB_ENCRYPTION_INTERFACE_VERSION 0x0300


struct st_mariadb_encryption
{
  int interface_version;                        

  

  
  unsigned int (*get_latest_key_version)(unsigned int key_id);

  
  unsigned int (*get_key)(unsigned int key_id, unsigned int version,
                          unsigned char *key, unsigned int *key_length);

  
  

  
  unsigned int (*crypt_ctx_size)(unsigned int key_id, unsigned int key_version);
  
  int (*crypt_ctx_init)(void *ctx, const unsigned char* key, unsigned int klen,
                        const unsigned char* iv, unsigned int ivlen,
                        int flags, unsigned int key_id,
                        unsigned int key_version);
  
  int (*crypt_ctx_update)(void *ctx, const unsigned char* src, unsigned int slen,
                          unsigned char* dst, unsigned int* dlen);
  
  int (*crypt_ctx_finish)(void *ctx, unsigned char* dst, unsigned int* dlen);
  
  unsigned int (*encrypted_length)(unsigned int slen, unsigned int key_id, unsigned int key_version);
};

#ifdef __cplusplus
}
#endif
#endif

