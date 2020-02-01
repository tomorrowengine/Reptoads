

#ifndef HEADER_UI_H
# define HEADER_UI_H

# include <openssl/opensslconf.h>

# if OPENSSL_API_COMPAT < 0x10100000L
#  include <openssl/crypto.h>
# endif
# include <openssl/safestack.h>
# include <openssl/pem.h>
# include <openssl/ossl_typ.h>
# include <openssl/uierr.h>


# if OPENSSL_API_COMPAT < 0x10200000L
#  ifdef OPENSSL_NO_UI_CONSOLE
#   define OPENSSL_NO_UI
#  endif
# endif

# ifdef  __cplusplus
extern "C" {
# endif




UI *UI_new(void);
UI *UI_new_method(const UI_METHOD *method);
void UI_free(UI *ui);


int UI_add_input_string(UI *ui, const char *prompt, int flags,
                        char *result_buf, int minsize, int maxsize);
int UI_dup_input_string(UI *ui, const char *prompt, int flags,
                        char *result_buf, int minsize, int maxsize);
int UI_add_verify_string(UI *ui, const char *prompt, int flags,
                         char *result_buf, int minsize, int maxsize,
                         const char *test_buf);
int UI_dup_verify_string(UI *ui, const char *prompt, int flags,
                         char *result_buf, int minsize, int maxsize,
                         const char *test_buf);
int UI_add_input_boolean(UI *ui, const char *prompt, const char *action_desc,
                         const char *ok_chars, const char *cancel_chars,
                         int flags, char *result_buf);
int UI_dup_input_boolean(UI *ui, const char *prompt, const char *action_desc,
                         const char *ok_chars, const char *cancel_chars,
                         int flags, char *result_buf);
int UI_add_info_string(UI *ui, const char *text);
int UI_dup_info_string(UI *ui, const char *text);
int UI_add_error_string(UI *ui, const char *text);
int UI_dup_error_string(UI *ui, const char *text);



# define UI_INPUT_FLAG_ECHO              0x01

# define UI_INPUT_FLAG_DEFAULT_PWD       0x02


# define UI_INPUT_FLAG_USER_BASE 16


char *UI_construct_prompt(UI *ui_method,
                          const char *object_desc, const char *object_name);


void *UI_add_user_data(UI *ui, void *user_data);

int UI_dup_user_data(UI *ui, void *user_data);

void *UI_get0_user_data(UI *ui);


const char *UI_get0_result(UI *ui, int i);
int UI_get_result_length(UI *ui, int i);


int UI_process(UI *ui);


int UI_ctrl(UI *ui, int cmd, long i, void *p, void (*f) (void));



# define UI_CTRL_PRINT_ERRORS            1

# define UI_CTRL_IS_REDOABLE             2


# define UI_set_app_data(s,arg)         UI_set_ex_data(s,0,arg)
# define UI_get_app_data(s)             UI_get_ex_data(s,0)

# define UI_get_ex_new_index(l, p, newf, dupf, freef) \
    CRYPTO_get_ex_new_index(CRYPTO_EX_INDEX_UI, l, p, newf, dupf, freef)
int UI_set_ex_data(UI *r, int idx, void *arg);
void *UI_get_ex_data(UI *r, int idx);


void UI_set_default_method(const UI_METHOD *meth);
const UI_METHOD *UI_get_default_method(void);
const UI_METHOD *UI_get_method(UI *ui);
const UI_METHOD *UI_set_method(UI *ui, const UI_METHOD *meth);

# ifndef OPENSSL_NO_UI_CONSOLE


UI_METHOD *UI_OpenSSL(void);

# endif


const UI_METHOD *UI_null(void);





typedef struct ui_string_st UI_STRING;
DEFINE_STACK_OF(UI_STRING)


enum UI_string_types {
    UIT_NONE = 0,
    UIT_PROMPT,                 
    UIT_VERIFY,                 
    UIT_BOOLEAN,                
    UIT_INFO,                   
    UIT_ERROR                   
};


UI_METHOD *UI_create_method(const char *name);
void UI_destroy_method(UI_METHOD *ui_method);
int UI_method_set_opener(UI_METHOD *method, int (*opener) (UI *ui));
int UI_method_set_writer(UI_METHOD *method,
                         int (*writer) (UI *ui, UI_STRING *uis));
int UI_method_set_flusher(UI_METHOD *method, int (*flusher) (UI *ui));
int UI_method_set_reader(UI_METHOD *method,
                         int (*reader) (UI *ui, UI_STRING *uis));
int UI_method_set_closer(UI_METHOD *method, int (*closer) (UI *ui));
int UI_method_set_data_duplicator(UI_METHOD *method,
                                  void *(*duplicator) (UI *ui, void *ui_data),
                                  void (*destructor)(UI *ui, void *ui_data));
int UI_method_set_prompt_constructor(UI_METHOD *method,
                                     char *(*prompt_constructor) (UI *ui,
                                                                  const char
                                                                  *object_desc,
                                                                  const char
                                                                  *object_name));
int UI_method_set_ex_data(UI_METHOD *method, int idx, void *data);
int (*UI_method_get_opener(const UI_METHOD *method)) (UI *);
int (*UI_method_get_writer(const UI_METHOD *method)) (UI *, UI_STRING *);
int (*UI_method_get_flusher(const UI_METHOD *method)) (UI *);
int (*UI_method_get_reader(const UI_METHOD *method)) (UI *, UI_STRING *);
int (*UI_method_get_closer(const UI_METHOD *method)) (UI *);
char *(*UI_method_get_prompt_constructor(const UI_METHOD *method))
    (UI *, const char *, const char *);
void *(*UI_method_get_data_duplicator(const UI_METHOD *method)) (UI *, void *);
void (*UI_method_get_data_destructor(const UI_METHOD *method)) (UI *, void *);
const void *UI_method_get_ex_data(const UI_METHOD *method, int idx);




enum UI_string_types UI_get_string_type(UI_STRING *uis);

int UI_get_input_flags(UI_STRING *uis);

const char *UI_get0_output_string(UI_STRING *uis);

const char *UI_get0_action_string(UI_STRING *uis);

const char *UI_get0_result_string(UI_STRING *uis);
int UI_get_result_string_length(UI_STRING *uis);

const char *UI_get0_test_string(UI_STRING *uis);

int UI_get_result_minsize(UI_STRING *uis);

int UI_get_result_maxsize(UI_STRING *uis);

int UI_set_result(UI *ui, UI_STRING *uis, const char *result);
int UI_set_result_ex(UI *ui, UI_STRING *uis, const char *result, int len);


int UI_UTIL_read_pw_string(char *buf, int length, const char *prompt,
                           int verify);
int UI_UTIL_read_pw(char *buf, char *buff, int size, const char *prompt,
                    int verify);
UI_METHOD *UI_UTIL_wrap_read_pem_callback(pem_password_cb *cb, int rwflag);


# ifdef  __cplusplus
}
# endif
#endif
