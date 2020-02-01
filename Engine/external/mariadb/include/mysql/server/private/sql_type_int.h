

#ifndef SQL_TYPE_INT_INCLUDED
#define SQL_TYPE_INT_INCLUDED



class Longlong_hybrid
{
protected:
  longlong m_value;
  bool m_unsigned;
  int cmp_signed(const Longlong_hybrid& other) const
  {
    return m_value < other.m_value ? -1 : m_value == other.m_value ? 0 : 1;
  }
  int cmp_unsigned(const Longlong_hybrid& other) const
  {
    return (ulonglong) m_value < (ulonglong) other.m_value ? -1 :
            m_value == other.m_value ? 0 : 1;
  }
public:
  Longlong_hybrid(longlong nr, bool unsigned_flag)
   :m_value(nr), m_unsigned(unsigned_flag)
  { }
  longlong value() const { return m_value; }
  bool is_unsigned() const { return m_unsigned; }
  bool is_unsigned_outside_of_signed_range() const
  {
    return m_unsigned && ((ulonglong) m_value) > (ulonglong) LONGLONG_MAX;
  }
  bool neg() const { return m_value < 0 && !m_unsigned; }
  ulonglong abs() const
  {
    if (m_unsigned)
      return (ulonglong) m_value;
    if (m_value == LONGLONG_MIN) 
      return ((ulonglong) LONGLONG_MAX) + 1;
    return m_value < 0 ? -m_value : m_value;
  }
  
  ulonglong to_ulonglong(ulonglong upper_bound) const
  {
    return neg() ? 0 :
           (ulonglong) m_value > upper_bound ? upper_bound :
           (ulonglong) m_value;
  }
  uint to_uint(uint upper_bound) const
  {
    return (uint) to_ulonglong(upper_bound);
  }
  int cmp(const Longlong_hybrid& other) const
  {
    if (m_unsigned == other.m_unsigned)
      return m_unsigned ? cmp_unsigned(other) : cmp_signed(other);
    if (is_unsigned_outside_of_signed_range())
      return 1;
    if (other.is_unsigned_outside_of_signed_range())
      return -1;
    
    return cmp_signed(other);
  }
};

#endif 
