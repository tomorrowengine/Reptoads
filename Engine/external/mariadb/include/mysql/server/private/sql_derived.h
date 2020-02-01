

#ifndef SQL_DERIVED_INCLUDED
#define SQL_DERIVED_INCLUDED

struct TABLE_LIST;
class THD;
struct LEX;

bool mysql_handle_derived(LEX *lex, uint phases);
bool mysql_handle_single_derived(LEX *lex, TABLE_LIST *derived, uint phases);
bool mysql_derived_reinit(THD *thd, LEX *lex, TABLE_LIST *derived);

bool pushdown_cond_for_derived(THD *thd, Item *cond, TABLE_LIST *derived);

#endif 
