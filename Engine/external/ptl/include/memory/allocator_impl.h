#pragma once
#include <vector>
#include "../memory/memory_resource.h"
#include "../memory/default_memresource.h"
#include "../memory/details/mem_utils.h"

namespace ptl{
template<typename T>
class Allocator
{
    MemoryResource* m_mresource;
    size_t defaultAligment{ static_cast<size_t>(32UL) };
    size_t newAlignOf{ MaxValue(alignof(T),defaultAligment) };
    size_t bigAllocationAligment{ 32 };

public:
    using value_type = T;
    using pointer = T * ;
    using const_pointer = const T*;
    using reference = T & ;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    
    using propagate_on_container_copy_assignment = std::true_type; 
    using propagate_on_container_move_assignment = std::true_type; 
    using propagate_on_container_swap = std::true_type; 
    
    template< class U > struct rebind { typedef Allocator<U> other; };


    Allocator():m_mresource(ptl::nulloc())
    {
        m_mresource = ptl::DefaultMemoryResource::get_default_allocator();
		assert(m_mresource != nullptr);
    };
    Allocator(Allocator&& other) = default;
    Allocator& operator=(Allocator&& other) = default;
    Allocator(const Allocator& other) = default;
    Allocator& operator=(const Allocator& other) = default;

    Allocator(MemoryResource* mr) : m_mresource(mr) {};

    template< class U >
    Allocator(const Allocator<U>& other) noexcept
    {
        m_mresource = other.GetResource();
    }

    MemoryResource* GetResource() const
    {
        return m_mresource;
    }

    T *  address(reference x) const { return &x; }
    const T *  address(const T& x) const { return &x; }

    size_type max_size() const { return static_cast<size_t>(-1) / sizeof(T); }

    template< class U, class... Args >
    void construct(U* p, Args&&... args)
    {
        ::new (const_cast<void *>(static_cast<const volatile void *>(p)))
            U(::std::forward<Args>(args)...);

        
    }

    template< class U >
    void destroy(U* p)
    {
        p->~U();
    }


    T *  allocate(size_type n, const void *)
    {
        
        return allocate(n);
    }

    T *  allocate(size_type bytes)
    {
        if (bytes == 0)
        {
            return nullptr;
        }
        size_t passedAlign = newAlignOf;
        if(bytes >= 4096)
        {
            passedAlign = MaxValue(newAlignOf, bigAllocationAligment);
        }
        
        if(m_mresource == nullptr)
            return static_cast<T *>(std::malloc(bytes));
        else
            return static_cast<T *>(m_mresource->allocate(GetSizeOfN<sizeof(value_type)>(bytes), passedAlign));
    }

    template<class... Args >
    T* allocate_object(size_type n,Args&&...args)
    {
        auto ptr = static_cast<T *>(allocate(n));
        if (ptr == nullptr) return nullptr;
        return construct(ptr, ::std::forward<Args>(args)...);
    }

    void deallocate(T *  p, std::size_t n)
    {
        size_t passedAlign = newAlignOf;
        if (n >= 4096)
        {
            passedAlign = MaxValue(newAlignOf, bigAllocationAligment);
        }
        if(m_mresource != nullptr){
            m_mresource->deallocate(static_cast<void*>(p), sizeof(value_type) * n, passedAlign);
        }else{
            std::free(p);
        }
    }

    Allocator<T> select_on_container_copy_construction() const
    {
        return { m_mresource };
    }
};

    template<typename A,typename B>
    bool operator==(const Allocator<A>& a, const Allocator<B>& b) noexcept
    {
		if(a.GetResource() == nullptr || b.GetResource() == nullptr)
			return a.GetResource() == b.GetResource();

		return *a.GetResource() == *b.GetResource();
    }

    template< class T1, class T2 >
    bool operator!=(const Allocator<T1>& , const Allocator<T2>& )
    {
        return false;
    }
};
