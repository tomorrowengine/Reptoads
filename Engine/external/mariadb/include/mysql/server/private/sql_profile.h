

#ifndef _SQL_PROFILE_H
#define _SQL_PROFILE_H

class Item;
struct TABLE_LIST;
class THD;
typedef struct st_field_info ST_FIELD_INFO;
typedef struct st_schema_table ST_SCHEMA_TABLE;

extern ST_FIELD_INFO query_profile_statistics_info[];
int fill_query_profile_statistics_info(THD *thd, TABLE_LIST *tables, Item *cond);
int make_profile_table_for_show(THD *thd, ST_SCHEMA_TABLE *schema_table);


#define PROFILE_NONE         (uint)0
#define PROFILE_CPU          (uint)(1<<0)
#define PROFILE_MEMORY       (uint)(1<<1)
#define PROFILE_BLOCK_IO     (uint)(1<<2)
#define PROFILE_CONTEXT      (uint)(1<<3)
#define PROFILE_PAGE_FAULTS  (uint)(1<<4)
#define PROFILE_IPC          (uint)(1<<5)
#define PROFILE_SWAPS        (uint)(1<<6)
#define PROFILE_SOURCE       (uint)(1<<16)
#define PROFILE_ALL          (uint)(~0)


#if defined(ENABLED_PROFILING)
#include "sql_priv.h"
#include "unireg.h"

#ifdef __WIN__
#include <psapi.h>
#endif

#ifdef HAVE_SYS_RESOURCE_H
#include <sys/resource.h>
#endif


class PROF_MEASUREMENT;
class QUERY_PROFILE;
class PROFILING;



template <class T> class Queue
{
private:

  struct queue_item
  {
    T *payload;
    struct queue_item *next, *previous;
  };

  struct queue_item *first, *last;

public:
  Queue()
  {
    elements= 0;
    first= last= NULL;
  }

  void empty()
  {
    struct queue_item *i, *after_i;
    for (i= first; i != NULL; i= after_i)
    {
      after_i= i->next;
      my_free(i);
    }
    elements= 0;
  }

  ulong elements;                       

  void push_back(T *payload)
  {
    struct queue_item *new_item;

    new_item= (struct queue_item *) my_malloc(sizeof(struct queue_item), MYF(0));

    new_item->payload= payload;

    if (first == NULL)
      first= new_item;
    if (last != NULL)
    {
      DBUG_ASSERT(last->next == NULL);
      last->next= new_item;
    }
    new_item->previous= last;
    new_item->next= NULL;
    last= new_item;

    elements++;
  }

  T *pop()
  {
    struct queue_item *old_item= first;
    T *ret= NULL;

    if (first == NULL)
    {
      DBUG_PRINT("warning", ("tried to pop nonexistent item from Queue"));
      return NULL;
    }

    ret= old_item->payload;
    if (first->next != NULL)
      first->next->previous= NULL;
    else
      last= NULL;
    first= first->next;

    my_free(old_item);
    elements--;

    return ret;
  }

  bool is_empty()
  {
    DBUG_ASSERT(((elements > 0) && (first != NULL)) || ((elements == 0) || (first == NULL)));
    return (elements == 0);
  }

  void *new_iterator()
  {
    return first;
  }

  void *iterator_next(void *current)
  {
    return ((struct queue_item *) current)->next;
  }

  T *iterator_value(void *current)
  {
    return ((struct queue_item *) current)->payload;
  }

};



class PROF_MEASUREMENT
{
private:
  friend class QUERY_PROFILE;
  friend class PROFILING;

  QUERY_PROFILE *profile;
  char *status;
#ifdef HAVE_GETRUSAGE
  struct rusage rusage;
#elif defined(_WIN32)
  FILETIME ftKernel, ftUser;
  IO_COUNTERS io_count;
  PROCESS_MEMORY_COUNTERS mem_count;
#endif

  char *function;
  char *file;
  unsigned int line;

  ulong m_seq;
  double time_usecs;
  char *allocated_status_memory;

  void set_label(const char *status_arg, const char *function_arg, 
                  const char *file_arg, unsigned int line_arg);
  void clean_up();
  
  PROF_MEASUREMENT();
  PROF_MEASUREMENT(QUERY_PROFILE *profile_arg, const char *status_arg);
  PROF_MEASUREMENT(QUERY_PROFILE *profile_arg, const char *status_arg,
                const char *function_arg,
                const char *file_arg, unsigned int line_arg);
  ~PROF_MEASUREMENT();
  void collect();
};



class QUERY_PROFILE
{
private:
  friend class PROFILING;

  PROFILING *profiling;

  query_id_t profiling_query_id;        
  char *query_source;

  double m_start_time_usecs;
  double m_end_time_usecs;
  ulong m_seq_counter;
  Queue<PROF_MEASUREMENT> entries;


  QUERY_PROFILE(PROFILING *profiling_arg, const char *status_arg);
  ~QUERY_PROFILE();

  void set_query_source(char *query_source_arg, size_t query_length_arg);

  
  void new_status(const char *status_arg,
              const char *function_arg,
              const char *file_arg, unsigned int line_arg);

  
  void reset();

  
  bool show(uint options);
};



class PROFILING
{
private:
  friend class PROF_MEASUREMENT;
  friend class QUERY_PROFILE;

  
  query_id_t profile_id_counter;     
  THD *thd;
  bool keeping;
  bool enabled;

  QUERY_PROFILE *current;
  QUERY_PROFILE *last;
  Queue<QUERY_PROFILE> history;
 
  query_id_t next_profile_id() { return(profile_id_counter++); }

public:
  PROFILING();
  ~PROFILING();

  
  void set_query_source(char *query_source_arg, size_t query_length_arg)
  {
    if (unlikely(current))
      current->set_query_source(query_source_arg, query_length_arg);
  }

  
  void start_new_query(const char *initial_state= "Starting")
  {
    DBUG_ASSERT(!current);
    if (unlikely(enabled))
      current= new QUERY_PROFILE(this, initial_state);
  }

  void discard_current_query();

  void finish_current_query()
  {
    if (unlikely(current))
      finish_current_query_impl();
  }

  void finish_current_query_impl();

  void status_change(const char *status_arg,
                     const char *function_arg,
                     const char *file_arg, unsigned int line_arg)
  {
    if (unlikely(current))
      current->new_status(status_arg, function_arg, file_arg, line_arg);
  }

  inline void set_thd(THD *thd_arg)
  {
    thd= thd_arg;
    reset();
  }

  
  bool show_profiles();

  
  int fill_statistics_info(THD *thd, TABLE_LIST *tables, Item *cond);
  void reset();
  void restart();
};

#  endif 
#endif 
