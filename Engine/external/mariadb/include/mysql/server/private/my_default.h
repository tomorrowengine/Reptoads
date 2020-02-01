



#ifndef MY_DEFAULT_INCLUDED
#define MY_DEFAULT_INCLUDED

C_MODE_START

extern const char *my_defaults_extra_file;
extern const char *my_defaults_group_suffix;
extern const char *my_defaults_file;
extern my_bool my_getopt_use_args_separator;
extern my_bool my_getopt_is_args_separator(const char* arg);


typedef int (*Process_option_func)(void *ctx, const char *group_name,
                                   const char *option);

extern int get_defaults_options(int argc, char **argv,
                                char **defaults, char **extra_defaults,
                                char **group_suffix);
extern int my_load_defaults(const char *conf_file, const char **groups,
                            int *argc, char ***argv, const char ***);
extern int load_defaults(const char *conf_file, const char **groups,
                         int *argc, char ***argv);
extern int my_search_option_files(const char *conf_file, int *argc,
                                  char ***argv, uint *args_used,
                                  Process_option_func func, void *func_ctx,
                                  const char **default_directories);
extern void free_defaults(char **argv);
extern void my_print_default_files(const char *conf_file);
extern void print_defaults(const char *conf_file, const char **groups);



#define load_defaults_or_exit(A, B, C, D) switch (load_defaults(A, B, C, D)) { \
                                          case 0: break; \
                                          case 4: my_end(0); exit(0); \
                                          default: my_end(0); exit(1); }

C_MODE_END

#endif 
