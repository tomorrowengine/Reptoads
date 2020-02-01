

#ifndef HEADER_ASYNCERR_H
# define HEADER_ASYNCERR_H

# ifdef  __cplusplus
extern "C"
# endif
int ERR_load_ASYNC_strings(void);


# define ASYNC_F_ASYNC_CTX_NEW                            100
# define ASYNC_F_ASYNC_INIT_THREAD                        101
# define ASYNC_F_ASYNC_JOB_NEW                            102
# define ASYNC_F_ASYNC_PAUSE_JOB                          103
# define ASYNC_F_ASYNC_START_FUNC                         104
# define ASYNC_F_ASYNC_START_JOB                          105
# define ASYNC_F_ASYNC_WAIT_CTX_SET_WAIT_FD               106


# define ASYNC_R_FAILED_TO_SET_POOL                       101
# define ASYNC_R_FAILED_TO_SWAP_CONTEXT                   102
# define ASYNC_R_INIT_FAILED                              105
# define ASYNC_R_INVALID_POOL_SIZE                        103

#endif
