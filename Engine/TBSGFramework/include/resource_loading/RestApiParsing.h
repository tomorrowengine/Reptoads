#pragma once

#include "memory/Containers.h"
#include "memory/string.h"
#include "net/WebRequestHelper.h"

namespace tbsg
{

	class RestApiParsing
	{
	public:
		tbsg::WebLoginResult GetNetworkAccessToken(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments);
		ptl::string GetGameDataString(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments);
		bool CheckIfSessionIsValid(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments);
		ptl::string GetUserDecksString(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments);
		bool RequestNewAccount(const ptl::string& username, const ptl::string& password, const ptl::string& email);
	private:
		ptl::string ParseStringResult(tbsg::WebRequestResult& webResult);
	};

}

