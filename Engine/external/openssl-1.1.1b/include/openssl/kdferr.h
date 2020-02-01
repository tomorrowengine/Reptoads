

#ifndef HEADER_KDFERR_H
# define HEADER_KDFERR_H

# ifdef  __cplusplus
extern "C"
# endif
int ERR_load_KDF_strings(void);


# define KDF_F_PKEY_HKDF_CTRL_STR                         103
# define KDF_F_PKEY_HKDF_DERIVE                           102
# define KDF_F_PKEY_HKDF_INIT                             108
# define KDF_F_PKEY_SCRYPT_CTRL_STR                       104
# define KDF_F_PKEY_SCRYPT_CTRL_UINT64                    105
# define KDF_F_PKEY_SCRYPT_DERIVE                         109
# define KDF_F_PKEY_SCRYPT_INIT                           106
# define KDF_F_PKEY_SCRYPT_SET_MEMBUF                     107
# define KDF_F_PKEY_TLS1_PRF_CTRL_STR                     100
# define KDF_F_PKEY_TLS1_PRF_DERIVE                       101
# define KDF_F_PKEY_TLS1_PRF_INIT                         110
# define KDF_F_TLS1_PRF_ALG                               111


# define KDF_R_INVALID_DIGEST                             100
# define KDF_R_MISSING_ITERATION_COUNT                    109
# define KDF_R_MISSING_KEY                                104
# define KDF_R_MISSING_MESSAGE_DIGEST                     105
# define KDF_R_MISSING_PARAMETER                          101
# define KDF_R_MISSING_PASS                               110
# define KDF_R_MISSING_SALT                               111
# define KDF_R_MISSING_SECRET                             107
# define KDF_R_MISSING_SEED                               106
# define KDF_R_UNKNOWN_PARAMETER_TYPE                     103
# define KDF_R_VALUE_ERROR                                108
# define KDF_R_VALUE_MISSING                              102

#endif
