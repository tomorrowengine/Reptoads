#ifndef MY_DECIMAL_LIMITS_INCLUDED
#define MY_DECIMAL_LIMITS_INCLUDED


#define DECIMAL_LONGLONG_DIGITS 22
#define DECIMAL_LONG_DIGITS 10
#define DECIMAL_LONG3_DIGITS 8


#define DECIMAL_BUFF_LENGTH 9


#define DECIMAL_MAX_POSSIBLE_PRECISION (DECIMAL_BUFF_LENGTH * 9)



#define DECIMAL_MAX_PRECISION (DECIMAL_MAX_POSSIBLE_PRECISION - 8*2)
#define DECIMAL_MAX_SCALE 38
#define DECIMAL_NOT_SPECIFIED 39


#define DECIMAL_MAX_STR_LENGTH (DECIMAL_MAX_POSSIBLE_PRECISION + 2)

#endif 
