

#ifndef HEADER_DRBG_RAND_H
# define HEADER_DRBG_RAND_H

# include <time.h>
# include <openssl/ossl_typ.h>
# include <openssl/obj_mac.h>




# define RAND_DRBG_FLAG_CTR_NO_DF            0x1


# if OPENSSL_API_COMPAT < 0x10200000L

#  define RAND_DRBG_USED_FLAGS  (RAND_DRBG_FLAG_CTR_NO_DF)
# endif


# define RAND_DRBG_STRENGTH             256

# define RAND_DRBG_TYPE                 NID_aes_256_ctr

# define RAND_DRBG_FLAGS                0


# ifdef  __cplusplus
extern "C" {
# endif


RAND_DRBG *RAND_DRBG_new(int type, unsigned int flags, RAND_DRBG *parent);
RAND_DRBG *RAND_DRBG_secure_new(int type, unsigned int flags, RAND_DRBG *parent);
int RAND_DRBG_set(RAND_DRBG *drbg, int type, unsigned int flags);
int RAND_DRBG_set_defaults(int type, unsigned int flags);
int RAND_DRBG_instantiate(RAND_DRBG *drbg,
                          const unsigned char *pers, size_t perslen);
int RAND_DRBG_uninstantiate(RAND_DRBG *drbg);
void RAND_DRBG_free(RAND_DRBG *drbg);


int RAND_DRBG_reseed(RAND_DRBG *drbg,
                     const unsigned char *adin, size_t adinlen,
                     int prediction_resistance);
int RAND_DRBG_generate(RAND_DRBG *drbg, unsigned char *out, size_t outlen,
                       int prediction_resistance,
                       const unsigned char *adin, size_t adinlen);
int RAND_DRBG_bytes(RAND_DRBG *drbg, unsigned char *out, size_t outlen);

int RAND_DRBG_set_reseed_interval(RAND_DRBG *drbg, unsigned int interval);
int RAND_DRBG_set_reseed_time_interval(RAND_DRBG *drbg, time_t interval);

int RAND_DRBG_set_reseed_defaults(
                                  unsigned int master_reseed_interval,
                                  unsigned int slave_reseed_interval,
                                  time_t master_reseed_time_interval,
                                  time_t slave_reseed_time_interval
                                  );

RAND_DRBG *RAND_DRBG_get0_master(void);
RAND_DRBG *RAND_DRBG_get0_public(void);
RAND_DRBG *RAND_DRBG_get0_private(void);


# define RAND_DRBG_get_ex_new_index(l, p, newf, dupf, freef) \
    CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_DRBG, l, p, newf, dupf, freef)
int RAND_DRBG_set_ex_data(RAND_DRBG *drbg, int idx, void *arg);
void *RAND_DRBG_get_ex_data(const RAND_DRBG *drbg, int idx);


typedef size_t (*RAND_DRBG_get_entropy_fn)(RAND_DRBG *drbg,
                                           unsigned char **pout,
                                           int entropy, size_t min_len,
                                           size_t max_len,
                                           int prediction_resistance);
typedef void (*RAND_DRBG_cleanup_entropy_fn)(RAND_DRBG *ctx,
                                             unsigned char *out, size_t outlen);
typedef size_t (*RAND_DRBG_get_nonce_fn)(RAND_DRBG *drbg, unsigned char **pout,
                                         int entropy, size_t min_len,
                                         size_t max_len);
typedef void (*RAND_DRBG_cleanup_nonce_fn)(RAND_DRBG *drbg,
                                           unsigned char *out, size_t outlen);

int RAND_DRBG_set_callbacks(RAND_DRBG *drbg,
                            RAND_DRBG_get_entropy_fn get_entropy,
                            RAND_DRBG_cleanup_entropy_fn cleanup_entropy,
                            RAND_DRBG_get_nonce_fn get_nonce,
                            RAND_DRBG_cleanup_nonce_fn cleanup_nonce);


# ifdef  __cplusplus
}
# endif

#endif
