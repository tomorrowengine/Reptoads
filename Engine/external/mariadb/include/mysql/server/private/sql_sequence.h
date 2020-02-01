

#ifndef SQL_SEQUENCE_INCLUDED
#define SQL_SEQUENCE_INCLUDED

#define seq_field_used_min_value 1
#define seq_field_used_max_value 2
#define seq_field_used_start     4
#define seq_field_used_increment 8
#define seq_field_used_cache     16
#define seq_field_used_cycle     32
#define seq_field_used_restart   64
#define seq_field_used_restart_value 128


#define NEXT_FIELD_NO 0
#define MIN_VALUE_FIELD_NO 1
#define ROUND_FIELD_NO 7



class sequence_definition :public Sql_alloc
{
public:
  sequence_definition():
    min_value(1), max_value(LONGLONG_MAX-1), start(1), increment(1),
      cache(1000), round(0), restart(0), cycle(0), used_fields(0)
  {}
  longlong reserved_until;
  longlong min_value;
  longlong max_value;
  longlong start;
  longlong increment;
  longlong cache;
  ulonglong round;
  longlong restart;              
  bool     cycle;
  uint used_fields;              

  bool check_and_adjust(bool set_reserved_until);
  void store_fields(TABLE *table);
  void read_fields(TABLE *table);
  int write_initial_sequence(TABLE *table);
  int write(TABLE *table, bool all_fields);
  
  void adjust_values(longlong next_value);
  inline void print_dbug()
  {
    DBUG_PRINT("sequence", ("reserved: %lld  start: %lld  increment: %lld  min_value: %lld  max_value: %lld  cache: %lld  round: %lld",
                      reserved_until, start, increment, min_value,
                        max_value, cache, round));
  }
protected:
  
  longlong real_increment;
  longlong next_free_value;
};



class SEQUENCE :public sequence_definition
{
public:
  enum seq_init { SEQ_UNINTIALIZED, SEQ_IN_PREPARE, SEQ_IN_ALTER,
                  SEQ_READY_TO_USE };
  SEQUENCE();
  ~SEQUENCE();
  int  read_initial_values(TABLE *table);
  int  read_stored_values(TABLE *table);
  void write_lock(TABLE *table);
  void write_unlock(TABLE *table);
  void read_lock(TABLE *table);
  void read_unlock(TABLE *table);
  void copy(sequence_definition *seq)
  {
    sequence_definition::operator= (*seq);
    adjust_values(reserved_until);
    all_values_used= 0;
  }
  longlong next_value(TABLE *table, bool second_round, int *error);
  int set_value(TABLE *table, longlong next_value, ulonglong round_arg,
                bool is_used);
  longlong increment_value(longlong value)
  {
    if (real_increment > 0)
    {
      if (value + real_increment > max_value ||
          value > max_value - real_increment)
        value= max_value + 1;
      else
        value+= real_increment;
    }
    else
    {
      if (value + real_increment < min_value ||
          value < min_value - real_increment)
        value= min_value - 1;
      else
        value+= real_increment;
    }
    return value;
  }

  bool all_values_used;
  seq_init initialized;

private:
  mysql_rwlock_t mutex;
};




class SEQUENCE_LAST_VALUE
{
public:
  SEQUENCE_LAST_VALUE(uchar *key_arg, uint length_arg)
    :key(key_arg), length(length_arg)
  {}
  ~SEQUENCE_LAST_VALUE()
  { my_free((void*) key); }
  
  bool check_version(TABLE *table);
  void set_version(TABLE *table);

  const uchar *key;
  uint length;
  bool null_value;
  longlong value;
  uchar table_version[MY_UUID_SIZE];
};


class Create_field;
extern bool prepare_sequence_fields(THD *thd, List<Create_field> *fields);
extern bool check_sequence_fields(LEX *lex, List<Create_field> *fields);
extern bool sequence_insert(THD *thd, LEX *lex, TABLE_LIST *table_list);
#endif 
