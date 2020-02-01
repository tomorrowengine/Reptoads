#ifndef GSTREAM_INCLUDED
#define GSTREAM_INCLUDED




#include <my_sys.h>                             
#include "m_ctype.h"           

class Gis_read_stream
{
public:
  enum enum_tok_types
  {
    unknown,
    eostream,
    word,
    numeric,
    l_bra,
    r_bra,
    comma
  };

  Gis_read_stream(CHARSET_INFO *charset, const char *buffer, int size)
    :m_cur(buffer), m_limit(buffer + size), m_err_msg(NULL), m_charset(charset)
  {}
  Gis_read_stream(): m_cur(NullS), m_limit(NullS), m_err_msg(NullS)
  {}
  ~Gis_read_stream()
  {
    my_free(m_err_msg);
  }

  enum enum_tok_types get_next_toc_type();
  bool lookup_next_word(LEX_STRING *res);
  bool get_next_word(LEX_STRING *);
  bool get_next_number(double *);
  bool check_next_symbol(char);

  inline void skip_space()
  {
    while ((m_cur < m_limit) && my_isspace(&my_charset_latin1, *m_cur))
      m_cur++;
  }
  
  inline bool skip_char(char skip)
  {
    skip_space();
    if ((m_cur >= m_limit) || *m_cur != skip)
      return 1;					
    m_cur++;
    return 0;
  }
  
  char next_symbol() 
  {
    skip_space();
    if (m_cur >= m_limit)
      return 0;                                 
    return *m_cur;
  }
  void set_error_msg(const char *msg);

  
  char *get_error_msg()
  {
    char *err_msg = m_err_msg;
    m_err_msg= NullS;
    return err_msg;
  }

protected:
  const char *m_cur;
  const char *m_limit;
  char *m_err_msg;
  CHARSET_INFO *m_charset;
};

#endif 
