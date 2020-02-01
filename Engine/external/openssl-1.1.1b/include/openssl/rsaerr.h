

#ifndef HEADER_RSAERR_H
# define HEADER_RSAERR_H

# ifdef  __cplusplus
extern "C"
# endif
int ERR_load_RSA_strings(void);


# define RSA_F_CHECK_PADDING_MD                           140
# define RSA_F_ENCODE_PKCS1                               146
# define RSA_F_INT_RSA_VERIFY                             145
# define RSA_F_OLD_RSA_PRIV_DECODE                        147
# define RSA_F_PKEY_PSS_INIT                              165
# define RSA_F_PKEY_RSA_CTRL                              143
# define RSA_F_PKEY_RSA_CTRL_STR                          144
# define RSA_F_PKEY_RSA_SIGN                              142
# define RSA_F_PKEY_RSA_VERIFY                            149
# define RSA_F_PKEY_RSA_VERIFYRECOVER                     141
# define RSA_F_RSA_ALGOR_TO_MD                            156
# define RSA_F_RSA_BUILTIN_KEYGEN                         129
# define RSA_F_RSA_CHECK_KEY                              123
# define RSA_F_RSA_CHECK_KEY_EX                           160
# define RSA_F_RSA_CMS_DECRYPT                            159
# define RSA_F_RSA_CMS_VERIFY                             158
# define RSA_F_RSA_ITEM_VERIFY                            148
# define RSA_F_RSA_METH_DUP                               161
# define RSA_F_RSA_METH_NEW                               162
# define RSA_F_RSA_METH_SET1_NAME                         163
# define RSA_F_RSA_MGF1_TO_MD                             157
# define RSA_F_RSA_MULTIP_INFO_NEW                        166
# define RSA_F_RSA_NEW_METHOD                             106
# define RSA_F_RSA_NULL                                   124
# define RSA_F_RSA_NULL_PRIVATE_DECRYPT                   132
# define RSA_F_RSA_NULL_PRIVATE_ENCRYPT                   133
# define RSA_F_RSA_NULL_PUBLIC_DECRYPT                    134
# define RSA_F_RSA_NULL_PUBLIC_ENCRYPT                    135
# define RSA_F_RSA_OSSL_PRIVATE_DECRYPT                   101
# define RSA_F_RSA_OSSL_PRIVATE_ENCRYPT                   102
# define RSA_F_RSA_OSSL_PUBLIC_DECRYPT                    103
# define RSA_F_RSA_OSSL_PUBLIC_ENCRYPT                    104
# define RSA_F_RSA_PADDING_ADD_NONE                       107
# define RSA_F_RSA_PADDING_ADD_PKCS1_OAEP                 121
# define RSA_F_RSA_PADDING_ADD_PKCS1_OAEP_MGF1            154
# define RSA_F_RSA_PADDING_ADD_PKCS1_PSS                  125
# define RSA_F_RSA_PADDING_ADD_PKCS1_PSS_MGF1             152
# define RSA_F_RSA_PADDING_ADD_PKCS1_TYPE_1               108
# define RSA_F_RSA_PADDING_ADD_PKCS1_TYPE_2               109
# define RSA_F_RSA_PADDING_ADD_SSLV23                     110
# define RSA_F_RSA_PADDING_ADD_X931                       127
# define RSA_F_RSA_PADDING_CHECK_NONE                     111
# define RSA_F_RSA_PADDING_CHECK_PKCS1_OAEP               122
# define RSA_F_RSA_PADDING_CHECK_PKCS1_OAEP_MGF1          153
# define RSA_F_RSA_PADDING_CHECK_PKCS1_TYPE_1             112
# define RSA_F_RSA_PADDING_CHECK_PKCS1_TYPE_2             113
# define RSA_F_RSA_PADDING_CHECK_SSLV23                   114
# define RSA_F_RSA_PADDING_CHECK_X931                     128
# define RSA_F_RSA_PARAM_DECODE                           164
# define RSA_F_RSA_PRINT                                  115
# define RSA_F_RSA_PRINT_FP                               116
# define RSA_F_RSA_PRIV_DECODE                            150
# define RSA_F_RSA_PRIV_ENCODE                            138
# define RSA_F_RSA_PSS_GET_PARAM                          151
# define RSA_F_RSA_PSS_TO_CTX                             155
# define RSA_F_RSA_PUB_DECODE                             139
# define RSA_F_RSA_SETUP_BLINDING                         136
# define RSA_F_RSA_SIGN                                   117
# define RSA_F_RSA_SIGN_ASN1_OCTET_STRING                 118
# define RSA_F_RSA_VERIFY                                 119
# define RSA_F_RSA_VERIFY_ASN1_OCTET_STRING               120
# define RSA_F_RSA_VERIFY_PKCS1_PSS_MGF1                  126
# define RSA_F_SETUP_TBUF                                 167


# define RSA_R_ALGORITHM_MISMATCH                         100
# define RSA_R_BAD_E_VALUE                                101
# define RSA_R_BAD_FIXED_HEADER_DECRYPT                   102
# define RSA_R_BAD_PAD_BYTE_COUNT                         103
# define RSA_R_BAD_SIGNATURE                              104
# define RSA_R_BLOCK_TYPE_IS_NOT_01                       106
# define RSA_R_BLOCK_TYPE_IS_NOT_02                       107
# define RSA_R_DATA_GREATER_THAN_MOD_LEN                  108
# define RSA_R_DATA_TOO_LARGE                             109
# define RSA_R_DATA_TOO_LARGE_FOR_KEY_SIZE                110
# define RSA_R_DATA_TOO_LARGE_FOR_MODULUS                 132
# define RSA_R_DATA_TOO_SMALL                             111
# define RSA_R_DATA_TOO_SMALL_FOR_KEY_SIZE                122
# define RSA_R_DIGEST_DOES_NOT_MATCH                      158
# define RSA_R_DIGEST_NOT_ALLOWED                         145
# define RSA_R_DIGEST_TOO_BIG_FOR_RSA_KEY                 112
# define RSA_R_DMP1_NOT_CONGRUENT_TO_D                    124
# define RSA_R_DMQ1_NOT_CONGRUENT_TO_D                    125
# define RSA_R_D_E_NOT_CONGRUENT_TO_1                     123
# define RSA_R_FIRST_OCTET_INVALID                        133
# define RSA_R_ILLEGAL_OR_UNSUPPORTED_PADDING_MODE        144
# define RSA_R_INVALID_DIGEST                             157
# define RSA_R_INVALID_DIGEST_LENGTH                      143
# define RSA_R_INVALID_HEADER                             137
# define RSA_R_INVALID_LABEL                              160
# define RSA_R_INVALID_MESSAGE_LENGTH                     131
# define RSA_R_INVALID_MGF1_MD                            156
# define RSA_R_INVALID_MULTI_PRIME_KEY                    167
# define RSA_R_INVALID_OAEP_PARAMETERS                    161
# define RSA_R_INVALID_PADDING                            138
# define RSA_R_INVALID_PADDING_MODE                       141
# define RSA_R_INVALID_PSS_PARAMETERS                     149
# define RSA_R_INVALID_PSS_SALTLEN                        146
# define RSA_R_INVALID_SALT_LENGTH                        150
# define RSA_R_INVALID_TRAILER                            139
# define RSA_R_INVALID_X931_DIGEST                        142
# define RSA_R_IQMP_NOT_INVERSE_OF_Q                      126
# define RSA_R_KEY_PRIME_NUM_INVALID                      165
# define RSA_R_KEY_SIZE_TOO_SMALL                         120
# define RSA_R_LAST_OCTET_INVALID                         134
# define RSA_R_MGF1_DIGEST_NOT_ALLOWED                    152
# define RSA_R_MODULUS_TOO_LARGE                          105
# define RSA_R_MP_COEFFICIENT_NOT_INVERSE_OF_R            168
# define RSA_R_MP_EXPONENT_NOT_CONGRUENT_TO_D             169
# define RSA_R_MP_R_NOT_PRIME                             170
# define RSA_R_NO_PUBLIC_EXPONENT                         140
# define RSA_R_NULL_BEFORE_BLOCK_MISSING                  113
# define RSA_R_N_DOES_NOT_EQUAL_PRODUCT_OF_PRIMES         172
# define RSA_R_N_DOES_NOT_EQUAL_P_Q                       127
# define RSA_R_OAEP_DECODING_ERROR                        121
# define RSA_R_OPERATION_NOT_SUPPORTED_FOR_THIS_KEYTYPE   148
# define RSA_R_PADDING_CHECK_FAILED                       114
# define RSA_R_PKCS_DECODING_ERROR                        159
# define RSA_R_PSS_SALTLEN_TOO_SMALL                      164
# define RSA_R_P_NOT_PRIME                                128
# define RSA_R_Q_NOT_PRIME                                129
# define RSA_R_RSA_OPERATIONS_NOT_SUPPORTED               130
# define RSA_R_SLEN_CHECK_FAILED                          136
# define RSA_R_SLEN_RECOVERY_FAILED                       135
# define RSA_R_SSLV3_ROLLBACK_ATTACK                      115
# define RSA_R_THE_ASN1_OBJECT_IDENTIFIER_IS_NOT_KNOWN_FOR_THIS_MD 116
# define RSA_R_UNKNOWN_ALGORITHM_TYPE                     117
# define RSA_R_UNKNOWN_DIGEST                             166
# define RSA_R_UNKNOWN_MASK_DIGEST                        151
# define RSA_R_UNKNOWN_PADDING_TYPE                       118
# define RSA_R_UNSUPPORTED_ENCRYPTION_TYPE                162
# define RSA_R_UNSUPPORTED_LABEL_SOURCE                   163
# define RSA_R_UNSUPPORTED_MASK_ALGORITHM                 153
# define RSA_R_UNSUPPORTED_MASK_PARAMETER                 154
# define RSA_R_UNSUPPORTED_SIGNATURE_TYPE                 155
# define RSA_R_VALUE_MISSING                              147
# define RSA_R_WRONG_SIGNATURE_LENGTH                     119

#endif
