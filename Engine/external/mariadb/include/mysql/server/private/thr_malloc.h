

#ifndef THR_MALLOC_INCLUDED
#define THR_MALLOC_INCLUDED

typedef struct st_mem_root MEM_ROOT;

void init_sql_alloc(MEM_ROOT *root, const char *area_name, uint block_size,
                    uint pre_alloc_size, myf my_flags);
char *sql_strmake_with_convert(THD *thd, const char *str, size_t arg_length,
			       CHARSET_INFO *from_cs,
			       size_t max_res_length,
			       CHARSET_INFO *to_cs, size_t *result_length);

#endif 
