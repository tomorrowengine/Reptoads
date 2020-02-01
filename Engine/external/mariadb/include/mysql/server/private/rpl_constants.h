

#ifndef RPL_CONSTANTS_H
#define RPL_CONSTANTS_H

#include <my_sys.h>
#include <my_crypt.h>


enum Incident {
  
  INCIDENT_NONE = 0,

  
  INCIDENT_LOST_EVENTS = 1,

  
  INCIDENT_COUNT
};



enum ExtraRowInfoFormat {

  
  ERIF_LASTRESERVED =  63,

  
  ERIF_OPEN1        =  64,
  ERIF_OPEN2        =  65,

  ERIF_LASTOPEN     =  254,

  
  ERIF_MULTI        =  255
};


#define EXTRA_ROW_INFO_LEN_OFFSET 0
#define EXTRA_ROW_INFO_FORMAT_OFFSET 1
#define EXTRA_ROW_INFO_HDR_BYTES 2
#define EXTRA_ROW_INFO_MAX_PAYLOAD (255 - EXTRA_ROW_INFO_HDR_BYTES)

enum enum_binlog_checksum_alg {
  BINLOG_CHECKSUM_ALG_OFF= 0,    
                                 
  BINLOG_CHECKSUM_ALG_CRC32= 1,  
  BINLOG_CHECKSUM_ALG_ENUM_END,  
  BINLOG_CHECKSUM_ALG_UNDEF= 255 
                                 
};

#define BINLOG_CRYPTO_SCHEME_LENGTH 1
#define BINLOG_KEY_VERSION_LENGTH   4
#define BINLOG_IV_LENGTH            MY_AES_BLOCK_SIZE
#define BINLOG_IV_OFFS_LENGTH       4
#define BINLOG_NONCE_LENGTH         (BINLOG_IV_LENGTH - BINLOG_IV_OFFS_LENGTH)

struct Binlog_crypt_data {
  uint  scheme;
  uint  key_version, key_length, ctx_size;
  uchar key[MY_AES_MAX_KEY_LENGTH];
  uchar nonce[BINLOG_NONCE_LENGTH];

  int init(uint sch, uint kv)
  {
    scheme= sch;
    ctx_size= encryption_ctx_size(ENCRYPTION_KEY_SYSTEM_DATA, kv);
    key_version= kv;
    key_length= sizeof(key);
    return encryption_key_get(ENCRYPTION_KEY_SYSTEM_DATA, kv, key, &key_length);
  }

  void set_iv(uchar* iv, uint32 offs) const
  {
    memcpy(iv, nonce, BINLOG_NONCE_LENGTH);
    int4store(iv + BINLOG_NONCE_LENGTH, offs);
  }
};

#endif 
