







#ifndef _ft_global_h
#define _ft_global_h
#ifdef  __cplusplus
extern "C" {
#endif

#include <my_compare.h>

#define HA_FT_MAXBYTELEN 254
#define HA_FT_MAXCHARLEN (HA_FT_MAXBYTELEN/3)

#define DEFAULT_FTB_SYNTAX "+ -><()~*:\"\"&|"

typedef struct st_ft_info FT_INFO;
struct _ft_vft
{
  int       (*read_next)(FT_INFO *, char *);
  float     (*find_relevance)(FT_INFO *, uchar *, uint);
  void      (*close_search)(FT_INFO *);
  float     (*get_relevance)(FT_INFO *);
  void      (*reinit_search)(FT_INFO *);
};

typedef struct st_ft_info_ext FT_INFO_EXT;
struct _ft_vft_ext
{
  uint      (*get_version)();        
  ulonglong (*get_flags)();
  ulonglong (*get_docid)(FT_INFO_EXT *);
  ulonglong (*count_matches)(FT_INFO_EXT *);
};


#define FTS_ORDERED_RESULT                (1LL << 1)
#define FTS_DOCID_IN_RESULT               (1LL << 2)

#define FTS_DOC_ID_COL_NAME "FTS_DOC_ID"

#ifndef FT_CORE
struct st_ft_info
{
  struct _ft_vft *please; 
};

struct st_ft_info_ext
{
  struct _ft_vft     *please; 
  struct _ft_vft_ext *could_you;
};
#endif

extern const char *ft_stopword_file;
extern const char *ft_precompiled_stopwords[];

extern ulong ft_min_word_len;
extern ulong ft_max_word_len;
extern ulong ft_query_expansion_limit;
extern const char *ft_boolean_syntax;
extern struct st_mysql_ftparser ft_default_parser;

int ft_init_stopwords(void);
void ft_free_stopwords(void);

#define FT_NL     0
#define FT_BOOL   1
#define FT_SORTED 2
#define FT_EXPAND 4   

FT_INFO *ft_init_search(uint,void *, uint, uchar *, size_t,
                        CHARSET_INFO *, uchar *);
my_bool ft_boolean_check_syntax_string(const uchar *);



#define HA_FT_WTYPE  HA_KEYTYPE_FLOAT
#define HA_FT_WLEN   4
#define FT_SEGS      2

#define ft_sintXkorr(A)    mi_sint4korr(A)
#define ft_intXstore(T,A)  mi_int4store(T,A)

extern const HA_KEYSEG ft_keysegs[FT_SEGS];

typedef union {int32 i; float f;} FT_WEIGTH;

#ifdef  __cplusplus
}
#endif
#endif
