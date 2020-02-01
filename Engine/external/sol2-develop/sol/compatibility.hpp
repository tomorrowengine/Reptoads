






















#ifndef SOL_COMPATIBILITY_HPP
#define SOL_COMPATIBILITY_HPP






#include "feature_test.hpp"
#include "compatibility/version.hpp"

#if !defined(SOL_NO_COMPAT) || !(SOL_NO_COMPAT)

#if defined(SOL_USING_CXX_LUA) && SOL_USING_CXX_LUA
#ifndef COMPAT53_LUA_CPP
#define COMPAT53_LUA_CPP 1
#endif 
#endif
#ifndef COMPAT53_INCLUDE_SOURCE
#define COMPAT53_INCLUDE_SOURCE 1
#endif 
#include "compatibility/compat-5.3.h"

#endif 

#endif 
