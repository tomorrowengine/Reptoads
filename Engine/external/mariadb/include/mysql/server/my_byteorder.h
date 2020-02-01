#ifndef MY_BYTEORDER_INCLUDED
#define MY_BYTEORDER_INCLUDED





#define int4net(A)        (int32) (((uint32) ((uchar) (A)[3]))        | \
                                  (((uint32) ((uchar) (A)[2])) << 8)  | \
                                  (((uint32) ((uchar) (A)[1])) << 16) | \
                                  (((uint32) ((uchar) (A)[0])) << 24))


#if defined(__i386__) || defined(_WIN32)
#define MY_BYTE_ORDER_ARCH_OPTIMIZED
#include "byte_order_generic_x86.h"
#elif defined(__x86_64__)
#include "byte_order_generic_x86_64.h"
#else
#include "byte_order_generic.h"
#endif


#ifdef WORDS_BIGENDIAN
#include "big_endian.h"
#else
#include "little_endian.h"
#endif

#endif 
