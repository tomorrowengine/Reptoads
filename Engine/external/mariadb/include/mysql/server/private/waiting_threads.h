

#ifndef _waiting_threads_h
#define _waiting_threads_h

#include <my_sys.h>

#include <lf.h>

C_MODE_START

typedef struct st_wt_resource_id WT_RESOURCE_ID;
typedef struct st_wt_resource WT_RESOURCE;

typedef struct st_wt_resource_type {
  my_bool (*compare)(const void *a, const void *b);
  const void *(*make_key)(const WT_RESOURCE_ID *id, uint *len); 
} WT_RESOURCE_TYPE;

struct st_wt_resource_id {
  ulonglong value;
  const WT_RESOURCE_TYPE *type;
};

#define sizeof_WT_RESOURCE_ID (sizeof(ulonglong)+sizeof(void*))

#define WT_WAIT_STATS  24
#define WT_CYCLE_STATS 32
extern ulonglong wt_wait_table[WT_WAIT_STATS];
extern uint32    wt_wait_stats[WT_WAIT_STATS+1];
extern uint32    wt_cycle_stats[2][WT_CYCLE_STATS+1];
extern uint32    wt_success_stats;

typedef struct st_wt_thd {
  
  DYNAMIC_ARRAY   my_resources;
  
  WT_RESOURCE    *waiting_for;
  LF_PINS        *pins;

  
  const ulong *timeout_short;
  const ulong *deadlock_search_depth_short;
  const ulong *timeout_long;
  const ulong *deadlock_search_depth_long;

  
  ulong volatile weight;
  
  my_bool killed;
#ifndef DBUG_OFF
  const char     *name;
#endif
} WT_THD;

#define WT_TIMEOUT              ETIMEDOUT
#define WT_OK                   0
#define WT_DEADLOCK             -1
#define WT_DEPTH_EXCEEDED       -2
#define WT_FREE_TO_GO           -3

void wt_init(void);
void wt_end(void);
void wt_thd_lazy_init(WT_THD *, const ulong *, const ulong *, const ulong *, const ulong *);
void wt_thd_destroy(WT_THD *);
int wt_thd_will_wait_for(WT_THD *, WT_THD *, const WT_RESOURCE_ID *);
int wt_thd_cond_timedwait(WT_THD *, mysql_mutex_t *);
void wt_thd_release(WT_THD *, const WT_RESOURCE_ID *);
#define wt_thd_release_all(THD) wt_thd_release((THD), 0)
my_bool wt_resource_id_memcmp(const void *, const void *);

C_MODE_END

#endif
