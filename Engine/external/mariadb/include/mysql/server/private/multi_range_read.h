





#include "sql_lifo_buffer.h"

class DsMrr_impl;
class Mrr_ordered_index_reader;



class Key_parameters
{
public:
  uint         key_tuple_length; 
  key_part_map key_tuple_map;    

  
  uint key_size_in_keybuf;

  
  bool use_key_pointers;

  
  bool index_ranges_unique;
};




class Key_value_records_iterator
{
  
  Mrr_ordered_index_reader *owner;

  
  Lifo_buffer_iterator identical_key_it;
  
  
  uchar *last_identical_key_ptr;

  
  bool get_next_row;
  
public:
  int init(Mrr_ordered_index_reader *owner_arg);
  int get_next(range_id_t *range_info);
  void move_to_next_key_value();
};



typedef struct st_buffer_manager
{
public:
  
  void *arg;
  
  
  void (*redistribute_buffer_space)(void *arg);

  
  void (*reset_buffer_sizes)(void *arg);

} Buffer_manager;




class Mrr_reader 
{
public:
  virtual int get_next(range_id_t *range_info) = 0;
  virtual int refill_buffer(bool initial) = 0;
  virtual ~Mrr_reader() {}; 
};




class Mrr_index_reader : public Mrr_reader
{
protected:
  handler *file; 
public:
  virtual int init(handler *h_arg, RANGE_SEQ_IF *seq_funcs, 
                   void *seq_init_param, uint n_ranges,
                   uint mode, Key_parameters *key_par, 
                   Lifo_buffer *key_buffer, 
                   Buffer_manager *buf_manager_arg) = 0;

  
  virtual uchar *get_rowid_ptr() = 0;
  
  virtual void position();
  virtual bool skip_record(range_id_t range_id, uchar *rowid) = 0;

  virtual void interrupt_read() {}
  virtual void resume_read() {}
};




class Mrr_simple_index_reader : public Mrr_index_reader
{
public:
  int init(handler *h_arg, RANGE_SEQ_IF *seq_funcs,
           void *seq_init_param, uint n_ranges,
           uint mode, Key_parameters *key_par,
           Lifo_buffer *key_buffer,
           Buffer_manager *buf_manager_arg);
  int get_next(range_id_t *range_info);
  int refill_buffer(bool initial) { return initial? 0: HA_ERR_END_OF_FILE; }
  uchar *get_rowid_ptr() { return file->ref; }
  bool skip_record(range_id_t range_id, uchar *rowid)
  {
    return (file->mrr_funcs.skip_record &&
            file->mrr_funcs.skip_record(file->mrr_iter, range_id, rowid));
  }
};




class Mrr_ordered_index_reader : public Mrr_index_reader
{
public:
  int init(handler *h_arg, RANGE_SEQ_IF *seq_funcs, 
           void *seq_init_param, uint n_ranges,
           uint mode, Key_parameters *key_par,
           Lifo_buffer *key_buffer,
           Buffer_manager *buf_manager_arg);
  int get_next(range_id_t *range_info);
  int refill_buffer(bool initial);
  uchar *get_rowid_ptr() { return file->ref; }
  
  bool skip_record(range_id_t range_info, uchar *rowid)
  {
    return (mrr_funcs.skip_record &&
            mrr_funcs.skip_record(mrr_iter, range_info, rowid));
  }

  bool skip_index_tuple(range_id_t range_info)
  {
    return (mrr_funcs.skip_index_tuple &&
            mrr_funcs.skip_index_tuple(mrr_iter, range_info));
  }
  
  bool set_interruption_temp_buffer(uint rowid_length, uint key_len, 
                                    uint saved_pk_len,
                                    uchar **space_start, uchar *space_end);
  void set_no_interruption_temp_buffer();

  void interrupt_read();
  void resume_read();
  void position();
private:
  Key_value_records_iterator kv_it;

  bool scanning_key_val_iter;
  
  
  Lifo_buffer *key_buffer;
  
  
  Buffer_manager *buf_manager;

  Key_parameters  keypar; 

  
  bool is_mrr_assoc;
  
  
  RANGE_SEQ_IF mrr_funcs;
  range_seq_t mrr_iter;
  
  
  bool source_exhausted;
   
   
  bool support_scan_interruptions;
  
  uchar *saved_rowid;
  
  
  bool have_saved_rowid;
  
  uchar *saved_key_tuple; 
  uchar *saved_primary_key; 
  
  
  bool read_was_interrupted;

  static int compare_keys(void* arg, uchar* key1, uchar* key2);
  static int compare_keys_reverse(void* arg, uchar* key1, uchar* key2);
  
  friend class Key_value_records_iterator; 
  friend class DsMrr_impl;
  friend class Mrr_ordered_rndpos_reader;
};




class Mrr_ordered_rndpos_reader : public Mrr_reader 
{
public:
  int init(handler *file, Mrr_index_reader *index_reader, uint mode,
           Lifo_buffer *buf);
  int get_next(range_id_t *range_info);
  int refill_buffer(bool initial);
private:
  handler *file; 
  
  
  Mrr_index_reader *index_reader;

  
  uchar *index_rowid;
  
  
  bool index_reader_exhausted;
  
  
  bool index_reader_needs_refill;

  
  bool is_mrr_assoc;
  
  
  uchar *last_identical_rowid;

  
  Lifo_buffer *rowid_buffer;
  
  int refill_from_index_reader();
};




class Mrr_reader_factory
{
public:
  Mrr_ordered_rndpos_reader ordered_rndpos_reader;
  Mrr_ordered_index_reader  ordered_index_reader;
  Mrr_simple_index_reader   simple_index_reader;
};


#define DSMRR_IMPL_SORT_KEYS   HA_MRR_IMPLEMENTATION_FLAG1
#define DSMRR_IMPL_SORT_ROWIDS HA_MRR_IMPLEMENTATION_FLAG2



class DsMrr_impl
{
public:
  typedef void (handler::*range_check_toggle_func_t)(bool on);

  DsMrr_impl()
    : secondary_file(NULL) {};
  
  void init(handler *h_arg, TABLE *table_arg)
  {
    primary_file= h_arg; 
    table= table_arg;
  }
  int dsmrr_init(handler *h_arg, RANGE_SEQ_IF *seq_funcs, 
                 void *seq_init_param, uint n_ranges, uint mode, 
                 HANDLER_BUFFER *buf);
  void dsmrr_close();
  int dsmrr_next(range_id_t *range_info);

  ha_rows dsmrr_info(uint keyno, uint n_ranges, uint keys, uint key_parts, 
                     uint *bufsz, uint *flags, Cost_estimate *cost);

  ha_rows dsmrr_info_const(uint keyno, RANGE_SEQ_IF *seq, 
                            void *seq_init_param, uint n_ranges, uint *bufsz,
                            uint *flags, Cost_estimate *cost);

  int dsmrr_explain_info(uint mrr_mode, char *str, size_t size);
private:
  
  Lifo_buffer *key_buffer;

  
  handler *primary_file;
  TABLE *table; 

  
  handler *secondary_file;
  
  uint keyno; 
  
  bool is_mrr_assoc;

  Mrr_reader_factory reader_factory;

  Mrr_reader *strategy;
  bool strategy_exhausted;

  Mrr_index_reader *index_strategy;

  
  uchar *full_buf;
  uchar *full_buf_end;
  
  
  uchar *rowid_buffer_end;
 
  
  Forward_lifo_buffer forward_key_buf;
  Backward_lifo_buffer backward_key_buf;

  
  Forward_lifo_buffer rowid_buffer;
  
  bool choose_mrr_impl(uint keyno, ha_rows rows, uint *flags, uint *bufsz, 
                       Cost_estimate *cost);
  bool get_disk_sweep_mrr_cost(uint keynr, ha_rows rows, uint flags, 
                               uint *buffer_size, Cost_estimate *cost);
  bool check_cpk_scan(THD *thd, TABLE_SHARE *share, uint keyno, uint mrr_flags);

  bool setup_buffer_sharing(uint key_size_in_keybuf, key_part_map key_tuple_map);

  
  Buffer_manager buf_manager;
  static void redistribute_buffer_space(void *dsmrr_arg);
  static void reset_buffer_sizes(void *dsmrr_arg);
  static void do_nothing(void *dsmrr_arg);

  Lifo_buffer* get_key_buffer() { return key_buffer; }

  friend class Key_value_records_iterator;
  friend class Mrr_ordered_index_reader;
  friend class Mrr_ordered_rndpos_reader;

  int  setup_two_handlers();
  void close_second_handler();
};



