#ifndef SQL_RECORDS_H
#define SQL_RECORDS_H 


#ifdef USE_PRAGMA_INTERFACE
#pragma interface                      
#endif

struct st_join_table;
class handler;
struct TABLE;
class THD;
class SQL_SELECT;
class Copy_field;
class SORT_INFO;

struct READ_RECORD;

void end_read_record(READ_RECORD *info);



struct READ_RECORD
{
  typedef int (*Read_func)(READ_RECORD*);
  typedef void (*Unlock_row_func)(st_join_table *);
  typedef int (*Setup_func)(struct st_join_table*);

  TABLE *table;                                 
  
  TABLE **forms;                                
  Unlock_row_func unlock_row;
  Read_func read_record_func;
  THD *thd;
  SQL_SELECT *select;
  uint cache_records;
  uint ref_length,struct_length,reclength,rec_cache_size,error_offset;
  uint index;
  uchar *ref_pos;				
  uchar *record;
  uchar *rec_buf;                
  uchar	*cache,*cache_pos,*cache_end,*read_positions;
  struct st_sort_addon_field *addon_field;     
  struct st_io_cache *io_cache;
  bool print_error;
  void    (*unpack)(struct st_sort_addon_field *, uchar *, uchar *);

  int read_record() { return read_record_func(this); }

  
  Copy_field *copy_field;
  Copy_field *copy_field_end;
public:
  READ_RECORD() : table(NULL), cache(NULL) {}
  ~READ_RECORD() { end_read_record(this); }
};

bool init_read_record(READ_RECORD *info, THD *thd, TABLE *reg_form,
		      SQL_SELECT *select, SORT_INFO *sort,
                      int use_record_cache,
                      bool print_errors, bool disable_rr_cache);
bool init_read_record_idx(READ_RECORD *info, THD *thd, TABLE *table,
                          bool print_error, uint idx, bool reverse);

void rr_unlock_row(st_join_table *tab);

#endif 
