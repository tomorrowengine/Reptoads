



#ifndef _PCRE_H
#define _PCRE_H



#define PCRE_MAJOR          8
#define PCRE_MINOR          42
#define PCRE_PRERELEASE     
#define PCRE_DATE           2018-03-20



#if defined(_WIN32) && !defined(PCRE_STATIC)
#  ifndef PCRE_EXP_DECL
#    define PCRE_EXP_DECL  extern __declspec(dllimport)
#  endif
#  ifdef __cplusplus
#    ifndef PCRECPP_EXP_DECL
#      define PCRECPP_EXP_DECL  extern __declspec(dllimport)
#    endif
#    ifndef PCRECPP_EXP_DEFN
#      define PCRECPP_EXP_DEFN  __declspec(dllimport)
#    endif
#  endif
#endif



#ifndef PCRE_EXP_DECL
#  ifdef __cplusplus
#    define PCRE_EXP_DECL  extern "C"
#  else
#    define PCRE_EXP_DECL  extern
#  endif
#endif

#ifdef __cplusplus
#  ifndef PCRECPP_EXP_DECL
#    define PCRECPP_EXP_DECL  extern
#  endif
#  ifndef PCRECPP_EXP_DEFN
#    define PCRECPP_EXP_DEFN
#  endif
#endif



#include <stdlib.h>



#ifdef __cplusplus
extern "C" {
#endif



#define PCRE_CASELESS           0x00000001  
#define PCRE_MULTILINE          0x00000002  
#define PCRE_DOTALL             0x00000004  
#define PCRE_EXTENDED           0x00000008  
#define PCRE_ANCHORED           0x00000010  
#define PCRE_DOLLAR_ENDONLY     0x00000020  
#define PCRE_EXTRA              0x00000040  
#define PCRE_NOTBOL             0x00000080  
#define PCRE_NOTEOL             0x00000100  
#define PCRE_UNGREEDY           0x00000200  
#define PCRE_NOTEMPTY           0x00000400  
#define PCRE_UTF8               0x00000800  
#define PCRE_UTF16              0x00000800  
#define PCRE_UTF32              0x00000800  
#define PCRE_NO_AUTO_CAPTURE    0x00001000  
#define PCRE_NO_UTF8_CHECK      0x00002000  
#define PCRE_NO_UTF16_CHECK     0x00002000  
#define PCRE_NO_UTF32_CHECK     0x00002000  
#define PCRE_AUTO_CALLOUT       0x00004000  
#define PCRE_PARTIAL_SOFT       0x00008000  
#define PCRE_PARTIAL            0x00008000  


#define PCRE_NEVER_UTF          0x00010000  
#define PCRE_DFA_SHORTEST       0x00010000  


#define PCRE_NO_AUTO_POSSESS    0x00020000  
#define PCRE_DFA_RESTART        0x00020000  

#define PCRE_FIRSTLINE          0x00040000  
#define PCRE_DUPNAMES           0x00080000  
#define PCRE_NEWLINE_CR         0x00100000  
#define PCRE_NEWLINE_LF         0x00200000  
#define PCRE_NEWLINE_CRLF       0x00300000  
#define PCRE_NEWLINE_ANY        0x00400000  
#define PCRE_NEWLINE_ANYCRLF    0x00500000  
#define PCRE_BSR_ANYCRLF        0x00800000  
#define PCRE_BSR_UNICODE        0x01000000  
#define PCRE_JAVASCRIPT_COMPAT  0x02000000  
#define PCRE_NO_START_OPTIMIZE  0x04000000  
#define PCRE_NO_START_OPTIMISE  0x04000000  
#define PCRE_PARTIAL_HARD       0x08000000  
#define PCRE_NOTEMPTY_ATSTART   0x10000000  
#define PCRE_UCP                0x20000000  



#define PCRE_ERROR_NOMATCH          (-1)
#define PCRE_ERROR_NULL             (-2)
#define PCRE_ERROR_BADOPTION        (-3)
#define PCRE_ERROR_BADMAGIC         (-4)
#define PCRE_ERROR_UNKNOWN_OPCODE   (-5)
#define PCRE_ERROR_UNKNOWN_NODE     (-5)  
#define PCRE_ERROR_NOMEMORY         (-6)
#define PCRE_ERROR_NOSUBSTRING      (-7)
#define PCRE_ERROR_MATCHLIMIT       (-8)
#define PCRE_ERROR_CALLOUT          (-9)  
#define PCRE_ERROR_BADUTF8         (-10)  
#define PCRE_ERROR_BADUTF16        (-10)  
#define PCRE_ERROR_BADUTF32        (-10)  
#define PCRE_ERROR_BADUTF8_OFFSET  (-11)  
#define PCRE_ERROR_BADUTF16_OFFSET (-11)  
#define PCRE_ERROR_PARTIAL         (-12)
#define PCRE_ERROR_BADPARTIAL      (-13)
#define PCRE_ERROR_INTERNAL        (-14)
#define PCRE_ERROR_BADCOUNT        (-15)
#define PCRE_ERROR_DFA_UITEM       (-16)
#define PCRE_ERROR_DFA_UCOND       (-17)
#define PCRE_ERROR_DFA_UMLIMIT     (-18)
#define PCRE_ERROR_DFA_WSSIZE      (-19)
#define PCRE_ERROR_DFA_RECURSE     (-20)
#define PCRE_ERROR_RECURSIONLIMIT  (-21)
#define PCRE_ERROR_NULLWSLIMIT     (-22)  
#define PCRE_ERROR_BADNEWLINE      (-23)
#define PCRE_ERROR_BADOFFSET       (-24)
#define PCRE_ERROR_SHORTUTF8       (-25)
#define PCRE_ERROR_SHORTUTF16      (-25)  
#define PCRE_ERROR_RECURSELOOP     (-26)
#define PCRE_ERROR_JIT_STACKLIMIT  (-27)
#define PCRE_ERROR_BADMODE         (-28)
#define PCRE_ERROR_BADENDIANNESS   (-29)
#define PCRE_ERROR_DFA_BADRESTART  (-30)
#define PCRE_ERROR_JIT_BADOPTION   (-31)
#define PCRE_ERROR_BADLENGTH       (-32)
#define PCRE_ERROR_UNSET           (-33)



#define PCRE_UTF8_ERR0               0
#define PCRE_UTF8_ERR1               1
#define PCRE_UTF8_ERR2               2
#define PCRE_UTF8_ERR3               3
#define PCRE_UTF8_ERR4               4
#define PCRE_UTF8_ERR5               5
#define PCRE_UTF8_ERR6               6
#define PCRE_UTF8_ERR7               7
#define PCRE_UTF8_ERR8               8
#define PCRE_UTF8_ERR9               9
#define PCRE_UTF8_ERR10             10
#define PCRE_UTF8_ERR11             11
#define PCRE_UTF8_ERR12             12
#define PCRE_UTF8_ERR13             13
#define PCRE_UTF8_ERR14             14
#define PCRE_UTF8_ERR15             15
#define PCRE_UTF8_ERR16             16
#define PCRE_UTF8_ERR17             17
#define PCRE_UTF8_ERR18             18
#define PCRE_UTF8_ERR19             19
#define PCRE_UTF8_ERR20             20
#define PCRE_UTF8_ERR21             21
#define PCRE_UTF8_ERR22             22  



#define PCRE_UTF16_ERR0              0
#define PCRE_UTF16_ERR1              1
#define PCRE_UTF16_ERR2              2
#define PCRE_UTF16_ERR3              3
#define PCRE_UTF16_ERR4              4  



#define PCRE_UTF32_ERR0              0
#define PCRE_UTF32_ERR1              1
#define PCRE_UTF32_ERR2              2  
#define PCRE_UTF32_ERR3              3



#define PCRE_INFO_OPTIONS            0
#define PCRE_INFO_SIZE               1
#define PCRE_INFO_CAPTURECOUNT       2
#define PCRE_INFO_BACKREFMAX         3
#define PCRE_INFO_FIRSTBYTE          4
#define PCRE_INFO_FIRSTCHAR          4  
#define PCRE_INFO_FIRSTTABLE         5
#define PCRE_INFO_LASTLITERAL        6
#define PCRE_INFO_NAMEENTRYSIZE      7
#define PCRE_INFO_NAMECOUNT          8
#define PCRE_INFO_NAMETABLE          9
#define PCRE_INFO_STUDYSIZE         10
#define PCRE_INFO_DEFAULT_TABLES    11
#define PCRE_INFO_OKPARTIAL         12
#define PCRE_INFO_JCHANGED          13
#define PCRE_INFO_HASCRORLF         14
#define PCRE_INFO_MINLENGTH         15
#define PCRE_INFO_JIT               16
#define PCRE_INFO_JITSIZE           17
#define PCRE_INFO_MAXLOOKBEHIND     18
#define PCRE_INFO_FIRSTCHARACTER    19
#define PCRE_INFO_FIRSTCHARACTERFLAGS 20
#define PCRE_INFO_REQUIREDCHAR      21
#define PCRE_INFO_REQUIREDCHARFLAGS 22
#define PCRE_INFO_MATCHLIMIT        23
#define PCRE_INFO_RECURSIONLIMIT    24
#define PCRE_INFO_MATCH_EMPTY       25



#define PCRE_CONFIG_UTF8                    0
#define PCRE_CONFIG_NEWLINE                 1
#define PCRE_CONFIG_LINK_SIZE               2
#define PCRE_CONFIG_POSIX_MALLOC_THRESHOLD  3
#define PCRE_CONFIG_MATCH_LIMIT             4
#define PCRE_CONFIG_STACKRECURSE            5
#define PCRE_CONFIG_UNICODE_PROPERTIES      6
#define PCRE_CONFIG_MATCH_LIMIT_RECURSION   7
#define PCRE_CONFIG_BSR                     8
#define PCRE_CONFIG_JIT                     9
#define PCRE_CONFIG_UTF16                  10
#define PCRE_CONFIG_JITTARGET              11
#define PCRE_CONFIG_UTF32                  12
#define PCRE_CONFIG_PARENS_LIMIT           13



#define PCRE_STUDY_JIT_COMPILE                0x0001
#define PCRE_STUDY_JIT_PARTIAL_SOFT_COMPILE   0x0002
#define PCRE_STUDY_JIT_PARTIAL_HARD_COMPILE   0x0004
#define PCRE_STUDY_EXTRA_NEEDED               0x0008



#define PCRE_EXTRA_STUDY_DATA             0x0001
#define PCRE_EXTRA_MATCH_LIMIT            0x0002
#define PCRE_EXTRA_CALLOUT_DATA           0x0004
#define PCRE_EXTRA_TABLES                 0x0008
#define PCRE_EXTRA_MATCH_LIMIT_RECURSION  0x0010
#define PCRE_EXTRA_MARK                   0x0020
#define PCRE_EXTRA_EXECUTABLE_JIT         0x0040



struct real_pcre8_or_16;          
typedef struct real_pcre8_or_16 pcre;

struct real_pcre8_or_16;          
typedef struct real_pcre8_or_16 pcre16;

struct real_pcre32;               
typedef struct real_pcre32 pcre32;

struct real_pcre_jit_stack;       
typedef struct real_pcre_jit_stack pcre_jit_stack;

struct real_pcre16_jit_stack;     
typedef struct real_pcre16_jit_stack pcre16_jit_stack;

struct real_pcre32_jit_stack;     
typedef struct real_pcre32_jit_stack pcre32_jit_stack;


#ifndef PCRE_UCHAR16
#define PCRE_UCHAR16 unsigned short
#endif

#ifndef PCRE_SPTR16
#define PCRE_SPTR16 const PCRE_UCHAR16 *
#endif


#ifndef PCRE_UCHAR32
#define PCRE_UCHAR32 unsigned int
#endif

#ifndef PCRE_SPTR32
#define PCRE_SPTR32 const PCRE_UCHAR32 *
#endif



#ifndef PCRE_SPTR
#define PCRE_SPTR const char *
#endif



typedef struct pcre_extra {
  unsigned long int flags;        
  void *study_data;               
  unsigned long int match_limit;  
  void *callout_data;             
  const unsigned char *tables;    
  unsigned long int match_limit_recursion; 
  unsigned char **mark;           
  void *executable_jit;           
} pcre_extra;



typedef struct pcre16_extra {
  unsigned long int flags;        
  void *study_data;               
  unsigned long int match_limit;  
  void *callout_data;             
  const unsigned char *tables;    
  unsigned long int match_limit_recursion; 
  PCRE_UCHAR16 **mark;            
  void *executable_jit;           
} pcre16_extra;



typedef struct pcre32_extra {
  unsigned long int flags;        
  void *study_data;               
  unsigned long int match_limit;  
  void *callout_data;             
  const unsigned char *tables;    
  unsigned long int match_limit_recursion; 
  PCRE_UCHAR32 **mark;            
  void *executable_jit;           
} pcre32_extra;



typedef struct pcre_callout_block {
  int          version;           
  
  int          callout_number;    
  int         *offset_vector;     
  PCRE_SPTR    subject;           
  int          subject_length;    
  int          start_match;       
  int          current_position;  
  int          capture_top;       
  int          capture_last;      
  void        *callout_data;      
  
  int          pattern_position;  
  int          next_item_length;  
  
  const unsigned char *mark;      
  
} pcre_callout_block;



typedef struct pcre16_callout_block {
  int          version;           
  
  int          callout_number;    
  int         *offset_vector;     
  PCRE_SPTR16  subject;           
  int          subject_length;    
  int          start_match;       
  int          current_position;  
  int          capture_top;       
  int          capture_last;      
  void        *callout_data;      
  
  int          pattern_position;  
  int          next_item_length;  
  
  const PCRE_UCHAR16 *mark;       
  
} pcre16_callout_block;



typedef struct pcre32_callout_block {
  int          version;           
  
  int          callout_number;    
  int         *offset_vector;     
  PCRE_SPTR32  subject;           
  int          subject_length;    
  int          start_match;       
  int          current_position;  
  int          capture_top;       
  int          capture_last;      
  void        *callout_data;      
  
  int          pattern_position;  
  int          next_item_length;  
  
  const PCRE_UCHAR32 *mark;       
  
} pcre32_callout_block;



#ifndef VPCOMPAT
PCRE_EXP_DECL void *(*pcre_malloc)(size_t);
PCRE_EXP_DECL void  (*pcre_free)(void *);
PCRE_EXP_DECL void *(*pcre_stack_malloc)(size_t);
PCRE_EXP_DECL void  (*pcre_stack_free)(void *);
PCRE_EXP_DECL int   (*pcre_callout)(pcre_callout_block *);
PCRE_EXP_DECL int   (*pcre_stack_guard)(void);

PCRE_EXP_DECL void *(*pcre16_malloc)(size_t);
PCRE_EXP_DECL void  (*pcre16_free)(void *);
PCRE_EXP_DECL void *(*pcre16_stack_malloc)(size_t);
PCRE_EXP_DECL void  (*pcre16_stack_free)(void *);
PCRE_EXP_DECL int   (*pcre16_callout)(pcre16_callout_block *);
PCRE_EXP_DECL int   (*pcre16_stack_guard)(void);

PCRE_EXP_DECL void *(*pcre32_malloc)(size_t);
PCRE_EXP_DECL void  (*pcre32_free)(void *);
PCRE_EXP_DECL void *(*pcre32_stack_malloc)(size_t);
PCRE_EXP_DECL void  (*pcre32_stack_free)(void *);
PCRE_EXP_DECL int   (*pcre32_callout)(pcre32_callout_block *);
PCRE_EXP_DECL int   (*pcre32_stack_guard)(void);
#else   
PCRE_EXP_DECL void *pcre_malloc(size_t);
PCRE_EXP_DECL void  pcre_free(void *);
PCRE_EXP_DECL void *pcre_stack_malloc(size_t);
PCRE_EXP_DECL void  pcre_stack_free(void *);
PCRE_EXP_DECL int   pcre_callout(pcre_callout_block *);
PCRE_EXP_DECL int   pcre_stack_guard(void);

PCRE_EXP_DECL void *pcre16_malloc(size_t);
PCRE_EXP_DECL void  pcre16_free(void *);
PCRE_EXP_DECL void *pcre16_stack_malloc(size_t);
PCRE_EXP_DECL void  pcre16_stack_free(void *);
PCRE_EXP_DECL int   pcre16_callout(pcre16_callout_block *);
PCRE_EXP_DECL int   pcre16_stack_guard(void);

PCRE_EXP_DECL void *pcre32_malloc(size_t);
PCRE_EXP_DECL void  pcre32_free(void *);
PCRE_EXP_DECL void *pcre32_stack_malloc(size_t);
PCRE_EXP_DECL void  pcre32_stack_free(void *);
PCRE_EXP_DECL int   pcre32_callout(pcre32_callout_block *);
PCRE_EXP_DECL int   pcre32_stack_guard(void);
#endif  



typedef pcre_jit_stack *(*pcre_jit_callback)(void *);
typedef pcre16_jit_stack *(*pcre16_jit_callback)(void *);
typedef pcre32_jit_stack *(*pcre32_jit_callback)(void *);



PCRE_EXP_DECL pcre *pcre_compile(const char *, int, const char **, int *,
                  const unsigned char *);
PCRE_EXP_DECL pcre16 *pcre16_compile(PCRE_SPTR16, int, const char **, int *,
                  const unsigned char *);
PCRE_EXP_DECL pcre32 *pcre32_compile(PCRE_SPTR32, int, const char **, int *,
                  const unsigned char *);
PCRE_EXP_DECL pcre *pcre_compile2(const char *, int, int *, const char **,
                  int *, const unsigned char *);
PCRE_EXP_DECL pcre16 *pcre16_compile2(PCRE_SPTR16, int, int *, const char **,
                  int *, const unsigned char *);
PCRE_EXP_DECL pcre32 *pcre32_compile2(PCRE_SPTR32, int, int *, const char **,
                  int *, const unsigned char *);
PCRE_EXP_DECL int  pcre_config(int, void *);
PCRE_EXP_DECL int  pcre16_config(int, void *);
PCRE_EXP_DECL int  pcre32_config(int, void *);
PCRE_EXP_DECL int  pcre_copy_named_substring(const pcre *, const char *,
                  int *, int, const char *, char *, int);
PCRE_EXP_DECL int  pcre16_copy_named_substring(const pcre16 *, PCRE_SPTR16,
                  int *, int, PCRE_SPTR16, PCRE_UCHAR16 *, int);
PCRE_EXP_DECL int  pcre32_copy_named_substring(const pcre32 *, PCRE_SPTR32,
                  int *, int, PCRE_SPTR32, PCRE_UCHAR32 *, int);
PCRE_EXP_DECL int  pcre_copy_substring(const char *, int *, int, int,
                  char *, int);
PCRE_EXP_DECL int  pcre16_copy_substring(PCRE_SPTR16, int *, int, int,
                  PCRE_UCHAR16 *, int);
PCRE_EXP_DECL int  pcre32_copy_substring(PCRE_SPTR32, int *, int, int,
                  PCRE_UCHAR32 *, int);
PCRE_EXP_DECL int  pcre_dfa_exec(const pcre *, const pcre_extra *,
                  const char *, int, int, int, int *, int , int *, int);
PCRE_EXP_DECL int  pcre16_dfa_exec(const pcre16 *, const pcre16_extra *,
                  PCRE_SPTR16, int, int, int, int *, int , int *, int);
PCRE_EXP_DECL int  pcre32_dfa_exec(const pcre32 *, const pcre32_extra *,
                  PCRE_SPTR32, int, int, int, int *, int , int *, int);
PCRE_EXP_DECL int  pcre_exec(const pcre *, const pcre_extra *, PCRE_SPTR,
                   int, int, int, int *, int);
PCRE_EXP_DECL int  pcre16_exec(const pcre16 *, const pcre16_extra *,
                   PCRE_SPTR16, int, int, int, int *, int);
PCRE_EXP_DECL int  pcre32_exec(const pcre32 *, const pcre32_extra *,
                   PCRE_SPTR32, int, int, int, int *, int);
PCRE_EXP_DECL int  pcre_jit_exec(const pcre *, const pcre_extra *,
                   PCRE_SPTR, int, int, int, int *, int,
                   pcre_jit_stack *);
PCRE_EXP_DECL int  pcre16_jit_exec(const pcre16 *, const pcre16_extra *,
                   PCRE_SPTR16, int, int, int, int *, int,
                   pcre16_jit_stack *);
PCRE_EXP_DECL int  pcre32_jit_exec(const pcre32 *, const pcre32_extra *,
                   PCRE_SPTR32, int, int, int, int *, int,
                   pcre32_jit_stack *);
PCRE_EXP_DECL void pcre_free_substring(const char *);
PCRE_EXP_DECL void pcre16_free_substring(PCRE_SPTR16);
PCRE_EXP_DECL void pcre32_free_substring(PCRE_SPTR32);
PCRE_EXP_DECL void pcre_free_substring_list(const char **);
PCRE_EXP_DECL void pcre16_free_substring_list(PCRE_SPTR16 *);
PCRE_EXP_DECL void pcre32_free_substring_list(PCRE_SPTR32 *);
PCRE_EXP_DECL int  pcre_fullinfo(const pcre *, const pcre_extra *, int,
                  void *);
PCRE_EXP_DECL int  pcre16_fullinfo(const pcre16 *, const pcre16_extra *, int,
                  void *);
PCRE_EXP_DECL int  pcre32_fullinfo(const pcre32 *, const pcre32_extra *, int,
                  void *);
PCRE_EXP_DECL int  pcre_get_named_substring(const pcre *, const char *,
                  int *, int, const char *, const char **);
PCRE_EXP_DECL int  pcre16_get_named_substring(const pcre16 *, PCRE_SPTR16,
                  int *, int, PCRE_SPTR16, PCRE_SPTR16 *);
PCRE_EXP_DECL int  pcre32_get_named_substring(const pcre32 *, PCRE_SPTR32,
                  int *, int, PCRE_SPTR32, PCRE_SPTR32 *);
PCRE_EXP_DECL int  pcre_get_stringnumber(const pcre *, const char *);
PCRE_EXP_DECL int  pcre16_get_stringnumber(const pcre16 *, PCRE_SPTR16);
PCRE_EXP_DECL int  pcre32_get_stringnumber(const pcre32 *, PCRE_SPTR32);
PCRE_EXP_DECL int  pcre_get_stringtable_entries(const pcre *, const char *,
                  char **, char **);
PCRE_EXP_DECL int  pcre16_get_stringtable_entries(const pcre16 *, PCRE_SPTR16,
                  PCRE_UCHAR16 **, PCRE_UCHAR16 **);
PCRE_EXP_DECL int  pcre32_get_stringtable_entries(const pcre32 *, PCRE_SPTR32,
                  PCRE_UCHAR32 **, PCRE_UCHAR32 **);
PCRE_EXP_DECL int  pcre_get_substring(const char *, int *, int, int,
                  const char **);
PCRE_EXP_DECL int  pcre16_get_substring(PCRE_SPTR16, int *, int, int,
                  PCRE_SPTR16 *);
PCRE_EXP_DECL int  pcre32_get_substring(PCRE_SPTR32, int *, int, int,
                  PCRE_SPTR32 *);
PCRE_EXP_DECL int  pcre_get_substring_list(const char *, int *, int,
                  const char ***);
PCRE_EXP_DECL int  pcre16_get_substring_list(PCRE_SPTR16, int *, int,
                  PCRE_SPTR16 **);
PCRE_EXP_DECL int  pcre32_get_substring_list(PCRE_SPTR32, int *, int,
                  PCRE_SPTR32 **);
PCRE_EXP_DECL const unsigned char *pcre_maketables(void);
PCRE_EXP_DECL const unsigned char *pcre16_maketables(void);
PCRE_EXP_DECL const unsigned char *pcre32_maketables(void);
PCRE_EXP_DECL int  pcre_refcount(pcre *, int);
PCRE_EXP_DECL int  pcre16_refcount(pcre16 *, int);
PCRE_EXP_DECL int  pcre32_refcount(pcre32 *, int);
PCRE_EXP_DECL pcre_extra *pcre_study(const pcre *, int, const char **);
PCRE_EXP_DECL pcre16_extra *pcre16_study(const pcre16 *, int, const char **);
PCRE_EXP_DECL pcre32_extra *pcre32_study(const pcre32 *, int, const char **);
PCRE_EXP_DECL void pcre_free_study(pcre_extra *);
PCRE_EXP_DECL void pcre16_free_study(pcre16_extra *);
PCRE_EXP_DECL void pcre32_free_study(pcre32_extra *);
PCRE_EXP_DECL const char *pcre_version(void);
PCRE_EXP_DECL const char *pcre16_version(void);
PCRE_EXP_DECL const char *pcre32_version(void);


PCRE_EXP_DECL int  pcre_pattern_to_host_byte_order(pcre *, pcre_extra *,
                  const unsigned char *);
PCRE_EXP_DECL int  pcre16_pattern_to_host_byte_order(pcre16 *, pcre16_extra *,
                  const unsigned char *);
PCRE_EXP_DECL int  pcre32_pattern_to_host_byte_order(pcre32 *, pcre32_extra *,
                  const unsigned char *);
PCRE_EXP_DECL int  pcre16_utf16_to_host_byte_order(PCRE_UCHAR16 *,
                  PCRE_SPTR16, int, int *, int);
PCRE_EXP_DECL int  pcre32_utf32_to_host_byte_order(PCRE_UCHAR32 *,
                  PCRE_SPTR32, int, int *, int);



PCRE_EXP_DECL pcre_jit_stack *pcre_jit_stack_alloc(int, int);
PCRE_EXP_DECL pcre16_jit_stack *pcre16_jit_stack_alloc(int, int);
PCRE_EXP_DECL pcre32_jit_stack *pcre32_jit_stack_alloc(int, int);
PCRE_EXP_DECL void pcre_jit_stack_free(pcre_jit_stack *);
PCRE_EXP_DECL void pcre16_jit_stack_free(pcre16_jit_stack *);
PCRE_EXP_DECL void pcre32_jit_stack_free(pcre32_jit_stack *);
PCRE_EXP_DECL void pcre_assign_jit_stack(pcre_extra *,
                  pcre_jit_callback, void *);
PCRE_EXP_DECL void pcre16_assign_jit_stack(pcre16_extra *,
                  pcre16_jit_callback, void *);
PCRE_EXP_DECL void pcre32_assign_jit_stack(pcre32_extra *,
                  pcre32_jit_callback, void *);
PCRE_EXP_DECL void pcre_jit_free_unused_memory(void);
PCRE_EXP_DECL void pcre16_jit_free_unused_memory(void);
PCRE_EXP_DECL void pcre32_jit_free_unused_memory(void);

#ifdef __cplusplus
}  
#endif

#endif 
