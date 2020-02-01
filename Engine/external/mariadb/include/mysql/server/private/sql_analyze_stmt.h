




class Exec_time_tracker
{
protected:
  ulonglong count;
  ulonglong cycles;
  ulonglong last_start;

  void cycles_stop_tracking()
  {
    ulonglong end= my_timer_cycles();
    cycles += end - last_start;
    if (unlikely(end < last_start))
      cycles += ULONGLONG_MAX;
  }
public:
  Exec_time_tracker() : count(0), cycles(0) {}
  
  
  void start_tracking()
  {
    last_start= my_timer_cycles();
  }

  void stop_tracking()
  {
    count++;
    cycles_stop_tracking();
  }

  
  ulonglong get_loops() const { return count; }
  double get_time_ms() const
  {
    
    return 1000 * ((double)cycles) / sys_timer_info.cycles.frequency;
  }
};




class Time_and_counter_tracker: public Exec_time_tracker
{
public: 
  const bool timed;
  
  Time_and_counter_tracker(bool timed_arg) : timed(timed_arg)
  {}
   
  
  void incr_loops() { count++; }
  
  
  void stop_tracking()
  {
    cycles_stop_tracking();
  }
};

#define ANALYZE_START_TRACKING(tracker) \
  { \
    (tracker)->incr_loops(); \
    if (unlikely((tracker)->timed)) \
    { (tracker)->start_tracking(); } \
  }

#define ANALYZE_STOP_TRACKING(tracker) \
  if (unlikely((tracker)->timed)) \
  { (tracker)->stop_tracking(); }



class Table_access_tracker 
{
public:
  Table_access_tracker() :
    r_scans(0), r_rows(0), 
    r_rows_after_where(0)
  {}

  ha_rows r_scans; 
  ha_rows r_rows; 
  ha_rows r_rows_after_where; 

  bool has_scans() { return (r_scans != 0); }
  ha_rows get_loops() { return r_scans; }
  double get_avg_rows()
  {
    return r_scans ? ((double)r_rows / r_scans): 0;
  }

  double get_filtered_after_where()
  {
    double r_filtered;
    if (r_rows > 0)
      r_filtered= (double)r_rows_after_where / r_rows;
    else
      r_filtered= 1.0;

    return r_filtered;
  }
  
  inline void on_scan_init() { r_scans++; }
  inline void on_record_read() { r_rows++; }
  inline void on_record_after_where() { r_rows_after_where++; }
};


class Json_writer;



class Filesort_tracker : public Sql_alloc
{
public:
  Filesort_tracker(bool do_timing) :
    time_tracker(do_timing), r_limit(0), r_used_pq(0),
    r_examined_rows(0), r_sorted_rows(0), r_output_rows(0),
    sort_passes(0),
    sort_buffer_size(0)
  {}
  
  

  inline void report_use(ha_rows r_limit_arg)
  {
    if (!time_tracker.get_loops())
      r_limit= r_limit_arg;
    else
      r_limit= (r_limit != r_limit_arg)? 0: r_limit_arg;

    ANALYZE_START_TRACKING(&time_tracker);
  }
  inline void incr_pq_used() { r_used_pq++; }

  inline void report_row_numbers(ha_rows examined_rows, 
                                 ha_rows sorted_rows,
                                 ha_rows returned_rows) 
  { 
    r_examined_rows += examined_rows;
    r_sorted_rows   += sorted_rows;
    r_output_rows   += returned_rows;
  }

  inline void report_merge_passes_at_start(ulong passes)
  {
    sort_passes -= passes;
  }
  inline void report_merge_passes_at_end(ulong passes)
  {
    ANALYZE_STOP_TRACKING(&time_tracker);
    sort_passes += passes;
  }

  inline void report_sort_buffer_size(size_t bufsize)
  {
    if (sort_buffer_size)
      sort_buffer_size= ulonglong(-1); 
    else
      sort_buffer_size= bufsize;
  }
  
  
  void print_json_members(Json_writer *writer);
  
  ulonglong get_r_loops() const { return time_tracker.get_loops(); }
  double get_avg_examined_rows() 
  { 
    return ((double)r_examined_rows) / get_r_loops();
  }
  double get_avg_returned_rows()
  { 
    return ((double)r_output_rows) / get_r_loops(); 
  }
  double get_r_filtered()
  {
    if (r_examined_rows > 0)
      return ((double)r_sorted_rows / r_examined_rows);
    else
      return 1.0;
  }
private:
  Time_and_counter_tracker time_tracker;

  
  
  ulonglong r_limit;
  ulonglong r_used_pq; 

  
  ulonglong r_examined_rows;
  
  
  ulonglong r_sorted_rows;

  
  ulonglong r_output_rows;

  
  ulonglong sort_passes;
  
  
  ulonglong sort_buffer_size;
};

