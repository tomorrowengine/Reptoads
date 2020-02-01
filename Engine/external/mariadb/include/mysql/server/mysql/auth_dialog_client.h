#ifndef MYSQL_AUTH_DIALOG_CLIENT_INCLUDED




struct st_mysql;

#define MYSQL_AUTH_DIALOG_CLIENT_INCLUDED


typedef char *(*mysql_authentication_dialog_ask_t)(struct st_mysql *mysql,
                      int type, const char *prompt, char *buf, int buf_len);


#define ORDINARY_QUESTION       "\2"
#define LAST_QUESTION           "\3"
#define PASSWORD_QUESTION       "\4"
#define LAST_PASSWORD           "\5"

#endif
