




#ifndef _opt_range_h
#define _opt_range_h

#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif

#include "records.h"                            
#include "queues.h"                             
#include "filesort.h"                           


#include "sql_class.h"                          
#include "set_var.h"                            

class JOIN;
class Item_sum;

struct KEY_PART {
  uint16           key,part;
  
  uint16           store_length, length;
  uint8            null_bit;
  
  uint8 flag;
  Field            *field;
  Field::imagetype image_type;
};


class RANGE_OPT_PARAM;


class SEL_ARG :public Sql_alloc
{
  static int sel_cmp(Field *field, uchar *a, uchar *b, uint8 a_flag,
                     uint8 b_flag);
public:
  uint8 min_flag,max_flag,maybe_flag;
  uint8 part;					
  uint8 maybe_null;
  
  uint16 max_part_no;
  
  uint32 elements;
  
  ulong use_count;

  Field *field;
  uchar *min_value,*max_value;			

  
  SEL_ARG *left,*right;   
  SEL_ARG *next,*prev;    
  SEL_ARG *parent;        
  SEL_ARG *next_key_part;
  enum leaf_color { BLACK,RED } color;
  enum Type { IMPOSSIBLE, MAYBE, MAYBE_KEY, KEY_RANGE } type;

  enum { MAX_SEL_ARGS = 16000 };

  SEL_ARG() {}
  SEL_ARG(SEL_ARG &);
  SEL_ARG(Field *,const uchar *, const uchar *);
  SEL_ARG(Field *field, uint8 part, uchar *min_value, uchar *max_value,
	  uint8 min_flag, uint8 max_flag, uint8 maybe_flag);
  SEL_ARG(enum Type type_arg)
    :min_flag(0), max_part_no(0) ,
     elements(1),use_count(1),left(0),right(0),
     next_key_part(0), color(BLACK), type(type_arg)
  {}
  
  inline bool is_same(const SEL_ARG *arg) const
  {
    if (type != arg->type || part != arg->part)
      return false;
    if (type != KEY_RANGE)
      return true;
    return cmp_min_to_min(arg) == 0 && cmp_max_to_max(arg) == 0;
  }
  
  bool all_same(const SEL_ARG *arg) const
  {
    if (type != arg->type || part != arg->part)
      return false;
    if (type != KEY_RANGE)
      return true;
    if (arg == this)
      return true;
    const SEL_ARG *cmp_arg= arg->first();
    const SEL_ARG *cur_arg= first();
    for (; cur_arg && cmp_arg && cur_arg->is_same(cmp_arg);
         cur_arg= cur_arg->next, cmp_arg= cmp_arg->next) ;
    if (cur_arg || cmp_arg)
      return false;
    return true;
  }
  inline void merge_flags(SEL_ARG *arg) { maybe_flag|=arg->maybe_flag; }
  inline void maybe_smaller() { maybe_flag=1; }
  
  inline bool is_null_interval() { return maybe_null && max_value[0] == 1; }
  inline int cmp_min_to_min(const SEL_ARG* arg) const
  {
    return sel_cmp(field,min_value, arg->min_value, min_flag, arg->min_flag);
  }
  inline int cmp_min_to_max(const SEL_ARG* arg) const
  {
    return sel_cmp(field,min_value, arg->max_value, min_flag, arg->max_flag);
  }
  inline int cmp_max_to_max(const SEL_ARG* arg) const
  {
    return sel_cmp(field,max_value, arg->max_value, max_flag, arg->max_flag);
  }
  inline int cmp_max_to_min(const SEL_ARG* arg) const
  {
    return sel_cmp(field,max_value, arg->min_value, max_flag, arg->min_flag);
  }
  SEL_ARG *clone_and(THD *thd, SEL_ARG* arg)
  {						
    uchar *new_min,*new_max;
    uint8 flag_min,flag_max;
    if (cmp_min_to_min(arg) >= 0)
    {
      new_min=min_value; flag_min=min_flag;
    }
    else
    {
      new_min=arg->min_value; flag_min=arg->min_flag; 
    }
    if (cmp_max_to_max(arg) <= 0)
    {
      new_max=max_value; flag_max=max_flag;
    }
    else
    {
      new_max=arg->max_value; flag_max=arg->max_flag;
    }
    return new (thd->mem_root) SEL_ARG(field, part, new_min, new_max, flag_min,
                                       flag_max,
                                       MY_TEST(maybe_flag && arg->maybe_flag));
  }
  SEL_ARG *clone_first(SEL_ARG *arg)
  {						
    return new SEL_ARG(field,part, min_value, arg->min_value,
		       min_flag, arg->min_flag & NEAR_MIN ? 0 : NEAR_MAX,
		       maybe_flag | arg->maybe_flag);
  }
  SEL_ARG *clone_last(SEL_ARG *arg)
  {						
    return new SEL_ARG(field, part, min_value, arg->max_value,
		       min_flag, arg->max_flag, maybe_flag | arg->maybe_flag);
  }
  SEL_ARG *clone(RANGE_OPT_PARAM *param, SEL_ARG *new_parent, SEL_ARG **next);

  bool copy_min(SEL_ARG* arg)
  {						
    if (cmp_min_to_min(arg) > 0)
    {
      min_value=arg->min_value; min_flag=arg->min_flag;
      if ((max_flag & (NO_MAX_RANGE | NO_MIN_RANGE)) ==
	  (NO_MAX_RANGE | NO_MIN_RANGE))
	return 1;				
    }
    maybe_flag|=arg->maybe_flag;
    return 0;
  }
  bool copy_max(SEL_ARG* arg)
  {						
    if (cmp_max_to_max(arg) <= 0)
    {
      max_value=arg->max_value; max_flag=arg->max_flag;
      if ((max_flag & (NO_MAX_RANGE | NO_MIN_RANGE)) ==
	  (NO_MAX_RANGE | NO_MIN_RANGE))
	return 1;				
    }
    maybe_flag|=arg->maybe_flag;
    return 0;
  }

  void copy_min_to_min(SEL_ARG *arg)
  {
    min_value=arg->min_value; min_flag=arg->min_flag;
  }
  void copy_min_to_max(SEL_ARG *arg)
  {
    max_value=arg->min_value;
    max_flag=arg->min_flag & NEAR_MIN ? 0 : NEAR_MAX;
  }
  void copy_max_to_min(SEL_ARG *arg)
  {
    min_value=arg->max_value;
    min_flag=arg->max_flag & NEAR_MAX ? 0 : NEAR_MIN;
  }
  
  int store_min(uint length, uchar **min_key,uint min_key_flag)
  {
    
    if ((min_flag & GEOM_FLAG) ||
        (!(min_flag & NO_MIN_RANGE) &&
	!(min_key_flag & (NO_MIN_RANGE | NEAR_MIN))))
    {
      if (maybe_null && *min_value)
      {
	**min_key=1;
	bzero(*min_key+1,length-1);
      }
      else
	memcpy(*min_key,min_value,length);
      (*min_key)+= length;
      return 1;
    }
    return 0;
  }
  
  int store_max(uint length, uchar **max_key, uint max_key_flag)
  {
    if (!(max_flag & NO_MAX_RANGE) &&
	!(max_key_flag & (NO_MAX_RANGE | NEAR_MAX)))
    {
      if (maybe_null && *max_value)
      {
	**max_key=1;
	bzero(*max_key+1,length-1);
      }
      else
	memcpy(*max_key,max_value,length);
      (*max_key)+= length;
      return 1;
    }
    return 0;
  }

  
  int store_min_key(KEY_PART *key,
                    uchar **range_key,
                    uint *range_key_flag,
                    uint last_part)
  {
    SEL_ARG *key_tree= first();
    uint res= key_tree->store_min(key[key_tree->part].store_length,
                                  range_key, *range_key_flag);
    *range_key_flag|= key_tree->min_flag;
    if (key_tree->next_key_part &&
	key_tree->next_key_part->type == SEL_ARG::KEY_RANGE &&
        key_tree->part != last_part &&
	key_tree->next_key_part->part == key_tree->part+1 &&
	!(*range_key_flag & (NO_MIN_RANGE | NEAR_MIN)))
      res+= key_tree->next_key_part->store_min_key(key,
                                                   range_key,
                                                   range_key_flag,
                                                   last_part);
    return res;
  }

  
  int store_max_key(KEY_PART *key,
                    uchar **range_key,
                    uint *range_key_flag,
                    uint last_part)
  {
    SEL_ARG *key_tree= last();
    uint res=key_tree->store_max(key[key_tree->part].store_length,
                                 range_key, *range_key_flag);
    (*range_key_flag)|= key_tree->max_flag;
    if (key_tree->next_key_part &&
	key_tree->next_key_part->type == SEL_ARG::KEY_RANGE &&
        key_tree->part != last_part &&
	key_tree->next_key_part->part == key_tree->part+1 &&
	!(*range_key_flag & (NO_MAX_RANGE | NEAR_MAX)))
      res+= key_tree->next_key_part->store_max_key(key,
                                                   range_key,
                                                   range_key_flag,
                                                   last_part);
    return res;
  }

  SEL_ARG *insert(SEL_ARG *key);
  SEL_ARG *tree_delete(SEL_ARG *key);
  SEL_ARG *find_range(SEL_ARG *key);
  SEL_ARG *rb_insert(SEL_ARG *leaf);
  friend SEL_ARG *rb_delete_fixup(SEL_ARG *root,SEL_ARG *key, SEL_ARG *par);
#ifdef EXTRA_DEBUG
  friend int test_rb_tree(SEL_ARG *element,SEL_ARG *parent);
  void test_use_count(SEL_ARG *root);
#endif
  SEL_ARG *first();
  const SEL_ARG *first() const;
  SEL_ARG *last();
  void make_root();
  inline bool simple_key()
  {
    return !next_key_part && elements == 1;
  }
  void increment_use_count(long count)
  {
    if (next_key_part)
    {
      next_key_part->use_count+=count;
      count*= (next_key_part->use_count-count);
      for (SEL_ARG *pos=next_key_part->first(); pos ; pos=pos->next)
	if (pos->next_key_part)
	  pos->increment_use_count(count);
    }
  }
  void incr_refs()
  {
    increment_use_count(1);
    use_count++;
  }
  void incr_refs_all()
  {
    for (SEL_ARG *pos=first(); pos ; pos=pos->next)
    {
      pos->increment_use_count(1);
    }
    use_count++;
  }
  void free_tree()
  {
    for (SEL_ARG *pos=first(); pos ; pos=pos->next)
      if (pos->next_key_part)
      {
	pos->next_key_part->use_count--;
	pos->next_key_part->free_tree();
      }
  }

  inline SEL_ARG **parent_ptr()
  {
    return parent->left == this ? &parent->left : &parent->right;
  }


  

  bool is_singlepoint()
  {
    
    if (min_flag || max_flag)
      return FALSE;
    uchar *min_val= min_value;
    uchar *max_val= max_value;

    if (maybe_null)
    {
      
      if (*min_val != *max_val)
        return FALSE;

      if (*min_val)
        return TRUE; 
      min_val++;
      max_val++;
    }
    return !field->key_cmp(min_val, max_val);
  }
  SEL_ARG *clone_tree(RANGE_OPT_PARAM *param);
};


class SEL_ARG_IMPOSSIBLE: public SEL_ARG
{
public:
  SEL_ARG_IMPOSSIBLE(Field *field)
   :SEL_ARG(field, 0, 0)
  {
    type= SEL_ARG::IMPOSSIBLE;
  }
};


class RANGE_OPT_PARAM
{
public:
  THD	*thd;   
  TABLE *table; 
  table_map prev_tables;
  table_map read_tables;
  table_map current_table; 

  
  KEY_PART *key_parts;
  KEY_PART *key_parts_end;
  MEM_ROOT *mem_root; 
  MEM_ROOT *old_root; 
  
  uint keys;

  
  bool using_real_indexes;

  
  bool remove_jump_scans;

  
  bool remove_false_where_parts;

  
  uint real_keynr[MAX_KEY];

  
  uchar *min_key;
  uchar *max_key;

  
  uint alloced_sel_args;

  bool force_default_mrr;
  KEY_PART *key[MAX_KEY]; 

  bool statement_should_be_aborted() const
  {
    return
      thd->is_fatal_error ||
      thd->is_error() ||
      alloced_sel_args > SEL_ARG::MAX_SEL_ARGS;
  }
};


class Explain_quick_select;

class QUICK_RANGE :public Sql_alloc {
 public:
  uchar *min_key,*max_key;
  uint16 min_length,max_length,flag;
  key_part_map min_keypart_map, 
               max_keypart_map; 
#ifdef HAVE_valgrind
  uint16 dummy;					
#endif
  QUICK_RANGE();				
  QUICK_RANGE(THD *thd, const uchar *min_key_arg, uint min_length_arg,
              key_part_map min_keypart_map_arg,
	      const uchar *max_key_arg, uint max_length_arg,
              key_part_map max_keypart_map_arg,
	      uint flag_arg)
    : min_key((uchar*) thd->memdup(min_key_arg, min_length_arg + 1)),
      max_key((uchar*) thd->memdup(max_key_arg, max_length_arg + 1)),
      min_length((uint16) min_length_arg),
      max_length((uint16) max_length_arg),
      flag((uint16) flag_arg),
      min_keypart_map(min_keypart_map_arg),
      max_keypart_map(max_keypart_map_arg)
    {
#ifdef HAVE_valgrind
      dummy=0;
#endif
    }

  
  void make_min_endpoint(key_range *kr, uint prefix_length, 
                         key_part_map keypart_map) {
    make_min_endpoint(kr);
    kr->length= MY_MIN(kr->length, prefix_length);
    kr->keypart_map&= keypart_map;
  }
  
  
  void make_min_endpoint(key_range *kr) {
    kr->key= (const uchar*)min_key;
    kr->length= min_length;
    kr->keypart_map= min_keypart_map;
    kr->flag= ((flag & NEAR_MIN) ? HA_READ_AFTER_KEY :
               (flag & EQ_RANGE) ? HA_READ_KEY_EXACT : HA_READ_KEY_OR_NEXT);
  }

  
  void make_max_endpoint(key_range *kr, uint prefix_length, 
                         key_part_map keypart_map) {
    make_max_endpoint(kr);
    kr->length= MY_MIN(kr->length, prefix_length);
    kr->keypart_map&= keypart_map;
  }

  
  void make_max_endpoint(key_range *kr) {
    kr->key= (const uchar*)max_key;
    kr->length= max_length;
    kr->keypart_map= max_keypart_map;
    
    kr->flag= (flag & NEAR_MAX ? HA_READ_BEFORE_KEY : HA_READ_AFTER_KEY);
  }
};




class QUICK_SELECT_I
{
public:
  ha_rows records;  
  double  read_time; 
  TABLE   *head;
  
  uint index;

  
  uint max_used_key_length;

  
  uint used_key_parts;

  QUICK_SELECT_I();
  virtual ~QUICK_SELECT_I(){};

  
  virtual int  init() = 0;

  
  virtual int  reset(void) = 0;

  virtual int  get_next() = 0;   

  
  virtual void range_end() {}

  virtual bool reverse_sorted() = 0;
  virtual bool unique_key_range() { return false; }

  
  virtual void need_sorted_output() = 0;
  enum {
    QS_TYPE_RANGE = 0,
    QS_TYPE_INDEX_INTERSECT = 1,
    QS_TYPE_INDEX_MERGE = 2,
    QS_TYPE_RANGE_DESC = 3,
    QS_TYPE_FULLTEXT   = 4,
    QS_TYPE_ROR_INTERSECT = 5,
    QS_TYPE_ROR_UNION = 6,
    QS_TYPE_GROUP_MIN_MAX = 7
  };

  
  virtual int get_type() = 0;

  
  virtual int init_ror_merged_scan(bool reuse_handler, MEM_ROOT *alloc)
  { DBUG_ASSERT(0); return 1; }

  
  virtual void save_last_pos(){};
  
  void add_key_and_length(String *key_names,
                          String *used_lengths,
                          bool *first);

  
  virtual void add_keys_and_lengths(String *key_names,
                                    String *used_lengths)=0;

  void add_key_name(String *str, bool *first);

  
  virtual Explain_quick_select* get_explain(MEM_ROOT *alloc)= 0;

  
  virtual bool is_keys_used(const MY_BITMAP *fields);

  
  virtual bool is_valid() { return index != MAX_KEY; };

  
  uchar    *last_rowid;

  
  uchar    *record;

  virtual void replace_handler(handler *new_file)
  {
    DBUG_ASSERT(0); 
  }

#ifndef DBUG_OFF
  
  virtual void dbug_dump(int indent, bool verbose)= 0;
#endif

  
  virtual QUICK_SELECT_I *make_reverse(uint used_key_parts_arg) { return NULL; }

  
  virtual void add_used_key_part_to_set()=0;
};


struct st_qsel_param;
class PARAM;



typedef struct st_quick_range_seq_ctx
{
  QUICK_RANGE **first;
  QUICK_RANGE **cur;
  QUICK_RANGE **last;
} QUICK_RANGE_SEQ_CTX;

range_seq_t quick_range_seq_init(void *init_param, uint n_ranges, uint flags);
bool quick_range_seq_next(range_seq_t rseq, KEY_MULTI_RANGE *range);



class QUICK_RANGE_SELECT : public QUICK_SELECT_I
{
protected:
  THD *thd;
  bool no_alloc;
  MEM_ROOT *parent_alloc;

  
  handler *file;

  
  bool in_ror_merged_scan;
  MY_BITMAP column_bitmap;
  bool free_file;   

  
  
  QUICK_RANGE_SEQ_CTX qr_traversal_ctx;
public:
  uint mrr_flags; 
protected:
  uint mrr_buf_size;   
  HANDLER_BUFFER *mrr_buf_desc; 

  
  
  DYNAMIC_ARRAY ranges;     
  QUICK_RANGE **cur_range;  
  
  QUICK_RANGE *last_range;
  
  KEY_PART *key_parts;
  KEY_PART_INFO *key_part_info;
  
  bool dont_free; 

  int cmp_next(QUICK_RANGE *range);
  int cmp_prev(QUICK_RANGE *range);
  bool row_in_ranges();
public:
  MEM_ROOT alloc;

  QUICK_RANGE_SELECT(THD *thd, TABLE *table,uint index_arg,bool no_alloc,
                     MEM_ROOT *parent_alloc, bool *create_err);
  ~QUICK_RANGE_SELECT();
  virtual QUICK_RANGE_SELECT *clone(bool *create_error)
    { return new QUICK_RANGE_SELECT(thd, head, index, no_alloc, parent_alloc,
                                    create_error); }
  
  void need_sorted_output();
  int init();
  int reset(void);
  int get_next();
  void range_end();
  int get_next_prefix(uint prefix_length, uint group_key_parts, 
                      uchar *cur_prefix);
  bool reverse_sorted() { return 0; }
  bool unique_key_range();
  int init_ror_merged_scan(bool reuse_handler, MEM_ROOT *alloc);
  void save_last_pos()
  { file->position(record); }
  int get_type() { return QS_TYPE_RANGE; }
  void add_keys_and_lengths(String *key_names, String *used_lengths);
  Explain_quick_select *get_explain(MEM_ROOT *alloc);
#ifndef DBUG_OFF
  void dbug_dump(int indent, bool verbose);
#endif
  virtual void replace_handler(handler *new_file) { file= new_file; }
  QUICK_SELECT_I *make_reverse(uint used_key_parts_arg);

  virtual void add_used_key_part_to_set();

private:
  
  friend class TRP_ROR_INTERSECT;
  friend
  QUICK_RANGE_SELECT *get_quick_select_for_ref(THD *thd, TABLE *table,
                                               struct st_table_ref *ref,
                                               ha_rows records);
  friend bool get_quick_keys(PARAM *param, QUICK_RANGE_SELECT *quick, 
                             KEY_PART *key, SEL_ARG *key_tree, 
                             uchar *min_key, uint min_key_flag,
                             uchar *max_key, uint max_key_flag);
  friend QUICK_RANGE_SELECT *get_quick_select(PARAM*,uint idx,
                                              SEL_ARG *key_tree,
                                              uint mrr_flags,
                                              uint mrr_buf_size,
                                              MEM_ROOT *alloc);
  friend class QUICK_SELECT_DESC;
  friend class QUICK_INDEX_SORT_SELECT;
  friend class QUICK_INDEX_MERGE_SELECT;
  friend class QUICK_ROR_INTERSECT_SELECT;
  friend class QUICK_INDEX_INTERSECT_SELECT;
  friend class QUICK_GROUP_MIN_MAX_SELECT;
  friend bool quick_range_seq_next(range_seq_t rseq, KEY_MULTI_RANGE *range);
  friend range_seq_t quick_range_seq_init(void *init_param,
                                          uint n_ranges, uint flags);
  friend 
  int read_keys_and_merge_scans(THD *thd, TABLE *head,
                                List<QUICK_RANGE_SELECT> quick_selects,
                                QUICK_RANGE_SELECT *pk_quick_select,
                                READ_RECORD *read_record,
                                bool intersection,
                                key_map *filtered_scans,
                                Unique **unique_ptr);

};


class QUICK_RANGE_SELECT_GEOM: public QUICK_RANGE_SELECT
{
public:
  QUICK_RANGE_SELECT_GEOM(THD *thd, TABLE *table, uint index_arg,
                          bool no_alloc, MEM_ROOT *parent_alloc, 
                          bool *create_err)
    :QUICK_RANGE_SELECT(thd, table, index_arg, no_alloc, parent_alloc,
    create_err)
    {};
  virtual QUICK_RANGE_SELECT *clone(bool *create_error)
    {
      DBUG_ASSERT(0);
      return new QUICK_RANGE_SELECT_GEOM(thd, head, index, no_alloc,
                                         parent_alloc, create_error);
    }
  virtual int get_next();
};




class QUICK_INDEX_SORT_SELECT : public QUICK_SELECT_I
{
protected:
  Unique *unique;
public:
  QUICK_INDEX_SORT_SELECT(THD *thd, TABLE *table);
  ~QUICK_INDEX_SORT_SELECT();

  int  init();
  void need_sorted_output() { DBUG_ASSERT(0);  }
  int  reset(void);
  bool reverse_sorted() { return false; }
  bool unique_key_range() { return false; }
  bool is_keys_used(const MY_BITMAP *fields);
#ifndef DBUG_OFF
  void dbug_dump(int indent, bool verbose);
#endif
  Explain_quick_select *get_explain(MEM_ROOT *alloc);

  bool push_quick_back(QUICK_RANGE_SELECT *quick_sel_range);

  
  List<QUICK_RANGE_SELECT> quick_selects;

  
  QUICK_RANGE_SELECT* pk_quick_select;

  MEM_ROOT alloc;
  THD *thd;
  virtual bool is_valid()
  {
    List_iterator_fast<QUICK_RANGE_SELECT> it(quick_selects);
    QUICK_RANGE_SELECT *quick;
    bool valid= true;
    while ((quick= it++))
    {
      if (!quick->is_valid())
      {
        valid= false;
        break;
      }
    }
    return valid;
  }
  virtual int read_keys_and_merge()= 0;
  
  READ_RECORD read_record;

  virtual void add_used_key_part_to_set();
};



class QUICK_INDEX_MERGE_SELECT : public QUICK_INDEX_SORT_SELECT
{
private:
  
  bool  doing_pk_scan;
protected:
  int read_keys_and_merge();

public:
  QUICK_INDEX_MERGE_SELECT(THD *thd_arg, TABLE *table)
    :QUICK_INDEX_SORT_SELECT(thd_arg, table) {}

  int get_next();
  int get_type() { return QS_TYPE_INDEX_MERGE; }
  void add_keys_and_lengths(String *key_names, String *used_lengths);
};

class QUICK_INDEX_INTERSECT_SELECT : public QUICK_INDEX_SORT_SELECT
{
protected:
  int read_keys_and_merge();

public:
  QUICK_INDEX_INTERSECT_SELECT(THD *thd_arg, TABLE *table)
    :QUICK_INDEX_SORT_SELECT(thd_arg, table) {}

  key_map filtered_scans;
  int get_next();
  int get_type() { return QS_TYPE_INDEX_INTERSECT; }
  void add_keys_and_lengths(String *key_names, String *used_lengths);
  Explain_quick_select *get_explain(MEM_ROOT *alloc);
};




class QUICK_ROR_INTERSECT_SELECT : public QUICK_SELECT_I
{
public:
  QUICK_ROR_INTERSECT_SELECT(THD *thd, TABLE *table,
                             bool retrieve_full_rows,
                             MEM_ROOT *parent_alloc);
  ~QUICK_ROR_INTERSECT_SELECT();

  int  init();
  void need_sorted_output() { DBUG_ASSERT(0);  }
  int  reset(void);
  int  get_next();
  bool reverse_sorted() { return false; }
  bool unique_key_range() { return false; }
  int get_type() { return QS_TYPE_ROR_INTERSECT; }
  void add_keys_and_lengths(String *key_names, String *used_lengths);
  Explain_quick_select *get_explain(MEM_ROOT *alloc);
  bool is_keys_used(const MY_BITMAP *fields);
  void add_used_key_part_to_set();
#ifndef DBUG_OFF
  void dbug_dump(int indent, bool verbose);
#endif
  int init_ror_merged_scan(bool reuse_handler, MEM_ROOT *alloc);
  bool push_quick_back(MEM_ROOT *alloc, QUICK_RANGE_SELECT *quick_sel_range);

  class QUICK_SELECT_WITH_RECORD : public Sql_alloc
  {
  public:
    QUICK_RANGE_SELECT *quick;
    uchar *key_tuple;
    ~QUICK_SELECT_WITH_RECORD() { delete quick; }
  };

  
  List<QUICK_SELECT_WITH_RECORD> quick_selects;

  virtual bool is_valid()
  {
    List_iterator_fast<QUICK_SELECT_WITH_RECORD> it(quick_selects);
    QUICK_SELECT_WITH_RECORD *quick;
    bool valid= true;
    while ((quick= it++))
    {
      if (!quick->quick->is_valid())
      {
        valid= false;
        break;
      }
    }
    return valid;
  }

  
  QUICK_RANGE_SELECT *cpk_quick;

  MEM_ROOT alloc; 
  THD *thd;       
  bool need_to_fetch_row; 
  
  bool scans_inited; 
};




class QUICK_ROR_UNION_SELECT : public QUICK_SELECT_I
{
public:
  QUICK_ROR_UNION_SELECT(THD *thd, TABLE *table);
  ~QUICK_ROR_UNION_SELECT();

  int  init();
  void need_sorted_output() { DBUG_ASSERT(0);  }
  int  reset(void);
  int  get_next();
  bool reverse_sorted() { return false; }
  bool unique_key_range() { return false; }
  int get_type() { return QS_TYPE_ROR_UNION; }
  void add_keys_and_lengths(String *key_names, String *used_lengths);
  Explain_quick_select *get_explain(MEM_ROOT *alloc);
  bool is_keys_used(const MY_BITMAP *fields);
  void add_used_key_part_to_set();
#ifndef DBUG_OFF
  void dbug_dump(int indent, bool verbose);
#endif

  bool push_quick_back(QUICK_SELECT_I *quick_sel_range);

  List<QUICK_SELECT_I> quick_selects; 

  virtual bool is_valid()
  {
    List_iterator_fast<QUICK_SELECT_I> it(quick_selects);
    QUICK_SELECT_I *quick;
    bool valid= true;
    while ((quick= it++))
    {
      if (!quick->is_valid())
      {
        valid= false;
        break;
      }
    }
    return valid;
  }

  QUEUE queue;    
  MEM_ROOT alloc; 

  THD *thd;             
  uchar *cur_rowid;      
  uchar *prev_rowid;     
  bool have_prev_rowid; 
  uint rowid_length;    
private:
  bool scans_inited; 
};




class QUICK_GROUP_MIN_MAX_SELECT : public QUICK_SELECT_I
{
private:
  handler * const file;   
  JOIN *join;            
  KEY  *index_info;      
  uchar *record;          
  uchar *tmp_record;      
  uchar *group_prefix;    
  const uint group_prefix_len; 
  uint group_key_parts;  
  uchar *last_prefix;     
  bool have_min;         
  bool have_max;         
  bool have_agg_distinct;
  bool seen_first_key;   
  bool doing_key_read;   

  KEY_PART_INFO *min_max_arg_part; 
                                   
  uint min_max_arg_len;  
  uchar *key_infix;       
  uint key_infix_len;
  DYNAMIC_ARRAY min_max_ranges; 
  uint real_prefix_len; 
  uint real_key_parts;  
  List<Item_sum> *min_functions;
  List<Item_sum> *max_functions;
  List_iterator<Item_sum> *min_functions_it;
  List_iterator<Item_sum> *max_functions_it;
  
  bool is_index_scan; 
public:
  
  MEM_ROOT alloc; 
  QUICK_RANGE_SELECT *quick_prefix_select;
private:
  int  next_prefix();
  int  next_min_in_range();
  int  next_max_in_range();
  int  next_min();
  int  next_max();
  void update_min_result();
  void update_max_result();
  int cmp_min_max_key(const uchar *key, uint16 length);
public:
  QUICK_GROUP_MIN_MAX_SELECT(TABLE *table, JOIN *join, bool have_min,
                             bool have_max, bool have_agg_distinct,
                             KEY_PART_INFO *min_max_arg_part,
                             uint group_prefix_len, uint group_key_parts,
                             uint used_key_parts, KEY *index_info, uint
                             use_index, double read_cost, ha_rows records, uint
                             key_infix_len, uchar *key_infix, MEM_ROOT
                             *parent_alloc, bool is_index_scan);
  ~QUICK_GROUP_MIN_MAX_SELECT();
  bool add_range(SEL_ARG *sel_range);
  void update_key_stat();
  void adjust_prefix_ranges();
  bool alloc_buffers();
  int init();
  void need_sorted_output() {  }
  int reset();
  int get_next();
  bool reverse_sorted() { return false; }
  bool unique_key_range() { return false; }
  int get_type() { return QS_TYPE_GROUP_MIN_MAX; }
  void add_keys_and_lengths(String *key_names, String *used_lengths);
  void add_used_key_part_to_set();
#ifndef DBUG_OFF
  void dbug_dump(int indent, bool verbose);
#endif
  bool is_agg_distinct() { return have_agg_distinct; }
  bool loose_scan_is_scanning() { return is_index_scan; }
  Explain_quick_select *get_explain(MEM_ROOT *alloc);
};


class QUICK_SELECT_DESC: public QUICK_RANGE_SELECT
{
public:
  QUICK_SELECT_DESC(QUICK_RANGE_SELECT *q, uint used_key_parts);
  virtual QUICK_RANGE_SELECT *clone(bool *create_error)
    { DBUG_ASSERT(0); return new QUICK_SELECT_DESC(this, used_key_parts); }
  int get_next();
  bool reverse_sorted() { return 1; }
  int get_type() { return QS_TYPE_RANGE_DESC; }
  QUICK_SELECT_I *make_reverse(uint used_key_parts_arg)
  {
    return this; 
  }
private:
  bool range_reads_after_key(QUICK_RANGE *range);
  int reset(void) { rev_it.rewind(); return QUICK_RANGE_SELECT::reset(); }
  List<QUICK_RANGE> rev_ranges;
  List_iterator<QUICK_RANGE> rev_it;
  uint used_key_parts;
};


class SQL_SELECT :public Sql_alloc {
 public:
  QUICK_SELECT_I *quick;	
  COND		*cond;		

  
  Item *pre_idx_push_select_cond;
  TABLE	*head;
  IO_CACHE file;		
  ha_rows records;		
  double read_time;		
  key_map quick_keys;		
  key_map needed_reg;		
  table_map const_tables,read_tables;
  
  key_map possible_keys;
  bool	free_cond; 

  SQL_SELECT();
  ~SQL_SELECT();
  void cleanup();
  void set_quick(QUICK_SELECT_I *new_quick) { delete quick; quick= new_quick; }
  bool check_quick(THD *thd, bool force_quick_range, ha_rows limit)
  {
    key_map tmp;
    tmp.set_all();
    return test_quick_select(thd, tmp, 0, limit, force_quick_range, FALSE, FALSE) < 0;
  }
     
  inline int skip_record(THD *thd)
  {
    int rc= MY_TEST(!cond || cond->val_int());
    if (thd->is_error())
      rc= -1;
    return rc;
  }
  int test_quick_select(THD *thd, key_map keys, table_map prev_tables,
			ha_rows limit, bool force_quick_range, 
                        bool ordered_output, bool remove_false_parts_of_where);
};


class SQL_SELECT_auto
{
  SQL_SELECT *select;
public:
  SQL_SELECT_auto(): select(NULL)
  {}
  ~SQL_SELECT_auto()
  {
    delete select;
  }
  SQL_SELECT_auto&
  operator= (SQL_SELECT *_select)
  {
    select= _select;
    return *this;
  }
  operator SQL_SELECT * () const
  {
    return select;
  }
  SQL_SELECT *
  operator-> () const
  {
    return select;
  }
  operator bool () const
  {
    return select;
  }
};


class FT_SELECT: public QUICK_RANGE_SELECT 
{
public:
  FT_SELECT(THD *thd, TABLE *table, uint key, bool *create_err) :
      QUICK_RANGE_SELECT (thd, table, key, 1, NULL, create_err) 
  { (void) init(); }
  ~FT_SELECT() { file->ft_end(); }
  virtual QUICK_RANGE_SELECT *clone(bool *create_error)
    { DBUG_ASSERT(0); return new FT_SELECT(thd, head, index, create_error); }
  int init() { return file->ft_init(); }
  int reset() { return 0; }
  int get_next() { return file->ha_ft_read(record); }
  int get_type() { return QS_TYPE_FULLTEXT; }
};

FT_SELECT *get_ft_select(THD *thd, TABLE *table, uint key);
QUICK_RANGE_SELECT *get_quick_select_for_ref(THD *thd, TABLE *table,
                                             struct st_table_ref *ref,
                                             ha_rows records);
SQL_SELECT *make_select(TABLE *head, table_map const_tables,
			table_map read_tables, COND *conds,
                        SORT_INFO* filesort,
                        bool allow_null_cond,  int *error);

bool calculate_cond_selectivity_for_table(THD *thd, TABLE *table, Item **cond);

bool eq_ranges_exceeds_limit(RANGE_SEQ_IF *seq, void *seq_init_param,
                             uint limit);

#ifdef WITH_PARTITION_STORAGE_ENGINE
bool prune_partitions(THD *thd, TABLE *table, Item *pprune_cond);
#endif
void store_key_image_to_rec(Field *field, uchar *ptr, uint len);

extern String null_string;


#define SELECTIVITY_SAMPLING_LIMIT 100

#define SELECTIVITY_SAMPLING_SHARE 0.10

#define SELECTIVITY_SAMPLING_THRESHOLD 10

#endif
