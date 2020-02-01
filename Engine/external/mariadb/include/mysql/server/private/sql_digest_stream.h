

#ifndef SQL_DIGEST_STREAM_H
#define SQL_DIGEST_STREAM_H

#include "sql_digest.h"


struct sql_digest_state
{
  
  int m_last_id_index;
  sql_digest_storage m_digest_storage;

  inline void reset(unsigned char *token_array, uint length)
  {
    m_last_id_index= 0;
    m_digest_storage.reset(token_array, length);
  }

  inline bool is_empty()
  {
    return m_digest_storage.is_empty();
  }
};
typedef struct sql_digest_state sql_digest_state;

#endif

