



#define JOIN_CACHE_INCREMENTAL_BIT           1
#define JOIN_CACHE_HASHED_BIT                2
#define JOIN_CACHE_BKA_BIT                   4


#define CACHE_BLOB      1        
#define CACHE_STRIPPED  2        
#define CACHE_VARSTR1   3         
#define CACHE_VARSTR2   4        
#define CACHE_ROWID     5        


typedef struct st_cache_field {
  uchar *str;    
  uint length;  
  
  Field *field;
  uint type;    
   
  uint referenced_field_no; 
  
  uint blob_length; 
  uint offset;      
} CACHE_FIELD;


class JOIN_TAB_SCAN;

class EXPLAIN_BKA_TYPE;

 

class JOIN_CACHE :public Sql_alloc
{

private:

     
  uint size_of_rec_ofs;    
  
  uint size_of_rec_len;
     
  uint size_of_fld_ofs;

  
  bool for_explain_only;

protected:
       
   

  
  uint offset_size(size_t len)
  { return (len < 256 ? 1 : len < 256*256 ? 2 : 4); }

  
  ulong get_offset(uint ofs_sz, uchar *ptr)
  {
    switch (ofs_sz) {
    case 1: return uint(*ptr);
    case 2: return uint2korr(ptr);
    case 4: return uint4korr(ptr);
    }
    return 0;
  }

   
  void store_offset(uint ofs_sz, uchar *ptr, ulong ofs)
  {
    switch (ofs_sz) {
    case 1: *ptr= (uchar) ofs; return;
    case 2: int2store(ptr, (uint16) ofs); return;
    case 4: int4store(ptr, (uint32) ofs); return;
    }
  }
  
  
  uint length;

     
  JOIN *join;  

  
  JOIN_TAB *start_tab;

  
  uint fields;

  
  uint flag_fields;

   
  uint blobs;

     
  uint referenced_fields;
   
  
  uint data_field_count; 

  
  uint data_field_ptr_count;
 
  
  CACHE_FIELD *field_descr;

  
  CACHE_FIELD **blob_ptr;

  
  bool with_match_flag; 
  
  bool with_length;

  
  uint pack_length;
  
  uint pack_length_with_blob_ptrs;

  
  uint base_prefix_length;

  
  size_t avg_record_length;

  
  size_t avg_aux_buffer_incr;

  
  size_t space_per_record; 

  
  uchar *buff;         
   
  size_t buff_size;
  
  size_t min_buff_size;
  
  size_t max_buff_size;
   
  size_t aux_buff_size;

   
  size_t records;
  
  size_t min_records;
  
  size_t max_records;

  
  uchar *pos;
  
  uchar *end_pos; 

  
  uchar *curr_rec_pos;
  
  uchar *last_rec_pos;

  
  bool last_rec_blob_data_is_in_rec_buff;

  
  uchar *curr_rec_link;

   
  bool last_written_is_null_compl;

  
  uint local_key_arg_fields;
  
  uint external_key_arg_fields;

  
  bool use_emb_key;
   
  uint emb_key_length;

  
  JOIN_TAB_SCAN *join_tab_scan;

  void calc_record_fields();     
  void collect_info_on_key_args();
  int alloc_fields();
  void create_flag_fields();
  void create_key_arg_fields();
  void create_remaining_fields();
  void set_constants();
  int alloc_buffer();

  
  virtual int realloc_buffer();
  
   
  bool check_emb_key_usage();

  uint get_size_of_rec_offset() { return size_of_rec_ofs; }
  uint get_size_of_rec_length() { return size_of_rec_len; }
  uint get_size_of_fld_offset() { return size_of_fld_ofs; }

  uchar *get_rec_ref(uchar *ptr)
  {
    return buff+get_offset(size_of_rec_ofs, ptr-size_of_rec_ofs);
  }
  ulong get_rec_length(uchar *ptr)
  { 
    return (ulong) get_offset(size_of_rec_len, ptr);
  }
  ulong get_fld_offset(uchar *ptr)
  { 
    return (ulong) get_offset(size_of_fld_ofs, ptr);
  }

  void store_rec_ref(uchar *ptr, uchar* ref)
  {
    store_offset(size_of_rec_ofs, ptr-size_of_rec_ofs, (ulong) (ref-buff));
  }
  void store_rec_length(uchar *ptr, ulong len)
  {
    store_offset(size_of_rec_len, ptr, len);
  }
  void store_fld_offset(uchar *ptr, ulong ofs)
  {
    store_offset(size_of_fld_ofs, ptr, ofs);
  }

   
  uint write_record_data(uchar *link, bool *is_full);

   
  virtual uint get_prefix_length() { return base_prefix_length; }
  
  virtual uint get_record_max_affix_length(); 

  
  virtual uint get_max_key_addon_space_per_record() { return 0; }

  
  virtual uint aux_buffer_incr(size_t recno);

   
  virtual size_t rem_space() 
  { 
    return MY_MAX(buff_size-(end_pos-buff)-aux_buff_size,0);
  }

  
  virtual uint extra_key_length() { return 0; }

  
  uint read_all_record_fields();
  
  
  uint read_flag_fields();

  
  uint read_record_field(CACHE_FIELD *copy, bool last_record);

  
  bool read_referenced_field(CACHE_FIELD *copy, uchar *rec_ptr, uint *len);

  
  virtual bool skip_if_matched();

  
  virtual bool skip_if_not_needed_match();

  
  bool blob_data_is_in_rec_buff(uchar *rec_ptr)
  {
    return rec_ptr == last_rec_pos && last_rec_blob_data_is_in_rec_buff;
  }

  
  virtual enum_nested_loop_state join_matching_records(bool skip_last);

  
  virtual int setup_aux_buffer(HANDLER_BUFFER &aux_buff) 
  {
    DBUG_ASSERT(0);
    return 0;
  }

    
  virtual uint get_number_of_ranges_for_mrr() { return 0; };

   
  virtual bool prepare_look_for_matches(bool skip_last)= 0;
     
  virtual uchar *get_next_candidate_for_match()= 0;
  
  virtual bool skip_next_candidate_for_match(uchar *rec_ptr)= 0;
  
  virtual void read_next_candidate_for_match(uchar *rec_ptr)= 0;

  
  virtual uchar **get_curr_association_ptr() { return 0; };

  
  virtual enum_nested_loop_state join_null_complements(bool skip_last);

  
  virtual void restore_last_record();

  
  bool set_match_flag_if_none(JOIN_TAB *first_inner, uchar *rec_ptr);

  enum_nested_loop_state generate_full_extensions(uchar *rec_ptr);

  
  bool check_match(uchar *rec_ptr);

     
  JOIN_CACHE(JOIN *j, JOIN_TAB *tab)
  {
    join= j;
    join_tab= tab;
    prev_cache= next_cache= 0;
    buff= 0;
  }

     
  JOIN_CACHE(JOIN *j, JOIN_TAB *tab, JOIN_CACHE *prev)   
  {  
    join= j;
    join_tab= tab;
    next_cache= 0;
    prev_cache= prev;
    buff= 0;
    if (prev)
      prev->next_cache= this;
  }

public:
 
  

  enum Join_algorithm
  { 
    BNL_JOIN_ALG,     
    BNLH_JOIN_ALG,    
    BKA_JOIN_ALG,     
    BKAH_JOIN_ALG    
  };

  
  enum Match_flag { MATCH_NOT_FOUND, MATCH_FOUND, MATCH_IMPOSSIBLE };

   
  JOIN_TAB *join_tab;

  
  JOIN_CACHE *prev_cache;
  
  JOIN_CACHE *next_cache;

   
  virtual int init(bool for_explain);

   
  size_t get_join_buffer_size() { return buff_size; }
  
  void set_join_buffer_size(size_t sz) { buff_size= sz; }

  
  virtual size_t get_min_join_buffer_size();
   
  virtual size_t get_max_join_buffer_size(bool optimize_buff_size);

  
  bool shrink_join_buffer_in_ratio(ulonglong n, ulonglong d);

  
  virtual enum Join_algorithm get_join_alg()= 0;

  
  virtual bool is_key_access()= 0;

  
  virtual void reset(bool for_writing);

   
  virtual bool put_record();

   
  virtual bool get_record();

   
  virtual void get_record_by_pos(uchar *rec_ptr);

  
  virtual enum Match_flag get_match_flag_by_pos(uchar *rec_ptr);

  
  virtual uchar *get_curr_rec() { return curr_rec_pos; }

  
  virtual void set_curr_rec_link(uchar *link) { curr_rec_link= link; }

  
  virtual uchar *get_curr_rec_link()
  { 
    return (curr_rec_link ? curr_rec_link : get_curr_rec());
  }
     
   
  enum_nested_loop_state join_records(bool skip_last);

  
  virtual bool save_explain_data(EXPLAIN_BKA_TYPE *explain);

  THD *thd();

  virtual ~JOIN_CACHE() {}
  void reset_join(JOIN *j) { join= j; }
  void free()
  { 
    my_free(buff);
    buff= 0;
  }   
  
  friend class JOIN_CACHE_HASHED;
  friend class JOIN_CACHE_BNL;
  friend class JOIN_CACHE_BKA;
  friend class JOIN_TAB_SCAN;
  friend class JOIN_TAB_SCAN_MRR;

};




class JOIN_CACHE_HASHED: public JOIN_CACHE
{

  typedef uint (JOIN_CACHE_HASHED::*Hash_func) (uchar *key, uint key_len);
  typedef bool (JOIN_CACHE_HASHED::*Hash_cmp_func) (uchar *key1, uchar *key2,
                                                    uint key_len);
  
private:

  
  uint size_of_key_ofs;

   
  uint key_entry_length;
 
  
  uchar *hash_table;
  
  uint hash_entries;


  
  uchar *curr_key_entry;

  
  uint data_fields_offset;

  inline uint get_hash_idx_simple(uchar *key, uint key_len);
  inline uint get_hash_idx_complex(uchar *key, uint key_len);

  inline bool equal_keys_simple(uchar *key1, uchar *key2, uint key_len);
  inline bool equal_keys_complex(uchar *key1, uchar *key2, uint key_len);

  int init_hash_table();
  void cleanup_hash_table();
  
protected:

      
  KEY *ref_key_info;
      
  uint ref_used_key_parts;

   
  Hash_func hash_func;
   
  Hash_cmp_func hash_cmp_func;

  
  uint key_length;
  
  uchar *key_buff;

  
  uint key_entries;

  
  uchar *last_key_entry;

   
  uint rec_fields_offset;

  uint get_size_of_key_offset() { return size_of_key_ofs; }

    
  uchar *get_next_key_ref(uchar *key_ref_ptr)
  {
    return hash_table-get_offset(size_of_key_ofs, key_ref_ptr);
  }

    
  void store_next_key_ref(uchar *key_ref_ptr, uchar *ref)
  {
    store_offset(size_of_key_ofs, key_ref_ptr, (ulong) (hash_table-ref));
  }     
  
  
  bool is_null_key_ref(uchar *key_ref_ptr)
  {
    ulong nil= 0;
    return memcmp(key_ref_ptr, &nil, size_of_key_ofs ) == 0;
  } 

  
  void store_null_key_ref(uchar *key_ref_ptr)
  {
    ulong nil= 0;
    store_offset(size_of_key_ofs, key_ref_ptr, nil);
  } 

  uchar *get_next_rec_ref(uchar *ref_ptr)
  {
    return buff+get_offset(get_size_of_rec_offset(), ref_ptr);
  }

  void store_next_rec_ref(uchar *ref_ptr, uchar *ref)
  {
    store_offset(get_size_of_rec_offset(), ref_ptr, (ulong) (ref-buff));
  } 

   
  uchar *get_curr_emb_key()
  {
    return get_curr_rec()+data_fields_offset;
  }

    
  uchar *get_emb_key(uchar *ref_ptr)
  {
    return buff+get_offset(get_size_of_rec_offset(), ref_ptr);
  }

    
  void store_emb_key_ref(uchar *ref_ptr, uchar *ref)
  {
    store_offset(get_size_of_rec_offset(), ref_ptr, (ulong) (ref-buff));
  }
  
   
  uint get_prefix_length() 
  { 
    return base_prefix_length + get_size_of_rec_offset();
  }

  
  uint get_max_key_addon_space_per_record();

   
  size_t rem_space() 
  { 
    return MY_MAX(last_key_entry-end_pos-aux_buff_size,0);
  }

  
  uint extra_key_length() { return key_entry_length; }

  
  bool skip_if_matched();

  
  bool skip_if_not_needed_match();

  
  bool key_search(uchar *key, uint key_len, uchar **key_ref_ptr);

  
  int realloc_buffer();

     
  JOIN_CACHE_HASHED(JOIN *j, JOIN_TAB *tab) :JOIN_CACHE(j, tab) {}

     
  JOIN_CACHE_HASHED(JOIN *j, JOIN_TAB *tab, JOIN_CACHE *prev) 
		    :JOIN_CACHE(j, tab, prev) {}

public:

         
  int init(bool for_explain);

  
  void reset(bool for_writing);

  
  bool put_record();

  
  bool get_record();

     
  virtual bool check_all_match_flags_for_key(uchar *key_chain_ptr);

  uint get_next_key(uchar **key); 
  
   
  uchar *get_curr_key_chain()
  {
    return get_next_rec_ref(curr_key_entry+key_entry_length-
                            get_size_of_rec_offset());
  }
  
};



   
class JOIN_TAB_SCAN: public Sql_alloc
{

private:
  
  bool is_first_record;

protected:

  
  JOIN_TAB *join_tab;
   
  JOIN_CACHE *cache;
     
  JOIN *join;

public:
  
  JOIN_TAB_SCAN(JOIN *j, JOIN_TAB *tab)
  {
    join= j;
    join_tab= tab;
    cache= join_tab->cache;
  }

  virtual ~JOIN_TAB_SCAN() {}
 
  
  virtual uint aux_buffer_incr(size_t recno) { return 0; }

  
  virtual int open();
  
  virtual int next();
   
  virtual void close();

};



class JOIN_CACHE_BNL :public JOIN_CACHE
{
private:
  
  uint rem_records;

protected:

  bool prepare_look_for_matches(bool skip_last);

  uchar *get_next_candidate_for_match();

  bool skip_next_candidate_for_match(uchar *rec_ptr);

  void read_next_candidate_for_match(uchar *rec_ptr);

public:

     
  JOIN_CACHE_BNL(JOIN *j, JOIN_TAB *tab) :JOIN_CACHE(j, tab) {}

     
  JOIN_CACHE_BNL(JOIN *j, JOIN_TAB *tab, JOIN_CACHE *prev) 
    :JOIN_CACHE(j, tab, prev) {}

         
  int init(bool for_explain);

  enum Join_algorithm get_join_alg() { return BNL_JOIN_ALG; }

  bool is_key_access() { return FALSE; }

};




class JOIN_CACHE_BNLH :public JOIN_CACHE_HASHED
{

protected:

  
  uchar *last_matching_rec_ref_ptr;
  
  uchar *next_matching_rec_ref_ptr;

  
  uchar *get_matching_chain_by_join_key();

  bool prepare_look_for_matches(bool skip_last);

  uchar *get_next_candidate_for_match();

  bool skip_next_candidate_for_match(uchar *rec_ptr);

  void read_next_candidate_for_match(uchar *rec_ptr);

public:

     
  JOIN_CACHE_BNLH(JOIN *j, JOIN_TAB *tab) : JOIN_CACHE_HASHED(j, tab) {}

     
  JOIN_CACHE_BNLH(JOIN *j, JOIN_TAB *tab, JOIN_CACHE *prev) 
    : JOIN_CACHE_HASHED(j, tab, prev) {}

         
  int init(bool for_explain);

  enum Join_algorithm get_join_alg() { return BNLH_JOIN_ALG; }

  bool is_key_access() { return TRUE; }

};



   
class JOIN_TAB_SCAN_MRR: public JOIN_TAB_SCAN
{
  
  RANGE_SEQ_IF range_seq_funcs;

  
  uint ranges;

   
  uint mrr_mode;

  
  HANDLER_BUFFER mrr_buff;

  
  virtual void init_mrr_buff()
  {
    cache->setup_aux_buffer(mrr_buff);
  }

public:

  JOIN_TAB_SCAN_MRR(JOIN *j, JOIN_TAB *tab, uint flags, RANGE_SEQ_IF rs_funcs)
    :JOIN_TAB_SCAN(j, tab), range_seq_funcs(rs_funcs), mrr_mode(flags) {}

  uint aux_buffer_incr(size_t recno);

  int open();
 
  int next();

  friend class JOIN_CACHE_BKA; 
};



class JOIN_CACHE_BKA :public JOIN_CACHE
{
private:

  
  uint mrr_mode;

  
  uint rem_records;

  
  uchar *curr_association;

protected:

  
  uint get_number_of_ranges_for_mrr() { return (uint)records; }

 
  int setup_aux_buffer(HANDLER_BUFFER &aux_buff)
  {
    aux_buff.buffer= end_pos;
    aux_buff.buffer_end= buff+buff_size;
    return 0;
  }

  bool prepare_look_for_matches(bool skip_last);

  uchar *get_next_candidate_for_match();

  bool skip_next_candidate_for_match(uchar *rec_ptr);

  void read_next_candidate_for_match(uchar *rec_ptr);

public:

     
  JOIN_CACHE_BKA(JOIN *j, JOIN_TAB *tab, uint flags)
    :JOIN_CACHE(j, tab), mrr_mode(flags) {}
     
  JOIN_CACHE_BKA(JOIN *j, JOIN_TAB *tab, uint flags, JOIN_CACHE *prev)
    :JOIN_CACHE(j, tab, prev), mrr_mode(flags) {}
  
  JOIN_CACHE_BKA(JOIN_CACHE_BKA *bka)
    :JOIN_CACHE(bka->join, bka->join_tab, bka->prev_cache),
      mrr_mode(bka->mrr_mode)  {}

  uchar **get_curr_association_ptr() { return &curr_association; }

         
  int init(bool for_explain);

  enum Join_algorithm get_join_alg() { return BKA_JOIN_ALG; }

  bool is_key_access() { return TRUE; }

  
  uint get_next_key(uchar **key);

  
  bool skip_index_tuple(range_id_t range_info);

  bool save_explain_data(EXPLAIN_BKA_TYPE *explain);
};





class JOIN_CACHE_BKAH :public JOIN_CACHE_BNLH
{

private:
  
  uint mrr_mode;

         
  bool no_association;

  
  uchar *curr_matching_chain;

protected:

  uint get_number_of_ranges_for_mrr() { return key_entries; }

  
  int setup_aux_buffer(HANDLER_BUFFER &aux_buff)
  {
    aux_buff.buffer= end_pos;
    aux_buff.buffer_end= last_key_entry;
    return 0;
  }

  bool prepare_look_for_matches(bool skip_last);

  

public:

     
  JOIN_CACHE_BKAH(JOIN *j, JOIN_TAB *tab, uint flags) 
    :JOIN_CACHE_BNLH(j, tab), mrr_mode(flags) {}

     
  JOIN_CACHE_BKAH(JOIN *j, JOIN_TAB *tab, uint flags, JOIN_CACHE *prev)
    :JOIN_CACHE_BNLH(j, tab, prev), mrr_mode(flags)  {}

  JOIN_CACHE_BKAH(JOIN_CACHE_BKAH *bkah)
    :JOIN_CACHE_BNLH(bkah->join, bkah->join_tab, bkah->prev_cache),
      mrr_mode(bkah->mrr_mode)  {}

  uchar **get_curr_association_ptr() { return &curr_matching_chain; }

         
  int init(bool for_explain);

  enum Join_algorithm get_join_alg() { return BKAH_JOIN_ALG; }

  
  bool skip_index_tuple(range_id_t range_info);

  bool save_explain_data(EXPLAIN_BKA_TYPE *explain);
};
