

#ifndef PASSWORD_INCLUDED
#define PASSWORD_INCLUDED

C_MODE_START

void my_make_scrambled_password_323(char *to, const char *password,
                                    size_t pass_len);
void my_make_scrambled_password(char *to, const char *password,
                                size_t pass_len);

void hash_password(ulong *result, const char *password, uint password_len);

C_MODE_END

#endif 
