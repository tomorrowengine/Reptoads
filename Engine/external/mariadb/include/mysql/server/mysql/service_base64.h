#ifndef MYSQL_SERVICE_BASE64_INCLUDED




#ifdef __cplusplus
extern "C" {
#endif

#ifndef MYSQL_ABI_CHECK
#include <stdlib.h>
#endif


#define MY_BASE64_DECODE_ALLOW_MULTIPLE_CHUNKS 1

extern struct base64_service_st {
  int (*base64_needed_encoded_length_ptr)(int length_of_data);
  int (*base64_encode_max_arg_length_ptr)(void);
  int (*base64_needed_decoded_length_ptr)(int length_of_encoded_data);
  int (*base64_decode_max_arg_length_ptr)();
  int (*base64_encode_ptr)(const void *src, size_t src_len, char *dst);
  int (*base64_decode_ptr)(const char *src, size_t src_len,
                           void *dst, const char **end_ptr, int flags);
} *base64_service;

#ifdef MYSQL_DYNAMIC_PLUGIN

#define my_base64_needed_encoded_length(A) base64_service->base64_needed_encoded_length_ptr(A)
#define my_base64_encode_max_arg_length() base64_service->base64_encode_max_arg_length_ptr()
#define my_base64_needed_decoded_length(A) base64_service->base64_needed_decoded_length_ptr(A)
#define my_base64_decode_max_arg_length() base64_service->base64_decode_max_arg_length_ptr()
#define my_base64_encode(A,B,C) base64_service->base64_encode_ptr(A,B,C)
#define my_base64_decode(A,B,C,D,E) base64_service->base64_decode_ptr(A,B,C,D,E)

#else


int my_base64_needed_encoded_length(int length_of_data);


int my_base64_encode_max_arg_length(void);


int my_base64_needed_decoded_length(int length_of_encoded_data);


int my_base64_decode_max_arg_length();


int my_base64_encode(const void *src, size_t src_len, char *dst);


int my_base64_decode(const char *src, size_t src_len,
                  void *dst, const char **end_ptr, int flags);

#endif

#ifdef __cplusplus
}
#endif

#define MYSQL_SERVICE_BASE64_INCLUDED
#endif
