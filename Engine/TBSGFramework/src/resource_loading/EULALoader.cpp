#include "resource_loading/EULALoader.h"

#include <fstream>

ptl::string tbsg::LoadEULAFromFile(const ptl::string& filename)
{
	return LoadEULAFromFile(filename.c_str());
}

ptl::string tbsg::LoadEULAFromFile(const char* filename)
{
	std::ifstream ifs(filename);
	if(ifs.is_open()){
	return { (std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()) };
	}else
	{
		return { "Could not load EULA File!" };
	}
}
