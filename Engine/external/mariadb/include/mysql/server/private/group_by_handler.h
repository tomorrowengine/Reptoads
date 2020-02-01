




struct Query
{
  List<Item> *select;
  bool        distinct;
  TABLE_LIST *from;
  Item       *where;
  ORDER      *group_by;
  ORDER      *order_by;
  Item       *having;
  
};

class group_by_handler
{
public:
  THD *thd;
  handlerton *ht;

  
  TABLE *table;

  group_by_handler(THD *thd_arg, handlerton *ht_arg)
    : thd(thd_arg), ht(ht_arg), table(0) {}
  virtual ~group_by_handler() {}

  

  
  virtual int init_scan()= 0;

  
  virtual int next_row()= 0;

  
  virtual int end_scan()=0;

  
  virtual void print_error(int error, myf errflag);
};

