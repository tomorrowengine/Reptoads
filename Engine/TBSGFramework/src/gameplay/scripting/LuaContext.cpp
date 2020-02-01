#include "gameplay/scripting/LuaContext.h"
#include "lua.h"

tbsg::scripting::LuaContext::~LuaContext()
{
	if(m_IsInitialized)
	{
		Deallocate();
	}
}

void tbsg::scripting::LuaContext::Initialize()
{
    m_luaState = luaL_newstate();
    luaL_openlibs(m_luaState);
    m_IsInitialized = true;
}

void tbsg::scripting::LuaContext::Deallocate()
{
	if(m_luaState != nullptr){
		lua_close(m_luaState);
		m_luaState = nullptr;
	}
}

sol::state& tbsg::scripting::LuaContext::GetState()
{
    sol::state lua;
    m_lua = std::move(lua);
    return m_lua;
}

lua_State& tbsg::scripting::LuaContext::GetLuaState()
{
    m_luaState = luaL_newstate();
    return *m_luaState;
}

void tbsg::scripting::LuaContext::OpenLib(sol::lib&)
{
}

sol::function tbsg::scripting::LuaContext::GetUnsafeFunction(const char* a_definition)
{
    sol::state_view lua = m_luaState;
    auto func = lua[a_definition];
    if (func.valid() && func.get_type() == sol::type::function) { return func; }
    sol::function badfunc;
    return badfunc;
}

void tbsg::scripting::LuaContext::GetFunction(sol::protected_function& func, const char* a_definition)
{
    sol::state_view lua = m_luaState;
    func = std::move(lua[a_definition]);
}

void tbsg::scripting::LuaContext::GetFunction(sol::state& a_lua, sol::protected_function&func, const char *a_definition)
{
    func = std::move(a_lua[a_definition]);
}

sol::protected_function tbsg::scripting::LuaContext::GetFunction(const char* a_definition)
{
    sol::state_view lua = m_luaState;
    sol::protected_function func = lua[a_definition];
    return func;
}

sol::protected_function tbsg::scripting::LuaContext::GetFunction(sol::state& a_lua, const char *a_definition)
{
    sol::protected_function func = a_lua[a_definition];
    return func;
}

sol::unsafe_function_result tbsg::scripting::LuaContext::RunUnsafeScript(const ptl::string& a_Luacode)
{
    sol::state_view lua = m_luaState;
    auto scriptResult = lua.script(a_Luacode.c_str());
    return scriptResult;
}

sol::protected_function_result tbsg::scripting::LuaContext::RunScript(const ptl::string& a_Luacode)
{
    sol::state_view lua = m_luaState;
    auto scriptResult = lua.script(a_Luacode.c_str(), [](lua_State*, sol::protected_function_result pfr) {
        return pfr;
    });
    return scriptResult;
}

sol::protected_function_result tbsg::scripting::LuaContext::RunScript(sol::state& a_lua, const ptl::string &a_Luacode)
{
    auto scriptResult = a_lua.script(a_Luacode.c_str(), [](lua_State*, sol::protected_function_result pfr) {
        return pfr;
    });
    return scriptResult;
}

sol::protected_function_result tbsg::scripting::LuaContext::LoadAndRunScriot(const ptl::string& a_path)
{
    sol::state_view lua = m_luaState;
    auto scriptResult = lua.safe_script_file(a_path.c_str(), [](lua_State*, sol::protected_function_result pfr) {
        return pfr;
    });
    return scriptResult;
}

bool tbsg::scripting::LuaContext::IsValid(sol::unsafe_function_result& a_UnsafeFunctionResult)
{
    return (a_UnsafeFunctionResult.valid() && a_UnsafeFunctionResult.status() == sol::call_status::ok);
}

bool tbsg::scripting::LuaContext::IsValid(sol::safe_function_result& a_FunctionResult)
{
    return (a_FunctionResult.valid() && a_FunctionResult.status() == sol::call_status::ok);
}

ptl::string tbsg::scripting::LuaContext::GetErrorMessage(sol::safe_function_result& a_FunctionResult)
{
    sol::error err = a_FunctionResult;
    return err.what();
}

sol::call_status tbsg::scripting::LuaContext::GetCallStatus(sol::unsafe_function_result& a_FunctionResult)
{
    return a_FunctionResult.status();
}

sol::call_status tbsg::scripting::LuaContext::GetCallStatus(sol::safe_function_result& a_FunctionResult)
{
    return a_FunctionResult.status();
}
