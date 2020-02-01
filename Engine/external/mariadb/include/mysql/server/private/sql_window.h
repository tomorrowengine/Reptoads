

#ifndef SQL_WINDOW_INCLUDED
#define SQL_WINDOW_INCLUDED

#include "filesort.h"
#include "records.h"

class Item_window_func;




class Window_frame_bound : public Sql_alloc
{

public:
 
  enum Bound_precedence_type
  {
    PRECEDING,
    CURRENT,           
    FOLLOWING
  };

  Bound_precedence_type precedence_type;
 

  
  Item *offset;

  Window_frame_bound(Bound_precedence_type prec_type,
                     Item *offset_val)
    : precedence_type(prec_type), offset(offset_val) {}

  bool is_unbounded() { return offset == NULL; }

  void print(String *str, enum_query_type query_type);

};


class Window_frame : public Sql_alloc
{
  
public:

  enum Frame_units
  {
    UNITS_ROWS,
    UNITS_RANGE
  };

  enum Frame_exclusion
  {
    EXCL_NONE,
    EXCL_CURRENT_ROW,
    EXCL_GROUP,
    EXCL_TIES
  };

  Frame_units units;

  Window_frame_bound *top_bound;

  Window_frame_bound *bottom_bound;

  Frame_exclusion exclusion;

  Window_frame(Frame_units win_frame_units,
               Window_frame_bound *win_frame_top_bound,
               Window_frame_bound *win_frame_bottom_bound,
               Frame_exclusion win_frame_exclusion)
    : units(win_frame_units), top_bound(win_frame_top_bound),
      bottom_bound(win_frame_bottom_bound), exclusion(win_frame_exclusion) {}

  bool check_frame_bounds();

  void print(String *str, enum_query_type query_type);

};

class Window_spec : public Sql_alloc
{
  bool window_names_are_checked;
 public:
  virtual ~Window_spec() {}

  LEX_CSTRING *window_ref;

  SQL_I_List<ORDER> *partition_list;

  SQL_I_List<ORDER> *order_list;

  Window_frame *window_frame;

  Window_spec *referenced_win_spec;

  Window_spec(LEX_CSTRING *win_ref, 
              SQL_I_List<ORDER> *part_list,
              SQL_I_List<ORDER> *ord_list,
              Window_frame *win_frame)
    : window_names_are_checked(false), window_ref(win_ref),
      partition_list(part_list), order_list(ord_list),
      window_frame(win_frame), referenced_win_spec(NULL) {}

  virtual const char *name() { return NULL; }

  bool check_window_names(List_iterator_fast<Window_spec> &it);

  const char *window_reference()
  {
    return window_ref ? window_ref->str : NULL;
  }

  void join_partition_and_order_lists()
  {
    *(partition_list->next)= order_list->first;
  }

  void disjoin_partition_and_order_lists()
  {
    *(partition_list->next)= NULL;
  }

  void print(String *str, enum_query_type query_type);
  void print_order(String *str, enum_query_type query_type);
  void print_partition(String *str, enum_query_type query_type);

};

class Window_def : public Window_spec
{
 public:

  LEX_CSTRING *window_name;

  Window_def(LEX_CSTRING *win_name,
             LEX_CSTRING *win_ref, 
             SQL_I_List<ORDER> *part_list,
             SQL_I_List<ORDER> *ord_list,
             Window_frame *win_frame) 
    : Window_spec(win_ref, part_list, ord_list, win_frame),
      window_name(win_name) {}
 
  const char *name() { return window_name->str; }

};

int setup_windows(THD *thd, Ref_ptr_array ref_pointer_array, TABLE_LIST *tables,
	          List<Item> &fields, List<Item> &all_fields, 
                  List<Window_spec> &win_specs, List<Item_window_func> &win_funcs);






class Frame_cursor;


class Window_func_runner : public Sql_alloc
{
public:
  
  bool add_function_to_run(Item_window_func *win_func);

  
  bool exec(THD *thd, TABLE *tbl, SORT_INFO *filesort_result);

private:
  
  List<Item_window_func> window_functions;
};




class Window_funcs_sort : public Sql_alloc
{
public:
  bool setup(THD *thd, SQL_SELECT *sel, List_iterator<Item_window_func> &it,
             st_join_table *join_tab);
  bool exec(JOIN *join, bool keep_filesort_result);
  void cleanup() { delete filesort; }

  friend class Window_funcs_computation;

private:
  Window_func_runner runner;

  
  Filesort *filesort;
};


struct st_join_table;
class Explain_aggr_window_funcs;



class Window_funcs_computation : public Sql_alloc
{
  List<Window_funcs_sort> win_func_sorts;
public:
  bool setup(THD *thd, List<Item_window_func> *window_funcs, st_join_table *tab);
  bool exec(JOIN *join, bool keep_last_filesort_result);

  Explain_aggr_window_funcs *save_explain_plan(MEM_ROOT *mem_root, bool is_analyze);
  void cleanup();
};


#endif 
