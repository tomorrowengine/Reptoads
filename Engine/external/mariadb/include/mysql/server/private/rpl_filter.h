

#ifndef RPL_FILTER_H
#define RPL_FILTER_H

#include "mysql.h"
#include "mysqld.h"
#include "sql_list.h"                           
#include "hash.h"                               

class String;
struct TABLE_LIST;
typedef struct st_dynamic_array DYNAMIC_ARRAY;

typedef struct st_table_rule_ent
{
  char* db;
  char* tbl_name;
  uint key_len;
} TABLE_RULE_ENT;


class Rpl_filter 
{
public:
  Rpl_filter();
  ~Rpl_filter();
  Rpl_filter(Rpl_filter const&);
  Rpl_filter& operator=(Rpl_filter const&);
 
  

#ifndef MYSQL_CLIENT
  bool tables_ok(const char* db, TABLE_LIST *tables);
#endif 
  bool db_ok(const char* db);
  bool db_ok_with_wild_table(const char *db);

  bool is_on();

  

  int add_do_table(const char* table_spec);
  int add_ignore_table(const char* table_spec);

  int set_do_table(const char* table_spec);
  int set_ignore_table(const char* table_spec);

  int add_wild_do_table(const char* table_spec);
  int add_wild_ignore_table(const char* table_spec);

  int set_wild_do_table(const char* table_spec);
  int set_wild_ignore_table(const char* table_spec);

  int add_do_db(const char* db_spec);
  int add_ignore_db(const char* db_spec);

  int set_do_db(const char* db_spec);
  int set_ignore_db(const char* db_spec);

  void set_parallel_mode(enum_slave_parallel_mode mode)
  {
    parallel_mode= mode;
  }
  
  enum_slave_parallel_mode get_parallel_mode()
  {
    return parallel_mode;
  }

  void add_db_rewrite(const char* from_db, const char* to_db);

  

  void get_do_table(String* str);
  void get_ignore_table(String* str);

  void get_wild_do_table(String* str);
  void get_wild_ignore_table(String* str);

  bool rewrite_db_is_empty();
  const char* get_rewrite_db(const char* db, size_t *new_len);
  void copy_rewrite_db(Rpl_filter *from);

  I_List<i_string>* get_do_db();
  I_List<i_string>* get_ignore_db();

  void get_do_db(String* str);
  void get_ignore_db(String* str);

private:

  void init_table_rule_hash(HASH* h, bool* h_inited);
  void init_table_rule_array(DYNAMIC_ARRAY* a, bool* a_inited);

  int add_table_rule(HASH* h, const char* table_spec);
  int add_wild_table_rule(DYNAMIC_ARRAY* a, const char* table_spec);

  typedef int (Rpl_filter::*Add_filter)(char const*);

  int parse_filter_rule(const char* spec, Add_filter func);

  void free_string_array(DYNAMIC_ARRAY *a);
  void free_string_list(I_List<i_string> *l);

  void table_rule_ent_hash_to_str(String* s, HASH* h, bool inited);
  void table_rule_ent_dynamic_array_to_str(String* s, DYNAMIC_ARRAY* a,
                                           bool inited);
  void db_rule_ent_list_to_str(String* s, I_List<i_string>* l);
  TABLE_RULE_ENT* find_wild(DYNAMIC_ARRAY *a, const char* key, int len);

  int add_string_list(I_List<i_string> *list, const char* spec);

  
  HASH do_table;
  HASH ignore_table;
  DYNAMIC_ARRAY wild_do_table;
  DYNAMIC_ARRAY wild_ignore_table;
  enum_slave_parallel_mode parallel_mode;

  bool table_rules_on;
  bool do_table_inited;
  bool ignore_table_inited;
  bool wild_do_table_inited;
  bool wild_ignore_table_inited;

  I_List<i_string> do_db;
  I_List<i_string> ignore_db;

  I_List<i_string_pair> rewrite_db;
};

extern Rpl_filter *global_rpl_filter;
extern Rpl_filter *binlog_filter;

#endif 
