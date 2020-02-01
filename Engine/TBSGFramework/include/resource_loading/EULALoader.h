#pragma once
#include "memory/String.h"

namespace tbsg
{
	ptl::string LoadEULAFromFile(const ptl::string& filename);
	ptl::string LoadEULAFromFile(const char* filename);
}