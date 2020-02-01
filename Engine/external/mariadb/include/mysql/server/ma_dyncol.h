

#ifndef ma_dyncol_h
#define ma_dyncol_h
#ifdef __cplusplus
extern "C" {
#endif

#include <decimal.h>
#include <my_decimal_limits.h>
#include <mysql_time.h>

#ifndef _my_sys_h
typedef struct st_dynamic_string
{
    char *str;
    size_t length,max_length,alloc_increment;
} DYNAMIC_STRING;
#endif

#ifndef MY_GLOBAL_INCLUDED
struct st_mysql_lex_string
{
  char *str;
  size_t length;
};
typedef struct st_mysql_lex_string MYSQL_LEX_STRING;
typedef struct st_mysql_lex_string LEX_STRING;
#endif


#define MAX_TOTAL_NAME_LENGTH 65535
#define MAX_NAME_LENGTH (MAX_TOTAL_NAME_LENGTH/4)


#define ER_DYNCOL_NO ER_DYNCOL_OK

#ifdef HAVE_CHARSET_utf8mb4
#define DYNCOL_UTF (&my_charset_utf8mb4_general_ci)
#else
#define DYNCOL_UTF (&my_charset_utf8_general_ci)
#endif


#define DYNCOL_JSON_ESC ((char)1)

enum enum_dyncol_func_result
{
  ER_DYNCOL_OK= 0,
  ER_DYNCOL_YES= 1,                
  ER_DYNCOL_FORMAT= -1,            
  ER_DYNCOL_LIMIT=  -2,            
  ER_DYNCOL_RESOURCE= -3,          
  ER_DYNCOL_DATA= -4,              
  ER_DYNCOL_UNKNOWN_CHARSET= -5,   
  ER_DYNCOL_TRUNCATED= 2           
};

typedef DYNAMIC_STRING DYNAMIC_COLUMN;

enum enum_dynamic_column_type
{
  DYN_COL_NULL= 0,
  DYN_COL_INT,
  DYN_COL_UINT,
  DYN_COL_DOUBLE,
  DYN_COL_STRING,
  DYN_COL_DECIMAL,
  DYN_COL_DATETIME,
  DYN_COL_DATE,
  DYN_COL_TIME,
  DYN_COL_DYNCOL
};

typedef enum enum_dynamic_column_type DYNAMIC_COLUMN_TYPE;

struct st_dynamic_column_value
{
  DYNAMIC_COLUMN_TYPE type;
  union
  {
    long long long_value;
    unsigned long long ulong_value;
    double double_value;
    struct {
      MYSQL_LEX_STRING value;
      CHARSET_INFO *charset;
    } string;
    struct {
      decimal_digit_t buffer[DECIMAL_BUFF_LENGTH];
      decimal_t value;
    } decimal;
    MYSQL_TIME time_value;
  } x;
};

typedef struct st_dynamic_column_value DYNAMIC_COLUMN_VALUE;

#ifdef MADYNCOL_DEPRECATED
enum enum_dyncol_func_result
dynamic_column_create(DYNAMIC_COLUMN *str,
                      uint column_nr, DYNAMIC_COLUMN_VALUE *value);

enum enum_dyncol_func_result
dynamic_column_create_many(DYNAMIC_COLUMN *str,
                           uint column_count,
                           uint *column_numbers,
                           DYNAMIC_COLUMN_VALUE *values);
enum enum_dyncol_func_result
dynamic_column_update(DYNAMIC_COLUMN *org, uint column_nr,
                      DYNAMIC_COLUMN_VALUE *value);
enum enum_dyncol_func_result
dynamic_column_update_many(DYNAMIC_COLUMN *str,
                           uint add_column_count,
                           uint *column_numbers,
                           DYNAMIC_COLUMN_VALUE *values);

enum enum_dyncol_func_result
dynamic_column_exists(DYNAMIC_COLUMN *org, uint column_nr);

enum enum_dyncol_func_result
dynamic_column_list(DYNAMIC_COLUMN *org, DYNAMIC_ARRAY *array_of_uint);

enum enum_dyncol_func_result
dynamic_column_get(DYNAMIC_COLUMN *org, uint column_nr,
                   DYNAMIC_COLUMN_VALUE *store_it_here);
#endif


enum enum_dyncol_func_result
mariadb_dyncol_create_many_num(DYNAMIC_COLUMN *str,
                               uint column_count,
                               uint *column_numbers,
                               DYNAMIC_COLUMN_VALUE *values,
                               my_bool new_string);
enum enum_dyncol_func_result
mariadb_dyncol_create_many_named(DYNAMIC_COLUMN *str,
                                 uint column_count,
                                 MYSQL_LEX_STRING *column_keys,
                                 DYNAMIC_COLUMN_VALUE *values,
                                 my_bool new_string);


enum enum_dyncol_func_result
mariadb_dyncol_update_many_num(DYNAMIC_COLUMN *str,
                               uint add_column_count,
                               uint *column_keys,
                               DYNAMIC_COLUMN_VALUE *values);
enum enum_dyncol_func_result
mariadb_dyncol_update_many_named(DYNAMIC_COLUMN *str,
                                 uint add_column_count,
                                 MYSQL_LEX_STRING *column_keys,
                                 DYNAMIC_COLUMN_VALUE *values);


enum enum_dyncol_func_result
mariadb_dyncol_exists_num(DYNAMIC_COLUMN *org, uint column_nr);
enum enum_dyncol_func_result
mariadb_dyncol_exists_named(DYNAMIC_COLUMN *str, MYSQL_LEX_STRING *name);


enum enum_dyncol_func_result
mariadb_dyncol_list_num(DYNAMIC_COLUMN *str, uint *count, uint **nums);
enum enum_dyncol_func_result
mariadb_dyncol_list_named(DYNAMIC_COLUMN *str, uint *count,
                          MYSQL_LEX_STRING **names);


enum enum_dyncol_func_result
mariadb_dyncol_get_num(DYNAMIC_COLUMN *org, uint column_nr,
                       DYNAMIC_COLUMN_VALUE *store_it_here);
enum enum_dyncol_func_result
mariadb_dyncol_get_named(DYNAMIC_COLUMN *str, MYSQL_LEX_STRING *name,
                         DYNAMIC_COLUMN_VALUE *store_it_here);

my_bool mariadb_dyncol_has_names(DYNAMIC_COLUMN *str);

enum enum_dyncol_func_result
mariadb_dyncol_check(DYNAMIC_COLUMN *str);

enum enum_dyncol_func_result
mariadb_dyncol_json(DYNAMIC_COLUMN *str, DYNAMIC_STRING *json);

#define mariadb_dyncol_init(A) memset((A), 0, sizeof(*(A)))
void mariadb_dyncol_free(DYNAMIC_COLUMN *str);


enum enum_dyncol_func_result
mariadb_dyncol_val_str(DYNAMIC_STRING *str, DYNAMIC_COLUMN_VALUE *val,
                       CHARSET_INFO *cs, my_bool quote);
enum enum_dyncol_func_result
mariadb_dyncol_val_long(longlong *ll, DYNAMIC_COLUMN_VALUE *val);
enum enum_dyncol_func_result
mariadb_dyncol_val_double(double *dbl, DYNAMIC_COLUMN_VALUE *val);


enum enum_dyncol_func_result
mariadb_dyncol_unpack(DYNAMIC_COLUMN *str,
                      uint *count,
                      MYSQL_LEX_STRING **names, DYNAMIC_COLUMN_VALUE **vals);

void mariadb_dyncol_unpack_free(MYSQL_LEX_STRING *names,
                                DYNAMIC_COLUMN_VALUE *vals);

int mariadb_dyncol_column_cmp_named(const MYSQL_LEX_STRING *s1,
                                    const MYSQL_LEX_STRING *s2);

enum enum_dyncol_func_result
mariadb_dyncol_column_count(DYNAMIC_COLUMN *str, uint *column_count);

#define mariadb_dyncol_value_init(V) (V)->type= DYN_COL_NULL


void mariadb_dyncol_prepare_decimal(DYNAMIC_COLUMN_VALUE *value);

#ifdef __cplusplus
}
#endif
#endif