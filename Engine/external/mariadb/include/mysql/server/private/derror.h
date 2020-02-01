

#ifndef DERROR_INCLUDED
#define DERROR_INCLUDED

bool init_errmessage(void);
void free_error_messages();
bool read_texts(const char *file_name, const char *language,
                const char ****data);

#endif 
