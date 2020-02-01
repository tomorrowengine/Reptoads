

#ifndef _mysql_time_h_
#define _mysql_time_h_


typedef long my_time_t;




enum enum_mysql_timestamp_type
{
  MYSQL_TIMESTAMP_NONE= -2, MYSQL_TIMESTAMP_ERROR= -1,
  MYSQL_TIMESTAMP_DATE= 0, MYSQL_TIMESTAMP_DATETIME= 1, MYSQL_TIMESTAMP_TIME= 2
};



typedef struct st_mysql_time
{
  unsigned int  year, month, day, hour, minute, second;
  unsigned long second_part;
  my_bool       neg;
  enum enum_mysql_timestamp_type time_type;
} MYSQL_TIME;

#endif 
