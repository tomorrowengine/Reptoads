

#include <openssl/opensslv.h>


#if !defined(HAVE_YASSL) && defined(OPENSSL_VERSION_NUMBER)

#if OPENSSL_VERSION_NUMBER >= 0x10002000L && !defined(LIBRESSL_VERSION_NUMBER)
#define HAVE_X509_check_host 1
#endif

#if OPENSSL_VERSION_NUMBER >= 0x10100000L && !defined(LIBRESSL_VERSION_NUMBER)
#define HAVE_OPENSSL11 1
#define SSL_LIBRARY OpenSSL_version(OPENSSL_VERSION)
#define ERR_remove_state(X) ERR_clear_error()
#define EVP_CIPHER_CTX_SIZE 176
#define EVP_MD_CTX_SIZE 48
#undef EVP_MD_CTX_init
#define EVP_MD_CTX_init(X) do { memset((X), 0, EVP_MD_CTX_SIZE); EVP_MD_CTX_reset(X); } while(0)
#undef EVP_CIPHER_CTX_init
#define EVP_CIPHER_CTX_init(X) do { memset((X), 0, EVP_CIPHER_CTX_SIZE); EVP_CIPHER_CTX_reset(X); } while(0)


#undef ERR_free_strings
#define ERR_free_strings()
#undef EVP_cleanup
#define EVP_cleanup()
#undef CRYPTO_cleanup_all_ex_data
#define CRYPTO_cleanup_all_ex_data()
#undef SSL_load_error_strings
#define SSL_load_error_strings()

#else
#define HAVE_OPENSSL10 1
#define SSL_LIBRARY SSLeay_version(SSLEAY_VERSION)

#ifdef HAVE_ERR_remove_thread_state
#define ERR_remove_state(X) ERR_remove_thread_state(NULL)
#endif 

#endif 

#elif defined(HAVE_YASSL)
#define SSL_LIBRARY "YaSSL " YASSL_VERSION
#define BN_free(X) do { } while(0)
#endif 

#ifndef HAVE_OPENSSL11
#define ASN1_STRING_get0_data(X)        ASN1_STRING_data(X)
#define OPENSSL_init_ssl(X,Y)           SSL_library_init()
#define DH_set0_pqg(D,P,Q,G)            ((D)->p= (P), (D)->g= (G))
#define EVP_CIPHER_CTX_buf_noconst(ctx) ((ctx)->buf)
#define EVP_CIPHER_CTX_encrypting(ctx)  ((ctx)->encrypt)
#define EVP_CIPHER_CTX_SIZE             sizeof(EVP_CIPHER_CTX)
#define EVP_MD_CTX_SIZE                 sizeof(EVP_MD_CTX)

#define EVP_MD_CTX_reset(X) EVP_MD_CTX_cleanup(X)
#define EVP_CIPHER_CTX_reset(X) EVP_CIPHER_CTX_cleanup(X)
#define X509_get0_notBefore(X) X509_get_notBefore(X)
#define X509_get0_notAfter(X) X509_get_notAfter(X)
#endif

#ifndef TLS1_3_VERSION
#define SSL_CTX_set_ciphersuites(X,Y) 0
#endif

#ifdef	__cplusplus
extern "C" {
#endif 

int check_openssl_compatibility();

#ifdef	__cplusplus
}
#endif
