#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include "memory/smart_ptr.h"
#include "memory/Containers.h"
#include "net/WebRequestHelper.h"

namespace tbsg
{

    // WebRequest class (Used to be pure virtual, moving towards excluding files instead)
    // This class will do most of the HTTP requests towards the database or rest api.
    class WebRequest
    {
    public:
        WebRequest();
        ~WebRequest() = default;

        // Main way to request data is using this exposed function which returns a struct that helps determine the result and info such status.
        tbsg::WebRequestResult RequestWebData(const ptl::string& webDomain, const ptl::string& webDirectoryURL, const tbsg::WebMethod method, const ptl::vector<tbsg::postDataPair> arguments = {});
    private:
        // Local pointers to the httplib lib classes in order to do web requests without exposing this.
        ptl::unique_ptr<httplib::SSLClient> requestor;
        ptl::shared_ptr<httplib::Response> response;

        // Related to performing web requests themselves.
        ptl::string GetRequestBody(const ptl::shared_ptr<httplib::Response>);
        ptl::string ConvertArgumentsToString(const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments);

        // Perform HTTP web requests
        ptl::shared_ptr<httplib::Response> GetWebRequest(const ptl::string& webDirectoryURL, const ptl::string& argumentString, const tbsg::WebMethod& method);
        bool TestWebConnection(const ptl::string& webDomain, int websitePort = 443, int timeoutTime = 0);
        int GetRequestStatus(const ptl::shared_ptr<httplib::Response>);
    };
}