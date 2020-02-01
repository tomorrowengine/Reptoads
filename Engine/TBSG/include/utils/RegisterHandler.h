#pragma once

#include "core/Config.h"
#include "resource_loading/RestApiParsing.h"

class GameDatabase;

namespace  tbsg
{
	class RegisterHandler{
	public:
		void RegisterAccount(const ptl::string& username,const ptl::string& email,const ptl::string& password);
	private:
		tbsg::RestApiParsing apiParser{};
	};

}
