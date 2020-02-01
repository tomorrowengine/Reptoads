

#ifndef HEADER_E_OS2_H
# define HEADER_E_OS2_H

# include <openssl/opensslconf.h>

#ifdef  __cplusplus
extern "C" {
#endif



# define OPENSSL_SYS_UNIX




# if defined(OPENSSL_SYS_MSDOS)
#  undef OPENSSL_SYS_UNIX
# endif



# if defined(OPENSSL_SYS_UEFI)
#  undef OPENSSL_SYS_UNIX
# elif defined(OPENSSL_SYS_UWIN)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_WIN32_UWIN
# else
#  if defined(__CYGWIN__) || defined(OPENSSL_SYS_CYGWIN)
#   define OPENSSL_SYS_WIN32_CYGWIN
#  else
#   if defined(_WIN32) || defined(OPENSSL_SYS_WIN32)
#    undef OPENSSL_SYS_UNIX
#    if !defined(OPENSSL_SYS_WIN32)
#     define OPENSSL_SYS_WIN32
#    endif
#   endif
#   if defined(_WIN64) || defined(OPENSSL_SYS_WIN64)
#    undef OPENSSL_SYS_UNIX
#    if !defined(OPENSSL_SYS_WIN64)
#     define OPENSSL_SYS_WIN64
#    endif
#   endif
#   if defined(OPENSSL_SYS_WINNT)
#    undef OPENSSL_SYS_UNIX
#   endif
#   if defined(OPENSSL_SYS_WINCE)
#    undef OPENSSL_SYS_UNIX
#   endif
#  endif
# endif


# if defined(OPENSSL_SYS_WIN32) || defined(OPENSSL_SYS_WIN64) || defined(OPENSSL_SYS_WINNT) || defined(OPENSSL_SYS_WINCE)
#  undef OPENSSL_SYS_UNIX
#  define OPENSSL_SYS_WINDOWS
#  ifndef OPENSSL_SYS_MSDOS
#   define OPENSSL_SYS_MSDOS
#  endif
# endif


# ifdef OPENSSL_SYS_WINDOWS
#  ifndef OPENSSL_OPT_WINDLL
#   if defined(_WINDLL)         
#    define OPENSSL_OPT_WINDLL
#   endif
#  endif
# endif


# if defined(__VMS) || defined(VMS) || defined(OPENSSL_SYS_VMS)
#  if !defined(OPENSSL_SYS_VMS)
#   undef OPENSSL_SYS_UNIX
#  endif
#  define OPENSSL_SYS_VMS
#  if defined(__DECC)
#   define OPENSSL_SYS_VMS_DECC
#  elif defined(__DECCXX)
#   define OPENSSL_SYS_VMS_DECC
#   define OPENSSL_SYS_VMS_DECCXX
#  else
#   define OPENSSL_SYS_VMS_NODECC
#  endif
# endif


# ifdef OPENSSL_SYS_UNIX
#  if defined(linux) || defined(__linux__) && !defined(OPENSSL_SYS_LINUX)
#   define OPENSSL_SYS_LINUX
#  endif
#  if defined(_AIX) && !defined(OPENSSL_SYS_AIX)
#   define OPENSSL_SYS_AIX
#  endif
# endif


# if defined(__VOS__) && !defined(OPENSSL_SYS_VOS)
#  define OPENSSL_SYS_VOS
#  ifdef __HPPA__
#   define OPENSSL_SYS_VOS_HPPA
#  endif
#  ifdef __IA32__
#   define OPENSSL_SYS_VOS_IA32
#  endif
# endif




# ifdef OPENSSL_SYS_MSDOS
#  define OPENSSL_UNISTD_IO <io.h>
#  define OPENSSL_DECLARE_EXIT extern void exit(int);
# else
#  define OPENSSL_UNISTD_IO OPENSSL_UNISTD
#  define OPENSSL_DECLARE_EXIT  
# endif



# if defined(OPENSSL_SYS_WINDOWS) && defined(OPENSSL_OPT_WINDLL)
#  define OPENSSL_EXPORT extern __declspec(dllexport)
#  define OPENSSL_EXTERN extern __declspec(dllimport)
# else
#  define OPENSSL_EXPORT extern
#  define OPENSSL_EXTERN extern
# endif


# ifdef OPENSSL_EXPORT_VAR_AS_FUNCTION
#  define OPENSSL_IMPLEMENT_GLOBAL(type,name,value)                      \
        type *_shadow_##name(void)                                      \
        { static type _hide_##name=value; return &_hide_##name; }
#  define OPENSSL_DECLARE_GLOBAL(type,name) type *_shadow_##name(void)
#  define OPENSSL_GLOBAL_REF(name) (*(_shadow_##name()))
# else
#  define OPENSSL_IMPLEMENT_GLOBAL(type,name,value) type _shadow_##name=value;
#  define OPENSSL_DECLARE_GLOBAL(type,name) OPENSSL_EXPORT type _shadow_##name
#  define OPENSSL_GLOBAL_REF(name) _shadow_##name
# endif

# ifdef _WIN32
#  ifdef _WIN64
#   define ossl_ssize_t __int64
#   define OSSL_SSIZE_MAX _I64_MAX
#  else
#   define ossl_ssize_t int
#   define OSSL_SSIZE_MAX INT_MAX
#  endif
# endif

# if defined(OPENSSL_SYS_UEFI) && !defined(ossl_ssize_t)
#  define ossl_ssize_t INTN
#  define OSSL_SSIZE_MAX MAX_INTN
# endif

# ifndef ossl_ssize_t
#  define ossl_ssize_t ssize_t
#  if defined(SSIZE_MAX)
#   define OSSL_SSIZE_MAX SSIZE_MAX
#  elif defined(_POSIX_SSIZE_MAX)
#   define OSSL_SSIZE_MAX _POSIX_SSIZE_MAX
#  else
#   define OSSL_SSIZE_MAX ((ssize_t)(SIZE_MAX>>1))
#  endif
# endif

# ifdef DEBUG_UNUSED
#  define __owur __attribute__((__warn_unused_result__))
# else
#  define __owur
# endif


# if defined(OPENSSL_SYS_UEFI)
typedef INT8 int8_t;
typedef UINT8 uint8_t;
typedef INT16 int16_t;
typedef UINT16 uint16_t;
typedef INT32 int32_t;
typedef UINT32 uint32_t;
typedef INT64 int64_t;
typedef UINT64 uint64_t;
# elif (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
     defined(__osf__) || defined(__sgi) || defined(__hpux) || \
     defined(OPENSSL_SYS_VMS) || defined (__OpenBSD__)
#  include <inttypes.h>
# elif defined(_MSC_VER) && _MSC_VER<=1500

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
# else
#  include <stdint.h>
# endif


# if !defined(inline) && !defined(__cplusplus)
#  if defined(__STDC_VERSION__) && __STDC_VERSION__>=199901L
   
#   define ossl_inline inline
#  elif defined(__GNUC__) && __GNUC__>=2
#   define ossl_inline __inline__
#  elif defined(_MSC_VER)
  
#   define ossl_inline __inline
#  else
#   define ossl_inline
#  endif
# else
#  define ossl_inline inline
# endif

# if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#  define ossl_noreturn _Noreturn
# elif defined(__GNUC__) && __GNUC__ >= 2
#  define ossl_noreturn __attribute__((noreturn))
# else
#  define ossl_noreturn
# endif


# if defined(__GNUC__)
#  define ossl_unused __attribute__((unused))
# else
#  define ossl_unused
# endif

#ifdef  __cplusplus
}
#endif
#endif
