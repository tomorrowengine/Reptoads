

#ifndef HEADER_OSSL_STORE_H
# define HEADER_OSSL_STORE_H

# include <stdarg.h>
# include <openssl/ossl_typ.h>
# include <openssl/pem.h>
# include <openssl/storeerr.h>

# ifdef  __cplusplus
extern "C" {
# endif



typedef struct ossl_store_ctx_st OSSL_STORE_CTX;


typedef OSSL_STORE_INFO *(*OSSL_STORE_post_process_info_fn)(OSSL_STORE_INFO *,
                                                            void *);


OSSL_STORE_CTX *OSSL_STORE_open(const char *uri, const UI_METHOD *ui_method,
                                void *ui_data,
                                OSSL_STORE_post_process_info_fn post_process,
                                void *post_process_data);


int OSSL_STORE_ctrl(OSSL_STORE_CTX *ctx, int cmd, ... );
int OSSL_STORE_vctrl(OSSL_STORE_CTX *ctx, int cmd, va_list args);



# define OSSL_STORE_C_USE_SECMEM      1

# define OSSL_STORE_C_CUSTOM_START    100


OSSL_STORE_INFO *OSSL_STORE_load(OSSL_STORE_CTX *ctx);


int OSSL_STORE_eof(OSSL_STORE_CTX *ctx);


int OSSL_STORE_error(OSSL_STORE_CTX *ctx);


int OSSL_STORE_close(OSSL_STORE_CTX *ctx);





# define OSSL_STORE_INFO_NAME           1   
# define OSSL_STORE_INFO_PARAMS         2   
# define OSSL_STORE_INFO_PKEY           3   
# define OSSL_STORE_INFO_CERT           4   
# define OSSL_STORE_INFO_CRL            5   


OSSL_STORE_INFO *OSSL_STORE_INFO_new_NAME(char *name);
int OSSL_STORE_INFO_set0_NAME_description(OSSL_STORE_INFO *info, char *desc);
OSSL_STORE_INFO *OSSL_STORE_INFO_new_PARAMS(EVP_PKEY *params);
OSSL_STORE_INFO *OSSL_STORE_INFO_new_PKEY(EVP_PKEY *pkey);
OSSL_STORE_INFO *OSSL_STORE_INFO_new_CERT(X509 *x509);
OSSL_STORE_INFO *OSSL_STORE_INFO_new_CRL(X509_CRL *crl);


int OSSL_STORE_INFO_get_type(const OSSL_STORE_INFO *info);
const char *OSSL_STORE_INFO_get0_NAME(const OSSL_STORE_INFO *info);
char *OSSL_STORE_INFO_get1_NAME(const OSSL_STORE_INFO *info);
const char *OSSL_STORE_INFO_get0_NAME_description(const OSSL_STORE_INFO *info);
char *OSSL_STORE_INFO_get1_NAME_description(const OSSL_STORE_INFO *info);
EVP_PKEY *OSSL_STORE_INFO_get0_PARAMS(const OSSL_STORE_INFO *info);
EVP_PKEY *OSSL_STORE_INFO_get1_PARAMS(const OSSL_STORE_INFO *info);
EVP_PKEY *OSSL_STORE_INFO_get0_PKEY(const OSSL_STORE_INFO *info);
EVP_PKEY *OSSL_STORE_INFO_get1_PKEY(const OSSL_STORE_INFO *info);
X509 *OSSL_STORE_INFO_get0_CERT(const OSSL_STORE_INFO *info);
X509 *OSSL_STORE_INFO_get1_CERT(const OSSL_STORE_INFO *info);
X509_CRL *OSSL_STORE_INFO_get0_CRL(const OSSL_STORE_INFO *info);
X509_CRL *OSSL_STORE_INFO_get1_CRL(const OSSL_STORE_INFO *info);

const char *OSSL_STORE_INFO_type_string(int type);


void OSSL_STORE_INFO_free(OSSL_STORE_INFO *info);





# define OSSL_STORE_SEARCH_BY_NAME              1 
# define OSSL_STORE_SEARCH_BY_ISSUER_SERIAL     2
# define OSSL_STORE_SEARCH_BY_KEY_FINGERPRINT   3
# define OSSL_STORE_SEARCH_BY_ALIAS             4


int OSSL_STORE_supports_search(OSSL_STORE_CTX *ctx, int search_type);



OSSL_STORE_SEARCH *OSSL_STORE_SEARCH_by_name(X509_NAME *name);
OSSL_STORE_SEARCH *OSSL_STORE_SEARCH_by_issuer_serial(X509_NAME *name,
                                                      const ASN1_INTEGER
                                                      *serial);
OSSL_STORE_SEARCH *OSSL_STORE_SEARCH_by_key_fingerprint(const EVP_MD *digest,
                                                        const unsigned char
                                                        *bytes, size_t len);
OSSL_STORE_SEARCH *OSSL_STORE_SEARCH_by_alias(const char *alias);


void OSSL_STORE_SEARCH_free(OSSL_STORE_SEARCH *search);


int OSSL_STORE_SEARCH_get_type(const OSSL_STORE_SEARCH *criterion);
X509_NAME *OSSL_STORE_SEARCH_get0_name(OSSL_STORE_SEARCH *criterion);
const ASN1_INTEGER *OSSL_STORE_SEARCH_get0_serial(const OSSL_STORE_SEARCH
                                                  *criterion);
const unsigned char *OSSL_STORE_SEARCH_get0_bytes(const OSSL_STORE_SEARCH
                                                  *criterion, size_t *length);
const char *OSSL_STORE_SEARCH_get0_string(const OSSL_STORE_SEARCH *criterion);
const EVP_MD *OSSL_STORE_SEARCH_get0_digest(const OSSL_STORE_SEARCH *criterion);


int OSSL_STORE_expect(OSSL_STORE_CTX *ctx, int expected_type);
int OSSL_STORE_find(OSSL_STORE_CTX *ctx, OSSL_STORE_SEARCH *search);




typedef struct ossl_store_loader_st OSSL_STORE_LOADER;
OSSL_STORE_LOADER *OSSL_STORE_LOADER_new(ENGINE *e, const char *scheme);
const ENGINE *OSSL_STORE_LOADER_get0_engine(const OSSL_STORE_LOADER *loader);
const char *OSSL_STORE_LOADER_get0_scheme(const OSSL_STORE_LOADER *loader);

typedef struct ossl_store_loader_ctx_st OSSL_STORE_LOADER_CTX;
typedef OSSL_STORE_LOADER_CTX *(*OSSL_STORE_open_fn)(const OSSL_STORE_LOADER
                                                     *loader,
                                                     const char *uri,
                                                     const UI_METHOD *ui_method,
                                                     void *ui_data);
int OSSL_STORE_LOADER_set_open(OSSL_STORE_LOADER *loader,
                               OSSL_STORE_open_fn open_function);
typedef int (*OSSL_STORE_ctrl_fn)(OSSL_STORE_LOADER_CTX *ctx, int cmd,
                                  va_list args);
int OSSL_STORE_LOADER_set_ctrl(OSSL_STORE_LOADER *loader,
                               OSSL_STORE_ctrl_fn ctrl_function);
typedef int (*OSSL_STORE_expect_fn)(OSSL_STORE_LOADER_CTX *ctx, int expected);
int OSSL_STORE_LOADER_set_expect(OSSL_STORE_LOADER *loader,
                                 OSSL_STORE_expect_fn expect_function);
typedef int (*OSSL_STORE_find_fn)(OSSL_STORE_LOADER_CTX *ctx,
                                  OSSL_STORE_SEARCH *criteria);
int OSSL_STORE_LOADER_set_find(OSSL_STORE_LOADER *loader,
                               OSSL_STORE_find_fn find_function);
typedef OSSL_STORE_INFO *(*OSSL_STORE_load_fn)(OSSL_STORE_LOADER_CTX *ctx,
                                               const UI_METHOD *ui_method,
                                               void *ui_data);
int OSSL_STORE_LOADER_set_load(OSSL_STORE_LOADER *loader,
                               OSSL_STORE_load_fn load_function);
typedef int (*OSSL_STORE_eof_fn)(OSSL_STORE_LOADER_CTX *ctx);
int OSSL_STORE_LOADER_set_eof(OSSL_STORE_LOADER *loader,
                              OSSL_STORE_eof_fn eof_function);
typedef int (*OSSL_STORE_error_fn)(OSSL_STORE_LOADER_CTX *ctx);
int OSSL_STORE_LOADER_set_error(OSSL_STORE_LOADER *loader,
                                OSSL_STORE_error_fn error_function);
typedef int (*OSSL_STORE_close_fn)(OSSL_STORE_LOADER_CTX *ctx);
int OSSL_STORE_LOADER_set_close(OSSL_STORE_LOADER *loader,
                                OSSL_STORE_close_fn close_function);
void OSSL_STORE_LOADER_free(OSSL_STORE_LOADER *loader);

int OSSL_STORE_register_loader(OSSL_STORE_LOADER *loader);
OSSL_STORE_LOADER *OSSL_STORE_unregister_loader(const char *scheme);


int OSSL_STORE_do_all_loaders(void (*do_function) (const OSSL_STORE_LOADER
                                                   *loader, void *do_arg),
                              void *do_arg);

# ifdef  __cplusplus
}
# endif
#endif
