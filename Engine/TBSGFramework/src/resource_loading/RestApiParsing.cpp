#include "resource_loading/RestApiParsing.h"

#if defined(PLATFORM_WINDOWS)
#include "Net/WebRequestPC.h"
#include "httplib/httplib.h"
#endif

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "core/Config.h"


tbsg::WebLoginResult tbsg::RestApiParsing::GetNetworkAccessToken(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments)
{
#if defined(PLATFORM_WINDOWS)
	ptl::string webDirectoryString = subDomain;
#endif

	tbsg::Config config = tbsg::Config::Get();
	tbsg::WebRequest requestor;

	tbsg::WebRequestResult webResult = requestor.RequestWebData(config.apiHost, webDirectoryString, method, arguments);
	if (webResult.statusCode == 200)
	{
		
		rapidjson::Document jsonResult;
		jsonResult.Parse(webResult.content.c_str());
		if (!jsonResult.HasMember("valid"))
		{
			std::cout << "[RestApiParsing] Web request didn't contain the required 'valid' json parameter" << std::endl;
			return tbsg::WebLoginResult{};
		}

		rapidjson::Value& validVal = jsonResult["valid"];

		if (validVal.GetBool())
		{
			rapidjson::Value& accessToken = jsonResult["token"];
			
			tbsg::WebLoginResult resultPackage;
			resultPackage.accessToken = accessToken.GetString();

			if (jsonResult.HasMember("profileId"))
			{
				rapidjson::Value& profileValue = jsonResult["profileId"];
				resultPackage.profileId = profileValue.GetInt();
			}

			return resultPackage;
		}
		else
		{
			
			rapidjson::Value& errorVal = jsonResult["error"];
			std::cout << "[RestApiParsing] API response error: " << errorVal.GetString() << std::endl;
		}
	}
	else
	{
		std::cout << "[RestApiParsing] Error, could not setup connection with provided web URL." << std::endl;
	}
	return tbsg::WebLoginResult{};
}

ptl::string tbsg::RestApiParsing::GetGameDataString(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments)
{
#if defined(PLATFORM_WINDOWS)
	ptl::string webDirectoryString = subDomain;
#endif

	tbsg::Config config = tbsg::Config::Get();
	tbsg::WebRequest requestor;
	tbsg::WebRequestResult webResult = requestor.RequestWebData(config.apiHost, webDirectoryString, method, arguments);
	return this->ParseStringResult(webResult);
}

bool tbsg::RestApiParsing::CheckIfSessionIsValid(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments)
{
#if defined(PLATFORM_WINDOWS)
	ptl::string webDirectoryString = subDomain;
#endif

	tbsg::Config config = tbsg::Config::Get();
	tbsg::WebRequest requestor;
	tbsg::WebRequestResult webResult = requestor.RequestWebData(config.apiHost, webDirectoryString, method, arguments);
	if (webResult.statusCode == 200)
	{
		
		rapidjson::Document jsonResult;
		jsonResult.Parse(webResult.content.c_str());
		
		if (!jsonResult.HasMember("valid"))
		{
			std::cout << "[RestApiParsing] Web request didn't contain the required 'valid' json parameter" << std::endl;
			return false;
		}
		rapidjson::Value& validVal = jsonResult["valid"];
		bool valid = validVal.GetBool();
		if (valid)
		{
			std::cout << "[RestApiParsing] Session token is still valid." << std::endl;
			return true;
		}
		else
		{
			rapidjson::Value& errorVal = jsonResult["error"];
			std::cout << "[RestApiParsing] Current session no longer valid, reason: " << errorVal.GetString() << std::endl;
		}
	}
	else
	{
		std::cout << "[RestApiParsing] Error, could not setup connection with provided web URL." << std::endl;
	}
	return false;
}

ptl::string tbsg::RestApiParsing::GetUserDecksString(const ptl::string& webDomain, const ptl::string& subDomain, const tbsg::WebMethod& method, const ptl::vector<tbsg::postDataPair>& arguments)
{
#if defined(PLATFORM_WINDOWS)
	ptl::string webDirectoryString = subDomain;
#endif

	tbsg::Config config = tbsg::Config::Get();
	tbsg::WebRequest requestor;
	tbsg::WebRequestResult webResult = requestor.RequestWebData(config.apiHost, webDirectoryString, method, arguments);
	return this->ParseStringResult(webResult);
}

bool tbsg::RestApiParsing::RequestNewAccount(const ptl::string& username, const ptl::string& password,
	const ptl::string& email)
{
	tbsg::WebRequest requestor;
	tbsg::Config& config = tbsg::Config::Get();
#if defined(PLATFORM_WINDOWS)
	const ptl::string webDirectoryString = "/v1/register.php";
#endif

	tbsg::WebRequestResult webResult = requestor.RequestWebData(config.apiHost, webDirectoryString, tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair>{ {"username", username}, { "password", password }, { "email", email }});
	rapidjson::Document jsonResult;
	jsonResult.Parse(webResult.content.c_str());
	if (!jsonResult.HasMember("valid"))
	{
		std::cout << "[RestApiParsing] Web request was not succesfull" << std::endl;
		return false;
	}
	rapidjson::Value& validVal = jsonResult["valid"];
	const bool valid = validVal.GetBool();

	if (jsonResult.HasMember("error"))
	{
		rapidjson::Value& errorVal = jsonResult["error"];
		std::cout << "[RestApiParsing] API response error: " << errorVal.GetString() << std::endl;
		return false;
	}

	if (!valid)
	{
		std::cout << "[RestApiParsing] API: Not valid, reason: " << std::endl;
		return false;
	}else
	{
		return true;
	}
}

ptl::string tbsg::RestApiParsing::ParseStringResult(tbsg::WebRequestResult& webResult)
{
	if (webResult.statusCode == 200)
	{
		rapidjson::Document jsonResult;
		jsonResult.Parse(webResult.content.c_str());

		if (!jsonResult.HasMember("valid"))
		{
			std::cout << "[RestApiParsing] Web request didn't contain the required 'valid' json parameter" << std::endl;
			return ptl::string{};
		}


		rapidjson::Value& validVal = jsonResult["valid"];
		bool valid = validVal.GetBool();
		ptl::string errorMessage{};

		if (jsonResult.HasMember("error"))
		{
			rapidjson::Value& errorVal = jsonResult["error"];
			std::cout << "[RestApiParsing] API response error: " << errorVal.GetString() << std::endl;
			return ptl::string{};
		}

		if (!valid)
		{
			std::cout << "[RestApiParsing] API: Not valid, reason: " << std::endl;
			return ptl::string{};
		}

		return ptl::string(webResult.content);
	}
	else
	{
		std::cout << "[RestApiParsing] Error, could not setup connection with provided web URL." << std::endl;
	}
	return ptl::string{};
}
