


#ifndef _my_nosys_h
#define _my_nosys_h
#ifdef	__cplusplus
extern "C" {
#endif

#ifndef __MY_NOSYS__
#define __MY_NOSYS__

#ifndef HAVE_STDLIB_H
#include <malloc.h>
#endif

#undef my_read
#undef my_write
#undef my_seek
#define my_read(a,b,c,d) my_quick_read(a,b,c,d)
#define my_write(a,b,c,d) my_quick_write(a,b,c)
extern size_t my_quick_read(File Filedes,uchar *Buffer,size_t Count,
                            myf myFlags);
extern size_t my_quick_write(File Filedes,const uchar *Buffer,size_t Count);

#endif 

#ifdef	__cplusplus
}
#endif
#endif
