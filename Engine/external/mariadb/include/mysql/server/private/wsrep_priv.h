



#ifndef WSREP_PRIV_H
#define WSREP_PRIV_H

#include "wsrep_mysqld.h"
#include "../wsrep/wsrep_api.h"

#include <log.h>
#include <pthread.h>
#include <cstdio>

my_bool wsrep_ready_set (my_bool x);

ssize_t wsrep_sst_prepare   (void** msg);
wsrep_cb_status wsrep_sst_donate_cb (void* app_ctx,
                                     void* recv_ctx,
                                     const void* msg, size_t msg_len,
                                     const wsrep_gtid_t* state_id,
                                     const char* state, size_t state_len,
                                     bool bypass);

extern wsrep_uuid_t  local_uuid;
extern wsrep_seqno_t local_seqno;


bool wsrep_sst_received (wsrep_t*            const wsrep,
                         const wsrep_uuid_t&       uuid,
                         const wsrep_seqno_t       seqno,
                         const void*         const state,
                         const size_t              state_len,
                         const bool                implicit);

void wsrep_sst_complete(const wsrep_uuid_t*, wsrep_seqno_t, bool);

void wsrep_notify_status (wsrep_member_status_t new_status,
                          const wsrep_view_info_t* view = 0);

#endif 
