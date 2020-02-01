


#ifndef LEX_STRING_INCLUDED
#define LEX_STRING_INCLUDED

typedef struct st_mysql_const_lex_string LEX_CSTRING;



static inline bool lex_string_cmp(CHARSET_INFO *charset, const LEX_CSTRING *a,
                                  const LEX_CSTRING *b)
{
  return my_strcasecmp(charset, a->str, b->str);
}



static inline bool cmp(const LEX_CSTRING *a, const LEX_CSTRING *b)
{
  return (a->length != b->length ||
          memcmp(a->str, b->str, a->length));
}



static inline bool lex_string_eq(const LEX_CSTRING *a, const LEX_CSTRING *b)
{
  if (a->length != b->length)
    return 0;                                   
  return strcasecmp(a->str, b->str) == 0;
}



static inline bool lex_string_eq(const LEX_CSTRING *a, const char *b, size_t b_length)
{
  if (a->length != b_length)
    return 0;                                   
  return strcasecmp(a->str, b) == 0;
}

#endif 
