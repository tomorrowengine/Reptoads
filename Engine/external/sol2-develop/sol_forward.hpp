

























#ifndef SOL_SINGLE_INCLUDE_FORWARD_HPP
#define SOL_SINGLE_INCLUDE_FORWARD_HPP





#if (defined(__cplusplus) && __cplusplus == 201703L) || (defined(_MSC_VER) && _MSC_VER > 1900 && ((defined(_HAS_CXX17) && _HAS_CXX17 == 1) || (defined(_MSVC_LANG) && (_MSVC_LANG > 201402L))))
#ifndef SOL_CXX17_FEATURES
#define SOL_CXX17_FEATURES 1
#endif 
#endif 

#if defined(SOL_CXX17_FEATURES) && SOL_CXX17_FEATURES
#if defined(__cpp_noexcept_function_type) || ((defined(_MSC_VER) && _MSC_VER > 1911) && (defined(_MSVC_LANG) && ((_MSVC_LANG >= 201403L))))
#ifndef SOL_NOEXCEPT_FUNCTION_TYPE
#define SOL_NOEXCEPT_FUNCTION_TYPE 1
#endif 
#endif 
#if defined(__clang__) && defined(__APPLE__)
#if defined(__has_include)
#if __has_include(<variant>)
#define SOL_STD_VARIANT 1
#endif 
#endif 
#else
#define SOL_STD_VARIANT 1
#endif 
#endif 



#ifdef _MSC_VER
	#if defined(_DEBUG) && !defined(NDEBUG)

	#ifndef SOL_IN_DEBUG_DETECTED
	#define SOL_IN_DEBUG_DETECTED 1
	#endif

	#endif 

	#ifndef _CPPUNWIND
	#ifndef SOL_NO_EXCEPTIONS
	#define SOL_NO_EXCEPTIONS 1
	#endif
	#endif 

	#ifndef _CPPRTTI
	#ifndef SOL_NO_RTTI
	#define SOL_NO_RTTI 1
	#endif
	#endif 
#elif defined(__GNUC__) || defined(__clang__)

	#if !defined(NDEBUG) && !defined(__OPTIMIZE__)

	#ifndef SOL_IN_DEBUG_DETECTED
	#define SOL_IN_DEBUG_DETECTED 1
	#endif

	#endif 

	#ifndef __EXCEPTIONS
	#ifndef SOL_NO_EXCEPTIONS
	#define SOL_NO_EXCEPTIONS 1
	#endif
	#endif 

	#ifndef __GXX_RTTI
	#ifndef SOL_NO_RTII
	#define SOL_NO_RTTI 1
	#endif
	#endif 

#endif 

#if defined(SOL_CHECK_ARGUMENTS) && SOL_CHECK_ARGUMENTS

	
	
	#if !defined(SOL_SAFE_GETTER)
	#define SOL_SAFE_GETTER 1
	#endif

	
	
	
	
	#if !defined(SOL_SAFE_USERTYPE)
	#define SOL_SAFE_USERTYPE 1
	#endif

	
	
	
	#if !defined(SOL_SAFE_REFERENCES)
	#define SOL_SAFE_REFERENCES 1
	#endif

	
	
	#if !defined(SOL_SAFE_FUNCTION)
	#define SOL_SAFE_FUNCTION 1
	#endif

	
	
	
	
	
	#if !defined(SOL_SAFE_FUNCTION_CALLS)
	#define SOL_SAFE_FUNCTION_CALLS 1
	#endif

	
	
	
	#if !defined(SOL_SAFE_PROXIES)
	#define SOL_SAFE_PROXIES 1
	#endif

	
	
	
	#if !defined(SOL_SAFE_NUMERICS)
	#define SOL_SAFE_NUMERICS 1
	#endif

	
	
	
	
	#if !defined(SOL_NO_CHECK_NUMBER_PRECISION)
	
	#define SOL_NO_CHECK_NUMBER_PRECISION 0
	#endif

#endif 

#if defined(SOL_IN_DEBUG_DETECTED) && SOL_IN_DEBUG_DETECTED

	#if !defined(SOL_SAFE_REFERENCES)
	
	#define SOL_SAFE_REFERENCES 1
	#endif

	
	
	
	
	#if !defined(SOL_SAFE_USERTYPE)
	#define SOL_SAFE_USERTYPE 1
	#endif

	#if !defined(SOL_SAFE_FUNCTION_CALLS)
	
	#define SOL_SAFE_FUNCTION_CALLS 1
	#endif

	
	
	#if !defined(SOL_PRINT_ERRORS)
	#define SOL_PRINT_ERRORS 1
	#endif

#endif 

#if !defined(SOL_PRINT_ERRORS)
#define SOL_PRINT_ERRORS 0
#endif

#if !defined(SOL_DEFAULT_PASS_ON_ERROR)
#define SOL_DEFAULT_PASS_ON_ERROR 0
#endif

#if !defined(SOL_ENABLE_INTEROP)
#define SOL_ENABLE_INTEROP 0
#endif

#if defined(__MAC_OS_X_VERSION_MAX_ALLOWED) || defined(__OBJC__) || defined(nil)
#if !defined(SOL_NO_NIL)
#define SOL_NO_NIL 1
#endif
#endif 

#if defined(SOL_USE_BOOST) && SOL_USE_BOOST
#ifndef SOL_UNORDERED_MAP_COMPATIBLE_HASH
#define SOL_UNORDERED_MAP_COMPATIBLE_HASH 1
#endif 
#endif 

#ifndef SOL_STACK_STRING_OPTIMIZATION_SIZE
#define SOL_STACK_STRING_OPTIMIZATION_SIZE 1024
#endif 









namespace sol {

	template <bool b>
	class basic_reference;
	using reference = basic_reference<false>;
	using main_reference = basic_reference<true>;
	class stack_reference;

	struct proxy_base_tag;
	template <typename Super>
	struct proxy_base;
	template <typename Table, typename Key>
	struct proxy;

	template <typename T>
	class usertype;
	template <typename T>
	class simple_usertype;
	template <bool, typename T>
	class basic_table_core;
	template <bool b>
	using table_core = basic_table_core<b, reference>;
	template <bool b>
	using main_table_core = basic_table_core<b, main_reference>;
	template <bool b>
	using stack_table_core = basic_table_core<b, stack_reference>;
	template <typename T>
	using basic_table = basic_table_core<false, T>;
	typedef table_core<false> table;
	typedef table_core<true> global_table;
	typedef main_table_core<false> main_table;
	typedef main_table_core<true> main_global_table;
	typedef stack_table_core<false> stack_table;
	typedef stack_table_core<true> stack_global_table;
	template <typename base_t>
	struct basic_environment;
	using environment = basic_environment<reference>;
	using main_environment = basic_environment<main_reference>;
	using stack_environment = basic_environment<stack_reference>;
	template <typename T, bool>
	class basic_function;
	template <typename T, bool, typename H>
	class basic_protected_function;
	using unsafe_function = basic_function<reference, false>;
	using safe_function = basic_protected_function<reference, false, reference>;
	using main_unsafe_function = basic_function<main_reference, false>;
	using main_safe_function = basic_protected_function<main_reference, false, reference>;
	using stack_unsafe_function = basic_function<stack_reference, false>;
	using stack_safe_function = basic_protected_function<stack_reference, false, reference>;
	using stack_aligned_unsafe_function = basic_function<stack_reference, true>;
	using stack_aligned_safe_function = basic_protected_function<stack_reference, true, reference>;
	using protected_function = safe_function;
	using main_protected_function = main_safe_function;
	using stack_protected_function = stack_safe_function;
	using stack_aligned_protected_function = stack_aligned_safe_function;
#if defined(SOL_SAFE_FUNCTION) && SOL_SAFE_FUNCTION
	using function = protected_function;
	using main_function = main_protected_function;
	using stack_function = stack_protected_function;
#else
	using function = unsafe_function;
	using main_function = main_unsafe_function;
	using stack_function = stack_unsafe_function;
#endif
	using stack_aligned_function = stack_aligned_unsafe_function;
	using stack_aligned_stack_handler_function = basic_protected_function<stack_reference, true, stack_reference>;

	struct unsafe_function_result;
	struct protected_function_result;
	using safe_function_result = protected_function_result;
#if defined(SOL_SAFE_FUNCTION) && SOL_SAFE_FUNCTION
	using function_result = safe_function_result;
#else
	using function_result = unsafe_function_result;
#endif

	template <typename base_t>
	class basic_object;
	template <typename base_t>
	class basic_userdata;
	template <typename base_t>
	class basic_lightuserdata;
	template <typename base_t>
	class basic_coroutine;
	template <typename base_t>
	class basic_thread;

	using object = basic_object<reference>;
	using userdata = basic_userdata<reference>;
	using lightuserdata = basic_lightuserdata<reference>;
	using thread = basic_thread<reference>;
	using coroutine = basic_coroutine<reference>;
	using main_object = basic_object<main_reference>;
	using main_userdata = basic_userdata<main_reference>;
	using main_lightuserdata = basic_lightuserdata<main_reference>;
	using main_coroutine = basic_coroutine<main_reference>;
	using stack_object = basic_object<stack_reference>;
	using stack_userdata = basic_userdata<stack_reference>;
	using stack_lightuserdata = basic_lightuserdata<stack_reference>;
	using stack_thread = basic_thread<stack_reference>;
	using stack_coroutine = basic_coroutine<stack_reference>;

	struct stack_proxy_base;
	struct stack_proxy;
	struct variadic_args;
	struct variadic_results;
	struct stack_count;
	struct this_state;
	struct this_main_state;
	struct this_environment;

	template <typename T>
	struct as_table_t;
	template <typename T>
	struct as_container_t;
	template <typename T>
	struct nested;
	template <typename T>
	struct light;
	template <typename T>
	struct user;
	template <typename T>
	struct as_args_t;
	template <typename T>
	struct protect_t;
	template <typename F, typename... Filters>
	struct filter_wrapper;

	template <typename T>
	struct usertype_traits;
	template <typename T>
	struct unique_usertype_traits;
} 



#endif 
