#pragma once
#ifndef UNUSED
    #define UNUSED(variable) (void)variable
#endif
namespace ptl{
        template<typename T> inline T alignToPow2(const T size, const T alignment)
        {
            return (size + alignment - T(1)) & (~(alignment - T(1)));
        }
        
        inline size_t align_offset(size_t alignment, size_t size, void * ptr, size_t space) noexcept
        {	
            size_t offset = static_cast<size_t>(reinterpret_cast<uintptr_t>(ptr) & (alignment - 1));
            if (offset != 0)
            {
                offset = alignment - offset;
            }

            if (space < offset || space - offset < size)
            {
                return 0;
            }
            return offset;
        }

        inline size_t alignForwardAdjustment(const void* address, size_t alignment)
        {
            const size_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));

            if (adjustment == alignment) return 0;

            
            return adjustment;
        }
        inline void* add(void* p, size_t x)
        {
            return (void*)(reinterpret_cast<uintptr_t>(p) + x);
        }

        inline const void* add(const void* p, size_t x)
        {
            return (const void*)(reinterpret_cast<uintptr_t>(p) + x);
        }
		
        template<typename T>
        inline uintptr_t alignForwardAdjustmentWithHeader(const void* address, uintptr_t alignment)
        {
            if (__alignof(T) > alignment)
                alignment = __alignof(T);

           return sizeof(T) + alignForwardAdjustment(add(address, sizeof(T)), alignment);
        }

        template<typename T> inline T alignTo(const T size, const T alignment)
        {
            return ((size + alignment - T(1)) / alignment) * alignment;
        }

        inline const void* subtract(const void* p, size_t x)
        {
            return (const void*)(reinterpret_cast<uintptr_t>(p) - x);
        }

        
        
        template<size_t T> inline
            size_t GetSizeOfN(const size_t count)
        {
            constexpr size_t maxPossible = static_cast<size_t>(-1) / T;
            size_t result = count * T;
            if (maxPossible < count)
            {
                result = static_cast<size_t>(-1);
            }

            return result;
        }

        template<> inline
            size_t GetSizeOfN<1>(const size_t count)
        {
            return count;
        }

        template<class T>
        constexpr const T& MaxValue(const T& _Left, const T& _Right)
        {
            return (_Left < _Right ? _Right : _Left);
        }

		inline size_t AlignSize(size_t size, size_t align)
		{
			return size + (align - size % align);
		}

		template <typename T>
		inline size_t SizeOfContainerData(T container) 
		{
			return container.size() * sizeof(typename T::value_type);
		}

        

}
