



#ifndef MY_GLOBAL_INCLUDED
#define MY_GLOBAL_INCLUDED


#if !defined(__WIN__) && defined(_WIN32)
#define __WIN__
#endif


#define INNODB_COMPATIBILITY_HOOKS

#ifdef __CYGWIN__

#undef WIN
#undef WIN32
#undef _WIN
#undef _WIN32
#undef _WIN64
#undef __WIN__
#undef __WIN32__
#define HAVE_ERRNO_AS_DEFINE
#define _POSIX_MONOTONIC_CLOCK
#define _POSIX_THREAD_CPUTIME
#endif 

#if defined(__OpenBSD__) && (OpenBSD >= 200411)
#define HAVE_ERRNO_AS_DEFINE
#endif

#if defined(i386) && !defined(__i386__)
#define __i386__
#endif


#ifdef __cplusplus
#define C_MODE_START    extern "C" {
#define C_MODE_END	}
#else
#define C_MODE_START
#define C_MODE_END
#endif

#ifdef __cplusplus
#define CPP_UNNAMED_NS_START  namespace {
#define CPP_UNNAMED_NS_END    }
#endif

#include <my_config.h>

#ifdef WITH_PERFSCHEMA_STORAGE_ENGINE
#define HAVE_PSI_INTERFACE
#endif 


#ifdef __WIN__
#define IF_WIN(A,B) A
#else
#define IF_WIN(A,B) B
#endif

#ifdef EMBEDDED_LIBRARY
#define IF_EMBEDDED(A,B) A
#else
#define IF_EMBEDDED(A,B) B
#endif

#ifdef WITH_PARTITION_STORAGE_ENGINE
#define IF_PARTITIONING(A,B) A
#else
#define IF_PARTITIONING(A,B) B
#endif

#if defined (_WIN32)

#undef SIZEOF_OFF_T
#define SIZEOF_OFF_T 8


#ifndef NOGDI
#define NOGDI
#endif


#include <winsock2.h>
#include <ws2tcpip.h> 
#include <io.h>       
#include <process.h>  

#define sleep(a) Sleep((a)*1000)


#define F_OK 0
#define W_OK 2
#define R_OK 4                        


#define F_RDLCK 1
#define F_WRLCK 2
#define F_UNLCK 3
#define F_TO_EOF 0x3FFFFFFF


#define HAVE_SMEM 1
#define HAVE_NAMED_PIPE 1
#define shared_memory_buffer_length 16000
#define default_shared_memory_base_name "MYSQL"
#endif 



#if defined(_AIX) && defined(_LARGE_FILE_API)
#undef _LARGE_FILE_API
#undef __GNUG__
#endif


#if defined(__APPLE__) && defined(__MACH__)
#  undef SIZEOF_CHARP 
#  undef SIZEOF_INT 
#  undef SIZEOF_LONG 
#  undef SIZEOF_LONG_LONG 
#  undef SIZEOF_OFF_T 
#  undef WORDS_BIGENDIAN
#  define SIZEOF_INT 4
#  define SIZEOF_LONG_LONG 8
#  define SIZEOF_OFF_T 8
#  if defined(__i386__) || defined(__ppc__)
#    define SIZEOF_CHARP 4
#    define SIZEOF_LONG 4
#  elif defined(__x86_64__) || defined(__ppc64__)
#    define SIZEOF_CHARP 8
#    define SIZEOF_LONG 8
#  else
#    error Building FAT binary for an unknown architecture.
#  endif
#  if defined(__ppc__) || defined(__ppc64__)
#    define WORDS_BIGENDIAN
#  endif
#endif 




#if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#define __builtin_expect(x, expected_value) (x)
#endif


#if defined(TARGET_OS_LINUX) || defined(__GLIBC__)
#undef  _GNU_SOURCE
#define _GNU_SOURCE 1
#endif


#if defined(HAVE_SYS_TYPES_H) && ( defined(sun) || defined(__sun) )
#include <sys/types.h>
#endif

#define __EXTENSIONS__ 1	
#ifndef __STDC_EXT__
#define __STDC_EXT__ 1          
#endif


#ifdef __sun
#if __STDC_VERSION__ - 0 >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif
#endif


#ifdef _AIX

#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif  
#endif


#if !defined(__WIN__)
#ifndef _POSIX_PTHREAD_SEMANTICS
#define _POSIX_PTHREAD_SEMANTICS 
#endif

#if !defined(SCO)
#define _REENTRANT	1	
#endif
#if !defined(_THREAD_SAFE) && !defined(_AIX)
#define _THREAD_SAFE            
#endif
#if defined(HPUX10) || defined(HPUX11)
C_MODE_START			
#include <pthread.h>
C_MODE_END
#else
#include <pthread.h>		
#endif
#if !defined(SCO) && !defined(_REENTRANT)
#define _REENTRANT	1	
#endif
#endif 


#ifdef _AIX			
#define _H_STRINGS
#define _SYS_STREAM_H
	
#define ulonglong2double(A) my_ulonglong2double(A)
#define my_off_t2double(A)  my_ulonglong2double(A)
C_MODE_START
inline double my_ulonglong2double(unsigned long long A) { return (double)A; }
C_MODE_END
#endif 

#ifdef UNDEF_HAVE_INITGROUPS			
#undef HAVE_INITGROUPS
#endif



#if defined(__GNUC) && defined(__EXCEPTIONS)
#error "Please add -fno-exceptions to CXXFLAGS and reconfigure/recompile"
#endif

#if defined(_lint) && !defined(lint)
#define lint
#endif
#if SIZEOF_LONG_LONG > 4 && !defined(_LONG_LONG)
#define _LONG_LONG 1		
#endif


#if defined(_AIX) && defined(_LARGE_FILE_API)
#undef _LARGE_FILE_API
#undef __GNUG__
#endif


#ifndef stdin
#include <stdio.h>
#endif
#include <stdarg.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDDEF_H
#include <stddef.h>
#endif

#include <math.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_FLOAT_H
#include <float.h>
#endif
#ifdef HAVE_FENV_H
#include <fenv.h> 
#endif

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif


#if defined(_AIX) && defined(_LARGE_FILE_API)
#undef _LARGE_FILE_API
#undef __GNUG__
#endif


#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif 
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#if defined(__cplusplus) && defined(NO_CPLUSPLUS_ALLOCA)
#undef HAVE_ALLOCA
#undef HAVE_ALLOCA_H
#endif
#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#include <errno.h>				

#if defined(HAVE_CRYPT_H)
#include <crypt.h>
#endif


#ifdef CHECK_UNLIKELY
C_MODE_START
extern void init_my_likely(), end_my_likely(FILE *);
extern int my_likely_ok(const char *file_name, uint line);
extern int my_likely_fail(const char *file_name, uint line);
C_MODE_END

#define likely(A) ((A) ? (my_likely_ok(__FILE__, __LINE__),1) : (my_likely_fail(__FILE__, __LINE__), 0))
#define unlikely(A) ((A) ? (my_likely_fail(__FILE__, __LINE__),1) : (my_likely_ok(__FILE__, __LINE__), 0))

#define checked_likely(A) (A)
#define checked_unlikely(A) (A)
#else


#define likely(x)	__builtin_expect(((x) != 0),1)
#define unlikely(x)	__builtin_expect(((x) != 0),0)
#define checked_likely(x) likely(x)
#define checked_unlikely(x) unlikely(x)
#endif 


#include <assert.h>


#ifdef _some_old_compiler_that_does_not_understand_the_construct_below_
#define compile_time_assert(X)  do { } while(0)
#else
#define compile_time_assert(X)                                  \
  do                                                            \
  {                                                             \
    typedef char compile_time_assert[(X) ? 1 : -1] __attribute__((unused)); \
  } while(0)
#endif


#if defined (HPUX11) && defined(_LARGEFILE_SOURCE)
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif
#endif

#if defined(_HPUX_SOURCE) && defined(HAVE_SYS_STREAM_H)
#include <sys/stream.h>		
#define HAVE_ULONG
#endif
#if defined(HPUX10) && defined(_LARGEFILE64_SOURCE)

#undef setrlimit
#define setrlimit cma_setrlimit64
#endif

#if HAVE_MADVISE && !HAVE_DECL_MADVISE && defined(__cplusplus)
extern "C" int madvise(void *addr, size_t len, int behav);
#endif

#define QUOTE_ARG(x)		#x	
#define STRINGIFY_ARG(x) QUOTE_ARG(x)	


#ifdef I_AM_PARANOID
#define DONT_ALLOW_USER_CHANGE 1
#define DONT_USE_MYSQL_PWD 1
#endif


#if !defined(HAVE_BSD_SIGNALS) && !defined(HAVE_SIGACTION)
#define SIGNAL_HANDLER_RESET_ON_DELIVERY
#endif


#undef STDERR_FILENO

#ifndef SO_EXT
#ifdef _WIN32
#define SO_EXT ".dll"
#else
#define SO_EXT ".so"
#endif
#endif


#if defined(__GNUC__)

#define UNINIT_VAR(x) x= x
#elif defined(_lint) || defined(FORCE_INIT_OF_VARS)
#define UNINIT_VAR(x) x= 0
#else
#define UNINIT_VAR(x) x
#endif


#if defined(_lint) || defined(FORCE_INIT_OF_VARS)
#define LINT_INIT(x) x= 0
#else
#define LINT_INIT(x)
#endif

#if !defined(HAVE_UINT)
#undef HAVE_UINT
#define HAVE_UINT
typedef unsigned int uint;
typedef unsigned short ushort;
#endif

#define swap_variables(t, a, b) do { t dummy; dummy= a; a= b; b= dummy; } while(0)
#define MY_TEST(a) ((a) ? 1 : 0)
#define set_if_bigger(a,b)  do { if ((a) < (b)) (a)=(b); } while(0)
#define set_if_smaller(a,b) do { if ((a) > (b)) (a)=(b); } while(0)
#define set_bits(type, bit_count) (sizeof(type)*8 <= (bit_count) ? ~(type) 0 : ((((type) 1) << (bit_count)) - (type) 1))
#define test_all_bits(a,b) (((a) & (b)) == (b))
#define array_elements(A) ((uint) (sizeof(A)/sizeof(A[0])))


#ifndef TRUE
#define TRUE		(1)	
#define FALSE		(0)	
#endif

#include <my_compiler.h>


#if defined(USE_MYSYS_NEW) && ! defined(DONT_DECLARE_CXA_PURE_VIRTUAL)
C_MODE_START
int __cxa_pure_virtual () __attribute__ ((weak));
C_MODE_END
#endif


#if defined(DBUG_ON) && defined(DBUG_OFF)
#undef DBUG_OFF
#endif


#if (defined(FORCE_DBUG_OFF) || defined(_lint)) && !defined(DBUG_OFF)
#  define DBUG_OFF
#  ifdef DBUG_ON
#    undef DBUG_ON
#  endif
#endif

#ifdef DBUG_OFF
#undef EXTRA_DEBUG
#endif



typedef int	File;		
#ifdef _WIN32
typedef SOCKET my_socket;
#else
typedef int	my_socket;	
#define INVALID_SOCKET -1
#endif

#define sig_handler RETSIGTYPE
C_MODE_START
#ifdef HAVE_SIGHANDLER_T
#define sig_return sighandler_t
#else
typedef void (*sig_return)(void); 
#endif
C_MODE_END
#if defined(__GNUC__) && !defined(_lint)
typedef char	pchar;		
typedef char	puchar;		
typedef char	pbool;		
typedef short	pshort;		
typedef float	pfloat;		
#else
typedef int	pchar;		
typedef uint	puchar;		
typedef int	pbool;		
typedef int	pshort;		
typedef double	pfloat;		
#endif
C_MODE_START
typedef int	(*qsort_cmp)(const void *,const void *);
typedef int	(*qsort_cmp2)(void*, const void *,const void *);
C_MODE_END
#define qsort_t RETQSORTTYPE	
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
typedef SOCKET_SIZE_TYPE size_socket;

#ifndef SOCKOPT_OPTLEN_TYPE
#define SOCKOPT_OPTLEN_TYPE size_socket
#endif



#ifndef O_SHARE			
#define O_SHARE		0	
#ifndef O_BINARY
#define O_BINARY	0	
#endif
#ifndef FILE_BINARY
#define FILE_BINARY	O_BINARY 
#endif
#ifdef HAVE_FCNTL
#define HAVE_FCNTL_LOCK
#define F_TO_EOF	0L	
#endif
#endif 

#ifndef O_SEQUENTIAL
#define O_SEQUENTIAL	0
#endif
#ifndef O_SHORT_LIVED
#define O_SHORT_LIVED	0
#endif
#ifndef O_NOFOLLOW
#define O_NOFOLLOW      0
#endif
#ifndef O_CLOEXEC
#define O_CLOEXEC       0
#endif
#ifdef __GLIBC__
#define STR_O_CLOEXEC "e"
#else
#define STR_O_CLOEXEC ""
#endif
#ifndef SOCK_CLOEXEC
#define SOCK_CLOEXEC    0
#else
#define HAVE_SOCK_CLOEXEC
#endif


#ifdef __WIN__
#define _SH_DENYRWD     0x110    
#define _SH_DENYWRD     0x120    
#define _SH_DENYRDD     0x130    
#define _SH_DENYDEL     0x140    
#endif 



#define FN_LEN		256	
#define FN_HEADLEN	253	
#define FN_EXTLEN	20	
#define FN_REFLEN	512	
#define FN_EXTCHAR	'.'
#define FN_HOMELIB	'~'	
#define FN_CURLIB	'.'	
#define FN_PARENTDIR	".."	

#ifdef _WIN32
#define FN_LIBCHAR	'\\'
#define FN_LIBCHAR2	'/'
#define FN_DIRSEP       "/\\"               
#define FN_EXEEXT   ".exe"
#define FN_SOEXT    ".dll"
#define FN_ROOTDIR	"\\"
#define FN_DEVCHAR	':'
#define FN_NETWORK_DRIVES	
#define FN_NO_CASE_SENCE	
#else
#define FN_LIBCHAR	'/'
#define FN_LIBCHAR2	'/'
#define FN_DIRSEP       "/"     
#define FN_EXEEXT   ""
#define FN_SOEXT    ".so"
#define FN_ROOTDIR	"/"
#endif



#ifdef _WIN32
#define MY_FILE_MIN  2048
#else
#define MY_FILE_MIN  0
#endif

 
#ifdef _WIN32
#define MY_NFILE (16384 + MY_FILE_MIN)
#else
#define MY_NFILE 64
#endif

#ifndef OS_FILE_LIMIT
#define OS_FILE_LIMIT	UINT_MAX
#endif


#define IO_SIZE			4096U

#define MALLOC_OVERHEAD 8

	
#define ONCE_ALLOC_INIT		(uint) 4096
	
#define RECORD_CACHE_SIZE	(uint) (128*1024)
	
#define KEY_CACHE_SIZE		(uint) (128L*1024L*1024L)
	
#define KEY_CACHE_BLOCK_SIZE	(uint) 1024

	

#ifdef _WIN32
#define NO_DIR_LIBRARY		
#endif



#undef remove		
#ifndef __WIN__
#define closesocket(A)	close(A)
#endif

#if defined(_MSC_VER)
#if !defined(_WIN64)
inline double my_ulonglong2double(unsigned long long value)
{
  long long nr=(long long) value;
  if (nr >= 0)
    return (double) nr;
  return (18446744073709551616.0 + (double) nr);
}
#define ulonglong2double my_ulonglong2double
#define my_off_t2double  my_ulonglong2double
#endif 
inline unsigned long long my_double2ulonglong(double d)
{
  double t= d - (double) 0x8000000000000000ULL;

  if (t >= 0)
    return  ((unsigned long long) t) + 0x8000000000000000ULL;
  return (unsigned long long) d;
}
#define double2ulonglong my_double2ulonglong
#endif

#ifndef ulonglong2double
#define ulonglong2double(A) ((double) (ulonglong) (A))
#define my_off_t2double(A)  ((double) (my_off_t) (A))
#endif
#ifndef double2ulonglong
#define double2ulonglong(A) ((ulonglong) (double) (A))
#endif

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
#define ulong_to_double(X) ((double) (ulong) (X))

#ifndef STACK_DIRECTION
#error "please add -DSTACK_DIRECTION=1 or -1 to your CPPFLAGS"
#endif

#if !defined(HAVE_STRTOK_R)
#define strtok_r(A,B,C) strtok((A),(B))
#endif

#if SIZEOF_LONG_LONG >= 8
#define HAVE_LONG_LONG 1
#else
#error WHAT? sizeof(long long) < 8 ???
#endif



#if defined(HAVE_LONG_LONG) && !defined(LONGLONG_MIN)
#define LONGLONG_MIN	((long long) 0x8000000000000000LL)
#define LONGLONG_MAX	((long long) 0x7FFFFFFFFFFFFFFFLL)
#endif

#if defined(HAVE_LONG_LONG) && !defined(ULONGLONG_MAX)

#ifdef ULLONG_MAX
#define ULONGLONG_MAX  ULLONG_MAX
#else
#define ULONGLONG_MAX ((unsigned long long)(~0ULL))
#endif
#endif 

#define INT_MIN64       (~0x7FFFFFFFFFFFFFFFLL)
#define INT_MAX64       0x7FFFFFFFFFFFFFFFLL
#define INT_MIN32       (~0x7FFFFFFFL)
#define INT_MAX32       0x7FFFFFFFL
#define UINT_MAX32      0xFFFFFFFFL
#define INT_MIN24       (~0x007FFFFF)
#define INT_MAX24       0x007FFFFF
#define UINT_MAX24      0x00FFFFFF
#define INT_MIN16       (~0x7FFF)
#define INT_MAX16       0x7FFF
#define UINT_MAX16      0xFFFF
#define INT_MIN8        (~0x7F)
#define INT_MAX8        0x7F
#define UINT_MAX8       0xFF


#ifndef DBL_MIN
#define DBL_MIN		4.94065645841246544e-324
#define FLT_MIN		((float)1.40129846432481707e-45)
#endif
#ifndef DBL_MAX
#define DBL_MAX		1.79769313486231470e+308
#define FLT_MAX		((float)3.40282346638528860e+38)
#endif
#ifndef SIZE_T_MAX
#define SIZE_T_MAX      (~((size_t) 0))
#endif


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.7182818284590452354
#endif
#ifndef M_LN2
#define M_LN2 0.69314718055994530942
#endif


#if SIZEOF_CHARP == 4
typedef long		my_ptrdiff_t;
#else
typedef long long	my_ptrdiff_t;
#endif

#define MY_ALIGN(A,L)	   (((A) + (L) - 1) & ~((L) - 1))
#define MY_ALIGN_DOWN(A,L) ((A) & ~((L) - 1))
#define ALIGN_SIZE(A)	MY_ALIGN((A),sizeof(double))
#define ALIGN_MAX_UNIT  (sizeof(double))

#define ALIGN_PTR(A, t) ((t*) MY_ALIGN((A), sizeof(double)))
#define ADD_TO_PTR(ptr,size,type) (type) ((uchar*) (ptr)+size)
#define PTR_BYTE_DIFF(A,B) (my_ptrdiff_t) ((uchar*) (A) - (uchar*) (B))
#define PREV_BITS(type,A)	((type) (((type) 1 << (A)) -1))



#define my_offsetof(TYPE, MEMBER) PTR_BYTE_DIFF(&((TYPE *)0x10)->MEMBER, 0x10)

#define NullS		(char *) 0

#ifdef STDCALL
#undef STDCALL
#endif

#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif



#ifndef HAVE_UCHAR
typedef unsigned char	uchar;	
#endif

#ifndef HAVE_INT8
typedef signed char int8;       
#endif
#ifndef HAVE_UINT8
typedef unsigned char uint8;    
#endif
#ifndef HAVE_INT16
typedef short int16;
#endif
#ifndef HAVE_UINT16
typedef unsigned short uint16;
#endif
#if SIZEOF_INT == 4
#ifndef HAVE_INT32
typedef int int32;
#endif
#ifndef HAVE_UINT32
typedef unsigned int uint32;
#endif
#elif SIZEOF_LONG == 4
#ifndef HAVE_INT32
typedef long int32;
#endif
#ifndef HAVE_UINT32
typedef unsigned long uint32;
#endif
#else
#error Neither int or long is of 4 bytes width
#endif

#if !defined(HAVE_ULONG) && !defined(__USE_MISC)
typedef unsigned long	ulong;		  
#endif
#ifndef longlong_defined

#if defined(HAVE_LONG_LONG) && SIZEOF_LONG_LONG == 8
typedef unsigned long long int ulonglong; 
typedef long long int	longlong;
#else
typedef unsigned long	ulonglong;	  
typedef long		longlong;
#endif
#endif
#ifndef HAVE_INT64
typedef longlong int64;
#endif
#ifndef HAVE_UINT64
typedef ulonglong uint64;
#endif

#if defined(NO_CLIENT_LONG_LONG)
typedef unsigned long my_ulonglong;
#elif defined (__WIN__)
typedef unsigned __int64 my_ulonglong;
#else
typedef unsigned long long my_ulonglong;
#endif

#if SIZEOF_CHARP == SIZEOF_INT
typedef unsigned int intptr;
#elif SIZEOF_CHARP == SIZEOF_LONG
typedef unsigned long intptr;
#elif SIZEOF_CHARP == SIZEOF_LONG_LONG
typedef unsigned long long intptr;
#else
#error sizeof(void *) is neither sizeof(int) nor sizeof(long) nor sizeof(long long)
#endif

#define MY_ERRPTR ((void*)(intptr)1)

#if defined(_WIN32)
typedef unsigned long long my_off_t;
typedef unsigned long long os_off_t;
#else
typedef off_t os_off_t;
#if SIZEOF_OFF_T > 4
typedef ulonglong my_off_t;
#else
typedef unsigned long my_off_t;
#endif
#endif 
#define MY_FILEPOS_ERROR	(~(my_off_t) 0)


typedef ulonglong table_map;          


typedef const struct charset_info_st CHARSET_INFO;
typedef struct st_mysql_lex_string LEX_STRING;

#if defined(__WIN__)
#define socket_errno	WSAGetLastError()
#define SOCKET_EINTR	WSAEINTR
#define SOCKET_EAGAIN	WSAEINPROGRESS
#define SOCKET_ETIMEDOUT WSAETIMEDOUT
#define SOCKET_EWOULDBLOCK WSAEWOULDBLOCK
#define SOCKET_EADDRINUSE WSAEADDRINUSE
#define SOCKET_ECONNRESET WSAECONNRESET
#define SOCKET_ENFILE	ENFILE
#define SOCKET_EMFILE	EMFILE
#else 
#define socket_errno	errno
#define closesocket(A)	close(A)
#define SOCKET_EINTR	EINTR
#define SOCKET_EAGAIN	EAGAIN
#define SOCKET_EWOULDBLOCK EWOULDBLOCK
#define SOCKET_EADDRINUSE EADDRINUSE
#define SOCKET_ETIMEDOUT ETIMEDOUT
#define SOCKET_ECONNRESET ECONNRESET
#define SOCKET_ENFILE	ENFILE
#define SOCKET_EMFILE	EMFILE
#endif

#include <mysql/plugin.h>  

typedef ulong		myf;	

#define MYF(v)		(myf) (v)


#ifndef USING_X
#define reg1 register
#define reg2 register
#define reg3 register
#define reg4 register
#define reg5 register
#define reg6 register
#define reg7 register
#define reg8 register
#define reg9 register
#define reg10 register
#define reg11 register
#define reg12 register
#define reg13 register
#define reg14 register
#define reg15 register
#define reg16 register
#endif

#include <my_dbug.h>


#define YESNO(X) ((X) ? "yes" : "no")

#define MY_HOW_OFTEN_TO_ALARM	2	
#define MY_HOW_OFTEN_TO_WRITE	10000	

#include <my_byteorder.h>

#ifdef HAVE_CHARSET_utf8mb4
#define MYSQL_UNIVERSAL_CLIENT_CHARSET "utf8mb4"
#elif defined(HAVE_CHARSET_utf8)
#define MYSQL_UNIVERSAL_CLIENT_CHARSET "utf8"
#else
#define MYSQL_UNIVERSAL_CLIENT_CHARSET MYSQL_DEFAULT_CHARSET_NAME
#endif

#if defined(EMBEDDED_LIBRARY) && !defined(HAVE_EMBEDDED_PRIVILEGE_CONTROL)
#define NO_EMBEDDED_ACCESS_CHECKS
#endif

#ifdef _WIN32
#define dlsym(lib, name) (void*)GetProcAddress((HMODULE)lib, name)
#define dlopen(libname, unused) LoadLibraryEx(libname, NULL, 0)
#define RTLD_DEFAULT GetModuleHandle(NULL)
#define dlclose(lib) FreeLibrary((HMODULE)lib)
static inline char *dlerror(void)
{
  static char win_errormsg[2048];
  FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,
    0, GetLastError(), 0, win_errormsg, 2048, NULL);
  return win_errormsg;
}
#define HAVE_DLOPEN 1
#define HAVE_DLERROR 1
#endif

#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif

#ifdef HAVE_DLOPEN
#ifndef HAVE_DLERROR
#define dlerror() ""
#endif
#ifndef HAVE_DLADDR
#define dladdr(A, B) 0

typedef struct { const char *dli_fname, dli_fbase; } Dl_info;
#endif
#else
#define dlerror() "No support for dynamic loading (static build?)"
#define dlopen(A,B) 0
#define dlsym(A,B) 0
#define dlclose(A) 0
#define dladdr(A, B) 0

typedef struct { const char *dli_fname, dli_fbase; } Dl_info;
#endif


#ifdef __cplusplus
#include <new>
#endif


#define MY_INT32_NUM_DECIMAL_DIGITS 11


#define MY_INT64_NUM_DECIMAL_DIGITS 21

#ifdef __cplusplus
#include <limits> 
#endif


#define MY_MAX(a, b)	((a) > (b) ? (a) : (b))
#define MY_MIN(a, b)	((a) < (b) ? (a) : (b))

#define CMP_NUM(a,b)    (((a) < (b)) ? -1 : ((a) == (b)) ? 0 : 1)


#ifdef TARGET_OS_LINUX
#define NEED_EXPLICIT_SYNC_DIR 1
#else

#define WT_RWLOCKS_USE_MUTEXES 1
#endif

#if !defined(__cplusplus) && !defined(bool)
#define bool In_C_you_should_use_my_bool_instead()
#endif


#if !defined (__func__)
#if defined(__STDC_VERSION__) && __STDC_VERSION__ < 199901L
#  if __GNUC__ >= 2
#    define __func__ __FUNCTION__
#  else
#    define __func__ "<unknown>"
#  endif
#elif defined(_MSC_VER)
#  if _MSC_VER < 1300
#    define __func__ "<unknown>"
#  else
#    define __func__ __FUNCTION__
#  endif
#elif defined(__BORLANDC__)
#  define __func__ __FUNC__
#else
#  define __func__ "<unknown>"
#endif
#endif 


#ifndef MYSQL_PLUGIN_IMPORT
#if (defined(_WIN32) && defined(MYSQL_DYNAMIC_PLUGIN))
#define MYSQL_PLUGIN_IMPORT __declspec(dllimport)
#else
#define MYSQL_PLUGIN_IMPORT
#endif
#endif



#ifdef EMBEDDED_LIBRARY




#undef HAVE_SMEM				

#else
#define HAVE_REPLICATION
#define HAVE_EXTERNAL_CLIENT
#endif 


#if defined(_AIX) && defined(_LARGE_FILE_API)
#undef _LARGE_FILE_API
#undef __GNUG__
#endif


#if !defined(CPU_LEVEL1_DCACHE_LINESIZE) || CPU_LEVEL1_DCACHE_LINESIZE == 0
  #if defined(CPU_LEVEL1_DCACHE_LINESIZE) && CPU_LEVEL1_DCACHE_LINESIZE == 0
    #undef CPU_LEVEL1_DCACHE_LINESIZE
  #endif

  #if defined(__s390__)
    #define CPU_LEVEL1_DCACHE_LINESIZE 256
  #elif defined(__powerpc__) || defined(__aarch64__)
    #define CPU_LEVEL1_DCACHE_LINESIZE 128
  #else
    #define CPU_LEVEL1_DCACHE_LINESIZE 64
  #endif
#endif

#define FLOATING_POINT_DECIMALS 31


#ifdef MYSQL_SERVER
#define NOT_FIXED_DEC           DECIMAL_NOT_SPECIFIED
#else
#define NOT_FIXED_DEC           FLOATING_POINT_DECIMALS
#endif
#endif 
