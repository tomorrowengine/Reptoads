#ifndef COMPAT56_H_INCLUDED
#define COMPAT56_H_INCLUDED




#define MY_PACKED_TIME_GET_INT_PART(x)     ((x) >> 24)
#define MY_PACKED_TIME_GET_FRAC_PART(x)    ((x) % (1LL << 24))
#define MY_PACKED_TIME_MAKE(i, f)          ((((longlong) (i)) << 24) + (f))
#define MY_PACKED_TIME_MAKE_INT(i)         ((((longlong) (i)) << 24))

longlong TIME_to_longlong_datetime_packed(const MYSQL_TIME *);
longlong TIME_to_longlong_time_packed(const MYSQL_TIME *);

void TIME_from_longlong_datetime_packed(MYSQL_TIME *ltime, longlong nr);
void TIME_from_longlong_time_packed(MYSQL_TIME *ltime, longlong nr);

void my_datetime_packed_to_binary(longlong nr, uchar *ptr, uint dec);
longlong my_datetime_packed_from_binary(const uchar *ptr, uint dec);
uint my_datetime_binary_length(uint dec);

void my_time_packed_to_binary(longlong nr, uchar *ptr, uint dec);
longlong my_time_packed_from_binary(const uchar *ptr, uint dec);
uint my_time_binary_length(uint dec);

void my_timestamp_to_binary(const struct timeval *tm, uchar *ptr, uint dec);
void my_timestamp_from_binary(struct timeval *tm, const uchar *ptr, uint dec);
uint my_timestamp_binary_length(uint dec);


#endif 
