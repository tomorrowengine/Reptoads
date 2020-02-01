


#define sint2korr(A)	(int16) (((int16) ((uchar) (A)[0])) +\
				 ((int16) ((int16) (A)[1]) << 8))
#define sint3korr(A)	((int32) ((((uchar) (A)[2]) & 128) ? \
				  (((uint32) 255L << 24) | \
				   (((uint32) (uchar) (A)[2]) << 16) |\
				   (((uint32) (uchar) (A)[1]) << 8) | \
				   ((uint32) (uchar) (A)[0])) : \
				  (((uint32) (uchar) (A)[2]) << 16) |\
				  (((uint32) (uchar) (A)[1]) << 8) | \
				  ((uint32) (uchar) (A)[0])))
#define sint4korr(A)	(int32) (((int32) ((uchar) (A)[0])) +\
				(((int32) ((uchar) (A)[1]) << 8)) +\
				(((int32) ((uchar) (A)[2]) << 16)) +\
				(((int32) ((int16) (A)[3]) << 24)))
#define sint8korr(A)	(longlong) uint8korr(A)
#define uint2korr(A)	(uint16) (((uint16) ((uchar) (A)[0])) +\
				  ((uint16) ((uchar) (A)[1]) << 8))
#define uint3korr(A)	(uint32) (((uint32) ((uchar) (A)[0])) +\
				  (((uint32) ((uchar) (A)[1])) << 8) +\
				  (((uint32) ((uchar) (A)[2])) << 16))
#define uint4korr(A)	(uint32) (((uint32) ((uchar) (A)[0])) +\
				  (((uint32) ((uchar) (A)[1])) << 8) +\
				  (((uint32) ((uchar) (A)[2])) << 16) +\
				  (((uint32) ((uchar) (A)[3])) << 24))
#define uint5korr(A)	((ulonglong)(((uint32) ((uchar) (A)[0])) +\
				    (((uint32) ((uchar) (A)[1])) << 8) +\
				    (((uint32) ((uchar) (A)[2])) << 16) +\
				    (((uint32) ((uchar) (A)[3])) << 24)) +\
				    (((ulonglong) ((uchar) (A)[4])) << 32))
#define uint6korr(A)	((ulonglong)(((uint32)    ((uchar) (A)[0]))          + \
                                     (((uint32)    ((uchar) (A)[1])) << 8)   + \
                                     (((uint32)    ((uchar) (A)[2])) << 16)  + \
                                     (((uint32)    ((uchar) (A)[3])) << 24)) + \
                         (((ulonglong) ((uchar) (A)[4])) << 32) +       \
                         (((ulonglong) ((uchar) (A)[5])) << 40))
#define uint8korr(A)	((ulonglong)(((uint32) ((uchar) (A)[0])) +\
				    (((uint32) ((uchar) (A)[1])) << 8) +\
				    (((uint32) ((uchar) (A)[2])) << 16) +\
				    (((uint32) ((uchar) (A)[3])) << 24)) +\
			(((ulonglong) (((uint32) ((uchar) (A)[4])) +\
				    (((uint32) ((uchar) (A)[5])) << 8) +\
				    (((uint32) ((uchar) (A)[6])) << 16) +\
				    (((uint32) ((uchar) (A)[7])) << 24))) <<\
				    32))
#define int2store(T,A)       do { uint def_temp= (uint) (A) ;\
                                  *((uchar*) (T))=  (uchar)(def_temp); \
                                   *((uchar*) (T)+1)=(uchar)((def_temp >> 8)); \
                             } while(0)
#define int3store(T,A)       do { \
                                  *((uchar*)(T))=(uchar) ((A));\
                                  *((uchar*) (T)+1)=(uchar) (((A) >> 8));\
                                  *((uchar*)(T)+2)=(uchar) (((A) >> 16)); \
                                  } while(0)
#define int4store(T,A)       do { *((char *)(T))=(char) ((A));\
                                  *(((char *)(T))+1)=(char) (((A) >> 8));\
                                  *(((char *)(T))+2)=(char) (((A) >> 16));\
                                  *(((char *)(T))+3)=(char) (((A) >> 24));\
                             } while(0)
#define int5store(T,A)       do { *((char *)(T))=     (char)((A));  \
                                  *(((char *)(T))+1)= (char)(((A) >> 8)); \
                                  *(((char *)(T))+2)= (char)(((A) >> 16)); \
                                  *(((char *)(T))+3)= (char)(((A) >> 24)); \
                                  *(((char *)(T))+4)= (char)(((A) >> 32)); \
		             } while(0)
#define int6store(T,A)       do { *((char *)(T))=     (char)((A)); \
                                  *(((char *)(T))+1)= (char)(((A) >> 8)); \
                                  *(((char *)(T))+2)= (char)(((A) >> 16)); \
                                  *(((char *)(T))+3)= (char)(((A) >> 24)); \
                                  *(((char *)(T))+4)= (char)(((A) >> 32)); \
                                  *(((char *)(T))+5)= (char)(((A) >> 40)); \
                             } while(0)
#define int8store(T,A)       do { uint def_temp= (uint) (A), \
                                       def_temp2= (uint) ((A) >> 32); \
                                  int4store((T),def_temp); \
                                  int4store((T+4),def_temp2);\
                             } while(0)
