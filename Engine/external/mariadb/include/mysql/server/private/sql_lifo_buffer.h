



class Forward_lifo_buffer;
class Backward_lifo_buffer;




class Lifo_buffer 
{
protected:
  size_t size1;
  size_t size2;

public:
  
  uchar *write_ptr1;
  uchar *write_ptr2;

  
  uchar *read_ptr1;
  uchar *read_ptr2;

protected:
  uchar *start; 
  uchar *end;   
public:

  enum enum_direction {
    BACKWARD=-1, 
    FORWARD=1  
  };

  virtual enum_direction type() = 0;

  

  
  void set_buffer_space(uchar *start_arg, uchar *end_arg) 
  {
    start= start_arg;
    end= end_arg;
    if (end != start)
      TRASH_ALLOC(start, size_t(end - start));
    reset();
  }
  
  
  void setup_writing(size_t len1, size_t len2)
  {
    size1= len1;
    size2= len2;
  }

  
  void setup_reading(size_t len1, size_t len2)
  {
    DBUG_ASSERT(len1 == size1);
    DBUG_ASSERT(len2 == size2);
  }
  
  bool can_write()
  {
    return have_space_for(size1 + size2);
  }
  virtual void write() = 0;

  bool is_empty() { return used_size() == 0; }
  virtual bool read() = 0;
  
  void sort(qsort2_cmp cmp_func, void *cmp_func_arg)
  {
    size_t elem_size= size1 + size2;
    size_t n_elements= used_size() / elem_size;
    my_qsort2(used_area(), n_elements, elem_size, cmp_func, cmp_func_arg);
  }

  virtual void reset() = 0;
  virtual uchar *end_of_space() = 0;
protected:
  virtual size_t used_size() = 0;
  
  
  virtual uchar *get_pos()= 0;
  virtual bool read(uchar **position, uchar **ptr1, uchar **ptr2)= 0;
  friend class Lifo_buffer_iterator;
public:
  virtual bool have_space_for(size_t bytes) = 0;

  virtual void remove_unused_space(uchar **unused_start, uchar **unused_end)=0;
  virtual uchar *used_area() = 0; 
  virtual ~Lifo_buffer() {};
};




class Forward_lifo_buffer: public Lifo_buffer
{
  uchar *pos;
public:
  enum_direction type() { return FORWARD; }
  size_t used_size()
  {
    return (size_t)(pos - start);
  }
  void reset()
  {
    pos= start;
  }
  uchar *end_of_space() { return pos; }
  bool have_space_for(size_t bytes)
  {
    return (pos + bytes < end);
  }

  void write()
  {
    write_bytes(write_ptr1, size1);
    if (size2)
      write_bytes(write_ptr2, size2);
  }
  void write_bytes(const uchar *data, size_t bytes)
  {
    DBUG_ASSERT(have_space_for(bytes));
    memcpy(pos, data, bytes);
    pos += bytes;
  }
  bool have_data(uchar *position, size_t bytes)
  {
    return ((position - start) >= (ptrdiff_t)bytes);
  }
  uchar *read_bytes(uchar **position, size_t bytes)
  {
    DBUG_ASSERT(have_data(*position, bytes));
    *position= (*position) - bytes;
    return *position;
  }
  bool read() { return read(&pos, &read_ptr1, &read_ptr2); }
  bool read(uchar **position, uchar **ptr1, uchar **ptr2)
  {
    if (!have_data(*position, size1 + size2))
      return TRUE;
    if (size2)
      *ptr2= read_bytes(position, size2);
    *ptr1= read_bytes(position, size1);
    return FALSE;
  }
  void remove_unused_space(uchar **unused_start, uchar **unused_end)
  {
    DBUG_ASSERT(0); 
  }
  
  void grow(uchar *unused_start, uchar *unused_end)
  {
    DBUG_ASSERT(unused_end >= unused_start);
    DBUG_ASSERT(end == unused_start);
    TRASH_ALLOC(unused_start, size_t(unused_end - unused_start));
    end= unused_end;
  }
  
  uchar *used_area() { return start; }
  friend class Lifo_buffer_iterator;
  uchar *get_pos() { return pos; }
};




class Backward_lifo_buffer: public Lifo_buffer
{
  uchar *pos;
public:
  enum_direction type() { return BACKWARD; }
 
  size_t used_size()
  {
    return (size_t)(end - pos);
  }
  void reset()
  {
    pos= end;
  }
  uchar *end_of_space() { return end; }
  bool have_space_for(size_t bytes)
  {
    return (pos - bytes >= start);
  }
  void write()
  {
    if (write_ptr2)
      write_bytes(write_ptr2, size2);
    write_bytes(write_ptr1, size1);
  }
  void write_bytes(const uchar *data, size_t bytes)
  {
    DBUG_ASSERT(have_space_for(bytes));
    pos -= bytes;
    memcpy(pos, data, bytes);
  }
  bool read()
  {
    return read(&pos, &read_ptr1, &read_ptr2);
  }
  bool read(uchar **position, uchar **ptr1, uchar **ptr2)
  {
    if (!have_data(*position, size1 + size2))
      return TRUE;
    *ptr1= read_bytes(position, size1);
    if (size2)
      *ptr2= read_bytes(position, size2);
    return FALSE;
  }
  bool have_data(uchar *position, size_t bytes)
  {
    return ((end - position) >= (ptrdiff_t)bytes);
  }
  uchar *read_bytes(uchar **position, size_t bytes)
  {
    DBUG_ASSERT(have_data(*position, bytes));
    uchar *ret= *position;
    *position= *position + bytes;
    return ret;
  }
  
  void remove_unused_space(uchar **unused_start, uchar **unused_end)
  {
    *unused_start= start;
    *unused_end= pos;
    start= pos;
  }
  void grow(uchar *unused_start, uchar *unused_end)
  {
    DBUG_ASSERT(0); 
  }
  
  uchar *used_area() { return pos; }
  friend class Lifo_buffer_iterator;
  uchar *get_pos() { return pos; }
};



class Lifo_buffer_iterator
{
  uchar *pos;
  Lifo_buffer *buf;
  
public:
  
  uchar *read_ptr1;
  uchar *read_ptr2;

  void init(Lifo_buffer *buf_arg)
  {
    buf= buf_arg;
    pos= buf->get_pos();
  }
  
  bool read() 
  {
    return buf->read(&pos, &read_ptr1, &read_ptr2);
  }
};


