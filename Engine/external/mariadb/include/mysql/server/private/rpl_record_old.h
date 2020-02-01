

#ifndef RPL_RECORD_OLD_H
#define RPL_RECORD_OLD_H

#include "log_event.h"                          

#ifndef MYSQL_CLIENT
size_t pack_row_old(TABLE *table, MY_BITMAP const* cols,
                    uchar *row_data, const uchar *record);

#ifdef HAVE_REPLICATION
int unpack_row_old(rpl_group_info *rgi,
                   TABLE *table, uint const colcnt, uchar *record,
                   uchar const *row, uchar const *row_buffer_end,
                   MY_BITMAP const *cols,
                   uchar const **row_end, ulong *master_reclength,
                   MY_BITMAP* const rw_set,
                   Log_event_type const event_type);
#endif
#endif
#endif
