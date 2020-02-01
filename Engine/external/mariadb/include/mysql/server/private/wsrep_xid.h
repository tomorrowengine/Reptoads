

#ifndef WSREP_XID_H
#define WSREP_XID_H

#include <my_config.h>

#ifdef WITH_WSREP

#include "../wsrep/wsrep_api.h"
#include "handler.h" 

void wsrep_xid_init(xid_t*, const wsrep_uuid_t&, wsrep_seqno_t);
const wsrep_uuid_t* wsrep_xid_uuid(const XID&);
wsrep_seqno_t wsrep_xid_seqno(const XID&);


bool wsrep_get_SE_checkpoint(wsrep_uuid_t&, wsrep_seqno_t&);

bool wsrep_set_SE_checkpoint(const wsrep_uuid_t&, wsrep_seqno_t);

void wsrep_sort_xid_array(XID *array, int len);

#endif 
#endif 
