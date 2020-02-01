






#ifndef FMT_POSIX_H_
#define FMT_POSIX_H_

#if defined(__MINGW32__) || defined(__CYGWIN__)

# undef __STRICT_ANSI__
#endif

#include <errno.h>
#include <fcntl.h>   
#include <locale.h>  
#include <stdio.h>
#include <stdlib.h>  

#include <cstddef>

#if defined __APPLE__ || defined(__FreeBSD__)
# include <xlocale.h>  
#endif

#include "format.h"

#ifndef FMT_POSIX
# if defined(_WIN32) && !defined(__MINGW32__)

#  define FMT_POSIX(call) _##call
# else
#  define FMT_POSIX(call) call
# endif
#endif


#ifdef FMT_SYSTEM
# define FMT_POSIX_CALL(call) FMT_SYSTEM(call)
#else
# define FMT_SYSTEM(call) call
# ifdef _WIN32

#  define FMT_POSIX_CALL(call) ::_##call
# else
#  define FMT_POSIX_CALL(call) ::call
# endif
#endif



#ifndef _WIN32
# define FMT_RETRY_VAL(result, expression, error_result) \
  do { \
    result = (expression); \
  } while (result == error_result && errno == EINTR)
#else
# define FMT_RETRY_VAL(result, expression, error_result) result = (expression)
#endif

#define FMT_RETRY(result, expression) FMT_RETRY_VAL(result, expression, -1)

FMT_BEGIN_NAMESPACE


template <typename Char>
class basic_cstring_view {
 private:
  const Char *data_;

 public:
  
  basic_cstring_view(const Char *s) : data_(s) {}

  
  basic_cstring_view(const std::basic_string<Char> &s) : data_(s.c_str()) {}

  
  const Char *c_str() const { return data_; }
};

typedef basic_cstring_view<char> cstring_view;
typedef basic_cstring_view<wchar_t> wcstring_view;


class error_code {
 private:
  int value_;

 public:
  explicit error_code(int value = 0) FMT_NOEXCEPT : value_(value) {}

  int get() const FMT_NOEXCEPT { return value_; }
};


class buffered_file {
 private:
  FILE *file_;

  friend class file;

  explicit buffered_file(FILE *f) : file_(f) {}

 public:
  
  buffered_file() FMT_NOEXCEPT : file_(FMT_NULL) {}

  
  FMT_API ~buffered_file() FMT_NOEXCEPT;

 private:
  buffered_file(const buffered_file &) = delete;
  void operator=(const buffered_file &) = delete;


 public:
  buffered_file(buffered_file &&other) FMT_NOEXCEPT : file_(other.file_) {
    other.file_ = FMT_NULL;
  }

  buffered_file& operator=(buffered_file &&other) {
    close();
    file_ = other.file_;
    other.file_ = FMT_NULL;
    return *this;
  }

  
  FMT_API buffered_file(cstring_view filename, cstring_view mode);

  
  FMT_API void close();

  
  FILE *get() const FMT_NOEXCEPT { return file_; }

  
  
  FMT_API int (fileno)() const;

  void vprint(string_view format_str, format_args args) {
    fmt::vprint(file_, format_str, args);
  }

  template <typename... Args>
  inline void print(string_view format_str, const Args & ... args) {
    vprint(format_str, make_format_args(args...));
  }
};







class file {
 private:
  int fd_;  

  
  explicit file(int fd) : fd_(fd) {}

 public:
  
  enum {
    RDONLY = FMT_POSIX(O_RDONLY), 
    WRONLY = FMT_POSIX(O_WRONLY), 
    RDWR   = FMT_POSIX(O_RDWR)    
  };

  
  file() FMT_NOEXCEPT : fd_(-1) {}

  
  FMT_API file(cstring_view path, int oflag);

 private:
  file(const file &) = delete;
  void operator=(const file &) = delete;

 public:
  file(file &&other) FMT_NOEXCEPT : fd_(other.fd_) {
    other.fd_ = -1;
  }

  file& operator=(file &&other) {
    close();
    fd_ = other.fd_;
    other.fd_ = -1;
    return *this;
  }

  
  FMT_API ~file() FMT_NOEXCEPT;

  
  int descriptor() const FMT_NOEXCEPT { return fd_; }

  
  FMT_API void close();

  
  
  FMT_API long long size() const;

  
  FMT_API std::size_t read(void *buffer, std::size_t count);

  
  FMT_API std::size_t write(const void *buffer, std::size_t count);

  
  
  FMT_API static file dup(int fd);

  
  
  FMT_API void dup2(int fd);

  
  
  FMT_API void dup2(int fd, error_code &ec) FMT_NOEXCEPT;

  
  
  FMT_API static void pipe(file &read_end, file &write_end);

  
  
  FMT_API buffered_file fdopen(const char *mode);
};


long getpagesize();

#if (defined(LC_NUMERIC_MASK) || defined(_MSC_VER)) && \
    !defined(__ANDROID__) && !defined(__CYGWIN__) && !defined(__OpenBSD__) && \
    !defined(__NEWLIB_H__)
# define FMT_LOCALE
#endif

#ifdef FMT_LOCALE

class Locale {
 private:
# ifdef _MSC_VER
  typedef _locale_t locale_t;

  enum { LC_NUMERIC_MASK = LC_NUMERIC };

  static locale_t newlocale(int category_mask, const char *locale, locale_t) {
    return _create_locale(category_mask, locale);
  }

  static void freelocale(locale_t locale) {
    _free_locale(locale);
  }

  static double strtod_l(const char *nptr, char **endptr, _locale_t locale) {
    return _strtod_l(nptr, endptr, locale);
  }
# endif

  locale_t locale_;

  Locale(const Locale &) = delete;
  void operator=(const Locale &) = delete;

 public:
  typedef locale_t Type;

  Locale() : locale_(newlocale(LC_NUMERIC_MASK, "C", FMT_NULL)) {
    if (!locale_)
      FMT_THROW(system_error(errno, "cannot create locale"));
  }
  ~Locale() { freelocale(locale_); }

  Type get() const { return locale_; }

  
  
  double strtod(const char *&str) const {
    char *end = FMT_NULL;
    double result = strtod_l(str, &end, locale_);
    str = end;
    return result;
  }
};
#endif  
FMT_END_NAMESPACE

#endif  
