

#ifndef SQL_UNION_INCLUDED
#define SQL_UNION_INCLUDED

class THD;
class select_result;
struct LEX;

typedef class st_select_lex_unit SELECT_LEX_UNIT;

bool mysql_union(THD *thd, LEX *lex, select_result *result,
                 SELECT_LEX_UNIT *unit, ulong setup_tables_done_option);


#endif 
