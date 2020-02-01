






















#ifndef SOL_CONFIG_HPP
#define SOL_CONFIG_HPP

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

#endif 
