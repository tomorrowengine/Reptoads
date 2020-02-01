#pragma once
#include "memory/string.h"
#include "memory/Containers.h"

namespace tbsg
{
    struct Config;

    class ParsedArguments
    {
    public:
        void AddResultStr(ptl::string argument, ptl::string value);
        bool HasResultStr(ptl::string argument);
        ptl::string& operator[] (ptl::string&& argument);
    private:
        ptl::unordered_map<ptl::string, ptl::string> strArguments{};
    };

    
    Config& ArgumentParsing(Config& config, int argc,char** argv, ParsedArguments& arguments);
}
