#ifndef SQL_TYPE_H_INCLUDED
#define SQL_TYPE_H_INCLUDED


#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif


#include "mysqld.h"
#include "sql_array.h"
#include "sql_const.h"
#include "sql_time.h"

class Field;
class Column_definition;
class Item;
class Item_param;
class Item_cache;
class Item_func_or_sum;
class Item_sum_hybrid;
class Item_sum_sum;
class Item_sum_avg;
class Item_sum_variance;
class Item_func_hex;
class Item_hybrid_func;
class Item_func_min_max;
class Item_func_hybrid_field_type;
class Item_bool_func2;
class Item_func_between;
class Item_func_in;
class Item_func_round;
class Item_func_int_val;
class Item_func_abs;
class Item_func_neg;
class Item_func_signed;
class Item_func_unsigned;
class Item_double_typecast;
class Item_decimal_typecast;
class Item_char_typecast;
class Item_time_typecast;
class Item_date_typecast;
class Item_datetime_typecast;
class Item_func_plus;
class Item_func_minus;
class Item_func_mul;
class Item_func_div;
class Item_func_mod;
class cmp_item;
class in_vector;
class Type_handler_hybrid_field_type;
class Sort_param;
class Arg_comparator;
class Spvar_definition;
struct st_value;
class Protocol;
class handler;
struct Schema_specification_st;
struct TABLE;
struct SORT_FIELD_ATTR;
class Vers_history_point;



class Time: private MYSQL_TIME
{
public:
  enum datetime_to_time_mode_t
  {
    DATETIME_TO_TIME_YYYYMMDD_000000DD_MIX_TO_HOURS,
    DATETIME_TO_TIME_YYYYMMDD_TRUNCATE
  };
  class Options
  {
    sql_mode_t              m_get_date_flags;
    datetime_to_time_mode_t m_datetime_to_time_mode;
  public:
    Options()
     :m_get_date_flags(flags_for_get_date()),
      m_datetime_to_time_mode(DATETIME_TO_TIME_YYYYMMDD_000000DD_MIX_TO_HOURS)
    { }
    Options(sql_mode_t flags)
     :m_get_date_flags(flags),
      m_datetime_to_time_mode(DATETIME_TO_TIME_YYYYMMDD_000000DD_MIX_TO_HOURS)
    { }
    Options(sql_mode_t flags, datetime_to_time_mode_t dtmode)
     :m_get_date_flags(flags),
      m_datetime_to_time_mode(dtmode)
    { }
    sql_mode_t get_date_flags() const
    { return m_get_date_flags; }
    datetime_to_time_mode_t datetime_to_time_mode() const
    { return m_datetime_to_time_mode; }
  };
  
  class Options_for_cast: public Options
  {
  public:
    Options_for_cast()
     :Options(flags_for_get_date(), DATETIME_TO_TIME_YYYYMMDD_TRUNCATE)
    { }
  };
private:
  bool is_valid_value_slow() const
  {
    return time_type == MYSQL_TIMESTAMP_NONE || is_valid_time_slow();
  }
  bool is_valid_time_slow() const
  {
    return time_type == MYSQL_TIMESTAMP_TIME &&
           year == 0 && month == 0 && day == 0 &&
           minute <= TIME_MAX_MINUTE &&
           second <= TIME_MAX_SECOND &&
           second_part <= TIME_MAX_SECOND_PART;
  }

  
  void valid_datetime_to_valid_time(const Options opt)
  {
    DBUG_ASSERT(time_type == MYSQL_TIMESTAMP_DATE ||
                time_type == MYSQL_TIMESTAMP_DATETIME);
    
    DBUG_ASSERT(day < 32);
    DBUG_ASSERT(hour < 24);
    if (year == 0 && month == 0 &&
        opt.datetime_to_time_mode() ==
        DATETIME_TO_TIME_YYYYMMDD_000000DD_MIX_TO_HOURS)
    {
      
      hour+= day * 24;
    }
    year= month= day= 0;
    time_type= MYSQL_TIMESTAMP_TIME;
    DBUG_ASSERT(is_valid_time_slow());
  }
  
  void valid_MYSQL_TIME_to_valid_value(const Options opt)
  {
    switch (time_type) {
    case MYSQL_TIMESTAMP_DATE:
    case MYSQL_TIMESTAMP_DATETIME:
      valid_datetime_to_valid_time(opt);
      break;
    case MYSQL_TIMESTAMP_NONE:
      break;
    case MYSQL_TIMESTAMP_ERROR:
      set_zero_time(this, MYSQL_TIMESTAMP_TIME);
      break;
    case MYSQL_TIMESTAMP_TIME:
      DBUG_ASSERT(is_valid_time_slow());
      break;
    }
  }
  void make_from_item(class Item *item, const Options opt);
public:
  Time() { time_type= MYSQL_TIMESTAMP_NONE; }
  Time(Item *item) { make_from_item(item, Options()); }
  Time(Item *item, const Options opt) { make_from_item(item, opt); }
  static sql_mode_t flags_for_get_date()
  { return TIME_TIME_ONLY | TIME_INVALID_DATES; }
  static sql_mode_t comparison_flags_for_get_date()
  { return TIME_TIME_ONLY | TIME_INVALID_DATES | TIME_FUZZY_DATES; }
  bool is_valid_time() const
  {
    DBUG_ASSERT(is_valid_value_slow());
    return time_type == MYSQL_TIMESTAMP_TIME;
  }
  const MYSQL_TIME *get_mysql_time() const
  {
    DBUG_ASSERT(is_valid_time_slow());
    return this;
  }
  bool copy_to_mysql_time(MYSQL_TIME *ltime) const
  {
    if (time_type == MYSQL_TIMESTAMP_NONE)
    {
      ltime->time_type= MYSQL_TIMESTAMP_NONE;
      return true;
    }
    DBUG_ASSERT(is_valid_time_slow());
    *ltime= *this;
    return false;
  }
  int cmp(const Time *other) const
  {
    DBUG_ASSERT(is_valid_time_slow());
    DBUG_ASSERT(other->is_valid_time_slow());
    longlong p0= pack_time(this);
    longlong p1= pack_time(other);
    if (p0 < p1)
      return -1;
    if (p0 > p1)
      return 1;
    return 0;
  }
  longlong to_seconds_abs() const
  {
    DBUG_ASSERT(is_valid_time_slow());
    return hour * 3600L + minute * 60 + second;
  }
  longlong to_seconds() const
  {
    return neg ? -to_seconds_abs() : to_seconds_abs();
  }
};




class Temporal_with_date: protected MYSQL_TIME
{
protected:
  void make_from_item(THD *thd, Item *item, sql_mode_t flags);
  Temporal_with_date(THD *thd, Item *item, sql_mode_t flags)
  {
    make_from_item(thd, item, flags);
  }
};



class Date: public Temporal_with_date
{
  bool is_valid_value_slow() const
  {
    return time_type == MYSQL_TIMESTAMP_NONE || is_valid_date_slow();
  }
  bool is_valid_date_slow() const
  {
    DBUG_ASSERT(time_type == MYSQL_TIMESTAMP_DATE);
    return !check_datetime_range(this);
  }
public:
  Date(THD *thd, Item *item, sql_mode_t flags)
   :Temporal_with_date(thd, item, flags)
  {
    if (time_type == MYSQL_TIMESTAMP_DATETIME)
      datetime_to_date(this);
    DBUG_ASSERT(is_valid_value_slow());
  }
  Date(const Temporal_with_date *d)
   :Temporal_with_date(*d)
  {
    datetime_to_date(this);
    DBUG_ASSERT(is_valid_date_slow());
  }
  bool is_valid_date() const
  {
    DBUG_ASSERT(is_valid_value_slow());
    return time_type == MYSQL_TIMESTAMP_DATE;
  }
  const MYSQL_TIME *get_mysql_time() const
  {
    DBUG_ASSERT(is_valid_date_slow());
    return this;
  }
};



class Datetime: public Temporal_with_date
{
  bool is_valid_value_slow() const
  {
    return time_type == MYSQL_TIMESTAMP_NONE || is_valid_datetime_slow();
  }
  bool is_valid_datetime_slow() const
  {
    DBUG_ASSERT(time_type == MYSQL_TIMESTAMP_DATETIME);
    return !check_datetime_range(this);
  }
public:
  Datetime(THD *thd, Item *item, sql_mode_t flags)
   :Temporal_with_date(thd, item, flags)
  {
    if (time_type == MYSQL_TIMESTAMP_DATE)
      date_to_datetime(this);
    DBUG_ASSERT(is_valid_value_slow());
  }
  bool is_valid_datetime() const
  {
    
    DBUG_ASSERT(is_valid_value_slow());
    return time_type == MYSQL_TIMESTAMP_DATETIME;
  }
  bool hhmmssff_is_zero() const
  {
    DBUG_ASSERT(is_valid_datetime_slow());
    return hour == 0 && minute == 0 && second == 0 && second_part == 0;
  }
  const MYSQL_TIME *get_mysql_time() const
  {
    DBUG_ASSERT(is_valid_datetime_slow());
    return this;
  }
  bool copy_to_mysql_time(MYSQL_TIME *ltime) const
  {
    if (time_type == MYSQL_TIMESTAMP_NONE)
    {
      ltime->time_type= MYSQL_TIMESTAMP_NONE;
      return true;
    }
    DBUG_ASSERT(is_valid_datetime_slow());
    *ltime= *this;
    return false;
  }
  
  bool copy_to_mysql_time(MYSQL_TIME *ltime, timestamp_type type)
  {
    DBUG_ASSERT(type == MYSQL_TIMESTAMP_DATE ||
                type == MYSQL_TIMESTAMP_DATETIME);
    if (copy_to_mysql_time(ltime))
      return true;
    DBUG_ASSERT(type != MYSQL_TIMESTAMP_DATE || hhmmssff_is_zero());
    ltime->time_type= type;
    return false;
  }
};



#define MY_COLL_ALLOW_SUPERSET_CONV   1
#define MY_COLL_ALLOW_COERCIBLE_CONV  2
#define MY_COLL_DISALLOW_NONE         4
#define MY_COLL_ALLOW_NUMERIC_CONV    8

#define MY_COLL_ALLOW_CONV (MY_COLL_ALLOW_SUPERSET_CONV | MY_COLL_ALLOW_COERCIBLE_CONV)
#define MY_COLL_CMP_CONV   (MY_COLL_ALLOW_CONV | MY_COLL_DISALLOW_NONE)


#define my_charset_numeric      my_charset_latin1
#define MY_REPERTOIRE_NUMERIC   MY_REPERTOIRE_ASCII


enum Derivation
{
  DERIVATION_IGNORABLE= 6,
  DERIVATION_NUMERIC= 5,
  DERIVATION_COERCIBLE= 4,
  DERIVATION_SYSCONST= 3,
  DERIVATION_IMPLICIT= 2,
  DERIVATION_NONE= 1,
  DERIVATION_EXPLICIT= 0
};




class DTCollation {
public:
  CHARSET_INFO     *collation;
  enum Derivation derivation;
  uint repertoire;

  void set_repertoire_from_charset(CHARSET_INFO *cs)
  {
    repertoire= cs->state & MY_CS_PUREASCII ?
                MY_REPERTOIRE_ASCII : MY_REPERTOIRE_UNICODE30;
  }
  DTCollation()
  {
    collation= &my_charset_bin;
    derivation= DERIVATION_NONE;
    repertoire= MY_REPERTOIRE_UNICODE30;
  }
  DTCollation(CHARSET_INFO *collation_arg)
  {
    
    collation= collation_arg;
    derivation= DERIVATION_IMPLICIT;
    repertoire= my_charset_repertoire(collation_arg);
  }
  DTCollation(CHARSET_INFO *collation_arg, Derivation derivation_arg)
  {
    collation= collation_arg;
    derivation= derivation_arg;
    set_repertoire_from_charset(collation_arg);
  }
  DTCollation(CHARSET_INFO *collation_arg,
              Derivation derivation_arg,
              uint repertoire_arg)
   :collation(collation_arg),
    derivation(derivation_arg),
    repertoire(repertoire_arg)
  { }
  void set(const DTCollation &dt)
  {
    collation= dt.collation;
    derivation= dt.derivation;
    repertoire= dt.repertoire;
  }
  void set(CHARSET_INFO *collation_arg, Derivation derivation_arg)
  {
    collation= collation_arg;
    derivation= derivation_arg;
    set_repertoire_from_charset(collation_arg);
  }
  void set(CHARSET_INFO *collation_arg,
           Derivation derivation_arg,
           uint repertoire_arg)
  {
    collation= collation_arg;
    derivation= derivation_arg;
    repertoire= repertoire_arg;
  }
  void set_numeric()
  {
    collation= &my_charset_numeric;
    derivation= DERIVATION_NUMERIC;
    repertoire= MY_REPERTOIRE_NUMERIC;
  }
  void set(CHARSET_INFO *collation_arg)
  {
    collation= collation_arg;
    set_repertoire_from_charset(collation_arg);
  }
  void set(Derivation derivation_arg)
  { derivation= derivation_arg; }
  bool aggregate(const DTCollation &dt, uint flags= 0);
  bool set(DTCollation &dt1, DTCollation &dt2, uint flags= 0)
  { set(dt1); return aggregate(dt2, flags); }
  const char *derivation_name() const
  {
    switch(derivation)
    {
      case DERIVATION_NUMERIC:   return "NUMERIC";
      case DERIVATION_IGNORABLE: return "IGNORABLE";
      case DERIVATION_COERCIBLE: return "COERCIBLE";
      case DERIVATION_IMPLICIT:  return "IMPLICIT";
      case DERIVATION_SYSCONST:  return "SYSCONST";
      case DERIVATION_EXPLICIT:  return "EXPLICIT";
      case DERIVATION_NONE:      return "NONE";
      default: return "UNKNOWN";
    }
  }
  int sortcmp(const String *s, const String *t) const
  {
    return collation->coll->strnncollsp(collation,
                                        (uchar *) s->ptr(), s->length(),
                                        (uchar *) t->ptr(), t->length());
  }
};


static inline uint32
char_to_byte_length_safe(size_t char_length_arg, uint32 mbmaxlen_arg)
{
  ulonglong tmp= ((ulonglong) char_length_arg) * mbmaxlen_arg;
  return tmp > UINT_MAX32 ? (uint32) UINT_MAX32 : static_cast<uint32>(tmp);
}


class Type_std_attributes
{
public:
  DTCollation collation;
  uint decimals;
  
  uint32 max_length;
  bool unsigned_flag;
  Type_std_attributes()
   :collation(&my_charset_bin, DERIVATION_COERCIBLE),
    decimals(0), max_length(0), unsigned_flag(false)
  { }
  Type_std_attributes(const Type_std_attributes *other)
   :collation(other->collation),
    decimals(other->decimals),
    max_length(other->max_length),
    unsigned_flag(other->unsigned_flag)
  { }
  Type_std_attributes(uint32 max_length_arg, uint decimals_arg,
                      bool unsigned_flag_arg, const DTCollation &dtc)
    :collation(dtc),
     decimals(decimals_arg),
     max_length(max_length_arg),
     unsigned_flag(unsigned_flag_arg)
  { }
  void set(const Type_std_attributes *other)
  {
    *this= *other;
  }
  void set(const Type_std_attributes &other)
  {
    *this= other;
  }
  uint32 max_char_length() const
  { return max_length / collation.collation->mbmaxlen; }
  void fix_length_and_charset(uint32 max_char_length_arg, CHARSET_INFO *cs)
  {
    max_length= char_to_byte_length_safe(max_char_length_arg, cs->mbmaxlen);
    collation.collation= cs;
  }
  void fix_char_length(uint32 max_char_length_arg)
  {
    max_length= char_to_byte_length_safe(max_char_length_arg,
                                         collation.collation->mbmaxlen);
  }
  void fix_char_length_temporal_not_fixed_dec(uint int_part_length, uint dec)
  {
    uint char_length= int_part_length;
    if ((decimals= dec))
    {
      if (decimals == NOT_FIXED_DEC)
        char_length+= TIME_SECOND_PART_DIGITS + 1;
      else
      {
        set_if_smaller(decimals, TIME_SECOND_PART_DIGITS);
        char_length+= decimals + 1;
      }
    }
    fix_char_length(char_length);
  }
  void fix_attributes_temporal_not_fixed_dec(uint int_part_length, uint dec)
  {
    collation.set_numeric();
    unsigned_flag= 0;
    fix_char_length_temporal_not_fixed_dec(int_part_length, dec);
  }
  void fix_attributes_time_not_fixed_dec(uint dec)
  {
    fix_attributes_temporal_not_fixed_dec(MIN_TIME_WIDTH, dec);
  }
  void fix_attributes_datetime_not_fixed_dec(uint dec)
  {
    fix_attributes_temporal_not_fixed_dec(MAX_DATETIME_WIDTH, dec);
  }
  void fix_attributes_temporal(uint int_part_length, uint dec)
  {
    collation.set_numeric();
    unsigned_flag= 0;
    decimals= MY_MIN(dec, TIME_SECOND_PART_DIGITS);
    max_length= decimals + int_part_length + (dec ? 1 : 0);
  }
  void fix_attributes_date()
  {
    fix_attributes_temporal(MAX_DATE_WIDTH, 0);
  }
  void fix_attributes_time(uint dec)
  {
    fix_attributes_temporal(MIN_TIME_WIDTH, dec);
  }
  void fix_attributes_datetime(uint dec)
  {
    fix_attributes_temporal(MAX_DATETIME_WIDTH, dec);
  }

  void count_only_length(Item **item, uint nitems);
  void count_octet_length(Item **item, uint nitems);
  void count_real_length(Item **item, uint nitems);
  void count_decimal_length(Item **item, uint nitems);
  bool count_string_length(const char *func_name, Item **item, uint nitems);
  uint count_max_decimals(Item **item, uint nitems);

  void aggregate_attributes_int(Item **items, uint nitems)
  {
    collation.set_numeric();
    count_only_length(items, nitems);
    decimals= 0;
  }
  void aggregate_attributes_real(Item **items, uint nitems)
  {
    collation.set_numeric();
    count_real_length(items, nitems);
  }
  void aggregate_attributes_decimal(Item **items, uint nitems)
  {
    collation.set_numeric();
    count_decimal_length(items, nitems);
  }
  bool aggregate_attributes_string(const char *func_name,
                                   Item **item, uint nitems)
  {
    return count_string_length(func_name, item, nitems);
  }
  void aggregate_attributes_temporal(uint int_part_length,
                                     Item **item, uint nitems)
  {
    fix_attributes_temporal(int_part_length, count_max_decimals(item, nitems));
  }

  bool agg_item_collations(DTCollation &c, const char *name,
                           Item **items, uint nitems,
                           uint flags, int item_sep);
  bool agg_item_set_converter(const DTCollation &coll, const char *fname,
                              Item **args, uint nargs,
                              uint flags, int item_sep);

  
  bool agg_arg_charsets(DTCollation &c, const char *func_name,
                        Item **items, uint nitems,
                        uint flags, int item_sep)
  {
    if (agg_item_collations(c, func_name, items, nitems, flags, item_sep))
      return true;
    return agg_item_set_converter(c, func_name, items, nitems, flags, item_sep);
  }
  
  bool agg_arg_charsets_for_string_result(DTCollation &c, const char *func_name,
                                          Item **items, uint nitems,
                                          int item_sep)
  {
    uint flags= MY_COLL_ALLOW_SUPERSET_CONV |
                MY_COLL_ALLOW_COERCIBLE_CONV |
                MY_COLL_ALLOW_NUMERIC_CONV;
    return agg_arg_charsets(c, func_name, items, nitems, flags, item_sep);
  }
  
  bool agg_arg_charsets_for_string_result_with_comparison(DTCollation &c,
                                                          const char *func_name,
                                                          Item **items,
                                                          uint nitems,
                                                          int item_sep)
  {
    uint flags= MY_COLL_ALLOW_SUPERSET_CONV |
                MY_COLL_ALLOW_COERCIBLE_CONV |
                MY_COLL_ALLOW_NUMERIC_CONV |
                MY_COLL_DISALLOW_NONE;
    return agg_arg_charsets(c, func_name, items, nitems, flags, item_sep);
  }

  
  bool agg_arg_charsets_for_comparison(DTCollation &c,
                                       const char *func_name,
                                       Item **items, uint nitems,
                                       int item_sep)
  {
    uint flags= MY_COLL_ALLOW_SUPERSET_CONV |
                MY_COLL_ALLOW_COERCIBLE_CONV |
                MY_COLL_DISALLOW_NONE;
    return agg_arg_charsets(c, func_name, items, nitems, flags, item_sep);
  }

};


class Type_all_attributes: public Type_std_attributes
{
public:
  Type_all_attributes()
   :Type_std_attributes()
  { }
  Type_all_attributes(const Type_all_attributes *other)
   :Type_std_attributes(other)
  { }
  virtual ~Type_all_attributes() {}
  virtual void set_maybe_null(bool maybe_null_arg)= 0;
  
  virtual uint decimal_precision() const= 0;
  
  virtual uint uint_geometry_type() const= 0;
  virtual void set_geometry_type(uint type)= 0;
  virtual TYPELIB *get_typelib() const= 0;
  virtual void set_typelib(TYPELIB *typelib)= 0;
};


class Type_cast_attributes
{
  CHARSET_INFO *m_charset;
  ulonglong m_length;
  ulonglong m_decimals;
  bool m_length_specified;
  bool m_decimals_specified;
public:
  Type_cast_attributes(const char *c_len, const char *c_dec, CHARSET_INFO *cs)
    :m_charset(cs), m_length(0), m_decimals(0),
     m_length_specified(false), m_decimals_specified(false)
  {
    set_length_and_dec(c_len, c_dec);
  }
  Type_cast_attributes(CHARSET_INFO *cs)
    :m_charset(cs), m_length(0), m_decimals(0),
     m_length_specified(false), m_decimals_specified(false)
  { }
  void set_length_and_dec(const char *c_len, const char *c_dec)
  {
    int error;
    
    if ((m_length_specified= (c_len != NULL)))
      m_length= (ulonglong) my_strtoll10(c_len, NULL, &error);
    if ((m_decimals_specified= (c_dec != NULL)))
      m_decimals= (ulonglong) my_strtoll10(c_dec, NULL, &error);
  }
  CHARSET_INFO *charset() const { return m_charset; }
  bool length_specified() const { return m_length_specified; }
  bool decimals_specified() const { return m_decimals_specified; }
  ulonglong length() const { return m_length; }
  ulonglong decimals() const { return m_decimals; }
};


class Name: private LEX_CSTRING
{
public:
  Name(const char *str_arg, uint length_arg)
  {
    DBUG_ASSERT(length_arg < UINT_MAX32);
    LEX_CSTRING::str= str_arg;
    LEX_CSTRING::length= length_arg;
  }
  const char *ptr() const { return LEX_CSTRING::str; }
  uint length() const { return (uint) LEX_CSTRING::length; }
};


class Record_addr
{
public:
  uchar *ptr;      
  
  uchar *null_ptr;
  uchar null_bit;  
  Record_addr(uchar *ptr_arg,
              uchar *null_ptr_arg,
              uchar null_bit_arg)
   :ptr(ptr_arg),
    null_ptr(null_ptr_arg),
    null_bit(null_bit_arg)
  { }
  Record_addr(bool maybe_null)
   :ptr(NULL),
    null_ptr(maybe_null ? (uchar*) "" : 0),
    null_bit(0)
  { }
};


class Information_schema_numeric_attributes
{
  enum enum_attr
  {
    ATTR_NONE= 0,
    ATTR_PRECISION= 1,
    ATTR_SCALE= 2,
    ATTR_PRECISION_AND_SCALE= (ATTR_PRECISION|ATTR_SCALE)
  };
  uint m_precision;
  uint m_scale;
  enum_attr m_available_attributes;
public:
  Information_schema_numeric_attributes()
   :m_precision(0), m_scale(0),
    m_available_attributes(ATTR_NONE)
  { }
  Information_schema_numeric_attributes(uint precision)
   :m_precision(precision), m_scale(0),
    m_available_attributes(ATTR_PRECISION)
  { }
  Information_schema_numeric_attributes(uint precision, uint scale)
   :m_precision(precision), m_scale(scale),
    m_available_attributes(ATTR_PRECISION_AND_SCALE)
  { }
  bool has_precision() const { return m_available_attributes & ATTR_PRECISION; }
  bool has_scale() const { return m_available_attributes & ATTR_SCALE; }
  uint precision() const
  {
    DBUG_ASSERT(has_precision());
    return (uint) m_precision;
  }
  uint scale() const
  {
    DBUG_ASSERT(has_scale());
    return (uint) m_scale;
  }
};


class Information_schema_character_attributes
{
  uint32 m_octet_length;
  uint32 m_char_length;
  bool m_is_set;
public:
  Information_schema_character_attributes()
   :m_octet_length(0), m_char_length(0), m_is_set(false)
  { }
  Information_schema_character_attributes(uint32 octet_length,
                                          uint32 char_length)
   :m_octet_length(octet_length), m_char_length(char_length), m_is_set(true)
  { }
  bool has_octet_length() const { return m_is_set; }
  bool has_char_length() const { return m_is_set; }
  uint32 octet_length() const
  {
    DBUG_ASSERT(has_octet_length());
    return m_octet_length;
  }
  uint char_length() const
  {
    DBUG_ASSERT(has_char_length());
    return m_char_length;
  }
};


class Type_handler
{
protected:
  String *print_item_value_csstr(THD *thd, Item *item, String *str) const;
  String *print_item_value_temporal(THD *thd, Item *item, String *str,
                                     const Name &type_name, String *buf) const;
  void make_sort_key_longlong(uchar *to,
                              bool maybe_null, bool null_value,
                              bool unsigned_flag,
                              longlong value) const;
  bool
  Item_func_or_sum_illegal_param(const char *name) const;
  bool
  Item_func_or_sum_illegal_param(const Item_func_or_sum *) const;
  bool check_null(const Item *item, st_value *value) const;
  bool Item_send_str(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_tiny(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_short(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_long(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_longlong(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_float(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_double(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_time(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_date(Item *item, Protocol *protocol, st_value *buf) const;
  bool Item_send_datetime(Item *item, Protocol *protocol, st_value *buf) const;
  bool Column_definition_prepare_stage2_legacy(Column_definition *c,
                                               enum_field_types type)
                                               const;
  bool Column_definition_prepare_stage2_legacy_num(Column_definition *c,
                                                   enum_field_types type)
                                                   const;
  bool Column_definition_prepare_stage2_legacy_real(Column_definition *c,
                                                    enum_field_types type)
                                                    const;
public:
  static const Type_handler *blob_type_handler(uint max_octet_length);
  static const Type_handler *string_type_handler(uint max_octet_length);
  static const Type_handler *bit_and_int_mixture_handler(uint max_char_len);
  static const Type_handler *type_handler_long_or_longlong(uint max_char_len);
  
  static const Type_handler *varstring_type_handler(const Item *item);
  static const Type_handler *blob_type_handler(const Item *item);
  static const Type_handler *get_handler_by_field_type(enum_field_types type);
  static const Type_handler *get_handler_by_real_type(enum_field_types type);
  static const Type_handler *get_handler_by_cmp_type(Item_result type);
  static const Type_handler *get_handler_by_result_type(Item_result type)
  {
    
    DBUG_ASSERT(type != TIME_RESULT);
    return get_handler_by_cmp_type(type);
  }
  static const
  Type_handler *aggregate_for_result_traditional(const Type_handler *h1,
                                                 const Type_handler *h2);
  static const
  Type_handler *aggregate_for_num_op_traditional(const Type_handler *h1,
                                                 const Type_handler *h2);

  virtual const Name name() const= 0;
  virtual enum_field_types field_type() const= 0;
  virtual enum_field_types real_field_type() const { return field_type(); }
  virtual Item_result result_type() const= 0;
  virtual Item_result cmp_type() const= 0;
  virtual enum_mysql_timestamp_type mysql_timestamp_type() const
  {
    return MYSQL_TIMESTAMP_ERROR;
  }
  virtual bool is_timestamp_type() const
  {
    return false;
  }
  
  virtual bool type_can_have_key_part() const
  {
    return false;
  }
  virtual bool type_can_have_auto_increment_attribute() const
  {
    return false;
  }
  
  virtual bool is_param_long_data_type() const { return false; }
  virtual const Type_handler *type_handler_for_comparison() const= 0;
  virtual const Type_handler *type_handler_for_item_field() const
  {
    return this;
  }
  virtual const Type_handler *type_handler_for_tmp_table(const Item *) const
  {
    return this;
  }
  virtual const Type_handler *type_handler_for_union(const Item *) const
  {
    return this;
  }
  virtual const Type_handler *cast_to_int_type_handler() const
  {
    return this;
  }
  virtual CHARSET_INFO *charset_for_protocol(const Item *item) const;
  virtual const Type_handler*
  type_handler_adjusted_to_max_octet_length(uint max_octet_length,
                                            CHARSET_INFO *cs) const
  { return this; }
  virtual bool adjust_spparam_type(Spvar_definition *def, Item *from) const
  {
    return false;
  }
  virtual ~Type_handler() {}
  
  virtual bool is_traditional_type() const
  {
    return true;
  }
  virtual bool is_scalar_type() const { return true; }
  virtual bool can_return_int() const { return true; }
  virtual bool can_return_decimal() const { return true; }
  virtual bool can_return_real() const { return true; }
  virtual bool can_return_str() const { return true; }
  virtual bool can_return_text() const { return true; }
  virtual bool can_return_date() const { return true; }
  virtual bool can_return_time() const { return true; }
  virtual bool is_general_purpose_string_type() const { return false; }
  virtual uint Item_time_precision(Item *item) const;
  virtual uint Item_datetime_precision(Item *item) const;
  virtual uint Item_decimal_scale(const Item *item) const;
  virtual uint Item_decimal_precision(const Item *item) const= 0;
  
  virtual uint Item_divisor_precision_increment(const Item *) const;
  
  virtual Field *make_num_distinct_aggregator_field(MEM_ROOT *,
                                                    const Item *) const;
  
  virtual Field *make_conversion_table_field(TABLE *TABLE,
                                             uint metadata,
                                             const Field *target) const= 0;
  virtual bool Column_definition_fix_attributes(Column_definition *c) const= 0;
  virtual bool Column_definition_prepare_stage1(THD *thd,
                                                MEM_ROOT *mem_root,
                                                Column_definition *c,
                                                handler *file,
                                                ulonglong table_flags) const;
  
  virtual bool Column_definition_redefine_stage1(Column_definition *def,
                                                 const Column_definition *dup,
                                                 const handler *file,
                                                 const Schema_specification_st *
                                                       schema)
                                                 const;
  virtual bool Column_definition_prepare_stage2(Column_definition *c,
                                                handler *file,
                                                ulonglong table_flags) const= 0;
  virtual Field *make_table_field(const LEX_CSTRING *name,
                                  const Record_addr &addr,
                                  const Type_all_attributes &attr,
                                  TABLE *table) const= 0;
  Field *make_and_init_table_field(const LEX_CSTRING *name,
                                   const Record_addr &addr,
                                   const Type_all_attributes &attr,
                                   TABLE *table) const;
  virtual void make_sort_key(uchar *to, Item *item,
                             const SORT_FIELD_ATTR *sort_field,
                             Sort_param *param) const= 0;
  virtual void sortlength(THD *thd,
                          const Type_std_attributes *item,
                          SORT_FIELD_ATTR *attr) const= 0;

  virtual uint32 max_display_length(const Item *item) const= 0;
  virtual uint32 calc_pack_length(uint32 length) const= 0;
  virtual bool Item_save_in_value(Item *item, st_value *value) const= 0;
  virtual void Item_param_setup_conversion(THD *thd, Item_param *) const {}
  virtual void Item_param_set_param_func(Item_param *param,
                                         uchar **pos, ulong len) const;
  virtual bool Item_param_set_from_value(THD *thd,
                                         Item_param *param,
                                         const Type_all_attributes *attr,
                                         const st_value *value) const= 0;
  virtual bool Item_send(Item *item, Protocol *p, st_value *buf) const= 0;
  virtual int Item_save_in_field(Item *item, Field *field,
                                 bool no_conversions) const= 0;

  
  virtual String *print_item_value(THD *thd, Item *item, String *str) const= 0;

  
  virtual bool
  can_change_cond_ref_to_const(Item_bool_func2 *target,
                               Item *target_expr, Item *target_value,
                               Item_bool_func2 *source,
                               Item *source_expr, Item *source_const) const= 0;
  virtual bool
  subquery_type_allows_materialization(const Item *inner,
                                       const Item *outer) const= 0;
  
  virtual Item *make_const_item_for_comparison(THD *thd,
                                               Item *src,
                                               const Item *cmp) const= 0;
  virtual Item_cache *Item_get_cache(THD *thd, const Item *item) const= 0;
  virtual Item *create_typecast_item(THD *thd, Item *item,
                                     const Type_cast_attributes &attr) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  virtual bool set_comparator_func(Arg_comparator *cmp) const= 0;
  virtual bool Item_hybrid_func_fix_attributes(THD *thd,
                                               const char *name,
                                               Type_handler_hybrid_field_type *,
                                               Type_all_attributes *atrr,
                                               Item **items,
                                               uint nitems) const= 0;
  virtual bool Item_func_min_max_fix_attributes(THD *thd,
                                                Item_func_min_max *func,
                                                Item **items,
                                                uint nitems) const;
  virtual bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *) const= 0;
  virtual bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const= 0;
  virtual bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const= 0;
  virtual
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const= 0;

  virtual bool Item_val_bool(Item *item) const= 0;
  virtual bool Item_get_date(Item *item, MYSQL_TIME *ltime,
                             ulonglong fuzzydate) const= 0;
  virtual longlong Item_val_int_signed_typecast(Item *item) const= 0;
  virtual longlong Item_val_int_unsigned_typecast(Item *item) const= 0;

  virtual String *Item_func_hex_val_str_ascii(Item_func_hex *item,
                                              String *str) const= 0;

  virtual
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const= 0;
  virtual
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const= 0;
  virtual
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const= 0;
  virtual
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const= 0;
  virtual
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const= 0;
  virtual
  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const= 0;
  virtual
  double Item_func_min_max_val_real(Item_func_min_max *) const= 0;
  virtual
  longlong Item_func_min_max_val_int(Item_func_min_max *) const= 0;
  virtual
  my_decimal *Item_func_min_max_val_decimal(Item_func_min_max *,
                                            my_decimal *) const= 0;
  virtual
  bool Item_func_min_max_get_date(Item_func_min_max*,
                                  MYSQL_TIME *, ulonglong fuzzydate) const= 0;
  virtual bool
  Item_func_between_fix_length_and_dec(Item_func_between *func) const= 0;
  virtual longlong
  Item_func_between_val_int(Item_func_between *func) const= 0;

  virtual cmp_item *
  make_cmp_item(THD *thd, CHARSET_INFO *cs) const= 0;

  virtual in_vector *
  make_in_vector(THD *thd, const Item_func_in *func, uint nargs) const= 0;

  virtual bool
  Item_func_in_fix_comparator_compatible_types(THD *thd, Item_func_in *)
                                                               const= 0;

  virtual bool
  Item_func_round_fix_length_and_dec(Item_func_round *round) const= 0;

  virtual bool
  Item_func_int_val_fix_length_and_dec(Item_func_int_val *func) const= 0;

  virtual bool
  Item_func_abs_fix_length_and_dec(Item_func_abs *func) const= 0;

  virtual bool
  Item_func_neg_fix_length_and_dec(Item_func_neg *func) const= 0;

  virtual bool
  Item_func_signed_fix_length_and_dec(Item_func_signed *item) const;
  virtual bool
  Item_func_unsigned_fix_length_and_dec(Item_func_unsigned *item) const;
  virtual bool
  Item_double_typecast_fix_length_and_dec(Item_double_typecast *item) const;
  virtual bool
  Item_decimal_typecast_fix_length_and_dec(Item_decimal_typecast *item) const;
  virtual bool
  Item_char_typecast_fix_length_and_dec(Item_char_typecast *item) const;
  virtual bool
  Item_time_typecast_fix_length_and_dec(Item_time_typecast *item) const;
  virtual bool
  Item_date_typecast_fix_length_and_dec(Item_date_typecast *item) const;
  virtual bool
  Item_datetime_typecast_fix_length_and_dec(Item_datetime_typecast *item) const;

  virtual bool
  Item_func_plus_fix_length_and_dec(Item_func_plus *func) const= 0;
  virtual bool
  Item_func_minus_fix_length_and_dec(Item_func_minus *func) const= 0;
  virtual bool
  Item_func_mul_fix_length_and_dec(Item_func_mul *func) const= 0;
  virtual bool
  Item_func_div_fix_length_and_dec(Item_func_div *func) const= 0;
  virtual bool
  Item_func_mod_fix_length_and_dec(Item_func_mod *func) const= 0;

  virtual bool
  Vers_history_point_resolve_unit(THD *thd, Vers_history_point *point) const;
};



class Type_handler_row: public Type_handler
{
  static const Name m_name_row;
public:
  virtual ~Type_handler_row() {}
  const Name name() const { return m_name_row; }
  bool is_scalar_type() const { return false; }
  bool can_return_int() const { return false; }
  bool can_return_decimal() const { return false; }
  bool can_return_real() const { return false; }
  bool can_return_str() const { return false; }
  bool can_return_text() const { return false; }
  bool can_return_date() const { return false; }
  bool can_return_time() const { return false; }
  enum_field_types field_type() const
  {
    DBUG_ASSERT(0);
    return MYSQL_TYPE_NULL;
  };
  Item_result result_type() const
  {
    return ROW_RESULT;
  }
  Item_result cmp_type() const
  {
    return ROW_RESULT;
  }
  const Type_handler *type_handler_for_comparison() const;
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const
  {
    DBUG_ASSERT(0);
    return false;
  }
  Field *make_num_distinct_aggregator_field(MEM_ROOT *, const Item *) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  Field *make_conversion_table_field(TABLE *TABLE,
                                     uint metadata,
                                     const Field *target) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  bool Column_definition_fix_attributes(Column_definition *c) const
  {
    return false;
  }
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_redefine_stage1(Column_definition *def,
                                         const Column_definition *dup,
                                         const handler *file,
                                         const Schema_specification_st *schema)
                                         const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  {
    return false;
  }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  void make_sort_key(uchar *to, Item *item,
                     const SORT_FIELD_ATTR *sort_field,
                     Sort_param *param) const
  {
    DBUG_ASSERT(0);
  }
  void sortlength(THD *thd, const Type_std_attributes *item,
                            SORT_FIELD_ATTR *attr) const
  {
    DBUG_ASSERT(0);
  }
  uint32 max_display_length(const Item *item) const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  uint32 calc_pack_length(uint32 length) const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  uint Item_decimal_precision(const Item *item) const
  {
    DBUG_ASSERT(0);
    return DECIMAL_MAX_PRECISION;
  }
  bool Item_save_in_value(Item *item, st_value *value) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const
  {
    DBUG_ASSERT(0);
    return 1;
  }
  String *print_item_value(THD *thd, Item *item, String *str) const;
  bool can_change_cond_ref_to_const(Item_bool_func2 *target,
                                   Item *target_expr, Item *target_value,
                                   Item_bool_func2 *source,
                                   Item *source_expr, Item *source_const) const
  {
    DBUG_ASSERT(0);
    return false;
  }
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *func) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_val_bool(Item *item) const
  {
    DBUG_ASSERT(0);
    return false;
  }
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  longlong Item_val_int_signed_typecast(Item *item) const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  longlong Item_val_int_unsigned_typecast(Item *item) const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  String *Item_func_hex_val_str_ascii(Item_func_hex *item, String *str) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const
  {
    DBUG_ASSERT(0);
    return 0.0;
  }
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const
  {
    DBUG_ASSERT(0);
    return true;
  }

  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  double Item_func_min_max_val_real(Item_func_min_max *) const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  longlong Item_func_min_max_val_int(Item_func_min_max *) const
  {
    DBUG_ASSERT(0);
    return 0;
  }
  my_decimal *Item_func_min_max_val_decimal(Item_func_min_max *,
                                            my_decimal *) const
  {
    DBUG_ASSERT(0);
    return NULL;
  }
  bool Item_func_min_max_get_date(Item_func_min_max*,
                                  MYSQL_TIME *, ulonglong fuzzydate) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_func_between_fix_length_and_dec(Item_func_between *func) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  longlong Item_func_between_val_int(Item_func_between *func) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *thd, const Item_func_in *f, uint nargs) const;
  bool Item_func_in_fix_comparator_compatible_types(THD *thd,
                                                    Item_func_in *) const;
  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;

  bool Item_func_signed_fix_length_and_dec(Item_func_signed *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_func_unsigned_fix_length_and_dec(Item_func_unsigned *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_double_typecast_fix_length_and_dec(Item_double_typecast *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_decimal_typecast_fix_length_and_dec(Item_decimal_typecast *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_char_typecast_fix_length_and_dec(Item_char_typecast *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_time_typecast_fix_length_and_dec(Item_time_typecast *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_date_typecast_fix_length_and_dec(Item_date_typecast *) const
  {
    DBUG_ASSERT(0);
    return true;
  }
  bool Item_datetime_typecast_fix_length_and_dec(Item_datetime_typecast *) const
  {
    DBUG_ASSERT(0);
    return true;
  }

  bool Item_func_plus_fix_length_and_dec(Item_func_plus *) const;
  bool Item_func_minus_fix_length_and_dec(Item_func_minus *) const;
  bool Item_func_mul_fix_length_and_dec(Item_func_mul *) const;
  bool Item_func_div_fix_length_and_dec(Item_func_div *) const;
  bool Item_func_mod_fix_length_and_dec(Item_func_mod *) const;
};



class Type_handler_numeric: public Type_handler
{
protected:
  bool Item_sum_hybrid_fix_length_and_dec_numeric(Item_sum_hybrid *func,
                                                  const Type_handler *handler)
                                                  const;
public:
  String *print_item_value(THD *thd, Item *item, String *str) const;
  double Item_func_min_max_val_real(Item_func_min_max *) const;
  longlong Item_func_min_max_val_int(Item_func_min_max *) const;
  my_decimal *Item_func_min_max_val_decimal(Item_func_min_max *,
                                            my_decimal *) const;
  bool Item_func_min_max_get_date(Item_func_min_max*,
                                  MYSQL_TIME *, ulonglong fuzzydate) const;
  virtual ~Type_handler_numeric() { }
  bool can_change_cond_ref_to_const(Item_bool_func2 *target,
                                   Item *target_expr, Item *target_value,
                                   Item_bool_func2 *source,
                                   Item *source_expr, Item *source_const) const;
  bool Item_func_between_fix_length_and_dec(Item_func_between *func) const;
  bool Item_char_typecast_fix_length_and_dec(Item_char_typecast *) const;
};




class Type_handler_real_result: public Type_handler_numeric
{
public:
  Item_result result_type() const { return REAL_RESULT; }
  Item_result cmp_type() const { return REAL_RESULT; }
  virtual ~Type_handler_real_result() {}
  const Type_handler *type_handler_for_comparison() const;
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const;
  void make_sort_key(uchar *to, Item *item, const SORT_FIELD_ATTR *sort_field,
                     Sort_param *param) const;
  void sortlength(THD *thd,
                  const Type_std_attributes *item,
                  SORT_FIELD_ATTR *attr) const;
  uint Item_decimal_precision(const Item *item) const;
  bool Item_save_in_value(Item *item, st_value *value) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const;
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  bool Item_func_min_max_fix_attributes(THD *thd, Item_func_min_max *func,
                                        Item **items, uint nitems) const;
  bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *func) const;
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const;
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const;
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const;
  bool Item_func_signed_fix_length_and_dec(Item_func_signed *item) const;
  bool Item_func_unsigned_fix_length_and_dec(Item_func_unsigned *item) const;
  bool Item_val_bool(Item *item) const;
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const;
  longlong Item_val_int_signed_typecast(Item *item) const;
  longlong Item_val_int_unsigned_typecast(Item *item) const;
  String *Item_func_hex_val_str_ascii(Item_func_hex *item, String *str) const;
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const;
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const;
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const;
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const;
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const;
  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const;
  longlong Item_func_between_val_int(Item_func_between *func) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *, const Item_func_in *, uint nargs) const;
  bool Item_func_in_fix_comparator_compatible_types(THD *thd,
                                                    Item_func_in *) const;

  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;
  bool Item_func_plus_fix_length_and_dec(Item_func_plus *) const;
  bool Item_func_minus_fix_length_and_dec(Item_func_minus *) const;
  bool Item_func_mul_fix_length_and_dec(Item_func_mul *) const;
  bool Item_func_div_fix_length_and_dec(Item_func_div *) const;
  bool Item_func_mod_fix_length_and_dec(Item_func_mod *) const;
};


class Type_handler_decimal_result: public Type_handler_numeric
{
public:
  Item_result result_type() const { return DECIMAL_RESULT; }
  Item_result cmp_type() const { return DECIMAL_RESULT; }
  virtual ~Type_handler_decimal_result() {};
  const Type_handler *type_handler_for_comparison() const;
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const;
  Field *make_num_distinct_aggregator_field(MEM_ROOT *, const Item *) const;
  void make_sort_key(uchar *to, Item *item, const SORT_FIELD_ATTR *sort_field,
                     Sort_param *param) const;
  void sortlength(THD *thd,
                  const Type_std_attributes *item,
                  SORT_FIELD_ATTR *attr) const;
  uint32 max_display_length(const Item *item) const;
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  uint Item_decimal_precision(const Item *item) const;
  bool Item_save_in_value(Item *item, st_value *value) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_str(item, protocol, buf);
  }
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const;
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *func) const;
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const;
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const;
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const;
  bool Item_val_bool(Item *item) const;
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const;
  longlong Item_val_int_signed_typecast(Item *item) const;
  longlong Item_val_int_unsigned_typecast(Item *item) const;
  String *Item_func_hex_val_str_ascii(Item_func_hex *item, String *str) const;
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const;
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const;
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const;
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const;
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const;
  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const;
  longlong Item_func_between_val_int(Item_func_between *func) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *, const Item_func_in *, uint nargs) const;
  bool Item_func_in_fix_comparator_compatible_types(THD *thd,
                                                    Item_func_in *) const;
  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;
  bool Item_func_plus_fix_length_and_dec(Item_func_plus *) const;
  bool Item_func_minus_fix_length_and_dec(Item_func_minus *) const;
  bool Item_func_mul_fix_length_and_dec(Item_func_mul *) const;
  bool Item_func_div_fix_length_and_dec(Item_func_div *) const;
  bool Item_func_mod_fix_length_and_dec(Item_func_mod *) const;
};


class Type_limits_int
{
private:
  uint32 m_precision;
  uint32 m_char_length;
public:
  Type_limits_int(uint32 prec, uint32 nchars)
   :m_precision(prec), m_char_length(nchars)
  { }
  uint32 precision() const { return m_precision; }
  uint32 char_length() const { return m_char_length; }
};



class Type_limits_uint8: public Type_limits_int
{
public:
  Type_limits_uint8()
   :Type_limits_int(MAX_TINYINT_WIDTH, MAX_TINYINT_WIDTH)
  { }
};


class Type_limits_sint8: public Type_limits_int
{
public:
  Type_limits_sint8()
   :Type_limits_int(MAX_TINYINT_WIDTH, MAX_TINYINT_WIDTH + 1)
  { }
};



class Type_limits_uint16: public Type_limits_int
{
public:
  Type_limits_uint16()
   :Type_limits_int(MAX_SMALLINT_WIDTH, MAX_SMALLINT_WIDTH)
  { }
};


class Type_limits_sint16: public Type_limits_int
{
public:
  Type_limits_sint16()
   :Type_limits_int(MAX_SMALLINT_WIDTH, MAX_SMALLINT_WIDTH + 1)
  { }
};



class Type_limits_uint24: public Type_limits_int
{
public:
  Type_limits_uint24()
   :Type_limits_int(MAX_MEDIUMINT_WIDTH, MAX_MEDIUMINT_WIDTH)
  { }
};


class Type_limits_sint24: public Type_limits_int
{
public:
  Type_limits_sint24()
   :Type_limits_int(MAX_MEDIUMINT_WIDTH - 1, MAX_MEDIUMINT_WIDTH)
  { }
};



class Type_limits_uint32: public Type_limits_int
{
public:
  Type_limits_uint32()
   :Type_limits_int(MAX_INT_WIDTH, MAX_INT_WIDTH)
  { }
};



class Type_limits_sint32: public Type_limits_int
{
public:
  Type_limits_sint32()
   :Type_limits_int(MAX_INT_WIDTH, MAX_INT_WIDTH + 1)
  { }
};



class Type_limits_uint64: public Type_limits_int
{
public:
  Type_limits_uint64(): Type_limits_int(MAX_BIGINT_WIDTH, MAX_BIGINT_WIDTH)
  { }
};


class Type_limits_sint64: public Type_limits_int
{
public:
  Type_limits_sint64()
   :Type_limits_int(MAX_BIGINT_WIDTH - 1, MAX_BIGINT_WIDTH)
  { }
};



class Type_handler_int_result: public Type_handler_numeric
{
public:
  Item_result result_type() const { return INT_RESULT; }
  Item_result cmp_type() const { return INT_RESULT; }
  virtual ~Type_handler_int_result() {}
  const Type_handler *type_handler_for_comparison() const;
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const;
  Field *make_num_distinct_aggregator_field(MEM_ROOT *, const Item *) const;
  void make_sort_key(uchar *to, Item *item, const SORT_FIELD_ATTR *sort_field,
                     Sort_param *param) const;
  void sortlength(THD *thd,
                  const Type_std_attributes *item,
                  SORT_FIELD_ATTR *attr) const;
  uint Item_decimal_precision(const Item *item) const;
  bool Item_save_in_value(Item *item, st_value *value) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const;
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *func) const;
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const;
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const;
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const;
  bool Item_val_bool(Item *item) const;
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const;
  longlong Item_val_int_signed_typecast(Item *item) const;
  longlong Item_val_int_unsigned_typecast(Item *item) const;
  String *Item_func_hex_val_str_ascii(Item_func_hex *item, String *str) const;
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const;
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const;
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const;
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const;
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const;
  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const;
  longlong Item_func_between_val_int(Item_func_between *func) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *, const Item_func_in *, uint nargs) const;
  bool Item_func_in_fix_comparator_compatible_types(THD *thd,
                                                    Item_func_in *) const;
  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;
  bool Item_func_plus_fix_length_and_dec(Item_func_plus *) const;
  bool Item_func_minus_fix_length_and_dec(Item_func_minus *) const;
  bool Item_func_mul_fix_length_and_dec(Item_func_mul *) const;
  bool Item_func_div_fix_length_and_dec(Item_func_div *) const;
  bool Item_func_mod_fix_length_and_dec(Item_func_mod *) const;
};


class Type_handler_general_purpose_int: public Type_handler_int_result
{
public:
  bool type_can_have_auto_increment_attribute() const { return true; }
  virtual const Type_limits_int *
    type_limits_int_by_unsigned_flag(bool unsigned_flag) const= 0;
  uint32 max_display_length(const Item *item) const;
  bool Vers_history_point_resolve_unit(THD *thd, Vers_history_point *p) const;
};


class Type_handler_temporal_result: public Type_handler
{
protected:
  uint Item_decimal_scale_with_seconds(const Item *item) const;
  uint Item_divisor_precision_increment_with_seconds(const Item *) const;
public:
  Item_result result_type() const { return STRING_RESULT; }
  Item_result cmp_type() const { return TIME_RESULT; }
  virtual ~Type_handler_temporal_result() {}
  void make_sort_key(uchar *to, Item *item,  const SORT_FIELD_ATTR *sort_field,
                     Sort_param *param) const;
  void sortlength(THD *thd,
                  const Type_std_attributes *item,
                  SORT_FIELD_ATTR *attr) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  uint32 max_display_length(const Item *item) const;
  bool can_change_cond_ref_to_const(Item_bool_func2 *target,
                                   Item *target_expr, Item *target_value,
                                   Item_bool_func2 *source,
                                   Item *source_expr, Item *source_const) const;
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const;
  bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *func) const;
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const;
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const;
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const;
  bool Item_val_bool(Item *item) const;
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const;
  longlong Item_val_int_signed_typecast(Item *item) const;
  longlong Item_val_int_unsigned_typecast(Item *item) const;
  String *Item_func_hex_val_str_ascii(Item_func_hex *item, String *str) const;
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const;
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const;
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const;
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const;
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const;
  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const;
  double Item_func_min_max_val_real(Item_func_min_max *) const;
  longlong Item_func_min_max_val_int(Item_func_min_max *) const;
  my_decimal *Item_func_min_max_val_decimal(Item_func_min_max *,
                                            my_decimal *) const;
  bool Item_func_min_max_get_date(Item_func_min_max*,
                                  MYSQL_TIME *, ulonglong fuzzydate) const;
  bool Item_func_between_fix_length_and_dec(Item_func_between *func) const;
  longlong Item_func_between_val_int(Item_func_between *func) const;
  bool Item_func_in_fix_comparator_compatible_types(THD *thd,
                                                    Item_func_in *) const;
  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;
  bool Item_func_plus_fix_length_and_dec(Item_func_plus *) const;
  bool Item_func_minus_fix_length_and_dec(Item_func_minus *) const;
  bool Item_func_mul_fix_length_and_dec(Item_func_mul *) const;
  bool Item_func_div_fix_length_and_dec(Item_func_div *) const;
  bool Item_func_mod_fix_length_and_dec(Item_func_mod *) const;
  bool Vers_history_point_resolve_unit(THD *thd, Vers_history_point *p) const;
};


class Type_handler_string_result: public Type_handler
{
  uint Item_temporal_precision(Item *item, bool is_time) const;
public:
  Item_result result_type() const { return STRING_RESULT; }
  Item_result cmp_type() const { return STRING_RESULT; }
  CHARSET_INFO *charset_for_protocol(const Item *item) const;
  virtual ~Type_handler_string_result() {}
  const Type_handler *type_handler_for_comparison() const;
  const Type_handler *
  type_handler_adjusted_to_max_octet_length(uint max_octet_length,
                                            CHARSET_INFO *cs) const;
  void make_sort_key(uchar *to, Item *item, const SORT_FIELD_ATTR *sort_field,
                     Sort_param *param) const;
  void sortlength(THD *thd,
                  const Type_std_attributes *item,
                  SORT_FIELD_ATTR *attr) const;
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_redefine_stage1(Column_definition *def,
                                         const Column_definition *dup,
                                         const handler *file,
                                         const Schema_specification_st *schema)
                                         const;
  uint32 max_display_length(const Item *item) const;
  uint Item_time_precision(Item *item) const
  {
    return Item_temporal_precision(item, true);
  }
  uint Item_datetime_precision(Item *item) const
  {
    return Item_temporal_precision(item, false);
  }
  uint Item_decimal_precision(const Item *item) const;
  bool Item_save_in_value(Item *item, st_value *value) const;
  void Item_param_setup_conversion(THD *thd, Item_param *) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_str(item, protocol, buf);
  }
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const;
  String *print_item_value(THD *thd, Item *item, String *str) const
  {
    return print_item_value_csstr(thd, item, str);
  }
  bool can_change_cond_ref_to_const(Item_bool_func2 *target,
                                   Item *target_expr, Item *target_value,
                                   Item_bool_func2 *source,
                                   Item *source_expr, Item *source_const) const;
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const;
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  bool Item_sum_hybrid_fix_length_and_dec(Item_sum_hybrid *func) const;
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const;
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const;
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const;
  bool Item_func_signed_fix_length_and_dec(Item_func_signed *item) const;
  bool Item_func_unsigned_fix_length_and_dec(Item_func_unsigned *item) const;
  bool Item_val_bool(Item *item) const;
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const;
  longlong Item_val_int_signed_typecast(Item *item) const;
  longlong Item_val_int_unsigned_typecast(Item *item) const;
  String *Item_func_hex_val_str_ascii(Item_func_hex *item, String *str) const;
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const;
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const;
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const;
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const;
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const;
  String *Item_func_min_max_val_str(Item_func_min_max *, String *) const;
  double Item_func_min_max_val_real(Item_func_min_max *) const;
  longlong Item_func_min_max_val_int(Item_func_min_max *) const;
  my_decimal *Item_func_min_max_val_decimal(Item_func_min_max *,
                                            my_decimal *) const;
  bool Item_func_min_max_get_date(Item_func_min_max*,
                                  MYSQL_TIME *, ulonglong fuzzydate) const;
  bool Item_func_between_fix_length_and_dec(Item_func_between *func) const;
  longlong Item_func_between_val_int(Item_func_between *func) const;
  bool Item_char_typecast_fix_length_and_dec(Item_char_typecast *) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *, const Item_func_in *, uint nargs) const;
  bool Item_func_in_fix_comparator_compatible_types(THD *thd,
                                                    Item_func_in *) const;
  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;
  bool Item_func_plus_fix_length_and_dec(Item_func_plus *) const;
  bool Item_func_minus_fix_length_and_dec(Item_func_minus *) const;
  bool Item_func_mul_fix_length_and_dec(Item_func_mul *) const;
  bool Item_func_div_fix_length_and_dec(Item_func_div *) const;
  bool Item_func_mod_fix_length_and_dec(Item_func_mod *) const;
};


class Type_handler_general_purpose_string: public Type_handler_string_result
{
public:
  bool is_general_purpose_string_type() const { return true; }
  bool Vers_history_point_resolve_unit(THD *thd, Vers_history_point *p) const;
};





class Type_handler_tiny: public Type_handler_general_purpose_int
{
  static const Name m_name_tiny;
  static const Type_limits_int m_limits_sint8;
  static const Type_limits_int m_limits_uint8;
public:
  virtual ~Type_handler_tiny() {}
  const Name name() const { return m_name_tiny; }
  enum_field_types field_type() const { return MYSQL_TYPE_TINY; }
  const Type_limits_int *type_limits_int_by_unsigned_flag(bool unsigned_fl) const
  {
    return unsigned_fl ? &m_limits_uint8 : &m_limits_sint8;
  }
  uint32 calc_pack_length(uint32 length) const { return 1; }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_tiny(item, protocol, buf);
  }
  Field *make_conversion_table_field(TABLE *TABLE, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_TINY); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_short: public Type_handler_general_purpose_int
{
  static const Name m_name_short;
  static const Type_limits_int m_limits_sint16;
  static const Type_limits_int m_limits_uint16;
public:
  virtual ~Type_handler_short() {}
  const Name name() const { return m_name_short; }
  enum_field_types field_type() const { return MYSQL_TYPE_SHORT; }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_short(item, protocol, buf);
  }
  const Type_limits_int *type_limits_int_by_unsigned_flag(bool unsigned_fl) const
  {
    return unsigned_fl ? &m_limits_uint16 : &m_limits_sint16;
  }
  uint32 calc_pack_length(uint32 length) const { return 2; }
  Field *make_conversion_table_field(TABLE *TABLE, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_SHORT); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_long: public Type_handler_general_purpose_int
{
  static const Name m_name_int;
  static const Type_limits_int m_limits_sint32;
  static const Type_limits_int m_limits_uint32;
public:
  virtual ~Type_handler_long() {}
  const Name name() const { return m_name_int; }
  enum_field_types field_type() const { return MYSQL_TYPE_LONG; }
  const Type_limits_int *type_limits_int_by_unsigned_flag(bool unsigned_fl) const
  {
    return unsigned_fl ? &m_limits_uint32 : &m_limits_sint32;
  }
  uint32 calc_pack_length(uint32 length) const { return 4; }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_long(item, protocol, buf);
  }
  Field *make_conversion_table_field(TABLE *TABLE, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_LONG); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_longlong: public Type_handler_general_purpose_int
{
  static const Name m_name_longlong;
  static const Type_limits_int m_limits_sint64;
  static const Type_limits_int m_limits_uint64;
public:
  virtual ~Type_handler_longlong() {}
  const Name name() const { return m_name_longlong; }
  enum_field_types field_type() const { return MYSQL_TYPE_LONGLONG; }
  const Type_limits_int *type_limits_int_by_unsigned_flag(bool unsigned_fl) const
  {
    return unsigned_fl ? &m_limits_uint64 : &m_limits_sint64;
  }
  uint32 calc_pack_length(uint32 length) const { return 8; }
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_longlong(item, protocol, buf);
  }
  Field *make_conversion_table_field(TABLE *TABLE, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  {
    return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_LONGLONG);
  }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_vers_trx_id: public Type_handler_longlong
{
public:
  virtual ~Type_handler_vers_trx_id() {}
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_int24: public Type_handler_general_purpose_int
{
  static const Name m_name_mediumint;
  static const Type_limits_int m_limits_sint24;
  static const Type_limits_int m_limits_uint24;
public:
  virtual ~Type_handler_int24() {}
  const Name name() const { return m_name_mediumint; }
  enum_field_types field_type() const { return MYSQL_TYPE_INT24; }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_long(item, protocol, buf);
  }
  const Type_limits_int *type_limits_int_by_unsigned_flag(bool unsigned_fl) const
  {
    return unsigned_fl ? &m_limits_uint24 : &m_limits_sint24;
  }
  uint32 calc_pack_length(uint32 length) const { return 3; }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_INT24); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_year: public Type_handler_int_result
{
  static const Name m_name_year;
public:
  virtual ~Type_handler_year() {}
  const Name name() const { return m_name_year; }
  enum_field_types field_type() const { return MYSQL_TYPE_YEAR; }
  uint32 max_display_length(const Item *item) const;
  uint32 calc_pack_length(uint32 length) const { return 1; }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_short(item, protocol, buf);
  }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_YEAR); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool Item_get_date(Item *item, MYSQL_TIME *ltime, ulonglong fuzzydate) const;
};


class Type_handler_bit: public Type_handler_int_result
{
  static const Name m_name_bit;
public:
  virtual ~Type_handler_bit() {}
  const Name name() const { return m_name_bit; }
  enum_field_types field_type() const { return MYSQL_TYPE_BIT; }
  uint32 max_display_length(const Item *item) const;
  uint32 calc_pack_length(uint32 length) const { return length / 8; }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_str(item, protocol, buf);
  }
  String *print_item_value(THD *thd, Item *item, String *str) const
  {
    return print_item_value_csstr(thd, item, str);
  }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_redefine_stage1(Column_definition *def,
                                         const Column_definition *dup,
                                         const handler *file,
                                         const Schema_specification_st *schema)
                                         const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  bool Vers_history_point_resolve_unit(THD *thd, Vers_history_point *p) const;
};


class Type_handler_float: public Type_handler_real_result
{
  static const Name m_name_float;
public:
  virtual ~Type_handler_float() {}
  const Name name() const { return m_name_float; }
  enum_field_types field_type() const { return MYSQL_TYPE_FLOAT; }
  bool type_can_have_auto_increment_attribute() const { return true; }
  uint32 max_display_length(const Item *item) const { return 25; }
  uint32 calc_pack_length(uint32 length) const { return sizeof(float); }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_float(item, protocol, buf);
  }
  Field *make_num_distinct_aggregator_field(MEM_ROOT *, const Item *) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_real(c, MYSQL_TYPE_FLOAT); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_double: public Type_handler_real_result
{
  static const Name m_name_double;
public:
  virtual ~Type_handler_double() {}
  const Name name() const { return m_name_double; }
  enum_field_types field_type() const { return MYSQL_TYPE_DOUBLE; }
  bool type_can_have_auto_increment_attribute() const { return true; }
  uint32 max_display_length(const Item *item) const { return 53; }
  uint32 calc_pack_length(uint32 length) const { return sizeof(double); }
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_double(item, protocol, buf);
  }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_real(c, MYSQL_TYPE_DOUBLE); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_time_common: public Type_handler_temporal_result
{
  static const Name m_name_time;
public:
  virtual ~Type_handler_time_common() { }
  const Name name() const { return m_name_time; }
  enum_field_types field_type() const { return MYSQL_TYPE_TIME; }
  enum_mysql_timestamp_type mysql_timestamp_type() const
  {
    return MYSQL_TIMESTAMP_TIME;
  }
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  uint Item_decimal_scale(const Item *item) const
  {
    return Item_decimal_scale_with_seconds(item);
  }
  uint Item_decimal_precision(const Item *item) const;
  uint Item_divisor_precision_increment(const Item *item) const
  {
    return Item_divisor_precision_increment_with_seconds(item);
  }
  const Type_handler *type_handler_for_comparison() const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Item_save_in_value(Item *item, st_value *value) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_time(item, protocol, buf);
  }
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const;
  String *print_item_value(THD *thd, Item *item, String *str) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  String *Item_func_hybrid_field_type_val_str(Item_func_hybrid_field_type *,
                                              String *) const;
  double Item_func_hybrid_field_type_val_real(Item_func_hybrid_field_type *)
                                              const;
  longlong Item_func_hybrid_field_type_val_int(Item_func_hybrid_field_type *)
                                               const;
  my_decimal *Item_func_hybrid_field_type_val_decimal(
                                              Item_func_hybrid_field_type *,
                                              my_decimal *) const;
  bool Item_func_hybrid_field_type_get_date(Item_func_hybrid_field_type *,
                                            MYSQL_TIME *,
                                            ulonglong fuzzydate) const;
  bool Item_func_min_max_get_date(Item_func_min_max*,
                                  MYSQL_TIME *, ulonglong fuzzydate) const;
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *, const Item_func_in *, uint nargs) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_time: public Type_handler_time_common
{
  
  static uint m_hires_bytes[MAX_DATETIME_PRECISION+1];
public:
  static uint hires_bytes(uint dec) { return m_hires_bytes[dec]; }
  virtual ~Type_handler_time() {}
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_TIME); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_time2: public Type_handler_time_common
{
public:
  virtual ~Type_handler_time2() {}
  enum_field_types real_field_type() const { return MYSQL_TYPE_TIME2; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_TIME2); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_temporal_with_date: public Type_handler_temporal_result
{
public:
  virtual ~Type_handler_temporal_with_date() {}
  bool Item_save_in_value(Item *item, st_value *value) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_date(item, protocol, buf);
  }
  int Item_save_in_field(Item *item, Field *field, bool no_conversions) const;
  Item *make_const_item_for_comparison(THD *, Item *src, const Item *cmp) const;
  bool set_comparator_func(Arg_comparator *cmp) const;
  cmp_item *make_cmp_item(THD *thd, CHARSET_INFO *cs) const;
  in_vector *make_in_vector(THD *, const Item_func_in *, uint nargs) const;
};


class Type_handler_date_common: public Type_handler_temporal_with_date
{
  static const Name m_name_date;
public:
  virtual ~Type_handler_date_common() {}
  const Name name() const { return m_name_date; }
  const Type_handler *type_handler_for_comparison() const;
  enum_field_types field_type() const { return MYSQL_TYPE_DATE; }
  enum_mysql_timestamp_type mysql_timestamp_type() const
  {
    return MYSQL_TIMESTAMP_DATE;
  }
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  uint Item_decimal_precision(const Item *item) const;
  String *print_item_value(THD *thd, Item *item, String *str) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};

class Type_handler_date: public Type_handler_date_common
{
public:
  virtual ~Type_handler_date() {}
  uint32 calc_pack_length(uint32 length) const { return 4; }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_DATE); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_newdate: public Type_handler_date_common
{
public:
  virtual ~Type_handler_newdate() {}
  enum_field_types real_field_type() const { return MYSQL_TYPE_NEWDATE; }
  uint32 calc_pack_length(uint32 length) const { return 3; }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_NEWDATE); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_datetime_common: public Type_handler_temporal_with_date
{
  static const Name m_name_datetime;
public:
  virtual ~Type_handler_datetime_common() {}
  const Name name() const { return m_name_datetime; }
  const Type_handler *type_handler_for_comparison() const;
  enum_field_types field_type() const { return MYSQL_TYPE_DATETIME; }
  enum_mysql_timestamp_type mysql_timestamp_type() const
  {
    return MYSQL_TIMESTAMP_DATETIME;
  }
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  uint Item_decimal_scale(const Item *item) const
  {
    return Item_decimal_scale_with_seconds(item);
  }
  uint Item_decimal_precision(const Item *item) const;
  uint Item_divisor_precision_increment(const Item *item) const
  {
    return Item_divisor_precision_increment_with_seconds(item);
  }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_datetime(item, protocol, buf);
  }
  String *print_item_value(THD *thd, Item *item, String *str) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_datetime: public Type_handler_datetime_common
{
  
  static uint m_hires_bytes[MAX_DATETIME_PRECISION + 1];
public:
  static uint hires_bytes(uint dec) { return m_hires_bytes[dec]; }
  virtual ~Type_handler_datetime() {}
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_DATETIME); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_datetime2: public Type_handler_datetime_common
{
public:
  virtual ~Type_handler_datetime2() {}
  enum_field_types real_field_type() const { return MYSQL_TYPE_DATETIME2; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_DATETIME2); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_timestamp_common: public Type_handler_temporal_with_date
{
  static const Name m_name_timestamp;
public:
  virtual ~Type_handler_timestamp_common() {}
  const Name name() const { return m_name_timestamp; }
  const Type_handler *type_handler_for_comparison() const;
  enum_field_types field_type() const { return MYSQL_TYPE_TIMESTAMP; }
  enum_mysql_timestamp_type mysql_timestamp_type() const
  {
    return MYSQL_TIMESTAMP_DATETIME;
  }
  bool is_timestamp_type() const
  {
    return true;
  }
  bool Column_definition_fix_attributes(Column_definition *c) const;
  uint Item_decimal_scale(const Item *item) const
  {
    return Item_decimal_scale_with_seconds(item);
  }
  uint Item_decimal_precision(const Item *item) const;
  uint Item_divisor_precision_increment(const Item *item) const
  {
    return Item_divisor_precision_increment_with_seconds(item);
  }
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const
  {
    return Item_send_datetime(item, protocol, buf);
  }
  String *print_item_value(THD *thd, Item *item, String *str) const;
  Item_cache *Item_get_cache(THD *thd, const Item *item) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
};


class Type_handler_timestamp: public Type_handler_timestamp_common
{
  
  static uint m_sec_part_bytes[MAX_DATETIME_PRECISION + 1];
public:
  static uint sec_part_bytes(uint dec) { return m_sec_part_bytes[dec]; }
  virtual ~Type_handler_timestamp() {}
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_TIMESTAMP); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_timestamp2: public Type_handler_timestamp_common
{
public:
  virtual ~Type_handler_timestamp2() {}
  enum_field_types real_field_type() const { return MYSQL_TYPE_TIMESTAMP2; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  {
    return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_TIMESTAMP2);
  }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_olddecimal: public Type_handler_decimal_result
{
  static const Name m_name_decimal;
public:
  virtual ~Type_handler_olddecimal() {}
  const Name name() const { return m_name_decimal; }
  enum_field_types field_type() const { return MYSQL_TYPE_DECIMAL; }
  uint32 calc_pack_length(uint32 length) const { return length; }
  const Type_handler *type_handler_for_tmp_table(const Item *item) const;
  const Type_handler *type_handler_for_union(const Item *item) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_DECIMAL); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_newdecimal: public Type_handler_decimal_result
{
  static const Name m_name_decimal;
public:
  virtual ~Type_handler_newdecimal() {}
  const Name name() const { return m_name_decimal; }
  enum_field_types field_type() const { return MYSQL_TYPE_NEWDECIMAL; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_redefine_stage1(Column_definition *def,
                                         const Column_definition *dup,
                                         const handler *file,
                                         const Schema_specification_st *schema)
                                         const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_null: public Type_handler_general_purpose_string
{
  static const Name m_name_null;
public:
  virtual ~Type_handler_null() {}
  const Name name() const { return m_name_null; }
  enum_field_types field_type() const { return MYSQL_TYPE_NULL; }
  const Type_handler *type_handler_for_comparison() const;
  const Type_handler *type_handler_for_tmp_table(const Item *item) const;
  const Type_handler *type_handler_for_union(const Item *) const;
  uint32 max_display_length(const Item *item) const { return 0; }
  uint32 calc_pack_length(uint32 length) const { return 0; }
  bool Item_save_in_value(Item *item, st_value *value) const;
  bool Item_send(Item *item, Protocol *protocol, st_value *buf) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_redefine_stage1(Column_definition *def,
                                         const Column_definition *dup,
                                         const handler *file,
                                         const Schema_specification_st *schema)
                                         const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy(c, MYSQL_TYPE_NULL); }
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_longstr: public Type_handler_general_purpose_string
{
public:
  bool type_can_have_key_part() const
  {
    return true;
  }
};


class Type_handler_string: public Type_handler_longstr
{
  static const Name m_name_char;
public:
  virtual ~Type_handler_string() {}
  const Name name() const { return m_name_char; }
  enum_field_types field_type() const { return MYSQL_TYPE_STRING; }
  bool is_param_long_data_type() const { return true; }
  uint32 calc_pack_length(uint32 length) const { return length; }
  const Type_handler *type_handler_for_tmp_table(const Item *item) const
  {
    return varstring_type_handler(item);
  }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};



class Type_handler_var_string: public Type_handler_string
{
  static const Name m_name_var_string;
public:
  virtual ~Type_handler_var_string() {}
  const Name name() const { return m_name_var_string; }
  enum_field_types field_type() const { return MYSQL_TYPE_VAR_STRING; }
  enum_field_types real_field_type() const { return MYSQL_TYPE_STRING; }
  const Type_handler *type_handler_for_tmp_table(const Item *item) const
  {
    return varstring_type_handler(item);
  }
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const
  { return Column_definition_prepare_stage2_legacy_num(c, MYSQL_TYPE_STRING); }
  const Type_handler *type_handler_for_union(const Item *item) const
  {
    return varstring_type_handler(item);
  }
};


class Type_handler_varchar: public Type_handler_longstr
{
  static const Name m_name_varchar;
public:
  virtual ~Type_handler_varchar() {}
  const Name name() const { return m_name_varchar; }
  enum_field_types field_type() const { return MYSQL_TYPE_VARCHAR; }
  uint32 calc_pack_length(uint32 length) const
  {
    return (length + (length < 256 ? 1: 2));
  }
  const Type_handler *type_handler_for_tmp_table(const Item *item) const
  {
    return varstring_type_handler(item);
  }
  const Type_handler *type_handler_for_union(const Item *item) const
  {
    return varstring_type_handler(item);
  }
  bool is_param_long_data_type() const { return true; }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
  bool adjust_spparam_type(Spvar_definition *def, Item *from) const;
};


class Type_handler_varchar_compressed: public Type_handler_varchar
{
public:
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
};


class Type_handler_blob_common: public Type_handler_longstr
{
public:
  virtual ~Type_handler_blob_common() { }
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  const Type_handler *type_handler_for_tmp_table(const Item *item) const
  {
    return blob_type_handler(item);
  }
  const Type_handler *type_handler_for_union(const Item *item) const
  {
    return blob_type_handler(item);
  }
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const
  {
    return false; 
  }
  bool is_param_long_data_type() const { return true; }
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  void Item_param_setup_conversion(THD *thd, Item_param *) const;

};


class Type_handler_tiny_blob: public Type_handler_blob_common
{
  static const Name m_name_tinyblob;
public:
  virtual ~Type_handler_tiny_blob() {}
  const Name name() const { return m_name_tinyblob; }
  enum_field_types field_type() const { return MYSQL_TYPE_TINY_BLOB; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_medium_blob: public Type_handler_blob_common
{
  static const Name m_name_mediumblob;
public:
  virtual ~Type_handler_medium_blob() {}
  const Name name() const { return m_name_mediumblob; }
  enum_field_types field_type() const { return MYSQL_TYPE_MEDIUM_BLOB; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_long_blob: public Type_handler_blob_common
{
  static const Name m_name_longblob;
public:
  virtual ~Type_handler_long_blob() {}
  const Name name() const { return m_name_longblob; }
  enum_field_types field_type() const { return MYSQL_TYPE_LONG_BLOB; }
  uint32 calc_pack_length(uint32 length) const;
  Item *create_typecast_item(THD *thd, Item *item,
                             const Type_cast_attributes &attr) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_blob: public Type_handler_blob_common
{
  static const Name m_name_blob;
public:
  virtual ~Type_handler_blob() {}
  const Name name() const { return m_name_blob; }
  enum_field_types field_type() const { return MYSQL_TYPE_BLOB; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_blob_compressed: public Type_handler_blob
{
public:
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
};


#ifdef HAVE_SPATIAL
class Type_handler_geometry: public Type_handler_string_result
{
  static const Name m_name_geometry;
public:
  virtual ~Type_handler_geometry() {}
  const Name name() const { return m_name_geometry; }
  enum_field_types field_type() const { return MYSQL_TYPE_GEOMETRY; }
  bool is_param_long_data_type() const { return true; }
  uint32 calc_pack_length(uint32 length) const;
  const Type_handler *type_handler_for_comparison() const;
  bool type_can_have_key_part() const
  {
    return true;
  }
  bool subquery_type_allows_materialization(const Item *inner,
                                            const Item *outer) const
  {
    return false; 
  }
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
  bool Item_param_set_from_value(THD *thd,
                                 Item_param *param,
                                 const Type_all_attributes *attr,
                                 const st_value *value) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;

  bool can_return_int() const { return false; }
  bool can_return_decimal() const { return false; }
  bool can_return_real() const { return false; }
  bool can_return_text() const { return false; }
  bool can_return_date() const { return false; }
  bool can_return_time() const { return false; }
  bool is_traditional_type() const
  {
    return false;
  }
  bool Item_func_round_fix_length_and_dec(Item_func_round *) const;
  bool Item_func_int_val_fix_length_and_dec(Item_func_int_val *) const;
  bool Item_func_abs_fix_length_and_dec(Item_func_abs *) const;
  bool Item_func_neg_fix_length_and_dec(Item_func_neg *) const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *h,
                                       Type_all_attributes *attr,
                                       Item **items, uint nitems) const;
  bool Item_sum_sum_fix_length_and_dec(Item_sum_sum *) const;
  bool Item_sum_avg_fix_length_and_dec(Item_sum_avg *) const;
  bool Item_sum_variance_fix_length_and_dec(Item_sum_variance *) const;

  bool Item_func_signed_fix_length_and_dec(Item_func_signed *) const;
  bool Item_func_unsigned_fix_length_and_dec(Item_func_unsigned *) const;
  bool Item_double_typecast_fix_length_and_dec(Item_double_typecast *) const;
  bool Item_decimal_typecast_fix_length_and_dec(Item_decimal_typecast *) const;
  bool Item_char_typecast_fix_length_and_dec(Item_char_typecast *) const;
  bool Item_time_typecast_fix_length_and_dec(Item_time_typecast *) const;
  bool Item_date_typecast_fix_length_and_dec(Item_date_typecast *) const;
  bool Item_datetime_typecast_fix_length_and_dec(Item_datetime_typecast *) const;
};

extern MYSQL_PLUGIN_IMPORT Type_handler_geometry type_handler_geometry;
#endif


class Type_handler_typelib: public Type_handler_general_purpose_string
{
public:
  virtual ~Type_handler_typelib() { }
  enum_field_types field_type() const { return MYSQL_TYPE_STRING; }
  const Type_handler *type_handler_for_item_field() const;
  const Type_handler *cast_to_int_type_handler() const;
  bool Item_hybrid_func_fix_attributes(THD *thd,
                                       const char *name,
                                       Type_handler_hybrid_field_type *,
                                       Type_all_attributes *atrr,
                                       Item **items, uint nitems) const;
  bool Column_definition_prepare_stage1(THD *thd,
                                        MEM_ROOT *mem_root,
                                        Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  bool Column_definition_redefine_stage1(Column_definition *def,
                                         const Column_definition *dup,
                                         const handler *file,
                                         const Schema_specification_st *schema)
                                         const;
  void Item_param_set_param_func(Item_param *param,
                                 uchar **pos, ulong len) const;
  bool Vers_history_point_resolve_unit(THD *thd, Vers_history_point *p) const;
};


class Type_handler_enum: public Type_handler_typelib
{
  static const Name m_name_enum;
public:
  virtual ~Type_handler_enum() {}
  const Name name() const { return m_name_enum; }
  virtual enum_field_types real_field_type() const { return MYSQL_TYPE_ENUM; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};


class Type_handler_set: public Type_handler_typelib
{
  static const Name m_name_set;
public:
  virtual ~Type_handler_set() {}
  const Name name() const { return m_name_set; }
  virtual enum_field_types real_field_type() const { return MYSQL_TYPE_SET; }
  uint32 calc_pack_length(uint32 length) const;
  Field *make_conversion_table_field(TABLE *, uint metadata,
                                     const Field *target) const;
  bool Column_definition_fix_attributes(Column_definition *c) const;
  bool Column_definition_prepare_stage2(Column_definition *c,
                                        handler *file,
                                        ulonglong table_flags) const;
  Field *make_table_field(const LEX_CSTRING *name,
                          const Record_addr &addr,
                          const Type_all_attributes &attr,
                          TABLE *table) const;
};



class Type_handler_hybrid_field_type
{
  const Type_handler *m_type_handler;
  bool aggregate_for_min_max(const Type_handler *other);

public:
  Type_handler_hybrid_field_type();
  Type_handler_hybrid_field_type(const Type_handler *handler)
   :m_type_handler(handler)
  { }
  Type_handler_hybrid_field_type(const Type_handler_hybrid_field_type *other)
    :m_type_handler(other->m_type_handler)
  { }
  void swap(Type_handler_hybrid_field_type &other)
  {
    swap_variables(const Type_handler *, m_type_handler, other.m_type_handler);
  }
  const Type_handler *type_handler() const { return m_type_handler; }
  enum_field_types real_field_type() const
  {
    return m_type_handler->real_field_type();
  }
  Item_result cmp_type() const { return m_type_handler->cmp_type(); }
  enum_mysql_timestamp_type mysql_timestamp_type() const
  {
    return m_type_handler->mysql_timestamp_type();
  }
  bool is_timestamp_type() const
  {
    return m_type_handler->is_timestamp_type();
  }
  void set_handler(const Type_handler *other)
  {
    m_type_handler= other;
  }
  const Type_handler *set_handler_by_result_type(Item_result type)
  {
    return (m_type_handler= Type_handler::get_handler_by_result_type(type));
  }
  const Type_handler *set_handler_by_cmp_type(Item_result type)
  {
    return (m_type_handler= Type_handler::get_handler_by_cmp_type(type));
  }
  const Type_handler *set_handler_by_result_type(Item_result type,
                                                 uint max_octet_length,
                                                 CHARSET_INFO *cs)
  {
    m_type_handler= Type_handler::get_handler_by_result_type(type);
    return m_type_handler=
      m_type_handler->type_handler_adjusted_to_max_octet_length(max_octet_length,
                                                                cs);
  }
  const Type_handler *set_handler_by_field_type(enum_field_types type)
  {
    return (m_type_handler= Type_handler::get_handler_by_field_type(type));
  }
  const Type_handler *set_handler_by_real_type(enum_field_types type)
  {
    return (m_type_handler= Type_handler::get_handler_by_real_type(type));
  }
  bool aggregate_for_comparison(const Type_handler *other);
  bool aggregate_for_comparison(const char *funcname,
                                Item **items, uint nitems,
                                bool treat_int_to_uint_as_decimal);
  bool aggregate_for_result(const Type_handler *other);
  bool aggregate_for_result(const char *funcname,
                            Item **item, uint nitems, bool treat_bit_as_number);
  bool aggregate_for_min_max(const char *funcname, Item **item, uint nitems);

  bool aggregate_for_num_op(const class Type_aggregator *aggregator,
                            const Type_handler *h0, const Type_handler *h1);
};


extern MYSQL_PLUGIN_IMPORT Type_handler_row         type_handler_row;
extern MYSQL_PLUGIN_IMPORT Type_handler_null        type_handler_null;

extern MYSQL_PLUGIN_IMPORT Type_handler_float       type_handler_float;
extern MYSQL_PLUGIN_IMPORT Type_handler_double      type_handler_double;

extern MYSQL_PLUGIN_IMPORT Type_handler_bit         type_handler_bit;

extern MYSQL_PLUGIN_IMPORT Type_handler_enum        type_handler_enum;
extern MYSQL_PLUGIN_IMPORT Type_handler_set         type_handler_set;

extern MYSQL_PLUGIN_IMPORT Type_handler_string      type_handler_string;
extern MYSQL_PLUGIN_IMPORT Type_handler_var_string  type_handler_var_string;
extern MYSQL_PLUGIN_IMPORT Type_handler_varchar     type_handler_varchar;

extern MYSQL_PLUGIN_IMPORT Type_handler_tiny_blob   type_handler_tiny_blob;
extern MYSQL_PLUGIN_IMPORT Type_handler_medium_blob type_handler_medium_blob;
extern MYSQL_PLUGIN_IMPORT Type_handler_long_blob   type_handler_long_blob;
extern MYSQL_PLUGIN_IMPORT Type_handler_blob        type_handler_blob;

extern MYSQL_PLUGIN_IMPORT Type_handler_tiny        type_handler_tiny;
extern MYSQL_PLUGIN_IMPORT Type_handler_short       type_handler_short;
extern MYSQL_PLUGIN_IMPORT Type_handler_int24       type_handler_int24;
extern MYSQL_PLUGIN_IMPORT Type_handler_long        type_handler_long;
extern MYSQL_PLUGIN_IMPORT Type_handler_longlong    type_handler_longlong;
extern MYSQL_PLUGIN_IMPORT Type_handler_longlong    type_handler_ulonglong;
extern MYSQL_PLUGIN_IMPORT Type_handler_vers_trx_id type_handler_vers_trx_id;

extern MYSQL_PLUGIN_IMPORT Type_handler_newdecimal  type_handler_newdecimal;
extern MYSQL_PLUGIN_IMPORT Type_handler_olddecimal  type_handler_olddecimal;

extern MYSQL_PLUGIN_IMPORT Type_handler_year        type_handler_year;
extern MYSQL_PLUGIN_IMPORT Type_handler_newdate     type_handler_newdate;
extern MYSQL_PLUGIN_IMPORT Type_handler_date        type_handler_date;
extern MYSQL_PLUGIN_IMPORT Type_handler_time        type_handler_time;
extern MYSQL_PLUGIN_IMPORT Type_handler_time2       type_handler_time2;
extern MYSQL_PLUGIN_IMPORT Type_handler_datetime    type_handler_datetime;
extern MYSQL_PLUGIN_IMPORT Type_handler_datetime2   type_handler_datetime2;
extern MYSQL_PLUGIN_IMPORT Type_handler_timestamp   type_handler_timestamp;
extern MYSQL_PLUGIN_IMPORT Type_handler_timestamp2  type_handler_timestamp2;

extern MYSQL_PLUGIN_IMPORT Type_handler_tiny_blob   type_handler_tiny_blob;
extern MYSQL_PLUGIN_IMPORT Type_handler_blob        type_handler_blob;
extern MYSQL_PLUGIN_IMPORT Type_handler_medium_blob type_handler_medium_blob;
extern MYSQL_PLUGIN_IMPORT Type_handler_long_blob   type_handler_long_blob;

class Type_aggregator
{
  bool m_is_commutative;
  class Pair
  {
  public:
    const Type_handler *m_handler1;
    const Type_handler *m_handler2;
    const Type_handler *m_result;
    Pair() { }
    Pair(const Type_handler *handler1,
         const Type_handler *handler2,
         const Type_handler *result)
     :m_handler1(handler1), m_handler2(handler2), m_result(result)
    { }
    bool eq(const Type_handler *handler1, const Type_handler *handler2) const
    {
      return m_handler1 == handler1 && m_handler2 == handler2;
    }
  };
  Dynamic_array<Pair> m_array;
  const Pair* find_pair(const Type_handler *handler1,
                        const Type_handler *handler2) const;
public:
  Type_aggregator(bool is_commutative= false)
   :m_is_commutative(is_commutative)
  { }
  bool add(const Type_handler *handler1,
           const Type_handler *handler2,
           const Type_handler *result)
  {
    return m_array.append(Pair(handler1, handler2, result));
  }
  const Type_handler *find_handler(const Type_handler *handler1,
                                   const Type_handler *handler2) const
  {
    const Pair* el= find_pair(handler1, handler2);
    return el ? el->m_result : NULL;
  }
  bool is_commutative() const { return m_is_commutative; }
};


class Type_aggregator_commutative: public Type_aggregator
{
public:
  Type_aggregator_commutative()
   :Type_aggregator(true)
  { }
};


class Type_handler_data
{
public:
  Type_aggregator_commutative m_type_aggregator_for_result;
  Type_aggregator_commutative m_type_aggregator_for_comparison;

  Type_aggregator_commutative m_type_aggregator_for_plus;
  Type_aggregator_commutative m_type_aggregator_for_mul;

  Type_aggregator m_type_aggregator_for_minus;
  Type_aggregator m_type_aggregator_for_div;
  Type_aggregator m_type_aggregator_for_mod;
#ifndef DBUG_OFF
  
  Type_aggregator m_type_aggregator_non_commutative_test;
#endif
  bool init();
};


extern Type_handler_data *type_handler_data;

#endif 
