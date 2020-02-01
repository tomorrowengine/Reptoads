#ifndef SQL_VIEW_INCLUDED
#define SQL_VIEW_INCLUDED




#include "sql_class.h"     
#include "sql_lex.h"       



class File_parser;




bool create_view_precheck(THD *thd, TABLE_LIST *tables, TABLE_LIST *view,
                          enum_view_create_mode mode);

bool mysql_create_view(THD *thd, TABLE_LIST *view,
                       enum_view_create_mode mode);

bool mysql_make_view(THD *thd, TABLE_SHARE *share, TABLE_LIST *table,
                     bool open_view_no_parse);


bool mysql_drop_view(THD *thd, TABLE_LIST *view, enum_drop_mode drop_mode);

bool check_key_in_view(THD *thd, TABLE_LIST * view);

bool insert_view_fields(THD *thd, List<Item> *list, TABLE_LIST *view);

int view_checksum(THD *thd, TABLE_LIST *view);
int view_check(THD *thd, TABLE_LIST *view, HA_CHECK_OPT *check_opt);
int view_repair(THD *thd, TABLE_LIST *view, HA_CHECK_OPT *check_opt);

extern TYPELIB updatable_views_with_limit_typelib;

bool check_duplicate_names(THD *thd, List<Item>& item_list,
                           bool gen_unique_view_names);
bool mysql_rename_view(THD *thd, const LEX_CSTRING *new_db, const LEX_CSTRING *new_name,
                       TABLE_LIST *view);

void make_valid_column_names(THD *thd, List<Item> &item_list);

#define VIEW_ANY_ACL (SELECT_ACL | UPDATE_ACL | INSERT_ACL | DELETE_ACL)

extern const LEX_CSTRING view_type;

void make_valid_column_names(List<Item> &item_list);

#endif 
