

#ifndef HEADER_SYMHACKS_H
# define HEADER_SYMHACKS_H

# include <openssl/e_os2.h>


# if defined(OPENSSL_SYS_VMS)
#  undef ERR_load_CRYPTO_strings
#  define ERR_load_CRYPTO_strings                 ERR_load_CRYPTOlib_strings
#  undef OCSP_crlID_new
#  define OCSP_crlID_new                          OCSP_crlID2_new

#  undef d2i_ECPARAMETERS
#  define d2i_ECPARAMETERS                        d2i_UC_ECPARAMETERS
#  undef i2d_ECPARAMETERS
#  define i2d_ECPARAMETERS                        i2d_UC_ECPARAMETERS
#  undef d2i_ECPKPARAMETERS
#  define d2i_ECPKPARAMETERS                      d2i_UC_ECPKPARAMETERS
#  undef i2d_ECPKPARAMETERS
#  define i2d_ECPKPARAMETERS                      i2d_UC_ECPKPARAMETERS


#  undef cms_Data_create
#  define cms_Data_create                         priv_cms_Data_create

# endif

#endif                          
