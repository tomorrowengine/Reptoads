#ifndef MYSQLD_SUFFIX_INCLUDED
#define MYSQLD_SUFFIX_INCLUDED





#ifdef MYSQL_SERVER_SUFFIX
#define MYSQL_SERVER_SUFFIX_STR STRINGIFY_ARG(MYSQL_SERVER_SUFFIX)
#else
#define MYSQL_SERVER_SUFFIX_STR MYSQL_SERVER_SUFFIX_DEF
#endif
#endif 
