

#ifndef STRFUNC_INCLUDED
#define STRFUNC_INCLUDED

typedef struct st_typelib TYPELIB;

ulonglong find_set(TYPELIB *lib, const char *x, size_t length, CHARSET_INFO *cs,
		   char **err_pos, uint *err_len, bool *set_warning);
ulonglong find_set_from_flags(TYPELIB *lib, uint default_name,
                              ulonglong cur_set, ulonglong default_set,
                              const char *str, uint length, CHARSET_INFO *cs,
                              char **err_pos, uint *err_len, bool *set_warning);
uint find_type(const TYPELIB *lib, const char *find, size_t length,
               bool part_match);
uint find_type2(const TYPELIB *lib, const char *find, size_t length,
                CHARSET_INFO *cs);
void unhex_type2(TYPELIB *lib);
uint check_word(TYPELIB *lib, const char *val, const char *end,
		const char **end_of_word);
int find_string_in_array(LEX_CSTRING * const haystack,
                         LEX_CSTRING * const needle,
                         CHARSET_INFO * const cs);
const char *flagset_to_string(THD *thd, LEX_CSTRING *result, ulonglong set,
                        const char *lib[]);
const char *set_to_string(THD *thd, LEX_CSTRING *result, ulonglong set,
                          const char *lib[]);


uint strconvert(CHARSET_INFO *from_cs, const char *from, size_t from_length,
                CHARSET_INFO *to_cs, char *to, size_t to_length, uint *errors);

#endif 
