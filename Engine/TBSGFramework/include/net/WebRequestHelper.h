#pragma once
#include "memory/string.h"

namespace tbsg
{
	enum class WebMethod
	{
		GET = 0,
		POST = 1
	};

	struct WebRequestResult
	{
		int statusCode = 0;
		ptl::string content = "";
	};

	struct WebLoginResult
	{
		ptl::string accessToken{};
		unsigned int profileId{};
	};

	struct postDataPair
	{
		ptl::string key;
		ptl::string value;
	};
}
