#ifndef _WIN32
#include "memory/String.h"

bool ptl::operator==(ptl::string& str1, std::string& str2)
{
    auto ret = strcmp(str1.c_str(), str2.c_str());

    if (ret < 0)
    {
        return false;
    }
    else if (ret > 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ptl::operator==(std::string& str1, ptl::string& str2)
{
    auto ret = strcmp(str1.c_str(), str2.c_str());

    if (ret < 0)
    {
        return false;
    }
    else if (ret > 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ptl::operator==(const std::string& str1, const ptl::string& str2)
{
    auto ret = strcmp(str1.c_str(), str2.c_str());

    if (ret < 0)
    {
        return false;
    }
    else if (ret > 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ptl::operator==(const ptl::string& str1, const std::string& str2)
{
    auto ret = strcmp(str1.c_str(), str2.c_str());

    if (ret < 0)
    {
        return false;
    }
    else if (ret > 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}
#endif
