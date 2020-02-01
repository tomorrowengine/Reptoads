

#ifndef _my_compare_h
#define _my_compare_h

#include "myisampack.h"
#ifdef	__cplusplus
extern "C" {
#endif

#include "m_ctype.h"                            



#define HA_MAX_POSSIBLE_KEY         255         


#define HA_MAX_KEY_LENGTH           1000        
#define HA_MAX_KEY_SEG              32          

#define HA_MAX_POSSIBLE_KEY_BUFF    (HA_MAX_KEY_LENGTH + 24+ 6+6)
#define HA_MAX_KEY_BUFF  (HA_MAX_KEY_LENGTH+HA_MAX_KEY_SEG*6+8+8)

typedef struct st_HA_KEYSEG		
{
  CHARSET_INFO *charset;
  uint32 start;				
  uint32 null_pos;			
  uint16 bit_pos;                       
  uint16 flag;
  uint16 length;			
  uint16 language;
  uint8  type;				
  uint8  null_bit;			
  uint8  bit_start;
  uint8  bit_length;                    
} HA_KEYSEG;

#define get_key_length(length,key) \
{ if (*(const uchar*) (key) != 255) \
    length= (uint) *(const uchar*) ((key)++); \
  else \
  { length= mi_uint2korr((key)+1); (key)+=3; } \
}

#define get_key_length_rdonly(length,key) \
{ if (*(const uchar*) (key) != 255) \
    length= ((uint) *(const uchar*) ((key))); \
  else \
  { length= mi_uint2korr((key)+1); } \
}

#define get_key_pack_length(length,length_pack,key) \
{ if (*(const uchar*) (key) != 255) \
  { length= (uint) *(const uchar*) ((key)++); length_pack= 1; }\
  else \
  { length=mi_uint2korr((key)+1); (key)+= 3; length_pack= 3; } \
}

#define store_key_length_inc(key,length) \
{ if ((length) < 255) \
  { *(key)++= (uchar)(length); } \
  else \
  { *(key)=255; mi_int2store((key)+1,(length)); (key)+=3; } \
}

#define size_to_store_key_length(length) ((length) < 255 ? 1 : 3)

static inline uchar get_rec_bits(const uchar *ptr, uchar ofs, uint len)
{
  uint16 val= ptr[0];
  if (ofs + len > 8)
    val|= (uint16)(ptr[1]) << 8;
  return (val >> ofs) & ((1 << len) - 1);
}

static inline void set_rec_bits(uint16 bits, uchar *ptr, uchar ofs, uint len)
{
  ptr[0]= (ptr[0] & ~(((1 << len) - 1) << ofs)) | (bits << ofs);
  if (ofs + len > 8)
    ptr[1]= (ptr[1] & ~((1 << (len - 8 + ofs)) - 1)) | (bits >> (8 - ofs));
}

#define clr_rec_bits(bit_ptr, bit_ofs, bit_len) \
  set_rec_bits(0, bit_ptr, bit_ofs, bit_len)

extern int ha_compare_text(CHARSET_INFO *, const uchar *, size_t,
                           const uchar *, size_t , my_bool);
extern int ha_key_cmp(HA_KEYSEG *keyseg, const uchar *a,
		      const uchar *b, uint key_length, uint nextflag,
		      uint *diff_pos);
extern HA_KEYSEG *ha_find_null(HA_KEYSEG *keyseg, const uchar *a);


#define portable_sizeof_char_ptr 8
#ifdef	__cplusplus
}
#endif



typedef enum icp_result {
  ICP_ERROR=-1,
  ICP_NO_MATCH=0,
  ICP_MATCH=1,
  ICP_OUT_OF_RANGE=2,
  ICP_ABORTED_BY_USER=3
} ICP_RESULT;

typedef ICP_RESULT (*index_cond_func_t)(void *param);

#endif 
