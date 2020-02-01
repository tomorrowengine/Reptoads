#include "net/WebRequestPC.h"
#include <iostream>

tbsg::WebRequest::WebRequest()
{

}

bool tbsg::WebRequest::TestWebConnection(const ptl::string& baseWebURL,  int websitePort,  int timeoutTime )
{
	UNUSED(timeoutTime);

	requestor = ptl::make_unique<httplib::SSLClient>(baseWebURL.c_str(), websitePort);
	requestor->set_ca_cert_path("./ca-bundle.crt");
	requestor->enable_server_certificate_verification(true);

	auto check = requestor->Get(baseWebURL.c_str());
	if (check) {
		return true;
	}
	else
	{
		auto result = requestor->get_openssl_verify_result();
		if (result)
		{
			std::cout << "verify error: " << X509_verify_cert_error_string(result) << std::endl;
		}
	}
	return false;
}

tbsg::WebRequestResult tbsg::WebRequest::RequestWebData(const ptl::string& webDomain, const ptl::string& webDirectoryURL, const tbsg::WebMethod method, const ptl::vector<tbsg::postDataPair> arguments)
{
	
	if (!this->TestWebConnection(webDomain))
	{
		std::cout << "[WebRequest] ERROR: Unable to connect to domain: " << webDomain << std::endl;
		return tbsg::WebRequestResult{};
	}
	
	ptl::string argumentString = this->ConvertArgumentsToString(method, arguments);
	auto request = GetWebRequest(webDirectoryURL, argumentString, method);
	if (request)
	{
		if (GetRequestStatus(request) != 0)
		{
			return tbsg::WebRequestResult{ GetRequestStatus(request) , GetRequestBody(request) };
		}
	}
	return tbsg::WebRequestResult{};
}

ptl::shared_ptr<httplib::Response> tbsg::WebRequest::GetWebRequest(const ptl::string& webDirectoryURL, const ptl::string& argumentString, const tbsg::WebMethod& method)
{
	if (requestor)
	{
		ptl::string txtType = "application/json";
		if (!argumentString.empty()) {
			txtType = "application/x-www-form-urlencoded";
		}
		if (method == tbsg::WebMethod::GET)
		{
			ptl::string combined = webDirectoryURL + argumentString;
			return requestor->Get(combined.c_str());
		}
		if (method == tbsg::WebMethod::POST)
		{
			return requestor->Post(webDirectoryURL.c_str(), argumentString, txtType.c_str());

		}
	}
	
	return nullptr;
}

int tbsg::WebRequest::GetRequestStatus(const ptl::shared_ptr<httplib::Response> response)
{
	if (response != nullptr)
	{
		return response->status;
	}
	return 0;
}

ptl::string tbsg::WebRequest::GetRequestBody(const ptl::shared_ptr<httplib::Response> response)
{
	if (response != nullptr)
	{
		return response->body;
	}
	return ptl::string{};
}

ptl::string tbsg::WebRequest::ConvertArgumentsToString(const tbsg::WebMethod& method,const ptl::vector<postDataPair>& arguments)
{
	if (arguments.empty())
	{
		return ptl::string{};
	}
	ptl::string returnString{};
	if (method == tbsg::WebMethod::GET)
	{
		returnString += "?";
	}

	size_t listSize = arguments.size();
	for (size_t i = 0; i < listSize; ++i)
	{
		returnString += arguments[i].key + "=" + arguments[i].value;
		if (i != listSize - 1)
		{
			returnString += "&";
		}
	}
	return returnString;
}
