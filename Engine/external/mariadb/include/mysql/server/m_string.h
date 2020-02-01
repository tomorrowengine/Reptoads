





#ifndef _m_string_h
#define _m_string_h

#include "my_decimal_limits.h"

#ifndef __USE_GNU
#define __USE_GNU				
#endif
#if defined(HAVE_STRINGS_H)
#include <strings.h>
#endif
#if defined(HAVE_STRING_H)
#include <string.h>
#endif


#if defined(HAVE_MEMORY_H) && !defined(__cplusplus)
#include <memory.h>
#endif

#if !defined(HAVE_MEMCPY) && !defined(HAVE_MEMMOVE)
# define memcpy(d, s, n)	bcopy ((s), (d), (n))
# define memset(A,C,B)		bfill((A),(B),(C))
# define memmove(d, s, n)	bmove ((d), (s), (n))
#elif defined(HAVE_MEMMOVE)
# define bmove(d, s, n)		memmove((d), (s), (n))
#endif


#if !defined(HAVE_BFILL)
# define bfill(A,B,C)           memset((A),(C),(B))
#endif

# define bmove_align(A,B,C)     memcpy((A),(B),(C))

# define bcmp(A,B,C)		memcmp((A),(B),(C))

#if !defined(bzero)
# define bzero(A,B)             memset((A),0,(B))
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#ifdef DBUG_OFF
#if defined(HAVE_STPCPY) && MY_GNUC_PREREQ(3, 4) && !defined(__INTEL_COMPILER)
#define strmov(A,B) __builtin_stpcpy((A),(B))
#elif defined(HAVE_STPCPY)
#define strmov(A,B) stpcpy((A),(B))
#endif
#endif


extern const char _dig_vec_upper[];
extern const char _dig_vec_lower[];

extern char *strmov_overlapp(char *dest, const char *src);

#if defined(_lint) || defined(FORCE_INIT_OF_VARS)
#define LINT_INIT_STRUCT(var) bzero(&var, sizeof(var)) 
#else
#define LINT_INIT_STRUCT(var)
#endif



extern	void bmove_upp(uchar *dst,const uchar *src,size_t len);
extern	void bchange(uchar *dst,size_t old_len,const uchar *src,
		     size_t new_len,size_t tot_len);
extern	void strappend(char *s,size_t len,pchar fill);
extern	char *strend(const char *s);
extern  char *strcend(const char *, pchar);
extern	char *strfill(char * s,size_t len,pchar fill);
extern	char *strmake(char *dst,const char *src,size_t length);

#if !defined(__GNUC__) || (__GNUC__ < 4)
#define strmake_buf(D,S)        strmake(D, S, sizeof(D) - 1)
#else
#define strmake_buf(D,S) ({                             \
  typeof (D) __x __attribute__((unused)) = { 2 };       \
  strmake(D, S, sizeof(D) - 1);                         \
  })
#endif

#ifndef strmov
extern	char *strmov(char *dst,const char *src);
#endif
extern	char *strnmov(char *dst, const char *src, size_t n);
extern	char *strcont(const char *src, const char *set);
extern	char *strxmov(char *dst, const char *src, ...);
extern	char *strxnmov(char *dst, size_t len, const char *src, ...);


#ifndef HAVE_STRNLEN
extern size_t strnlen(const char *s, size_t n);
#endif

extern int is_prefix(const char *, const char *);


typedef enum {
  MY_GCVT_ARG_FLOAT,
  MY_GCVT_ARG_DOUBLE
} my_gcvt_arg_type;

double my_strtod(const char *str, char **end, int *error);
double my_atof(const char *nptr);
size_t my_fcvt(double x, int precision, char *to, my_bool *error);
size_t my_gcvt(double x, my_gcvt_arg_type type, int width, char *to,
               my_bool *error);


#define FLOATING_POINT_BUFFER (311 + DECIMAL_NOT_SPECIFIED)


#define MAX_DECPT_FOR_F_FORMAT DBL_DIG


#define MY_GCVT_MAX_FIELD_WIDTH (DBL_DIG + 4 + MY_MAX(5, MAX_DECPT_FOR_F_FORMAT)) \

extern char *llstr(longlong value,char *buff);
extern char *ullstr(longlong value,char *buff);
#ifndef HAVE_STRTOUL
extern long strtol(const char *str, char **ptr, int base);
extern ulong strtoul(const char *str, char **ptr, int base);
#endif

extern char *int2str(long val, char *dst, int radix, int upcase);
extern char *int10_to_str(long val,char *dst,int radix);
extern char *str2int(const char *src,int radix,long lower,long upper,
			 long *val);
longlong my_strtoll10(const char *nptr, char **endptr, int *error);
#if SIZEOF_LONG == SIZEOF_LONG_LONG
#define ll2str(A,B,C,D) int2str((A),(B),(C),(D))
#define longlong10_to_str(A,B,C) int10_to_str((A),(B),(C))
#undef strtoll
#define strtoll(A,B,C) strtol((A),(B),(C))
#define strtoull(A,B,C) strtoul((A),(B),(C))
#ifndef HAVE_STRTOULL
#define HAVE_STRTOULL
#endif
#ifndef HAVE_STRTOLL
#define HAVE_STRTOLL
#endif
#else
#ifdef HAVE_LONG_LONG
extern char *ll2str(longlong val,char *dst,int radix, int upcase);
extern char *longlong10_to_str(longlong val,char *dst,int radix);
#if (!defined(HAVE_STRTOULL) || defined(NO_STRTOLL_PROTO))
extern longlong strtoll(const char *str, char **ptr, int base);
extern ulonglong strtoull(const char *str, char **ptr, int base);
#endif
#endif
#endif
#define longlong2str(A,B,C) ll2str((A),(B),(C),1)

#if defined(__cplusplus)
}
#endif

#include <mysql/plugin.h>

#define STRING_WITH_LEN(X) (X), ((size_t) (sizeof(X) - 1))
#define USTRING_WITH_LEN(X) ((uchar*) X), ((size_t) (sizeof(X) - 1))
#define C_STRING_WITH_LEN(X) ((char *) (X)), ((size_t) (sizeof(X) - 1))
#define LEX_STRING_WITH_LEN(X) (X).str, (X).length

typedef struct st_mysql_const_lex_string LEX_CSTRING;


struct st_mysql_const_unsigned_lex_string
{
  const uchar *str;
  size_t length;
};
typedef struct st_mysql_const_unsigned_lex_string LEX_CUSTRING;

static inline void lex_string_set(LEX_CSTRING *lex_str, const char *c_str)
{
  lex_str->str= c_str;
  lex_str->length= strlen(c_str);
}
static inline void lex_string_set3(LEX_CSTRING *lex_str, const char *c_str,
                                   size_t len)
{
  lex_str->str= c_str;
  lex_str->length= len;
}

#ifdef __cplusplus
static inline char *safe_str(char *str)
{ return str ? str : const_cast<char*>(""); }
#endif

static inline const char *safe_str(const char *str)
{ return str ? str : ""; }

static inline size_t safe_strlen(const char *str)
{ return str ? strlen(str) : 0; }

#endif
