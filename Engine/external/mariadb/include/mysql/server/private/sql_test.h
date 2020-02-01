

#ifndef SQL_TEST_INCLUDED
#define SQL_TEST_INCLUDED

#include "mysqld.h"

class JOIN;
struct TABLE_LIST;
typedef class Item COND;
typedef class st_select_lex SELECT_LEX;
struct SORT_FIELD;

#ifndef DBUG_OFF
void print_where(COND *cond,const char *info, enum_query_type query_type);
void TEST_filesort(SORT_FIELD *sortorder,uint s_length);
void TEST_join(JOIN *join);
void print_plan(JOIN* join,uint idx, double record_count, double read_time,
                double current_read_time, const char *info);
void print_keyuse_array(DYNAMIC_ARRAY *keyuse_array);
void print_sjm(SJ_MATERIALIZATION_INFO *sjm);
void dump_TABLE_LIST_graph(SELECT_LEX *select_lex, TABLE_LIST* tl);
#endif
void mysql_print_status();

#endif 
