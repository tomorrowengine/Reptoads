#include "utils/RegisterHandler.h"


void tbsg::RegisterHandler::RegisterAccount(const ptl::string& username,const ptl::string& email,const ptl::string& password)
{
	std::cout << "[RegisterHandler] tries to create account\n";

	const auto& config = tbsg::Config::Get();
	ptl::string url = config.apiHost;

	auto loginResult = apiParser.GetNetworkAccessToken(url, "/v1/login.php", tbsg::WebMethod::POST, ptl::vector<tbsg::postDataPair> { {"username", username}, { "password", password }});
}