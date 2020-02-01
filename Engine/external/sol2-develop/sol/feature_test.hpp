






















#ifndef SOL_FEATURE_TEST_HPP
#define SOL_FEATURE_TEST_HPP

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

#include <sol/config.hpp>
#include "config_setup.hpp"

#endif 
