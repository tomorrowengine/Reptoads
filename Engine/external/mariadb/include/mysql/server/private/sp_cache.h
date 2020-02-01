


#ifndef _SP_CACHE_H_
#define _SP_CACHE_H_

#ifdef USE_PRAGMA_INTERFACE
#pragma interface			
#endif



class sp_head;
class sp_cache;
class Database_qualified_name;



void sp_cache_init();
void sp_cache_end();
void sp_cache_clear(sp_cache **cp);
void sp_cache_insert(sp_cache **cp, sp_head *sp);
sp_head *sp_cache_lookup(sp_cache **cp, const Database_qualified_name *name);
void sp_cache_invalidate();
void sp_cache_flush_obsolete(sp_cache **cp, sp_head **sp);
ulong sp_cache_version();
void sp_cache_enforce_limit(sp_cache *cp, ulong upper_limit_for_elements);

#endif 
