#ifndef SQL_CRYPT_INCLUDED
#define SQL_CRYPT_INCLUDED




#ifdef USE_PRAGMA_INTERFACE 
#pragma interface			
#endif

#include "sql_alloc.h"                       
#include "my_rnd.h"                          

class SQL_CRYPT :public Sql_alloc
{
  struct my_rnd_struct rand,org_rand;
  char decode_buff[256],encode_buff[256];
  uint shift;
 public:
  SQL_CRYPT() {}
  SQL_CRYPT(ulong *seed)
  {
    init(seed);
  }
  ~SQL_CRYPT() {}
  void init(ulong *seed);
  void reinit() { shift=0; rand=org_rand; }
  void encode(char *str, uint length);
  void decode(char *str, uint length);
};

#endif 
