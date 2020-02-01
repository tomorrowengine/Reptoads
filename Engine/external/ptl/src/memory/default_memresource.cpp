#include "memory/default_memresource.h"
#include "memory/details/mem_options.h"
#include "memory/details/mem_defines.h"
#include "memory/malloc_resource.h"
#include "memory/pool_resource.h"
#include "memory/freelist_resources.h"

#if _MSVC_LANG == 201703L
#define IF_CONSTEXPR if constexpr (mem::enable) {
#define IF_CONSTEXPR_ELSE     } else {
#define END_IF_CONSTEXPR }
#else

#define IF_CONSTEXPR if(mem::enable){
#define IF_CONSTEXPR_ELSE } else {
#define END_IF_CONSTEXPR }

#endif


#define MONOTONIC(SIZE,ALIGMENT) ptl::MemoryResourceOption{ SIZE,ALIGMENT}
#define POOL PoolResourceOption( mem::sizeOfPools[i],mem::sizeOfChunks[i],32 )
#define FREELIST(SIZE,ALIGMENT) ptl::MemoryResourceOption{ SIZE,ALIGMENT}


ptl::DefaultMemoryResource::DefaultMemoryResource() :lmr(MONOTONIC(64 * 1024, alignof(size_t)))
{
    printf("[WARNING] CONSTRUCT DefaultMemoryResource\n");
 
IF_CONSTEXPR
   
    const auto max_size = mem::numberOfPools - 1;
    allocs[mem::numberOfPools] = lmr.allocate_object<FreeListResource>(alignof(PoolResourceOption), FREELIST(2'000'000'000, alignof(size_t)), ptl::nulloc());
    for (auto i = max_size; i >= 0;)
        {
            if (i != max_size) {
                allocs[i] = lmr.allocate_object<PoolResource>(alignof(PoolResourceOption), POOL, allocs[i + 1]);
            }
            else
            {

                allocs[i] = lmr.allocate_object<PoolResource>(alignof(PoolResourceOption), POOL, allocs[mem::numberOfPools]);
            }
            if (i == 0)
                break;
            if (i != 0)
                --i;
        }

END_IF_CONSTEXPR
}


ptl::MemoryResource* ptl::DefaultMemoryResource::get_default_allocator()
{
IF_CONSTEXPR
    
        
        static DefaultMemoryResource instance;
        
        return instance.GetPool(0);
   
IF_CONSTEXPR_ELSE
            
        return ptl::malloc_resource();
END_IF_CONSTEXPR


}

ptl::MonotonicResource* ptl::DefaultMemoryResource::get_default_monotonic()
{
    static ptl::MonotonicResource instance{MONOTONIC(100000, 32)};
    return &instance;
};

#undef POOL
#undef MONOTONIC

