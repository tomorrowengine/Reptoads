

#ifndef SQL_CALLBACK_INCLUDED
#define SQL_CALLBACK_INCLUDED



#define MYSQL_CALLBACK(OBJ, FUNC, PARAMS)         \
  do {                                            \
    if ((OBJ) && ((OBJ)->FUNC))                   \
      (OBJ)->FUNC PARAMS;                         \
  } while (0)

#define MYSQL_CALLBACK_ELSE(OBJ, FUNC, PARAMS, ELSE)    \
  (((OBJ) && ((OBJ)->FUNC)) ? (OBJ)->FUNC PARAMS : (ELSE))

#endif 
