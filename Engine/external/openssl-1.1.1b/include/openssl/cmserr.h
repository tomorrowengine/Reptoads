

#ifndef HEADER_CMSERR_H
# define HEADER_CMSERR_H

# include <openssl/opensslconf.h>

# ifndef OPENSSL_NO_CMS

#  ifdef  __cplusplus
extern "C"
#  endif
int ERR_load_CMS_strings(void);


#  define CMS_F_CHECK_CONTENT                              99
#  define CMS_F_CMS_ADD0_CERT                              164
#  define CMS_F_CMS_ADD0_RECIPIENT_KEY                     100
#  define CMS_F_CMS_ADD0_RECIPIENT_PASSWORD                165
#  define CMS_F_CMS_ADD1_RECEIPTREQUEST                    158
#  define CMS_F_CMS_ADD1_RECIPIENT_CERT                    101
#  define CMS_F_CMS_ADD1_SIGNER                            102
#  define CMS_F_CMS_ADD1_SIGNINGTIME                       103
#  define CMS_F_CMS_COMPRESS                               104
#  define CMS_F_CMS_COMPRESSEDDATA_CREATE                  105
#  define CMS_F_CMS_COMPRESSEDDATA_INIT_BIO                106
#  define CMS_F_CMS_COPY_CONTENT                           107
#  define CMS_F_CMS_COPY_MESSAGEDIGEST                     108
#  define CMS_F_CMS_DATA                                   109
#  define CMS_F_CMS_DATAFINAL                              110
#  define CMS_F_CMS_DATAINIT                               111
#  define CMS_F_CMS_DECRYPT                                112
#  define CMS_F_CMS_DECRYPT_SET1_KEY                       113
#  define CMS_F_CMS_DECRYPT_SET1_PASSWORD                  166
#  define CMS_F_CMS_DECRYPT_SET1_PKEY                      114
#  define CMS_F_CMS_DIGESTALGORITHM_FIND_CTX               115
#  define CMS_F_CMS_DIGESTALGORITHM_INIT_BIO               116
#  define CMS_F_CMS_DIGESTEDDATA_DO_FINAL                  117
#  define CMS_F_CMS_DIGEST_VERIFY                          118
#  define CMS_F_CMS_ENCODE_RECEIPT                         161
#  define CMS_F_CMS_ENCRYPT                                119
#  define CMS_F_CMS_ENCRYPTEDCONTENT_INIT                  179
#  define CMS_F_CMS_ENCRYPTEDCONTENT_INIT_BIO              120
#  define CMS_F_CMS_ENCRYPTEDDATA_DECRYPT                  121
#  define CMS_F_CMS_ENCRYPTEDDATA_ENCRYPT                  122
#  define CMS_F_CMS_ENCRYPTEDDATA_SET1_KEY                 123
#  define CMS_F_CMS_ENVELOPEDDATA_CREATE                   124
#  define CMS_F_CMS_ENVELOPEDDATA_INIT_BIO                 125
#  define CMS_F_CMS_ENVELOPED_DATA_INIT                    126
#  define CMS_F_CMS_ENV_ASN1_CTRL                          171
#  define CMS_F_CMS_FINAL                                  127
#  define CMS_F_CMS_GET0_CERTIFICATE_CHOICES               128
#  define CMS_F_CMS_GET0_CONTENT                           129
#  define CMS_F_CMS_GET0_ECONTENT_TYPE                     130
#  define CMS_F_CMS_GET0_ENVELOPED                         131
#  define CMS_F_CMS_GET0_REVOCATION_CHOICES                132
#  define CMS_F_CMS_GET0_SIGNED                            133
#  define CMS_F_CMS_MSGSIGDIGEST_ADD1                      162
#  define CMS_F_CMS_RECEIPTREQUEST_CREATE0                 159
#  define CMS_F_CMS_RECEIPT_VERIFY                         160
#  define CMS_F_CMS_RECIPIENTINFO_DECRYPT                  134
#  define CMS_F_CMS_RECIPIENTINFO_ENCRYPT                  169
#  define CMS_F_CMS_RECIPIENTINFO_KARI_ENCRYPT             178
#  define CMS_F_CMS_RECIPIENTINFO_KARI_GET0_ALG            175
#  define CMS_F_CMS_RECIPIENTINFO_KARI_GET0_ORIG_ID        173
#  define CMS_F_CMS_RECIPIENTINFO_KARI_GET0_REKS           172
#  define CMS_F_CMS_RECIPIENTINFO_KARI_ORIG_ID_CMP         174
#  define CMS_F_CMS_RECIPIENTINFO_KEKRI_DECRYPT            135
#  define CMS_F_CMS_RECIPIENTINFO_KEKRI_ENCRYPT            136
#  define CMS_F_CMS_RECIPIENTINFO_KEKRI_GET0_ID            137
#  define CMS_F_CMS_RECIPIENTINFO_KEKRI_ID_CMP             138
#  define CMS_F_CMS_RECIPIENTINFO_KTRI_CERT_CMP            139
#  define CMS_F_CMS_RECIPIENTINFO_KTRI_DECRYPT             140
#  define CMS_F_CMS_RECIPIENTINFO_KTRI_ENCRYPT             141
#  define CMS_F_CMS_RECIPIENTINFO_KTRI_GET0_ALGS           142
#  define CMS_F_CMS_RECIPIENTINFO_KTRI_GET0_SIGNER_ID      143
#  define CMS_F_CMS_RECIPIENTINFO_PWRI_CRYPT               167
#  define CMS_F_CMS_RECIPIENTINFO_SET0_KEY                 144
#  define CMS_F_CMS_RECIPIENTINFO_SET0_PASSWORD            168
#  define CMS_F_CMS_RECIPIENTINFO_SET0_PKEY                145
#  define CMS_F_CMS_SD_ASN1_CTRL                           170
#  define CMS_F_CMS_SET1_IAS                               176
#  define CMS_F_CMS_SET1_KEYID                             177
#  define CMS_F_CMS_SET1_SIGNERIDENTIFIER                  146
#  define CMS_F_CMS_SET_DETACHED                           147
#  define CMS_F_CMS_SIGN                                   148
#  define CMS_F_CMS_SIGNED_DATA_INIT                       149
#  define CMS_F_CMS_SIGNERINFO_CONTENT_SIGN                150
#  define CMS_F_CMS_SIGNERINFO_SIGN                        151
#  define CMS_F_CMS_SIGNERINFO_VERIFY                      152
#  define CMS_F_CMS_SIGNERINFO_VERIFY_CERT                 153
#  define CMS_F_CMS_SIGNERINFO_VERIFY_CONTENT              154
#  define CMS_F_CMS_SIGN_RECEIPT                           163
#  define CMS_F_CMS_STREAM                                 155
#  define CMS_F_CMS_UNCOMPRESS                             156
#  define CMS_F_CMS_VERIFY                                 157
#  define CMS_F_KEK_UNWRAP_KEY                             180


#  define CMS_R_ADD_SIGNER_ERROR                           99
#  define CMS_R_CERTIFICATE_ALREADY_PRESENT                175
#  define CMS_R_CERTIFICATE_HAS_NO_KEYID                   160
#  define CMS_R_CERTIFICATE_VERIFY_ERROR                   100
#  define CMS_R_CIPHER_INITIALISATION_ERROR                101
#  define CMS_R_CIPHER_PARAMETER_INITIALISATION_ERROR      102
#  define CMS_R_CMS_DATAFINAL_ERROR                        103
#  define CMS_R_CMS_LIB                                    104
#  define CMS_R_CONTENTIDENTIFIER_MISMATCH                 170
#  define CMS_R_CONTENT_NOT_FOUND                          105
#  define CMS_R_CONTENT_TYPE_MISMATCH                      171
#  define CMS_R_CONTENT_TYPE_NOT_COMPRESSED_DATA           106
#  define CMS_R_CONTENT_TYPE_NOT_ENVELOPED_DATA            107
#  define CMS_R_CONTENT_TYPE_NOT_SIGNED_DATA               108
#  define CMS_R_CONTENT_VERIFY_ERROR                       109
#  define CMS_R_CTRL_ERROR                                 110
#  define CMS_R_CTRL_FAILURE                               111
#  define CMS_R_DECRYPT_ERROR                              112
#  define CMS_R_ERROR_GETTING_PUBLIC_KEY                   113
#  define CMS_R_ERROR_READING_MESSAGEDIGEST_ATTRIBUTE      114
#  define CMS_R_ERROR_SETTING_KEY                          115
#  define CMS_R_ERROR_SETTING_RECIPIENTINFO                116
#  define CMS_R_INVALID_ENCRYPTED_KEY_LENGTH               117
#  define CMS_R_INVALID_KEY_ENCRYPTION_PARAMETER           176
#  define CMS_R_INVALID_KEY_LENGTH                         118
#  define CMS_R_MD_BIO_INIT_ERROR                          119
#  define CMS_R_MESSAGEDIGEST_ATTRIBUTE_WRONG_LENGTH       120
#  define CMS_R_MESSAGEDIGEST_WRONG_LENGTH                 121
#  define CMS_R_MSGSIGDIGEST_ERROR                         172
#  define CMS_R_MSGSIGDIGEST_VERIFICATION_FAILURE          162
#  define CMS_R_MSGSIGDIGEST_WRONG_LENGTH                  163
#  define CMS_R_NEED_ONE_SIGNER                            164
#  define CMS_R_NOT_A_SIGNED_RECEIPT                       165
#  define CMS_R_NOT_ENCRYPTED_DATA                         122
#  define CMS_R_NOT_KEK                                    123
#  define CMS_R_NOT_KEY_AGREEMENT                          181
#  define CMS_R_NOT_KEY_TRANSPORT                          124
#  define CMS_R_NOT_PWRI                                   177
#  define CMS_R_NOT_SUPPORTED_FOR_THIS_KEY_TYPE            125
#  define CMS_R_NO_CIPHER                                  126
#  define CMS_R_NO_CONTENT                                 127
#  define CMS_R_NO_CONTENT_TYPE                            173
#  define CMS_R_NO_DEFAULT_DIGEST                          128
#  define CMS_R_NO_DIGEST_SET                              129
#  define CMS_R_NO_KEY                                     130
#  define CMS_R_NO_KEY_OR_CERT                             174
#  define CMS_R_NO_MATCHING_DIGEST                         131
#  define CMS_R_NO_MATCHING_RECIPIENT                      132
#  define CMS_R_NO_MATCHING_SIGNATURE                      166
#  define CMS_R_NO_MSGSIGDIGEST                            167
#  define CMS_R_NO_PASSWORD                                178
#  define CMS_R_NO_PRIVATE_KEY                             133
#  define CMS_R_NO_PUBLIC_KEY                              134
#  define CMS_R_NO_RECEIPT_REQUEST                         168
#  define CMS_R_NO_SIGNERS                                 135
#  define CMS_R_PRIVATE_KEY_DOES_NOT_MATCH_CERTIFICATE     136
#  define CMS_R_RECEIPT_DECODE_ERROR                       169
#  define CMS_R_RECIPIENT_ERROR                            137
#  define CMS_R_SIGNER_CERTIFICATE_NOT_FOUND               138
#  define CMS_R_SIGNFINAL_ERROR                            139
#  define CMS_R_SMIME_TEXT_ERROR                           140
#  define CMS_R_STORE_INIT_ERROR                           141
#  define CMS_R_TYPE_NOT_COMPRESSED_DATA                   142
#  define CMS_R_TYPE_NOT_DATA                              143
#  define CMS_R_TYPE_NOT_DIGESTED_DATA                     144
#  define CMS_R_TYPE_NOT_ENCRYPTED_DATA                    145
#  define CMS_R_TYPE_NOT_ENVELOPED_DATA                    146
#  define CMS_R_UNABLE_TO_FINALIZE_CONTEXT                 147
#  define CMS_R_UNKNOWN_CIPHER                             148
#  define CMS_R_UNKNOWN_DIGEST_ALGORITHM                   149
#  define CMS_R_UNKNOWN_ID                                 150
#  define CMS_R_UNSUPPORTED_COMPRESSION_ALGORITHM          151
#  define CMS_R_UNSUPPORTED_CONTENT_TYPE                   152
#  define CMS_R_UNSUPPORTED_KEK_ALGORITHM                  153
#  define CMS_R_UNSUPPORTED_KEY_ENCRYPTION_ALGORITHM       179
#  define CMS_R_UNSUPPORTED_RECIPIENTINFO_TYPE             155
#  define CMS_R_UNSUPPORTED_RECIPIENT_TYPE                 154
#  define CMS_R_UNSUPPORTED_TYPE                           156
#  define CMS_R_UNWRAP_ERROR                               157
#  define CMS_R_UNWRAP_FAILURE                             180
#  define CMS_R_VERIFICATION_FAILURE                       158
#  define CMS_R_WRAP_ERROR                                 159

# endif
#endif
