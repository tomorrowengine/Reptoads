#ifndef FIELD_COMP_H_INCLUDED
#define FIELD_COMP_H_INCLUDED



#define MAX_COMPRESSION_METHODS 16

struct Compression_method
{
  const char *name;
  uint (*compress)(THD *thd, char *to, const char *from, uint length);
  int (*uncompress)(String *to, const uchar *from, uint from_length,
                    uint field_length);
};


extern Compression_method compression_methods[MAX_COMPRESSION_METHODS];
#define zlib_compression_method (&compression_methods[8])

#endif
