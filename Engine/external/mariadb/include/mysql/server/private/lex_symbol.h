




#ifndef _lex_symbol_h
#define _lex_symbol_h

struct st_sym_group;

typedef struct st_symbol {
  const char *name;
  uint	tok;
  uint length;
  struct st_sym_group *group;
} SYMBOL;

typedef struct st_lex_symbol
{
  SYMBOL *symbol;
  char   *str;
  uint   length;
} LEX_SYMBOL;

typedef struct st_sym_group {
  const char *name;
  const char *needed_define;
} SYM_GROUP;

extern SYM_GROUP sym_group_common;
extern SYM_GROUP sym_group_geom;
extern SYM_GROUP sym_group_rtree;

#endif 
