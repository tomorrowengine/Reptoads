#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include "memory/smart_ptr.h"
#include "memory/Containers.h"
#include "net/WebRequestHelper.h"

namespace tbsg
{
	class WebRequest
	{
	public:
		WebRequest();
		~WebRequest() = default;

		tbsg::WebRequestResult RequestWebData(const ptl::string& webDomain, const ptl::string& webDirectoryURL, const tbsg::WebMethod method, const ptl::vector<tbsg::postDataPair> arguments = {});
	private:
		bool TestWebConnection(const ptl::string& webDomain, int websitePort = 443,  int timeoutTime = 0);
		ptl::unique_ptr<httplib::SSLClient> requestor;
		ptl::shared_ptr<httplib::Response> response;

		ptl::shared_ptr<httplib::Response> GetWebRequest(const ptl::string& webDirectoryURL, const ptl::string& argumentString, const tbsg::WebMethod& method);

		int GetRequestStatus(const ptl::shared_ptr<httplib::Response>);
		ptl::string GetRequestBody(const ptl::shared_ptr<httplib::Response>);
		ptl::string ConvertArgumentsToString(const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments);
	};
}