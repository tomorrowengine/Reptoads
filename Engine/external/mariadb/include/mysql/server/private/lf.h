

#ifndef INCLUDE_LF_INCLUDED
#define INCLUDE_LF_INCLUDED

#include <my_atomic.h>
#include <my_cpu.h>

C_MODE_START


#define LF_DYNARRAY_LEVEL_LENGTH 256
#define LF_DYNARRAY_LEVELS       4

typedef struct {
  void * volatile level[LF_DYNARRAY_LEVELS];
  uint size_of_element;
} LF_DYNARRAY;

typedef int (*lf_dynarray_func)(void *, void *);

void lf_dynarray_init(LF_DYNARRAY *array, uint element_size);
void lf_dynarray_destroy(LF_DYNARRAY *array);

void *lf_dynarray_value(LF_DYNARRAY *array, uint idx);
void *lf_dynarray_lvalue(LF_DYNARRAY *array, uint idx);
int lf_dynarray_iterate(LF_DYNARRAY *array, lf_dynarray_func func, void *arg);



#define LF_PINBOX_PINS 4
#define LF_PURGATORY_SIZE 100

typedef void lf_pinbox_free_func(void *, void *, void*);

typedef struct {
  LF_DYNARRAY pinarray;
  lf_pinbox_free_func *free_func;
  void *free_func_arg;
  uint free_ptr_offset;
  uint32 volatile pinstack_top_ver;         
  uint32 volatile pins_in_array;            
} LF_PINBOX;

typedef struct {
  void * volatile pin[LF_PINBOX_PINS];
  LF_PINBOX *pinbox;
  void  **stack_ends_here;
  void  *purgatory;
  uint32 purgatory_count;
  uint32 volatile link;
  
  char pad[CPU_LEVEL1_DCACHE_LINESIZE];
} LF_PINS;


#define lf_pin(PINS, PIN, ADDR)                                \
  do {                                                          \
    compile_time_assert(PIN < LF_PINBOX_PINS);                  \
    my_atomic_storeptr(&(PINS)->pin[PIN], (ADDR));              \
  } while(0)

#define lf_unpin(PINS, PIN)        lf_pin(PINS, PIN, NULL)
#define lf_assert_pin(PINS, PIN)   assert((PINS)->pin[PIN] != 0)
#define lf_assert_unpin(PINS, PIN) assert((PINS)->pin[PIN] == 0)

void lf_pinbox_init(LF_PINBOX *pinbox, uint free_ptr_offset,
                    lf_pinbox_free_func *free_func, void * free_func_arg);
void lf_pinbox_destroy(LF_PINBOX *pinbox);

LF_PINS *lf_pinbox_get_pins(LF_PINBOX *pinbox);
void lf_pinbox_put_pins(LF_PINS *pins);
void lf_pinbox_free(LF_PINS *pins, void *addr);



typedef struct st_lf_allocator {
  LF_PINBOX pinbox;
  uchar * volatile top;
  uint element_size;
  uint32 volatile mallocs;
  void (*constructor)(uchar *); 
  void (*destructor)(uchar *);  
} LF_ALLOCATOR;

void lf_alloc_init(LF_ALLOCATOR *allocator, uint size, uint free_ptr_offset);
void lf_alloc_destroy(LF_ALLOCATOR *allocator);
uint lf_alloc_pool_count(LF_ALLOCATOR *allocator);

#define lf_alloc_free(PINS, PTR)       lf_pinbox_free((PINS), (PTR))
#define lf_alloc_get_pins(A)           lf_pinbox_get_pins(&(A)->pinbox)
#define lf_alloc_put_pins(PINS)        lf_pinbox_put_pins(PINS)
#define lf_alloc_direct_free(ALLOC, ADDR) \
  do {                                    \
    if ((ALLOC)->destructor)              \
      (ALLOC)->destructor((uchar*) ADDR); \
    my_free(ADDR);                        \
  } while(0)

void *lf_alloc_new(LF_PINS *pins);

C_MODE_END


#include <hash.h>

C_MODE_START

typedef struct st_lf_hash LF_HASH;
typedef void (*lf_hash_initializer)(LF_HASH *hash, void *dst, const void *src);

#define LF_HASH_UNIQUE 1


extern const int LF_HASH_OVERHEAD;

struct st_lf_hash {
  LF_DYNARRAY array;                    
  LF_ALLOCATOR alloc;                   
  my_hash_get_key get_key;              
  lf_hash_initializer initializer;      
  my_hash_function hash_function;       
  CHARSET_INFO *charset;                
  uint key_offset, key_length;          
  uint element_size;                    
  uint flags;                           
  int32 volatile size;                  
  int32 volatile count;                 
};

void lf_hash_init(LF_HASH *hash, uint element_size, uint flags,
                  uint key_offset, uint key_length, my_hash_get_key get_key,
                  CHARSET_INFO *charset);
void lf_hash_destroy(LF_HASH *hash);
int lf_hash_insert(LF_HASH *hash, LF_PINS *pins, const void *data);
void *lf_hash_search(LF_HASH *hash, LF_PINS *pins, const void *key, uint keylen);
void *lf_hash_search_using_hash_value(LF_HASH *hash, LF_PINS *pins,
                                      my_hash_value_type hash_value,
                                      const void *key, uint keylen);
int lf_hash_delete(LF_HASH *hash, LF_PINS *pins, const void *key, uint keylen);
int lf_hash_iterate(LF_HASH *hash, LF_PINS *pins,
                    my_hash_walk_action action, void *argument);

#define lf_hash_get_pins(HASH)       lf_alloc_get_pins(&(HASH)->alloc)
#define lf_hash_put_pins(PINS)       lf_pinbox_put_pins(PINS)
#define lf_hash_search_unpin(PINS)   lf_unpin((PINS), 2)


C_MODE_END

#endif

