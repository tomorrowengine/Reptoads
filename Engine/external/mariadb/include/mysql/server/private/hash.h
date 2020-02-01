



#ifndef _hash_h
#define _hash_h

#include "my_sys.h"                             


#ifdef	__cplusplus
extern "C" {
#endif


#define HASH_OVERHEAD (sizeof(char*)*2)


#define HASH_UNIQUE     1       
#define HASH_THREAD_SPECIFIC 2  

typedef uint32 my_hash_value_type;
typedef uchar *(*my_hash_get_key)(const uchar *,size_t*,my_bool);
typedef my_hash_value_type (*my_hash_function)(CHARSET_INFO *,
                                               const uchar *, size_t);
typedef void (*my_hash_free_key)(void *);
typedef my_bool (*my_hash_walk_action)(void *,void *);

typedef struct st_hash {
  size_t key_offset,key_length;		
  size_t blength;
  ulong records;
  uint flags;
  DYNAMIC_ARRAY array;				
  my_hash_get_key get_key;
  my_hash_function hash_function;
  void (*free)(void *);
  CHARSET_INFO *charset;
} HASH;


typedef uint HASH_SEARCH_STATE;

#define my_hash_init(A,B,C,D,E,F,G,H) my_hash_init2(A,0,B,C,D,E,F,0,G,H)
my_bool my_hash_init2(HASH *hash, uint growth_size, CHARSET_INFO *charset,
                      ulong default_array_elements, size_t key_offset,
                      size_t key_length, my_hash_get_key get_key,
                      my_hash_function hash_function,
                      void (*free_element)(void*),
                      uint flags);
void my_hash_free(HASH *tree);
void my_hash_reset(HASH *hash);
uchar *my_hash_element(HASH *hash, size_t idx);
uchar *my_hash_search(const HASH *info, const uchar *key, size_t length);
uchar *my_hash_search_using_hash_value(const HASH *info,
                                       my_hash_value_type hash_value,
                                       const uchar *key, size_t length);
my_hash_value_type my_hash_sort(CHARSET_INFO *cs,
                                const uchar *key, size_t length);
#define my_calc_hash(A, B, C) my_hash_sort((A)->charset, B, C)
uchar *my_hash_first(const HASH *info, const uchar *key, size_t length,
                     HASH_SEARCH_STATE *state);
uchar *my_hash_first_from_hash_value(const HASH *info,
                                     my_hash_value_type hash_value,
                                     const uchar *key,
                                     size_t length,
                                     HASH_SEARCH_STATE *state);
uchar *my_hash_next(const HASH *info, const uchar *key, size_t length,
                    HASH_SEARCH_STATE *state);
my_bool my_hash_insert(HASH *info, const uchar *data);
my_bool my_hash_delete(HASH *hash, uchar *record);
my_bool my_hash_update(HASH *hash, uchar *record, uchar *old_key,
                       size_t old_key_length);
void my_hash_replace(HASH *hash, HASH_SEARCH_STATE *state, uchar *new_row);
my_bool my_hash_check(HASH *hash); 
my_bool my_hash_iterate(HASH *hash, my_hash_walk_action action, void *argument);

#define my_hash_clear(H) bzero((char*) (H), sizeof(*(H)))
#define my_hash_inited(H) ((H)->blength != 0)
#define my_hash_init_opt(A,B,C,D,E,F,G,H) \
          (!my_hash_inited(A) && my_hash_init(A,B,C,D,E,F,G,H))

#ifdef	__cplusplus
}
#endif
#endif
