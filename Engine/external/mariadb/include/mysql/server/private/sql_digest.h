

#ifndef SQL_DIGEST_H
#define SQL_DIGEST_H

#include <string.h>
class String;
#include "my_md5.h"

#define MAX_DIGEST_STORAGE_SIZE (1024*1024)


struct sql_digest_storage
{
  bool m_full;
  uint m_byte_count;
  unsigned char m_md5[MD5_HASH_SIZE];
  
  uint m_charset_number;
  
  unsigned char *m_token_array;
  
  uint m_token_array_length;

  sql_digest_storage()
  {
    reset(NULL, 0);
  }

  inline void reset(unsigned char *token_array, size_t length)
  {
    m_token_array= token_array;
    m_token_array_length= (uint)length;
    reset();
  }

  inline void reset()
  {
    m_full= false;
    m_byte_count= 0;
    m_charset_number= 0;
    memset(m_md5, 0, MD5_HASH_SIZE);
  }

  inline bool is_empty()
  {
    return (m_byte_count == 0);
  }

  inline void copy(const sql_digest_storage *from)
  {
    
    uint byte_count_copy= m_token_array_length < from->m_byte_count ?
                          m_token_array_length : from->m_byte_count;

    if (byte_count_copy > 0)
    {
      m_full= from->m_full;
      m_byte_count= byte_count_copy;
      m_charset_number= from->m_charset_number;
      memcpy(m_token_array, from->m_token_array, m_byte_count);
      memcpy(m_md5, from->m_md5, MD5_HASH_SIZE);
    }
    else
    {
      m_full= false;
      m_byte_count= 0;
      m_charset_number= 0;
    }
  }
};
typedef struct sql_digest_storage sql_digest_storage;


void compute_digest_md5(const sql_digest_storage *digest_storage, unsigned char *md5);


void compute_digest_text(const sql_digest_storage *digest_storage,
                         String *digest_text);

#endif

