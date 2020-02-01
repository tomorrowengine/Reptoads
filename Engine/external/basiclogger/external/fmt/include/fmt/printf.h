






#ifndef FMT_PRINTF_H_
#define FMT_PRINTF_H_

#include <algorithm>  
#include <limits>     

#include "ostream.h"

FMT_BEGIN_NAMESPACE
namespace internal {




template <typename Char>
class null_terminating_iterator {
 public:
  typedef std::ptrdiff_t difference_type;
  typedef Char value_type;
  typedef const Char* pointer;
  typedef const Char& reference;
  typedef std::random_access_iterator_tag iterator_category;

  null_terminating_iterator() : ptr_(0), end_(0) {}

  FMT_CONSTEXPR null_terminating_iterator(const Char *ptr, const Char *end)
    : ptr_(ptr), end_(end) {}

  template <typename Range>
  FMT_CONSTEXPR explicit null_terminating_iterator(const Range &r)
    : ptr_(r.begin()), end_(r.end()) {}

  FMT_CONSTEXPR null_terminating_iterator &operator=(const Char *ptr) {
    assert(ptr <= end_);
    ptr_ = ptr;
    return *this;
  }

  FMT_CONSTEXPR Char operator*() const {
    return ptr_ != end_ ? *ptr_ : Char();
  }

  FMT_CONSTEXPR null_terminating_iterator operator++() {
    ++ptr_;
    return *this;
  }

  FMT_CONSTEXPR null_terminating_iterator operator++(int) {
    null_terminating_iterator result(*this);
    ++ptr_;
    return result;
  }

  FMT_CONSTEXPR null_terminating_iterator operator--() {
    --ptr_;
    return *this;
  }

  FMT_CONSTEXPR null_terminating_iterator operator+(difference_type n) {
    return null_terminating_iterator(ptr_ + n, end_);
  }

  FMT_CONSTEXPR null_terminating_iterator operator-(difference_type n) {
    return null_terminating_iterator(ptr_ - n, end_);
  }

  FMT_CONSTEXPR null_terminating_iterator operator+=(difference_type n) {
    ptr_ += n;
    return *this;
  }

  FMT_CONSTEXPR difference_type operator-(
      null_terminating_iterator other) const {
    return ptr_ - other.ptr_;
  }

  FMT_CONSTEXPR bool operator!=(null_terminating_iterator other) const {
    return ptr_ != other.ptr_;
  }

  bool operator>=(null_terminating_iterator other) const {
    return ptr_ >= other.ptr_;
  }

  
  
  template <typename CharT>
  friend FMT_CONSTEXPR_DECL const CharT *pointer_from(
      null_terminating_iterator<CharT> it);

 private:
  const Char *ptr_;
  const Char *end_;
};

template <typename T>
FMT_CONSTEXPR const T *pointer_from(const T *p) { return p; }

template <typename Char>
FMT_CONSTEXPR const Char *pointer_from(null_terminating_iterator<Char> it) {
  return it.ptr_;
}





template <typename Iterator, typename ErrorHandler>
FMT_CONSTEXPR unsigned parse_nonnegative_int(Iterator &it, ErrorHandler &&eh) {
  assert('0' <= *it && *it <= '9');
  if (*it == '0') {
    ++it;
    return 0;
  }
  unsigned value = 0;
  
  unsigned max_int = (std::numeric_limits<int>::max)();
  unsigned big = max_int / 10;
  do {
    
    if (value > big) {
      value = max_int + 1;
      break;
    }
    value = value * 10 + unsigned(*it - '0');
    
    auto next = it;
    ++next;
    it = next;
  } while ('0' <= *it && *it <= '9');
  if (value > max_int)
    eh.on_error("number is too big");
  return value;
}



template <bool IsSigned>
struct int_checker {
  template <typename T>
  static bool fits_in_int(T value) {
    unsigned max = std::numeric_limits<int>::max();
    return value <= max;
  }
  static bool fits_in_int(bool) { return true; }
};

template <>
struct int_checker<true> {
  template <typename T>
  static bool fits_in_int(T value) {
    return value >= std::numeric_limits<int>::min() &&
           value <= std::numeric_limits<int>::max();
  }
  static bool fits_in_int(int) { return true; }
};

class printf_precision_handler: public function<int> {
 public:
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, int>::type
      operator()(T value) {
    if (!int_checker<std::numeric_limits<T>::is_signed>::fits_in_int(value))
      FMT_THROW(format_error("number is too big"));
    return static_cast<int>(value);
  }

  template <typename T>
  typename std::enable_if<!std::is_integral<T>::value, int>::type operator()(T) {
    FMT_THROW(format_error("precision is not integer"));
    return 0;
  }
};


class is_zero_int: public function<bool> {
 public:
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, bool>::type
      operator()(T value) { return value == 0; }

  template <typename T>
  typename std::enable_if<!std::is_integral<T>::value, bool>::type
      operator()(T) { return false; }
};

template <typename T>
struct make_unsigned_or_bool : std::make_unsigned<T> {};

template <>
struct make_unsigned_or_bool<bool> {
  typedef bool type;
};

template <typename T, typename Context>
class arg_converter: public function<void> {
 private:
  typedef typename Context::char_type Char;

  basic_format_arg<Context> &arg_;
  typename Context::char_type type_;

 public:
  arg_converter(basic_format_arg<Context> &arg, Char type)
    : arg_(arg), type_(type) {}

  void operator()(bool value) {
    if (type_ != 's')
      operator()<bool>(value);
  }

  template <typename U>
  typename std::enable_if<std::is_integral<U>::value>::type
      operator()(U value) {
    bool is_signed = type_ == 'd' || type_ == 'i';
    typedef typename std::conditional<
        std::is_same<T, void>::value, U, T>::type TargetType;
    if (const_check(sizeof(TargetType) <= sizeof(int))) {
      
      if (is_signed) {
        arg_ = internal::make_arg<Context>(
          static_cast<int>(static_cast<TargetType>(value)));
      } else {
        typedef typename make_unsigned_or_bool<TargetType>::type Unsigned;
        arg_ = internal::make_arg<Context>(
          static_cast<unsigned>(static_cast<Unsigned>(value)));
      }
    } else {
      if (is_signed) {
        
        
        
        arg_ = internal::make_arg<Context>(static_cast<long long>(value));
      } else {
        arg_ = internal::make_arg<Context>(
          static_cast<typename make_unsigned_or_bool<U>::type>(value));
      }
    }
  }

  template <typename U>
  typename std::enable_if<!std::is_integral<U>::value>::type operator()(U) {
    
  }
};





template <typename T, typename Context, typename Char>
void convert_arg(basic_format_arg<Context> &arg, Char type) {
  visit_format_arg(arg_converter<T, Context>(arg, type), arg);
}


template <typename Context>
class char_converter: public function<void> {
 private:
  basic_format_arg<Context> &arg_;

 public:
  explicit char_converter(basic_format_arg<Context> &arg) : arg_(arg) {}

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value>::type
      operator()(T value) {
    typedef typename Context::char_type Char;
    arg_ = internal::make_arg<Context>(static_cast<Char>(value));
  }

  template <typename T>
  typename std::enable_if<!std::is_integral<T>::value>::type operator()(T) {
    
  }
};



template <typename Char>
class printf_width_handler: public function<unsigned> {
 private:
  typedef basic_format_specs<Char> format_specs;

  format_specs &spec_;

 public:
  explicit printf_width_handler(format_specs &spec) : spec_(spec) {}

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, unsigned>::type
      operator()(T value) {
    typedef typename internal::int_traits<T>::main_type UnsignedType;
    UnsignedType width = static_cast<UnsignedType>(value);
    if (internal::is_negative(value)) {
      spec_.align_ = ALIGN_LEFT;
      width = 0 - width;
    }
    unsigned int_max = std::numeric_limits<int>::max();
    if (width > int_max)
      FMT_THROW(format_error("number is too big"));
    return static_cast<unsigned>(width);
  }

  template <typename T>
  typename std::enable_if<!std::is_integral<T>::value, unsigned>::type
      operator()(T) {
    FMT_THROW(format_error("width is not integer"));
    return 0;
  }
};

template <typename Char, typename Context>
void printf(basic_buffer<Char> &buf, basic_string_view<Char> format,
            basic_format_args<Context> args) {
  Context(std::back_inserter(buf), format, args).format();
}
}  

using internal::printf;  

template <typename Range>
class printf_arg_formatter;

template <
    typename OutputIt, typename Char,
    typename ArgFormatter =
      printf_arg_formatter<back_insert_range<internal::basic_buffer<Char>>>>
class basic_printf_context;


template <typename Range>
class printf_arg_formatter:
  public internal::function<
    typename internal::arg_formatter_base<Range>::iterator>,
  public internal::arg_formatter_base<Range> {
 private:
  typedef typename Range::value_type char_type;
  typedef decltype(internal::declval<Range>().begin()) iterator;
  typedef internal::arg_formatter_base<Range> base;
  typedef basic_printf_context<iterator, char_type> context_type;

  context_type &context_;

  void write_null_pointer(char) {
    this->spec()->type = 0;
    this->write("(nil)");
  }

  void write_null_pointer(wchar_t) {
    this->spec()->type = 0;
    this->write(L"(nil)");
  }

 public:
  typedef typename base::format_specs format_specs;

  
  printf_arg_formatter(internal::basic_buffer<char_type> &buffer,
                       format_specs &spec, context_type &ctx)
    : base(back_insert_range<internal::basic_buffer<char_type>>(buffer), &spec,
           ctx.locale()),
      context_(ctx) {}

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, iterator>::type
      operator()(T value) {
    
    
    if (std::is_same<T, bool>::value) {
      format_specs &fmt_spec = *this->spec();
      if (fmt_spec.type != 's')
        return base::operator()(value ? 1 : 0);
      fmt_spec.type = 0;
      this->write(value != 0);
    } else if (std::is_same<T, char_type>::value) {
      format_specs &fmt_spec = *this->spec();
      if (fmt_spec.type && fmt_spec.type != 'c')
        return (*this)(static_cast<int>(value));
      fmt_spec.flags = 0;
      fmt_spec.align_ = ALIGN_RIGHT;
      return base::operator()(value);
    } else {
      return base::operator()(value);
    }
    return this->out();
  }

  template <typename T>
  typename std::enable_if<std::is_floating_point<T>::value, iterator>::type
      operator()(T value) {
    return base::operator()(value);
  }

  
  iterator operator()(const char *value) {
    if (value)
      base::operator()(value);
    else if (this->spec()->type == 'p')
      write_null_pointer(char_type());
    else
      this->write("(null)");
    return this->out();
  }

  
  iterator operator()(const wchar_t *value) {
    if (value)
      base::operator()(value);
    else if (this->spec()->type == 'p')
      write_null_pointer(char_type());
    else
      this->write(L"(null)");
    return this->out();
  }

  iterator operator()(basic_string_view<char_type> value) {
    return base::operator()(value);
  }

  iterator operator()(monostate value) {
    return base::operator()(value);
  }

  
  iterator operator()(const void *value) {
    if (value)
      return base::operator()(value);
    this->spec()->type = 0;
    write_null_pointer(char_type());
    return this->out();
  }

  
  iterator operator()(typename basic_format_arg<context_type>::handle handle) {
    handle.format(context_);
    return this->out();
  }
};

template <typename T>
struct printf_formatter {
  template <typename ParseContext>
  auto parse(ParseContext &ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const T &value, FormatContext &ctx) -> decltype(ctx.out()) {
    internal::format_value(internal::get_container(ctx.out()), value);
    return ctx.out();
  }
};


template <typename OutputIt, typename Char, typename ArgFormatter>
class basic_printf_context :
  
  
  public internal::context_base<
    OutputIt, basic_printf_context<OutputIt, Char, ArgFormatter>, Char> {
 public:
  
  typedef Char char_type;

  template <typename T>
  struct formatter_type { typedef printf_formatter<T> type; };

 private:
  typedef internal::context_base<OutputIt, basic_printf_context, Char> base;
  typedef typename base::format_arg format_arg;
  typedef basic_format_specs<char_type> format_specs;
  typedef internal::null_terminating_iterator<char_type> iterator;

  void parse_flags(format_specs &spec, iterator &it);

  
  
  format_arg get_arg(
      iterator it,
      unsigned arg_index = (std::numeric_limits<unsigned>::max)());

  
  unsigned parse_header(iterator &it, format_specs &spec);

 public:
  
  basic_printf_context(OutputIt out, basic_string_view<char_type> format_str,
                       basic_format_args<basic_printf_context> args)
    : base(out, format_str, args) {}

  using base::parse_context;
  using base::out;
  using base::advance_to;

  
  void format();
};

template <typename OutputIt, typename Char, typename AF>
void basic_printf_context<OutputIt, Char, AF>::parse_flags(
    format_specs &spec, iterator &it) {
  for (;;) {
    switch (*it++) {
      case '-':
        spec.align_ = ALIGN_LEFT;
        break;
      case '+':
        spec.flags |= SIGN_FLAG | PLUS_FLAG;
        break;
      case '0':
        spec.fill_ = '0';
        break;
      case ' ':
        spec.flags |= SIGN_FLAG;
        break;
      case '#':
        spec.flags |= HASH_FLAG;
        break;
      default:
        --it;
        return;
    }
  }
}

template <typename OutputIt, typename Char, typename AF>
typename basic_printf_context<OutputIt, Char, AF>::format_arg
  basic_printf_context<OutputIt, Char, AF>::get_arg(
    iterator it, unsigned arg_index) {
  (void)it;
  if (arg_index == std::numeric_limits<unsigned>::max())
    return this->do_get_arg(this->parse_context().next_arg_id());
  return base::get_arg(arg_index - 1);
}

template <typename OutputIt, typename Char, typename AF>
unsigned basic_printf_context<OutputIt, Char, AF>::parse_header(
  iterator &it, format_specs &spec) {
  unsigned arg_index = std::numeric_limits<unsigned>::max();
  char_type c = *it;
  if (c >= '0' && c <= '9') {
    
    
    internal::error_handler eh;
    unsigned value = parse_nonnegative_int(it, eh);
    if (*it == '$') {  
      ++it;
      arg_index = value;
    } else {
      if (c == '0')
        spec.fill_ = '0';
      if (value != 0) {
        
        
        spec.width_ = value;
        return arg_index;
      }
    }
  }
  parse_flags(spec, it);
  
  if (*it >= '0' && *it <= '9') {
    internal::error_handler eh;
    spec.width_ = parse_nonnegative_int(it, eh);
  } else if (*it == '*') {
    ++it;
    spec.width_ = visit_format_arg(
          internal::printf_width_handler<char_type>(spec), get_arg(it));
  }
  return arg_index;
}

template <typename OutputIt, typename Char, typename AF>
void basic_printf_context<OutputIt, Char, AF>::format() {
  auto &buffer = internal::get_container(this->out());
  auto start = iterator(this->parse_context());
  auto it = start;
  using internal::pointer_from;
  while (*it) {
    char_type c = *it++;
    if (c != '%') continue;
    if (*it == c) {
      buffer.append(pointer_from(start), pointer_from(it));
      start = ++it;
      continue;
    }
    buffer.append(pointer_from(start), pointer_from(it) - 1);

    format_specs spec;
    spec.align_ = ALIGN_RIGHT;

    
    unsigned arg_index = parse_header(it, spec);

    
    if (*it == '.') {
      ++it;
      if ('0' <= *it && *it <= '9') {
        internal::error_handler eh;
        spec.precision = static_cast<int>(parse_nonnegative_int(it, eh));
      } else if (*it == '*') {
        ++it;
        spec.precision =
            visit_format_arg(internal::printf_precision_handler(), get_arg(it));
      } else {
        spec.precision = 0;
      }
    }

    format_arg arg = get_arg(it, arg_index);
    if (spec.has(HASH_FLAG) && visit_format_arg(internal::is_zero_int(), arg))
      spec.flags = static_cast<uint_least8_t>(spec.flags & (~internal::to_unsigned<int>(HASH_FLAG)));
    if (spec.fill_ == '0') {
      if (arg.is_arithmetic())
        spec.align_ = ALIGN_NUMERIC;
      else
        spec.fill_ = ' ';  
    }

    
    using internal::convert_arg;
    switch (*it++) {
    case 'h':
      if (*it == 'h')
        convert_arg<signed char>(arg, *++it);
      else
        convert_arg<short>(arg, *it);
      break;
    case 'l':
      if (*it == 'l')
        convert_arg<long long>(arg, *++it);
      else
        convert_arg<long>(arg, *it);
      break;
    case 'j':
      convert_arg<intmax_t>(arg, *it);
      break;
    case 'z':
      convert_arg<std::size_t>(arg, *it);
      break;
    case 't':
      convert_arg<std::ptrdiff_t>(arg, *it);
      break;
    case 'L':
      
      
      break;
    default:
      --it;
      convert_arg<void>(arg, *it);
    }

    
    if (!*it)
      FMT_THROW(format_error("invalid format string"));
    spec.type = static_cast<char>(*it++);
    if (arg.is_integral()) {
      
      switch (spec.type) {
      case 'i': case 'u':
        spec.type = 'd';
        break;
      case 'c':
        
        visit_format_arg(
              internal::char_converter<basic_printf_context>(arg), arg);
        break;
      }
    }

    start = it;

    
    visit_format_arg(AF(buffer, spec, *this), arg);
  }
  buffer.append(pointer_from(start), pointer_from(it));
}

template <typename Buffer>
struct basic_printf_context_t {
  typedef basic_printf_context<
    std::back_insert_iterator<Buffer>, typename Buffer::value_type> type;
};

typedef basic_printf_context_t<internal::buffer>::type printf_context;
typedef basic_printf_context_t<internal::wbuffer>::type wprintf_context;

typedef basic_format_args<printf_context> printf_args;
typedef basic_format_args<wprintf_context> wprintf_args;


template<typename... Args>
inline format_arg_store<printf_context, Args...>
  make_printf_args(const Args &... args) { return {args...}; }


template<typename... Args>
inline format_arg_store<wprintf_context, Args...>
  make_wprintf_args(const Args &... args) { return {args...}; }

template <typename S, typename Char = FMT_CHAR(S)>
inline std::basic_string<Char>
vsprintf(const S &format,
         basic_format_args<typename basic_printf_context_t<
           internal::basic_buffer<Char>>::type> args) {
  basic_memory_buffer<Char> buffer;
  printf(buffer, to_string_view(format), args);
  return to_string(buffer);
}


template <typename S, typename... Args>
inline FMT_ENABLE_IF_T(
    internal::is_string<S>::value, std::basic_string<FMT_CHAR(S)>)
    sprintf(const S &format, const Args & ... args) {
  internal::check_format_string<Args...>(format);
  typedef internal::basic_buffer<FMT_CHAR(S)> buffer;
  typedef typename basic_printf_context_t<buffer>::type context;
  format_arg_store<context, Args...> as{ args... };
  return vsprintf(to_string_view(format),
                  basic_format_args<context>(as));
}

template <typename S, typename Char = FMT_CHAR(S)>
inline int vfprintf(std::FILE *f, const S &format,
                    basic_format_args<typename basic_printf_context_t<
                      internal::basic_buffer<Char>>::type> args) {
  basic_memory_buffer<Char> buffer;
  printf(buffer, to_string_view(format), args);
  std::size_t size = buffer.size();
  return std::fwrite(
    buffer.data(), sizeof(Char), size, f) < size ? -1 : static_cast<int>(size);
}


template <typename S, typename... Args>
inline FMT_ENABLE_IF_T(internal::is_string<S>::value, int)
    fprintf(std::FILE *f, const S &format, const Args & ... args) {
  internal::check_format_string<Args...>(format);
  typedef internal::basic_buffer<FMT_CHAR(S)> buffer;
  typedef typename basic_printf_context_t<buffer>::type context;
  format_arg_store<context, Args...> as{ args... };
  return vfprintf(f, to_string_view(format),
                  basic_format_args<context>(as));
}

template <typename S, typename Char = FMT_CHAR(S)>
inline int vprintf(const S &format,
                   basic_format_args<typename basic_printf_context_t<
                    internal::basic_buffer<Char>>::type> args) {
  return vfprintf(stdout, to_string_view(format), args);
}


template <typename S, typename... Args>
inline FMT_ENABLE_IF_T(internal::is_string<S>::value, int)
    printf(const S &format_str, const Args & ... args) {
  internal::check_format_string<Args...>(format_str);
  typedef internal::basic_buffer<FMT_CHAR(S)> buffer;
  typedef typename basic_printf_context_t<buffer>::type context;
  format_arg_store<context, Args...> as{ args... };
  return vprintf(to_string_view(format_str),
                 basic_format_args<context>(as));
}

template <typename S, typename Char = FMT_CHAR(S)>
inline int vfprintf(std::basic_ostream<Char> &os,
                    const S &format,
                    basic_format_args<typename basic_printf_context_t<
                      internal::basic_buffer<Char>>::type> args) {
  basic_memory_buffer<Char> buffer;
  printf(buffer, to_string_view(format), args);
  internal::write(os, buffer);
  return static_cast<int>(buffer.size());
}


template <typename S, typename... Args>
inline FMT_ENABLE_IF_T(internal::is_string<S>::value, int)
    fprintf(std::basic_ostream<FMT_CHAR(S)> &os,
            const S &format_str, const Args & ... args) {
  internal::check_format_string<Args...>(format_str);
  typedef internal::basic_buffer<FMT_CHAR(S)> buffer;
  typedef typename basic_printf_context_t<buffer>::type context;
  format_arg_store<context, Args...> as{ args... };
  return vfprintf(os, to_string_view(format_str),
                  basic_format_args<context>(as));
}
FMT_END_NAMESPACE

#endif  
