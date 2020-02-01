

#ifndef DES_KEY_FILE_INCLUDED
#define DES_KEY_FILE_INCLUDED

#ifdef HAVE_OPENSSL
#include <openssl/des.h>

#include "violite.h"                

struct st_des_keyblock
{
  DES_cblock key1, key2, key3;
};

struct st_des_keyschedule
{
  DES_key_schedule ks1, ks2, ks3;
};

extern struct st_des_keyschedule des_keyschedule[10];
extern uint des_default_key;

bool load_des_key_file(const char *file_name);
#endif 

#endif 
