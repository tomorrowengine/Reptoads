



#ifndef _my_user_h_
#define _my_user_h_

C_MODE_START

int parse_user(const char *user_id_str, size_t user_id_len,
               char *user_name_str, size_t *user_name_len,
               char *host_name_str, size_t *host_name_len);

C_MODE_END

#endif 
