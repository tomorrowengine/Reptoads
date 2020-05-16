#pragma once
#include <sol.hpp>
#include "memory/String.h"
#include "memory/Containers.h"

namespace tbsg
{
    namespace scripting
    {
        class LuaContext
        {
        public:
            ~LuaContext();


            // Control the Lua context and state object
            void Initialize();
            void OpenLib(sol::lib& lib);
            void Deallocate();

            // Get the state objects
            sol::state& GetState();
            lua_State& GetLuaState();

            //// Lua Function Getters directly or through sol

            // Get a function by passing the state and/or function object.
            void GetFunction(sol::protected_function& func, const char*);
            void GetFunction(sol::state& state, sol::protected_function& func, const char*);

            // Unsafe or protected function getters
            sol::function GetUnsafeFunction(const char*);
            sol::protected_function GetFunction(const char*);
            sol::protected_function GetFunction(sol::state& state, const char*);


            // Templated function setters and getters to allow for any function to be created and be usable 
            template<typename T, typename ...Args>
            sol::function& GetUnsafeFunction(const char* a_definition, std::function<T(Args...)> function);

            template<typename T, typename ...Args>
            T& GetFunctionResult(sol::protected_function a_SolFunction, Args...args);

            template<typename T, typename ...Args>
            void SetFunction(const char* a_Declaration, std::function<T(Args...)> a_Function);

            template<typename T>
            void SetType(const char* a_Declaration);

            //// Lua Script Execution helper functions
            sol::unsafe_function_result RunUnsafeScript(const ptl::string&);

            sol::protected_function_result RunScript(const ptl::string&);
            sol::protected_function_result RunScript(sol::state& state, const ptl::string&);
            sol::protected_function_result LoadAndRunScriot(const ptl::string&);

            // Status getting functions for each matching type
            bool IsValid(sol::unsafe_function_result&);
            bool IsValid(sol::safe_function_result&);

            ptl::string GetErrorMessage(sol::safe_function_result&);
            sol::call_status GetCallStatus(sol::unsafe_function_result&);
            sol::call_status GetCallStatus(sol::safe_function_result&);

            // Check whether the state is initialised
            bool GetIsInitialized() { return m_IsInitialized; }
        private:
            sol::state m_lua = nullptr;
            lua_State* m_luaState = nullptr;
            ptl::vector<char> m_openedLibs;
            bool m_IsInitialized = false;
        };

        // Get an unsafe function directly by providing the definition and arguments expected.
        template <typename T, typename ... Args>
        sol::function& LuaContext::GetUnsafeFunction(const char* a_definition, std::function<T(Args...)> function)
        {
            sol::state_view lua = m_luaState;
            sol::object function_obj = lua[a_definition];
            if (function_obj.get_type() == sol::type::function && function_obj.is<std::function<T(Args...)>>())
            {
                sol::function func = lua[a_definition];
                return func;
            }
            sol::function func;
            return func;
        }

        // Get the result of the function executed
        template <typename T, typename ... Args>
        T& LuaContext::GetFunctionResult(sol::protected_function a_SolFunction, Args...args)
        {
            return a_SolFunction(args...);
        }

        // Create a new function definition in the Lua environment
        template <typename T, typename...Args>
        void LuaContext::SetFunction(const char* a_Declaration, std::function<T(Args...)> a_Function)
        {
            sol::state_view lua = m_luaState;
            lua.set_function(a_Declaration, a_Function);
        }

        // Set a new Type definition in the Lua environment to support our own C++ types to be used there.
        template<typename T>
        void LuaContext::SetType(const char* a_Declaration)
        {
            sol::state& lua = GetState();
            lua.new_usertype<T>(a_Declaration, "new", sol::no_constructor);
        }
    }
}
