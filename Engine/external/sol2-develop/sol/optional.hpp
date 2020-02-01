






















#ifndef SOL_OPTIONAL_HPP
#define SOL_OPTIONAL_HPP

#include "compatibility.hpp"
#include "in_place.hpp"
#if defined(SOL_USE_BOOST) && SOL_USE_BOOST
#include <boost/optional.hpp>
#else
#include "optional_implementation.hpp"
#endif 

namespace sol {

#if defined(SOL_USE_BOOST) && SOL_USE_BOOST
	template <typename T>
	using optional = boost::optional<T>;
	using nullopt_t = boost::none_t;
	const nullopt_t nullopt = boost::none;
#endif 

	namespace meta {
		template <typename T>
		struct is_optional : std::false_type {};
		template <typename T>
		struct is_optional<optional<T>> : std::true_type {};
	} 
} 

#endif 
