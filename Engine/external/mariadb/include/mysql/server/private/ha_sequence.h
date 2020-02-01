#ifndef HA_SEQUENCE_INCLUDED
#define HA_SEQUENCE_INCLUDED


#include "sql_sequence.h"
#include "table.h"
#include "handler.h"

extern handlerton *sql_sequence_hton;



class ha_sequence :public handler
{
private:
  handler *file;
  SEQUENCE *sequence;                     

public:
  
  bool write_locked;

  ha_sequence(handlerton *hton, TABLE_SHARE *share);
  ~ha_sequence();

  
  int open(const char *name, int mode, uint test_if_locked);
  int create(const char *name, TABLE *form,
             HA_CREATE_INFO *create_info);
  handler *clone(const char *name, MEM_ROOT *mem_root);
  int write_row(uchar *buf);
  Table_flags table_flags() const;
  
  int update_row(const uchar *old_data, const uchar *new_data)
  { return HA_ERR_WRONG_COMMAND; }
  int delete_row(const uchar *buf)
  { return HA_ERR_WRONG_COMMAND; }
  
  int truncate()
  { return HA_ERR_WRONG_COMMAND; }
  
  uint8 table_cache_type()
  { return HA_CACHE_TBL_NOCACHE; }
  void print_error(int error, myf errflag);
  int info(uint);
  LEX_CSTRING *engine_name() { return hton_name(file->ht); }
  int external_lock(THD *thd, int lock_type);
  int extra(enum ha_extra_function operation);
  
  bool check_if_incompatible_data(HA_CREATE_INFO *create_info,
                                  uint table_changes);
  void write_lock() { write_locked= 1;}
  void unlock() { write_locked= 0; }
  bool is_locked() { return write_locked; }

  
  int rnd_init(bool scan)
  { return file->rnd_init(scan); }
  
  int rnd_next(uchar *buf)
  {
    int error;
    table->s->sequence->read_lock(table);
    error= file->rnd_next(buf);
    table->s->sequence->read_unlock(table);
    return error;
  }
  int rnd_end()
  { return file->rnd_end(); }
  int rnd_pos(uchar *buf, uchar *pos)
  {
    int error;
    table->s->sequence->read_lock(table);
    error= file->rnd_pos(buf, pos);
    table->s->sequence->read_unlock(table);
    return error;
  }
  void position(const uchar *record)
  { return file->position(record); }
  const char *table_type() const
  { return file->table_type(); }
  ulong index_flags(uint inx, uint part, bool all_parts) const
  { return file->index_flags(inx, part, all_parts); }
  THR_LOCK_DATA **store_lock(THD *thd, THR_LOCK_DATA **to,
                             enum thr_lock_type lock_type)
  { return file->store_lock(thd, to, lock_type); }
  int close(void)
  { return file->close(); }
  const char **bas_ext() const
  { return file->bas_ext(); }
  int delete_table(const char*name)
  { return file->delete_table(name); }
  int rename_table(const char *from, const char *to)
  { return file->rename_table(from, to); }
  void unbind_psi()
  { return file->unbind_psi(); }
  void rebind_psi()
  { return file->rebind_psi(); }

  bool auto_repair(int error) const
  { return file->auto_repair(error); }
  int repair(THD* thd, HA_CHECK_OPT* check_opt)
  { return file->repair(thd, check_opt); }
  bool check_and_repair(THD *thd)
  { return file->check_and_repair(thd); }
  bool is_crashed() const
  { return file->is_crashed(); }
  void column_bitmaps_signal()
  { return file->column_bitmaps_signal(); }

  
  void register_original_handler(handler *file_arg)
  {
    file= file_arg;
    init();                                     
  }
};
#endif
