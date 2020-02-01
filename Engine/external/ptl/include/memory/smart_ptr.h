#pragma once
#include "memory/allocator.h"
#include "default_memresource.h"
#include <memory>

namespace ptl
{

    
    template<class T>
    struct default_delete
    {	
        constexpr default_delete() noexcept = default;

        void operator()(T * ptr) const noexcept
        {	
            static_assert(0 < sizeof(T),
                "can't delete an incomplete type");
            DefaultMemoryResource::get_default_allocator()->deallocate_object(ptr, alignof(T));
        }
    };

    
    template<class T>
    struct default_delete<T[]>
    {
        constexpr default_delete() noexcept = default;

        template<class U,
            std::enable_if_t<std::is_convertible<U(*)[], T(*)[]>::value, int> = 0>
            void operator()(U * ptr) const noexcept
        {	
            static_assert(0 < sizeof(U),
                "can't delete an incomplete type");
            DefaultMemoryResource::get_default_allocator()->deallocate_object(ptr, alignof(T));
        }
    };
    template<typename T>
    using unique_ptr = std::unique_ptr<T, ptl::default_delete<T>>;

    template<typename T>
    using shared_ptr = std::shared_ptr<T>;

    template< class T,class... Args >
        ptl::shared_ptr<T> make_shared(Args&&... args){
            return std::allocate_shared <T, ptl::Allocator<T>,Args...> (ptl::Allocator<T>{}, ::std::forward<Args>(args)...);
    };

     template< class T, class... Args >
     ptl::unique_ptr<T> make_unique(Args&&... args) {
         return std::unique_ptr<T,ptl::default_delete<T>>(DefaultMemoryResource::get_default_allocator()->allocate_object<T>(alignof(T),std::forward<Args>(args)...));
     };


	 template<typename T>
	 using weak_ptr = std::weak_ptr<T>;

	 template<typename T>
	 using enable_shared_from_this = std::enable_shared_from_this<T>;


	
	template<typename T, typename U>
	ptl::unique_ptr<T> static_pointer_cast(ptl::unique_ptr<U>&& from)
	{
		U* fromPtr = from.release();
		T* toPtr = static_cast<T*>(fromPtr);
		return ptl::unique_ptr<T>{toPtr};
	}
}
