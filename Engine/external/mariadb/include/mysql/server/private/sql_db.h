

#ifndef SQL_DB_INCLUDED
#define SQL_DB_INCLUDED

#include "hash.h"                               

class THD;

int mysql_create_db(THD *thd, const LEX_CSTRING *db, DDL_options_st options,
                    const Schema_specification_st *create);
bool mysql_alter_db(THD *thd, const LEX_CSTRING *db,
                    const Schema_specification_st *create);
bool mysql_rm_db(THD *thd, const LEX_CSTRING *db, bool if_exists);
bool mysql_upgrade_db(THD *thd, const LEX_CSTRING *old_db);
bool mysql_change_db(THD *thd, const LEX_CSTRING *new_db_name,
                     bool force_switch);

bool mysql_opt_change_db(THD *thd,
                         const LEX_CSTRING *new_db_name,
                         LEX_STRING *saved_db_name,
                         bool force_switch,
                         bool *cur_db_changed);
bool my_dboptions_cache_init(void);
void my_dboptions_cache_free(void);
bool check_db_dir_existence(const char *db_name);
bool load_db_opt(THD *thd, const char *path, Schema_specification_st *create);
bool load_db_opt_by_name(THD *thd, const char *db_name,
                         Schema_specification_st *db_create_info);
CHARSET_INFO *get_default_db_collation(THD *thd, const char *db_name);
bool my_dbopt_init(void);
void my_dbopt_cleanup(void);

const char *normalize_db_name(const char *db, char *buffer,
                              size_t buffer_size);

#define MY_DB_OPT_FILE "db.opt"

#endif 
