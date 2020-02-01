#ifndef SQL_HSET_INCLUDED
#define SQL_HSET_INCLUDED


#include "hash.h"




template <typename T>
class Hash_set
{
public:
  enum { START_SIZE= 8 };
  
  Hash_set(uchar *(*K)(const T *, size_t *, my_bool),
           CHARSET_INFO *cs= &my_charset_bin)
  {
    my_hash_clear(&m_hash);
    m_hash.get_key= (my_hash_get_key)K;
    m_hash.charset= cs;
  }
  
  ~Hash_set()
  {
    my_hash_free(&m_hash);
  }
  
  bool insert(T *value)
  {
    my_hash_init_opt(&m_hash, m_hash.charset, START_SIZE, 0, 0,
                     m_hash.get_key, 0, MYF(0));
    size_t key_len;
    uchar *v= reinterpret_cast<uchar *>(value);
    const uchar *key= m_hash.get_key(v, &key_len, FALSE);
    if (find(key, key_len) == NULL)
      return my_hash_insert(&m_hash, v);
    return FALSE;
  }
  bool remove(T *value)
  {
    return my_hash_delete(&m_hash, reinterpret_cast<uchar*>(value));
  }
  T *find(const void *key, size_t klen) const
  {
    return (T*)my_hash_search(&m_hash, reinterpret_cast<const uchar *>(key), klen);
  }
  
  bool is_empty() const { return m_hash.records == 0; }
  
  size_t size() const { return static_cast<size_t>(m_hash.records); }
  const T* at(size_t i) const
  {
    return reinterpret_cast<T*>(my_hash_element(const_cast<HASH*>(&m_hash), i));
  }
  
  class Iterator
  {
  public:
    Iterator(Hash_set &hash_set)
      : m_hash(&hash_set.m_hash),
        m_idx(0)
    {}
    
    inline T *operator++(int)
    {
      if (m_idx < m_hash->records)
        return reinterpret_cast<T*>(my_hash_element(m_hash, m_idx++));
      return NULL;
    }
    void rewind() { m_idx= 0; }
  private:
    HASH *m_hash;
    uint m_idx;
  };
private:
  HASH m_hash;
};

#endif 
