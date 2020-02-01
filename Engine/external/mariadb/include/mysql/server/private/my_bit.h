

#ifndef MY_BIT_INCLUDED
#define MY_BIT_INCLUDED



C_MODE_START

extern const uchar _my_bits_reverse_table[256];



static inline uint my_bit_log2(ulong value)
{
  uint bit;
  for (bit=0 ; value > 1 ; value>>=1, bit++) ;
  return bit;
}



static inline uint my_count_bits_uint32(uint32 v)
{
  v = v - ((v >> 1) & 0x55555555);
  v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
  return (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;
}


static inline uint my_count_bits(ulonglong x)
{
  return my_count_bits_uint32((uint32)x) + my_count_bits_uint32((uint32)(x >> 32));
}






static inline uint32 my_round_up_to_next_power(uint32 v)
{
  v--;			
  v|= v >> 1;		
  v|= v >> 2;		
  v|= v >> 4;		
  v|= v >> 8;		
  v|= v >> 16;		
  return v+1;		
}

static inline uint32 my_clear_highest_bit(uint32 v)
{
  uint32 w=v >> 1;
  w|= w >> 1;
  w|= w >> 2;
  w|= w >> 4;
  w|= w >> 8;
  w|= w >> 16;
  return v & w;
}

static inline uint32 my_reverse_bits(uint32 key)
{
  return
    ((uint32)_my_bits_reverse_table[ key      & 255] << 24) |
    ((uint32)_my_bits_reverse_table[(key>> 8) & 255] << 16) |
    ((uint32)_my_bits_reverse_table[(key>>16) & 255] <<  8) |
     (uint32)_my_bits_reverse_table[(key>>24)      ];
}


static inline uint64 my_set_bits(int n)
{
  return (((1ULL << (n - 1)) - 1) << 1) | 1;
}

C_MODE_END

#endif 
