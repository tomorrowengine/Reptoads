






















#ifndef SOL_VERSION_HPP
#define SOL_VERSION_HPP

#include "../feature_test.hpp"

#if defined(SOL_USING_CXX_LUA) && SOL_USING_CXX_LUA
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#if defined(SOL_USING_CXX_LUAJIT) && SOL_USING_CXX_LUAJIT
#include <luajit.h>
#endif 
#if (!defined(SOL_EXCEPTIONS_SAFE_PROPAGATION) || !(SOL_EXCEPTIONS_SAFE_PROPAGATION)) && (!defined(SOL_EXCEPTIONS_ALWAYS_UNSAFE) || !(SOL_EXCEPTIONS_ALWAYS_UNSAFE))
#define SOL_EXCEPTIONS_SAFE_PROPAGATION 1
#endif 
#else
#include <lua.hpp>
#endif 

#ifdef LUAJIT_VERSION
#ifndef SOL_LUAJIT
#define SOL_LUAJIT 1
#ifndef SOL_LUAJIT_VERSION
#define SOL_LUAJIT_VERSION LUAJIT_VERSION_NUM
#endif 
#endif 
#endif 

#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 502
#define SOL_LUA_VERSION LUA_VERSION_NUM
#elif defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501
#define SOL_LUA_VERSION LUA_VERSION_NUM
#elif !defined(LUA_VERSION_NUM) || !(LUA_VERSION_NUM)

#define SOL_LUA_VERSION 500
#else

#define SOL_LUA_VERSION 502
#endif 

#endif 
