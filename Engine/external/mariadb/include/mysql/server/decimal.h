

#ifndef _decimal_h
#define _decimal_h

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{TRUNCATE=0, HALF_EVEN, HALF_UP, CEILING, FLOOR}
  decimal_round_mode;
typedef int32 decimal_digit_t;


typedef struct st_decimal_t {
  int    intg, frac, len;
  my_bool sign;
  decimal_digit_t *buf;
} decimal_t;

int internal_str2dec(const char *from, decimal_t *to, char **end,
                     my_bool fixed);
int decimal2string(const decimal_t *from, char *to, int *to_len,
                   int fixed_precision, int fixed_decimals,
                   char filler);
int decimal2ulonglong(const decimal_t *from, ulonglong *to);
int ulonglong2decimal(ulonglong from, decimal_t *to);
int decimal2longlong(const decimal_t *from, longlong *to);
int longlong2decimal(longlong from, decimal_t *to);
int decimal2double(const decimal_t *from, double *to);
int double2decimal(double from, decimal_t *to);
int decimal_actual_fraction(const decimal_t *from);
int decimal2bin(const decimal_t *from, uchar *to, int precision, int scale);
int bin2decimal(const uchar *from, decimal_t *to, int precision, int scale);

int decimal_size(int precision, int scale);
int decimal_bin_size(int precision, int scale);
int decimal_result_size(decimal_t *from1, decimal_t *from2, char op,
                        int param);

int decimal_intg(const decimal_t *from);
int decimal_add(const decimal_t *from1, const decimal_t *from2, decimal_t *to);
int decimal_sub(const decimal_t *from1, const decimal_t *from2, decimal_t *to);
int decimal_cmp(const decimal_t *from1, const decimal_t *from2);
int decimal_mul(const decimal_t *from1, const decimal_t *from2, decimal_t *to);
int decimal_div(const decimal_t *from1, const decimal_t *from2, decimal_t *to,
                int scale_incr);
int decimal_mod(const decimal_t *from1, const decimal_t *from2, decimal_t *to);
int decimal_round(const decimal_t *from, decimal_t *to, int new_scale,
                  decimal_round_mode mode);
int decimal_is_zero(const decimal_t *from);
void max_decimal(int precision, int frac, decimal_t *to);

#define string2decimal(A,B,C) internal_str2dec((A), (B), (C), 0)
#define string2decimal_fixed(A,B,C) internal_str2dec((A), (B), (C), 1)



#define decimal_make_zero(dec)        do {                \
                                        (dec)->buf[0]=0;    \
                                        (dec)->intg=1;      \
                                        (dec)->frac=0;      \
                                        (dec)->sign=0;      \
                                      } while(0)



#define decimal_string_size(dec) (((dec)->intg ? (dec)->intg : 1) + \
				  (dec)->frac + ((dec)->frac > 0) + 2)


#define decimal_neg(dec) do { (dec)->sign^=1; } while(0)



#define E_DEC_OK                0
#define E_DEC_TRUNCATED         1
#define E_DEC_OVERFLOW          2
#define E_DEC_DIV_ZERO          4
#define E_DEC_BAD_NUM           8
#define E_DEC_OOM              16

#define E_DEC_ERROR            31
#define E_DEC_FATAL_ERROR      30

#ifdef __cplusplus
}
#endif

#endif

