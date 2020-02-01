

#ifndef SQL_EXPRESSION_CACHE_INCLUDED
#define SQL_EXPRESSION_CACHE_INCLUDED

#include "sql_select.h"




extern ulong subquery_cache_miss, subquery_cache_hit;

class Expression_cache :public Sql_alloc
{
public:
  enum result {ERROR, HIT, MISS};

  Expression_cache(){};
  virtual ~Expression_cache() {};
  
  virtual result check_value(Item **value)= 0;
  
  virtual my_bool put_value(Item *value)= 0;

  
  virtual void print(String *str, enum_query_type query_type)= 0;

  
  virtual bool is_inited()= 0;
  
  virtual void init()= 0;

  
  virtual void update_tracker()= 0;
};

struct st_table_ref;
struct st_join_table;
class Item_field;


class Expression_cache_tracker :public Sql_alloc
{
public:
  enum expr_cache_state {UNINITED, STOPPED, OK};
  Expression_cache_tracker(Expression_cache *c) :
    cache(c), hit(0), miss(0), state(UNINITED)
  {}

  Expression_cache *cache;
  ulong hit, miss;
  enum expr_cache_state state;

  static const char* state_str[3];
  void set(ulong h, ulong m, enum expr_cache_state s)
  {hit= h; miss= m; state= s;}

  void fetch_current_stats()
  {
    if (cache)
      cache->update_tracker();
  }
};




class Expression_cache_tmptable :public Expression_cache
{
public:
  Expression_cache_tmptable(THD *thd, List<Item> &dependants, Item *value);
  virtual ~Expression_cache_tmptable();
  virtual result check_value(Item **value);
  virtual my_bool put_value(Item *value);

  void print(String *str, enum_query_type query_type);
  bool is_inited() { return inited; };
  void init();

  void set_tracker(Expression_cache_tracker *st)
  {
    tracker= st;
    update_tracker();
  }
  virtual void update_tracker()
  {
    if (tracker)
    {
      tracker->set(hit, miss, (inited ? (cache_table ?
                                         Expression_cache_tracker::OK :
                                         Expression_cache_tracker::STOPPED) :
                               Expression_cache_tracker::UNINITED));
    }
  }

private:
  void disable_cache();

  
  TMP_TABLE_PARAM cache_table_param;
  
  TABLE *cache_table;
  
  THD *table_thd;
  
  Expression_cache_tracker *tracker;
  
  struct st_table_ref ref;
  
  Item_field *cached_result;
  
  List<Item> &items;
  
  Item *val;
  
  ulong hit, miss;
  
  bool inited;
};

#endif 
