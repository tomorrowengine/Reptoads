



#ifndef _my_attribute_h
#define _my_attribute_h

#if defined(__GNUC__)
# ifndef GCC_VERSION
#  define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
# endif
#endif


#ifndef __attribute__
# if !defined(__GNUC__) && !defined(__clang__)
#  define __attribute__(A)
# elif defined(__GNUC__)
#  ifndef GCC_VERSION
#   define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#  endif
#  if GCC_VERSION < 2008
#   define __attribute__(A)
#  elif defined(__cplusplus) && GCC_VERSION < 3004
#   define __attribute__(A)
#  endif
# endif
#endif


#ifndef ATTRIBUTE_FORMAT
# define ATTRIBUTE_FORMAT(style, m, n) __attribute__((format(style, m, n)))
#endif


#ifndef ATTRIBUTE_FORMAT_FPTR
# if (GCC_VERSION >= 3001)
#  define ATTRIBUTE_FORMAT_FPTR(style, m, n) ATTRIBUTE_FORMAT(style, m, n)
# else
#  define ATTRIBUTE_FORMAT_FPTR(style, m, n)
# endif 
#endif


#endif
