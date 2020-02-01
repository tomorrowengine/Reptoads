#pragma once
#include <cstddef>
#include <cassert>
#include <forward_list>
#include <iostream>

namespace ptl{
	struct MemoryResourceOption;
	
	class MemoryResource
	{
		MemoryResource* m_upstream{nullptr};
	public:
		MemoryResource() = default;
		MemoryResource(MemoryResource* mem) :m_upstream(mem) {};
		void* allocate(std::size_t n, const void *)
		{
			
			return allocate(n);
		}

		void* allocate(size_t n,const size_t aligment = alignof(max_align_t))
		{

			auto* ptr = DoAllocate(n, aligment);
#ifdef _DEBUG
			if (ptr != nullptr) {
				++m_numberOfalloc;
				return ptr;
			}
#else
			if (ptr != nullptr) return ptr;
#endif
			assert(m_upstream != nullptr);
			return m_upstream->allocate(n, aligment);
			
		}
		template <typename Arg, typename... Args>
		void doPrint(std::ostream& out, Arg&& arg, Args&&... args)
		{
			out << std::forward<Arg>(arg);
			using expander = int[];
			(void)expander {
				0, (void(out << ',' << std::forward<Args>(args)), 0)...
			};
		}
		template<typename T,class... Args >
		T* allocate_object(const size_t aligment , Args&&...args)
		{
			auto ptr = allocate(sizeof(T), aligment);
			if (ptr == nullptr) return nullptr;
			return ::new (const_cast<void *>(static_cast<const volatile void *>(ptr)))
				T{ ::std::forward<Args>(args)... };
		}

		void deallocate(void* ptr, size_t n,size_t aligment = alignof(max_align_t))
		{
			if (ptr == nullptr) return;
			if (!DoDeallocate(ptr, n, aligment)){
				m_upstream->deallocate(ptr, n, aligment);
			}
#ifdef _DEBUG
			else
			{
				++m_numberOfdealloc;
			}
#endif
		}

		void deallocate(void* ptr)
		{
			if (ptr == nullptr) return;
			if (!DoDeallocate(ptr, alignof(max_align_t))) {
				m_upstream->deallocate(ptr, alignof(max_align_t));
			}
#ifdef _DEBUG
			else
			{
				++m_numberOfdealloc;
			}
#endif
		}

		template<typename T>
		void deallocate_object(T* p, size_t aligment = alignof(max_align_t))
		{
			if (p == nullptr) return;
			p->~T();
			deallocate(p, sizeof(T), aligment);
		}

		bool IsEqual(const MemoryResource& rhs) const noexcept
		{
			return DoIsEqual(rhs);
		}
		MemoryResource* GetUpstream()
		{
			return m_upstream;
		}
		virtual ~MemoryResource() = default;
#ifdef _DEBUG

		unsigned GetNumberOfAlloc() const
		{
			return m_numberOfalloc;
		}

		unsigned GetNumberOfDealloc() const
		{
			return m_numberOfdealloc;
		}
#endif
	private:
		virtual void* DoAllocate(size_t size, const size_t aligment) = 0;
		virtual bool DoDeallocate(void*& ptr, size_t size, size_t aligment) = 0;
		virtual bool DoDeallocate(void*& ptr, size_t aligment) = 0;
		virtual bool DoIsEqual(const MemoryResource& rhs) const = 0;

#ifdef _DEBUG
	protected:
		unsigned int m_numberOfalloc{};
		unsigned int m_numberOfdealloc{};
#endif
	};

	inline bool operator==(const MemoryResource& a ,const MemoryResource& b) noexcept
	{
		return (&a == &b) || (&a != nullptr && &b != nullptr && a.IsEqual(b));
	}

	class NullMemoryResource : public MemoryResource
	{
		void* DoAllocate(size_t , const size_t ) override
		{
			assert(false);
			return nullptr;

		};
		bool DoDeallocate(void*& , size_t , size_t) override
		{
			assert(false);
			return false;
		};
		bool DoIsEqual(const MemoryResource& ) const override
		{
			assert(false);
			return false;
		}

		bool DoDeallocate(void*&, size_t) override
		{
			assert(false);
			return false;
		}
	};

	inline MemoryResource* nulloc() noexcept
	{
		static NullMemoryResource instance;
		return &instance;
	}

}
