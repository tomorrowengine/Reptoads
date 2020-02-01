

class Json_writer;



class Single_line_formatting_helper
{
  enum enum_state
  {
    INACTIVE,
    ADD_MEMBER,
    IN_ARRAY,
    DISABLED
  };

  
  enum enum_state state;
  enum { MAX_LINE_LEN= 80 };
  char buffer[80];

  
  char *buf_ptr;
  uint line_len;

  Json_writer *owner;
public:
  Single_line_formatting_helper() : state(INACTIVE), buf_ptr(buffer) {}

  void init(Json_writer *owner_arg) { owner= owner_arg; }

  bool on_add_member(const char *name);

  bool on_start_array();
  bool on_end_array();
  void on_start_object();
  
   
  bool on_add_str(const char *str);

  void flush_on_one_line();
  void disable_and_flush();
};




class Json_writer
{
public:
  
  Json_writer& add_member(const char *name);
  
  
  void add_str(const char* val);
  void add_str(const String &str);

  void add_ll(longlong val);
  void add_size(longlong val);
  void add_double(double val);
  void add_bool(bool val);
  void add_null();

private:
  void add_unquoted_str(const char* val);
public:
  
  void start_object();
  void start_array();

  void end_object();
  void end_array();
  
  Json_writer() : 
    indent_level(0), document_start(true), element_started(false), 
    first_child(true)
  {
    fmt_helper.init(this);
  }
private:
  
  int indent_level;
  enum { INDENT_SIZE = 2 };

  friend class Single_line_formatting_helper;
  friend class Json_writer_nesting_guard;
  bool document_start;
  bool element_started;
  bool first_child;

  Single_line_formatting_helper fmt_helper;

  void append_indent();
  void start_element();
  void start_sub_element();

  
public:
  String output;
};




class Json_writer_nesting_guard
{
#ifdef DBUG_OFF
public:
  Json_writer_nesting_guard(Json_writer *) {}
#else
  Json_writer* writer;
  int indent_level;
public:
  Json_writer_nesting_guard(Json_writer *writer_arg) : 
    writer(writer_arg),
    indent_level(writer->indent_level)
  {}

  ~Json_writer_nesting_guard()
  {
    DBUG_ASSERT(indent_level == writer->indent_level);
  }
#endif
};


