

#ifndef SQL_STATISTICS_H
#define SQL_STATISTICS_H

typedef
enum enum_use_stat_tables_mode
{
  NEVER,
  COMPLEMENTARY,
  PREFERABLY,
} Use_stat_tables_mode;

typedef
enum enum_histogram_type
{
  SINGLE_PREC_HB,
  DOUBLE_PREC_HB
} Histogram_type;

enum enum_stat_tables
{
  TABLE_STAT,
  COLUMN_STAT,
  INDEX_STAT,
};




enum enum_table_stat_col
{
  TABLE_STAT_DB_NAME,
  TABLE_STAT_TABLE_NAME,
  TABLE_STAT_CARDINALITY,
  TABLE_STAT_N_FIELDS
};

enum enum_column_stat_col
{
  COLUMN_STAT_DB_NAME,
  COLUMN_STAT_TABLE_NAME,
  COLUMN_STAT_COLUMN_NAME,
  COLUMN_STAT_MIN_VALUE,
  COLUMN_STAT_MAX_VALUE,
  COLUMN_STAT_NULLS_RATIO,
  COLUMN_STAT_AVG_LENGTH,
  COLUMN_STAT_AVG_FREQUENCY,
  COLUMN_STAT_HIST_SIZE,
  COLUMN_STAT_HIST_TYPE,
  COLUMN_STAT_HISTOGRAM,
  COLUMN_STAT_N_FIELDS
};

enum enum_index_stat_col
{
  INDEX_STAT_DB_NAME,
  INDEX_STAT_TABLE_NAME,
  INDEX_STAT_INDEX_NAME,
  INDEX_STAT_PREFIX_ARITY,
  INDEX_STAT_AVG_FREQUENCY,
  INDEX_STAT_N_FIELDS
};

inline
Use_stat_tables_mode get_use_stat_tables_mode(THD *thd)
{ 
  return (Use_stat_tables_mode) (thd->variables.use_stat_tables);
}

int read_statistics_for_tables_if_needed(THD *thd, TABLE_LIST *tables);
int collect_statistics_for_table(THD *thd, TABLE *table);
int alloc_statistics_for_table_share(THD* thd, TABLE_SHARE *share,
                                     bool is_safe);
void delete_stat_values_for_table_share(TABLE_SHARE *table_share);
int alloc_statistics_for_table(THD *thd, TABLE *table);
int update_statistics_for_table(THD *thd, TABLE *table);
int delete_statistics_for_table(THD *thd, const LEX_CSTRING *db, const LEX_CSTRING *tab);
int delete_statistics_for_column(THD *thd, TABLE *tab, Field *col);
int delete_statistics_for_index(THD *thd, TABLE *tab, KEY *key_info,
                                bool ext_prefixes_only);
int rename_table_in_stat_tables(THD *thd, const LEX_CSTRING *db, const LEX_CSTRING *tab,
                                const LEX_CSTRING *new_db, const LEX_CSTRING *new_tab);
int rename_column_in_stat_tables(THD *thd, TABLE *tab, Field *col,
                                  const char *new_name);
void set_statistics_for_table(THD *thd, TABLE *table);

double get_column_avg_frequency(Field * field);

double get_column_range_cardinality(Field *field,
                                    key_range *min_endp,
                                    key_range *max_endp,
                                    uint range_flag);
bool is_stat_table(const LEX_CSTRING *db, LEX_CSTRING *table);
bool is_eits_usable(Field* field);

class Histogram
{

private:
  Histogram_type type;
  uint8 size; 
  uchar *values;

  uint prec_factor()
  {
    switch (type) {
    case SINGLE_PREC_HB:
      return ((uint) (1 << 8) - 1);
    case DOUBLE_PREC_HB:
      return ((uint) (1 << 16) - 1);
    }
    return 1;
  }

public:
  uint get_width()
  {
    switch (type) {
    case SINGLE_PREC_HB:
      return size;
    case DOUBLE_PREC_HB:
      return size / 2;
    }
    return 0;
  }

private:
  uint get_value(uint i)
  {
    DBUG_ASSERT(i < get_width());
    switch (type) {
    case SINGLE_PREC_HB:
      return (uint) (((uint8 *) values)[i]);
    case DOUBLE_PREC_HB:
      return (uint) uint2korr(values + i * 2);
    }
    return 0;
  }

  
  uint find_bucket(double pos, bool first)
  {
    uint val= (uint) (pos * prec_factor());
    int lp= 0;
    int rp= get_width() - 1;
    int d= get_width() / 2;
    uint i= lp + d;
    for ( ; d;  d= (rp - lp) / 2, i= lp + d)
    {
      if (val == get_value(i))
	break; 
      if (val < get_value(i))
        rp= i;
      else if (val > get_value(i + 1))
        lp= i + 1;
      else
        break;
    }

    if (val > get_value(i) && i < (get_width() - 1))
      i++;

    if (val == get_value(i))
    {
      if (first)
      {
        while(i && val == get_value(i - 1))
          i--;
      }
      else
      {
        while(i + 1 < get_width() && val == get_value(i + 1))
          i++;
      }
    }
    return i;
  }

public:

  uint get_size() { return (uint) size; }

  Histogram_type get_type() { return type; }

  uchar *get_values() { return (uchar *) values; }

  void set_size (ulonglong sz) { size= (uint8) sz; }

  void set_type (Histogram_type t) { type= t; }

  void set_values (uchar *vals) { values= (uchar *) vals; }

  bool is_available() { return get_size() > 0 && get_values(); }

  void set_value(uint i, double val)
  {
    switch (type) {
    case SINGLE_PREC_HB:   
      ((uint8 *) values)[i]= (uint8) (val * prec_factor());
      return;
    case DOUBLE_PREC_HB:
      int2store(values + i * 2, val * prec_factor());
      return;
    }
  }

  void set_prev_value(uint i)
  {
    switch (type) {
    case SINGLE_PREC_HB:   
      ((uint8 *) values)[i]= ((uint8 *) values)[i-1];
      return;
    case DOUBLE_PREC_HB:
      int2store(values + i * 2, uint2korr(values + i * 2 - 2));
      return;
    }
  }

  double range_selectivity(double min_pos, double max_pos)
  {
    double sel;
    double bucket_sel= 1.0/(get_width() + 1);  
    uint min= find_bucket(min_pos, TRUE);
    uint max= find_bucket(max_pos, FALSE);
    sel= bucket_sel * (max - min + 1);
    return sel;
  } 
  
  
  double point_selectivity(double pos, double avg_sel);
};


class Columns_statistics;
class Index_statistics;



class Table_statistics
{

public:
  my_bool cardinality_is_null;      
  ha_rows cardinality;              
  uchar *min_max_record_buffers;    
  Column_statistics *column_stats;  
  Index_statistics *index_stats;    
  ulong *idx_avg_frequency;   
  ulong total_hist_size;            
  uchar *histograms;                                    
};




class Column_statistics
{

private:
  static const uint Scale_factor_nulls_ratio= 100000;
  static const uint Scale_factor_avg_length= 100000;
  static const uint Scale_factor_avg_frequency= 100000;

public:
  
  uint32 column_stat_nulls;
  
  
  
  Field *min_value; 
     
  Field *max_value;

private:

  
  ulong nulls_ratio;
 
  
  ulong avg_length;

  
  ulong avg_frequency;

public:

  Histogram histogram;

  uint32 no_values_provided_bitmap()
  {
    return
     ((1 << (COLUMN_STAT_HISTOGRAM-COLUMN_STAT_COLUMN_NAME))-1) <<
      (COLUMN_STAT_COLUMN_NAME+1);
  }
 
  void set_all_nulls()
  {
    column_stat_nulls= no_values_provided_bitmap();
  }

  void set_not_null(uint stat_field_no)
  {
    column_stat_nulls&= ~(1 << stat_field_no);
  }

  bool is_null(uint stat_field_no)
  {
    return MY_TEST(column_stat_nulls & (1 << stat_field_no));
  }

  double get_nulls_ratio()
  {
    return (double) nulls_ratio /  Scale_factor_nulls_ratio;
  }

  double get_avg_length()
  {
    return (double) avg_length / Scale_factor_avg_length;
  }

  double get_avg_frequency()
  {
    return (double) avg_frequency / Scale_factor_avg_frequency;
  }

  void set_nulls_ratio (double val)
  {
    nulls_ratio= (ulong) (val * Scale_factor_nulls_ratio);
  }

  void set_avg_length (double val)
  {
    avg_length= (ulong) (val * Scale_factor_avg_length);
  }

  void set_avg_frequency (double val)
  {
    avg_frequency= (ulong) (val * Scale_factor_avg_frequency);
  }

  bool min_max_values_are_provided()
  {
    return !is_null(COLUMN_STAT_MIN_VALUE) && 
      !is_null(COLUMN_STAT_MAX_VALUE);
  }
  
  bool no_stat_values_provided()
  {
    if (column_stat_nulls == no_values_provided_bitmap())
      return true;
    return false;
  }
};




class Index_statistics
{

private:
  static const uint Scale_factor_avg_frequency= 100000;
  
  ulong *avg_frequency;

public:

  void init_avg_frequency(ulong *ptr) { avg_frequency= ptr; }

  bool avg_frequency_is_inited() { return avg_frequency != NULL; }

  double get_avg_frequency(uint i)
  {
    return (double) avg_frequency[i] / Scale_factor_avg_frequency;
  }

  void set_avg_frequency(uint i, double val)
  {
    avg_frequency[i]= (ulong) (val * Scale_factor_avg_frequency);
  }

};

#endif 
