

#ifndef _my_plugin_ftparser_h
#define _my_plugin_ftparser_h
#include "plugin.h"

#ifdef __cplusplus
extern "C" {
#endif



#define MYSQL_FTPARSER_INTERFACE_VERSION 0x0100


enum enum_ftparser_mode
{

  MYSQL_FTPARSER_SIMPLE_MODE= 0,


  MYSQL_FTPARSER_WITH_STOPWORDS= 1,


  MYSQL_FTPARSER_FULL_BOOLEAN_INFO= 2
};



enum enum_ft_token_type
{
  FT_TOKEN_EOF= 0,
  FT_TOKEN_WORD= 1,
  FT_TOKEN_LEFT_PAREN= 2,
  FT_TOKEN_RIGHT_PAREN= 3,
  FT_TOKEN_STOPWORD= 4
};



typedef struct st_mysql_ftparser_boolean_info
{
  enum enum_ft_token_type type;
  int yesno;
  int weight_adjust;
  char wasign;
  char trunc;
  
  char prev;
  char *quot;
} MYSQL_FTPARSER_BOOLEAN_INFO;


#define MYSQL_FTFLAGS_NEED_COPY 1



typedef struct st_mysql_ftparser_param
{
  int (*mysql_parse)(struct st_mysql_ftparser_param *,
                     const char *doc, int doc_len);
  int (*mysql_add_word)(struct st_mysql_ftparser_param *,
                        const char *word, int word_len,
                        MYSQL_FTPARSER_BOOLEAN_INFO *boolean_info);
  void *ftparser_state;
  void *mysql_ftparam;
  const struct charset_info_st *cs;
  const char *doc;
  int length;
  unsigned int flags;
  enum enum_ftparser_mode mode;
} MYSQL_FTPARSER_PARAM;



struct st_mysql_ftparser
{
  int interface_version;
  int (*parse)(MYSQL_FTPARSER_PARAM *param);
  int (*init)(MYSQL_FTPARSER_PARAM *param);
  int (*deinit)(MYSQL_FTPARSER_PARAM *param);
};


#ifdef __cplusplus
}
#endif

#endif

