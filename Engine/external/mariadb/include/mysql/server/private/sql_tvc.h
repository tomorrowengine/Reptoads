

#ifndef SQL_TVC_INCLUDED
#define SQL_TVC_INCLUDED
#include "sql_type.h"

typedef List<Item> List_item;
class select_result;
class Explain_select;
class Explain_query;
class Item_func_in;
class st_select_lex_unit;
typedef class st_select_lex SELECT_LEX;


class table_value_constr : public Sql_alloc
{
public:
  List<List_item> lists_of_values;
  select_result *result;
  SELECT_LEX *select_lex;

  enum { QEP_NOT_PRESENT_YET, QEP_AVAILABLE} have_query_plan;

  Explain_select *explain;
  ulonglong select_options;
  
  table_value_constr(List<List_item> tvc_values, SELECT_LEX *sl,
		     ulonglong select_options_arg) :
    lists_of_values(tvc_values), result(0), select_lex(sl),
    have_query_plan(QEP_NOT_PRESENT_YET), explain(0),
    select_options(select_options_arg)
  { };

  ha_rows get_records() { return lists_of_values.elements; }
  
  bool prepare(THD *thd_arg, SELECT_LEX *sl, 
	       select_result *tmp_result,
	       st_select_lex_unit *unit_arg);

  int save_explain_data_intern(THD *thd_arg,
			       Explain_query *output);
  bool optimize(THD *thd_arg);
  bool exec(SELECT_LEX *sl);

  void print(THD *thd_arg, String *str, enum_query_type query_type);
};
#endif 
