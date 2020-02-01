

#ifndef INNODB_PRIV_INCLUDED
#define INNODB_PRIV_INCLUDED



#include <sql_priv.h>

class THD;

int get_quote_char_for_identifier(THD *thd, const char *name, size_t length);
bool schema_table_store_record(THD *thd, TABLE *table);
void localtime_to_TIME(MYSQL_TIME *to, struct tm *from);
uint strconvert(CHARSET_INFO *from_cs, const char *from, size_t from_length,
                CHARSET_INFO *to_cs, char *to, size_t to_length,
                uint *errors);

void sql_print_error(const char *format, ...);

#define thd_binlog_pos(X, Y, Z) mysql_bin_log_commit_pos(X, Z, Y)

#endif 
