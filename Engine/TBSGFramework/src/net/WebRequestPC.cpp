#include "net/WebRequestPC.h"
#include <iostream>

tbsg::WebRequest::WebRequest() {}

bool tbsg::WebRequest::TestWebConnection(const ptl::string& baseWebURL, int websitePort, int timeoutTime /*= 0*/)
{
    // Setup the client and SSL requirements.
    requestor = ptl::make_unique<httplib::SSLClient>(baseWebURL.c_str(), websitePort);
    requestor->set_ca_cert_path("./ca-bundle.crt");
    requestor->enable_server_certificate_verification(true);

    // Use the base url to ping to the webserver, returns whether a valid response (200 OK usually) was retrieved.
    auto validResponse = requestor->Get(baseWebURL.c_str());
    if (validResponse)
    {
        return true;
    }

    // If the webhost didn't return anything (valid), report to console.
    auto result = requestor->get_openssl_verify_result();
    if (result)
    {
        std::cout << "verify error: " << X509_verify_cert_error_string(result) << std::endl;
    }
    return false;
}

tbsg::WebRequestResult tbsg::WebRequest::RequestWebData(const ptl::string& webDomain, const ptl::string& webDirectoryURL, const tbsg::WebMethod method, const ptl::vector<tbsg::postDataPair> arguments)
{
    // Before making the webrequest, do a check whether the domain is reachable.
    // TODO: Make this check debug only to speed up the process.
    if (!this->TestWebConnection(webDomain))
    {
        std::cout << "[WebRequest] ERROR: Unable to connect to domain: " << webDomain << std::endl;
        return tbsg::WebRequestResult{};
    }

    // Convert the arguments to a string and perform the request.
    const ptl::string argumentString = this->ConvertArgumentsToString(method, arguments);
    auto webResponse = GetWebRequest(webDirectoryURL, argumentString, method);
    if (webResponse)
    {
        if (GetRequestStatus(webResponse) != 0)
        {
            return tbsg::WebRequestResult{ GetRequestStatus(webResponse) , GetRequestBody(webResponse) };
        }
    }

    // Return empty result struct if valid
    // TODO Add debug info in debug mode to quickly debug.
    return tbsg::WebRequestResult{};
}

ptl::shared_ptr<httplib::Response> tbsg::WebRequest::GetWebRequest(const ptl::string& webDirectoryURL, const ptl::string& argumentString, const tbsg::WebMethod& method)
{
    if (!requestor)
    {
        std::cout << "[WebRequest] ERROR: Unable to perform web request as requestor is nullptr." << std::endl;
        return nullptr;
    }

    // Build the application request
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

    // method not implemented, return nullptr.
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
    return ptl::string{};		//return empty string as invalid marking
}

ptl::string tbsg::WebRequest::ConvertArgumentsToString(const tbsg::WebMethod& method, const ptl::vector<postDataPair>& arguments)
{
    if (arguments.empty())
    {
        return ptl::string{};
    }

    // Build GET request argument string
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
