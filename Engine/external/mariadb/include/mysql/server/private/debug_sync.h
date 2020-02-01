#ifndef DEBUG_SYNC_INCLUDED
#define DEBUG_SYNC_INCLUDED





#ifdef USE_PRAGMA_INTERFACE
#pragma interface                      
#endif

class THD;

#if defined(ENABLED_DEBUG_SYNC)


extern MYSQL_PLUGIN_IMPORT uint opt_debug_sync_timeout;


#define DEBUG_SYNC_DEFAULT_WAIT_TIMEOUT 300


extern int  debug_sync_init(void);
extern void debug_sync_end(void);
extern void debug_sync_init_thread(THD *thd);
extern void debug_sync_end_thread(THD *thd);
extern bool debug_sync_set_action(THD *thd, const char *action_str, size_t len);
extern bool debug_sync_update(THD *thd, char *val_str, size_t len);
extern uchar *debug_sync_value_ptr(THD *thd);

#endif 

#endif 
