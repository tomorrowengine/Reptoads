#pragma once
#include <fstream>
#include <streambuf>
#include "Payloads.h"

namespace tbsg
{
    namespace scripting
    {
        class LuaContextHelper
        {
        public:
            void LoadScript(const char* path, tbsg::Script* script)
            {
                std::ifstream stream(path);
                const ptl::string string_result((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
                if (!string_result.empty())
                {
                    script->code = string_result;
                }
            }

            void LoadScript(const ptl::string& a_path, tbsg::Script& a_script)
            {
                std::ifstream stream(a_path.c_str());
                const ptl::string string_result((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
                if (!string_result.empty())
                {
                    a_script.code = string_result;
                }
            }

            void LoadScript(const char* path, ptl::string& a_script)
            {
                std::ifstream stream(path);
                const ptl::string string_result((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
                if (!string_result.empty())
                {
                    a_script = string_result;
                }
            }
        };
    };
}