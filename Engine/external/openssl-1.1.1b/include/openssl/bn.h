

#ifndef HEADER_BN_H
# define HEADER_BN_H

# include <openssl/e_os2.h>
# ifndef OPENSSL_NO_STDIO
#  include <stdio.h>
# endif
# include <openssl/opensslconf.h>
# include <openssl/ossl_typ.h>
# include <openssl/crypto.h>
# include <openssl/bnerr.h>

#ifdef  __cplusplus
extern "C" {
#endif


# ifdef SIXTY_FOUR_BIT_LONG
#  define BN_ULONG        unsigned long
#  define BN_BYTES        8
# endif


# ifdef SIXTY_FOUR_BIT
#  define BN_ULONG        unsigned long long
#  define BN_BYTES        8
# endif

# ifdef THIRTY_TWO_BIT
#  define BN_ULONG        unsigned int
#  define BN_BYTES        4
# endif

# define BN_BITS2       (BN_BYTES * 8)
# define BN_BITS        (BN_BITS2 * 2)
# define BN_TBIT        ((BN_ULONG)1 << (BN_BITS2 - 1))

# define BN_FLG_MALLOCED         0x01
# define BN_FLG_STATIC_DATA      0x02


# define BN_FLG_CONSTTIME        0x04
# define BN_FLG_SECURE           0x08

# if OPENSSL_API_COMPAT < 0x00908000L

#  define BN_FLG_EXP_CONSTTIME BN_FLG_CONSTTIME
#  define BN_FLG_FREE            0x8000 
# endif

void BN_set_flags(BIGNUM *b, int n);
int BN_get_flags(const BIGNUM *b, int n);


#define BN_RAND_TOP_ANY    -1
#define BN_RAND_TOP_ONE     0
#define BN_RAND_TOP_TWO     1


#define BN_RAND_BOTTOM_ANY  0
#define BN_RAND_BOTTOM_ODD  1


void BN_with_flags(BIGNUM *dest, const BIGNUM *b, int flags);


int BN_GENCB_call(BN_GENCB *cb, int a, int b);

BN_GENCB *BN_GENCB_new(void);
void BN_GENCB_free(BN_GENCB *cb);


void BN_GENCB_set_old(BN_GENCB *gencb, void (*callback) (int, int, void *),
                      void *cb_arg);


void BN_GENCB_set(BN_GENCB *gencb, int (*callback) (int, int, BN_GENCB *),
                  void *cb_arg);

void *BN_GENCB_get_arg(BN_GENCB *cb);

# define BN_prime_checks 0      



# define BN_prime_checks_for_size(b) ((b) >= 3747 ?  3 : \
                                (b) >=  1345 ?  4 : \
                                (b) >=  476 ?  5 : \
                                (b) >=  400 ?  6 : \
                                (b) >=  347 ?  7 : \
                                (b) >=  308 ?  8 : \
                                (b) >=  55  ? 27 : \
                                 34)

# define BN_num_bytes(a) ((BN_num_bits(a)+7)/8)

int BN_abs_is_word(const BIGNUM *a, const BN_ULONG w);
int BN_is_zero(const BIGNUM *a);
int BN_is_one(const BIGNUM *a);
int BN_is_word(const BIGNUM *a, const BN_ULONG w);
int BN_is_odd(const BIGNUM *a);

# define BN_one(a)       (BN_set_word((a),1))

void BN_zero_ex(BIGNUM *a);

# if OPENSSL_API_COMPAT >= 0x00908000L
#  define BN_zero(a)      BN_zero_ex(a)
# else
#  define BN_zero(a)      (BN_set_word((a),0))
# endif

const BIGNUM *BN_value_one(void);
char *BN_options(void);
BN_CTX *BN_CTX_new(void);
BN_CTX *BN_CTX_secure_new(void);
void BN_CTX_free(BN_CTX *c);
void BN_CTX_start(BN_CTX *ctx);
BIGNUM *BN_CTX_get(BN_CTX *ctx);
void BN_CTX_end(BN_CTX *ctx);
int BN_rand(BIGNUM *rnd, int bits, int top, int bottom);
int BN_priv_rand(BIGNUM *rnd, int bits, int top, int bottom);
int BN_rand_range(BIGNUM *rnd, const BIGNUM *range);
int BN_priv_rand_range(BIGNUM *rnd, const BIGNUM *range);
int BN_pseudo_rand(BIGNUM *rnd, int bits, int top, int bottom);
int BN_pseudo_rand_range(BIGNUM *rnd, const BIGNUM *range);
int BN_num_bits(const BIGNUM *a);
int BN_num_bits_word(BN_ULONG l);
int BN_security_bits(int L, int N);
BIGNUM *BN_new(void);
BIGNUM *BN_secure_new(void);
void BN_clear_free(BIGNUM *a);
BIGNUM *BN_copy(BIGNUM *a, const BIGNUM *b);
void BN_swap(BIGNUM *a, BIGNUM *b);
BIGNUM *BN_bin2bn(const unsigned char *s, int len, BIGNUM *ret);
int BN_bn2bin(const BIGNUM *a, unsigned char *to);
int BN_bn2binpad(const BIGNUM *a, unsigned char *to, int tolen);
BIGNUM *BN_lebin2bn(const unsigned char *s, int len, BIGNUM *ret);
int BN_bn2lebinpad(const BIGNUM *a, unsigned char *to, int tolen);
BIGNUM *BN_mpi2bn(const unsigned char *s, int len, BIGNUM *ret);
int BN_bn2mpi(const BIGNUM *a, unsigned char *to);
int BN_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int BN_usub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int BN_uadd(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int BN_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
int BN_mul(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, BN_CTX *ctx);
int BN_sqr(BIGNUM *r, const BIGNUM *a, BN_CTX *ctx);

void BN_set_negative(BIGNUM *b, int n);

int BN_is_negative(const BIGNUM *b);

int BN_div(BIGNUM *dv, BIGNUM *rem, const BIGNUM *m, const BIGNUM *d,
           BN_CTX *ctx);
# define BN_mod(rem,m,d,ctx) BN_div(NULL,(rem),(m),(d),(ctx))
int BN_nnmod(BIGNUM *r, const BIGNUM *m, const BIGNUM *d, BN_CTX *ctx);
int BN_mod_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m,
               BN_CTX *ctx);
int BN_mod_add_quick(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                     const BIGNUM *m);
int BN_mod_sub(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m,
               BN_CTX *ctx);
int BN_mod_sub_quick(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                     const BIGNUM *m);
int BN_mod_mul(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, const BIGNUM *m,
               BN_CTX *ctx);
int BN_mod_sqr(BIGNUM *r, const BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);
int BN_mod_lshift1(BIGNUM *r, const BIGNUM *a, const BIGNUM *m, BN_CTX *ctx);
int BN_mod_lshift1_quick(BIGNUM *r, const BIGNUM *a, const BIGNUM *m);
int BN_mod_lshift(BIGNUM *r, const BIGNUM *a, int n, const BIGNUM *m,
                  BN_CTX *ctx);
int BN_mod_lshift_quick(BIGNUM *r, const BIGNUM *a, int n, const BIGNUM *m);

BN_ULONG BN_mod_word(const BIGNUM *a, BN_ULONG w);
BN_ULONG BN_div_word(BIGNUM *a, BN_ULONG w);
int BN_mul_word(BIGNUM *a, BN_ULONG w);
int BN_add_word(BIGNUM *a, BN_ULONG w);
int BN_sub_word(BIGNUM *a, BN_ULONG w);
int BN_set_word(BIGNUM *a, BN_ULONG w);
BN_ULONG BN_get_word(const BIGNUM *a);

int BN_cmp(const BIGNUM *a, const BIGNUM *b);
void BN_free(BIGNUM *a);
int BN_is_bit_set(const BIGNUM *a, int n);
int BN_lshift(BIGNUM *r, const BIGNUM *a, int n);
int BN_lshift1(BIGNUM *r, const BIGNUM *a);
int BN_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);

int BN_mod_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
               const BIGNUM *m, BN_CTX *ctx);
int BN_mod_exp_mont(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *m_ctx);
int BN_mod_exp_mont_consttime(BIGNUM *rr, const BIGNUM *a, const BIGNUM *p,
                              const BIGNUM *m, BN_CTX *ctx,
                              BN_MONT_CTX *in_mont);
int BN_mod_exp_mont_word(BIGNUM *r, BN_ULONG a, const BIGNUM *p,
                         const BIGNUM *m, BN_CTX *ctx, BN_MONT_CTX *m_ctx);
int BN_mod_exp2_mont(BIGNUM *r, const BIGNUM *a1, const BIGNUM *p1,
                     const BIGNUM *a2, const BIGNUM *p2, const BIGNUM *m,
                     BN_CTX *ctx, BN_MONT_CTX *m_ctx);
int BN_mod_exp_simple(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                      const BIGNUM *m, BN_CTX *ctx);

int BN_mask_bits(BIGNUM *a, int n);
# ifndef OPENSSL_NO_STDIO
int BN_print_fp(FILE *fp, const BIGNUM *a);
# endif
int BN_print(BIO *bio, const BIGNUM *a);
int BN_reciprocal(BIGNUM *r, const BIGNUM *m, int len, BN_CTX *ctx);
int BN_rshift(BIGNUM *r, const BIGNUM *a, int n);
int BN_rshift1(BIGNUM *r, const BIGNUM *a);
void BN_clear(BIGNUM *a);
BIGNUM *BN_dup(const BIGNUM *a);
int BN_ucmp(const BIGNUM *a, const BIGNUM *b);
int BN_set_bit(BIGNUM *a, int n);
int BN_clear_bit(BIGNUM *a, int n);
char *BN_bn2hex(const BIGNUM *a);
char *BN_bn2dec(const BIGNUM *a);
int BN_hex2bn(BIGNUM **a, const char *str);
int BN_dec2bn(BIGNUM **a, const char *str);
int BN_asc2bn(BIGNUM **a, const char *str);
int BN_gcd(BIGNUM *r, const BIGNUM *a, const BIGNUM *b, BN_CTX *ctx);
int BN_kronecker(const BIGNUM *a, const BIGNUM *b, BN_CTX *ctx); 
BIGNUM *BN_mod_inverse(BIGNUM *ret,
                       const BIGNUM *a, const BIGNUM *n, BN_CTX *ctx);
BIGNUM *BN_mod_sqrt(BIGNUM *ret,
                    const BIGNUM *a, const BIGNUM *n, BN_CTX *ctx);

void BN_consttime_swap(BN_ULONG swap, BIGNUM *a, BIGNUM *b, int nwords);


DEPRECATEDIN_0_9_8(BIGNUM *BN_generate_prime(BIGNUM *ret, int bits, int safe,
                                             const BIGNUM *add,
                                             const BIGNUM *rem,
                                             void (*callback) (int, int,
                                                               void *),
                                             void *cb_arg))
DEPRECATEDIN_0_9_8(int
                   BN_is_prime(const BIGNUM *p, int nchecks,
                               void (*callback) (int, int, void *),
                               BN_CTX *ctx, void *cb_arg))
DEPRECATEDIN_0_9_8(int
                   BN_is_prime_fasttest(const BIGNUM *p, int nchecks,
                                        void (*callback) (int, int, void *),
                                        BN_CTX *ctx, void *cb_arg,
                                        int do_trial_division))


int BN_generate_prime_ex(BIGNUM *ret, int bits, int safe, const BIGNUM *add,
                         const BIGNUM *rem, BN_GENCB *cb);
int BN_is_prime_ex(const BIGNUM *p, int nchecks, BN_CTX *ctx, BN_GENCB *cb);
int BN_is_prime_fasttest_ex(const BIGNUM *p, int nchecks, BN_CTX *ctx,
                            int do_trial_division, BN_GENCB *cb);

int BN_X931_generate_Xpq(BIGNUM *Xp, BIGNUM *Xq, int nbits, BN_CTX *ctx);

int BN_X931_derive_prime_ex(BIGNUM *p, BIGNUM *p1, BIGNUM *p2,
                            const BIGNUM *Xp, const BIGNUM *Xp1,
                            const BIGNUM *Xp2, const BIGNUM *e, BN_CTX *ctx,
                            BN_GENCB *cb);
int BN_X931_generate_prime_ex(BIGNUM *p, BIGNUM *p1, BIGNUM *p2, BIGNUM *Xp1,
                              BIGNUM *Xp2, const BIGNUM *Xp, const BIGNUM *e,
                              BN_CTX *ctx, BN_GENCB *cb);

BN_MONT_CTX *BN_MONT_CTX_new(void);
int BN_mod_mul_montgomery(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                          BN_MONT_CTX *mont, BN_CTX *ctx);
int BN_to_montgomery(BIGNUM *r, const BIGNUM *a, BN_MONT_CTX *mont,
                     BN_CTX *ctx);
int BN_from_montgomery(BIGNUM *r, const BIGNUM *a, BN_MONT_CTX *mont,
                       BN_CTX *ctx);
void BN_MONT_CTX_free(BN_MONT_CTX *mont);
int BN_MONT_CTX_set(BN_MONT_CTX *mont, const BIGNUM *mod, BN_CTX *ctx);
BN_MONT_CTX *BN_MONT_CTX_copy(BN_MONT_CTX *to, BN_MONT_CTX *from);
BN_MONT_CTX *BN_MONT_CTX_set_locked(BN_MONT_CTX **pmont, CRYPTO_RWLOCK *lock,
                                    const BIGNUM *mod, BN_CTX *ctx);


# define BN_BLINDING_NO_UPDATE   0x00000001
# define BN_BLINDING_NO_RECREATE 0x00000002

BN_BLINDING *BN_BLINDING_new(const BIGNUM *A, const BIGNUM *Ai, BIGNUM *mod);
void BN_BLINDING_free(BN_BLINDING *b);
int BN_BLINDING_update(BN_BLINDING *b, BN_CTX *ctx);
int BN_BLINDING_convert(BIGNUM *n, BN_BLINDING *b, BN_CTX *ctx);
int BN_BLINDING_invert(BIGNUM *n, BN_BLINDING *b, BN_CTX *ctx);
int BN_BLINDING_convert_ex(BIGNUM *n, BIGNUM *r, BN_BLINDING *b, BN_CTX *);
int BN_BLINDING_invert_ex(BIGNUM *n, const BIGNUM *r, BN_BLINDING *b,
                          BN_CTX *);

int BN_BLINDING_is_current_thread(BN_BLINDING *b);
void BN_BLINDING_set_current_thread(BN_BLINDING *b);
int BN_BLINDING_lock(BN_BLINDING *b);
int BN_BLINDING_unlock(BN_BLINDING *b);

unsigned long BN_BLINDING_get_flags(const BN_BLINDING *);
void BN_BLINDING_set_flags(BN_BLINDING *, unsigned long);
BN_BLINDING *BN_BLINDING_create_param(BN_BLINDING *b,
                                      const BIGNUM *e, BIGNUM *m, BN_CTX *ctx,
                                      int (*bn_mod_exp) (BIGNUM *r,
                                                         const BIGNUM *a,
                                                         const BIGNUM *p,
                                                         const BIGNUM *m,
                                                         BN_CTX *ctx,
                                                         BN_MONT_CTX *m_ctx),
                                      BN_MONT_CTX *m_ctx);

DEPRECATEDIN_0_9_8(void BN_set_params(int mul, int high, int low, int mont))
DEPRECATEDIN_0_9_8(int BN_get_params(int which)) 

BN_RECP_CTX *BN_RECP_CTX_new(void);
void BN_RECP_CTX_free(BN_RECP_CTX *recp);
int BN_RECP_CTX_set(BN_RECP_CTX *recp, const BIGNUM *rdiv, BN_CTX *ctx);
int BN_mod_mul_reciprocal(BIGNUM *r, const BIGNUM *x, const BIGNUM *y,
                          BN_RECP_CTX *recp, BN_CTX *ctx);
int BN_mod_exp_recp(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                    const BIGNUM *m, BN_CTX *ctx);
int BN_div_recp(BIGNUM *dv, BIGNUM *rem, const BIGNUM *m,
                BN_RECP_CTX *recp, BN_CTX *ctx);

# ifndef OPENSSL_NO_EC2M




int BN_GF2m_add(BIGNUM *r, const BIGNUM *a, const BIGNUM *b);
#  define BN_GF2m_sub(r, a, b) BN_GF2m_add(r, a, b)

int BN_GF2m_mod(BIGNUM *r, const BIGNUM *a, const BIGNUM *p);

int BN_GF2m_mod_mul(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                    const BIGNUM *p, BN_CTX *ctx);

int BN_GF2m_mod_sqr(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);

int BN_GF2m_mod_inv(BIGNUM *r, const BIGNUM *b, const BIGNUM *p, BN_CTX *ctx);

int BN_GF2m_mod_div(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                    const BIGNUM *p, BN_CTX *ctx);

int BN_GF2m_mod_exp(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                    const BIGNUM *p, BN_CTX *ctx);

int BN_GF2m_mod_sqrt(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                     BN_CTX *ctx);

int BN_GF2m_mod_solve_quad(BIGNUM *r, const BIGNUM *a, const BIGNUM *p,
                           BN_CTX *ctx);
#  define BN_GF2m_cmp(a, b) BN_ucmp((a), (b))


int BN_GF2m_mod_arr(BIGNUM *r, const BIGNUM *a, const int p[]);

int BN_GF2m_mod_mul_arr(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                        const int p[], BN_CTX *ctx);

int BN_GF2m_mod_sqr_arr(BIGNUM *r, const BIGNUM *a, const int p[],
                        BN_CTX *ctx);

int BN_GF2m_mod_inv_arr(BIGNUM *r, const BIGNUM *b, const int p[],
                        BN_CTX *ctx);

int BN_GF2m_mod_div_arr(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                        const int p[], BN_CTX *ctx);

int BN_GF2m_mod_exp_arr(BIGNUM *r, const BIGNUM *a, const BIGNUM *b,
                        const int p[], BN_CTX *ctx);

int BN_GF2m_mod_sqrt_arr(BIGNUM *r, const BIGNUM *a,
                         const int p[], BN_CTX *ctx);

int BN_GF2m_mod_solve_quad_arr(BIGNUM *r, const BIGNUM *a,
                               const int p[], BN_CTX *ctx);
int BN_GF2m_poly2arr(const BIGNUM *a, int p[], int max);
int BN_GF2m_arr2poly(const int p[], BIGNUM *a);

# endif


int BN_nist_mod_192(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_224(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_256(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_384(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);
int BN_nist_mod_521(BIGNUM *r, const BIGNUM *a, const BIGNUM *p, BN_CTX *ctx);

const BIGNUM *BN_get0_nist_prime_192(void);
const BIGNUM *BN_get0_nist_prime_224(void);
const BIGNUM *BN_get0_nist_prime_256(void);
const BIGNUM *BN_get0_nist_prime_384(void);
const BIGNUM *BN_get0_nist_prime_521(void);

int (*BN_nist_mod_func(const BIGNUM *p)) (BIGNUM *r, const BIGNUM *a,
                                          const BIGNUM *field, BN_CTX *ctx);

int BN_generate_dsa_nonce(BIGNUM *out, const BIGNUM *range,
                          const BIGNUM *priv, const unsigned char *message,
                          size_t message_len, BN_CTX *ctx);


BIGNUM *BN_get_rfc2409_prime_768(BIGNUM *bn);
BIGNUM *BN_get_rfc2409_prime_1024(BIGNUM *bn);


BIGNUM *BN_get_rfc3526_prime_1536(BIGNUM *bn);
BIGNUM *BN_get_rfc3526_prime_2048(BIGNUM *bn);
BIGNUM *BN_get_rfc3526_prime_3072(BIGNUM *bn);
BIGNUM *BN_get_rfc3526_prime_4096(BIGNUM *bn);
BIGNUM *BN_get_rfc3526_prime_6144(BIGNUM *bn);
BIGNUM *BN_get_rfc3526_prime_8192(BIGNUM *bn);

# if OPENSSL_API_COMPAT < 0x10100000L
#  define get_rfc2409_prime_768 BN_get_rfc2409_prime_768
#  define get_rfc2409_prime_1024 BN_get_rfc2409_prime_1024
#  define get_rfc3526_prime_1536 BN_get_rfc3526_prime_1536
#  define get_rfc3526_prime_2048 BN_get_rfc3526_prime_2048
#  define get_rfc3526_prime_3072 BN_get_rfc3526_prime_3072
#  define get_rfc3526_prime_4096 BN_get_rfc3526_prime_4096
#  define get_rfc3526_prime_6144 BN_get_rfc3526_prime_6144
#  define get_rfc3526_prime_8192 BN_get_rfc3526_prime_8192
# endif

int BN_bntest_rand(BIGNUM *rnd, int bits, int top, int bottom);


# ifdef  __cplusplus
}
# endif
#endif
