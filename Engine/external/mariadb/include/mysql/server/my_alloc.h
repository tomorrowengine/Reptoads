



#ifndef _my_alloc_h
#define _my_alloc_h

#define ALLOC_MAX_BLOCK_TO_DROP			4096
#define ALLOC_MAX_BLOCK_USAGE_BEFORE_DROP	10

#ifdef __cplusplus
extern "C" {
#endif

typedef struct st_used_mem
{				   
  struct st_used_mem *next;	   
  size_t left;                     
  size_t size;                     
} USED_MEM;


typedef struct st_mem_root
{
  USED_MEM *free;                  
  USED_MEM *used;                  
  USED_MEM *pre_alloc;             
  
  size_t min_malloc;
  size_t block_size;               
  size_t total_alloc;
  unsigned int block_num;          
  
  unsigned int first_block_usage;

  void (*error_handler)(void);
  const char *name;
} MEM_ROOT;

#ifdef  __cplusplus
}
#endif

#endif
