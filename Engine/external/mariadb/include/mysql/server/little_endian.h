



#ifndef MY_BYTE_ORDER_ARCH_OPTIMIZED
#define float4get(V,M)   memcpy(&V, (M), sizeof(float))
#define float4store(V,M) memcpy(V, (&M), sizeof(float))
#define float8get(V,M)   doubleget((V),(M))
#define float8store(V,M) doublestore((V),(M))


#if defined(__FLOAT_WORD_ORDER) && (__FLOAT_WORD_ORDER == __BIG_ENDIAN)
#define doublestore(T,V) do { *(((char*)T)+0)=(char) ((uchar *) &V)[4];\
                              *(((char*)T)+1)=(char) ((uchar *) &V)[5];\
                              *(((char*)T)+2)=(char) ((uchar *) &V)[6];\
                              *(((char*)T)+3)=(char) ((uchar *) &V)[7];\
                              *(((char*)T)+4)=(char) ((uchar *) &V)[0];\
                              *(((char*)T)+5)=(char) ((uchar *) &V)[1];\
                              *(((char*)T)+6)=(char) ((uchar *) &V)[2];\
                              *(((char*)T)+7)=(char) ((uchar *) &V)[3]; }\
                         while(0)
#define doubleget(V,M)   do { double def_temp;\
                              ((uchar*) &def_temp)[0]=(M)[4];\
                              ((uchar*) &def_temp)[1]=(M)[5];\
                              ((uchar*) &def_temp)[2]=(M)[6];\
                              ((uchar*) &def_temp)[3]=(M)[7];\
                              ((uchar*) &def_temp)[4]=(M)[0];\
                              ((uchar*) &def_temp)[5]=(M)[1];\
                              ((uchar*) &def_temp)[6]=(M)[2];\
                              ((uchar*) &def_temp)[7]=(M)[3];\
                              (V) = def_temp; } while(0)
#else 


#define doublestore(T,V) memcpy((T), (void*) &V, sizeof(double))
#define doubleget(V,M)	 memcpy(&V, (M), sizeof(double))

#endif 

#endif 

#define ushortget(V,M)	do { uchar *pM= (uchar*)(M);V = uint2korr(pM);} while(0)
#define shortget(V,M)	do { uchar *pM= (uchar*)(M);V = sint2korr(pM);} while(0)
#define longget(V,M)	do { uchar *pM= (uchar*)(M);V = sint4korr(pM);} while(0)
#define ulongget(V,M)   do { uchar *pM= (uchar*)(M);V = uint4korr(pM);} while(0)
#define shortstore(T,V) int2store(T,V)
#define longstore(T,V)	int4store(T,V)

#ifndef floatstore

#define floatstore(T,V)  memcpy((T), (void*) (&V), sizeof(float))
#define floatget(V,M)    memcpy(&V, (M), sizeof(float))
#endif
#ifndef doubleget
#define doubleget(V,M)	 memcpy(&V, (M), sizeof(double))
#define doublestore(T,V) memcpy((T), (void *) &V, sizeof(double))
#endif 

#define longlongget(V,M) memcpy(&V, (M), sizeof(ulonglong))
#define longlongstore(T,V) memcpy((T), &V, sizeof(ulonglong))
