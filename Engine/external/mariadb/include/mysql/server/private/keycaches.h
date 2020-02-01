#ifndef KEYCACHES_INCLUDED
#define KEYCACHES_INCLUDED



#include "sql_list.h"
#include <keycache.h>
#include <rpl_filter.h>

extern "C"
{
  typedef int (*process_key_cache_t) (const char *, KEY_CACHE *, void *);
}

class NAMED_ILINK;

class NAMED_ILIST: public I_List<NAMED_ILINK>
{
  public:
  void delete_elements(void (*free_element)(const char*, uchar*));
  bool delete_element(const char *name, size_t length, void (*free_element)(const char*, uchar*));
};


extern LEX_CSTRING default_key_cache_base;
extern KEY_CACHE zero_key_cache;
extern NAMED_ILIST key_caches;

KEY_CACHE *create_key_cache(const char *name, size_t length);
KEY_CACHE *get_key_cache(const LEX_CSTRING *cache_name);
KEY_CACHE *get_or_create_key_cache(const char *name, size_t length);
void free_key_cache(const char *name, KEY_CACHE *key_cache);
bool process_key_caches(process_key_cache_t func, void *param);


extern LEX_CSTRING default_rpl_filter_base;
extern NAMED_ILIST rpl_filters;

Rpl_filter *create_rpl_filter(const char *name, size_t length);
Rpl_filter *get_rpl_filter(LEX_CSTRING *filter_name);
Rpl_filter *get_or_create_rpl_filter(const char *name, size_t length);
void free_rpl_filter(const char *name, Rpl_filter *filter);
void free_all_rpl_filters(void);

#endif 
