


#ifndef SQL_BOOTSTRAP_H
#define SQL_BOOTSTRAP_H


#define MAX_BOOTSTRAP_QUERY_SIZE 20000

#define MAX_BOOTSTRAP_LINE_SIZE 20000
#define MAX_BOOTSTRAP_ERROR_LEN 256

#define READ_BOOTSTRAP_SUCCESS     0
#define READ_BOOTSTRAP_EOF         1
#define READ_BOOTSTRAP_ERROR       2
#define READ_BOOTSTRAP_QUERY_SIZE  3

typedef void *fgets_input_t;
typedef char * (*fgets_fn_t)(char *, size_t, fgets_input_t, int *error);

int read_bootstrap_query(char *query, int *query_length,
                         fgets_input_t input, fgets_fn_t fgets_fn, int *error);

#endif


