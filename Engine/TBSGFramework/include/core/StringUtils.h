#pragma once
#include <memory/string.h>

static bool endsWith(const ptl::string& str, const ptl ::string& suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool startsWith(const ptl::string& str, const ptl::string& prefix)
{
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}