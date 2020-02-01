#ifndef SQL_SORT_INCLUDED
#define SQL_SORT_INCLUDED



#include "my_base.h"                            
#include <my_sys.h>                             
#include "queues.h"

typedef struct st_buffpek BUFFPEK;

struct SORT_FIELD;
class Field;
struct TABLE;



#define MERGEBUFF		7
#define MERGEBUFF2		15



typedef struct st_sort_addon_field
{
  
  Field *field;          
  uint   offset;         
  uint   null_offset;    
  uint   length;         
  uint8  null_bit;       
} SORT_ADDON_FIELD;

struct BUFFPEK_COMPARE_CONTEXT
{
  qsort_cmp2 key_compare;
  void *key_compare_arg;
};


class Sort_param {
public:
  uint rec_length;            
  uint sort_length;           
  uint ref_length;            
  uint res_length;            
  uint max_keys_per_buffer;   
  uint min_dupl_count;
  ha_rows max_rows;           
  ha_rows examined_rows;      
  TABLE *sort_form;           
  SORT_FIELD *local_sortorder;
  SORT_FIELD *end;
  SORT_ADDON_FIELD *addon_field; 
  LEX_STRING addon_buf;          

  uchar *unique_buff;
  bool not_killable;
  char* tmp_buffer;
  
  qsort2_cmp compare;
  BUFFPEK_COMPARE_CONTEXT cmp_context;

  Sort_param()
  {
    memset(this, 0, sizeof(*this));
  }
  void init_for_filesort(uint sortlen, TABLE *table,
                         ulong max_length_for_sort_data,
                         ha_rows maxrows, bool sort_positions);
};


int merge_many_buff(Sort_param *param, uchar *sort_buffer,
		    BUFFPEK *buffpek,
		    uint *maxbuffer, IO_CACHE *t_file);
ulong read_to_buffer(IO_CACHE *fromfile,BUFFPEK *buffpek,
                     uint sort_length);
bool merge_buffers(Sort_param *param,IO_CACHE *from_file,
                   IO_CACHE *to_file, uchar *sort_buffer,
                   BUFFPEK *lastbuff,BUFFPEK *Fb,
                   BUFFPEK *Tb,int flag);
int merge_index(Sort_param *param, uchar *sort_buffer,
		BUFFPEK *buffpek, uint maxbuffer,
		IO_CACHE *tempfile, IO_CACHE *outfile);
void reuse_freed_buff(QUEUE *queue, BUFFPEK *reuse, uint key_length);

#endif 
