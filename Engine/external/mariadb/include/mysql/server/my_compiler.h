#ifndef MY_COMPILER_INCLUDED
#define MY_COMPILER_INCLUDED








#if defined __GNUC__

# define MY_GNUC_PREREQ(maj, min) \
    ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))

# define MY_ALIGN_EXT

# if MY_GNUC_PREREQ(4,5)
#   define MY_ASSERT_UNREACHABLE()   __builtin_unreachable()
# endif


#elif defined _MSC_VER
# define MY_ALIGNOF(type)   __alignof(type)
# define MY_ALIGNED(n)      __declspec(align(n))


#elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
# if __SUNPRO_C >= 0x590
#   define MY_ALIGN_EXT
# endif


#elif defined __xlC__
# if __xlC__ >= 0x0600
#   define MY_ALIGN_EXT
# endif


#elif defined(__HP_aCC) || defined(__HP_cc)
# if (__HP_aCC >= 60000) || (__HP_cc >= 60000)
#   define MY_ALIGN_EXT
# endif
#endif

#ifdef MY_ALIGN_EXT

# define MY_ALIGNOF(type)   __alignof__(type)

# define MY_ALIGNED(n)      __attribute__((__aligned__((n))))
#endif



#ifndef MY_GNUC_PREREQ
# define MY_GNUC_PREREQ(maj, min) (0)
#endif

#ifndef MY_ALIGNOF
# ifdef __cplusplus
    template<typename type> struct my_alignof_helper { char m1; type m2; };
    
#   define MY_ALIGNOF(type)   offsetof(my_alignof_helper<type>, m2)
# else
#   define MY_ALIGNOF(type)   offsetof(struct { char m1; type m2; }, m2)
# endif
#endif

#ifndef MY_ASSERT_UNREACHABLE
# define MY_ASSERT_UNREACHABLE()  do { assert(0); } while (0)
#endif



#ifdef __cplusplus


# if defined(MY_ALIGNED)

template<size_t alignment> struct my_alignment_imp;
template<> struct MY_ALIGNED(1) my_alignment_imp<1> {};
template<> struct MY_ALIGNED(2) my_alignment_imp<2> {};
template<> struct MY_ALIGNED(4) my_alignment_imp<4> {};
template<> struct MY_ALIGNED(8) my_alignment_imp<8> {};
template<> struct MY_ALIGNED(16) my_alignment_imp<16> {};

# else
template<size_t alignment>
struct my_alignment_imp { double m1; };
# endif


template <size_t size, size_t alignment>
struct my_aligned_storage
{
  union
  {
    char data[size];
    my_alignment_imp<alignment> align;
  };
};

#endif 

# ifndef MY_ALIGNED

#define MY_ALIGNED(size)
#endif

#ifdef __GNUC__
# define ATTRIBUTE_NORETURN __attribute__((noreturn))
# if MY_GNUC_PREREQ(4,3)

#  define ATTRIBUTE_COLD __attribute__((cold))
# endif
#elif defined _MSC_VER
# define ATTRIBUTE_NORETURN __declspec(noreturn)
#else
# define ATTRIBUTE_NORETURN 
#endif

#ifndef ATTRIBUTE_COLD
# define ATTRIBUTE_COLD 
#endif

#include <my_attribute.h>

#endif 
