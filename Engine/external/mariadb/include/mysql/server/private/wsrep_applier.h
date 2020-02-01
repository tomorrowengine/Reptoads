

#ifndef WSREP_APPLIER_H
#define WSREP_APPLIER_H

#include <my_config.h>
#include "../wsrep/wsrep_api.h"

void wsrep_set_apply_format(THD* thd, Format_description_log_event* ev);
Format_description_log_event* wsrep_get_apply_format(THD* thd);


extern "C" {

wsrep_cb_status_t wsrep_apply_cb(void *ctx,
                                 const void* buf, size_t buf_len,
                                 uint32_t flags,
                                 const wsrep_trx_meta_t* meta);

wsrep_cb_status_t wsrep_commit_cb(void *ctx,
                                  uint32_t flags,
                                  const wsrep_trx_meta_t* meta,
                                  wsrep_bool_t* exit,
                                  bool commit);

wsrep_cb_status_t wsrep_unordered_cb(void*       ctx,
                                     const void* data,
                                     size_t      size);

} 
#endif 
