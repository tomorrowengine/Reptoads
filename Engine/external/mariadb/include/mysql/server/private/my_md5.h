#ifndef MY_MD5_INCLUDED
#define MY_MD5_INCLUDED



#include "m_string.h"

#define MD5_HASH_SIZE 16 


#ifdef __cplusplus
extern "C" {
#endif

#define compute_md5_hash(A,B,C) my_md5(A,B,C)


static inline void array_to_hex(char *to, const unsigned char *str, uint len)
{
  const unsigned char *str_end= str + len;
  for (; str != str_end; ++str)
  {
    *to++= _dig_vec_lower[((uchar) *str) >> 4];
    *to++= _dig_vec_lower[((uchar) *str) & 0x0F];
  }
}

#ifdef __cplusplus
}
#endif

#endif 
