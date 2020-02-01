

#ifndef HEADER_MD5_H
# define HEADER_MD5_H

# include <openssl/opensslconf.h>

# ifndef OPENSSL_NO_MD5
# include <openssl/e_os2.h>
# include <stddef.h>
# ifdef  __cplusplus
extern "C" {
# endif


# define MD5_LONG unsigned int

# define MD5_CBLOCK      64
# define MD5_LBLOCK      (MD5_CBLOCK/4)
# define MD5_DIGEST_LENGTH 16

typedef struct MD5state_st {
    MD5_LONG A, B, C, D;
    MD5_LONG Nl, Nh;
    MD5_LONG data[MD5_LBLOCK];
    unsigned int num;
} MD5_CTX;

int MD5_Init(MD5_CTX *c);
int MD5_Update(MD5_CTX *c, const void *data, size_t len);
int MD5_Final(unsigned char *md, MD5_CTX *c);
unsigned char *MD5(const unsigned char *d, size_t n, unsigned char *md);
void MD5_Transform(MD5_CTX *c, const unsigned char *b);
# ifdef  __cplusplus
}
# endif
# endif

#endif
