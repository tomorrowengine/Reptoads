#include "memory/String.h"
#include "sol.hpp"
#include "gameplay/scripting/LuaContext.h"
#include "Payloads.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

ptl::string ReturnErrorCode(sol::protected_function_result& a_FuncResult)
{
    switch(a_FuncResult.status())
    {
    case sol::call_status::file:
        return "FILE";
    case sol::call_status::syntax:
        return "SYNTAX";
    case sol::call_status::yielded:
        return "YIELDED";
    case sol::call_status::memory:
        return "MEMORY";
    case sol::call_status::runtime:
        return "RUNTIME";
    };
    return "UNKNOWN CALLSTATUS";
}


bool Validate(tbsg::scripting::LuaContext& LuaContext,const ptl::string& Path)
{
    sol::protected_function_result scriptCode = LuaContext.LoadAndRunScriot(Path);
    if (!LuaContext.IsValid(scriptCode)) {
        std::cout << "[LuaValidator][SOL2] CODE: " << ReturnErrorCode(scriptCode) << "\nFile: "<< Path << "\nMESSAGE: " << LuaContext.GetErrorMessage(scriptCode) << std::endl;
        return false;
    }
    else
    {
        sol::protected_function func;
        LuaContext.GetFunction(func, "OnCardPlay");
        sol::protected_function_result result = func.call();
        if (!func.valid() && LuaContext.GetCallStatus(result) != sol::call_status::ok)
        {
            std::cout << "[LuaValidator][SOL2] ERROR: " << LuaContext.GetErrorMessage(result) << std::endl;
            return false;
        }
    }
    return true;
}

void Process(fs::directory_entry& fs_itr,const std::function<void(fs::directory_entry&)>& func)
{
    if (fs_itr.is_directory())
    {
        auto DirItr = fs::directory_iterator{ fs_itr.path() };
        for (auto file_itr : DirItr)
        {
            Process(file_itr, func);
        }
    }
    else
    {
        return func(fs_itr);
    }
};


int main(int argc, char* const argv[])
{

    if (argc < 1) { std::cout << "[LuaValidator] No arguments given, possible commands: basic, folder and onPlay" << std::endl; return 0; }
    
    tbsg::scripting::LuaContext LuaContext;
    LuaContext.Initialize();

    ptl::string ValidationMode = argv[1];
    if (argc < 2) {
        std::cout << "[LuaValidator] Missing argument 'path', example usage: basic path-to-file" << std::endl;
        return 0;
    }

    ptl::string Path{ argv[2] };
    std::cout << "[LuaValidator] Mode: " << ValidationMode << " Path: " << Path << "\n";
    auto IsLuaFile = [](const auto& ext) ->bool
    {
        return ext == ".lua";
    };

    if (ValidationMode == "basic" || ValidationMode == "Basic")
    {
            Validate(LuaContext, Path);
    }
    else if (ValidationMode == "folder")
    {
        if (fs::exists(Path))
        {
            auto DirItr = fs::directory_iterator{ Path };
            int FileCounter = 0;
            int OKFiles = 0;
            int FailFiles = 0;
            auto Check = [&IsLuaFile,&LuaContext,&FileCounter,&FailFiles,&OKFiles](auto& file_itr)
            {
                if (IsLuaFile(file_itr.path().extension().generic_string())) {
                    if(Validate(LuaContext, file_itr.path().generic_string()))
                    {
                        std::cout << "[LuaValidator] File: " << file_itr.path() << "\nStatus: OK\n";
                        ++OKFiles;
                    }else
                    {
                        ++FailFiles;
                        std::cout << "[LuaValidator] File: " << file_itr.path() << "\nStatus: FAIL\n";
                    }
                    FileCounter++;
                }
            };
            for(auto file_itr : DirItr)
            {
                Process(file_itr,Check);

            }
            std::cout << "[LuaValidator] processed Lua files: " << FileCounter << " OK: "<< OKFiles << " Failed: "<< FailFiles << "\n";
        }else
        {
            std::cout << "[LuaValidator] [ERROR] Specified path does not exist" << std::endl;
        }
    }
    else if (ValidationMode == "onPlay")
    {

    }
    LuaContext.Deallocate();

    return 0;
}