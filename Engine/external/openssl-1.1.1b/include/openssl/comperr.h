

#ifndef HEADER_COMPERR_H
# define HEADER_COMPERR_H

# include <openssl/opensslconf.h>

# ifndef OPENSSL_NO_COMP

#  ifdef  __cplusplus
extern "C"
#  endif
int ERR_load_COMP_strings(void);


#  define COMP_F_BIO_ZLIB_FLUSH                            99
#  define COMP_F_BIO_ZLIB_NEW                              100
#  define COMP_F_BIO_ZLIB_READ                             101
#  define COMP_F_BIO_ZLIB_WRITE                            102
#  define COMP_F_COMP_CTX_NEW                              103


#  define COMP_R_ZLIB_DEFLATE_ERROR                        99
#  define COMP_R_ZLIB_INFLATE_ERROR                        100
#  define COMP_R_ZLIB_NOT_SUPPORTED                        101

# endif
#endif
