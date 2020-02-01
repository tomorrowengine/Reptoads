

#ifndef MY_CONFIG_H
#define MY_CONFIG_H
#define DOT_FRM_VERSION 6

#define STDC_HEADERS 1












#define HAVE_FCNTL_H 1

#define HAVE_FLOAT_H 1







#define HAVE_LIMITS_H 1


#define HAVE_LOCALE_H 1
#define HAVE_MALLOC_H 1
#define HAVE_MEMORY_H 1







#define HAVE_STDDEF_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STDARG_H 1

#define HAVE_STRING_H 1




















#define HAVE_SYS_STAT_H 1


#define HAVE_SYS_TIMEB_H 1


#define HAVE_SYS_TYPES_H 1









#define HAVE_TIME_H 1


#define HAVE_VARARGS_H 1
#define HAVE_SYS_UTIME_H 1























#define HAVE_ACCESS 1


#define HAVE_ALLOCA 1

















#define HAVE_FTIME 1

#define HAVE_GETCWD 1











#define HAVE_GMTIME_R 1




#define HAVE_LDIV 1

#define HAVE_LOCALTIME_R 1









#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1





#define HAVE_PERROR 1






















#define HAVE_PUTENV 1



#define HAVE_RENAME 1


#define HAVE_SELECT 1

#define HAVE_SETLOCALE 1






#define HAVE_SNPRINTF 1

#define HAVE_STRERROR 1
#define HAVE_STRCOLL 1
#define HAVE_STRNLEN 1
#define HAVE_STRPBRK 1
#define HAVE_STRTOK_R 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOUL 1
#define HAVE_STRTOULL 1
#define HAVE_TELL 1


#define HAVE_TIME 1


#define HAVE_VIO_READ_BUFF 1

#define HAVE_VSNPRINTF 1

#define HAVE_TZNAME 1








#define HAVE_IPV6 1



#define STRUCT_TIMESPEC_HAS_TV_SEC 1
#define STRUCT_TIMESPEC_HAS_TV_NSEC 1

#define USE_MB 1
#define USE_MB_IDENT 1








#ifdef __APPLE__
  
  #if(__LP64__)
    #define SIZEOF_LONG 8
  #else
    #define SIZEOF_LONG 4
  #endif
  #define SIZEOF_VOIDP   SIZEOF_LONG
  #define SIZEOF_CHARP   SIZEOF_LONG
  #define SIZEOF_SIZE_T  SIZEOF_LONG
#else

#define SIZEOF_LONG   4
#define SIZEOF_VOIDP  8
#define SIZEOF_CHARP  8
#define SIZEOF_SIZE_T 8
#endif

#define HAVE_LONG 1
#define HAVE_CHARP 1
#define SIZEOF_INT 4
#define HAVE_INT 1
#define SIZEOF_LONG_LONG 8
#define HAVE_LONG_LONG 1
#define SIZEOF_OFF_T 4
#define HAVE_OFF_T 1























#define SOCKET_SIZE_TYPE int



#define MAX_INDEXES 64

#define QSORT_TYPE_IS_VOID 1
#define RETQSORTTYPE void

#define RETSIGTYPE void
#define VOID_SIGHANDLER 1

#define STRUCT_RLIMIT struct rlimit

#ifdef __APPLE__
  #if __BIG_ENDIAN
    #define WORDS_BIGENDIAN 1
  #endif
#else

#endif



#if !(C_HAS_inline)
#ifndef __cplusplus
# define inline __inline
#endif
#endif




#define HAVE_WCTYPE_H 1
#define HAVE_WCHAR_H 1













#define HAVE_STRCASECMP 1





















#define NO_FCNTL_NONBLOCK 1
#define NO_ALARM 1


#define _LARGEFILE_SOURCE 1




#define STACK_DIRECTION -1

#define SYSTEM_TYPE "Win64"
#define MACHINE_TYPE "AMD64"
#define DEFAULT_MACHINE "x64"


#define SIGNAL_WITH_VIO_CLOSE 1


#define S_IROTH _S_IREAD
#define S_IFIFO _S_IFIFO


#define sigset_t int
#define mode_t int
#define SIGQUIT SIGTERM
#define SIGPIPE SIGINT
#define popen _popen
#define pclose _pclose
#define ssize_t SSIZE_T
#define strcasecmp _stricmp
#define strncasecmp _strnicmp

#define strtok_r strtok_s
#define strtoll _strtoi64
#define strtoull _strtoui64

#if defined(_MSC_VER) && (_MSC_VER > 1800)
#define tzname _tzname
#define P_tmpdir "C:\\TEMP"
#endif
#if defined(_MSC_VER) && (_MSC_VER > 1310)
# define HAVE_SETENV
#define setenv(a,b,c) _putenv_s(a,b)
#endif
#define PSAPI_VERSION 1     


#ifdef __WIN__
#define NOMINMAX 1
#endif


#define LOCAL_INFILE_MODE_OFF  0
#define LOCAL_INFILE_MODE_ON   1
#define LOCAL_INFILE_MODE_AUTO 2
#define ENABLED_LOCAL_INFILE LOCAL_INFILE_MODE_AUTO

#define ENABLED_PROFILING 1

#define USE_SYMDIR 1


#define MYSQL_DEFAULT_CHARSET_NAME "latin1"
#define MYSQL_DEFAULT_COLLATION_NAME "latin1_swedish_ci"

#define USE_MB 1
#define USE_MB_IDENT 1


#define FN_NO_CASE_SENSE 1

#define HAVE_CHARSET_armscii8 1
#define HAVE_CHARSET_ascii 1
#define HAVE_CHARSET_big5 1
#define HAVE_CHARSET_cp1250 1
#define HAVE_CHARSET_cp1251 1
#define HAVE_CHARSET_cp1256 1
#define HAVE_CHARSET_cp1257 1
#define HAVE_CHARSET_cp850 1
#define HAVE_CHARSET_cp852 1 
#define HAVE_CHARSET_cp866 1
#define HAVE_CHARSET_cp932 1
#define HAVE_CHARSET_dec8 1
#define HAVE_CHARSET_eucjpms 1
#define HAVE_CHARSET_euckr 1
#define HAVE_CHARSET_gb2312 1
#define HAVE_CHARSET_gbk 1
#define HAVE_CHARSET_geostd8 1
#define HAVE_CHARSET_greek 1
#define HAVE_CHARSET_hebrew 1
#define HAVE_CHARSET_hp8 1
#define HAVE_CHARSET_keybcs2 1
#define HAVE_CHARSET_koi8r 1
#define HAVE_CHARSET_koi8u 1
#define HAVE_CHARSET_latin1 1
#define HAVE_CHARSET_latin2 1
#define HAVE_CHARSET_latin5 1
#define HAVE_CHARSET_latin7 1
#define HAVE_CHARSET_macce 1
#define HAVE_CHARSET_macroman 1
#define HAVE_CHARSET_sjis 1
#define HAVE_CHARSET_swe7 1
#define HAVE_CHARSET_tis620 1
#define HAVE_CHARSET_ucs2 1
#define HAVE_CHARSET_ujis 1
#define HAVE_CHARSET_utf8mb4 1

#define HAVE_CHARSET_utf8 1
#define HAVE_CHARSET_utf16 1
#define HAVE_CHARSET_utf32 1
#define HAVE_UCA_COLLATIONS 1
#define HAVE_COMPRESS 1




#define HAVE_SPATIAL 1
#define HAVE_RTREE_KEYS 1
#define HAVE_QUERY_CACHE 1
#define BIG_TABLES 1


#define WITH_INNOBASE_STORAGE_ENGINE 1
#define WITH_PARTITION_STORAGE_ENGINE 1
#define WITH_PERFSCHEMA_STORAGE_ENGINE 1
#define WITH_ARIA_STORAGE_ENGINE 1
#define USE_ARIA_FOR_TMP_TABLES 1

#define DEFAULT_MYSQL_HOME "C:/Program Files/MariaDB 10.3"
#define SHAREDIR "share"
#define DEFAULT_BASEDIR "C:/Program Files/MariaDB 10.3"
#define MYSQL_DATADIR "C:/Program Files/MariaDB 10.3/data"
#define DEFAULT_CHARSET_HOME "C:/Program Files/MariaDB 10.3"
#define PLUGINDIR "C:/Program Files/MariaDB 10.3/lib/plugin"

#define DEFAULT_TMPDIR P_tmpdir



#define MYSQL_VERSION_MAJOR 10
#define MYSQL_VERSION_MINOR 3
#define MYSQL_VERSION_PATCH 14
#define MYSQL_VERSION_EXTRA ""

#define PACKAGE "mysql"
#define PACKAGE_BUGREPORT ""
#define PACKAGE_NAME "MySQL Server"
#define PACKAGE_STRING "MySQL Server 10.3.14"
#define PACKAGE_TARNAME "mysql"
#define PACKAGE_VERSION "10.3.14"
#define VERSION "10.3.14"
#define PROTOCOL_VERSION 10

#define MALLOC_LIBRARY "system"



#define SIZEOF_TIME_T 8
#define TIME_T_UNSIGNED 1

#ifndef EMBEDDED_LIBRARY



#endif

#if !defined(__STDC_FORMAT_MACROS)
#define __STDC_FORMAT_MACROS
#endif  

#endif
