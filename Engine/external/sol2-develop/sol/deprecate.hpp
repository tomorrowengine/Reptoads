






















#ifndef SOL_DEPRECATE_HPP
#define SOL_DEPRECATE_HPP

#ifndef SOL_DEPRECATED
#ifdef _MSC_VER
#define SOL_DEPRECATED __declspec(deprecated)
#elif __GNUC__
#define SOL_DEPRECATED __attribute__((deprecated))
#else
#define SOL_DEPRECATED [[deprecated]]
#endif 
#endif 

namespace sol {
namespace detail {
	template <typename T>
	struct SOL_DEPRECATED deprecate_type {
		using type = T;
	};
}
} 

#endif 
