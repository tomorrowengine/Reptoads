




















#ifndef SOL_HPP
#define SOL_HPP

#if defined(UE_BUILD_DEBUG) || defined(UE_BUILD_DEVELOPMENT) || defined(UE_BUILD_TEST) || defined(UE_BUILD_SHIPPING) || defined(UE_SERVER)
#define SOL_INSIDE_UNREAL 1
#endif 

#if defined(SOL_INSIDE_UNREAL) && SOL_INSIDE_UNREAL
#ifdef check
#define SOL_INSIDE_UNREAL_REMOVED_CHECK 1
#undef check
#endif 
#endif 

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#if __GNUC__ > 6
#pragma GCC diagnostic ignored "-Wnoexcept-type"
#endif
#elif defined(__clang__)

#elif defined _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4324 ) 
#pragma warning( disable : 4503 ) 
#pragma warning( disable : 4702 ) 
#pragma warning( disable: 4127 ) 
#pragma warning( disable: 4505 ) 
#endif 

#include "sol/forward.hpp"
#include "sol/state.hpp"
#include "sol/object.hpp"
#include "sol/function.hpp"
#include "sol/protected_function.hpp"
#include "sol/state.hpp"
#include "sol/coroutine.hpp"
#include "sol/variadic_args.hpp"
#include "sol/variadic_results.hpp"

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined _MSC_VER
#pragma warning( push )
#endif 

#if defined(SOL_INSIDE_UNREAL) && SOL_INSIDE_UNREAL
#if defined(SOL_INSIDE_UNREAL_REMOVED_CHECK) && SOL_INSIDE_UNREAL_REMOVED_CHECK
#if defined(DO_CHECK) && DO_CHECK
#define check(expr) { if(UNLIKELY(!(expr))) { FDebug::LogAssertFailedMessage( #expr, __FILE__, __LINE__ ); _DebugBreakAndPromptForRemote(); FDebug::AssertFailed( #expr, __FILE__, __LINE__ ); CA_ASSUME(false); } }
#else
#define check(expr) { CA_ASSUME(expr); }
#endif
#endif 
#endif 

#endif 
