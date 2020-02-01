

#ifndef RPL_RECORD_H
#define RPL_RECORD_H

#include <rpl_reporting.h>

struct rpl_group_info;
struct TABLE;
typedef struct st_bitmap MY_BITMAP;

#if !defined(MYSQL_CLIENT)
size_t pack_row(TABLE* table, MY_BITMAP const* cols,
                uchar *row_data, const uchar *data);
#endif

#if !defined(MYSQL_CLIENT) && defined(HAVE_REPLICATION)
int unpack_row(rpl_group_info *rgi,
               TABLE *table, uint const colcnt,
               uchar const *const row_data, MY_BITMAP const *cols,
               uchar const **const curr_row_end, ulong *const master_reclength,
               uchar const *const row_end);


int prepare_record(TABLE *const table, const uint skip, const bool check);
int fill_extra_persistent_columns(TABLE *table, int master_cols);
#endif

#endif
